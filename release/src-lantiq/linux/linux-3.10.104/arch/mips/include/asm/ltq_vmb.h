/******************************************************************************

                         Copyright (c) 2012, 2014, 2015
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _LTQ_VMB_H
#define _LTQ_VMB_H

#include <asm/gic.h>
#include <asm/gcmpregs.h>

//const uint8_t vmb_cores = (GCMPGCB(GC) & GCMP_GCB_GC_NUMCORES_MSK) + 1;
//const uint8_t vmb_vpes = (GCMPCLCB(CFG) & GCMP_CCB_CFG_NUMVPE_MSK) + 1;
//#define MAX_CORE vmb_cores
//#define MAX_CPUS vmb_vpes

#define MAX_CORE 2 
#define MAX_VPE 2 
#define MAX_TC	6	/* Used internally for Database ONLY */
#define MAX_TCS	4
#define MAX_CPU (MAX_CORE * MAX_VPE)

#define NLINUX_CPUS (vmb_vpes - NR_CPUS)

#define vmb_msg_size (sizeof(VMB_fw_msg_t) + sizeof(FW_vmb_msg_t))
#define CPU_LAUNCH 0xa0000000

/* REVERT BACK TO 10 */
#define QUEUE_TIMEOUT (100 * HZ) 

#define get_cpu_id(core, vpe)   ((core * 2) + vpe)
#define which_core(cpu)         (cpu / 2)
#define vpe_in_core(cpu)        (cpu % 2)


#define VMB_CPU_START	0x00000001
#define VMB_CPU_STOP	0x00000002
#define VMB_TC_START	0x00000004
#define VMB_TC_STOP	0x00000008
#define VMB_TC_PAUSE	0x00000010
#define VMB_TC_RESUME	0x00000020

#define FW_VMB_ACK	0x00000001
#define FW_VMB_NACK	0x00000002
#define FW_RESET	0x00000004
#define IBL_IN_WAIT	0x00000008
#define FW_VMB_PRIV_INFO	0x00000010


#define IBL_ACTIVE	0x1
#define IBL_INACTIVE	0x2

#define CORE_ACTIVE	0x1
#define CORE_INACTIVE	0x2

#define CPU_ACTIVE	0x1
#define CPU_INACTIVE	0x2
#define CPU_BOOTUP_DT	0x10

#define TC_ACTIVE	0x1
#define TC_INACTIVE	0x2

#define MAX_YIELD_INTF	16
#define	YR_ACTIVE	IBL_ACTIVE
#define YR_INACTIVE	IBL_INACTIVE

#define MAX_ITC_SEMID	16
#define	ITC_ACTIVE	IBL_ACTIVE
#define ITC_INACTIVE	IBL_INACTIVE

/* ERROR codes */
#define VMB_SUCCESS		1
#define VMB_ERROR		2
#define VMB_EBUSY		3
#define VMB_EAVAIL		4
#define VMB_ERESET		5
#define VMB_ETIMEOUT		6
#define VMB_ENACK		7
#define VMB_ENOPRM		8


typedef struct {
	uint32_t	start_addr;
	uint32_t	sp;
	uint32_t	gp;
	uint32_t	a0;
	uint32_t	eva;
	uint32_t	mt_group;
	uint32_t	yield_res;
	uint32_t	priv_info;
} CPU_launch_t; 

typedef struct {
	uint32_t	tc_num;
	uint32_t	mt_group;
	uint32_t	start_addr;
	uint32_t	sp;
	uint32_t	gp;
	uint32_t	a0;
	uint32_t	state;
	uint32_t	priv_info;
} TC_launch_t;

typedef struct {
	uint32_t	msg_id;
	CPU_launch_t	cpu_launch;
	TC_launch_t	tc_launch[MAX_TCS];
	uint32_t	tc_num;
} VMB_fw_msg_t;

typedef struct {
	uint32_t	status;
	uint32_t	priv_info;
} FW_vmb_msg_t;

/* Structure for Event Handling used during handshake between VMB and FW/Linux */

typedef struct { 
	wait_queue_head_t	vmb_wq;   /* event object */
	uint8_t		wakeup_vpe;  	 /* wakeup condition flag */
} VMB_evt_t;

typedef struct	{
	uint8_t		vpe_id; /* Indicates which VPE this TC is attached */
	uint8_t		tc_status; 
	uint8_t         tcmt_grp;
} VMB_tc_t;

typedef struct {
	int8_t cpu_id;
	int8_t yr_status;	
} VMB_yr_t;

typedef struct {
	int8_t cpu_id;
	int8_t tc_id;
	int8_t itc_status;
} VMB_itc_t;

typedef struct {
	char name[16];	/* Name of the FW or LinuxOS */
	int8_t		bl_status;
	int8_t		cpu_status;
	int8_t		core_id;
	int8_t		cpu_id; /* Keep track of CPU numbers like CORE0/VPE0 - CPU0 , 
				CORE0/VPE1 - CPU1, CORE1/VPE0 - CPU2, CORE1/VPE1 - CPU3. This will save lot of internal processing */
	spinlock_t	vpe_lock;
	void	(*vmb_callback_fn)(uint32_t status);
	VMB_evt_t 		v_wq;
	FW_vmb_msg_t		fw_vmb;
	uint8_t		vpemt_grp;
} VMB_vpe_t;

typedef struct {
	uint8_t		active; /* May be used for power management or hotplug to check the status of the core */
	VMB_vpe_t	vpe_t[MAX_VPE];	
	VMB_tc_t	tc_t[MAX_TC];	/* TCs can be unevenly distrubited to VPEs so should be under core structure and not in vpe structure */ 
	VMB_yr_t	yr_t[MAX_YIELD_INTF];
} VMB_core_t;

/* IRQ Numbers and handler definations */
	#define	 VMB_CPU_IPI1		20
	#define	 VMB_CPU_IPI2		21
	#define	 VMB_CPU_IPI3		85

/* 
  IPI numbers generated from FW (MPE/Voice) --> VMB for handling of requests w.r.t initial  boot-up handshake OR reset/NMI scenarios
  The fw_vmb_msg_t->status field acts as the differentiator to recognise if the IPI is from FW  due FW_VMB_ACK or NACK or FW_RESET (Async reset)
  IPI numbers generated from InterAptiv-BL --> VMB to indicate that BL is ready. 
  The fw_vmb_msg_t->status field acts as the differentiator to recognise if the IPI is from FW (FW_VMB_ACK or NACK) OR from InterAptiv-BL (IBL_IN_WAIT) 
*/
 
	#define FW_VMB_IPI1		(87 + MIPS_GIC_IRQ_BASE) /* FW/IBL on CPU1 -> VMB */
	#define FW_VMB_IPI2		(88 + MIPS_GIC_IRQ_BASE) /* FW/IBL on CPU2 -> VMB */
	#define FW_VMB_IPI3		(110 + MIPS_GIC_IRQ_BASE) /* FW/IBL on CPU3 -> VMB */

/* Function Definations as per Spec*/
/* return type should be int[16/8]_t due to negative error return types */
/* if int8_t used then retrun type cannot cross +- 253 */

typedef void (*vmb_callback_func)(uint32_t status);

int8_t vmb_cpu_alloc(int8_t cpu, char* fw_name);
int8_t vmb_cpu_start(int8_t cpu, CPU_launch_t cpu_launch, TC_launch_t tc_launch[], uint8_t num_tcs, uint8_t num_yr);
int8_t vmb_cpu_free(int8_t cpu);
int8_t vmb_cpu_stop(int8_t cpu);
int8_t vmb_cpu_force_stop(int8_t cpu);
void vmb_register_callback (uint8_t cpu_num, vmb_callback_func func); 

int8_t vmb_tc_stop(uint8_t cpu, uint8_t tc_num);
int8_t vmb_tc_start(uint8_t cpu, TC_launch_t tc_launch[], uint8_t num_tcs);
int8_t vmb_tc_alloc (uint8_t cpu);
int8_t vmb_tc_free(int8_t cpu, int8_t tc_num);
int8_t vmb_tc_pause(uint8_t cpu, uint8_t tc_num);
int8_t vmb_tc_resume(uint8_t cpu, uint8_t tc_num);
int8_t vmb_get_vpeid (uint8_t cpu, uint8_t tc_num);
void *VMB_get_msg_addr (int cpu, int direction);
int32_t vmb_yr_get(uint8_t cpu, uint16_t num_yr);
void vmb_yr_free (uint8_t cpu, int16_t yr);
int32_t vmb_itc_sem_get(uint8_t cpu, uint8_t tc_num);
void vmb_itc_sem_free(int8_t semID);
int32_t fw_vmb_get_irq(uint8_t cpu);

#endif
