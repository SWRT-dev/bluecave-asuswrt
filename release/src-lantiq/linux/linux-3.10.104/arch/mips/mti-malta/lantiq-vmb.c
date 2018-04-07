#include <linux/init.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/cpu.h>
#include <linux/interrupt.h>
#include <asm/cacheflush.h>
#include <asm/smp-ops.h>
#include <asm/traps.h>
#include <asm/fw/fw.h>
#include <asm/gcmpregs.h>
#include <linux/of_platform.h>
#include <linux/of_fdt.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched.h>
#include <linux/bootmem.h>
#include <asm/ltq_vmb.h>

#undef LINUX_SHM_DDR

/* Number of cores*/
VMB_core_t core_t[MAX_CORE];
static DEFINE_SPINLOCK(vmb_lock);


static struct proc_dir_entry *vmb_proc, *vmb_proc_dir ;
extern void amon_cpu_start(int, unsigned long, unsigned long,
                    unsigned long, unsigned long);

void *VMB_get_msg_addr (int cpu, int direction);

static irqreturn_t mpe_vmb_ipi1_hdlr(int irq, void *ptr) {
        int i,j;
        VMB_core_t *ct = core_t;
	int cpu = -1, cid =0 , vid=0;
	VMB_vpe_t *vpet_t;
	FW_vmb_msg_t *fw_t ;

	/* CPU number can also be found using ptr and passing the cpu to it */
	/* Get the CPU number from the name as we know this handler is coupled with MPE */

	spin_lock(&vmb_lock);
	for (i=0; i < MAX_CORE; i++) {
                VMB_vpe_t *vpet = ct[i].vpe_t;
           for (j=0; j < MAX_VPE; j++) {
		if (!strncmp(vpet[j].name, "MPEFW",strlen("MPEFW"))) {
			cpu = get_cpu_id(i, j);
			cid = i;
			vid = j;
			goto loop_out;
		}
	   }
	}
	spin_unlock(&vmb_lock);

	if (cpu == -1) {
		printk ("Interrupt Handler for IRQ %d is not for FW_NAME = %s .. \n", irq, "VOICEFW");
		return 0;
	}

loop_out:
	vpet_t = &core_t[cid].vpe_t[vid];
	fw_t = (FW_vmb_msg_t *)VMB_get_msg_addr(cpu, 1);

	/* May lead to deadlock in case of FW_RESET as spinlock for that VPE is taken and then wait_event_called thus 
	   commenting the locks if this doesnt work then need to use  IBL_IN_ handler to wakeup this MPE handler*/

	spin_lock(&vpet_t->vpe_lock);

	/* W.r.t FW copy the DDR section to Internal DB */
        memcpy(&vpet_t->fw_vmb, fw_t, sizeof(FW_vmb_msg_t));
        smp_rmb();

	if(vpet_t->fw_vmb.status == FW_RESET) {
		vmb_cpu_free(cpu);
		
		printk("[%s]:[%d], cpu = %d \n", __FUNCTION__, __LINE__, cpu);
		if (vpet_t->vmb_callback_fn && vpet_t->vmb_callback_fn != NULL)
			vpet_t->vmb_callback_fn(vpet_t->fw_vmb.status);

		/* Be cautious : TEST Throughly, VPE will go for a reset so it will be in deactivated STATE*/
		vpet_t->bl_status = IBL_INACTIVE;

		goto res_ext;	

	} else if(vpet_t->fw_vmb.status == IBL_IN_WAIT) {
		vmb_cpu_free(cpu);
		vpet_t->bl_status = IBL_ACTIVE;
		printk("[%s]:[%d], cpu = %d \n", __FUNCTION__, __LINE__, cpu);
	}

       /* Clear DDR section */
       //  memset(fw_t, 0, sizeof(FW_vmb_msg_t));

        /* Set the wakeup_vpe and wakeup the waitqueue */
	vpet_t->v_wq.wakeup_vpe = 1;
        wake_up_interruptible(&vpet_t->v_wq.vmb_wq);

res_ext:
        /* unlock */
        spin_unlock(&vpet_t->vpe_lock);

	return 0;	
}

static irqreturn_t vce_vmb_ipi1_hdlr(int irq, void *ptr) {
        int i,j;
        VMB_core_t *ct = core_t;
	int cpu = -1, cid =0 , vid=0;
	VMB_vpe_t *vpet_t;
	FW_vmb_msg_t *fw_t ;

	/* CPU number can also be found using ptr and passing the cpu to it */
	/* Get the CPU number from the name as we know this handler is coupled with MPE */

	spin_lock(&vmb_lock);
	for (i=0; i < MAX_CORE; i++) {
                VMB_vpe_t *vpet = ct[i].vpe_t;
           for (j=0; j < MAX_VPE; j++) {
		if (!strncmp(vpet[j].name, "VOICEFW",strlen("VOICEFW"))) {
			cpu = get_cpu_id(i, j);
			cid = i;
			vid = j;
			goto loop_out;
		}
	   }
	}
	spin_unlock(&vmb_lock);

	if (cpu == -1) {
		printk ("Interrupt Handler for IRQ %d is not for FW_NAME = %s .. \n", irq, "VOICEFW");
		return 0;
	}

loop_out:
	vpet_t = &core_t[cid].vpe_t[vid];
	fw_t = (FW_vmb_msg_t *)VMB_get_msg_addr(cpu, 1);

	/* May lead to deadlock in case of FW_RESET as spinlock for that VPE is taken and then wait_event_called thus 
	   commenting the locks if this doesnt work then need to use  IBL_IN_ handler to wakeup this MPE handler*/

	spin_lock(&vpet_t->vpe_lock);

	/* W.r.t FW copy the DDR section to Internal DB */
        memcpy(&vpet_t->fw_vmb, fw_t, sizeof(FW_vmb_msg_t));
        smp_rmb();

	if(vpet_t->fw_vmb.status == FW_RESET) {
		vmb_cpu_free(cpu);
		
		printk("[%s]:[%d], cpu = %d \n", __FUNCTION__, __LINE__, cpu);
		if (vpet_t->vmb_callback_fn && vpet_t->vmb_callback_fn != NULL)
			vpet_t->vmb_callback_fn(vpet_t->fw_vmb.status);

		/* Be cautious : TEST Throughly, VPE will go for a reset so it will be in deactivated STATE*/
		vpet_t->bl_status = IBL_INACTIVE;

		goto res_ext;	

	} else if(vpet_t->fw_vmb.status == IBL_IN_WAIT) {
		vmb_cpu_free(cpu);
		vpet_t->bl_status = IBL_ACTIVE;

		printk("[%s]:[%d], cpu = %d \n", __FUNCTION__, __LINE__, cpu);
	}

       /* Clear DDR section */
       //  memset(fw_t, 0, sizeof(FW_vmb_msg_t));

        /* Set the wakeup_vpe and wakeup the waitqueue */
	vpet_t->v_wq.wakeup_vpe = 1;
        wake_up_interruptible(&vpet_t->v_wq.vmb_wq);

res_ext:
        /* unlock */
        spin_unlock(&vpet_t->vpe_lock);

	return 0;	
}


static irqreturn_t bl_vmb_ipi1_hdlr(int irq, void *ptr) {

	/* Orginating CPU unknown !!! */

	return 0;	
}

/* Update a dummy vmb_msg_t->status in DB as this is for Linux SMP which doesn't update DDR in Linux --> VMB direction*/
static int linux_cpu_ipi_update(unsigned long cpu, unsigned long action) {
	unsigned long flags;
	int c_id = which_core(cpu);
	int v_id = vpe_in_core(cpu);
	VMB_vpe_t *vpet_t = &core_t[c_id].vpe_t[v_id];

#ifdef LINUX_SHM_DDR
	FW_vmb_msg_t *fw_t = (FW_vmb_msg_t *)VMB_get_msg_addr(cpu, 1);
	spin_lock_irqsave(&vpet_t->vpe_lock, flags);	
	
	/* W.r.t FW copy the DDR section to Internal DB */
	memcpy(&vpet_t->fw_vmb, fw_t, sizeof(FW_vmb_msg_t));
	smp_rmb();

	/* Clear DDR section */
	memset(fw_t, 0, sizeof(FW_vmb_msg_t));
	
	/* Set the wakeup_vpe and wakeup the waitqueue */
	vpet_t->v_wq.wakeup_vpe = 1;
	wake_up_interruptible(&vpet_t->v_wq.vmb_wq);

	/* unlock */
	spin_unlock_irqrestore(&vpet_t->vpe_lock, flags);
#else
	spin_lock_irqsave(&vpet_t->vpe_lock, flags);	

	vpet_t->fw_vmb.status= (uint32_t)FW_VMB_ACK;
	vpet_t->fw_vmb.priv_info = 0;

	/* Set the wakeup_vpe and wakeup the waitqueue */
	vpet_t->v_wq.wakeup_vpe = 1;
	wake_up_interruptible(&vpet_t->v_wq.vmb_wq);

	/* unlock */
	spin_unlock_irqrestore(&vpet_t->vpe_lock, flags);
#endif
	return 0;
}

static int __cpuinit vmb_cpu_active(struct notifier_block *nfb,
                                      unsigned long action, void *hcpu)
{
        switch (action) {
        case CPU_STARTING:
                printk ("[%s]:[%d] action = %ld , cpu = %ld\n", __FUNCTION__, __LINE__, action, (long)hcpu);
		/* Come here on LINUX secondary is up . Get the DDR and update DB on the cpu_status, boot_flag and ACK and NACK and 
		   wakeup the waitqueue in vmb_cpu_start() */
		linux_cpu_ipi_update((unsigned long)hcpu, action);
                return NOTIFY_OK;
        default:
                return NOTIFY_DONE;
        }

	return 0;
}

/* API to get the memory in DDR for strutures VMB_fw_msg_t/FW_vmb_msg_t */
void *VMB_get_msg_addr (int cpu, int direction) {
#ifdef CONFIG_EVA
	void *msg_t =  (void *)(CPU_LAUNCH);
#else
	void *msg_t =  (void *)CKSEG0ADDR(CPU_LAUNCH);
#endif

	/* VMB --> FW : VMB_fw_msg_t structure */
	if (direction == 0)
		msg_t = msg_t + (vmb_msg_size * cpu) ;
	else
		msg_t = msg_t + (vmb_msg_size * cpu) + sizeof(VMB_fw_msg_t);

	return (msg_t);
}

/* Lock --> Access and Copy DDR --> check for status and update Internal DB */
static void vmb_check_IBL_fw_msg(void) {
	int i,j;
	VMB_core_t *ct = core_t;
	FW_vmb_msg_t *fw_msg_t;

       for (i=0; i < MAX_CORE; i++) {
		VMB_vpe_t *vpet = ct[i].vpe_t;
		int cpu;
           for (j=0; j < MAX_VPE; j++) {
			if ((i == 0) && (j == 0))
				continue;

                	spin_lock(&vpet[j].vpe_lock);
			cpu = get_cpu_id(i, j);
                	printk ("[%s]:[%d] vpet[j].bl_status = %d , cpu = %d\n", __FUNCTION__, __LINE__, vpet[j].bl_status, cpu);
			fw_msg_t = (FW_vmb_msg_t *)VMB_get_msg_addr(cpu, 1);
			memcpy(&vpet[j].fw_vmb, fw_msg_t, sizeof(FW_vmb_msg_t));
			if (vpet[j].fw_vmb.status == IBL_IN_WAIT)
				vpet[j].bl_status = IBL_ACTIVE;
			else
				vpet[j].bl_status = IBL_INACTIVE;
                	spin_unlock(&vpet[j].vpe_lock);
		}
	}
}

static int update_DB_from_DT(VMB_vpe_t *vt) {
	struct device_node *np;
	char str1[16], *name;
	int ret;

	memset(str1, '\0', sizeof(str1));
	sprintf(str1,"%s%d","/cpus/cpu@",vt->cpu_id);

	printk("[%s]:[%d], str1 = %s , cpuid = %d\n", __FUNCTION__, __LINE__, str1, vt->cpu_id);
	np = of_find_node_by_path(str1);
	if (!np)
                return -ENODEV;

	ret = of_property_read_string_index(np, "default-OS", 0, (const char **)&name);
      	if (ret < 0 && ret != -EINVAL) {
		printk ("ERROR : Property could be read from DT \n");
                return ret;
        }

	strncpy(vt->name, name, sizeof(vt->name));
	printk("[%s]:[%d], name = %s \n", __FUNCTION__, __LINE__, vt->name);
	vt->cpu_status |= CPU_BOOTUP_DT;

	return 0;
}

static void initialise_vmb_DB(void) {
	VMB_core_t *coret = core_t;
	int i,j;

	coret[0].active |= CORE_ACTIVE;

	for (i=0; i < MAX_CORE; i++) {
		VMB_tc_t *tct = coret[i].tc_t;
		VMB_vpe_t *vpet = coret[i].vpe_t;
		for (j=0; j < MAX_VPE; j++) {
			vpet[j].core_id = (((i * 2) + j)/2);
			spin_lock_init(&vpet[j].vpe_lock);
			printk ("MAX_VPE j = %d \n", j);
			printk (" j = %d , vmb_wq = %p \n", j, &vpet[j].v_wq.vmb_wq);
			init_waitqueue_head(&vpet[j].v_wq.vmb_wq);
			vpet[j].v_wq.wakeup_vpe = 0;
			vpet[j].cpu_id = ((i * 2) + j);
			if ((i == 0) && (j == 0)) { 
				/* Core0/VPE0 always active*/
				vpet[j].bl_status = IBL_ACTIVE;
				vpet[j].cpu_status = CPU_ACTIVE;
			} else {
				vpet[j].bl_status = IBL_INACTIVE;
				vpet[j].cpu_status = CPU_INACTIVE;
			}
			tct[j].vpe_id = j;
			/* Get the information from DT for the FW names  also update the bootflag for cpu */
			update_DB_from_DT(&vpet[j]);
		}

		for (j=0; j < MAX_TC; j++) {
			if (j >= 2) {
				tct[j].vpe_id = MAX_VPE;
				tct[j].tc_status = TC_INACTIVE;
			} else {
				tct[j].vpe_id = j;
				tct[j].tc_status = TC_ACTIVE;
			}	
		}
	}
}


static void initialise_vmb_IRQhdlr(void) {
	int err = 0;

	err = request_irq(MPEFW_VMB_IPI1, mpe_vmb_ipi1_hdlr, IRQF_DISABLED, "mpe_vmb_ipi1", NULL);
	if (err)
		printk ("request_irq for IRQ MPEFW_VMB_IPI1 = %d failed !!! \n", MPEFW_VMB_IPI1);

	err = request_irq(VCEFW_VMB_IPI1, vce_vmb_ipi1_hdlr, IRQF_DISABLED, "vce_vmb_ipi1", NULL);
	if (err)
		printk ("request_irq for IRQ VCEFW_VMB_IPI1 = %d failed !!! \n", VCEFW_VMB_IPI1);

	err = request_irq(BLW_VMB_IPI1, bl_vmb_ipi1_hdlr, IRQF_DISABLED, "bl_vmb_ipi1", NULL);
	if (err)
		printk ("request_irq for IRQ BLW_VMB_IPI1 = %d failed !!! \n", BLW_VMB_IPI1);
}

/* Dump the tree */
static int dump_vmb_tree(struct seq_file *s, void *v) {
	VMB_core_t *coret = core_t;
	int i,j;

        for (i=0; i < MAX_CORE; i++){
                //VMB_tc_t *tct = coret[i].tc_t;
                VMB_vpe_t *vpet = coret[i].vpe_t;

		for (j=0; j < MAX_VPE; j++) {
		spin_lock(&vpet[j].vpe_lock);
			printk ("CORE ID %d \n", i);
			printk ("\t - VPE ID %d \n", j);
			printk ("\t \t - OS Name = %s \n", vpet[j].name);
			printk ("\t \t - InterAptiv-BL status = %s \n", (vpet[j].bl_status == IBL_ACTIVE ? "ACTIVE" : "INACTIVE"));
			printk ("\t \t - CPU status = %s \n", ((vpet[j].cpu_status & CPU_ACTIVE) == CPU_ACTIVE ? "ACTIVE" : "INACTIVE")); 
			printk ("\t \t - Core ID = %d \n", vpet[j].core_id);
			printk ("\t \t - CPU ID = %d \n", vpet[j].cpu_id);
			printk ("\t \t - FW_VMB_MSG Structure \n");
			printk ("\t \t \t - status = 0X%x\n", vpet[j].fw_vmb.status);
			printk ("\t \t \t - priv_info = 0X%x\n", vpet[j].fw_vmb.priv_info);
				
		spin_unlock(&vpet[j].vpe_lock);
		}
	}

	return 0;
}

static int vmb_proc_open(struct inode *inode, struct file *file)
{
 return single_open(file, dump_vmb_tree , NULL);
}

static const struct file_operations vmb_proc_fops = {
 .open           = vmb_proc_open,
 .read           = seq_read,
 .llseek         = seq_lseek,
 .release        = single_release,
};

/* Initialise VMB structures and IRQ */
int8_t vmb_init(void) 
{
	memset(core_t, 0 , sizeof(VMB_core_t));

	printk ("size of core = %d, MAXCORE = %d , MAXCPU = %d, MAXTCS = %d MAX_VPE = %d\n", sizeof(core_t), MAX_CORE, MAX_CPU, MAX_TC, MAX_VPE);

	/* CPU notifier for Linux SMP bootup indication notify (CPU_STARTING) from start_secondary */
	cpu_notifier(vmb_cpu_active, CPU_PRI_SCHED_ACTIVE);

	/* Initialise a New core before vmbDB init as it will get time for dumping IBL_INWAIT in DDR 
           which can be picked by vmb_check_IBL() */


	/* Basic initialisation of the DB */
	initialise_vmb_DB();

	/* register IRQ handlers for VMB_FW_IRQx */
	initialise_vmb_IRQhdlr();
	
	/* Initialise the /proc/vm/debug to dump the Internal DB */
	vmb_proc_dir = proc_mkdir("vmb", NULL);
	vmb_proc = proc_create("status", 0644, vmb_proc_dir, &vmb_proc_fops);

	/* API to update th IBL status if the IBL_IN_WAIT IPI is missed during initialisation. 
	   Mainly a fail-safe Linux running VPE updates as there is no IPI mentioned to handle IBL_INWAIT for Linux */
	vmb_check_IBL_fw_msg();

	spin_lock_init(&vmb_lock);

	return 0;
}

/* Allocate Free CPU from the pool */
int8_t vmb_cpu_alloc(int8_t cpu, char* fw_name) {
	int ret = -VMB_ERROR;

/*CHECK : May be for alloc we need to have a global lock to avoid a scenario where 2 FWs call at the same time race condi 
and getting same cpu number */

	if (cpu == MAX_CPU) {
        	VMB_core_t *coret = core_t;
        	int i,j;

        	for (i=0; i < MAX_CORE; i++) {
                	VMB_vpe_t *vpet = coret[i].vpe_t;
                	for (j=0; j < MAX_VPE; j++) {
				if ((vpet[j].cpu_status & CPU_BOOTUP_DT) == CPU_BOOTUP_DT) {
					if (strncmp(vpet[j].name, fw_name, sizeof(vpet[j].name)) != 0) {
						ret=-VMB_EAVAIL;
						continue;
					}
				}

				if ((vpet[j].bl_status == IBL_ACTIVE) && ((vpet[j].cpu_status & CPU_INACTIVE) == CPU_INACTIVE)) {
					ret = get_cpu_id(i, j);
					vpet[j].cpu_status &= ~CPU_INACTIVE;
					vpet[j].cpu_status |= CPU_ACTIVE;
					printk ("[%s]:[%d] CPU vpet[j].cpu_status = %x \n", __FUNCTION__, __LINE__, vpet[j].cpu_status);
#ifdef CONFIG_LTQ_DYN_CPU_ALLOC
					if ((vpet[j].cpu_status & CPU_BOOTUP_DT) == CPU_BOOTUP_DT)
						vpet[j].cpu_status &= ~CPU_BOOTUP_DT;
					else
						strncpy(vpet[j].name, fw_name, sizeof(vpet[j].name));
#endif
					goto fin_alloc;
				} else 
					ret = -VMB_EBUSY;
			}
		}
	} else {
        	int c_id= which_core(cpu);
	        int v_id = vpe_in_core(cpu);
        	VMB_vpe_t *vpet = &core_t[c_id].vpe_t[v_id];
		
		if ((vpet->cpu_status & CPU_BOOTUP_DT) == CPU_BOOTUP_DT) {
			if (strncmp(vpet->name, fw_name, sizeof(vpet->name)) != 0) {
				printk("WARNING : As per DT Bootup Scenario %s shouldn't run on CPU %d \n", fw_name, cpu);
				printk(" Please retry with MAX_CPUS !!! \n");
				ret=-VMB_EAVAIL;
				goto fin_alloc;
			}
		}
		
		if ((vpet->bl_status == IBL_ACTIVE) && ((vpet->cpu_status & CPU_INACTIVE) == CPU_INACTIVE)) {
			ret = cpu;
			vpet->cpu_status &= ~CPU_INACTIVE;
			vpet->cpu_status |= CPU_ACTIVE;
			printk ("[%s]:[%d] CPU vpet.cpu_status = %x \n", __FUNCTION__, __LINE__, vpet->cpu_status);
#ifdef CONFIG_LTQ_DYN_CPU_ALLOC
			if ((vpet->cpu_status & CPU_BOOTUP_DT) == CPU_BOOTUP_DT)
				vpet->cpu_status &= ~CPU_BOOTUP_DT;
			else
				strncpy(vpet->name, fw_name, sizeof(vpet->name));
#endif
		}
		else 
			ret = -VMB_EBUSY;
	}

fin_alloc: 
	return ret;
}

void vmb_register_callback (uint8_t cpu, vmb_callback_func func) {
	int c_id= which_core(cpu);
	int v_id = vpe_in_core(cpu);
	VMB_vpe_t *vpet = &core_t[c_id].vpe_t[v_id];

	vpet->vmb_callback_fn = func;
}

/* Mark the VPE as free (INACTIVE) */
int8_t vmb_cpu_free(int8_t cpu) {
	int c_id= which_core(cpu);
	int v_id = vpe_in_core(cpu);
	VMB_vpe_t *vpet = &core_t[c_id].vpe_t[v_id];

	if (vpet->bl_status != IBL_ACTIVE)
		printk("WARNING : IBL is not active for CPU = %d !!! \n", cpu);

	vpet->cpu_status &= ~CPU_ACTIVE;
	vpet->cpu_status |= CPU_INACTIVE;

	return VMB_SUCCESS;
}

int8_t vmb_cpu_force_stop(int8_t cpu) {
	int c_id, v_id;
	VMB_vpe_t *vpet;
	int ret, ret1;

	c_id = which_core(cpu);
	v_id = vpe_in_core(cpu);
	vpet = &core_t[c_id].vpe_t[v_id];

	vpet->bl_status &= ~IBL_ACTIVE;
	vpet->bl_status |= IBL_INACTIVE;

	/* Generate a NMI for that VPE*/
	
	/* TODO ????????????*/

	/* Wait for IBL_IN_WAIT or timeout */
	ret1 = wait_event_interruptible_timeout(vpet->v_wq.vmb_wq, 
					(vpet->v_wq.wakeup_vpe == 1), QUEUE_TIMEOUT);
	if (ret1 <= 0 ) {
		printk ("[%s]:[%d] wait_event timeout occured for CPU = %d starting FW = %s \n", __FUNCTION__, __LINE__, cpu, vpet->name);
		printk ("Consider reseting the CPU using vmb_cpu_force_stop .... \n");
		vpet->v_wq.wakeup_vpe = 0;

		ret = -VMB_ETIMEOUT;	
		goto fin_fstop;	
	}

	printk ("[%s]:[%d] OUTSIDE WAIT wakeup_vpe = %d vpet->fw_vmb.status = %d ACK = %d cpu = %d ret1 = %d vpet = %p, smp_processor_id() = %d , vpet->fw_vmb =%p !!!! \n ", __FUNCTION__, __LINE__, vpet->v_wq.wakeup_vpe, vpet->fw_vmb.status, FW_VMB_ACK , cpu, ret1 , vpet, smp_processor_id(), &vpet->fw_vmb);

	vpet->v_wq.wakeup_vpe = 0;

	if (vpet->fw_vmb.status == (uint32_t)IBL_IN_WAIT) {
		ret = VMB_SUCCESS;
		printk (KERN_ERR "[%s]:[%d]  ret = %d \n ", __FUNCTION__, __LINE__, ret);
	} else {
		ret = -VMB_ENACK;
		printk (KERN_ERR "[%s]:[%d] -ENACK recieved from FW .. Consider reseting the CPU using vmb_cpu_force_stop \n ", __FUNCTION__, __LINE__);
	}

fin_fstop:
	return ret;
}

int8_t vmb_cpu_stop(int8_t cpu) {
	int c_id, v_id;
	VMB_vpe_t *vpet;
	int ret, ret1;
	VMB_fw_msg_t *vmb_fw_msg_t;

/* CHECK :  GLOBAL LOCK needed as "cpu" can for a simutaneous vmb_cpu_start from 2 FWs */

	c_id = which_core(cpu);
	v_id = vpe_in_core(cpu);
	vpet = &core_t[c_id].vpe_t[v_id];

/* CHECK : GLOBAL UNLOCK needed as "cpu" can for a simutaneous vmb_cpu_start from 2 FWs */

/* Per-VPE Lock and update the vmb_fw_msg_t struct and update DDR */

	spin_lock(&vpet->vpe_lock);

	if ((vpet->bl_status == IBL_ACTIVE) && ((vpet->cpu_status & CPU_ACTIVE) == CPU_ACTIVE)) {
		vmb_fw_msg_t = (VMB_fw_msg_t *)VMB_get_msg_addr(cpu, 0);
		memset (vmb_fw_msg_t, 0 , sizeof(VMB_fw_msg_t));

	printk ("[%s]:[%d]  vmb_fw_msg_t = %p cpu = %d vmb_msg_size = %x sizeof(VMB_fw_msg_t) = %d sizeof(FW_vmb_msg_t) = %d !!!! \n ", __FUNCTION__, __LINE__, vmb_fw_msg_t, cpu, vmb_msg_size, sizeof(VMB_fw_msg_t), sizeof(FW_vmb_msg_t));

		vmb_fw_msg_t->msg_id = VMB_CPU_STOP;
	} else {
		ret = -VMB_ERROR;
		spin_unlock(&vpet->vpe_lock);
		vmb_cpu_free(cpu);
		vmb_tc_free(cpu, -1);
		goto fin_stop;
	}

	spin_unlock(&vpet->vpe_lock);

	/* Generate an IPI */

	/* TODO ???? */

	printk ("[%s]:[%d] WAITING FOR RESPONSE vpet->v_wq.wakeup_vpe = %d !!!! \n ", __FUNCTION__, __LINE__, vpet->v_wq.wakeup_vpe);

	/* Wait for timeout */
	ret1 = wait_event_interruptible_timeout(vpet->v_wq.vmb_wq, 
					(vpet->v_wq.wakeup_vpe == 1), QUEUE_TIMEOUT);
	if (ret1 <= 0 ) {
		printk ("[%s]:[%d] wait_event timeout occured for CPU = %d starting FW = %s \n", __FUNCTION__, __LINE__, cpu, vpet->name);
		printk ("CPU not responding so going for a force cpu stop .... \n");
		vpet->v_wq.wakeup_vpe = 0;
		vmb_cpu_free(cpu);
		vmb_tc_free(cpu, -1);
		vmb_cpu_force_stop(cpu);
		ret = -VMB_ETIMEOUT;	
		goto fin_stop;	
	}

	printk ("[%s]:[%d] OUTSIDE WAIT wakeup_vpe = %d vpet->fw_vmb.status = %d ACK = %d cpu = %d ret1 = %d vpet = %p, smp_processor_id() = %d , vpet->fw_vmb =%p !!!! \n ", __FUNCTION__, __LINE__, vpet->v_wq.wakeup_vpe, vpet->fw_vmb.status, FW_VMB_ACK , cpu, ret1 , vpet, smp_processor_id(), &vpet->fw_vmb);

	vpet->v_wq.wakeup_vpe = 0;

	if (vpet->fw_vmb.status == (uint32_t)FW_VMB_ACK) {
		ret = VMB_SUCCESS;
		vmb_cpu_free(cpu);
		vmb_tc_free(cpu, -1);
		printk (KERN_ERR "[%s]:[%d]  ret = %d \n ", __FUNCTION__, __LINE__, ret);
	} else {
		ret = -VMB_ENACK;
		printk (KERN_ERR "[%s]:[%d] -ENACK recieved from FW .. Consider reseting the CPU \n ", __FUNCTION__, __LINE__);
		vmb_cpu_free(cpu);
		vmb_tc_free(cpu, -1);
		vmb_cpu_force_stop(cpu);
	}

fin_stop:
	return ret;
}

int8_t vmb_cpu_start(int8_t cpu, char* fw_name, CPU_launch_t cpu_launch, TC_launch_t tc_launch[], uint8_t num_tcs ) {
	int c_id, v_id;
	VMB_vpe_t *vpet;
	int ret, ret1, i;
	VMB_fw_msg_t *vmb_fw_msg_t;

/* CHECK :  GLOBAL LOCK needed as "cpu" can for a simutaneous vmb_cpu_start from 2 FWs */

	c_id = which_core(cpu);
	v_id = vpe_in_core(cpu);
	vpet = &core_t[c_id].vpe_t[v_id];

/* CHECK : GLOBAL UNLOCK needed as "cpu" can for a simutaneous vmb_cpu_start from 2 FWs */

/* After getting the cpu then we may use Per-VPE locks */

	if ((void *)&cpu_launch == NULL)
		return -VMB_ENOPRM;

/* Per-VPE Lock and update the vmb_fw_msg_t struct and update DDR */

	spin_lock(&vpet->vpe_lock);

	strncpy(vpet->name, fw_name, sizeof(vpet->name));
	vmb_fw_msg_t = (VMB_fw_msg_t *)VMB_get_msg_addr(cpu, 0);

	memset (vmb_fw_msg_t, 0 , sizeof(VMB_fw_msg_t));

	printk ("[%s]:[%d]  vmb_fw_msg_t = %p cpu = %d vmb_msg_size = %x sizeof(VMB_fw_msg_t) = %d sizeof(FW_vmb_msg_t) = %d !!!! \n ", __FUNCTION__, __LINE__, vmb_fw_msg_t, cpu, vmb_msg_size, sizeof(VMB_fw_msg_t), sizeof(FW_vmb_msg_t));

	vmb_fw_msg_t->msg_id = VMB_CPU_START;
	memcpy(&vmb_fw_msg_t->cpu_launch, &cpu_launch, sizeof(vmb_fw_msg_t->cpu_launch));
	if ((void *)tc_launch != NULL) {
		for (i=0; i < (sizeof(tc_launch)/sizeof(tc_launch[0])); i++)
			memcpy(&vmb_fw_msg_t->tc_launch[i], &tc_launch[i], sizeof(vmb_fw_msg_t->tc_launch[i]));
	}

	spin_unlock(&vpet->vpe_lock);

	/* Generate an IPI */

	/* TODO ???? */

#ifdef CONFIG_MIPS_MALTA
	/* Linux bootup in MALTA*/
	amon_cpu_start(cpu, cpu_launch.start_addr, cpu_launch.sp, cpu_launch.gp, cpu_launch.a0);
#endif

	printk ("[%s]:[%d] WAITING FOR RESPONSE vpet->v_wq.wakeup_vpe = %d !!!! \n ", __FUNCTION__, __LINE__, vpet->v_wq.wakeup_vpe);

	/* Wait for timeout */
	ret1 = wait_event_interruptible_timeout(vpet->v_wq.vmb_wq, 
					(vpet->v_wq.wakeup_vpe == 1), QUEUE_TIMEOUT);
	if (ret1 <= 0 ) {
		printk ("[%s]:[%d] wait_event timeout occured for CPU = %d starting FW = %s \n", __FUNCTION__, __LINE__, cpu, vpet->name);
		printk ("CPU not responding so going for a force cpu stop .... \n");
		vpet->v_wq.wakeup_vpe = 0;
		vmb_cpu_free(cpu);
		vmb_tc_free(cpu, -1);
		vmb_cpu_force_stop(cpu);
		ret = -VMB_ETIMEOUT;	
		goto fin;	
	}

	printk ("[%s]:[%d] OUTSIDE WAIT wakeup_vpe = %d vpet->fw_vmb.status = %d ACK = %d cpu = %d ret1 = %d vpet = %p, smp_processor_id() = %d , vpet->fw_vmb =%p !!!! \n ", __FUNCTION__, __LINE__, vpet->v_wq.wakeup_vpe, vpet->fw_vmb.status, FW_VMB_ACK , cpu, ret1 , vpet, smp_processor_id(), &vpet->fw_vmb);

	vpet->v_wq.wakeup_vpe = 0;

	if (vpet->fw_vmb.status == (uint32_t)FW_VMB_ACK) {
		ret = VMB_SUCCESS;
		printk (KERN_ERR "[%s]:[%d]  ret = %d \n ", __FUNCTION__, __LINE__, ret);
	} else {
		ret = -VMB_ENACK;
		printk (KERN_ERR "[%s]:[%d] -ENACK recieved from FW .. Consider reseting the CPU \n ", __FUNCTION__, __LINE__);
		vmb_cpu_free(cpu);
		vmb_tc_free(cpu, -1);
		vmb_cpu_force_stop(cpu);
	}

fin:
	return ret;
}

/********************************** VMB TC APIs **************************/

int8_t vmb_tc_alloc (uint8_t cpu) {
	int ret = -VMB_ERROR;
	int i, c_id, v_id;
 	VMB_core_t *coret = core_t;
	VMB_vpe_t *vpet;
	
	c_id = which_core(cpu);
	v_id = vpe_in_core(cpu);
	vpet = &core_t[c_id].vpe_t[v_id];

	if ((vpet->bl_status == IBL_ACTIVE) && ((vpet->cpu_status & CPU_ACTIVE) == CPU_ACTIVE)) {
		VMB_tc_t *tct = coret[c_id].tc_t;
		for (i=2; i < MAX_TC; i++) {
			if ((tct[i].tc_status & TC_INACTIVE) == TC_INACTIVE) {
				tct[i].tc_status &= ~TC_INACTIVE;
				tct[i].tc_status |= TC_ACTIVE;
				ret = i;
				tct[i].vpe_id = v_id;
				printk ("[%s]:[%d] CPU tct[i].tc_status = %x \n", __FUNCTION__, __LINE__, tct[i].tc_status);
				goto fin_talloc;
			} else 
				ret = -VMB_EBUSY;
		}
	}
fin_talloc:
		return ret;
}


int8_t vmb_tc_free(int8_t cpu, int8_t tc_num) {
	int ret = -VMB_ERROR;
	int i, c_id, v_id;
 	VMB_core_t *coret = core_t;
	VMB_vpe_t *vpet;
	
	c_id = which_core(cpu);
	v_id = vpe_in_core(cpu);
	vpet = &core_t[c_id].vpe_t[v_id];

	if ((vpet->bl_status == IBL_ACTIVE) && ((vpet->cpu_status & CPU_ACTIVE) == CPU_ACTIVE))
		printk ("VPE %d is not active !!! still freeing the TC %d \n", cpu, tc_num);

	if (tc_num == -1) {
		VMB_tc_t *tct = coret[c_id].tc_t;
		for (i=2; i < MAX_TC; i++) {
			ret = get_cpu_id (c_id, tct[i].vpe_id);
			if (ret == cpu) {
				tct[i].tc_status &= ~TC_ACTIVE;
				tct[i].tc_status |= TC_INACTIVE;
			}
		}
		ret = VMB_SUCCESS;
	} else {
		VMB_tc_t *tct = &coret[c_id].tc_t[tc_num];
		ret = get_cpu_id (c_id, tct->vpe_id);

		if (ret == cpu) {
			tct->tc_status &= ~TC_ACTIVE;
			tct->tc_status |= TC_INACTIVE;
			ret = VMB_SUCCESS;
		} else {
			printk (" TC %d is not attached to CPU %d . Please use vmb_get_vpeid(cpu, tc_num) to get CPU to which active TC is attached \n", tc_num, cpu);
			ret = -VMB_EAVAIL;
		}
	}
	return ret;
}

int8_t vmb_get_vpeid (uint8_t cpu, uint8_t tc_num) {
	int ret = -VMB_ERROR;
	int c_id;
 	VMB_core_t *coret = core_t;
	VMB_tc_t *tct;
	
	c_id = which_core(cpu);
	tct = &coret[c_id].tc_t[tc_num];
	ret = get_cpu_id (c_id, tct->vpe_id);

	return ret;
}

int8_t vmb_tc_start(uint8_t cpu, TC_launch_t tc_launch[], uint8_t num_tcs ) {
	int c_id, v_id;
	VMB_vpe_t *vpet;
	int ret, ret1, i;
	VMB_fw_msg_t *vmb_fw_msg_t;

/* CHECK :  GLOBAL LOCK needed as "cpu" can for a simutaneous vmb_cpu_start from 2 FWs */

	c_id = which_core(cpu);
	v_id = vpe_in_core(cpu);
	vpet = &core_t[c_id].vpe_t[v_id];

/* CHECK : GLOBAL UNLOCK needed as "cpu" can for a simutaneous vmb_cpu_start from 2 FWs */

/* After getting the cpu then we may use Per-VPE locks */

	if ((void *)tc_launch == NULL || ((sizeof(tc_launch)/sizeof(tc_launch[0])) != num_tcs))
		return -VMB_ENOPRM;

/* Per-VPE Lock and update the vmb_fw_msg_t struct and update DDR */

	spin_lock(&vpet->vpe_lock);

	vmb_fw_msg_t = (VMB_fw_msg_t *)VMB_get_msg_addr(cpu, 0);
	memset (vmb_fw_msg_t, 0 , sizeof(VMB_fw_msg_t));

	printk ("[%s]:[%d]  vmb_fw_msg_t = %p cpu = %d vmb_msg_size = %x sizeof(VMB_fw_msg_t) = %d sizeof(FW_vmb_msg_t) = %d !!!! \n ", __FUNCTION__, __LINE__, vmb_fw_msg_t, cpu, vmb_msg_size, sizeof(VMB_fw_msg_t), sizeof(FW_vmb_msg_t));

	vmb_fw_msg_t->msg_id = VMB_TC_START;

	for (i=0; i < (sizeof(tc_launch)/sizeof(tc_launch[0])); i++)
		memcpy(&vmb_fw_msg_t->tc_launch[i], &tc_launch[i], sizeof(vmb_fw_msg_t->tc_launch[i]));

	spin_unlock(&vpet->vpe_lock);

	/* Generate an IPI */

	/* TODO ???? */

	printk ("[%s]:[%d] WAITING FOR RESPONSE vpet->v_wq.wakeup_vpe = %d !!!! \n ", __FUNCTION__, __LINE__, vpet->v_wq.wakeup_vpe);

	/* Wait for timeout */
	ret1 = wait_event_interruptible_timeout(vpet->v_wq.vmb_wq, 
					(vpet->v_wq.wakeup_vpe == 1), QUEUE_TIMEOUT);
	if (ret1 <= 0 ) {
		printk ("[%s]:[%d] wait_event timeout occured for CPU = %d starting FW = %s \n", __FUNCTION__, __LINE__, cpu, vpet->name);
		printk ("CPU not responding so going for a force cpu stop .... \n");
		vpet->v_wq.wakeup_vpe = 0;
		vmb_tc_free(cpu, -1);
		ret = -VMB_ETIMEOUT;	
		goto fin_tcstart;	
	}

	printk ("[%s]:[%d] OUTSIDE WAIT wakeup_vpe = %d vpet->fw_vmb.status = %d ACK = %d cpu = %d ret1 = %d vpet = %p, smp_processor_id() = %d , vpet->fw_vmb =%p !!!! \n ", __FUNCTION__, __LINE__, vpet->v_wq.wakeup_vpe, vpet->fw_vmb.status, FW_VMB_ACK , cpu, ret1 , vpet, smp_processor_id(), &vpet->fw_vmb);

	vpet->v_wq.wakeup_vpe = 0;

	if (vpet->fw_vmb.status == (uint32_t)FW_VMB_ACK) {
		ret = VMB_SUCCESS;
		printk (KERN_ERR "[%s]:[%d]  ret = %d \n ", __FUNCTION__, __LINE__, ret);
	} else {
		ret = -VMB_ENACK;
		printk (KERN_ERR "[%s]:[%d] -ENACK recieved from FW .. Consider reseting the CPU \n ", __FUNCTION__, __LINE__);
		vmb_tc_free(cpu, -1);
	}

fin_tcstart:
	return ret;
}


int8_t vmb_tc_stop(uint8_t cpu, uint8_t tc_num ) {
	int c_id, v_id;
	VMB_vpe_t *vpet;
	int ret, ret1;
	VMB_fw_msg_t *vmb_fw_msg_t;

/* CHECK :  GLOBAL LOCK needed as "cpu" can for a simutaneous vmb_cpu_start from 2 FWs */

	c_id = which_core(cpu);
	v_id = vpe_in_core(cpu);
	vpet = &core_t[c_id].vpe_t[v_id];

/* CHECK : GLOBAL UNLOCK needed as "cpu" can for a simutaneous vmb_cpu_start from 2 FWs */


/* Per-VPE Lock and update the vmb_fw_msg_t struct and update DDR */

	spin_lock(&vpet->vpe_lock);

	vmb_fw_msg_t = (VMB_fw_msg_t *)VMB_get_msg_addr(cpu, 0);
	memset (vmb_fw_msg_t, 0 , sizeof(VMB_fw_msg_t));

	printk ("[%s]:[%d]  vmb_fw_msg_t = %p cpu = %d vmb_msg_size = %x sizeof(VMB_fw_msg_t) = %d sizeof(FW_vmb_msg_t) = %d !!!! \n ", __FUNCTION__, __LINE__, vmb_fw_msg_t, cpu, vmb_msg_size, sizeof(VMB_fw_msg_t), sizeof(FW_vmb_msg_t));

	vmb_fw_msg_t->msg_id = VMB_TC_STOP;
	memcpy(&vmb_fw_msg_t->tc_num, &tc_num, sizeof(vmb_fw_msg_t->tc_num));

	spin_unlock(&vpet->vpe_lock);

	/* Generate an IPI */

	/* TODO ???? */

	printk ("[%s]:[%d] WAITING FOR RESPONSE vpet->v_wq.wakeup_vpe = %d !!!! \n ", __FUNCTION__, __LINE__, vpet->v_wq.wakeup_vpe);

	/* Wait for timeout */
	ret1 = wait_event_interruptible_timeout(vpet->v_wq.vmb_wq, 
					(vpet->v_wq.wakeup_vpe == 1), QUEUE_TIMEOUT);
	if (ret1 <= 0 ) {
		printk ("[%s]:[%d] wait_event timeout occured for CPU = %d starting FW = %s \n", __FUNCTION__, __LINE__, cpu, vpet->name);
		printk ("CPU not responding so going for a force cpu stop .... \n");
		vpet->v_wq.wakeup_vpe = 0;
		ret = -VMB_ETIMEOUT;	
		goto fin_tcstop;	
	}

	printk ("[%s]:[%d] OUTSIDE WAIT wakeup_vpe = %d vpet->fw_vmb.status = %d ACK = %d cpu = %d ret1 = %d vpet = %p, smp_processor_id() = %d , vpet->fw_vmb =%p !!!! \n ", __FUNCTION__, __LINE__, vpet->v_wq.wakeup_vpe, vpet->fw_vmb.status, FW_VMB_ACK , cpu, ret1 , vpet, smp_processor_id(), &vpet->fw_vmb);

	vpet->v_wq.wakeup_vpe = 0;

	if (vpet->fw_vmb.status == (uint32_t)FW_VMB_ACK) {
		ret = VMB_SUCCESS;
		printk (KERN_ERR "[%s]:[%d]  ret = %d \n ", __FUNCTION__, __LINE__, ret);
	} else {
		ret = -VMB_ENACK;
		printk (KERN_ERR "[%s]:[%d] -ENACK recieved from FW .. Consider reseting the CPU \n ", __FUNCTION__, __LINE__);
	}

fin_tcstop:
	vmb_tc_free(cpu, tc_num);
	return ret;
}


int8_t vmb_tc_pause(uint8_t cpu, uint8_t tc_num ) {
	int c_id, v_id;
	VMB_vpe_t *vpet;
	int ret, ret1;
	VMB_fw_msg_t *vmb_fw_msg_t;

/* CHECK :  GLOBAL LOCK needed as "cpu" can for a simutaneous vmb_cpu_start from 2 FWs */

	c_id = which_core(cpu);
	v_id = vpe_in_core(cpu);
	vpet = &core_t[c_id].vpe_t[v_id];

/* CHECK : GLOBAL UNLOCK needed as "cpu" can for a simutaneous vmb_cpu_start from 2 FWs */


/* Per-VPE Lock and update the vmb_fw_msg_t struct and update DDR */

	spin_lock(&vpet->vpe_lock);

	vmb_fw_msg_t = (VMB_fw_msg_t *)VMB_get_msg_addr(cpu, 0);
	memset (vmb_fw_msg_t, 0 , sizeof(VMB_fw_msg_t));
	vmb_fw_msg_t->msg_id = VMB_TC_PAUSE;
	memcpy(&vmb_fw_msg_t->tc_num, &tc_num, sizeof(vmb_fw_msg_t->tc_num));

	spin_unlock(&vpet->vpe_lock);

	/* Generate an IPI */

	/* TODO ???? */

	printk ("[%s]:[%d] WAITING FOR RESPONSE vpet->v_wq.wakeup_vpe = %d !!!! \n ", __FUNCTION__, __LINE__, vpet->v_wq.wakeup_vpe);

	/* Wait for timeout */
	ret1 = wait_event_interruptible_timeout(vpet->v_wq.vmb_wq, 
					(vpet->v_wq.wakeup_vpe == 1), QUEUE_TIMEOUT);
	if (ret1 <= 0 ) {
		printk ("[%s]:[%d] wait_event timeout occured for CPU = %d starting FW = %s \n", __FUNCTION__, __LINE__, cpu, vpet->name);
		printk ("CPU could not be Paused !!!.... \n");
		vpet->v_wq.wakeup_vpe = 0;
		ret = -VMB_ETIMEOUT;	
		goto fin_tcpause;	
	}

	printk ("[%s]:[%d] OUTSIDE WAIT wakeup_vpe = %d vpet->fw_vmb.status = %d ACK = %d cpu = %d ret1 = %d vpet = %p, smp_processor_id() = %d , vpet->fw_vmb =%p !!!! \n ", __FUNCTION__, __LINE__, vpet->v_wq.wakeup_vpe, vpet->fw_vmb.status, FW_VMB_ACK , cpu, ret1 , vpet, smp_processor_id(), &vpet->fw_vmb);

	vpet->v_wq.wakeup_vpe = 0;

	if (vpet->fw_vmb.status == (uint32_t)FW_VMB_ACK) {
		ret = VMB_SUCCESS;
		printk (KERN_ERR "[%s]:[%d]  ret = %d \n ", __FUNCTION__, __LINE__, ret);
	} else {
		ret = -VMB_ENACK;
		printk (KERN_ERR "[%s]:[%d] -ENACK recieved from FW .. Consider reseting the CPU \n ", __FUNCTION__, __LINE__);
	}

fin_tcpause:
	return ret;
}


int8_t vmb_tc_resume(uint8_t cpu, uint8_t tc_num ) {
	int c_id, v_id;
	VMB_vpe_t *vpet;
	int ret, ret1;
	VMB_fw_msg_t *vmb_fw_msg_t;

/* CHECK :  GLOBAL LOCK needed as "cpu" can for a simutaneous vmb_cpu_start from 2 FWs */

	c_id = which_core(cpu);
	v_id = vpe_in_core(cpu);
	vpet = &core_t[c_id].vpe_t[v_id];

/* CHECK : GLOBAL UNLOCK needed as "cpu" can for a simutaneous vmb_cpu_start from 2 FWs */


/* Per-VPE Lock and update the vmb_fw_msg_t struct and update DDR */

	spin_lock(&vpet->vpe_lock);

	vmb_fw_msg_t = (VMB_fw_msg_t *)VMB_get_msg_addr(cpu, 0);
	memset (vmb_fw_msg_t, 0 , sizeof(VMB_fw_msg_t));
	vmb_fw_msg_t->msg_id = VMB_TC_RESUME;
	memcpy(&vmb_fw_msg_t->tc_num, &tc_num, sizeof(vmb_fw_msg_t->tc_num));

	spin_unlock(&vpet->vpe_lock);

	/* Generate an IPI */

	/* TODO ???? */

	printk ("[%s]:[%d] WAITING FOR RESPONSE vpet->v_wq.wakeup_vpe = %d !!!! \n ", __FUNCTION__, __LINE__, vpet->v_wq.wakeup_vpe);

	/* Wait for timeout */
	ret1 = wait_event_interruptible_timeout(vpet->v_wq.vmb_wq, 
					(vpet->v_wq.wakeup_vpe == 1), QUEUE_TIMEOUT);
	if (ret1 <= 0 ) {
		printk ("[%s]:[%d] wait_event timeout occured for CPU = %d starting FW = %s \n", __FUNCTION__, __LINE__, cpu, vpet->name);
		printk ("CPU could not be Resumed !!!.... \n");
		vpet->v_wq.wakeup_vpe = 0;
		ret = -VMB_ETIMEOUT;	
		goto fin_tcres;	
	}

	printk ("[%s]:[%d] OUTSIDE WAIT wakeup_vpe = %d vpet->fw_vmb.status = %d ACK = %d cpu = %d ret1 = %d vpet = %p, smp_processor_id() = %d , vpet->fw_vmb =%p !!!! \n ", __FUNCTION__, __LINE__, vpet->v_wq.wakeup_vpe, vpet->fw_vmb.status, FW_VMB_ACK , cpu, ret1 , vpet, smp_processor_id(), &vpet->fw_vmb);

	vpet->v_wq.wakeup_vpe = 0;

	if (vpet->fw_vmb.status == (uint32_t)FW_VMB_ACK) {
		ret = VMB_SUCCESS;
		printk (KERN_ERR "[%s]:[%d]  ret = %d \n ", __FUNCTION__, __LINE__, ret);
	} else {
		ret = -VMB_ENACK;
		printk ("CPU could not be Resumed !!!.... \n");
		printk (KERN_ERR "[%s]:[%d] -ENACK recieved from FW .. Consider reseting the CPU \n ", __FUNCTION__, __LINE__);
	}

fin_tcres:
	return ret;
}
