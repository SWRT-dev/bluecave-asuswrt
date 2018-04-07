#include <linux/fs.h>
#include <linux/seq_file.h>
#include <net/datapath_proc_api.h>
#include <net/datapath_api.h>
#include <lantiq.h>
#include <lantiq_soc.h>
#include <lantiq_irq.h>
#include <lantiq_pcie.h>
#include <lantiq_atm.h>

#include "include/vrx318_ppe_fw_ds.h"
#include "include/vrx318_ppe_atm_ds.h"
#include "include/vrx318_fw_prereq.h"
#include "include/vrx318_ppe_fw_const.h"
#include "include/unified_qos_ds_be.h"
#include "include/vrx318_edma.h"
#include "include/vrx318_common.h"
#include "include/vrx318_framework.h"
#include "include/vrx318_atm_tc.h"
#include "include/vrx318_ptm_tc.h"
#include "include/vrx318_api.h"
#include "include/vrx318_proc.h"
#include "include/vrx318_ppe_ptm_tc_ds.h"
#include "include/vrx318_ppe_bonding_ds.h"

char *vrx_dbg_flag_str[] = {
	"err",
	"rx",
	"tx",
	"event",
	"rx_dump",
	"tx_dump",
	"init",
	"info",
	"oam_rx",
	"oam_tx",
	"oam_rx_dump",
	"oam_tx_dump",
	"qos",
	"tc",
	"loopback",
	"coc"
};

static int proc_read_atm_cfg(struct seq_file *seq, void *v)
{
	int i;
	struct vrx318_atm_tc *priv = (struct vrx318_atm_tc *)seq->private;
	u32 qos_en = priv->frwk->param.atm_qos;

	seq_printf(seq, "VRX318 ATM on line: %d\n", priv->ep_id);
	seq_printf(seq, "Showtime: %d\n", atm_in_showtime());
	seq_printf(seq, "Power Saving: %s\n",
		atm_power_saving(priv, 0, 0) == 1 ? "Enable" : "Disable");

	seq_puts(seq, "PVC list:\n");
	spin_lock_bh(&priv->atm_lock);
	seq_printf(seq, "PVC MAP: 0x%x\n", priv->pvc_tbl);
	seq_printf(seq, "Tx Queue MAP: 0x%x\n", priv->sw_txq_tbl);

	for (i = 0; i < ATM_PVC_NUM(qos_en); i++)
		if (priv->pvc_tbl & BIT(i))
			seq_printf(seq, "[%d]: VPI: %d, VCI: %d, dev: %s, MPoA type: %d, MPoA mode: %d, Subif_id: 0x%x, Tx Queue Table: 0x%x\n "
				, i, priv->conn[i].vcc->vpi,
				priv->conn[i].vcc->vci,
				priv->conn[i].dev->name,
				priv->conn[i].mpoa_type,
				priv->conn[i].mpoa_mode,
				priv->conn[i].subif_id,
				priv->conn[i].sw_txq_tbl);
	spin_unlock_bh(&priv->atm_lock);
	return 0;
}

static int proc_read_atm_cfg_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_atm_cfg, PDE_DATA(inode));
}

static ssize_t vrx318_atm_cfg_proc_write(struct file *file,
		const char __user *buf,	size_t count, loff_t *data)
{
	struct vrx318_atm_tc *priv;
	u32 base, ep_id, en;
	int num, len, rlen;
	char *param_list[32];
	char str[128];

	priv = (struct vrx318_atm_tc *)PDE_DATA(file_inode(file));
	ep_id = priv->ep_id;
	base = (u32)priv->frwk->ep_dev[ep_id].membase;

	if (!access_ok(VERIFY_READ, buf, count))
		return -EFAULT;

	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));

	if (dp_strcmpi(param_list[0], "en") == 0
		|| dp_strcmpi(param_list[0], "enble") == 0) {
		en = 1;
	} else if (dp_strcmpi(param_list[0], "dis") == 0
		|| dp_strcmpi(param_list[0], "disable") == 0) {
		en = 0;
	} else
		goto proc_atm_cfg_help;

	if (dp_strcmpi(param_list[1], "ps") == 0
		|| dp_strcmpi(param_list[1], "powersaving") == 0) {
		atm_power_saving(priv, en, 1);
	}

	return count;

proc_atm_cfg_help:
	vrx318_dbg(DBG_INFO, "echo [enable | disable] powersaving > cfg\n");
	return count;
}



static const struct file_operations vrx318_atm_cfg_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_atm_cfg_seq_open,
	.read		= seq_read,
	.write		= vrx318_atm_cfg_proc_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static ssize_t vrx318_mem_proc_write(struct file *file, const char __user *buf,
				size_t count, loff_t *data)
{
	int c, i;
	int action = 0;
	u32 addr, set_val, repeat_cnt;
	struct vrx318_tc *priv;
	u32 base, ep_id, soc_flag;
	int num, len, rlen, idx;
	char *param_list[32], *p;
	char str[128];

	priv = (struct vrx318_tc *)PDE_DATA(file_inode(file));
	ep_id = priv->ep_id;
	base = (u32)priv->frwk->ep_dev[ep_id].membase;
	addr = set_val = repeat_cnt = 0;
	soc_flag = 0;

	if (!access_ok(VERIFY_READ, buf, count))
		return -EFAULT;

	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));

	if (dp_strcmpi(param_list[0], "r") == 0) {
		action = PPE_PROC_READ;
	} else if (dp_strcmpi(param_list[0], "w") == 0) {
			action = PPE_PROC_WRITE;
	} else if ((dp_strcmpi(param_list[0], "s") == 0) &&
			 (dp_strcmpi(param_list[1], "w") == 0)) {
		action = PPE_PROC_WRITE;
		soc_flag = 1;
	} else if ((dp_strcmpi(param_list[0], "s") == 0)
			&& (dp_strcmpi(param_list[1], "r") == 0)) {
		action = PPE_PROC_READ;
		soc_flag = 1;
	} else {
		goto proc_mem_help;
	}

	idx = soc_flag ? 2 : 1;
	num -= (soc_flag) ? 2 : 1;

	p = param_list[idx++];
	addr = get_num_from_str(&p, &len, 1);
	addr = SB_BUFFER(addr);

	if (action == PPE_PROC_READ) {
		repeat_cnt = dp_atoi(param_list[idx]);
		for (i = 0; i < repeat_cnt; i += 4) {
			if (soc_flag)
				printk("0x%08x: ", addr + (i << 2));
			else
				printk("0x%08x: ", ((addr + i * 4) & VRX318_SMASK) + base);
			for (c = 0; c < 4 && (i + c) < repeat_cnt; c++)
				if (soc_flag)
					printk("%08x  ", ltq_r32((void *)(addr + ((i + c) << 2))));
				else
					printk("%08x  ", vrx318_r32(addr + ((i + c) << 2), base));
				printk("\n");
		}
	} else if (action == PPE_PROC_WRITE) {
		if (dp_strcmpi(param_list[idx + 1], "r") == 0 ||
			dp_strcmpi(param_list[idx + 1], "rpt") == 0) {
			repeat_cnt = dp_atoi(param_list[idx + 2]);
			set_val = dp_atoi(param_list[idx]);
			for (i = 0; i < repeat_cnt; i++) {
				if (soc_flag)
					ltq_w32(set_val, (void *)(addr + (i << 2)));
				else
					vrx318_w32(set_val, addr + (i << 2), base);
			}
		} else {
			for (i = 0; i < num; i++) {
				set_val = dp_atoi(param_list[idx + i]);
				if (soc_flag)
					ltq_w32(set_val, (void *)(addr + (i << 2)));
				else
					vrx318_w32(set_val, addr + (i << 2), base);
			}
		}
	}

	return count;

proc_mem_help:
	vrx318_dbg(DBG_INFO, "echo <s> r [soc_addr] [count] > mem\n");
	vrx318_dbg(DBG_INFO, "echo <s> w [addr] [value] [val0] <val1> ... > mem\n");
	vrx318_dbg(DBG_INFO, "echo <s> w [addr] <value> r [repeat_count] > mem\n");
	return count;
}

static int proc_vrx318_read_mem(struct seq_file *seq, void *v)
{
	return 0;
}

static int proc_read_mem_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_vrx318_read_mem, NULL);
}

static const struct file_operations vrx318_mem_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_mem_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= vrx318_mem_proc_write,
};

static ssize_t vrx318_pp32_proc_write(struct file *file, const char __user *buf,
				size_t count, loff_t *data)
{
		struct vrx318_tc *priv;
		int ep_id;
		u32 base, reg_val;
		char str[128];
		u32 addr;
		int len, rlen, pc;
		int pp32 = 0;

		int num;
		char *param_list[20];

		priv = (struct vrx318_tc *)PDE_DATA(file_inode(file));
		ep_id = priv->ep_id;
		base = (u32)priv->frwk->ep_dev[ep_id].membase;

		len = count < sizeof(str) ? count : sizeof(str) - 1;
		rlen = len - copy_from_user(str, buf, len);
		str[rlen] = 0;
		num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));
		pp32 = dp_atoi(param_list[1]);
		if (dp_strcmpi(param_list[0], "pp32") == 0) {
			if (dp_strcmpi(param_list[2], "restart") == 0) {
				reg_val = vrx318_r32(PP32_FREEZE, base) & (~(1 << (pp32 << 4)));
				vrx318_w32(reg_val, PP32_FREEZE, base);
			} else if (dp_strcmpi(param_list[2], "freeze") == 0) {
				reg_val = vrx318_r32(PP32_FREEZE, base) | (1 << (pp32 << 4));
				vrx318_w32(reg_val, PP32_FREEZE, base);
			} else if (dp_strcmpi(param_list[2], "start") == 0) {
				vrx318_w32(PP32_CTRL_CMD_RESTART, PP32_CTRL_CMD(pp32), base);
			} else if (dp_strcmpi(param_list[2], "stop") == 0) {
				vrx318_w32(PP32_CTRL_CMD_STOP, PP32_CTRL_CMD(pp32), base);
			} else if (dp_strcmpi(param_list[2], "step") == 0) {
				vrx318_w32(PP32_CTRL_CMD_STEP, PP32_CTRL_CMD(pp32), base);
			} else if ((dp_strcmpi(param_list[2], "pc0") == 0)
					|| (dp_strcmpi(param_list[0], "pc1") == 0)) {
				if (dp_strcmpi(param_list[2], "pc0") == 0)
					pc = 0;
				else
					pc = 1;

				if (dp_strcmpi(param_list[3], "off") == 0) {
					vrx318_w32(PP32_BRK_GRPi_PCn_OFF(pc, pc), PP32_BRK_TRIG(pp32), base);
					vrx318_w32(PP32_BRK_CONTEXT_MASK_EN,
						PP32_BRK_PC_MASK(pp32, pc), base);
					vrx318_w32(0, PP32_BRK_PC(pp32, pc), base);
				} else {
					addr = dp_atoi(param_list[3]);
					vrx318_w32(addr, PP32_BRK_PC(pp32, pc), base);
					vrx318_w32(PP32_BRK_CONTEXT_MASK_EN | PP32_BRK_CONTEXT_MASK(0) |
						PP32_BRK_CONTEXT_MASK(1) | PP32_BRK_CONTEXT_MASK(2) | PP32_BRK_CONTEXT_MASK(3),
						PP32_BRK_PC_MASK(pp32, pc), base);
					vrx318_w32(PP32_BRK_GRPi_PCn_ON(pc, pc), PP32_BRK_TRIG(pp32), base);
				}
			} else if ((dp_strcmpi(param_list[2], "daddr0") == 0)
						|| (dp_strcmpi(param_list[2], "daddr1") == 0)) {
				if (dp_strcmpi(param_list[2], "daddr0") == 0)
						pc = 0;
				else
						pc = 1;

				if (dp_strcmpi(param_list[3], "off") == 0) {
					vrx318_w32(PP32_BRK_GRPi_DATA_ADDRn_OFF(pc, pc), PP32_BRK_TRIG(pp32), base);
					vrx318_w32(PP32_BRK_CONTEXT_MASK_EN, PP32_BRK_DATA_ADDR_MASK(pp32, pc), base);
					vrx318_w32(0, PP32_BRK_DATA_ADDR(pp32, pc), base);

				} else {
					addr = dp_atoi(param_list[3]);
					vrx318_w32(addr, PP32_BRK_DATA_ADDR(pp32, pc), base);
					vrx318_w32(PP32_BRK_CONTEXT_MASK_EN | PP32_BRK_CONTEXT_MASK(0) |
						PP32_BRK_CONTEXT_MASK(1) | PP32_BRK_CONTEXT_MASK(2) | PP32_BRK_CONTEXT_MASK(3),
						PP32_BRK_DATA_ADDR_MASK(pp32, pc), base);
					vrx318_w32(PP32_BRK_GRPi_DATA_ADDRn_ON(pc, pc), PP32_BRK_TRIG(pp32), base);
				}
			}
		} else {
			goto proc_pp32_help;
		}
		if (vrx318_r32(PP32_BRK_TRIG(pp32), base))
			vrx318_w32(PP32_CTRL_OPT_STOP_ON_BREAKPOINT_ON,
				PP32_CTRL_OPT(pp32), base);
		else
			vrx318_w32(PP32_CTRL_OPT_STOP_ON_BREAKPOINT_OFF,
				PP32_CTRL_OPT(pp32), base);
		return count;

proc_pp32_help:
	vrx318_dbg(DBG_INFO, "echo pp32 <id> freeze/start/stop/step/restart > pp32\n");
	vrx318_dbg(DBG_INFO, "echo pp32 <id> pc0 off/[addr] > pp32\n");
	vrx318_dbg(DBG_INFO, "echo pp32 <id> daddr0 off/[addr] > pp32\n");
	return count;
}
static int proc_vrx318_read_pp32(struct seq_file *seq, void *v)
{
	struct vrx318_atm_tc *priv = (struct vrx318_atm_tc *)seq->private;

	static const char *stron = " on";
	static const char *stroff = "off";

	int cur_context;
	int f_stopped;
	char strlength;
	int i, j;

	int pp32, ep_id;
	u32 base;

	if (!priv) {
		pr_err("priv pointer is NULL!!!\n");
		return -EINVAL;
	}

	ep_id = priv->ep_id;
	base = (u32)priv->frwk->ep_dev[ep_id].membase;

	for (pp32 = 0; pp32 < 2; pp32++) {
		f_stopped = 0;
		seq_printf(seq, "===== pp32 core %d =====\n", pp32);

		cur_context = PP32_BRK_CUR_CONTEXT(base, pp32);
		seq_printf(seq, "Context: %d, PC: 0x%04x ", cur_context, PP32_CPU_CUR_PC(base, pp32));
		if ((vrx318_r32(PP32_FREEZE, base) & (1 << (pp32 << 4))) != 0) {
			seq_puts(seq, "freezed");
			f_stopped = 1;
		} else if (PP32_CPU_USER_STOPPED(base, pp32) || PP32_CPU_USER_BREAKIN_RCV(base, pp32) || PP32_CPU_USER_BREAKPOINT_MET(base, pp32)) {
			strlength = 0;
			f_stopped = 1;
			if (PP32_CPU_USER_STOPPED(base, pp32)) {
				seq_puts(seq, "stopped");
				strlength = 1;
			}
			if (PP32_CPU_USER_BREAKPOINT_MET(base, pp32)) {
				seq_printf(seq, "%s", strlength ? " | breakpoint" : "breakpoint");
				strlength = 1;
			}
			if (PP32_CPU_USER_BREAKIN_RCV(base, pp32))
				seq_printf(seq, "%s", strlength ? " | breakin" : "breakin");

		} else if (PP32_CPU_CUR_PC(base, pp32) == PP32_CPU_CUR_PC(base, pp32)) {
			unsigned int pc_value[64] = {0};
			f_stopped = 1;
			for (i = 0; f_stopped && i < ARRAY_SIZE(pc_value); i++) {
				pc_value[i] = PP32_CPU_CUR_PC(base, pp32);
				for (j = 0; j < i; j++)
					if (pc_value[j] != pc_value[i]) {
						f_stopped = 0;
						break;
					}
				}
				if (f_stopped)
					seq_puts(seq, "hang");
		}
		if (!f_stopped)
			seq_puts(seq, "running");

		seq_puts(seq, "\n");

		strlength = 0;
		if (PP32_CPU_USER_BREAKPOINT_MET(base, pp32)) {
			seq_puts(seq, "break reason: ");
			if (PP32_BRK_PC_MET(base, pp32, 0)) {
				seq_puts(seq, "pc0");
				strlength = 1;
			}
			if (PP32_BRK_PC_MET(base, pp32, 1)) {
				seq_printf(seq, "%s", strlength ? " | pc1" : "pc1");
				strlength = 1;
			}
			if (PP32_BRK_DATA_ADDR_MET(base, pp32, 0)) {
				seq_printf(seq, "%s", strlength ? " | daddr0" : "daddr0");
				strlength = 1;
			}
			if (PP32_BRK_DATA_ADDR_MET(base, pp32, 1)) {
				seq_printf(seq, "%s", strlength ? " | daddr1" : "daddr1");
				strlength = 1;
			}
			if (PP32_BRK_DATA_VALUE_RD_MET(base, pp32, 0)) {
				seq_printf(seq, "%s", strlength ? " | rdval0" : "rdval0");
				strlength = 1;
				if (PP32_BRK_DATA_VALUE_RD_LO_EQ(base, pp32, 0)) {
					if (PP32_BRK_DATA_VALUE_RD_GT_EQ(base, pp32, 0))
						seq_puts(seq, " ==");
					else
						seq_puts(seq, " <=");
				} else if (PP32_BRK_DATA_VALUE_RD_GT_EQ(base, pp32, 0))
					seq_puts(seq, " >=");
			}
			if (PP32_BRK_DATA_VALUE_RD_MET(base, pp32, 1)) {
				seq_printf(seq, "%s", strlength ? " | rdval1" : "rdval1");
				strlength = 1;
				if (PP32_BRK_DATA_VALUE_RD_LO_EQ(base, pp32, 1)) {
					if (PP32_BRK_DATA_VALUE_RD_GT_EQ(base, pp32, 1))
						seq_puts(seq, " ==");
					else
						seq_puts(seq, " <=");
				} else if (PP32_BRK_DATA_VALUE_RD_GT_EQ(base, pp32, 1))
					seq_puts(seq, " >=");
			}
			if (PP32_BRK_DATA_VALUE_WR_MET(base, pp32, 0)) {
				seq_printf(seq, "%s", strlength ? " | wtval0" : "wtval0");
				strlength = 1;
				if (PP32_BRK_DATA_VALUE_WR_LO_EQ(base, pp32, 0)) {
					if (PP32_BRK_DATA_VALUE_WR_GT_EQ(base, pp32, 0))
						seq_puts(seq, " ==");
					else
						seq_puts(seq, " <=");
				} else if (PP32_BRK_DATA_VALUE_WR_GT_EQ(base, pp32, 0))
					seq_puts(seq, " >=");
			}
			if (PP32_BRK_DATA_VALUE_WR_MET(base, pp32, 1)) {
				seq_printf(seq, "%s", strlength ? " | wtval1" : "wtval1");
				strlength = 1;
				if (PP32_BRK_DATA_VALUE_WR_LO_EQ(base, pp32, 1)) {
					if (PP32_BRK_DATA_VALUE_WR_GT_EQ(base, pp32, 1))
						seq_puts(seq, " ==");
					else
						seq_puts(seq, " <=");
				} else if (PP32_BRK_DATA_VALUE_WR_GT_EQ(base, pp32, 1))
					seq_puts(seq, " >=");
			}
			seq_puts(seq, "\n");
		}

		if (f_stopped) {
			seq_printf(seq, "General Purpose Register (Context %d):\n", cur_context);
			for (i = 0; i < 4; i++) {
				for (j = 0; j < 4; j++)
					seq_printf(seq, "   %2d: %08x", i + j * 4, PP32_GP_CONTEXTi_REGn(base, pp32, cur_context, i + j * 4));
				seq_puts(seq, "\n");
			}
		}

		seq_printf(seq, "break out on: break in - %s, stop - %s\n",
			PP32_CTRL_OPT_BREAKOUT_ON_BREAKIN(base, pp32) ? stron : stroff,
			PP32_CTRL_OPT_BREAKOUT_ON_STOP(base, pp32) ? stron : stroff);
		seq_printf(seq, "     stop on: break in - %s, break point - %s\n",
			PP32_CTRL_OPT_STOP_ON_BREAKIN(base, pp32) ? stron : stroff,
			PP32_CTRL_OPT_STOP_ON_BREAKPOINT(base, pp32) ? stron : stroff);
		seq_puts(seq, "breakpoint:\n");
		seq_printf(seq, "     pc0: 0x%08x, %s\n", vrx318_r32(PP32_BRK_PC(pp32, 0), base), PP32_BRK_GRPi_PCn(base, pp32, 0, 0) ? "group 0" : "off");
		seq_printf(seq, "     pc1: 0x%08x, %s\n", vrx318_r32(PP32_BRK_PC(pp32, 1), base), PP32_BRK_GRPi_PCn(base, pp32, 1, 1) ? "group 1" : "off");
		seq_printf(seq, "  daddr0: 0x%08x, %s\n", vrx318_r32(PP32_BRK_DATA_ADDR(pp32, 0), base), PP32_BRK_GRPi_DATA_ADDRn(base, pp32, 0, 0) ? "group 0" : "off");
		seq_printf(seq, "  daddr1: 0x%08x, %s\n", vrx318_r32(PP32_BRK_DATA_ADDR(pp32, 1), base), PP32_BRK_GRPi_DATA_ADDRn(base, pp32, 1, 1) ? "group 1" : "off");
		seq_printf(seq, "  rdval0: 0x%08x\n", vrx318_r32(PP32_BRK_DATA_VALUE_RD(pp32, 0), base));
		seq_printf(seq, "  rdval1: 0x%08x\n", vrx318_r32(PP32_BRK_DATA_VALUE_RD(pp32, 1), base));
		seq_printf(seq, "  wrval0: 0x%08x\n", vrx318_r32(PP32_BRK_DATA_VALUE_WR(pp32, 0), base));
		seq_printf(seq, "  wrval1: 0x%08x\n", vrx318_r32(PP32_BRK_DATA_VALUE_WR(pp32, 1), base));
	}

	return 0;
}

static int proc_read_pp32_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_vrx318_read_pp32, PDE_DATA(inode));
}

static const struct file_operations vrx318_pp32_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_pp32_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= vrx318_pp32_proc_write,
};

void print_stat_mib(struct seq_file *seq, struct atm_stats *stat)
{
	seq_printf(seq, "AAL5 RX PKTs:\t\t %u\n", stat->aal5_rx_pkts);
	seq_printf(seq, "AAL5 RX Bytes:\t\t %u\n", stat->aal5_rx_bytes);
	seq_printf(seq, "AAL5 RX Err:\t\t %u\n", stat->aal5_rx_errors);
	seq_printf(seq, "AAL5 RX Drop:\t\t %u\n", stat->aal5_rx_dropped);

	seq_printf(seq, "OAM  RX PKTs:\t\t %u\n", stat->oam_rx_pkts);
	seq_printf(seq, "OAM  RX Bytes:\t\t %u\n", stat->oam_rx_bytes);
	seq_printf(seq, "OAM  RX Err:\t\t %u\n", stat->oam_rx_errors);
	seq_printf(seq, "OAM  RX Drop:\t\t %u\n", stat->oam_rx_dropped);

	seq_printf(seq, "AAL5 TX PKTs:\t\t %u\n", stat->aal5_tx_pkts);
	seq_printf(seq, "AAL5 TX Bytes:\t\t %u\n", stat->aal5_tx_bytes);
	seq_printf(seq, "AAL5 TX Err:\t\t %u\n", stat->aal5_tx_errors);
	seq_printf(seq, "AAL5 TX Drop:\t\t %u\n", stat->aal5_tx_dropped);

	seq_printf(seq, "OAM  TX PKTs:\t\t %u\n", stat->oam_tx_pkts);
	seq_printf(seq, "OAM  TX Bytes:\t\t %u\n", stat->oam_tx_bytes);
	seq_printf(seq, "OAM  TX Err:\t\t %u\n", stat->oam_tx_errors);
	seq_printf(seq, "OAM  TX Drop:\t\t %u\n\n", stat->oam_tx_dropped);
}

void print_drv_mib(struct seq_file *seq, struct vrx318_atm_tc *priv)
{
	seq_puts(seq, "Driver Total MIB:\n");
	print_stat_mib(seq, &priv->stats);
}
static int proc_read_atm_wanmib_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_atm_wanmib, PDE_DATA(inode));
}

static const struct file_operations vrx318_wanmib_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_atm_wanmib_seq_open,
	.read		= seq_read,
	.write		= proc_write_atm_wanmib,
	.llseek		= seq_lseek,
	.release	= single_release,
};
static int proc_read_htu_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_htu, PDE_DATA(inode));
}

static const struct file_operations vrx318_htu_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_htu_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
static int proc_read_queue_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_queue, PDE_DATA(inode));
}

static const struct file_operations vrx318_queue_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_queue_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int proc_atm_read_ver(struct seq_file *seq, void *v)
{
	struct vrx318_atm_tc *priv = (struct vrx318_atm_tc *)seq->private;
	char buf[128];

	print_fw_version(buf, &priv->fw_ver);
	seq_printf(seq, "%s", buf);
	return 0;
}

static int proc_atm_read_ver_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_atm_read_ver, PDE_DATA(inode));
}

static const struct file_operations vrx318_atm_ver_proc_fops = {
	.owner      = THIS_MODULE,
	.open       = proc_atm_read_ver_seq_open,
	.read       = seq_read,
	.llseek     = seq_lseek,
	.release    = single_release,
};


static void set_q_prio(struct vrx318_atm_tc *priv, int conn, int prio, int queue)
{
	if (conn < 0 || conn >= ATM_PVC_NUMBER || prio < 0 || prio >= ATM_PRIO_Q_NUM) {
		vrx318_dbg(DBG_ERR, "%s: invalid parameter: conn: %d, prio: %d\n",
			__func__, conn, prio);
		return;
	}

	vrx318_dbg(DBG_INFO, "conn: %d, prio: %d, queue: %d\n", conn, prio, queue);
	spin_lock_bh(&priv->atm_lock);
	if (priv->conn[conn].sw_txq_tbl & BIT(queue))
		priv->conn[conn].prio_queue_map[prio] = queue;
	spin_unlock_bh(&priv->atm_lock);
}
ssize_t proc_atm_write_prio(struct file *file, const char __user *buf,
				size_t count, loff_t *data)
{
	struct vrx318_atm_tc *priv;
	char local_buf[256];
	int len;
	char *p1;
	int conn = -1;
	unsigned int vpi, vci;
	int prio = -1;
	int queue = -1;
	int ret, num;
	int i;
	char *param_list[64];

	len = sizeof(local_buf) < count ? sizeof(local_buf) - 1 : count;
	len = len - copy_from_user(local_buf, buf, len);
	local_buf[len] = 0;
	p1 = local_buf;

	num = dp_split_buffer(local_buf, param_list, ARRAY_SIZE(param_list));
	priv =  (struct vrx318_atm_tc *)PDE_DATA(file_inode(file));
	if (dp_strcmpi(param_list[0], "help") == 0)
		goto proc_atm_prio_help;
	else if (dp_strcmpi(param_list[0], "pvc") == 0) {
		p1 += 4;
		len -= 4;
		ignore_space(&p1, &len);
		vpi = get_num_from_str(&p1, &len, 0);
		ignore_space(&p1, &len);
		vci = get_num_from_str(&p1, &len, 0);
		conn = find_vpivci(vpi, vci);

		if (conn < 0) {
			vrx318_dbg(DBG_ERR, "PVC(%d:%d) is invalid\n", vpi, vci);
			return count;
		}
		if (dp_strcmpi(param_list[2], "add") == 0) {
			ret = sw_tx_queue_add(priv, conn);
			if (ret < 0)
				vrx318_dbg(DBG_ERR, "Failed in adding queue for PVC(%d:%d)\n", vpi, vci);
			else
				vrx318_dbg(DBG_INFO, "Add queue for PVC(%d:%d) successfully\n", vpi, vci);
		} else if (dp_strcmpi(param_list[2], "del") == 0 || dp_strcmpi(param_list[2], "rem") == 0) {
			ret = sw_tx_queue_del(priv, conn);
			if (ret < 0)
				vrx318_dbg(DBG_ERR, "Failed in deleting queue for PVC(%d:%d)\n", vpi, vci);
			else
				vrx318_dbg(DBG_INFO, "Delete queue for PVC(%d:%d) successfully\n", vpi, vci);
		} else {
			prio = queue = -1;
			for (i = 2; i < num; i += 4) {
				if ((dp_strcmpi(param_list[i], "prio") == 0)
					|| (dp_strcmpi(param_list[i], "p") == 0))
					prio = dp_atoi(param_list[i+1]);
				else
					vrx318_dbg(DBG_ERR, "Error: expecting prio\n");

				if ((dp_strcmpi(param_list[i+2], "queue") == 0) ||
					(dp_strcmpi(param_list[i+2], "q") == 0))
					queue = dp_atoi(param_list[i+3]);
				else
					vrx318_dbg(DBG_ERR, "Error: expecting queue\n");

				if (prio >= 0 && queue >= 0)
					set_q_prio(priv, conn, prio, queue);
			}
		}
	} else
		vrx318_dbg(DBG_INFO, "Wrong format!, Please try echo help > prio\n");

	return count;

proc_atm_prio_help:
	vrx318_dbg(DBG_INFO, "echo pvc vpi.vci prio xx queue xx [prio xx queue xx] > prio\n");
	vrx318_dbg(DBG_INFO, "echo pvc vpi.vci <add/del> > prio\n");
	return count;
}
static int proc_atm_read_prio(struct seq_file *seq, void *v)
{
	int i, j;
	struct vrx318_atm_tc *priv = (struct vrx318_atm_tc *)seq->private;
	struct atm_pvc *conn;

	for (i = 0; i < ATM_PVC_NUMBER; i++) {
		if (priv->pvc_tbl & BIT(i)) {
			conn = &priv->conn[i];
			seq_printf(seq, "PVC(%d:%d): Phy_Qid: ",
				conn->vcc->vpi,
				conn->vcc->vci);
			for (j = 0; j < ATM_PVC_NUMBER; j++) {
				if (conn->sw_txq_tbl & BIT(j))
					seq_printf(seq, "%d ", j);
			}
			seq_puts(seq, "\n   Prio: ");
			for (j = 0; j < ATM_PRIO_Q_NUM; j++)
				seq_printf(seq, "%d ", j);
			seq_puts(seq, "\n   Qid:  ");
			for (j = 0; j < ATM_PRIO_Q_NUM; j++)
				seq_printf(seq, "%d ",
					conn->prio_queue_map[j]);
			seq_puts(seq, "\n");
		}
	}
	return 0;
}

static int proc_atm_read_prio_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_atm_read_prio, PDE_DATA(inode));
}

static const struct file_operations vrx318_atm_prio_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_atm_read_prio_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.write		= proc_atm_write_prio,
	.release	= single_release,
};

static int pvc_mib_seq_show(struct seq_file *seq, void *v)
{
	struct atm_pvc *pvc = (struct atm_pvc *)v;
	int j;
	struct vrx318_atm_tc *priv = seq->private;

	spin_lock_bh(&priv->atm_lock);
	if (pvc == NULL || pvc->vcc == NULL || pvc->dev == NULL) {
		spin_unlock_bh(&priv->atm_lock);
		return 0;
	}
	seq_printf(seq, "PVC %u/%u, dev: %s, MIB:\n",
		pvc->vcc->vpi, pvc->vcc->vci,
		(pvc->dev) ? pvc->dev->name : "N.A");
	spin_unlock_bh(&priv->atm_lock);

	print_stat_mib(seq, &pvc->stats);

	for (j = 0; j < ATM_PRIO_Q_NUM; j++) {
		if (pvc->prio_tx_packets[j] != 0) {
			seq_printf(seq, "Queue[%d]: TX PKTs:\t %u\n",
				j, pvc->prio_tx_packets[j]);
		}
	}
	seq_puts(seq, "\n");

	return 0;
}
static void *pvc_mib_seq_start(struct seq_file *seq, loff_t *pos)
{
	struct vrx318_atm_tc *priv = seq->private;

	if (!priv->pvc_tbl)
		return NULL;

	if (*pos >= ATM_PVC_NUMBER)
		return NULL;

	return &priv->conn[*pos];
}

static void *pvc_mib_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct vrx318_atm_tc *priv = seq->private;

	if (++*pos < ATM_PVC_NUMBER)
		return &priv->conn[*pos];

	return NULL;
}

static void pvc_mib_seq_stop(struct seq_file *seq, void *v)
{

}

static const struct seq_operations pvc_mib_seq_ops = {
	.start = pvc_mib_seq_start,
	.next = pvc_mib_seq_next,
	.stop = pvc_mib_seq_stop,
	.show = pvc_mib_seq_show,
};

static int proc_read_pvc_mib_seq_open(struct inode *inode, struct file *file)
{
	int ret = seq_open(file, &pvc_mib_seq_ops);
	if (ret == 0) {
		struct seq_file *m = file->private_data;
		m->private = PDE_DATA(inode);
	}
	return ret;
}

static const struct file_operations vrx318_atm_pvc_mib_proc_fops = {
	.owner	= THIS_MODULE,
	.open	= proc_read_pvc_mib_seq_open,
	.read	= seq_read,
	.llseek	= seq_lseek,
	.release = seq_release,
};

int vrx318_atm_proc_init(void *atm_priv)
{
	struct proc_dir_entry *entry;
	struct proc_dir_entry *vrx318_dir;
	struct vrx318_atm_tc *priv = (struct vrx318_atm_tc *)atm_priv;

	vrx318_dir = proc_mkdir("atm", priv->frwk->proc_dir);
	if (!vrx318_dir)
		return -ENOMEM;

	entry = proc_create_data("cfg", 0644,
			vrx318_dir, &vrx318_atm_cfg_proc_fops, (void *)priv);
	if (!entry)
		goto err1;

	entry = proc_create_data("mem", 0644,
			vrx318_dir, &vrx318_mem_proc_fops, (void *)priv);
	if (!entry)
		goto err2;

	entry = proc_create_data("pp32", 0644,
			vrx318_dir, &vrx318_pp32_proc_fops, (void *)priv);
	if (!entry)
		goto err3;

	entry = proc_create_data("wanmib", 0644,
			vrx318_dir, &vrx318_wanmib_proc_fops, (void *)priv);
	if (!entry)
		goto err4;

	entry = proc_create_data("htu", 0644,
			vrx318_dir, &vrx318_htu_proc_fops, (void *)priv);
	if (!entry)
		goto err5;

	entry = proc_create_data("queue", 0644,
			vrx318_dir, &vrx318_queue_proc_fops, (void *)priv);
	if (!entry)
		goto err6;

	entry = proc_create_data("ver", 0644,
			vrx318_dir, &vrx318_atm_ver_proc_fops, (void *)priv);
	if (!entry)
		goto err7;

	entry = proc_create_data("prio", 0644,
			vrx318_dir, &vrx318_atm_prio_proc_fops, (void *)priv);
	if (!entry)
		goto err8;

	entry = proc_create_data("pvc_mib", 0644,
			vrx318_dir, &vrx318_atm_pvc_mib_proc_fops,
			(void *)priv);
	if (!entry)
		goto err9;

	priv->proc_dir = vrx318_dir;

	return 0;

err9:
	remove_proc_entry("prio", vrx318_dir);
err8:
	remove_proc_entry("ver", vrx318_dir);
err7:
	remove_proc_entry("queue", vrx318_dir);
err6:
	remove_proc_entry("htu", vrx318_dir);
err5:
	remove_proc_entry("wanmib", vrx318_dir);
err4:
	remove_proc_entry("pp32", vrx318_dir);
err3:
	remove_proc_entry("mem", vrx318_dir);
err2:
	remove_proc_entry("cfg", vrx318_dir);
err1:
	remove_proc_entry("driver/ltq_vrx318/atm/", NULL);
	return -ENOMEM;
}
EXPORT_SYMBOL(vrx318_atm_proc_init);

void vrx318_atm_proc_exit(void *atm_priv)
{
	struct vrx318_atm_tc *priv = (struct vrx318_atm_tc *)atm_priv;
	remove_proc_entry("cfg", priv->proc_dir);
	remove_proc_entry("mem", priv->proc_dir);
	remove_proc_entry("pp32", priv->proc_dir);
	remove_proc_entry("wanmib", priv->proc_dir);
	remove_proc_entry("htu", priv->proc_dir);
	remove_proc_entry("queue", priv->proc_dir);
	remove_proc_entry("ver", priv->proc_dir);
	remove_proc_entry("prio", priv->proc_dir);
	remove_proc_entry("pvc_mib", priv->proc_dir);
	remove_proc_entry("atm", priv->frwk->proc_dir);
}
EXPORT_SYMBOL(vrx318_atm_proc_exit);


static int proc_read_vrx_cfg(struct seq_file *seq, void *v)
{
	struct vrx318_priv *priv = (struct vrx318_priv *)seq->private;
	const char *tc_mode_str[] = {
		"VRX318 Running",
		"VRX318 Init",
		"VRX318 NO_TC_Loaded",
		"VRX318 Switching",
		"VRX318 Exit"
	};

	seq_printf(seq, "VRX318 TC mode: %s\n",
		tc_mode_str[priv->tc_stat]);
	seq_printf(seq, "DFE loopback: %s\n",
		priv->param.dfe_loopback ? "Enable" : "Disable");
	seq_printf(seq, "P2P: %s\n",
		priv->param.p2p_en ? "Enable" : "Disable");
	seq_printf(seq, "CDMA_WRITE: %s\n",
		priv->param.cdma_wr_en ? "Enable" : "Disable");
	seq_printf(seq, "Link List in SB: %s\n",
		priv->param.lle_in_sb ? "Enable" : "Disable");
	seq_printf(seq, "PMAC Enable: %s\n",
		priv->param.pmac_en ? "Yes" : "No");
	seq_printf(seq, "ATM QOS number: %d\n", ATM_PVC_NUM(priv->param.atm_qos));
	seq_printf(seq, "DP ID: %d\n", priv->port_id);

	seq_puts(seq, "ATM parameters:\n");
	seq_printf(seq, "ATM OAM PRIO: %d\n", priv->param.oam_prio);
	seq_printf(seq, "ATM QSB_SRVM: %d\n", priv->param.qsb_srvm);
	seq_printf(seq, "ATM QSB_TAU: %d\n", priv->param.qsb_tau);
	seq_printf(seq, "ATM QSB_TSTEP: %d\n", priv->param.qsb_tstep);
	seq_printf(seq, "ATM AAL5 RX MAX PKTSZ: %d\n",
		priv->param.aal5r_max_pktsz);
	seq_printf(seq, "ATM AAL5 RX MAX PKTSZ: %d\n",
		priv->param.aal5r_min_pktsz);
	seq_printf(seq, "ATM AAL5 TX MAX PKTSZ: %d\n",
		priv->param.aal5s_max_pktsz);

	return 0;
}

static int proc_read_vrx_cfg_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_vrx_cfg, PDE_DATA(inode));
}

static const struct file_operations vrx318_cfg_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_vrx_cfg_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static ssize_t proc_write_dbg(struct file *file, const char __user *buf,
			size_t count, loff_t *data)
{
	char str[32];
	int len, rlen, i, j;
	int num, enable = 0;
	char *param_list[20];
	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;

	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));

	if ((dp_strcmpi(param_list[0], "enable") == 0)
		|| (dp_strcmpi(param_list[0], "en") == 0)) {
		enable = 1;
	} else if (dp_strcmpi(param_list[0], "disable") == 0
		|| (dp_strcmpi(param_list[0], "dis") == 0)) {
		enable = -1;
	} else {
		goto proc_dbg_help;
	}
	if (!param_list[1]) {
		set_vrx_dbg_flag(g_vrx318_dbg, enable, -1);
		return count;
	}
	for (i = 1; i < num; i++) {
		for (j = 0; j < ARRAY_SIZE(vrx_dbg_flag_str); j++) {
			if (dp_strcmpi(param_list[i], vrx_dbg_flag_str[j]) == 0) {
				set_vrx_dbg_flag(g_vrx318_dbg, enable, BIT(j));
				break;
			}
		}
	}
	vrx318_dbg(DBG_INFO, "dbg write:%d\n", g_vrx318_dbg);

	return count;

proc_dbg_help:
	vrx318_dbg(DBG_INFO, "echo enable err/rx/tx/event/rx_dump/tx_dump/init/info/oam_rx/oam_tx/oam_rx_dump/oam_tx_dump/qos/tc/loopback > dbg\n");
	vrx318_dbg(DBG_INFO, "echo disable err/rx/tx/event/rx_dump/tx_dump/init/info/oam_rx/oam_tx/oam_rx_dump/oam_tx_dump/qos/tc/loopback > dbg\n");
	return count;
}

static int proc_read_dbg(struct seq_file *seq, void *v)
{
	int i;
	seq_printf(seq, "Dbg Enable:0x%08x\n", g_vrx318_dbg);
	for (i = 0; i < ARRAY_SIZE(vrx_dbg_flag_str); i++) {
		if ((g_vrx318_dbg & BIT(i)) == BIT(i))
			seq_printf(seq, "%s ", vrx_dbg_flag_str[i]);
	}
	seq_puts(seq, "\n");
	return 0;
}

static int proc_read_dbg_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_dbg, PDE_DATA(inode));
}

static const struct file_operations vrx318_dbg_proc_fops = {
	.owner      = THIS_MODULE,
	.open       = proc_read_dbg_seq_open,
	.read       = seq_read,
	.write      = proc_write_dbg,
	.llseek     = seq_lseek,
	.release    = single_release,
};

static ssize_t proc_write_tc_switch(struct file *file, const char __user *buf,
			size_t count, loff_t *data)
{
	char str[32];
	int len, rlen;
	int num, epid;
	char *param_list[20];
	struct vrx318_priv *priv;

	priv = (struct vrx318_priv *)PDE_DATA(file_inode(file));

	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));
	if (dp_strcmpi(param_list[0], "unload") == 0) {
		spin_lock_bh(&priv->fw_lock);
		priv->tc_stat = VRX318_SWITCHING;
		spin_unlock_bh(&priv->fw_lock);
		epid = dp_atoi(param_list[1]);
		if (epid < priv->ep_num)
			vrx318_unload_tc(priv, epid);
		else {
			vrx318_dbg(DBG_ERR, "epid is not correct\n");
			return count;
		}
		priv->tc_mode[0] = TC_NONE_MODE;
		spin_lock_bh(&priv->fw_lock);
		priv->tc_stat = VRX318_NO_TC;
		spin_unlock_bh(&priv->fw_lock);
	} else if (dp_strcmpi(param_list[0], "load") == 0) {
		if (priv->tc_stat != VRX318_NO_TC) {
			vrx318_dbg(DBG_ERR, "VRX318 Status is not correct\n");
			return count;
		}
		epid = dp_atoi(param_list[2]);
		if (epid > priv->ep_num - 1) {
			vrx318_dbg(DBG_ERR, "epid is not correct\n");
			return count;
		}
		if (dp_strcmpi(param_list[1], "atm") == 0) {
			vrx318_load_tc(priv, epid, TC_ATM_SL_MODE);
			priv->tc_mode[0] = TC_ATM_SL_MODE;
		} else if (dp_strcmpi(param_list[1], "ptm") == 0) {
			vrx318_load_tc(priv, epid, TC_PTM_SL_MODE);
			priv->tc_mode[0] = TC_PTM_SL_MODE;
		} else if (dp_strcmpi(param_list[1], "bond") == 0) {
			vrx318_load_tc(priv, epid, TC_PTM_BND_MODE);
			priv->tc_mode[0] = TC_PTM_BND_MODE;
		} else {
			vrx318_dbg(DBG_ERR, "mode is not correct\n");
			goto proc_tc_switch_help;
		}

		spin_lock_bh(&priv->fw_lock);
		priv->tc_stat = VRX318_RUNNING;
		spin_unlock_bh(&priv->fw_lock);
	} else
		goto proc_tc_switch_help;

	return count;

proc_tc_switch_help:
	vrx318_dbg(DBG_INFO, "echo load atm/ptm/bond [epid] > tc_switch\n");
	vrx318_dbg(DBG_INFO, "echo unload [epid] > tc_switch\n");
	return count;
}

static const struct file_operations vrx318_tc_switch_proc_fops = {
	.owner      = THIS_MODULE,
	.write      = proc_write_tc_switch,
	.llseek     = noop_llseek,
};
static ssize_t proc_write_show_time(struct file *file, const char __user *buf,
			size_t count, loff_t *data)
{
	char str[32];
	int len, rlen;
	int num, epid;
	char *param_list[20];
	struct vrx318_priv *priv;

	priv = (struct vrx318_priv *)PDE_DATA(file_inode(file));

	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));
	if (dp_strcmpi(param_list[1], "yes") == 0) {
		epid = dp_atoi(param_list[0]);
		if (epid > 1) {
			vrx318_dbg(DBG_ERR, "epid is not correct\n");
			return count;
			}
	priv->show_time_stat[epid] = 1;
	} else if (dp_strcmpi(param_list[1], "no") == 0) {
		epid = dp_atoi(param_list[0]);
		if (epid > 1) {
			vrx318_dbg(DBG_ERR, "epid is not correct\n");
			return count;
		}
	priv->show_time_stat[epid] = 0;
	} else {
		goto proc_show_time_help;
	}

	return count;

proc_show_time_help:
	vrx318_dbg(DBG_INFO, "echo [epid] yes/no > show_time\n");
	return count;
}

static const struct file_operations vrx318_show_time_proc_fops = {
	.owner      = THIS_MODULE,
	.write      = proc_write_show_time,
	.llseek     = noop_llseek,
};

static int proc_read_ver(struct seq_file *seq, void *v)
{
	char *drv_name, *drv_ver;

	get_vrx318_drv_ver(&drv_name, &drv_ver);
	seq_printf(seq, "%s: Version: %s\n", drv_name, drv_ver);

	return 0;
}

static int proc_read_ver_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_ver, PDE_DATA(inode));
}

static const struct file_operations vrx318_ver_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_ver_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

int vrx318_proc_init(struct vrx318_priv *priv)
{
	struct proc_dir_entry *entry;
	struct proc_dir_entry *vrx318_dir;

	vrx318_dir = proc_mkdir("driver/ltq_vrx318", NULL);
	if (!vrx318_dir)
		return -ENOMEM;

	entry = proc_create_data("cfg", 0644,
			vrx318_dir, &vrx318_cfg_proc_fops, (void *)priv);
	if (!entry)
		goto err1;

	entry = proc_create_data("dbg", 0644,
			vrx318_dir, &vrx318_dbg_proc_fops, (void *)priv);
	if (!entry)
		goto err2;

	entry = proc_create_data("tc_switch", 0200,
			vrx318_dir, &vrx318_tc_switch_proc_fops, (void *)priv);
	if (!entry)
		goto err3;

	entry = proc_create_data("show_time", 0200,
			vrx318_dir, &vrx318_show_time_proc_fops, (void *)priv);
	if (!entry)
		goto err4;

	entry = proc_create_data("ver", 0644,
			vrx318_dir, &vrx318_ver_proc_fops, (void *)priv);
	if (!entry)
		goto err5;

	priv->proc_dir = vrx318_dir;

	return 0;

err5:
	remove_proc_entry("show_time", vrx318_dir);
err4:
	remove_proc_entry("tc_swtich", vrx318_dir);
err3:
	remove_proc_entry("dbg", vrx318_dir);
err2:
	remove_proc_entry("cfg", vrx318_dir);
err1:
	remove_proc_entry("driver/ltq_vrx318", NULL);
	return -ENOMEM;
}

void vrx318_proc_exit(struct vrx318_priv *priv)
{
	remove_proc_entry("cfg", priv->proc_dir);
	remove_proc_entry("dbg", priv->proc_dir);
	remove_proc_entry("tc_switch", priv->proc_dir);
	remove_proc_entry("show_time", priv->proc_dir);
	remove_proc_entry("ver", priv->proc_dir);
	remove_proc_entry("driver/ltq_vrx318", NULL);
}

static int proc_read_ptm_wanmib_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_ptm_wanmib, PDE_DATA(inode));
}

static const struct file_operations vrx318_ptm_wanmib_proc_fops = {
	.owner      = THIS_MODULE,
	.open       = proc_read_ptm_wanmib_seq_open,
	.read       = seq_read,
	.write      = proc_write_ptm_wanmib,
	.llseek     = seq_lseek,
	.release    = single_release,
};

static int proc_ptm_read_ver(struct seq_file *seq, void *v)
{
	struct vrx318_ptm_tc *priv = (struct vrx318_ptm_tc *)seq->private;
	int i, j;
	char buf[128];

	if (priv->tc_mode == TC_PTM_BND_MODE) {
		for (i = 0; i < MAX_VRX318_NUM; i++) {
			for (j = 0; j < 2; j++) {
				print_fw_version(buf, &priv->fw_ver[i][j]);
				seq_printf(seq, "%s", buf);
			}
		}
	} else {
		for (j = 0; j < 2; j++) {
			print_fw_version(buf, &priv->fw_ver[priv->ep_id][j]);
			seq_printf(seq, "%s", buf);
		}
	}
	return 0;
}

static int proc_ptm_read_ver_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_ptm_read_ver, PDE_DATA(inode));
}

static const struct file_operations vrx318_ptm_ver_proc_fops = {
	.owner      = THIS_MODULE,
	.open       = proc_ptm_read_ver_seq_open,
	.read       = seq_read,
	.llseek     = seq_lseek,
	.release    = single_release,
};

static int proc_ptm_read_cfg(struct seq_file *seq, void *v)
{
	struct vrx318_ptm_tc *priv = (struct vrx318_ptm_tc *)seq->private;

	seq_printf(seq, "PTM mode: %s\n",
		(priv->tc_mode == TC_PTM_BND_MODE) ?
		"PTM Bonding mode" : "PTM Single Line mode");
	if (priv->tc_mode != TC_PTM_BND_MODE)
		seq_printf(seq, "VRX318 on line: %d\n", priv->ep_id);
	seq_printf(seq, "Showtime: %d\n", ptm_in_showtime());
	seq_printf(seq, "Power Saving: %s\n",
		ptm_power_saving(priv, 0, 0) == 1 ? "Enable" : "Disable");
	return 0;
}

static int proc_read_cfg_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_ptm_read_cfg, PDE_DATA(inode));
}

static ssize_t vrx318_ptm_cfg_proc_write(struct file *file,
			const char __user *buf, size_t count, loff_t *data)
{
	struct vrx318_ptm_tc *priv;
	u32 base, ep_id, en;
	int num, len, rlen;
	char *param_list[32];
	char str[128];

	priv = (struct vrx318_ptm_tc *)PDE_DATA(file_inode(file));
	ep_id = priv->ep_id;
	base = (u32)priv->frwk->ep_dev[ep_id].membase;

	if (!access_ok(VERIFY_READ, buf, count))
		return -EFAULT;

	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));

	if (dp_strcmpi(param_list[0], "en") == 0
		|| dp_strcmpi(param_list[0], "enble") == 0) {
		en = 1;
	} else if (dp_strcmpi(param_list[0], "dis") == 0
		|| dp_strcmpi(param_list[0], "disable") == 0) {
		en = 0;
	} else
		goto proc_ptm_cfg_help;

	if (dp_strcmpi(param_list[1], "ps") == 0
		|| dp_strcmpi(param_list[1], "powersaving") == 0) {
		ptm_power_saving(priv, en, 1);
	}

	return count;

proc_ptm_cfg_help:
	vrx318_dbg(DBG_INFO, "echo [enable | disable] powersaving > cfg\n");
	return count;
}


static const struct file_operations vrx318_ptm_cfg_proc_fops = {
	.owner = THIS_MODULE,
	.open = proc_read_cfg_seq_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.write = vrx318_ptm_cfg_proc_write,
	.release = single_release,
};

ssize_t proc_ptm_write_prio(struct file *file, const char __user *buf,
				size_t count, loff_t *data)
{
	struct vrx318_ptm_tc *priv;
	char local_buf[256];
	int len;
	char *p1;
	int prio = -1;
	int queue = -1;
	int num, qnum;
	int i;
	char *param_list[64];

	len = sizeof(local_buf) < count ? sizeof(local_buf) - 1 : count;
	len = len - copy_from_user(local_buf, buf, len);
	local_buf[len] = 0;
	p1 = local_buf;
	qnum = ptm_qos_qnum();

	num = dp_split_buffer(local_buf, param_list, ARRAY_SIZE(param_list));
	priv =  (struct vrx318_ptm_tc *)PDE_DATA(file_inode(file));
	if (dp_strcmpi(param_list[0], "help") == 0)
		goto proc_ptm_prio_help;
	else {
		for (i = 0; i < num; i += 4) {
			if ((dp_strcmpi(param_list[i], "prio") == 0)
				|| (dp_strcmpi(param_list[i], "p") == 0))
				prio = dp_atoi(param_list[i+1]);
			else
				vrx318_dbg(DBG_ERR, "Error: expecting prio\n");

			if ((dp_strcmpi(param_list[i+2], "queue") == 0) ||
				(dp_strcmpi(param_list[i+2], "q") == 0))
				queue = dp_atoi(param_list[i+3]);
			else
				vrx318_dbg(DBG_ERR, "Error: expecting queue\n");

			if (queue < 0 || queue >= qnum
				|| prio < 0 || prio >= qnum) {
				vrx318_dbg(DBG_ERR, "%s: invalid parameter: prio: %d, queue: %d\n",
					__func__, prio, queue);
				goto proc_ptm_prio_help;
			} else {
				vrx318_dbg(DBG_INFO, "prio: %d, queue: %d\n", prio, queue);
				priv->ptm_prio_queue_map[prio] = queue;
			}
		}
	}
	return count;

proc_ptm_prio_help:
	vrx318_dbg(DBG_INFO, "echo prio xx queue xx [prio xx queue xx] > prio\n");
	return count;
}

static int proc_ptm_read_prio(struct seq_file *seq, void *v)
{
	int i, qnum;
	struct vrx318_ptm_tc *priv = (struct vrx318_ptm_tc *)seq->private;
	qnum = ptm_qos_qnum();

	seq_puts(seq, "\n   Prio: ");
	for (i = 0; i < qnum; i++)
		seq_printf(seq, "%d ", i);
		seq_puts(seq, "\n   Qid:  ");
	for (i = 0; i < qnum; i++)
		seq_printf(seq, "%d ",
			priv->ptm_prio_queue_map[i]);
		seq_puts(seq, "\n");
	return 0;
}

static int proc_ptm_read_prio_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_ptm_read_prio, PDE_DATA(inode));
}

static const struct file_operations vrx318_ptm_prio_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_ptm_read_prio_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.write		= proc_ptm_write_prio,
	.release	= single_release,
};

ssize_t proc_ptm_write_qos(struct file *file, const char __user *buf,
				size_t count, loff_t *data)
{
	struct vrx318_ptm_tc *priv;
	char local_buf[256];
	int len;
	char *p1;
	int num, idx, qnum;
	u32 base, addr;
	char *param_list[64];

	qnum = ptm_qos_qnum();

	len = sizeof(local_buf) < count ? sizeof(local_buf) - 1 : count;
	len = len - copy_from_user(local_buf, buf, len);
	local_buf[len] = 0;
	p1 = local_buf;
	
	num = dp_split_buffer(local_buf, param_list, ARRAY_SIZE(param_list));
	priv =  (struct vrx318_ptm_tc *)PDE_DATA(file_inode(file));
	idx = priv->ep_id;
	base = (u32)priv->frwk->ep_dev[idx].membase;

	if ((dp_strcmpi(param_list[0], "clear") == 0)
		|| (dp_strcmpi(param_list[0], "clean") == 0)
		|| (dp_strcmpi(param_list[0], "c") == 0)) {
	}

	addr = ptm_qos_mib_base();
	if (dp_strcmpi(param_list[0], "help") == 0)
		goto proc_ptm_qos_help;
	else
		dword_ep_clear(SB_BUFFER(addr), sizeof(qosq_mib_t) * qnum, base);

	return count;

proc_ptm_qos_help:
	vrx318_dbg(DBG_INFO, "echo clear > qos\n");
	return count;
}

static int proc_ptm_read_qos(struct seq_file *seq, void *v)
{
	int i, qnum;
	struct vrx318_ptm_tc *priv = (struct vrx318_ptm_tc *)seq->private;
	u32 base = (u32)priv->frwk->ep_dev[priv->ep_id].membase;
	qosq_mib_t mib;
	u32 addr;

	qnum = ptm_qos_qnum();
	addr = ptm_qos_mib_base();

	seq_printf(seq, "\nTotal QoS Qnum: %d\n", qnum);
	seq_puts(seq, "\t rx_pkt/rx_bytes \t tx_pkt/tx_bytes \t small_drop[cnt/bytes] \t large_drop[cnt/bytes] \t address\n");
	for (i = 0; i < qnum; i++) {
		dword_mem_read(&mib,
			SB_BUFFER(addr + (i * sizeof(mib) / 4)),
			sizeof(mib), base);
		seq_printf(seq, "Q[%d]:\t %010u/%010u \t %010u/%010u \t %010u/%010u \t %010u/%010u \t @0x%4x\n",
			i, mib.rx_pkt_cnt, mib.rx_byte_cnt,
			mib.tx_pkt_cnt, mib.tx_byte_cnt,
			mib.small_pkt_drop_cnt, mib.small_pkt_drop_byte_cnt,
			mib.large_pkt_drop_cnt, mib.large_pkt_drop_byte_cnt,
			addr + (i * sizeof(mib) / 4));
	}

	return 0;
}

static int proc_ptm_read_qos_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_ptm_read_qos, PDE_DATA(inode));
}

static const struct file_operations vrx318_ptm_qos_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_ptm_read_qos_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.write		= proc_ptm_write_qos,
	.release	= single_release,
};

int vrx318_ptm_proc_init(void *ptm_priv)
{
	struct proc_dir_entry *entry;
	struct proc_dir_entry *vrx318_dir;
	struct vrx318_ptm_tc *priv = (struct vrx318_ptm_tc *)ptm_priv;

	vrx318_dir = proc_mkdir("ptm", priv->frwk->proc_dir);
	if (!vrx318_dir)
		return -ENOMEM;

	entry = proc_create_data("mem", 0644,
			vrx318_dir, &vrx318_mem_proc_fops, (void *)priv);
	if (!entry)
		goto err1;

	entry = proc_create_data("pp32", 0644,
			vrx318_dir, &vrx318_pp32_proc_fops, (void *)priv);
	if (!entry)
		goto err2;

	entry = proc_create_data("wanmib", 0644,
			vrx318_dir, &vrx318_ptm_wanmib_proc_fops, (void *)priv);
	if (!entry)
		goto err3;

	entry = proc_create_data("ver", 0644,
			vrx318_dir, &vrx318_ptm_ver_proc_fops, (void *)priv);
	if (!entry)
		goto err4;

	entry = proc_create_data("cfg", 0644,
			vrx318_dir, &vrx318_ptm_cfg_proc_fops, (void *)priv);
	if (!entry)
		goto err5;

	entry = proc_create_data("prio", 0644,
			vrx318_dir, &vrx318_ptm_prio_proc_fops, (void *)priv);
	if (!entry)
		goto err6;

	entry = proc_create_data("qos", 0644,
			vrx318_dir, &vrx318_ptm_qos_proc_fops, (void *)priv);
	if (!entry)
		goto err7;

	priv->proc_dir = vrx318_dir;
	return 0;

err7:
	remove_proc_entry("prio", vrx318_dir);
err6:
	remove_proc_entry("cfg", vrx318_dir);
err5:
	remove_proc_entry("ver", vrx318_dir);
err4:
	remove_proc_entry("wanmib", vrx318_dir);
err3:
	remove_proc_entry("pp32", vrx318_dir);
err2:
	remove_proc_entry("mem", vrx318_dir);
err1:
	remove_proc_entry("driver/ltq_vrx318/ptm/", NULL);
	return -ENOMEM;
}
EXPORT_SYMBOL(vrx318_ptm_proc_init);

void vrx318_ptm_proc_exit(void *ptm_priv)
{
	struct vrx318_ptm_tc *priv = (struct vrx318_ptm_tc *)ptm_priv;
	remove_proc_entry("cfg", priv->proc_dir);
	remove_proc_entry("ver", priv->proc_dir);
	remove_proc_entry("wanmib", priv->proc_dir);
	remove_proc_entry("mem", priv->proc_dir);
	remove_proc_entry("pp32", priv->proc_dir);
	remove_proc_entry("prio", priv->proc_dir);
	remove_proc_entry("qos", priv->proc_dir);
	remove_proc_entry("ptm", priv->frwk->proc_dir);
}
EXPORT_SYMBOL(vrx318_ptm_proc_exit);
