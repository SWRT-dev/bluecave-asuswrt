/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2011 MIPS Technologies, Inc.
 */
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/mipsregs.h>

#ifdef CONFIG_PROC_FS
static struct proc_dir_entry *segments;

static void proc_build_segment_config(char *str, unsigned int cfg)
{
	unsigned int am;
	int len = 0;
	static const char * const am_str[] = {
		"UK  ", "MK  ", "MSK  ", "MUSK  ", "MUSUK  ", "USK  ",
		"*Reserved*  ", "UUSK  "};

	/* Segment access mode. */
	am = (cfg & MIPS_SEGCFG_AM) >> MIPS_SEGCFG_AM_SHIFT;
	len += sprintf(str + len, "%s", am_str[am]);

	/*
	 * Access modes MK, MSK and MUSK are mapped segments. Therefore
	 * there is no direct physical address mapping.
	 */
	if ((am == 0) || (am > 3))
		len += sprintf(str + len, "          %03lx",
			((cfg & MIPS_SEGCFG_PA) >> MIPS_SEGCFG_PA_SHIFT));
	else
		len += sprintf(str + len, "          UND");

	/*
	 * Access modes MK, MSK and MUSK are mapped segments. Therefore
	 * there is no defined cache mode setting.
	 */
	if ((am == 0) || (am > 3))
		len += sprintf(str + len, "         %01ld",
			((cfg & MIPS_SEGCFG_C) >> MIPS_SEGCFG_C_SHIFT));
	else
		len += sprintf(str + len, "         U");

	/* Exception configuration. */
	len += sprintf(str + len, "         %01ld\n",
		((cfg & MIPS_SEGCFG_EU) >> MIPS_SEGCFG_EU_SHIFT));
}

static int proc_read_segments(struct seq_file *m, void *v)
{
	unsigned int segcfg;
	char str[42];

	seq_printf(m, "\nSegment   Virtual    Size   Access Mode    Physical    Caching     EU\n");

	seq_printf(m, "-------   --------   ----   -----------   ----------   -------   ------\n");

	segcfg = read_c0_segctl0();
	proc_build_segment_config(str, segcfg);
	seq_printf(m, "   0      e0000000   512M      %s", str);

	segcfg >>= 16;
	proc_build_segment_config(str, segcfg);
	seq_printf(m, "   1      c0000000   512M      %s", str);

	segcfg = read_c0_segctl1();
	proc_build_segment_config(str, segcfg);
	seq_printf(m, "   2      a0000000   512M      %s", str);

	segcfg >>= 16;
	proc_build_segment_config(str, segcfg);
	seq_printf(m, "   3      80000000   512M      %s", str);

	segcfg = read_c0_segctl2();
	proc_build_segment_config(str, segcfg);
	seq_printf(m, "   4      40000000    1G       %s", str);

	segcfg >>= 16;
	proc_build_segment_config(str, segcfg);
	seq_printf(m, "   5      00000000    1G       %s\n", str);

	return 0;
}

static int segment_proc_open(struct inode *inode, struct file *file)
{
        return single_open(file, proc_read_segments, NULL);
}

static const struct file_operations segments_proc_fops = {
        .open           = segment_proc_open,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};

static int __init segments_info(void)
{
	if (cpu_has_segments) {
		segments = proc_create("segments", 0444, NULL, &segments_proc_fops);
		if (!segments)
			return -ENOMEM;
	}
	return 0;
}

__initcall(segments_info);
#endif /* CONFIG_PROC_FS */
