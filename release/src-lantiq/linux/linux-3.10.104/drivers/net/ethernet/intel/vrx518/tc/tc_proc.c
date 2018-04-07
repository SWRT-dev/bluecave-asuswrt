/*******************************************************************************

  Intel SmartPHY DSL PCIe TC driver
  Copyright(c) 2016 Intel Corporation.

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

*******************************************************************************/
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <net/datapath_api.h>
#include <net/dsl_tc.h>
#include <net/genetlink.h>
#include <linux/time.h>
#include "inc/tc_main.h"
#include "inc/reg_addr.h"
#include "inc/tc_common.h"
#include "inc/tc_api.h"
#include "inc/atm_tc.h"
#include "inc/ptm_tc.h"
#include "inc/tc_proc.h"
#include "inc/platform.h"

#define ATM_HEADER_SIZE (ATM_CELL_SIZE - ATM_CELL_PAYLOAD)
static char *dbg_flag_str[] = {
	"rx",
	"tx",
	"event",
	"rx_data",
	"tx_data",
	"init",
	"oam_rx",
	"oam_tx",
	"oam_rx_data",
	"oam_tx_data",
	"qos",
	"switch",
	"loopback",
	"coc",
	"mib"
};

static inline int lower_ch(int ch)
{
	if (ch >= 'A' && ch <= 'Z')
		return ch + 'a' - 'A';
	return ch;
}

static void vrx_replace_ch(char *p, int len, char orig_ch, char new_ch)
{
	int i;

	if (p)
		for (i = 0; i < len; i++) {
			if (p[i] == orig_ch)
				p[i] = new_ch;
		}
}

static void vrx_remove_leading_whitespace(char **p, int *len)
{
	while (*len && ((**p == ' ') || (**p == '\r') || (**p == '\r'))) {
		(*p)++;
		(*len)--;
	}
}
static unsigned int vrx_btoi(char *str)
{
	unsigned int sum = 0;
	signed len = 0, i = 0;

	len = strlen(str);
	len = len - 1;
	while (len >= 0) {
		if (*(str + len) == '1')
			sum = (sum | (1 << i));
		i++;
		len--;
	}
	return sum;
}
static int vrx_atoi(unsigned char *str)
{
	unsigned int n = 0;
	int i = 0;
	int nega_sign = 0;

	if (!str)
		return 0;
	vrx_replace_ch(str, strlen(str), '.', 0);
	vrx_replace_ch(str, strlen(str), ' ', 0);
	vrx_replace_ch(str, strlen(str), '\r', 0);
	vrx_replace_ch(str, strlen(str), '\n', 0);
	if (str[0] == 0)
		return 0;

	if (str[0] == 'b' || str[0] == 'B') {	/*binary format */
		n = vrx_btoi(str + 1);
	} else if ((str[0] == '0') && ((str[1] == 'x') || (str[1] == 'X'))) {
		/*hex format */
		str += 2;

		while (str[i]) {
			n = n * 16;
			if (('0' <= str[i] && str[i] <= '9')) {
				n += str[i] - '0';
			} else if (('A' <= str[i] && str[i] <= 'F')) {
				n += str[i] - 'A' + 10;
				;
			} else if (('a' <= str[i] && str[i] <= 'f')) {
				n += str[i] - 'a' + 10;
				;
			} else
				pr_err("Wrong value:%u\n", str[i]);

			i++;
		}

	} else {
		if (str[i] == '-') {	/*negative sign */
			nega_sign = 1;
			i++;
		}
		while (str[i]) {
			n *= 10;
			n += str[i] - '0';
			i++;
		}
	}
	if (nega_sign)
		n = -(int)n;
	return n;
}

int vrx_strcmpi(char const *s1, char const *s2)
{
	int c1, c2;

	if (!s1 || !s2)
		return 1;
	while (*s1 && *s2) {	/*same length */
		c1 = lower_ch(*s1);
		c2 = lower_ch(*s2);
		s1++;
		s2++;

		if (c1 != c2)
			return c1 - c2;
	}
	return *s1 - *s2;
}

static int get_num_from_str(char **p, int *len, int is_hex)
{
	int ret = 0;
	int n = 0;

	if ((*p)[0] == '0' && (*p)[1] == 'x') {
		is_hex = 1;
		(*p) += 2;
		(*len) -= 2;
	}

	if (is_hex) {
		while (*len && ((**p >= '0' && **p <= '9')
			|| (**p >= 'a' && **p <= 'f')
			|| (**p >= 'A' && **p <= 'F'))) {

			if (**p >= '0' && **p <= '9')
				n = **p - '0';
			else if (**p >= 'a' && **p <= 'f')
				n = **p - 'a' + 10;
			else if (**p >= 'A' && **p <= 'F')
				n = **p - 'A' + 10;
			ret = (ret << 4) | n;
			(*p)++;
			(*len)--;
		}
	} else {
		while (*len && **p >= '0' && **p <= '9') {
			n = **p - '0';
			ret = ret * 10 + n;
			(*p)++;
			(*len)--;
		}
	}

	return ret;
}

/*Split buffer to multiple segment with seperator space.
And put pointer to array[].
By the way, original buffer will be overwritten with '\0' at some place.
*/
int vrx_split_buffer(char *buffer, char *array[], int max_param_num)
{
	int i, set_copy = 0;
	int res = 0;
	int len;

	for (i = 0; i < max_param_num; i++)
		array[i] = NULL;
	if (!buffer)
		return 0;
	len = strlen(buffer);
	for (i = 0; i < max_param_num;) {
		if (len <= 0)
			break;
		vrx_remove_leading_whitespace(&buffer, &len);
		for (;
		     *buffer != ' ' && *buffer != '\0' && *buffer != '\r'
		     && *buffer != '\n' && *buffer != '\t' && len >= 0;
		     buffer++, len--) {
			/*Find first valid charactor */
			set_copy = 1;
			if (!array[i])
				array[i] = buffer;
		}

		if (set_copy == 1) {
			i++;
			if (*buffer == '\0' || *buffer == '\r'
			    || *buffer == '\n') {
				*buffer = 0;
				break;
			}
			*buffer = 0;
			buffer++;
			len--;
			set_copy = 0;

		} else {
			if (*buffer == '\0' || *buffer == '\r'
			    || *buffer == '\n')
				break;
			buffer++;
			len--;
		}
	}
	res = i;

	return res;
}

static void print_ppe_clk(struct seq_file *seq, struct dc_ep_dev *ep_dev)
{
	u32 sys_clk, ppe_clk;
	char *str_ppe_clk[] = {"36MHZ", "576MHZ", "494MHZ", "432MHZ", "288MHZ"};

	ep_dev->hw_ops->clk_get(ep_dev, &sys_clk, &ppe_clk);
	WARN_ON(ppe_clk >= PPE_CLK_MAX);
	seq_printf(seq, "PPE CLK: %s\n", str_ppe_clk[ppe_clk]);

	return;
}

static ssize_t mem_proc_write(struct file *file, const char __user *buf,
				size_t count, loff_t *data)
{
	int c, i;
	int action;
	u32 addr, set_val, repeat_cnt, rdaddr;
	struct tc_comm *priv;
	struct tc_priv *tcpriv;
	u32 soc_flag, prt_num;
	int num, len, rlen, idx, buflen;
	char *param_list[32], *p;
	char str[128], bufstr[128];

	priv = ((struct tc_comm *)PDE_DATA(file_inode(file)));
	if (priv == NULL) {
		pr_err("%s: Invalid priv data\n", __func__);
		return -EFAULT;
	}
	tcpriv = priv->tc_priv;
	if (tcpriv == NULL) {
		pr_err("%s: framework pointer is NULL!!!\n", __func__);
		return -EINVAL;
	}
	addr = set_val = repeat_cnt = 0;

	if (!access_ok(VERIFY_READ, buf, count))
		return -EFAULT;

	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;
	num = vrx_split_buffer(str, param_list, ARRAY_SIZE(param_list)) - 1;

	idx = 0;
	soc_flag = 0;
	action = 0;
	prt_num = 4;

	if (vrx_strcmpi(param_list[idx], "s") == 0) {
		idx++;
		soc_flag = 1;
	}
	if (vrx_strcmpi(param_list[idx], "r") == 0) {
		idx++;
		action = PPE_PROC_READ;
	} else if (vrx_strcmpi(param_list[idx], "w") == 0) {
		idx++;
		action = PPE_PROC_WRITE;
	} else if (vrx_strcmpi(param_list[idx], "d") == 0) {
		idx++;
		action = PPE_PROC_READ;
		prt_num = 1;
	}

	if (!action)
		goto proc_mem_help;

	num = num - idx + 1;
	if (num < 2)
		goto proc_mem_help;

	p = param_list[idx++];
	addr = get_num_from_str(&p, &len, 1);
	num --;
	/* addr = SB_BUFFER(addr); need to specify this is SB or FPI address */

	if (action == PPE_PROC_READ) {
		repeat_cnt = vrx_atoi(param_list[idx]);
		for (i = 0; i < repeat_cnt; i += prt_num) {
			memset(bufstr, 0 , 128);
			buflen = 0;
			if (soc_flag)
				sprintf(bufstr, "0x%08x: ", addr + TO_BYSZ(i));
			else
				sprintf(bufstr, "0x%08x: ",
					((fpi_addr(addr) + TO_BYSZ(i))));
			for (c = 0; c < prt_num && (i + c) < repeat_cnt; c++) {
				buflen = strlen(bufstr);
				if (soc_flag) {
					rdaddr = addr + TO_BYSZ(i + c);
					sprintf(bufstr + buflen, "%08x  ",
						readl((void __iomem *)rdaddr));
				} else {
					rdaddr = fpi_addr(addr)
							+ TO_BYSZ(i + c);
					sprintf(bufstr + buflen, "%08x  ",
						tc_r32(rdaddr));
				}
			}
			pr_info("%s\n", bufstr);
		}
	} else if (action == PPE_PROC_WRITE) {
		if (vrx_strcmpi(param_list[idx + 1], "r") == 0 ||
			vrx_strcmpi(param_list[idx + 1], "rpt") == 0) {
			repeat_cnt = vrx_atoi(param_list[idx + 2]);
			set_val = vrx_atoi(param_list[idx]);
			for (i = 0; i < repeat_cnt; i++) {
				if (soc_flag)
					writel(set_val,
					(void __iomem *)(addr + TO_BYSZ(i)));
				else
					tc_w32(set_val,
						fpi_addr(addr) + TO_BYSZ(i));
			}
		} else {
			for (i = 0; i < num; i++) {
				set_val = vrx_atoi(param_list[idx + i]);
				if (soc_flag)
					writel(set_val,
					(void __iomem *)(addr + TO_BYSZ(i)));
				else
					tc_w32(set_val,
						fpi_addr(addr) + TO_BYSZ(i));
			}
		}
	}

	return count;

proc_mem_help:
	pr_info("echo <s> r [soc_addr] [count] > mem\n");
	pr_info("echo <s> w [addr] [value] [val0] <val1> ... > mem\n");
	pr_info("echo <s> w [addr] <value> r [repeat_count] > mem\n");
	return count;
}

static int proc_read_mem(struct seq_file *seq, void *v)
{
	return 0;
}

static int proc_read_mem_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_mem, NULL);
}

static const struct file_operations mem_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_mem_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= mem_proc_write,
};

static ssize_t pp32_proc_write(struct file *file, const char __user *buf,
				size_t count, loff_t *data)
{
	struct tc_comm *priv;
	u32 reg_val;
	char str[128];
	u32 addr;
	int len, rlen, pc;
	int pp32 = 0;

	int num;
	char *param_list[20];

	priv = ((struct tc_comm *)PDE_DATA(file_inode(file)));
	if (priv == NULL) {
		pr_err("%s: priv pointer is NULL!!!\n", __func__);
		return -EINVAL;
	}


	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;
	num = vrx_split_buffer(str, param_list, ARRAY_SIZE(param_list));
	pp32 = vrx_atoi(param_list[1]);
	if (vrx_strcmpi(param_list[0], "pp32") == 0) {
		if (vrx_strcmpi(param_list[2], "restart") == 0) {
			reg_val = tc_r32(PPE_FREEZE) & (~(1 << (pp32 << 4)));
			tc_w32(reg_val, PPE_FREEZE);
		} else if (vrx_strcmpi(param_list[2], "freeze") == 0) {
			reg_val = tc_r32(PPE_FREEZE) | (1 << (pp32 << 4));
			tc_w32(reg_val, PPE_FREEZE);
		} else if (vrx_strcmpi(param_list[2], "start") == 0) {
			tc_w32(CTRL_CMD_RESTART, PP32_CTRL_CMD(pp32));
		} else if (vrx_strcmpi(param_list[2], "stop") == 0) {
			tc_w32(CTRL_CMD_STOP, PP32_CTRL_CMD(pp32));
		} else if (vrx_strcmpi(param_list[2], "step") == 0) {
			tc_w32(0, PP32_CTRL_CMD(pp32));
			tc_w32(CTRL_CMD_STEP, PP32_CTRL_CMD(pp32));
		} else if ((vrx_strcmpi(param_list[2], "pc0") == 0)
				|| (vrx_strcmpi(param_list[2], "pc1") == 0)) {
			if (vrx_strcmpi(param_list[2], "pc0") == 0)
				pc = 0;
			else
				pc = 1;

			if (vrx_strcmpi(param_list[3], "off") == 0) {
				tc_w32(BRK_GRP_PC_OFF(pc, pc), BRK_TRIG(pp32));
				tc_w32(BRK_CONTEXT_MASK_EN,
					BRK_PC_MASK(pp32, pc));
				tc_w32(0, BRK_PC(pp32, pc));
			} else {
				addr = vrx_atoi(param_list[3]);
				tc_w32(addr, BRK_PC(pp32, pc));
				tc_w32(BRK_CONTEXT_MASK_EN | BRK_CONTEXT_MASK,
					BRK_PC_MASK(pp32, pc));
				tc_w32(BRK_GRP_PC_ON(pc, pc), BRK_TRIG(pp32));
			}
		} else if ((vrx_strcmpi(param_list[2], "daddr0") == 0)
			|| (vrx_strcmpi(param_list[2], "daddr1") == 0)) {
			if (vrx_strcmpi(param_list[2], "daddr0") == 0)
					pc = 0;
			else
					pc = 1;

			if (vrx_strcmpi(param_list[3], "off") == 0) {
				tc_w32(BRK_GRP_DATA_ADDR_OFF(pc, pc),
					BRK_TRIG(pp32));
				tc_w32(BRK_CONTEXT_MASK_EN,
					BRK_DATA_ADDR_MASK(pp32, pc));
				tc_w32(0, BRK_DATA_ADDR(pp32, pc));

			} else {
				addr = vrx_atoi(param_list[3]);
				tc_w32(addr, BRK_DATA_ADDR(pp32, pc));
				tc_w32(BRK_CONTEXT_MASK_EN | BRK_CONTEXT_MASK,
					BRK_DATA_ADDR_MASK(pp32, pc));
				tc_w32(BRK_GRP_DATA_ADDR_ON(pc, pc),
					BRK_TRIG(pp32));
			}
		}
	} else {
		goto proc_pp32_help;
	}
	if (tc_r32(BRK_TRIG(pp32)))
		tc_w32(STOP_ON_BREAKPOINT_ON,
			PP32_CTRL_OPT(pp32));
	else
		tc_w32(STOP_ON_BREAKPOINT_OFF,
			PP32_CTRL_OPT(pp32));
	return count;

proc_pp32_help:
	pr_info("echo pp32 <id> freeze/start/stop/step/restart > pp32\n");
	pr_info("echo pp32 <id> pc0 off/[addr] > pp32\n");
	pr_info("echo pp32 <id> daddr0 off/[addr] > pp32\n");
	return count;
}
static int proc_read_pp32(struct seq_file *seq, void *v)
{
	struct tc_comm *priv = ((struct tc_comm *)seq->private);
	static const char *stron = " on";
	static const char *stroff = "off";

	int cur_context;
	int f_stopped;
	char strlength;
	int i, j;

	int pp32;

	if (!priv) {
		pr_err("priv pointer is NULL!!!\n");
		return -EINVAL;
	}

	for (pp32 = 0; pp32 < PP32_MAX; pp32++) {
		f_stopped = 0;
		seq_printf(seq, "===== pp32 core %d =====\n", pp32);

		cur_context = BRK_CUR_CONTEXT(pp32);
		seq_printf(seq, "Context: %d, PC: 0x%04x ",
			cur_context, CUR_PC(pp32));
		if ((tc_r32(PPE_FREEZE) & (1 << (pp32 << 4))) != 0) {
			seq_puts(seq, "freezed");
			f_stopped = 1;
		} else if (USER_STOPPED(pp32) || USER_BREAKIN_RCV(pp32)
			|| USER_BREAKPOINT_MET(pp32)) {
			strlength = 0;
			f_stopped = 1;
			if (USER_STOPPED(pp32)) {
				seq_puts(seq, "stopped");
				strlength = 1;
			}
			if (USER_BREAKPOINT_MET(pp32)) {
				seq_printf(seq, "%s", strlength ?
					" | breakpoint" : "breakpoint");
				strlength = 1;
			}
			if (USER_BREAKIN_RCV(pp32))
				seq_printf(seq, "%s", strlength ?
					" | breakin" : "breakin");

		} else if (CUR_PC(pp32) == CUR_PC(pp32)) {
			unsigned int pc_val[64] = {0};
			f_stopped = 1;
			for (i = 0; f_stopped && i < ARRAY_SIZE(pc_val); i++) {
				pc_val[i] = CUR_PC(pp32);
				for (j = 0; j < i; j++)
					if (pc_val[j] != pc_val[i]) {
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
		if (USER_BREAKPOINT_MET(pp32)) {
			seq_puts(seq, "break reason: ");
			if (BRK_PC_MET(pp32, 0)) {
				seq_puts(seq, "pc0");
				strlength = 1;
			}
			if (BRK_PC_MET(pp32, 1)) {
				seq_printf(seq, "%s", strlength ?
					" | pc1" : "pc1");
				strlength = 1;
			}
			if (BRK_DATA_ADDR_MET(pp32, 0)) {
				seq_printf(seq, "%s", strlength ?
					" | daddr0" : "daddr0");
				strlength = 1;
			}
			if (BRK_DATA_ADDR_MET(pp32, 1)) {
				seq_printf(seq, "%s", strlength ?
					" | daddr1" : "daddr1");
				strlength = 1;
			}
			if (BRK_DATA_VAL_RD_MET(pp32, 0)) {
				seq_printf(seq, "%s", strlength ?
					" | rdval0" : "rdval0");
				strlength = 1;
				if (BRK_DATA_VAL_RD_LO_EQ(pp32, 0)) {
					if (BRK_DATA_VAL_RD_GT_EQ(pp32, 0))
						seq_puts(seq, " ==");
					else
						seq_puts(seq, " <=");
				} else if (BRK_DATA_VAL_RD_GT_EQ(pp32, 0))
					seq_puts(seq, " >=");
			}
			if (BRK_DATA_VAL_RD_MET(pp32, 1)) {
				seq_printf(seq, "%s", strlength ?
					" | rdval1" : "rdval1");
				strlength = 1;
				if (BRK_DATA_VAL_RD_LO_EQ(pp32, 1)) {
					if (BRK_DATA_VAL_RD_GT_EQ(pp32, 1))
						seq_puts(seq, " ==");
					else
						seq_puts(seq, " <=");
				} else if (BRK_DATA_VAL_RD_GT_EQ(pp32, 1))
					seq_puts(seq, " >=");
			}
			if (BRK_DATA_VAL_WR_MET(pp32, 0)) {
				seq_printf(seq, "%s", strlength ?
					" | wtval0" : "wtval0");
				strlength = 1;
				if (BRK_DATA_VAL_WR_LO_EQ(pp32, 0)) {
					if (BRK_DATA_VAL_WR_GT_EQ(pp32, 0))
						seq_puts(seq, " ==");
					else
						seq_puts(seq, " <=");
				} else if (BRK_DATA_VAL_WR_GT_EQ(pp32, 0))
					seq_puts(seq, " >=");
			}
			if (BRK_DATA_VAL_WR_MET(pp32, 1)) {
				seq_printf(seq, "%s", strlength ?
					" | wtval1" : "wtval1");
				strlength = 1;
				if (BRK_DATA_VAL_WR_LO_EQ(pp32, 1)) {
					if (BRK_DATA_VAL_WR_GT_EQ(pp32, 1))
						seq_puts(seq, " ==");
					else
						seq_puts(seq, " <=");
				} else if (BRK_DATA_VAL_WR_GT_EQ(pp32, 1))
					seq_puts(seq, " >=");
			}
			seq_puts(seq, "\n");
		}

		if (f_stopped) {
			seq_puts(seq, "General Purpose Register (Context ");
			seq_printf(seq, "%d):\n", cur_context);
			for (i = 0; i < 4; i++) {
				for (j = 0; j < 4; j++)
					seq_printf(seq, "   %2d: %08x",
						i + j * 4,
						GP_CONTEXT_REG(pp32,
							cur_context,
							i + j * 4));
				seq_puts(seq, "\n");
			}
		}

		seq_printf(seq, "break out on: break in - %s, stop - %s\n",
			BREAKOUT_ON_BREAKIN(pp32) ? stron : stroff,
			BREAKOUT_ON_STOP(pp32) ? stron : stroff);
		seq_printf(seq, "\tstop on: break in - %s, break point - %s\n",
			STOP_ON_BREAKIN(pp32) ? stron : stroff,
			STOP_ON_BREAKPOINT(pp32) ? stron : stroff);
		seq_puts(seq, "breakpoint:\n");
		seq_printf(seq, "     pc0: 0x%08x, %s\n",
			tc_r32(BRK_PC(pp32, 0)),
			BRK_GRP_PC(pp32, 0, 0) ? "group 0" : "off");
		seq_printf(seq, "     pc1: 0x%08x, %s\n",
			tc_r32(BRK_PC(pp32, 1)),
			BRK_GRP_PC(pp32, 1, 1) ? "group 1" : "off");
		seq_printf(seq, "  daddr0: 0x%08x, %s\n",
			tc_r32(BRK_DATA_ADDR(pp32, 0)),
			BRK_GRP_DATA_ADDR(pp32, 0, 0) ? "group 0" : "off");
		seq_printf(seq, "  daddr1: 0x%08x, %s\n",
			tc_r32(BRK_DATA_ADDR(pp32, 1)),
			BRK_GRP_DATA_ADDR(pp32, 1, 1) ? "group 1" : "off");
		seq_printf(seq, "  rdval0: 0x%08x\n",
			tc_r32(BRK_DATA_VAL_RD(pp32, 0)));
		seq_printf(seq, "  rdval1: 0x%08x\n",
			tc_r32(BRK_DATA_VAL_RD(pp32, 1)));
		seq_printf(seq, "  wrval0: 0x%08x\n",
			tc_r32(BRK_DATA_VAL_WR(pp32, 0)));
		seq_printf(seq, "  wrval1: 0x%08x\n",
			tc_r32(BRK_DATA_VAL_WR(pp32, 1)));
	}

	return 0;
}

static int proc_read_pp32_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_pp32, PDE_DATA(inode));
}

static const struct file_operations pp32_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_pp32_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= pp32_proc_write,
};

static int proc_read_tc_cfg(struct seq_file *seq, void *v)
{
	struct tc_priv *priv = (struct tc_priv *)seq->private;
	const char *tc_mode_str[] = {
		"VRX518 Running",
		"VRX518 Init",
		"VRX518 NO_TC_Loaded",
		"VRX518 Switching",
		"VRX518 Exit",
		"VRX518 Error",
		NULL,
	};
	seq_printf(seq, "VRX518 TC mode: %s\n",
		tc_mode_str[priv->tc_stat]);
	seq_printf(seq, "DFE loopback: %s\n",
		priv->param.dfe_loopback ? "Enable" : "Disable");
	seq_printf(seq, "P2P: %s\n",
		priv->param.p2p ? "Enable" : "Disable");
	seq_printf(seq, "EP num: %d\n", priv->ep_num);
	seq_printf(seq, "ShowTime: %s\n", showtime_stat(priv) ? "Yes" : "No");
	seq_printf(seq, "PCIe Switch present: %s\n",
		!!(priv->switch_exist) ? "Yes" : "No");

	return 0;
}

static int proc_read_tc_cfg_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_tc_cfg, PDE_DATA(inode));
}

static const struct file_operations tc_cfg_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_tc_cfg_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static ssize_t proc_write_dbg(struct file *file, const char __user *buf,
			size_t count, loff_t *data)
{
	struct tc_priv *priv;
	char str[32];
	int len, rlen, i, j;
	int num, enable = 0;
	char *param_list[20];
	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;
	priv = (struct tc_priv *)PDE_DATA(file_inode(file));
	if (priv == NULL)
		return count;

	num = vrx_split_buffer(str, param_list, ARRAY_SIZE(param_list));

	if ((vrx_strcmpi(param_list[0], "enable") == 0)
		|| (vrx_strcmpi(param_list[0], "en") == 0)) {
		enable = 1;
	} else if (vrx_strcmpi(param_list[0], "disable") == 0
		|| (vrx_strcmpi(param_list[0], "dis") == 0)) {
		enable = -1;
	} else {
		goto proc_dbg_help;
	}
	if (!param_list[1]) {
		set_vrx_dbg_flag(priv->msg_enable, enable, -1);
		return count;
	}
	for (i = 1; i < num; i++) {
		for (j = 0; j < ARRAY_SIZE(dbg_flag_str); j++) {
			if (vrx_strcmpi(param_list[i], dbg_flag_str[j]) == 0) {
				set_vrx_dbg_flag(priv->msg_enable,
					enable, BIT(j));
				break;
			}
		}
	}
	pr_info("dbg write:%d\n", priv->msg_enable);

	return count;

proc_dbg_help:
	pr_info("echo [enable| disable] <dbg> > dbg \n");
	for (i = 0; i < ARRAY_SIZE(dbg_flag_str); i++)
		pr_info("%s ", dbg_flag_str[i]);
	pr_info("\n");

	return count;
}

static int proc_read_dbg(struct seq_file *seq, void *v)
{
	int i;
	struct tc_priv *priv;
	priv = (struct tc_priv *)seq->private;
	if (priv == NULL)
		return -1;
	seq_printf(seq, "Dbg Enable:0x%08x\n", priv->msg_enable);
	for (i = 0; i < ARRAY_SIZE(dbg_flag_str); i++) {
		if ((priv->msg_enable & BIT(i)) == BIT(i))
			seq_printf(seq, "%s ", dbg_flag_str[i]);
	}
	seq_puts(seq, "\n");
	return 0;
}

static int proc_read_dbg_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_dbg, PDE_DATA(inode));
}

static const struct file_operations tc_dbg_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_dbg_seq_open,
	.read		= seq_read,
	.write		= proc_write_dbg,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static ssize_t proc_write_tc_switch(struct file *file, const char __user *buf,
			size_t count, loff_t *data)
{
	char str[32];
	int len, rlen;
	int num, epid;
	char *param_list[20];
	struct tc_priv *priv;

	priv = (struct tc_priv *)PDE_DATA(file_inode(file));

	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;
	num = vrx_split_buffer(str, param_list, ARRAY_SIZE(param_list));
	if (vrx_strcmpi(param_list[0], "unload") == 0) {
		epid = vrx_atoi(param_list[1]);
		if (epid < priv->ep_num) {
			simu_tc_request(epid, TC_NONE_MODE);
			/* tc_unload(priv, epid); */
		} else {
			pr_err("epid is not correct\n");
			return count;
		}
	} else if (vrx_strcmpi(param_list[0], "load") == 0) {
		if (priv->tc_stat != NO_TC) {
			pr_err("TC Status is not correct\n");
			return count;
		}
		epid = vrx_atoi(param_list[2]);
		if (epid > priv->ep_num - 1) {
			pr_err("id is not correct\n");
			return count;
		}
		if (vrx_strcmpi(param_list[1], "atm") == 0)
			simu_tc_request(epid, TC_ATM_SL_MODE);
		else if (vrx_strcmpi(param_list[1], "ptm") == 0)
			simu_tc_request(epid, TC_PTM_SL_MODE);
		else if (vrx_strcmpi(param_list[1], "bond") == 0)
			simu_tc_request(epid, TC_PTM_BND_MODE);
		else {
			pr_err("mode is not correct\n");
			goto proc_tc_switch_help;
		}
	} else
		goto proc_tc_switch_help;

	return count;

proc_tc_switch_help:
	pr_info("echo load atm/ptm/bond [epid] > tc_switch\n");
	pr_info("echo unload [epid] > tc_switch\n");
	return count;
}

static const struct file_operations tc_switch_proc_fops = {
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
	struct tc_priv *priv;

	priv = (struct tc_priv *)PDE_DATA(file_inode(file));

	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;
	num = vrx_split_buffer(str, param_list, ARRAY_SIZE(param_list));
	if (vrx_strcmpi(param_list[1], "yes") == 0) {
		epid = vrx_atoi(param_list[0]);
		if (epid >= ARRAY_SIZE(priv->showtime)) {
			pr_err("epid is not correct\n");
			return count;
		}
		priv->showtime[epid] = 1;
		if (num > 2) {
			struct port_cell_info cell_info;
			unsigned int atm_rate;
			atm_rate = vrx_atoi(param_list[2]);
			cell_info.port_num = 1; /* only one ATM port */
			cell_info.tx_link_rate[0] = atm_rate;
			cell_info.tx_link_rate[1] = atm_rate;
			priv->tc_ops.showtime_enter(epid, &cell_info, NULL);
		} else
			priv->tc_ops.showtime_enter(epid, NULL, NULL);
	} else if (vrx_strcmpi(param_list[1], "no") == 0) {
		epid = vrx_atoi(param_list[0]);
		if (epid > 1) {
			pr_err("epid is not correct\n");
			return count;
		}
		priv->showtime[epid] = 0;
		priv->tc_ops.showtime_exit(epid);
	} else {
		goto proc_show_time_help;
	}

	return count;

proc_show_time_help:
	pr_info("echo [epid] yes/no > showtime\n");
	return count;
}

static const struct file_operations tc_show_time_proc_fops = {
	.owner      = THIS_MODULE,
	.write      = proc_write_show_time,
	.llseek     = noop_llseek,
};

static int proc_read_ver(struct seq_file *seq, void *v)
{
	const char *drv_name, *drv_ver;
	struct tc_priv *tcpriv;
	u32 date;

	tcpriv = (struct tc_priv *)seq->private;
	tc_get_drv_version(&drv_name, &drv_ver);
	seq_printf(seq, "%s: Version: %s\n", drv_name, drv_ver);

	seq_printf(seq, "PTM FW ver: %d.%d.%d(0x%02x%02x%02x)\n",
		tcpriv->fw.fw_hdr.ptm_ver.major,
		tcpriv->fw.fw_hdr.ptm_ver.mid,
		tcpriv->fw.fw_hdr.ptm_ver.minor,
		tcpriv->fw.fw_hdr.ptm_ver.major,
		tcpriv->fw.fw_hdr.ptm_ver.mid,
		tcpriv->fw.fw_hdr.ptm_ver.minor);

	seq_printf(seq, "ATM FW ver: %d.%d.%d(0x%02x%02x%02x)\n",
		tcpriv->fw.fw_hdr.atm_ver.major,
		tcpriv->fw.fw_hdr.atm_ver.mid,
		tcpriv->fw.fw_hdr.atm_ver.minor,
		tcpriv->fw.fw_hdr.atm_ver.major,
		tcpriv->fw.fw_hdr.atm_ver.mid,
		tcpriv->fw.fw_hdr.atm_ver.minor);

	date = tcpriv->fw.fw_hdr.date;
	seq_printf(seq, "compatible id: %d\n",
		tcpriv->fw.fw_hdr.comp_id);
	seq_printf(seq, "FW buit date(MM/DD/YYYY): %d/%d/%d\n",
		(date >> 24) & 0xff,
		(date >> 16) & 0xff,
		(date & 0xffff));

	return 0;
}

static int proc_read_ver_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_ver, PDE_DATA(inode));
}

static const struct file_operations tc_ver_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_ver_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int proc_read_soc(struct seq_file *seq, void *v)
{
	struct tc_priv *tcpriv;

	tcpriv = (struct tc_priv *)seq->private;

	seq_printf(seq, "TXIN Base: 0x%08x, TXIN num: %d\n",
			tcpriv->cfg.txin_dbase,
			tcpriv->cfg.txin_dnum);
	seq_printf(seq, "TXOUT Base: 0x%08x, TXOUT num: %d\n",
			tcpriv->cfg.txout_dbase,
			tcpriv->cfg.txout_dnum);
	seq_printf(seq, "RXIN Base: 0x%08x, RXIN num: %d\n",
			tcpriv->cfg.rxin_dbase,
			tcpriv->cfg.rxin_dnum);
	seq_printf(seq, "RXOUT Base: 0x%08x, RXOUT num: %d\n",
			tcpriv->cfg.rxout_dbase,
			tcpriv->cfg.rxout_dnum);

	return 0;
}

static int proc_read_soc_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_soc, PDE_DATA(inode));
}

static const struct file_operations tc_soc_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_soc_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};


static struct tc_proc_list tc_procs[] = {
	{TC_PROC_DIR,	0,	NULL,			1},
	{"cfg",		0644, &tc_cfg_proc_fops,	0},
	{"dbg",		0644, &tc_dbg_proc_fops,	0},
	{"tc_switch",	0200, &tc_switch_proc_fops,	0},
	{"showtime",	0200, &tc_show_time_proc_fops,	0},
	{"ver",		0644, &tc_ver_proc_fops,	0},
	{"soc",		0644, &tc_soc_proc_fops,	0},
};

int tc_proc_init(struct tc_priv *priv)
{
	int i;
	struct proc_dir_entry *entry = NULL;
	struct proc_dir_entry *tc_dir = NULL;
	struct tc_proc_list *p;

	for (i = 0; i < ARRAY_SIZE(tc_procs); i++) {
		p = &tc_procs[i];
		if (p->is_folder) {
			tc_dir = proc_mkdir(p->proc_name, NULL);
			if (!tc_dir)
				return -ENOMEM;
		} else {
			entry = proc_create_data(p->proc_name, p->mode,
					tc_dir, p->fops, (void *)priv);
			if (!entry)
				goto err;
		}
	}
	priv->proc_dir = tc_dir;

	return 0;

err:
	for (i = i - 1; i >= 0; i--) {
		p = &tc_procs[i];
		if (!p->is_folder)
			remove_proc_entry(p->proc_name, tc_dir);
		else
			remove_proc_entry(p->proc_name, NULL);
	}

	return -ENOMEM;
}

void tc_proc_exit(struct tc_priv *priv)
{
	int i;
	struct tc_proc_list *p;

	for (i = ARRAY_SIZE(tc_procs) - 1; i >= 0; i--) {
		p = &tc_procs[i];
		if (!p->is_folder)
			remove_proc_entry(p->proc_name, priv->proc_dir);
		else
			remove_proc_entry(p->proc_name, NULL);
	}
}

static int proc_read_ptm_wanmib_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_ptm_wanmib, PDE_DATA(inode));
}

static const struct file_operations ptm_wanmib_proc_fops = {
	.owner      = THIS_MODULE,
	.open       = proc_read_ptm_wanmib_seq_open,
	.read       = seq_read,
	.write      = proc_write_ptm_wanmib,
	.llseek     = seq_lseek,
	.release    = single_release,
};

static int proc_ptm_read_cfg(struct seq_file *seq, void *v)
{
	struct ptm_ep_priv *priv = (struct ptm_ep_priv *)seq->private;
	struct ptm_priv *ptm_tc = (struct ptm_priv *)priv->tc_priv->priv;

	if (!priv || !ptm_tc) {
		pr_err("%s: Invalid private data\n", __func__);
		return -EINVAL;
	}

	seq_printf(seq, "PTM mode: %s\n",
		(ptm_tc->tc_priv->tc_mode == TC_PTM_BND_MODE) ?
		"PTM Bonding mode" : "PTM Single Line mode");
	if (priv->tc_priv->tc_mode != TC_PTM_BND_MODE)
		seq_printf(seq, "VRX518 on line: %d\n", priv->ep_id);

	seq_printf(seq, "Showtime: %d\n", priv->tc_priv->showtime[priv->ep_id]);
	seq_printf(seq, "Power Saving: %s\n",
		ptm_power_saving(priv, 0, 0) == 1 ? "Enable" : "Disable");
	seq_printf(seq, "irq: %d\n", priv->ep->irq);
	seq_printf(seq, "membase: 0x%x\n", (u32)priv->ep->membase);
	print_ppe_clk(seq, priv->ep);

	return 0;
}

static int proc_read_cfg_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_ptm_read_cfg, PDE_DATA(inode));
}

static ssize_t ptm_cfg_proc_write(struct file *file,
			const char __user *buf, size_t count, loff_t *data)
{
	struct ptm_ep_priv *priv;
	u32 en, clk;
	int num, len, rlen, i;
	char *param_list[32];
	char str[128];
	const int clk_list[] = {36, 576, 494, 432, 288};

	priv = (struct ptm_ep_priv *)PDE_DATA(file_inode(file));
	if (priv == NULL) {
		pr_err("%s: Invalid private data\n", __func__);
		return -EINVAL;
	}

	if (!access_ok(VERIFY_READ, buf, count))
		return -EFAULT;

	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;
	num = vrx_split_buffer(str, param_list, ARRAY_SIZE(param_list));

	en = clk = 0;
	if (vrx_strcmpi(param_list[0], "en") == 0
		|| vrx_strcmpi(param_list[0], "enable") == 0) {
		en = 1;
	} else if (vrx_strcmpi(param_list[0], "dis") == 0
		|| vrx_strcmpi(param_list[0], "disable") == 0) {
		en = 0;
	} else if (vrx_strcmpi(param_list[0], "clk") == 0
		|| vrx_strcmpi(param_list[0], "clock") == 0) {
		clk = 1;
	} else
		goto proc_ptm_cfg_help;

	if (vrx_strcmpi(param_list[1], "ps") == 0
		|| vrx_strcmpi(param_list[1], "powersaving") == 0) {
		ptm_power_saving(priv, en, 1);
	} else if (clk == 1) {
		clk = vrx_atoi(param_list[1]);
		for (i = 1; i < ARRAY_SIZE(clk_list); i++)
			if (clk == clk_list[i])
				break;
		if (i >= ARRAY_SIZE(clk_list))
			goto proc_ptm_cfg_help;
		tc_ppe_clkset(priv->ep, i);
	}

	return count;

proc_ptm_cfg_help:
	pr_info("echo [enable | disable] powersaving > cfg\n");
	pr_info("echo clk <576 | 494 | 432 | 288> > cfg\n");
	return count;
}


static const struct file_operations ptm_cfg_proc_fops = {
	.owner = THIS_MODULE,
	.open = proc_read_cfg_seq_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.write = ptm_cfg_proc_write,
	.release = single_release,
};

static ssize_t proc_ptm_write_prio(struct file *file, const char __user *buf,
				size_t count, loff_t *data)
{
	struct ptm_ep_priv *priv;
	struct ptm_priv *ptm_tc;
	char local_buf[256];
	int len;
	char *p1;
	int prio = -1;
	int queue = -1;
	int preempt = -1;
	int num;
	int i;
	char *param_list[20];

	len = sizeof(local_buf) < count ? sizeof(local_buf) - 1 : count;
	len = len - copy_from_user(local_buf, buf, len);
	local_buf[len] = 0;
	p1 = local_buf;

	num = vrx_split_buffer(local_buf, param_list, ARRAY_SIZE(param_list));
	priv = (struct ptm_ep_priv *)PDE_DATA(file_inode(file));

	if (priv == NULL) {
		pr_err("%s: Invalid private data\n", __func__);
		return -EINVAL;
	}
	ptm_tc = (struct ptm_priv *)priv->ptm_tc;

	if (vrx_strcmpi(param_list[0], "help") == 0)
		goto proc_ptm_prio_help;
	else {
		for (i = 0; i < num; i += 6) {
			if ((vrx_strcmpi(param_list[i], "prio") == 0)
				|| (vrx_strcmpi(param_list[i], "p") == 0))
				prio = vrx_atoi(param_list[i+1]);
			else
				pr_err("Error: expecting prio\n");

			if ((vrx_strcmpi(param_list[i+2], "queue") == 0) ||
				(vrx_strcmpi(param_list[i+2], "q") == 0))
				queue = vrx_atoi(param_list[i+3]);
			else
				pr_err("Error: expecting queue\n");

			if ((vrx_strcmpi(param_list[i+4], "pre") == 0) ||
				(vrx_strcmpi(param_list[i+4], "r") == 0))
				preempt = vrx_atoi(param_list[i+5]);
			else
				pr_err("Error: expecting preemption\n");
			if (queue < 0 || queue >= PTM_PRIO_Q_NUM
				|| prio < 0 || prio >= PTM_PRIO_Q_NUM
				|| preempt < 0 || preempt >= OUTQ_PNUM) {
				pr_err("%s: invalid parameter: prio: %d, queue: %d, preemption: %d\n",
					__func__, prio, queue, preempt);
				goto proc_ptm_prio_help;
			} else {
				pr_info("prio: %d, queue: %d, preemption:%d\n",
					prio, queue, preempt);
				ptm_tc->prio_q_map[prio] = queue;
				ptm_set_q_preempt(priv, queue, preempt);
			}
		}
	}
	return count;

proc_ptm_prio_help:
	pr_info("echo prio xx queue xx pre xx [prio xx queue xx pre xx] > prio\n");
	return count;
}

static int proc_ptm_read_prio(struct seq_file *seq, void *v)
{
	int i;
	struct ptm_ep_priv *priv = (struct ptm_ep_priv *)seq->private;
	struct ptm_priv *ptm_tc = priv->ptm_tc;

	seq_puts(seq, "Prio:\t\t");
	for (i = 0; i < PTM_PRIO_Q_NUM; i++)
		seq_printf(seq, "%d ", i);
	seq_puts(seq, "\nQid:\t\t");
	for (i = 0; i < PTM_PRIO_Q_NUM; i++)
		seq_printf(seq, "%d ",
			ptm_tc->prio_q_map[i]);
	seq_puts(seq, "\n");

	seq_puts(seq, "VRX518: 1:Preemption / 0: Non-Preemption\n");
	seq_puts(seq, "Preemption:\t");
	for (i = 0; i < PTM_PRIO_Q_NUM; i++)
		seq_printf(seq, "%d ", ptm_get_q_preempt(priv, i));
	seq_puts(seq, "\n");

	return 0;
}

static int proc_ptm_read_prio_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_ptm_read_prio, PDE_DATA(inode));
}

static const struct file_operations ptm_prio_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_ptm_read_prio_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.write		= proc_ptm_write_prio,
	.release	= single_release,
};

static int proc_ptm_read_bond_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_ptm_read_bond, PDE_DATA(inode));
}

static const struct file_operations ptm_bond_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_ptm_read_bond_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int proc_ptm_read_bondmib_seq_open(struct inode *inode,
							struct file *file)
{
	return single_open(file, proc_ptm_read_bondmib, PDE_DATA(inode));
}

static const struct file_operations ptm_bondmib_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_ptm_read_bondmib_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.write		= proc_ptm_write_bondmib,
	.release	= single_release,
};

struct fwdbg_t {
	char *cmd;
	void (*pfunc)(struct tc_comm *priv,
			char *tokens[], int token_num, char *result);
	void (*pfunc_help)(char *result);
};

static void fwdbg_print_codeword(struct tc_comm *priv,
	char *result,
	unsigned int sb_addr,
	unsigned int len)
{
	int i = 0;
	for (i = 0; i < len; i++) {
		if (i % 4 == 0) {
			sprintf(result + strlen(result),
				"\n\t");
		}
		sprintf(result + strlen(result),
			" %08x",
			sb_r32(sb_addr + i));
	}
	sprintf(result + strlen(result),
					"\n");
}
static void fwdbg_read_tx_cb(struct tc_comm *priv,
	char *tokens[], int token_num, char *result)
{
	unsigned int start_pg;
	unsigned int print_pg_num;
	unsigned int num;
	int codeword = 0;
	struct tc_priv *tcpriv;

	codeword = vrx_atoi(tokens[1]);
	tcpriv = priv->tc_priv;
	if (codeword >= 0 && codeword < 2) {
		unsigned int dbase0;
		unsigned int pnum0, i;
		unsigned int cw;
		struct ffsm_dba dba;
		struct ffsm_cfg cfg;

		tc_mem_read(priv, &dba,
			FFSM_DBA(codeword), sizeof(struct ffsm_dba));
		dbase0 = dba.dbase + 0x2000;
		tc_mem_read(priv, &cfg,
			FFSM_CFG(codeword), sizeof(struct ffsm_cfg));
		pnum0 = cfg.pnum;


		start_pg = 0;
		print_pg_num = pnum0;
		if (token_num >= 2)
			start_pg =  vrx_atoi(tokens[2]);
		if (token_num >= 3)
			print_pg_num =  vrx_atoi(tokens[3]);

		start_pg = start_pg % pnum0;
		if (print_pg_num > pnum0)
			print_pg_num = pnum0;
		sprintf(result + strlen(result),
			"%s TX BC 0 CELL data/ctrl buffer:\n",
			tcpriv->tc_mode == TC_ATM_SL_MODE ? "ATM" : "PTM"
			);

		for (i = start_pg, num = 0;
			num < print_pg_num;
			num ++, i = (i + 1) % pnum0) {
			cw = dbase0 + i * CW_PAGE_SIZE;
			fwdbg_print_codeword(priv,
				result,
				cw,
				17);
			pr_info("%s", result);
			memset(result, 0, strlen(result) + 1);
		}
	}
}
static void fwdbg_read_tx_cb_help(char *result)
{
	sprintf(result + strlen(result),
		"echo read_tx_cb [0|1] [start_pg] [print_pg_num]\n");
	sprintf(result + strlen(result),
		"\tread tx bare channel 0 & 1\n");
	sprintf(result + strlen(result),
		"\tcell/codeword buffer for bc0 or bc1\n");
}

static void fwdbg_read_rx_cb(struct tc_comm *priv,
		char *tokens[], int token_num, char *result)
{
	unsigned int start_pg;
	unsigned int print_pg_num;
	unsigned int num;
	int codeword = 0;
	struct tc_priv *tcpriv;
	codeword = vrx_atoi(tokens[1]);
	tcpriv = priv->tc_priv;
	if (codeword >= 0 && codeword < 2) {
		unsigned int dbase0, cbase0;
		unsigned int pnum0, i;
		unsigned int cw;
		struct sfsm_dba dba;
		struct sfsm_cba cba;
		struct sfsm_cfg cfg;

		tc_mem_read(priv, &dba,
			SFSM_DBA(codeword), sizeof(struct sfsm_dba));
		dbase0 = dba.dbase + 0x2000;
		tc_mem_read(priv, &cba,
			SFSM_CBA(codeword), sizeof(struct sfsm_cba));
		cbase0 = cba.cbase + 0x2000;
		tc_mem_read(priv, &cfg,
			SFSM_CFG(codeword), sizeof(struct sfsm_cfg));
		pnum0 = cfg.pnum;

		start_pg = 0;
		print_pg_num = pnum0;

		if (token_num >= 2)
			start_pg =  vrx_atoi(tokens[2]);
		if (token_num >= 3)
			print_pg_num =  vrx_atoi(tokens[3]);

		start_pg = start_pg % pnum0;
		if (print_pg_num > pnum0)
			print_pg_num = pnum0;

		sprintf(result + strlen(result),
			"%s RX BC %d CELL data/ctrl buffer (pnum0 = %d):\n",
			tcpriv->tc_mode == TC_ATM_SL_MODE ? "ATM" : "PTM",
			codeword, pnum0);
		for (i = start_pg, num = 0;
				num < print_pg_num;
				num ++, i = (i + 1) % pnum0) {
			if (tcpriv->tc_mode == TC_ATM_SL_MODE) {
				struct atm_cw_ctrl_t ctrl;
				cw = dbase0 + i * CW_PAGE_SIZE;
				tc_mem_read(priv, &ctrl,
					fpi_addr(cbase0 + i),
					sizeof(struct atm_cw_ctrl_t));

				sprintf(result + strlen(result),
					"cvm=%1x cvc=%03x bsm=%01x",
					ctrl.cvm, ctrl.cvc, ctrl.bsm);
				sprintf(result + strlen(result),
					" csp=%03x idle=%01x drop=%01x",
					ctrl.csp, ctrl.idle,
					ctrl.drop);
				sprintf(result + strlen(result),
					"ber=%03x bad=%03x state=%01x",
					ctrl.ber, ctrl.bad,
					ctrl.state);
				fwdbg_print_codeword(priv,
					result, cw, 17);
			} else {
				struct ptm_cw_ctrl_t ctrl;
				char *cwid_txt[8] = {
				"All_data",
				"End_of_frame",
				"Start_while_tx",
				"All_idle",
				"Start_while_idle",
				"All_idle_nosync",
				"Error",
				"Res"
				};
				cw = dbase0 + i * CW_PAGE_SIZE;
				tc_mem_read(priv, &ctrl,
					fpi_addr(cbase0 + i),
					sizeof(struct ptm_cw_ctrl_t));
				sprintf(result + strlen(result),
					"cwid=%02x[%-16s] cwer=%02x",
					ctrl.cwid, cwid_txt[ctrl.cwid],
					ctrl.cwer);
				sprintf(result + strlen(result),
					"  preempt=%01x shrt=%01x ffbn=%03x",
					ctrl.preempt, ctrl.shrt,
					ctrl.ffbn);
				sprintf(result + strlen(result),
					" spos=%02x ber=%03x\n",
					ctrl.spos,
					ctrl.ber);
				sprintf(result + strlen(result),
					" bad=%03x state=%01x",
					ctrl.bad, ctrl.state);
				fwdbg_print_codeword(priv,
					result, cw, 17);
			}
			pr_info("%s", result);
			memset(result, 0, strlen(result) + 1);
		}
	}
}
static void fwdbg_read_rx_cb_help(char *result)
{
	sprintf(result + strlen(result),
		"echo read_rx_cb [0|1] [start_pg] [print_pg_num]\n");
	sprintf(result + strlen(result),
		"\tread rx bare channel 0 & 1\n");
	sprintf(result + strlen(result),
		"\tcell/codeword buffer for bc0 or bc1\n");
}

static void fwdbg_clear_tx_cb(struct tc_comm *priv,
	char *tokens[], int token_num, char *result)
{
	unsigned int start_pg;
	unsigned int clear_pg_num;
	int codeword;
	codeword = vrx_atoi(tokens[1]);
	if (codeword >= 0 && codeword < 2) {
		unsigned int dbase0;
		unsigned int pnum0;
		struct ffsm_dba dba;
		struct ffsm_cfg cfg;

		tc_mem_read(priv, &dba,
			FFSM_DBA(codeword), sizeof(struct ffsm_dba));
		dbase0 = dba.dbase + 0x2000;
		tc_mem_read(priv, &cfg,
			FFSM_CFG(codeword), sizeof(struct ffsm_cfg));
		pnum0 = cfg.pnum;

		start_pg = 0;
		clear_pg_num = pnum0;

		if (token_num >= 2)
			start_pg =  vrx_atoi(tokens[2]);
		if (token_num >= 3)
			clear_pg_num =  vrx_atoi(tokens[3]);

		start_pg = start_pg % pnum0;
		if (clear_pg_num > pnum0)
			clear_pg_num = pnum0;

		dbase0 = fpi_addr(dbase0 + start_pg * CW_PAGE_SIZE);
		tc_memset(priv, dbase0, 0,
			CW_PAGE_SIZE * sizeof(unsigned int) * clear_pg_num);
	}
}

static void fwdbg_clear_tx_cb_help(char *result)
{
	sprintf(result + strlen(result),
		"echo clear_tx_cb [0|1] [start_pg] [clear_pg_num]\n");
	sprintf(result + strlen(result),
		"\tclear tx bare channel 0 & 1\n");
	sprintf(result + strlen(result),
		"\tcell/codeword buffer for bc0 or bc1\n");
}

static void fwdbg_clear_rx_cb(struct tc_comm *priv,
	char *tokens[], int token_num, char *result)
{
	unsigned int start_pg;
	unsigned int clear_pg_num;
	int codeword;
	codeword = vrx_atoi(tokens[1]);
	if (codeword >= 0 && codeword < 2) {
		unsigned int dbase0, cbase0;
		unsigned int pnum0;
		struct sfsm_dba dba;
		struct sfsm_cba cba;
		struct sfsm_cfg cfg;

		tc_mem_read(priv, &dba,
			SFSM_DBA(codeword), sizeof(struct sfsm_dba));
		dbase0 = dba.dbase + 0x2000;
		tc_mem_read(priv, &cba,
			SFSM_CBA(codeword), sizeof(struct sfsm_cba));
		cbase0 = cba.cbase + 0x2000;
		tc_mem_read(priv, &cfg,
			SFSM_CFG(codeword), sizeof(struct sfsm_cfg));
		pnum0 = cfg.pnum;
		start_pg = 0;
		clear_pg_num = pnum0;

		if (token_num >= 2)
			start_pg =  vrx_atoi(tokens[2]);
		if (token_num >= 3)
			clear_pg_num =  vrx_atoi(tokens[3]);

		start_pg = start_pg % pnum0;
		if (clear_pg_num > pnum0)
			clear_pg_num = pnum0;

		dbase0 = fpi_addr(dbase0 + start_pg * CW_PAGE_SIZE);
		cbase0 = fpi_addr(cbase0 + start_pg);
		tc_memset(priv, dbase0, 0,
			CW_PAGE_SIZE * sizeof(unsigned int) * clear_pg_num);
		tc_memset(priv, cbase0, 0,
			sizeof(unsigned int) * clear_pg_num);
	}
}
static void fwdbg_clear_rx_cb_help(char *result)
{
	sprintf(result + strlen(result),
		"echo clear_rx_cb [0|1] [start_pg] [clear_pg_num]\n");
	sprintf(result + strlen(result),
		"\tclear rx bare channel 0 & 1\n");
	sprintf(result + strlen(result),
		"\tcell/codeword buffer for bc0 or bc1\n");
}
static void fwdbg_help(struct tc_comm *priv,
		char *tokens[], int token_num, char *result);
struct fwdbg_t cmds[] = {
	{"help\0", fwdbg_help, NULL},
	{"read_tx_cb\0", fwdbg_read_tx_cb, fwdbg_read_tx_cb_help},
	{"read_rx_cb\0", fwdbg_read_rx_cb, fwdbg_read_rx_cb_help},
	{"clear_tx_cb\0", fwdbg_clear_tx_cb, fwdbg_clear_tx_cb_help},
	{"clear_rx_cb\0", fwdbg_clear_rx_cb, fwdbg_clear_rx_cb_help},
	{NULL, NULL, NULL}
};
static void fwdbg_help(struct tc_comm *priv,
	char *tokens[], int token_num, char *result)
{
	const char *proc_file = "fwdbg";
	int i;

	/* print commands only */
	if (!token_num) {
		for (i = 0; cmds[i].cmd != NULL; i++) {
			sprintf(result + strlen(result)
				, "%s\t", cmds[i].cmd);
			if (i % 3 == 0)
				sprintf(result + strlen(result),
					"\n");
		}

		sprintf(result + strlen(result),
			"\n\n");
		sprintf(result + strlen(result),
			"echo help [cmd] > %s for details\n",
			proc_file);
		return;
	}
	if (!tokens)
		return;
	for (i = 0; cmds[i].cmd != NULL; i++) {
		if (vrx_strcmpi(tokens[1], cmds[i].cmd) == 0)
			if (cmds[i].pfunc_help) {
				cmds[i].pfunc_help(result);
				break;
			}
	}
	if (!cmds[i].cmd)
		fwdbg_help(priv, NULL, 0, result);
}
int fw_dbg_start(struct tc_comm *priv,
	char *commands[], int num, char *result)
{
	int i;

	if (!priv || !commands || !result) {
		pr_err("<%s>: Invalid arguments\n", __func__);
		return -1;
	}
	for (i = 0; cmds[i].cmd != NULL; i++) {
		if (vrx_strcmpi(commands[0], cmds[i].cmd) == 0) {
			cmds[i].pfunc(priv, commands, num - 1, result);
			break;
		}
	}

	return 0;
}

ssize_t proc_write_fwdbg_seq(
	struct file *file,
	const char __user *buf,
	size_t count,
	loff_t *data
	)
	{
	char results[512];
	char local_buf[128];
	int len;
	char *param_list[20];
	int num;
	struct tc_comm *priv;

	len = sizeof(local_buf) < count ? sizeof(local_buf) - 1 : count;
	len = len - copy_from_user(local_buf, buf, len);
	local_buf[len] = 0;

	num = vrx_split_buffer(local_buf, param_list,
		ARRAY_SIZE(param_list));
	priv =  (struct tc_comm *)PDE_DATA(file_inode(file));
	if (priv == NULL) {
		pr_err("<%s>: Invalid private data\n", __func__);
		return count;
	}
	memset(results, 0, 512);
	/* execute user's command */
	if (fw_dbg_start(priv, param_list, num, results) != 0)
		goto proc_fwdbg_help;
	/* print out result */
	pr_info("%s", results);
	return count;
proc_fwdbg_help:
	pr_info("echo help > fwdbg\n");
	return count;
}

static int proc_read_fwdbg(struct seq_file *seq, void *v)
{
	return 0;
}

static int proc_read_fwdbg_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_fwdbg, NULL);
}
static const struct file_operations fwdbg_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_fwdbg_seq_open,
	.read		= seq_read,
	.write		= proc_write_fwdbg_seq,
	.llseek		= seq_lseek,
	.release	= single_release,
};
static struct tc_proc_list ptm_sl_procs[] = {
	{TC_PROC_PTM_DIR, 0,	NULL,			1},
	{"mem",		0644,	&mem_proc_fops,		0},
	{"pp32",	0644,	&pp32_proc_fops,	0},
	{"wanmib",	0644,	&ptm_wanmib_proc_fops,	0},
	{"cfg",		0644,	&ptm_cfg_proc_fops,	0},
	{"prio",	0644,	&ptm_prio_proc_fops,	0},
	{"fwdbg",	0644,	&fwdbg_proc_fops,	0},
};

static struct tc_proc_list ptm_bond_procs[] = {
	{"bonding",	0644,	&ptm_bond_proc_fops,	0},
	{"bondmib",	0644,	&ptm_bondmib_proc_fops,	0},
};




int ptm_tc_proc_init(void *ptm_priv)
{
	struct proc_dir_entry *entry;
	struct proc_dir_entry *tc_dir = NULL;
	struct ptm_ep_priv *priv;
	struct tc_proc_list *p;
	char proc_name[64];
	int i, j;

	priv = (struct ptm_ep_priv *)ptm_priv;
	for (i = 0; i < ARRAY_SIZE(ptm_sl_procs); i++) {
		p = &ptm_sl_procs[i];
		if (p->is_folder) {
			sprintf(proc_name, "%s%d", p->proc_name, priv->ep_id);
			tc_dir = proc_mkdir(proc_name, priv->tc_priv->proc_dir);
			if (!tc_dir)
				return -ENOMEM;
		} else {
			entry = proc_create_data(p->proc_name, p->mode,
					tc_dir, p->fops, (void *)priv);
			if (!entry)
				goto sl_proc_err;
		}
	}

	if (priv->tc_priv->tc_mode == TC_PTM_BND_MODE)
		for (j = 0; j < ARRAY_SIZE(ptm_bond_procs); j++) {
			p = &ptm_bond_procs[j];
			entry = proc_create_data(p->proc_name, p->mode,
					tc_dir, p->fops, (void *)priv);
			if (!entry)
				goto bond_proc_err;
		}

	priv->proc_dir = tc_dir;

	return 0;

bond_proc_err:
	for (j = j - 1; j >= 0; j--) {
		p = &ptm_bond_procs[j];
		if (!p->is_folder)
			remove_proc_entry(p->proc_name, tc_dir);
		else
			remove_proc_entry(p->proc_name, NULL);
	}
sl_proc_err:
	for (i = i - 1; i >= 0; i--) {
		p = &ptm_sl_procs[i];
		if (!p->is_folder)
			remove_proc_entry(p->proc_name, tc_dir);
		else {
			sprintf(proc_name, "%s%d", p->proc_name, priv->ep_id);
			remove_proc_entry(proc_name, priv->tc_priv->proc_dir);
		}
	}
	return -ENOMEM;
}

void ptm_tc_proc_exit(void *ptm_priv)
{
	struct ptm_ep_priv *priv = (struct ptm_ep_priv *)ptm_priv;
	struct tc_proc_list *p;
	char proc_name[64];
	int i;

	if (priv == NULL) {
		pr_err("%s: Invalid private data\n", __func__);
		return;
	}

	if (priv->tc_priv->tc_mode == TC_PTM_BND_MODE) {
		for (i = ARRAY_SIZE(ptm_bond_procs) - 1; i >= 0; i--) {
			p = &ptm_bond_procs[i];
			if (!p->is_folder)
				remove_proc_entry(p->proc_name, priv->proc_dir);
			else
				remove_proc_entry(p->proc_name, NULL);
		}
	}

	for (i = ARRAY_SIZE(ptm_sl_procs) - 1; i >= 0; i--) {
		p = &ptm_sl_procs[i];
		if (!p->is_folder)
			remove_proc_entry(p->proc_name, priv->proc_dir);
		else {
			sprintf(proc_name, "%s%d", p->proc_name, priv->ep_id);
			remove_proc_entry(proc_name, priv->tc_priv->proc_dir);
		}
	}
}

/* ATM Proc functions */
static int proc_read_atm_cfg(struct seq_file *seq, void *v)
{
	int i;
	struct atm_priv *priv = (struct atm_priv *)seq->private;
	if (priv == NULL) {
		pr_err("%s: invalid private data\n", __func__);
		return -EINVAL;
	}
	spin_lock_bh(&priv->atm_lock);
	/* ATM'PVC list*/
	seq_puts(seq, "PVC List:\n");
	seq_printf(seq, "PVC MAP: 0x%x\n", priv->pvc_tbl);
	seq_printf(seq, "TX Queue Map: 0x%x\n", priv->sw_txq_tbl);

	for (i = 0; i < ATM_PVC_NUMBER; i++) {
		if ((priv->pvc_tbl & BIT(i)) == 0)
			continue;
		seq_printf(seq, "[%d]: VPI: %d, VCI: %d, dev: %s, MPoA type: %d, MPoA mode: %d, Subif_id: 0x%x, Tx Queue Table: 0x%x\n",
			i, priv->conn[i].vcc->vpi, priv->conn[i].vcc->vci,
			priv->conn[i].dev->name, priv->conn[i].mpoa_type,
			priv->conn[i].mpoa_mode, priv->conn[i].subif_id,
			priv->conn[i].sw_txq_tbl);
	}
	spin_unlock_bh(&priv->atm_lock);

	return 0;
}

static int proc_read_atm_cfg_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_atm_cfg, PDE_DATA(inode));
}

static ssize_t atm_cfg_proc_write(struct file *file,
		const char __user *buf,	size_t count, loff_t *data)
{
	struct atm_priv *priv;
	u32 en;
	int num, len, rlen;
	char *param_list[32];
	char str[128];

	priv = (struct atm_priv *)PDE_DATA(file_inode(file));

	if (!access_ok(VERIFY_READ, buf, count))
		return -EFAULT;

	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;
	num = vrx_split_buffer(str, param_list, ARRAY_SIZE(param_list));

	if (vrx_strcmpi(param_list[0], "en") == 0
		|| vrx_strcmpi(param_list[0], "enble") == 0) {
		en = 1;
	} else if (vrx_strcmpi(param_list[0], "dis") == 0
		|| vrx_strcmpi(param_list[0], "disable") == 0) {
		en = 0;
	} else
		goto proc_atm_cfg_help;

	if (vrx_strcmpi(param_list[1], "ps") == 0
		|| vrx_strcmpi(param_list[1], "powersaving") == 0) {
		atm_power_saving(priv, en, 1);
	}

	return count;

proc_atm_cfg_help:
	pr_info("echo [enable | disable] powersaving > cfg\n");
	return count;
}

static const struct file_operations atm_cfg_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_atm_cfg_seq_open,
	.read		= seq_read,
	.write		= atm_cfg_proc_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static ssize_t proc_write_atm_wanmib(struct file *file, const char __user *buf,
				size_t count, loff_t *data)
{
	struct atm_priv *priv;
	char str[32];
	int len, rlen;
	int num;
	char *param_list[20];

	priv = (struct atm_priv *)PDE_DATA(file_inode(file));
	if (priv == NULL) {
		pr_err("%s: Invalid private data\n", __func__);
		return -EINVAL;
	}
	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;
	num = vrx_split_buffer(str, param_list, ARRAY_SIZE(param_list));
	if ((vrx_strcmpi(param_list[0], "clear") == 0)
		|| (vrx_strcmpi(param_list[0], "clean") == 0)
		|| (vrx_strcmpi(param_list[0], "c") == 0)) {
		proc_clear_atm_wanmib(priv);
	} else {
		goto proc_atm_wanmib_help;
	}

	return count;

proc_atm_wanmib_help:
	pr_info("echo clear/clean/c > wanmib\n");
return count;

}

static int proc_read_atm_wanmib_seq_open(struct inode *inode,
	struct file *file)
{
	return single_open(file, proc_read_atm_wanmib, PDE_DATA(inode));
}



static const struct file_operations atm_wanmib_proc_fops = {
	.owner      = THIS_MODULE,
	.open       = proc_read_atm_wanmib_seq_open,
	.read       = seq_read,
	.write      = proc_write_atm_wanmib,
	.llseek     = seq_lseek,
	.release    = single_release,
};

static int proc_read_htu_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_htu, PDE_DATA(inode));
}

static const struct file_operations htu_proc_fops = {
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

static const struct file_operations queue_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_queue_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static void set_q_prio(struct atm_priv *priv,
	int conn, int prio, int queue)
{
	if (priv == NULL) {
		pr_err("<%s>: Invalid private data\n", __func__);
		return;
	}
	if (conn < 0 || conn >= ATM_PVC_NUMBER
		|| prio < 0 || prio >= ATM_PRIO_Q_NUM) {
		tc_err(priv->tc_priv, MSG_INIT,
			"invalid parameter: conn: %d, prio: %d\n",
			conn, prio);
		return;
	}

	tc_dbg(priv->tc_priv, MSG_INIT,
		"conn: %d, prio: %d, queue: %d\n", conn, prio, queue);
	spin_lock_bh(&priv->atm_lock);
	if (priv->conn[conn].sw_txq_tbl & BIT(queue))
		priv->conn[conn].prio_queue_map[prio] = queue;
	spin_unlock_bh(&priv->atm_lock);
}

static ssize_t proc_atm_write_prio(struct file *file, const char __user *buf,
				size_t count, loff_t *data)
{
	struct atm_priv *priv;
	char local_buf[256];
	int len;
	char *p1;
	int conn = -1;
	unsigned int vpi, vci;
	int prio = -1;
	int queue = -1;
	int ret, num;
	int i;
	char *param_list[20];

	len = sizeof(local_buf) < count ? sizeof(local_buf) - 1 : count;
	len = len - copy_from_user(local_buf, buf, len);
	local_buf[len] = 0;
	p1 = local_buf;

	num = vrx_split_buffer(local_buf, param_list, ARRAY_SIZE(param_list));
	priv =  (struct atm_priv *)PDE_DATA(file_inode(file));
	if (vrx_strcmpi(param_list[0], "help") == 0)
		goto proc_atm_prio_help;
	else if (vrx_strcmpi(param_list[0], "pvc") == 0) {
		char *p2;
		p1 += 4;
		len -= 4;
		p2 = param_list[1];

		vpi = get_num_from_str(&p2, &len, 0);
		vci = get_num_from_str(&p2, &len, 0);
		conn = find_vpivci(vpi, vci);
		tc_dbg(priv->tc_priv, MSG_INIT, "VCI[%d]VCI[%d]", vpi, vci);
		if (!conn_valid(conn)) {
			tc_err(priv->tc_priv,
				MSG_INIT, "PVC(%d:%d) is invalid\n", vpi, vci);
			return count;
		}
		if (vrx_strcmpi(param_list[2], "add") == 0) {
			ret = sw_tx_queue_add(priv, conn);
			if (ret < 0)
				tc_err(priv->tc_priv, MSG_INIT,
				"Failed in adding queue for PVC(%d:%d)\n",
				vpi, vci);
			else
				tc_info(priv->tc_priv, MSG_INIT,
				"Add queue for PVC(%d:%d) successfully\n",
				vpi, vci);
		} else if (vrx_strcmpi(param_list[2], "del") == 0
			|| vrx_strcmpi(param_list[2], "rem") == 0) {
			ret = sw_tx_queue_del(priv, conn);
			if (ret < 0)
				tc_err(priv->tc_priv, MSG_INIT,
				"Failed in deleting queue for PVC(%d:%d)\n",
				vpi, vci);
			else
				tc_info(priv->tc_priv, MSG_INIT,
				"Delete queue for PVC(%d:%d) successfully\n",
				vpi, vci);
		} else {
			prio = queue = -1;
			for (i = 2; i < num; i += 4) {
				if ((vrx_strcmpi(
					param_list[i], "prio") == 0)
					|| (vrx_strcmpi(
						param_list[i], "p") == 0))
					prio = vrx_atoi(param_list[i+1]);
				else
					tc_err(priv->tc_priv, MSG_INIT,
					"Error: expecting prio\n");

				if ((vrx_strcmpi(
					param_list[i+2], "queue") == 0) ||
					(vrx_strcmpi(
					param_list[i+2], "q") == 0))
					queue = vrx_atoi(param_list[i+3]);
				else
					tc_err(priv->tc_priv, MSG_INIT,
						"Error: expecting queue\n");

				if (prio >= 0 && queue >= 0)
					set_q_prio(priv, conn, prio, queue);
			}
		}
	} else
		pr_info("Wrong format!, Please try echo help > prio\n");

	return count;

proc_atm_prio_help:
	pr_info("echo pvc vpi.vci prio xx queue xx [prio xx queue xx] > prio\n");
	pr_info("echo pvc vpi.vci <add/del> > prio\n");
	return count;
}

static int proc_atm_read_prio(struct seq_file *seq, void *v)
{
	int i, j;
	struct atm_priv *priv = (struct atm_priv *)seq->private;
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

static int pvc_mib_seq_show(struct seq_file *seq, void *v)
{
	struct atm_pvc *pvc = (struct atm_pvc *)v;
	int j;
	struct atm_priv *priv = seq->private;

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
	struct atm_priv *priv = seq->private;

	if (!priv->pvc_tbl)
		return NULL;

	if (*pos >= ATM_PVC_NUMBER)
		return NULL;

	return &priv->conn[*pos];
}

static void *pvc_mib_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct atm_priv *priv = seq->private;

	if (++*pos < ATM_PVC_NUMBER)
		return &priv->conn[*pos];

	return NULL;
}

static void pvc_mib_seq_stop(struct seq_file *seq, void *v)
{
	return;
}

static int proc_atm_read_prio_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_atm_read_prio, PDE_DATA(inode));
}

static const struct seq_operations pvc_mib_seq_ops = {
	.start = pvc_mib_seq_start,
	.next = pvc_mib_seq_next,
	.stop = pvc_mib_seq_stop,
	.show = pvc_mib_seq_show,
};

static const struct file_operations atm_prio_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_atm_read_prio_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.write		= proc_atm_write_prio,
	.release	= single_release,
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

static const struct file_operations atm_pvc_mib_proc_fops = {
	.owner	= THIS_MODULE,
	.open	= proc_read_pvc_mib_seq_open,
	.read	= seq_read,
	.llseek	= seq_lseek,
	.release = seq_release,
};

static ssize_t proc_write_cell(struct file *file,
		const char __user *buf, size_t count, loff_t *data
	)
{
	char local_buf[128];
	int len;
	char *p2;
	int conn = -1;
	unsigned int vpi = 0, vci = 0, gfc = 0, pti = 0, clp = 0;
	unsigned char cell[CELL_SIZE] = {0};
	struct uni_cell_header *header = (struct uni_cell_header *)cell;
	unsigned int pos = sizeof(*header);
	unsigned long sys_flag;
	char *param_list[20];
	int ret, num;
	struct atm_priv *priv;
	int i;

	len = sizeof(local_buf) < count ? sizeof(local_buf) - 1 : count;
	len = len - copy_from_user(local_buf, buf, len);
	local_buf[len] = 0;

	num = vrx_split_buffer(local_buf, param_list, ARRAY_SIZE(param_list));
	priv =  (struct atm_priv *)PDE_DATA(file_inode(file));
	if (priv == NULL) {
		pr_err("<%s>: Invalid private data\n", __func__);
		return count;
	}
	if (vrx_strcmpi(param_list[0], "help") == 0)
		goto proc_atm_cell_help;
	else {
		if (vrx_strcmpi(param_list[0], "pvc") == 0) {
			p2 = param_list[1];
			vpi = get_num_from_str(&p2, &len, 0);
			p2 += 1;
			vci = get_num_from_str(&p2, &len, 0);
			if (vci == 0x03 || vci == 0x04)
				conn = find_vpi(vpi);
			else
				conn = find_vpivci(vpi, vci);
			tc_dbg(priv->tc_priv,
				MSG_INIT,
				"vpi = %u, vci = %u, conn = %d",
				vpi, vci, conn);
		}
		i = 0;
		pos = 0;
		while ((i + 2) < num) {
			if (vrx_strcmpi(param_list[2 + i], "pti") == 0) {
				if (vrx_strcmpi(param_list[3 + i],
					"ATM_PTI_SEGF5") == 0)
					pti = ATM_PTI_SEGF5;
				else if (vrx_strcmpi(param_list[3 + i],
					"SEGF5") == 0)
					pti = ATM_PTI_SEGF5;
				else if (vrx_strcmpi(param_list[3 + i],
					"ATM_PTI_E2EF5") == 0)
					pti = ATM_PTI_E2EF5;
				else if (vrx_strcmpi(param_list[3 + i],
					"E2EF5") == 0)
					pti = ATM_PTI_E2EF5;
				else
					pti = get_num_from_str(
						&param_list[3 + i],
						&len,
						1);
				i++;
			} else if (vrx_strcmpi(param_list[2 + i], "gfc") == 0) {
				gfc = get_num_from_str(
						&param_list[3 + i],
						&len,
						1);
				i++;
			} else if (vrx_strcmpi(param_list[2 + i], "clp") == 0) {
				clp = get_num_from_str(
					&param_list[3 + i],
					&len,
					1);
				i++;
			} else if (pos + ATM_HEADER_SIZE < ARRAY_SIZE(cell)) {
				p2 = param_list[2 + i];
				cell[pos + ATM_HEADER_SIZE] =
					get_num_from_str(&p2,
						&len,
						1);
				pos++;
			}
			i++;
		}
	}
	if (conn_valid(conn)) {
		header->gfc = gfc;
		header->vpi = vpi;
		header->vci = vci;
		header->pti = pti;
		header->clp = clp;

		pr_info(
			"Header gfc[%x] vpi[%x] vci[%x] pti[%x] clp[%x]\n",
			header->gfc,
			header->vpi,
			header->vci,
			header->pti,
			header->clp);
		ret = ppe_send_oam(priv->conn[conn].vcc, cell, 0);
		local_irq_save(sys_flag);
		for (i = 0; i < ARRAY_SIZE(cell); i++) {
			if (i == 0)
				pr_info("  cell");
			if (i % 8 == 0 && i != 0)
				pr_info("\n       ");
			pr_info(" %02x",
				(unsigned int)cell[i]);
		}
		if (i % 8 != 0)
			pr_info("\n");
		local_irq_restore(sys_flag);
	}
	return count;
proc_atm_cell_help:
	pr_info("echo <pvc vpi.vci> [pti] [gfc] [clp] <hex data> > cell\n");
	return count;
}

static int proc_read_cell(struct seq_file *seq, void *v)
{
	return 0;
}

static int proc_read_cell_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_cell, NULL);
}

static const struct file_operations atm_cell_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_cell_seq_open,
	.read		= seq_read,
	.write		= proc_write_cell,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static struct tc_proc_list atm_procs[] = {
	{TC_PROC_ATM_DIR, 0, NULL, 1},
	{"cfg",		0644,	&atm_cfg_proc_fops,	0},
	{"mem",		0644,	&mem_proc_fops,		0},
	{"pp32",	0644,	&pp32_proc_fops,	0},
	{"wanmib",	0644,	&atm_wanmib_proc_fops,	0},
	{"htu",		0644,	&htu_proc_fops,		0},
	{"queue",	0644,	&queue_proc_fops,	0},
	{"prio",	0644,	&atm_prio_proc_fops,	0},
	{"pvc_mib",	0644,	&atm_pvc_mib_proc_fops,	0},
	{"cell",	0644,	&atm_cell_proc_fops,	0},
	{"fwdbg",	0644,	&fwdbg_proc_fops,	0},
};

int atm_tc_proc_init(void *atm_priv)
{
	struct proc_dir_entry *entry;
	struct atm_priv *priv = (struct atm_priv *)atm_priv;
	struct proc_dir_entry *tc_dir = NULL;
	struct tc_proc_list *p;
	int i, j;

	for (i = 0; i < ARRAY_SIZE(atm_procs); i++) {
		p = &atm_procs[i];
		if (p->is_folder) {
			tc_dir = proc_mkdir(p->proc_name,
				priv->tc_priv->proc_dir);
			if (!tc_dir) {
				tc_err(priv->tc_priv,
					MSG_INIT,
					"failed to create %s",
					p->proc_name);
				return -ENOMEM;
			}
		} else {
			entry = proc_create_data(p->proc_name, p->mode,
					tc_dir, p->fops, (void *)priv);
			if (!entry) {
				tc_err(priv->tc_priv,
					MSG_INIT,
					"failed to create %s",
					p->proc_name);
				goto atm_proc_err;
			}
		}
	}

	priv->proc_dir = tc_dir;

	return 0;

atm_proc_err:
	for (j = i - 1; j >= 0; j--) {
		p = &atm_procs[j];
		if (!p->is_folder)
			remove_proc_entry(p->proc_name, tc_dir);
		else
			remove_proc_entry(p->proc_name,
				priv->tc_priv->proc_dir);
	}
	return -ENOMEM;
}

int atm_tc_proc_exit(void *atm_priv)
{
	struct atm_priv *priv = (struct atm_priv *)atm_priv;
	struct tc_proc_list *p;
	int i;
	for (i = (ARRAY_SIZE(atm_procs) - 1); i >= 0; i--) {
		p = &atm_procs[i];
		if (!p->is_folder)
			remove_proc_entry(p->proc_name, priv->proc_dir);
		else
			remove_proc_entry(p->proc_name,
				priv->tc_priv->proc_dir);
	}
	return 0;
}

