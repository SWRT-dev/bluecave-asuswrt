/*
 * /proc hooks for CPU performance counter support for SMTC kernel 
 * (and ultimately others)
 * Copyright (C) 2006 Mips Technologies, Inc
 */

#include <linux/kernel.h>

#include <asm/cpu.h>
#include <asm/processor.h>
#include <asm/barrier.h>
#include <asm/mipsregs.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

/*
 * /proc diagnostic and statistics hooks
 */

/* Internal software-extended event counters */

static unsigned long long extencount[4] = {0,0,0,0};

static struct proc_dir_entry *perf_proc;

static int proc_read_perf(struct seq_file *s, void *v)
{

	seq_printf(s, "PerfCnt[0].Ctl : 0x%08x\n", read_c0_perfctrl0());
	seq_printf(s, "PerfCnt[0].Cnt : %Lu\n", 
	  extencount[0] + (unsigned long long)((unsigned)read_c0_perfcntr0()));
	seq_printf(s, "PerfCnt[1].Ctl : 0x%08x\n", read_c0_perfctrl1());
	seq_printf(s, "PerfCnt[1].Cnt : %Lu\n",
	  extencount[1] + (unsigned long long)((unsigned)read_c0_perfcntr1()));
	seq_printf(s, "PerfCnt[2].Ctl : 0x%08x\n", read_c0_perfctrl2());
	seq_printf(s, "PerfCnt[2].Cnt : %Lu\n",
	  extencount[2] + (unsigned long long)((unsigned)read_c0_perfcntr2()));
	seq_printf(s, "PerfCnt[3].Ctl : 0x%08x\n", read_c0_perfctrl3());
	seq_printf(s, "PerfCnt[3].Cnt : %Lu\n",
	  extencount[3] + (unsigned long long)((unsigned)read_c0_perfcntr3()));

	return 0;
}

/*
 * Write to perf counter registers based on text input
 */

#define TXTBUFSZ 512

static ssize_t perf_proc_write(struct file *file, const char *buffer, 
					size_t count, loff_t *data)
{
	size_t len;
	int nparsed;
	int index;
	char mybuf[TXTBUFSZ];

	int which[4];
	unsigned long control[4];
	long long ctrdata[4];

	if(count >= TXTBUFSZ) len = TXTBUFSZ-1;
	else len = count;
	memset(mybuf,0,TXTBUFSZ);
	if(copy_from_user(mybuf, buffer, len)) return -EFAULT;

	nparsed = sscanf(mybuf,
			"%d %lx %Ld %d %lx %Ld %d %lx %Ld %d %lx %Ld", 
				&which[0], &control[0], &ctrdata[0],
				&which[1], &control[1], &ctrdata[1],
				&which[2], &control[2], &ctrdata[2],
				&which[3], &control[3], &ctrdata[3]);

	for(index = 0; nparsed >= 3; index++) { 
		switch (which[index]) {
		case 0:
			write_c0_perfctrl0(control[index]);
			if(ctrdata[index] != -1) {
			    extencount[0] = (unsigned long long)ctrdata[index];
			    write_c0_perfcntr0((unsigned long)0);
			}
			break;
		case 1:
			write_c0_perfctrl1(control[index]);
			if(ctrdata[index] != -1) {
			    extencount[1] = (unsigned long long)ctrdata[index];
			    write_c0_perfcntr1((unsigned long)0);
			}
			break;
		case 2:
			write_c0_perfctrl2(control[index]);
			if(ctrdata[index] != -1) {
			    extencount[2] = (unsigned long long)ctrdata[index];
			    write_c0_perfcntr2((unsigned long)0);
			}
			break;
		case 3:
			write_c0_perfctrl3(control[index]);
			if(ctrdata[index] != -1) {
			    extencount[3] = (unsigned long long)ctrdata[index];
			    write_c0_perfcntr3((unsigned long)0);
			}
			break;
		}
		nparsed -= 3;
	}
	return (len);
}

extern int (*perf_irq)(struct pt_regs *regs);

/*
 * Invoked when timer interrupt vector picks up a perf counter overflow
 */

static int perf_proc_irq(struct pt_regs *regs)
{
	unsigned long snapshot;

	/* 
	 * It would be nice to do this as a loop, but we don't have
	 * indirect access to CP0 registers.
	 */
	snapshot = read_c0_perfcntr0();
	if((long)snapshot < 0) {
		extencount[0] += 
			(unsigned long long)((unsigned)read_c0_perfcntr0());
		write_c0_perfcntr0(0);
	}
	snapshot = read_c0_perfcntr1();
	if((long)snapshot < 0) {
		extencount[1] += 
			(unsigned long long)((unsigned)read_c0_perfcntr1());
		write_c0_perfcntr1(0);
	}
	snapshot = read_c0_perfcntr2();
	if((long)snapshot < 0) {
		extencount[2] += 
			(unsigned long long)((unsigned)read_c0_perfcntr2());
		write_c0_perfcntr2(0);
	}
	snapshot = read_c0_perfcntr3();
	if((long)snapshot < 0) {
		extencount[3] += 
			(unsigned long long)((unsigned)read_c0_perfcntr3());
		write_c0_perfcntr3(0);
	}
	return 0;
}

static int perf_proc_open(struct inode *inode, struct file *file)
{
 return single_open(file,proc_read_perf, NULL);
}

static const struct file_operations perf_proc_fops={
 .open           = perf_proc_open,
 .read           = seq_read,
 .write          = perf_proc_write,
 .llseek         = seq_lseek,
 .release        = single_release,
};

static int __init init_perf_proc(void)
{
	extern struct proc_dir_entry *get_mips_proc_dir(void);

	struct proc_dir_entry *mips_proc_dir = get_mips_proc_dir();

	write_c0_perfcntr0(0);
	write_c0_perfcntr1(0);
	write_c0_perfcntr2(0);
	write_c0_perfcntr3(0);
	perf_proc = proc_create("perf", 0644, mips_proc_dir,&perf_proc_fops);
	perf_irq = perf_proc_irq;

	return 0;
}

/* Automagically create the entry */
module_init(init_perf_proc);
