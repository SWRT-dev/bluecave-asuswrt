#include <linux/module.h>
#include <lantiq.h>
#include <net/datapath_proc_api.h>	/*for proc api */
#include <net/datapath_api.h>
#include "datapath_pmac.h"
#include <net/drv_tmu_ll.h>
#include <net/lantiq_cbm.h>
#include <net/lantiq_cbm_api.h>
#include <asm/ltq_vmb.h>	/*vmb */
#include <asm/ltq_itc.h>	/*mips itc */
#include <linux/list.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <xway/switch-api/lantiq_gsw_api.h>
#include <xway/switch-api/lantiq_gsw_flow.h>

#include "../cbm/reg/fsqm.h"	/* hardcoded path  */

#include "datapath.h"
#include "datapath_mib.h"

#define DP_PROC_NAME       "dp"
#define DP_PROC_BASE       "/proc/"DP_PROC_NAME"/"
#define DP_PROC_PARENT     ""

#define DP_PROC_FILE_DBG   "dbg"
#define DP_PROC_FILE_PORT   "port"
#define DP_PROC_FILE_PARSER "parser"
#define DP_PROC_FILE_RMON_PORTS  "rmon"
#define DP_PROC_FILE_MEM "mem"
#define DP_PROC_FILE_EP "ep"	/*EP/port ID info */
#define DP_PROC_FILE_DPORT "dport"	/*TMU dequeue port info */
#define DP_PROC_PRINT_MODE "print_mode"
#define DP_PROC_FILE_CHECKSUM "checksum"
#define DP_PROC_FILE_MIB_TIMER "mib_timer"
#define DP_PROC_FILE_MIB_INSIDE "mib_inside"
#define DP_PROC_FILE_MIBPORT "mib_port"
#define DP_PROC_FILE_MIBVAP "mib_vap"
#define DP_PROC_FILE_COC "coc"
#define DP_PROC_FILE_COMMON_CMD "cmd"
#define DP_PROC_FILE_CBM_BUF_TEST   "cbm_buf"
#define DP_PROC_FILE_PCE  "pce"
#define DP_PROC_FILE_ROUTE  "route"
#define DP_PROC_FILE_PMAC  "pmac"
#define DP_PROC_FILE_DT  "dt"


#undef NIPQUAD
#define NIPQUAD(addr) (\
	((unsigned char *)&addr)[0], \
	((unsigned char *)&addr)[1], \
	((unsigned char *)&addr)[2], \
	((unsigned char *)&addr)[3])

#undef NIPQUAD_FMT
#define NIPQUAD_FMT ("%u.%u.%u.%u")

#undef NIP6
#define NIP6(addr) (\
	ntohs(((unsigned short *)addr)[0]), \
	ntohs(((unsigned short *)addr)[1]), \
	ntohs(((unsigned short *)addr)[2]), \
	ntohs(((unsigned short *)addr)[3]), \
	ntohs(((unsigned short *)addr)[4]), \
	ntohs(((unsigned short *)addr)[5]), \
	ntohs(((unsigned short *)addr)[6]), \
	ntohs(((unsigned short *)addr)[7]))

#undef NIP6_FMT
#define NIP6_FMT ("%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x")

#define smart_proc_printf(s, fmt, arg...) \
	do { \
		if (!s) \
			PRINTK(fmt, ##arg); \
		else \
			SEQ_PRINTF(s, fmt, ##arg); \
	} while (0)

static ssize_t proc_port_write(struct file *file, const char *buf,
			       size_t count, loff_t *ppos);
#if defined(CONFIG_LTQ_DATAPATH_DBG) && CONFIG_LTQ_DATAPATH_DBG
static void proc_dbg_read(struct seq_file *s);
static ssize_t proc_dbg_write(struct file *, const char *, size_t, loff_t *);
#endif
static int proc_port_dump(struct seq_file *s, int pos);
static void proc_parser_read(struct seq_file *s);
static ssize_t proc_parser_write(struct file *, const char *, size_t,
				 loff_t *);
static int proc_gsw_rmon_port_start(void);
static int proc_gsw_port_rmon_dump(struct seq_file *s, int pos);
static ssize_t proc_gsw_rmon_write(struct file *file, const char *buf,
				   size_t count, loff_t *ppos);
static int proc_write_mem(struct file *, const char *, size_t, loff_t *);
static int proc_dport_dump(struct seq_file *s, int pos);
static int proc_ep_dump(struct seq_file *s, int pos);
static ssize_t ep_port_write(struct file *, const char *, size_t, loff_t *);
static void proc_checksum_read(struct seq_file *s);
static int proc_common_cmd_dump(struct seq_file *s, int pos);
static int proc_common_cmd_start(void);
static ssize_t proc_checksum_write(struct file *file, const char *buf,
				   size_t count, loff_t *ppos);
static ssize_t proc_cbm_buf_write(struct file *file, const char *buf,
				  size_t count, loff_t *ppos);
static void proc_cbm_buf_read(struct seq_file *s);
static int proc_gsw_pce_dump(struct seq_file *s, int pos);
static int proc_gsw_pce_start(void);
static ssize_t proc_gsw_route_write(struct file *file, const char *buf,
				    size_t count, loff_t *ppos);
static ssize_t proc_gsw_pmac_write(struct file *file, const char *buf,
				    size_t count, loff_t *ppos);
static int inet_pton4(const char *src, u_char *dst);
static int inet_pton6(const char *src, u_char *dst);
static int proc_gsw_route_dump(struct seq_file *seq, int pos);
static void print_device_tree_node(struct device_node *node, int depth);
static ssize_t proc_dt_write(struct file *file, const char *buf,
	size_t count, loff_t *ppos);

static int rmon_display_tmu_mib = 1;
static int rmon_display_port_full;
static struct dp_proc_entry dp_proc_entries[] = {
	/*name single_callback_t multi_callback_t/_start write_callback_t */
#if defined(CONFIG_LTQ_DATAPATH_DBG) && CONFIG_LTQ_DATAPATH_DBG
	{DP_PROC_FILE_DBG, proc_dbg_read, NULL, NULL, proc_dbg_write},
#endif
	{DP_PROC_FILE_PORT, NULL, proc_port_dump, NULL, proc_port_write},
	{DP_PROC_FILE_PARSER, proc_parser_read, NULL, NULL,
	 proc_parser_write},
	{DP_PROC_FILE_RMON_PORTS, NULL, proc_gsw_port_rmon_dump,
	 proc_gsw_rmon_port_start, proc_gsw_rmon_write},
	{DP_PROC_FILE_MEM, NULL, NULL, NULL, proc_write_mem},
	{DP_PROC_FILE_EP, NULL, proc_ep_dump, NULL, ep_port_write},
	{DP_PROC_FILE_DPORT, NULL, proc_dport_dump, NULL, NULL},
	{DP_PROC_PRINT_MODE, proc_print_mode_read, NULL, NULL,
	 proc_print_mode_write},
	{DP_PROC_FILE_CHECKSUM, proc_checksum_read, NULL, NULL,
	 proc_checksum_write},
#ifdef CONFIG_LTQ_DATAPATH_MIB
	{DP_PROC_FILE_MIB_TIMER, proc_mib_timer_read, NULL, NULL,
	 proc_mib_timer_write},
	{DP_PROC_FILE_MIB_INSIDE, NULL, proc_mib_inside_dump,
	 proc_mib_inside_start, proc_mib_inside_write},
	{DP_PROC_FILE_MIBPORT, NULL, proc_mib_port_dump,
	 proc_mib_port_start, proc_mib_port_write},
	{DP_PROC_FILE_MIBVAP, NULL, proc_mib_vap_dump, proc_mib_vap_start,
	 proc_mib_vap_write},
#endif
#ifdef CONFIG_LTQ_DATAPATH_CPUFREQ
	{DP_PROC_FILE_COC, proc_coc_read, NULL, NULL, proc_coc_write},
#endif
	{DP_PROC_FILE_CBM_BUF_TEST, proc_cbm_buf_read, NULL, NULL,
	 proc_cbm_buf_write},
	{DP_PROC_FILE_COMMON_CMD, NULL, proc_common_cmd_dump,
	 proc_common_cmd_start, NULL},
	{DP_PROC_FILE_PCE, NULL, proc_gsw_pce_dump, proc_gsw_pce_start, NULL},
	{DP_PROC_FILE_ROUTE, NULL, proc_gsw_route_dump, NULL,
	 proc_gsw_route_write},
	 {DP_PROC_FILE_PMAC, NULL, NULL, NULL,
	 proc_gsw_pmac_write},
	 {DP_PROC_FILE_DT, NULL, NULL, NULL, proc_dt_write},

	/*the last place holder */
	{NULL, NULL, NULL, NULL, NULL}
};

static struct proc_dir_entry *dp_proc_node;

struct proc_dir_entry *dp_proc_install(void)
{

	dp_proc_node = proc_mkdir(DP_PROC_PARENT DP_PROC_NAME, NULL);

	if (dp_proc_node != NULL) {
		int i;

		for (i = 0; i < ARRAY_SIZE(dp_proc_entries); i++)
			dp_proc_entry_create(dp_proc_node,
					     &dp_proc_entries[i]);
	} else {
		PRINTK("cannot create proc entry");
		return NULL;
	}

	return dp_proc_node;
}

int proc_port_dump(struct seq_file *s, int pos)
{
	int i;
	int ret;
	struct pmac_port_info *port = get_port_info(pos);

	if (!port) {
		PR_ERR("Why port is NULL\n");
		return -1;
	}

	if (port->status == PORT_FREE) {
		if (pos == 0) {
			SEQ_PRINTF(s,
				   "Reserved Port: rx_err_drop=0x%08x  tx_err_drop=0x%08x\n",
				   port->rx_err_drop, port->tx_err_drop);

		} else
			SEQ_PRINTF(s,
				   "%02d: rx_err_drop=0x%08x  tx_err_drop=0x%08x\n",
				   pos, port->rx_err_drop, port->tx_err_drop);

		goto EXIT;
	}

	SEQ_PRINTF(s,
		   "%02d: module=0x0x%0x(name:%8s) dev_port=%02d dp_port=%02d itf_base=%d(%s)\n",
		   pos, (u32) port->owner, port->owner->name, port->dev_port,
		   port->port_id,
		   port->itf_info ? port->itf_info->start : 0,
	       port->itf_info ? "Enabled" : "Not Enabled");
	SEQ_PRINTF(s, "    status:            %s\n",
		   dp_port_status_str[port->status]);

	SEQ_PRINTF(s, "    allocate_flags:    ");

	for (i = 0; i < get_dp_port_type_str_size(); i++) {
		if (port->alloc_flags & dp_port_flag[i])
			SEQ_PRINTF(s, "%s ", dp_port_type_str[i]);
	}

	SEQ_PRINTF(s, "\n");

	SEQ_PRINTF(s, "    cb->rx_fn:         0x%0x\n", (u32) port->cb.rx_fn);
	SEQ_PRINTF(s, "    cb->restart_fn:    0x%0x\n",
		   (u32) port->cb.restart_fn);
	SEQ_PRINTF(s, "    cb->stop_fn:       0x%0x\n",
		   (u32) port->cb.stop_fn);
	SEQ_PRINTF(s, "    cb->get_subifid_fn:0x%0x\n",
		   (u32) port->cb.get_subifid_fn);
	SEQ_PRINTF(s, "    num_subif:         %02d\n", port->num_subif);

	for (i = 0; i < MAX_SUBIF_PER_PORT; i++) {
		if (port->subif_info[i].flags) {
			SEQ_PRINTF(s,
				   "      [%02d]: subif=0x%04x(vap=%d) netif=0x%0x(name=%s), device_name=%s\n",
				   i, port->subif_info[i].subif,
				   (port->subif_info[i].subif >> VAP_OFFSET)
				   & 0xF, (u32) port->subif_info[i].netif,
				   port->subif_info[i].netif ? port->
				   subif_info[i].netif->name : "NULL/DSL",
				   port->subif_info[i].device_name);
			SEQ_PRINTF(s, "          : rx_fn_rxif_pkt =0x%08x\n",
				   port->subif_info[i].mib.rx_fn_rxif_pkt);
			SEQ_PRINTF(s, "          : rx_fn_txif_pkt =0x%08x\n",
				   port->subif_info[i].mib.rx_fn_txif_pkt);
			SEQ_PRINTF(s, "          : rx_fn_dropped  =0x%08x\n",
				   port->subif_info[i].mib.rx_fn_dropped);
			SEQ_PRINTF(s, "          : tx_cbm_pkt     =0x%08x\n",
				   port->subif_info[i].mib.tx_cbm_pkt);
			SEQ_PRINTF(s, "          : tx_tso_pkt     =0x%08x\n",
				   port->subif_info[i].mib.tx_tso_pkt);
			SEQ_PRINTF(s, "          : tx_pkt_dropped =0x%08x\n",
				   port->subif_info[i].mib.tx_pkt_dropped);
			SEQ_PRINTF(s, "          : tx_clone_pkt   =0x%08x\n",
				   port->subif_info[i].mib.tx_clone_pkt);
			SEQ_PRINTF(s, "          : tx_hdr_room_pkt=0x%08x\n",
				   port->subif_info[i].mib.tx_hdr_room_pkt);
		}
	}

	ret =
	    SEQ_PRINTF(s, "    rx_err_drop=0x%08x  tx_err_drop=0x%08x\n",
		       port->rx_err_drop, port->tx_err_drop);

	if (ret)
		return pos;

 EXIT:
	pos++;

	if (pos >= PMAC_MAX_NUM)
		pos = -1;	/*end of the loop */

	return pos;
}

int display_port_info(u8 pos, int start_vap, int end_vap, u32 flag)
{
	int i;
	int ret;
	struct pmac_port_info *port = get_port_info(pos);

	if (!port) {
		PR_ERR("Why port is NULL\n");
		return -1;
	}

	if (port->status == PORT_FREE) {
		if (pos == 0) {
			PRINTK
			    ("Reserved Port: rx_err_drop=0x%08x  tx_err_drop=0x%08x\n",
			     port->rx_err_drop, port->tx_err_drop);

		} else
			PRINTK
			    ("%02d: rx_err_drop=0x%08x  tx_err_drop=0x%08x\n",
			     pos, port->rx_err_drop, port->tx_err_drop);

		goto EXIT;
	}

	PRINTK("%02d: module=0x0x%0x(name:%8s) dev_port=%02d dp_port=%02d itf_base=%d(%s)\n",
	       pos, (u32) port->owner, port->owner->name, port->dev_port,
	       port->port_id,
	       port->itf_info ? port->itf_info->start : 0,
	       port->itf_info ? "Enabled" : "Not Enabled");
	PRINTK("    status:            %s\n",
	       dp_port_status_str[port->status]);

	PRINTK("    allocate_flags:    ");

	for (i = 0; i < get_dp_port_type_str_size(); i++) {
		if (port->alloc_flags & dp_port_flag[i])
			PRINTK("%s ", dp_port_type_str[i]);
	}

	PRINTK("\n");

	if (!flag) {
		PRINTK("    cb->rx_fn:         0x%0x\n",
		       (u32) port->cb.rx_fn);
		PRINTK("    cb->restart_fn:    0x%0x\n",
		       (u32) port->cb.restart_fn);
		PRINTK("    cb->stop_fn:       0x%0x\n",
		       (u32) port->cb.stop_fn);
		PRINTK("    cb->get_subifid_fn:0x%0x\n",
		       (u32) port->cb.get_subifid_fn);
		PRINTK("    num_subif:         %02d\n", port->num_subif);
	}

	for (i = start_vap; i < end_vap; i++) {
		if (port->subif_info[i].flags) {
			PRINTK
			    ("      [%02d]: subif=0x%04x(vap=%d) netif=0x%0x(name=%s), device_name=%s\n",
			     i, port->subif_info[i].subif,
			     (port->subif_info[i].subif >> VAP_OFFSET)
			     & 0xF, (u32) port->subif_info[i].netif,
			     port->subif_info[i].netif ? port->subif_info[i].
			     netif->name : "NULL/DSL",
			     port->subif_info[i].device_name);
			PRINTK("          : rx_fn_rxif_pkt =0x%08x\n",
			       port->subif_info[i].mib.rx_fn_rxif_pkt);
			PRINTK("          : rx_fn_txif_pkt =0x%08x\n",
			       port->subif_info[i].mib.rx_fn_txif_pkt);
			PRINTK("          : rx_fn_dropped  =0x%08x\n",
			       port->subif_info[i].mib.rx_fn_dropped);
			PRINTK("          : tx_cbm_pkt     =0x%08x\n",
			       port->subif_info[i].mib.tx_cbm_pkt);
			PRINTK("          : tx_tso_pkt     =0x%08x\n",
			       port->subif_info[i].mib.tx_tso_pkt);
			PRINTK("          : tx_pkt_dropped =0x%08x\n",
			       port->subif_info[i].mib.tx_pkt_dropped);
			PRINTK("          : tx_clone_pkt   =0x%08x\n",
			       port->subif_info[i].mib.tx_clone_pkt);
			PRINTK("          : tx_hdr_room_pkt=0x%08x\n",
			       port->subif_info[i].mib.tx_hdr_room_pkt);
		}
	}

	ret =
	    PRINTK("    rx_err_drop=0x%08x  tx_err_drop=0x%08x\n",
		   port->rx_err_drop, port->tx_err_drop);
 EXIT:
	return 0;
}

ssize_t proc_port_write(struct file *file, const char *buf, size_t count,
			loff_t *ppos)
{
	int len;
	char str[64];
	int num, i;
	u8 index_start = 0;
	u8 index_end = PMAC_MAX_NUM;
	u8 vap_start = 0;
	u8 vap_end = MAX_SUBIF_PER_PORT;
	char *param_list[10];

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));

	if (num < 1)
		goto help;
	if (param_list[1]) {
		index_start = dp_atoi(param_list[1]);
		index_end = index_start + 1;
	}

	if (param_list[2]) {
		vap_start = dp_atoi(param_list[2]);
		vap_end = vap_start + 1;
	}

	if (index_start >= PMAC_MAX_NUM) {
		PR_ERR("wrong index: 0 ~ 15\n");
		return count;
	}

	if (vap_start >= MAX_SUBIF_PER_PORT) {
		PR_ERR("wrong VAP: 0 ~ 15\n");
		return count;
	}

	if (dp_strcmpi(param_list[0], "mib") == 0) {
		for (i = index_start; i < index_end; i++)
			display_port_info(i, vap_start, vap_end, 1);

	} else if (dp_strcmpi(param_list[0], "port") == 0) {
		for (i = index_start; i < index_end; i++)
			display_port_info(i, vap_start, vap_end, 0);

	} else
		goto help;

	return count;
 help:
	PRINTK("usage:\n");
	PRINTK("  echo mib  [ep][vap] > /prooc/dp/port\n");
	PRINTK("  echo port [ep][vap] > /prooc/dp/port\n");
	return count;
}

#if defined(CONFIG_LTQ_DATAPATH_DBG) && CONFIG_LTQ_DATAPATH_DBG
void proc_dbg_read(struct seq_file *s)
{
	int i;

	SEQ_PRINTF(s, "dp_dbg_flag=0x%08x\n", dp_dbg_flag);

	SEQ_PRINTF(s, "Supported Flags =%d\n", get_dp_dbg_flag_str_size());
	SEQ_PRINTF(s, "Enabled Flags(0x%0x):", dp_dbg_flag);

	for (i = 0; i < get_dp_dbg_flag_str_size(); i++)
		if ((dp_dbg_flag & dp_dbg_flag_list[i]) ==
		    dp_dbg_flag_list[i])
			SEQ_PRINTF(s, "%s ", dp_dbg_flag_str[i]);

	SEQ_PRINTF(s, "\n\n");

	SEQ_PRINTF(s, "dp_drop_all_tcp_err=%d @ 0x%p\n", dp_drop_all_tcp_err,
		   &dp_drop_all_tcp_err);
	SEQ_PRINTF(s, "dp_pkt_size_check=%d @ 0x%p\n", dp_pkt_size_check,
		   &dp_pkt_size_check);

	SEQ_PRINTF(s, "dp_rx_test_mode=%d @ 0x%p\n", dp_rx_test_mode,
		   &dp_rx_test_mode);
	SEQ_PRINTF(s, "dp_dbg_err(flat to print error or not)=%d @ 0x%p\n", dp_dbg_err,
		   &dp_dbg_err);
	print_parser_status(s);
}

ssize_t proc_dbg_write(struct file *file, const char *buf, size_t count,
		       loff_t *ppos)
{
	int len, i, j;
	char str[64];
	int num;
	char *param_list[20];
	int f_enable;

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));

	if (dp_strcmpi(param_list[0], "enable") == 0)
		f_enable = 1;
	else if (dp_strcmpi(param_list[0], "disable") == 0)
		f_enable = -1;
	else
		goto help;

	if (!param_list[1]) {	/*no parameter after enable or disable */
		set_ltq_dbg_flag(dp_dbg_flag, f_enable, -1);
		goto EXIT;
	}

	for (i = 1; i < num; i++) {
		for (j = 0; j < get_dp_dbg_flag_str_size(); j++)
			if (dp_strcmpi(param_list[i], dp_dbg_flag_str[j]) ==
			    0) {
				set_ltq_dbg_flag(dp_dbg_flag, f_enable,
						 dp_dbg_flag_list[j]);
				break;
			}
	}

 EXIT:
	return count;
 help:
	PRINTK("echo <enable/disable> ");

	for (i = 0; i < get_dp_dbg_flag_str_size(); i++)
		PRINTK("%s ", dp_dbg_flag_str[i]);

	PRINTK(" > /proc/dp/dbg\n");
	PRINTK(" display command: cat /proc/dp/cmd\n");

	return count;
}
#endif

static void proc_parser_read(struct seq_file *s)
{
	int8_t cpu, mpe1, mpe2, mpe3;

	dp_get_gsw_parser(&cpu, &mpe1, &mpe2, &mpe3);
	SEQ_PRINTF(s, "cpu : %s with parser size =%d bytes\n",
		   parser_flag_str(cpu), parser_size_via_index(0));
	SEQ_PRINTF(s, "mpe1: %s with parser size =%d bytes\n",
		   parser_flag_str(mpe1), parser_size_via_index(1));
	SEQ_PRINTF(s, "mpe2: %s with parser size =%d bytes\n",
		   parser_flag_str(mpe2), parser_size_via_index(2));
	SEQ_PRINTF(s, "mpe3: %s with parser size =%d bytes\n",
		   parser_flag_str(mpe3), parser_size_via_index(3));
}

ssize_t proc_parser_write(struct file *file, const char *buf, size_t count,
			  loff_t *ppos)
{
	int len;
	char str[64];
	int num, i;
	char *param_list[20];
	int8_t cpu = 0, mpe1 = 0, mpe2 = 0, mpe3 = 0, flag = 0;
	static int pce_rule_id;
	union gsw_var rule;

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));

	if (dp_strcmpi(param_list[0], "enable") == 0) {
		for (i = 1; i < num; i++) {
			if (dp_strcmpi(param_list[i], "cpu") == 0) {
				flag |= 0x1;
				cpu = 2;
			}

			if (dp_strcmpi(param_list[i], "mpe1") == 0) {
				flag |= 0x2;
				mpe1 = 2;
			}

			if (dp_strcmpi(param_list[i], "mpe2") == 0) {
				flag |= 0x4;
				mpe2 = 2;
			}

			if (dp_strcmpi(param_list[i], "mpe3") == 0) {
				flag |= 0x8;
				mpe3 = 2;
			}
		}

		if (!flag) {
			flag = 0x1 | 0x2 | 0x4 | 0x8;
			cpu = mpe1 = mpe2 = mpe3 = 2;
		}

		DP_DEBUG(DP_DBG_FLAG_DBG,
			 "flag=0x%x mpe3/2/1/cpu=%d/%d/%d/%d\n", flag, mpe3,
			 mpe2, mpe1, cpu);
		dp_set_gsw_parser(flag, cpu, mpe1, mpe2, mpe3);
	} else if (dp_strcmpi(param_list[0], "disable") == 0) {
		for (i = 1; i < num; i++) {
			if (dp_strcmpi(param_list[i], "cpu") == 0) {
				flag |= 0x1;
				cpu = 0;
			}

			if (dp_strcmpi(param_list[i], "mpe1") == 0) {
				flag |= 0x2;
				mpe1 = 0;
			}

			if (dp_strcmpi(param_list[i], "mpe2") == 0) {
				flag |= 0x4;
				mpe2 = 0;
			}

			if (dp_strcmpi(param_list[i], "mpe3") == 0) {
				flag |= 0x8;
				mpe3 = 0;
			}
		}

		if (!flag) {
			flag = 0x1 | 0x2 | 0x4 | 0x8;
			cpu = mpe1 = mpe2 = mpe3 = 0;
		}

		DP_DEBUG(DP_DBG_FLAG_DBG,
			 "flag=0x%x mpe3/2/1/cpu=%d/%d/%d/%d\n", flag, mpe3,
			 mpe2, mpe1, cpu);
		dp_set_gsw_parser(flag, cpu, mpe1, mpe2, mpe3);
	} else if (dp_strcmpi(param_list[0], "refresh") == 0) {
		dp_get_gsw_parser(NULL, NULL, NULL, NULL);
		PR_INFO("value:cpu=%d mpe1=%d mpe2=%d mpe3=%d\n", pinfo[0].v,
			pinfo[1].v, pinfo[2].v, pinfo[3].v);
		PR_INFO("size :cpu=%d mpe1=%d mpe2=%d mpe3=%d\n",
			pinfo[0].size, pinfo[1].size, pinfo[2].size,
			pinfo[3].size);
		return count;
	} else if (dp_strcmpi(param_list[0], "mark") == 0) {
		int flag = dp_atoi(param_list[1]);
		if (flag < 0)
			flag = 0;
		else if (flag > 3)
			flag = 3;
		PR_INFO("eProcessPath_Action set to %d\n", flag);
		/*: All packets set to same mpe flag as specified */
		memset(&rule, 0, sizeof(rule));
		rule.pce.pattern.nIndex = pce_rule_id;
		rule.pce.pattern.bEnable = 1;

		rule.pce.pattern.bParserFlagMSB_Enable = 1;
		/* rule.pce.pattern.nParserFlagMSB = 0x0021; */
		rule.pce.pattern.nParserFlagMSB_Mask = 0xffff;
		rule.pce.pattern.bParserFlagLSB_Enable = 1;
		/* rule.pce.pattern.nParserFlagLSB = 0x0000; */
		rule.pce.pattern.nParserFlagLSB_Mask = 0xffff;
		/* rule.pce.pattern.eDstIP_Select = 2; */
		/* in6_pton("ff00:0:0:0:0:0:0:0",-1,(void*)&rule.pce.pattern.nDstIP.nIPv6,-1,&end); */
		rule.pce.pattern.nDstIP_Mask = 0xffffffff;
		rule.pce.pattern.bDstIP_Exclude = 0;

		rule.pce.action.bRtDstPortMaskCmp_Action = 1;
		rule.pce.action.bRtSrcPortMaskCmp_Action = 1;
		rule.pce.action.bRtDstIpMaskCmp_Action = 1;
		rule.pce.action.bRtSrcIpMaskCmp_Action = 1;

		rule.pce.action.bRoutExtId_Action = 1;
		rule.pce.action.nRoutExtId = 0;	/*RT_EXTID_UDP; */
		rule.pce.action.bRtAccelEna_Action = 1;
		rule.pce.action.bRtCtrlEna_Action = 1;
		rule.pce.action.eProcessPath_Action = flag;
		rule.pce.action.bRMON_Action = 1;
		rule.pce.action.nRMON_Id = 0;	/*RMON_UDP_CNTR; */

		if (dp_gsw_kioctl
		    (GSWIP_R_DEV_NAME, GSW_PCE_RULE_WRITE, (u32) &rule)) {
			PRINTK("PCE rule add returned failure for GSW_PCE_RULE_WRITE\n");
			return count;
		}

	} else if (dp_strcmpi(param_list[0], "unmark") == 0) {
		/*: All packets set to same mpe flag as specified */
		memset(&rule, 0, sizeof(rule));
		rule.pce.pattern.nIndex = pce_rule_id;
		rule.pce.pattern.bEnable = 0;
		if (dp_gsw_kioctl
		    (GSWIP_R_DEV_NAME, GSW_PCE_RULE_WRITE, (u32) &rule)) {
			PRINTK("PCE rule add returned failure for GSW_PCE_RULE_WRITE\n");
			return count;
		}
	} else {
		PRINTK
		    ("Usage: echo <enable/disable> [cpu] [mpe1] [mpe2] [mpe3] > parser\n");
		PRINTK("Usage: echo <refresh> parser\n");

		PRINTK
		    ("Usage: echo mark eProcessPath_Action_value(0~3) > parser\n");
		PRINTK("Usage: echo unmark > parser\n");
		return count;
	}

	return count;
}

#define MAX_GSW_L_PMAC_PORT  7
#define MAX_GSW_R_PMAC_PORT  16
static GSW_RMON_Port_cnt_t gsw_l_rmon_mib[MAX_GSW_L_PMAC_PORT];
static GSW_RMON_Port_cnt_t gsw_r_rmon_mib[MAX_GSW_R_PMAC_PORT];
static GSW_RMON_Redirect_cnt_t gswr_rmon_redirect;
enum RMON_MIB_TYPE {
	RX_GOOD_PKTS = 0,
	RX_FILTER_PKTS,
	RX_DROP_PKTS,
	RX_OTHERS,

	TX_GOOD_PKTS,
	TX_ACM_PKTS,
	TX_DROP_PKTS,
	TX_OTHERS,

	REDIRECT_MIB,
	DP_DRV_MIB,

	/*last entry */
	RMON_MAX
};

static char f_q_mib_title_proc;

#define GSW_PORT_RMON_PRINT(res, title, var)  do { \
		res = SEQ_PRINTF(s, \
				 "%-14s%10s %12u %12u %12u %12u %12u %12u %12u\n", \
				 title, "L(0-6)", \
				 gsw_l_rmon_mib[0].var, gsw_l_rmon_mib[1].var, \
				 gsw_l_rmon_mib[2].var, gsw_l_rmon_mib[3].var, \
				 gsw_l_rmon_mib[4].var, gsw_l_rmon_mib[5].var, \
				 gsw_l_rmon_mib[6].var); \
		res = SEQ_PRINTF(s, \
				 "%-14s%10s %12u %12u %12u %12u %12u %12u %12u %12u\n", \
				 title, "R(0-6,15)", \
				 gsw_r_rmon_mib[0].var, gsw_r_rmon_mib[1].var, \
				 gsw_r_rmon_mib[2].var, gsw_r_rmon_mib[3].var, \
				 gsw_r_rmon_mib[4].var, gsw_r_rmon_mib[5].var, \
				 gsw_r_rmon_mib[6].var, gsw_r_rmon_mib[15].var); \
		res = SEQ_PRINTF(s, \
				 "%-14s%10s %12u %12u %12u %12u %12u %12u %12u %12u\n", \
				 title, "R(7-14)", \
				 gsw_r_rmon_mib[7].var, gsw_r_rmon_mib[8].var, \
				 gsw_r_rmon_mib[9].var, gsw_r_rmon_mib[10].var, \
				 gsw_r_rmon_mib[11].var, gsw_r_rmon_mib[12].var, \
				 gsw_r_rmon_mib[13].var, gsw_r_rmon_mib[14].var); \
		res = SEQ_PRINTF(s, "------------------------------------------------------------------------------------------------------------------------------------\n"); \
	} while (0)

int low_10dec(u64 x)
{
	char buf[26];
	char *p;
	int len;

	sprintf(buf, "%llu", x);
	len = strlen(buf);
	if (len >= 10)
		p = buf + len - 10;
	else
		p = buf;

	return dp_atoi(p);
}

int high_10dec(u64 x)
{
	char buf[26];
	int len;

	sprintf(buf, "%llu", x);
	len = strlen(buf);
	if (len >= 10)
		buf[len - 10] = 0;
	else
		buf[0] = 0;

	return dp_atoi(buf);
}

#define GSW_PORT_RMON64_PRINT(res, title, var)  do { \
		res = SEQ_PRINTF(s, \
				 "%-14s%10s %12u %12u %12u %12u %12u %12u %12u\n", \
				 title"(H)", "L(0-6)", \
				 high_10dec(gsw_l_rmon_mib[0].var), high_10dec(gsw_l_rmon_mib[1].var), \
				 high_10dec(gsw_l_rmon_mib[2].var), high_10dec(gsw_l_rmon_mib[3].var), \
				 high_10dec(gsw_l_rmon_mib[4].var), high_10dec(gsw_l_rmon_mib[5].var), \
				 high_10dec(gsw_l_rmon_mib[6].var)); \
		res = SEQ_PRINTF(s, \
				 "%-14s%10s %12u %12u %12u %12u %12u %12u %12u\n", \
				 title"(L)", "L(0-6)", \
				 low_10dec(gsw_l_rmon_mib[0].var), low_10dec(gsw_l_rmon_mib[1].var), \
				 low_10dec(gsw_l_rmon_mib[2].var), low_10dec(gsw_l_rmon_mib[3].var), \
				 low_10dec(gsw_l_rmon_mib[4].var), low_10dec(gsw_l_rmon_mib[5].var), \
				 low_10dec(gsw_l_rmon_mib[6].var)); \
		res = SEQ_PRINTF(s, \
				 "%-14s%10s %12u %12u %12u %12u %12u %12u %12u %12u\n", \
				 title"(H)", "R(0-6,15)", \
				 high_10dec(gsw_r_rmon_mib[0].var), high_10dec(gsw_r_rmon_mib[1].var), \
				 high_10dec(gsw_r_rmon_mib[2].var), high_10dec(gsw_r_rmon_mib[3].var), \
				 high_10dec(gsw_r_rmon_mib[4].var), high_10dec(gsw_r_rmon_mib[5].var), \
				 high_10dec(gsw_r_rmon_mib[6].var), high_10dec(gsw_r_rmon_mib[15].var)); \
		res = SEQ_PRINTF(s, \
				 "%-14s%10s %12u %12u %12u %12u %12u %12u %12u %12u\n", \
				 title"(L)", "R(0-6,15)", \
				 low_10dec(gsw_r_rmon_mib[0].var), low_10dec(gsw_r_rmon_mib[1].var), \
				 low_10dec(gsw_r_rmon_mib[2].var), low_10dec(gsw_r_rmon_mib[3].var), \
				 low_10dec(gsw_r_rmon_mib[4].var), low_10dec(gsw_r_rmon_mib[5].var), \
				 low_10dec(gsw_r_rmon_mib[6].var), low_10dec(gsw_r_rmon_mib[15].var)); \
		res = SEQ_PRINTF(s, \
				 "%-14s%10s %12u %12u %12u %12u %12u %12u %12u %12u\n", \
				 title"(H)", "R(7-14)", \
				 high_10dec(gsw_r_rmon_mib[7].var), high_10dec(gsw_r_rmon_mib[8].var), \
				 high_10dec(gsw_r_rmon_mib[9].var), high_10dec(gsw_r_rmon_mib[10].var), \
				 high_10dec(gsw_r_rmon_mib[11].var), high_10dec(gsw_r_rmon_mib[12].var), \
				 high_10dec(gsw_r_rmon_mib[13].var), high_10dec(gsw_r_rmon_mib[14].var)); \
		res = SEQ_PRINTF(s, \
				 "%-14s%10s %12u %12u %12u %12u %12u %12u %12u %12u\n", \
				 title"(L)", "R(7-14)", \
				 low_10dec(gsw_r_rmon_mib[7].var), low_10dec(gsw_r_rmon_mib[8].var), \
				 low_10dec(gsw_r_rmon_mib[9].var), low_10dec(gsw_r_rmon_mib[10].var), \
				 low_10dec(gsw_r_rmon_mib[11].var), low_10dec(gsw_r_rmon_mib[12].var), \
				 low_10dec(gsw_r_rmon_mib[13].var), low_10dec(gsw_r_rmon_mib[14].var)); \
		res = SEQ_PRINTF(s, "------------------------------------------------------------------------------------------------------------------------------------\n"); \
	} while (0)

int proc_gsw_port_rmon_dump(struct seq_file *s, int pos)
{
	int i;
	int ret = 0;
	GSW_API_HANDLE gsw_handle = 0;
	char flag_buf[20];

	if (pos == 0) {
		memset(gsw_r_rmon_mib, 0, sizeof(gsw_r_rmon_mib));
		memset(gsw_l_rmon_mib, 0, sizeof(gsw_l_rmon_mib));

		/*read gswip-r rmon counter */
		gsw_handle = gsw_api_kopen((char *)GSWIP_R_DEV_NAME);

		if (gsw_handle == 0) {
			PR_ERR("Open GSWIP-R device FAILED !\n");
			return -1;
		}

		for (i = 0; i < ARRAY_SIZE(gsw_r_rmon_mib); i++) {
			gsw_r_rmon_mib[i].nPortId = i;
			ret =
			    gsw_api_kioctl(gsw_handle, GSW_RMON_PORT_GET,
					   (u32) &gsw_r_rmon_mib[i]);

			if (ret != GSW_statusOk) {
				PR_ERR("RMON_PORT_GET fail for Port %d\n", i);
				return -1;
			}
		}

		/*read pmac rmon redirect mib */
		memset(&gswr_rmon_redirect, 0, sizeof(gswr_rmon_redirect));
		ret =
		    gsw_api_kioctl(gsw_handle, GSW_RMON_REDIRECT_GET,
				   (u32) &gswr_rmon_redirect);

		if (ret != GSW_statusOk) {
			PR_ERR("GSW_RMON_REDIRECT_GET fail for Port %d\n", i);
			return -1;
		}

		gsw_api_kclose(gsw_handle);

		/*read gswip-l rmon counter */
		gsw_handle = gsw_api_kopen((char *)GSWIP_L_DEV_NAME);

		if (gsw_handle == 0) {
			PR_ERR("Open GSWIP-L FAILED !!\n");
			return -1;
		}

		for (i = 0; i < ARRAY_SIZE(gsw_l_rmon_mib); i++) {
			gsw_l_rmon_mib[i].nPortId = i;
			ret =
			    gsw_api_kioctl(gsw_handle, GSW_RMON_PORT_GET,
					   (u32) &gsw_l_rmon_mib[i]);

			if (ret != GSW_statusOk) {
				PR_ERR("RMON_PORT_GET fail for Port %d\n", i);
				return -1;
			}
		}

		gsw_api_kclose(gsw_handle);

		ret =
		    SEQ_PRINTF(s,
			       "%-24s %12u %12u %12u %12u %12u %12u %12u\n",
			       "GSWIP-L", 0, 1, 2, 3, 4, 5, 6);
		ret =
		    SEQ_PRINTF(s,
			       "%-24s %12u %12u %12u %12u %12u %12u %12u %12u\n",
			       "GSWIP-R(Fixed)", 0, 1, 2, 3, 4, 5, 6, 15);
		ret =
		    SEQ_PRINTF(s,
			       "%-24s %12u %12u %12u %12u %12u %12u %12u %12u\n",
			       "GSWIP-R(Dynamic)", 7, 8, 9, 10, 11, 12, 13,
			       14);
		ret =
		    SEQ_PRINTF(s,
			       "------------------------------------------------------------------------------------------------------------------------------------\n");
	}

	if (pos == RX_GOOD_PKTS)
		GSW_PORT_RMON_PRINT(ret, "RX_Good", nRxGoodPkts);
	else if (pos == RX_FILTER_PKTS)
		GSW_PORT_RMON_PRINT(ret, "RX_FILTER", nRxFilteredPkts);
	else if (pos == RX_DROP_PKTS)
		GSW_PORT_RMON_PRINT(ret, "RX_DROP", nRxDroppedPkts);
	else if (pos == RX_OTHERS) {
		if (!rmon_display_port_full)
			goto NEXT;

		GSW_PORT_RMON_PRINT(ret, "RX_UNICAST", nRxUnicastPkts);
		GSW_PORT_RMON_PRINT(ret, "RX_BROADCAST", nRxBroadcastPkts);
		GSW_PORT_RMON_PRINT(ret, "RX_MULTICAST", nRxMulticastPkts);
		GSW_PORT_RMON_PRINT(ret, "RX_FCS_ERR", nRxFCSErrorPkts);
		GSW_PORT_RMON_PRINT(ret, "RX_UNDER_GOOD",
				    nRxUnderSizeGoodPkts);
		GSW_PORT_RMON_PRINT(ret, "RX_OVER_GOOD", nRxOversizeGoodPkts);
		GSW_PORT_RMON_PRINT(ret, "RX_UNDER_ERR",
				    nRxUnderSizeErrorPkts);
		GSW_PORT_RMON_PRINT(ret, "RX_OVER_ERR", nRxOversizeErrorPkts);
		GSW_PORT_RMON_PRINT(ret, "RX_ALIGN_ERR", nRxAlignErrorPkts);
		GSW_PORT_RMON_PRINT(ret, "RX_64B", nRx64BytePkts);
		GSW_PORT_RMON_PRINT(ret, "RX_127B", nRx127BytePkts);
		GSW_PORT_RMON_PRINT(ret, "RX_255B", nRx255BytePkts);
		GSW_PORT_RMON_PRINT(ret, "RX_511B", nRx511BytePkts);
		GSW_PORT_RMON_PRINT(ret, "RX_1023B", nRx1023BytePkts);
		GSW_PORT_RMON_PRINT(ret, "RX_MAXB", nRxMaxBytePkts);
		GSW_PORT_RMON64_PRINT(ret, "RX_BAD_b", nRxBadBytes);
	} else if (pos == TX_GOOD_PKTS)
		GSW_PORT_RMON_PRINT(ret, "TX_Good", nTxGoodPkts);
	else if (pos == TX_ACM_PKTS)
		GSW_PORT_RMON_PRINT(ret, "TX_ACM_DROP", nTxAcmDroppedPkts);
	else if (pos == TX_DROP_PKTS)
		GSW_PORT_RMON_PRINT(ret, "TX_Drop", nTxDroppedPkts);
	else if (pos == TX_OTHERS) {
		if (!rmon_display_port_full)
			goto NEXT;

		GSW_PORT_RMON_PRINT(ret, "TX_UNICAST", nTxUnicastPkts);
		GSW_PORT_RMON_PRINT(ret, "TX_BROADAST", nTxBroadcastPkts);
		GSW_PORT_RMON_PRINT(ret, "TX_MULTICAST", nTxMulticastPkts);
		GSW_PORT_RMON_PRINT(ret, "TX_SINGLE_COLL",
				    nTxSingleCollCount);
		GSW_PORT_RMON_PRINT(ret, "TX_MULT_COLL", nTxMultCollCount);
		GSW_PORT_RMON_PRINT(ret, "TX_LATE_COLL", nTxLateCollCount);
		GSW_PORT_RMON_PRINT(ret, "TX_EXCESS_COLL",
				    nTxExcessCollCount);
		GSW_PORT_RMON_PRINT(ret, "TX_COLL", nTxCollCount);
		GSW_PORT_RMON_PRINT(ret, "TX_PAUSET", nTxPauseCount);
		GSW_PORT_RMON_PRINT(ret, "TX_64B", nTx64BytePkts);
		GSW_PORT_RMON_PRINT(ret, "TX_127B", nTx127BytePkts);
		GSW_PORT_RMON_PRINT(ret, "TX_255B", nTx255BytePkts);
		GSW_PORT_RMON_PRINT(ret, "TX_511B", nTx511BytePkts);
		GSW_PORT_RMON_PRINT(ret, "TX_1023B", nTx1023BytePkts);
		GSW_PORT_RMON_PRINT(ret, "TX_MAX_B", nTxMaxBytePkts);
		GSW_PORT_RMON_PRINT(ret, "TX_UNICAST", nTxUnicastPkts);
		GSW_PORT_RMON_PRINT(ret, "TX_UNICAST", nTxUnicastPkts);
		GSW_PORT_RMON_PRINT(ret, "TX_UNICAST", nTxUnicastPkts);
		GSW_PORT_RMON64_PRINT(ret, "TX_GOOD_b", nTxGoodBytes);

	} else if (pos == REDIRECT_MIB) {
		/*GSWIP-R PMAC Redirect conter */
		ret =
		    SEQ_PRINTF(s, "%-24s %12s %12s %12s %12s\n",
			       "GSW-R Redirect", "Rx_Pkts", "Tx_Pkts",
			       "Rx_DropsPkts", "Tx_DropsPkts");

		ret =
		    SEQ_PRINTF(s, "%-24s %12d %12d %12d %12d\n", "",
			       gswr_rmon_redirect.nRxPktsCount,
			       gswr_rmon_redirect.nTxPktsCount,
			       gswr_rmon_redirect.nRxDiscPktsCount,
			       gswr_rmon_redirect.nTxDiscPktsCount);
		ret =
		    SEQ_PRINTF(s,
			       "------------------------------------------------------------------------------------------------------------------------------------\n");
	} else if (pos == DP_DRV_MIB) {
		u64 eth0_rx = 0, eth0_tx = 0;
		u64 eth1_rx = 0, eth1_tx = 0;
		u64 dsl_rx = 0, dsl_tx = 0;
		u64 other_rx = 0, other_tx = 0;
		int i, j;
		struct pmac_port_info *port;

		for (i = 1; i < PMAC_MAX_NUM; i++) {
			port = get_port_info(i);

			if (!port)
				continue;

			if (i < 6) {
				for (j = 0; j < 16; j++) {
					eth0_rx +=
					    port->subif_info[j].mib.
					    rx_fn_rxif_pkt;
					eth0_rx +=
					    port->subif_info[j].mib.
					    rx_fn_txif_pkt;
					eth0_tx +=
					    port->subif_info[j].mib.
					    tx_cbm_pkt;
					eth0_tx +=
					    port->subif_info[j].mib.
					    tx_tso_pkt;
				}
			} else if (i == 15) {
				for (j = 0; j < 16; j++) {
					eth1_rx +=
					    port->subif_info[j].mib.
					    rx_fn_rxif_pkt;
					eth1_rx +=
					    port->subif_info[j].mib.
					    rx_fn_txif_pkt;
					eth1_tx +=
					    port->subif_info[j].mib.
					    tx_cbm_pkt;
					eth1_tx +=
					    port->subif_info[j].mib.
					    tx_tso_pkt;
				}
			} else if (port->alloc_flags & DP_F_FAST_DSL) {
				for (j = 0; j < 16; j++) {
					dsl_rx +=
					    port->subif_info[j].mib.
					    rx_fn_rxif_pkt;
					dsl_rx +=
					    port->subif_info[j].mib.
					    rx_fn_txif_pkt;
					dsl_tx +=
					    port->subif_info[j].mib.
					    tx_cbm_pkt;
					dsl_tx +=
					    port->subif_info[j].mib.
					    tx_tso_pkt;
				}
			} else {
				for (j = 0; j < 16; j++) {
					other_rx +=
					    port->subif_info[j].mib.
					    rx_fn_rxif_pkt;
					other_rx +=
					    port->subif_info[j].mib.
					    rx_fn_txif_pkt;
					other_tx +=
					    port->subif_info[j].mib.
					    tx_cbm_pkt;
					other_tx +=
					    port->subif_info[j].mib.
					    tx_tso_pkt;
				}
			}
		}

		ret =
		    SEQ_PRINTF(s, "%-15s %22s %22s %22s %22s\n", "DP Drv Mib",
			       "ETH_LAN", "ETH_WAN", "DSL", "Others");

		ret =
		    SEQ_PRINTF(s, "%15s %22llu %22llu %22llu %22llu\n",
			       "Rx_Pkts", eth0_rx, eth1_rx, dsl_rx, other_rx);
		ret =
		    SEQ_PRINTF(s, "%15s %22llu %22llu %22llu %22llu\n",
			       "Tx_Pkts", eth0_tx, eth1_tx, dsl_tx, other_tx);
		ret =
		    SEQ_PRINTF(s,
			       "------------------------------------------------------------------------------------------------------------------------------------\n");
	} else if ((pos >= RMON_MAX) &&
		   (pos < (RMON_MAX + EGRESS_QUEUE_ID_MAX))) {
		uint32_t qdc[4], enq_c, deq_c, index;
		uint32_t wq, qrth, qocc, qavg;
		struct tmu_equeue_link equeue_link;
		char *flag_s;

		if (!rmon_display_tmu_mib)
			goto NEXT;

		index = pos - RMON_MAX;
		enq_c = get_enq_counter(index);
		deq_c = get_deq_counter(index);
		tmu_qdct_read(index, qdc);
		tmu_qoct_read(index, &wq, &qrth, &qocc, &qavg);
		tmu_equeue_link_get(index, &equeue_link);
		flag_s =
		    get_dma_flags_str(equeue_link.epn, flag_buf,
				      sizeof(flag_buf));

		if ((enq_c || deq_c || (qdc[0] + qdc[1] + qdc[2] + qdc[3]))
		    || qocc || qavg) {
			if (!f_q_mib_title_proc) {
				ret =
				    SEQ_PRINTF(s,
					       "%-15s %10s %10s %10s (%10s %10s %10s %10s) %10s %10s %10s\n",
					       "TMU MIB     QID", "enq",
					       "deq", "drop", "No-Color",
					       "Green", "Yellow", "Red",
					       "qocc", "qavg", "  DMA  Flag");
				f_q_mib_title_proc = 1;
			}

			ret =
			    SEQ_PRINTF(s,
				       "%15d %10u %10u %10u (%10u %10u %10u %10u) %10u %10u %10s\n",
				       index, enq_c, deq_c,
				       (qdc[0] + qdc[1] + qdc[2] + qdc[3]),
				       qdc[0], qdc[1], qdc[2], qdc[3], qocc,
				       (qavg >> 8), flag_s ? flag_s : "");

		} else
			goto NEXT;
	} else
		goto NEXT;

	if (ret)		/*buffer over flow and don't increase pos */
		return pos;

 NEXT:
	pos++;

	if (pos - RMON_MAX + 1 >= EGRESS_QUEUE_ID_MAX)
		return -1;

	return pos;
}

int proc_gsw_rmon_port_start(void)
{
	f_q_mib_title_proc = 0;
	return 0;
}

void dp_sys_mib_reset(uint32_t flag)
{
#ifdef CONFIG_LTQ_DATAPATH_MIB
	dp_reset_sys_mib(0);
#else
	gsw_mib_reset(0, 0); /* GSW L */
	gsw_mib_reset(1, 0); /* GSW R */
	dp_clear_all_mib_inside(0);
	tmu_reset_mib_all(flag);
#endif
}

ssize_t proc_gsw_rmon_write(struct file *file, const char *buf, size_t count,
			    loff_t *ppos)
{
	int len;
	char str[64];
	int num;
	char *param_list[10];

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));

	if (num < 1)
		goto help;

	if (dp_strcmpi(param_list[0], "clear") == 0 ||
	    dp_strcmpi(param_list[0], "c") == 0 ||
	    dp_strcmpi(param_list[0], "rest") == 0 ||
	    dp_strcmpi(param_list[0], "r") == 0) {
		dp_sys_mib_reset(0);
		goto EXIT_OK;
	}

	if (dp_strcmpi(param_list[0], "TMU") == 0) {
		if (dp_strcmpi(param_list[1], "on") == 0) {
			rmon_display_tmu_mib = 1;
			goto EXIT_OK;
		} else if (dp_strcmpi(param_list[1], "off") == 0) {
			rmon_display_tmu_mib = 0;
			goto EXIT_OK;
		}
	}

	if (dp_strcmpi(param_list[0], "RMON") == 0) {
		if (dp_strcmpi(param_list[1], "Full") == 0) {
			rmon_display_port_full = 1;
			goto EXIT_OK;
		} else if (dp_strcmpi(param_list[1], "Basic") == 0) {
			rmon_display_port_full = 0;
			goto EXIT_OK;
		}
	}

	/*unknow command */
	goto help;

 EXIT_OK:
	return count;

 help:
	PRINTK("usage: echo clear > /proc/dp/rmon\n");
	PRINTK("usage: echo TMU on > /proc/dp/rmon\n");
	PRINTK("usage: echo TMU off > /proc/dp/rmon\n");
	PRINTK("usage: echo RMON Full > /proc/dp/rmon\n");
	PRINTK("usage: echo RMON Basic > /proc/dp/rmon\n");
	return count;
}

/**
* \brief directly read memory address with 4 bytes alignment.
* \param  reg_addr memory address (it must be 4 bytes alignment)
* \param  shift to the expected bits (its value is from 0 ~ 31)
* \param  size the bits number (its value is from 1 ~ 32).
*  Note: shift + size <= 32
* \param  buffer destionation
* \return on Success return 0
*/
int32_t dp_mem_read(uint32_t reg_addr, uint32_t shift, uint32_t size,
		    uint32_t *buffer, uint32_t base)
{
	u32 v;
	uint32_t mask = 0;
	int i;

	/*generate mask */
	for (i = 0; i < size; i++)
		mask |= 1 << i;

	mask = mask << shift;

	/*read data from specified address */
	if (base == 4)
		v = *(u32 *) reg_addr;
	else if (base == 2)
		v = *(u16 *) reg_addr;
	else
		v = *(u8 *) reg_addr;

	v = dp_get_val(v, mask, shift);

	*buffer = v;
	return 0;
}

/**
* \brief directly write memory address with
* \param  reg_addr memory address (it must be 4 bytes alignment)
* \param  shift to the expected bits (its value is from 0 ~ 31)
* \param  size the bits number (its value is from 1 ~ 32)
* \param  value value writen to
* \return on Success return 0
*/
int32_t dp_mem_write(uint32_t reg_addr, uint32_t shift, uint32_t size,
		     uint32_t value, uint32_t base)
{
	u32 tmp = 0;
	uint32_t mask = 0;
	int i;

	/*generate mask */
	for (i = 0; i < size; i++)
		mask |= 1 << i;

	mask = mask << shift;

	/*read data from specified address */
	if (base == 4)
		tmp = *(u32 *) reg_addr;
	else if (base == 2)
		tmp = *(u16 *) reg_addr;
	else if (base == 1)
		tmp = *(u8 *) reg_addr;
	else {
		PR_ERR("wrong base in dp_mem_write\n");
		return 0;
	}

	dp_set_val(tmp, value, mask, shift);

	if (base == 4)
		*(u32 *) reg_addr = tmp;
	else if (base == 2)
		*(u16 *) reg_addr = tmp;
	else
		*(u8 *) reg_addr = tmp;

	return 0;
}

#define MODE_ACCESS_BYTE  1
#define MODE_ACCESS_SHORT 2
#define MODE_ACCESS_DWORD 4

#define ACT_READ   1
#define ACT_WRITE  2
static int proc_write_mem(struct file *file, const char *buf, size_t count,
			  loff_t *ppos)
{
	char str[100];
	int num;
	char *param_list[20] = { NULL };
	int i, k, len;
	u32 line_max_num = 32;	/* per line number printed */
	u32 addr = 0;
	u32 v = 0;
	u32 act = ACT_READ;
	u32 bit_offset = 0;
	u32 bit_num = 32;
	u32 repeat = 1;
	u32 mode = MODE_ACCESS_DWORD;
	int v_flag = 0;

	len = sizeof(str) < count ? sizeof(str) - 1 : count;
	len = len - copy_from_user(str, buf, len);
	str[len] = 0;
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));

	if (num < 2)
		goto proc_help;

	if (dp_strncmpi(param_list[0], "w", 1) == 0)
		act = ACT_WRITE;
	else if (dp_strncmpi(param_list[0], "r", 1) == 0)
		act = ACT_READ;
	else
		goto proc_help;

	if (num < 3)
		goto proc_help;

	k = 1;

	while (k < num) {
		if (dp_strcmpi(param_list[k], "-s") == 0) {
			addr = dp_atoi(param_list[k + 1]);
			k += 2;
		} else if (dp_strcmpi(param_list[k], "-o") == 0) {
			bit_offset = dp_atoi(param_list[k + 1]);
			k += 2;
		} else if (dp_strcmpi(param_list[k], "-n") == 0) {
			bit_num = dp_atoi(param_list[k + 1]);
			k += 2;
		} else if (dp_strcmpi(param_list[k], "-r") == 0) {
			repeat = dp_atoi(param_list[k + 1]);
			k += 2;
		} else if (dp_strcmpi(param_list[k], "-v") == 0) {
			v = dp_atoi(param_list[k + 1]);
			k += 2;
			v_flag = 1;
		} else if (dp_strcmpi(param_list[k], "-b") == 0) {
			mode = MODE_ACCESS_BYTE;
			k += 1;
		} else if (dp_strcmpi(param_list[k], "-w") == 0) {
			mode = MODE_ACCESS_SHORT;
			k += 1;
		} else if (dp_strcmpi(param_list[k], "-d") == 0) {
			mode = MODE_ACCESS_DWORD;
			k += 1;
		} else {
			PR_INFO("unknown command option: %s\n",
				param_list[k]);
			break;
		}
	}

	if (bit_num > mode * 8)
		bit_num = mode * 8;

	if (repeat == 0)
		repeat = 1;

	if (!addr) {
		PR_ERR("addr cannot be zero\n");
		goto EXIT;
	}

	if ((mode != MODE_ACCESS_DWORD) && (mode != MODE_ACCESS_SHORT) &&
	    (mode != MODE_ACCESS_BYTE)) {
		PR_ERR("wrong access mode: %d bytes\n", mode);
		goto EXIT;
	}

	if ((act == ACT_WRITE) && !v_flag) {
		PR_ERR("For write command it needs to provide -v\n");
		goto EXIT;
	}

	if (bit_offset > mode * 8 - 1) {
		PR_ERR("valid bit_offset range:0 ~ %d\n", mode * 8 - 1);
		goto EXIT;
	}

	if ((bit_num > mode * 8) || (bit_num < 1)) {
		PR_ERR("valid bit_num range:0 ~ %d. Current bit_num=%d\n",
		       mode * 8, bit_num);
		goto EXIT;
	}

	if ((bit_offset + bit_num) > mode * 8) {
		PR_ERR("valid bit_offset+bit_num range:0 ~ %d\n", mode * 8);
		goto EXIT;
	}

	if ((addr % mode) != 0) {	/*access alignment */
		PR_ERR("Cannot access 0x%08x in %d bytes\n", addr, mode);
		goto EXIT;
	}

	line_max_num /= mode;

	if (act == ACT_WRITE)
		PRINTK
		    ("act=%s addr=0x%08x mode=%s bit_offset=%d bit_num=%d v=0x%08x\n",
		     "write", addr,
		     (mode ==
		      MODE_ACCESS_DWORD) ? "dword" : ((mode ==
						       MODE_ACCESS_SHORT) ?
						      "short" : "DWORD"),
		     bit_offset, bit_num, v);
	else if (act == ACT_READ)
		PRINTK
		    ("act=%s addr=0x%08x mode=%s bit_offset=%d bit_num=%d\n",
		     "Read", addr,
		     (mode ==
		      MODE_ACCESS_DWORD) ? "dword" : ((mode ==
						       MODE_ACCESS_SHORT) ?
						      "short" : "DWORD"),
		     bit_offset, bit_num);

	if (act == ACT_WRITE)
		for (i = 0; i < repeat; i++)
			dp_mem_write(addr + mode * i, bit_offset, bit_num, v,
				     mode);
	else {
		for (i = 0; i < repeat; i++) {
			v = 0;
			dp_mem_read(addr + mode * i, bit_offset, bit_num, &v,
				    mode);

			/*print format control */
			if ((i % line_max_num) == 0)
				PRINTK("0x%08x: ", addr + mode * i);

			if (mode == MODE_ACCESS_DWORD)
				PRINTK("0x%08x ", v);
			else if (mode == MODE_ACCESS_SHORT)
				PRINTK("0x%04x ", v);
			else
				PRINTK("0x%02x ", v);

			if ((i % line_max_num) == (line_max_num - 1))
				PRINTK("\n");
		}
	}

	PRINTK("\n");
 EXIT:
	return count;
 proc_help:
	PR_INFO
	    ("echo <write/w> [-d/w/b] -s <start_v_addr> [-r <repeat_times>] -v <value> [-o <bit_offset>] [-n <bit_num>]\n");
	PR_INFO
	    ("echo <read/r>  [-d/w/b] -s <start_v_addr> [-r <repeat_times>] [-o <bit_offset>] [-n <bit_num>]\n");
	PR_INFO("\t -d: default read/write in dwords, ie 4 bytes\n");
	PR_INFO("\t -w: read/write in short, ie 2 bytes\n");
	PR_INFO("\t -b: read/write in bytes, ie 1 bytes\n");

	return count;
}

int proc_ep_dump(struct seq_file *s, int pos)
{
#if defined(NEW_CBM_API) && NEW_CBM_API
	uint32_t num;
	cbm_tmu_res_t *res = NULL;
	uint32_t flag = 0;
	int i;
	struct pmac_port_info *port = get_port_info(pos);

	if (cbm_dp_port_resources_get
	    (&pos, &num, &res, port ? port->alloc_flags : flag) == 0) {
		for (i = 0; i < num; i++) {
			SEQ_PRINTF(s, "ep=%d tmu_port=%d queue=%d sid=%d\n",
				   pos, res[i].tmu_port, res[i].tmu_q,
				   res[i].tmu_sched);
		}

		kfree(res);
	}
#endif
	pos++;

	if (pos >= PMAC_MAX_NUM)
		pos = -1;	/*end of the loop */

	return pos;
}

typedef int (*ingress_pmac_set_callback_t) (dp_pmac_cfg_t *pmac_cfg,
					    u32 value);
typedef int (*egress_pmac_set_callback_t) (dp_pmac_cfg_t *pmac_cfg,
					   u32 value);
struct ingress_pmac_entry {
	char *name;
	ingress_pmac_set_callback_t ingress_callback;
};
struct egress_pmac_entry {
	char *name;
	egress_pmac_set_callback_t egress_callback;
};
static int ingress_err_disc_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->ig_pmac.err_disc = value;
	pmac_cfg->ig_pmac_flags = IG_PMAC_F_ERR_DISC;
	return 0;
}

static int ingress_pmac_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->ig_pmac.pmac = value;
	pmac_cfg->ig_pmac_flags = IG_PMAC_F_PRESENT;
	return 0;
}

static int ingress_pmac_pmap_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->ig_pmac.def_pmac_pmap = value;
	pmac_cfg->ig_pmac_flags = IG_PMAC_F_PMAP;
	return 0;
}

static int ingress_pmac_en_pmap_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->ig_pmac.def_pmac_en_pmap = value;
	pmac_cfg->ig_pmac_flags = IG_PMAC_F_PMAPENA;
	return 0;
}

static int ingress_pmac_tc_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->ig_pmac.def_pmac_tc = value;
	pmac_cfg->ig_pmac_flags = IG_PMAC_F_CLASS;
	return 0;
}

static int ingress_pmac_en_tc_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->ig_pmac.def_pmac_en_tc = value;
	pmac_cfg->ig_pmac_flags = IG_PMAC_F_CLASSENA;
	return 0;
}

static int ingress_pmac_subifid_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->ig_pmac.def_pmac_subifid = value;
	pmac_cfg->ig_pmac_flags = IG_PMAC_F_SUBIF;
	return 0;
}

static int ingress_pmac_srcport_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->ig_pmac.def_pmac_src_port = value;
	pmac_cfg->ig_pmac_flags = IG_PMAC_F_SPID;
	return 0;
}

static int ingress_pmac_hdr1_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	uint8_t hdr;
	hdr = (uint8_t) value;
	pmac_cfg->ig_pmac.def_pmac_hdr[0] = hdr;
	pmac_cfg->ig_pmac_flags = IG_PMAC_F_PMACHDR1;
	return 0;
}

static int ingress_pmac_hdr2_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	uint8_t hdr;
	hdr = (uint8_t) value;
	pmac_cfg->ig_pmac.def_pmac_hdr[1] = hdr;
	pmac_cfg->ig_pmac_flags = IG_PMAC_F_PMACHDR2;
	return 0;
}

static int ingress_pmac_hdr3_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	uint8_t hdr;
	hdr = (uint8_t) value;
	pmac_cfg->ig_pmac.def_pmac_hdr[2] = hdr;
	pmac_cfg->ig_pmac_flags = IG_PMAC_F_PMACHDR3;
	return 0;
}

static int ingress_pmac_hdr4_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	uint8_t hdr;
	hdr = (uint8_t) value;
	pmac_cfg->ig_pmac.def_pmac_hdr[3] = hdr;
	pmac_cfg->ig_pmac_flags = IG_PMAC_F_PMACHDR4;
	return 0;
}

static int ingress_pmac_hdr5_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	uint8_t hdr;
	hdr = (uint8_t) value;
	pmac_cfg->ig_pmac.def_pmac_hdr[4] = hdr;
	pmac_cfg->ig_pmac_flags = IG_PMAC_F_PMACHDR5;
	return 0;
}

static int ingress_pmac_hdr6_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	uint8_t hdr;
	hdr = (uint8_t) value;
	pmac_cfg->ig_pmac.def_pmac_hdr[5] = hdr;
	pmac_cfg->ig_pmac_flags = IG_PMAC_F_PMACHDR6;
	return 0;
}

static int ingress_pmac_hdr7_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	uint8_t hdr;
	hdr = (uint8_t) value;
	pmac_cfg->ig_pmac.def_pmac_hdr[6] = hdr;
	pmac_cfg->ig_pmac_flags = IG_PMAC_F_PMACHDR7;
	return 0;
}

static int ingress_pmac_hdr8_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	uint8_t hdr;
	hdr = (uint8_t) value;
	pmac_cfg->ig_pmac.def_pmac_hdr[7] = hdr;
	pmac_cfg->ig_pmac_flags = IG_PMAC_F_PMACHDR8;
	return 0;
}

static int egress_fcs_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->eg_pmac.fcs = value;
	pmac_cfg->eg_pmac_flags = EG_PMAC_F_FCS;
	return 0;
}

static int egress_l2hdr_bytes_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->eg_pmac.num_l2hdr_bytes_rm = value;
	pmac_cfg->eg_pmac_flags = EG_PMAC_F_L2HDR_RM;
	return 0;
}

static int egress_rx_dma_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->eg_pmac.rx_dma_chan = value;
	pmac_cfg->eg_pmac_flags = EG_PMAC_F_RXID;
	return 0;
}

static int egress_pmac_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->eg_pmac.pmac = value;
	pmac_cfg->eg_pmac_flags = EG_PMAC_F_PMAC;
	return 0;
}

static int egress_res_dw_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->eg_pmac.res_dw1 = value;
	pmac_cfg->eg_pmac_flags = EG_PMAC_F_RESDW1;
	return 0;
}

static int egress_res1_dw_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->eg_pmac.res1_dw0 = value;
	pmac_cfg->eg_pmac_flags = EG_PMAC_F_RES1DW0;
	return 0;
}

static int egress_res2_dw_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->eg_pmac.res2_dw0 = value;
	pmac_cfg->eg_pmac_flags = EG_PMAC_F_RES2DW0;
	return 0;
}

static int egress_tc_ena_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->eg_pmac.tc_enable = value;
	pmac_cfg->eg_pmac_flags = EG_PMAC_F_TCENA;
	return 0;
}

static int egress_dec_flag_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->eg_pmac.dec_flag = value;
	pmac_cfg->eg_pmac_flags = EG_PMAC_F_DECFLG;
	return 0;
}

static int egress_enc_flag_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->eg_pmac.enc_flag = value;
	pmac_cfg->eg_pmac_flags = EG_PMAC_F_ENCFLG;
	return 0;
}

static int egress_mpe1_flag_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->eg_pmac.mpe1_flag = value;
	pmac_cfg->eg_pmac_flags = EG_PMAC_F_MPE1FLG;
	return 0;
}

static int egress_mpe2_flag_set(dp_pmac_cfg_t *pmac_cfg, u32 value)
{
	pmac_cfg->eg_pmac.mpe2_flag = value;
	pmac_cfg->eg_pmac_flags = EG_PMAC_F_MPE2FLG;
	return 0;
}

static struct ingress_pmac_entry ingress_entries[] = {
	{"errdisc", ingress_err_disc_set},
	{"pmac", ingress_pmac_set},
	{"pmac_pmap", ingress_pmac_pmap_set},
	{"pmac_en_pmap", ingress_pmac_en_pmap_set},
	{"pmac_tc", ingress_pmac_tc_set},
	{"pmac_en_tc", ingress_pmac_en_tc_set},
	{"pmac_subifid", ingress_pmac_subifid_set},
	{"pmac_srcport", ingress_pmac_srcport_set},
	{"pmac_hdr1", ingress_pmac_hdr1_set},
	{"pmac_hdr2", ingress_pmac_hdr2_set},
	{"pmac_hdr3", ingress_pmac_hdr3_set},
	{"pmac_hdr4", ingress_pmac_hdr4_set},
	{"pmac_hdr5", ingress_pmac_hdr5_set},
	{"pmac_hdr6", ingress_pmac_hdr6_set},
	{"pmac_hdr7", ingress_pmac_hdr7_set},
	{"pmac_hdr8", ingress_pmac_hdr8_set},
	{NULL, NULL}
};

static struct egress_pmac_entry egress_entries[] = {
	{"rx_dmachan", egress_rx_dma_set},
	{"rm_l2hdr", egress_l2hdr_bytes_set},
	{"fcs", egress_fcs_set},
	{"pmac", egress_pmac_set},
	{"res_dw1", egress_res_dw_set},
	{"res1_dw0", egress_res1_dw_set},
	{"res2_dw0", egress_res2_dw_set},
	{"tc_enable", egress_tc_ena_set},
	{"dec_flag", egress_dec_flag_set},
	{"enc_flag", egress_enc_flag_set},
	{"mpe1_flag", egress_mpe1_flag_set},
	{"mpe2_flag", egress_mpe2_flag_set},
	{NULL, NULL}
};

ssize_t ep_port_write(struct file *file, const char *buf, size_t count,
		      loff_t *ppos)
{
	int len;
	char str[64];
	int num, i, j, ret;
	u32 value;
	uint32_t port;
	char *param_list[10];
	dp_pmac_cfg_t pmac_cfg;
	memset(&pmac_cfg, 0, sizeof(dp_pmac_cfg_t));
	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));

	if (dp_strcmpi(param_list[0], "ingress") == 0) {
		port = dp_atoi(param_list[1]);

		for (i = 2; i < num; i += 2) {
			for (j = 0; j < ARRAY_SIZE(ingress_entries); j++) {
				if (dp_strcmpi
				    (param_list[i],
				     ingress_entries[j].name) == 0) {
					value = dp_atoi(param_list[i + 1]);
					ingress_entries[j].
					    ingress_callback(&pmac_cfg,
							     value);
					PR_INFO
					    ("ingress pmac ep %s configured\n",
					     ingress_entries[j].name);
					break;
				}
			}
		}

		ret = dp_pmac_set(port, &pmac_cfg);

		if (ret != 0) {
			PR_INFO("pmac set configuration failed\n");
			return -1;
		}
	} else if (dp_strcmpi(param_list[0], "egress") == 0) {
		port = dp_atoi(param_list[1]);

		for (i = 2; i < num; i += 2) {
			for (j = 0; j < ARRAY_SIZE(egress_entries); j++) {
				if (dp_strcmpi
				    (param_list[i],
				     egress_entries[j].name) == 0) {
					if (dp_strcmpi
					    (egress_entries[j].name,
					     "rm_l2hdr") == 0) {
						if (dp_atoi(param_list[i + 1])
						    > 0) {
							pmac_cfg.eg_pmac.
							    rm_l2hdr = 1;
							value =
							    dp_atoi(param_list
								    [i + 1]);
							egress_entries[j].
							    egress_callback
							    (&pmac_cfg,
							     value);
							PR_INFO
							    ("egress pmac ep %s configured successfully\n",
							     egress_entries
							     [j].name);
							break;
						}

						pmac_cfg.eg_pmac.rm_l2hdr =
						    dp_atoi(param_list
							    [i + 1]);
					} else {
						value =
						    dp_atoi(param_list
							    [i + 1]);
						egress_entries[j].
						    egress_callback(&pmac_cfg,
								    value);
						PR_INFO
						    ("egress pmac ep %s configured successfully\n",
						     egress_entries[j].name);
						break;
					}
				}
			}
		}

		ret = dp_pmac_set(port, &pmac_cfg);

		if (ret != 0) {
			PR_INFO("pmac set configuration failed\n");
			return -1;
		}
	} else {
		PR_ERR("wrong command\n");
		goto help;
	}

	return count;
 help:
	PR_INFO
	    ("echo ingress/egress [ep_port] ['ingress/egress fields'] [value] > /proc/dp/ep\n");
	PR_INFO("(eg) echo ingress 1 pmac 1 > /proc/dp/ep\n");
	PR_INFO("(eg) echo egress 1 rm_l2hdr 2 > /proc/dp/ep\n");
	PR_INFO
	    ("echo ingress [ep_port] ['errdisc/pmac/pmac_pmap/pmac_en_pmap/pmac_tc");
	PR_INFO
	    ("                         /pmac_en_tc/pmac_subifid/pmac_srcport'] [value] > /proc/dp/ep\n");
	PR_INFO
	    ("echo egress [ep_port] ['rx_dmachan/fcs/pmac/res_dw1/res1_dw0/res2_dw0] [value] > /proc/dp/ep\n");
	PR_INFO("echo egress [ep_port] ['rm_l2hdr'] [value] > /proc/dp/ep\n");
	return count;
}

void dp_send_packet(u8 *pdata, int len, char *devname, u32 flag)
{
	struct sk_buff *skb;
	dp_subif_t subif = { 0 };

	skb = cbm_alloc_skb(len + 8, GFP_ATOMIC);

	if (unlikely(!skb)) {
		PR_ERR("allocate cbm buffer fail\n");
		return;
	}

	skb->DW0 = 0;
	skb->DW1 = 0;
	skb->DW2 = 0;
	skb->DW3 = 0;
	memcpy(skb->data, pdata, len);
	skb->len = 0;
	skb_put(skb, len);
	skb->dev = dev_get_by_name(&init_net, devname);

	if (dp_get_netif_subifid(skb->dev, skb, NULL, skb->data, &subif, 0)) {
		PR_ERR("dp_get_netif_subifid failed for %s\n",
		       skb->dev->name);
		dev_kfree_skb_any(skb);
		return;
	}

	((struct dma_tx_desc_1 *)&(skb->DW1))->field.ep = subif.port_id;
	((struct dma_tx_desc_0 *)&(skb->DW0))->field.dest_sub_if_id =
	    subif.subif;

	dp_xmit(skb->dev, &subif, skb, skb->len, flag);
}

static u8 ipv4_plain_udp[] = {
	0x00, 0x00, 0x01, 0x00, 0x00, 0x01,	/*mac */
	0x00, 0x10, 0x94, 0x00, 0x00, 0x02,
	0x08, 0x00,		/*type */
	0x45, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x11,	/*ip header */
	0x3A, 0x56, 0xC0, 0x55, 0x01, 0x02, 0xC0, 0x00, 0x00, 0x01,
	0x04, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x7A, 0x41, 0x00, 0x00,	/*udp header */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00
};

static u8 ipv4_plain_tcp[1514] = {
	0x00, 0x01, 0x01, 0x01, 0x01, 0x01,	/*mac */
	0x00, 0x10, 0x94, 0x00, 0x00, 0x02,
	0x08, 0x00,		/*type */
	0x45, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x06,	/*ip header */
	0x3A, 0x61, 0xC0, 0x55, 0x01, 0x02, 0xC0, 0x00, 0x00, 0x01,
	0x04, 0x00, 0x04, 0x00, 0x00, 0x01, 0xE2, 0x40, 0x00, 0x03,	/*tcp header */
	0x94, 0x47, 0x50, 0x10, 0x10, 0x00, 0x9F, 0xD9, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	/*data */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00
};

static u8 ipv6_plain_udp[] = {
	0x00, 0x00, 0x01, 0x00, 0x00, 0x01,	/*mac */
	0x00, 0x10, 0x94, 0x00, 0x00, 0x02,
	0x86, 0xDD,		/*type */
	0x60, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x11, 0xFF, 0x20, 0x00,	/*ip header */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x04, 0x00, 0x00, 0x00, 0x00, 0x3E, 0xBB, 0x6F, 0x00, 0x00,	/*udp header */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00
};

static u8 ipv6_plain_tcp[] = {
	0x00, 0x00, 0x01, 0x00, 0x00, 0x01,	/*mac */
	0x00, 0x10, 0x94, 0x00, 0x00, 0x02,
	0x86, 0xDD,		/*type */
	0x60, 0x00, 0x00, 0x00, 0x00, 0x46, 0x06, 0xFF, 0x20, 0x00,	/*ip header */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x04, 0x00, 0x04, 0x00, 0x00, 0x01, 0xE2, 0x40, 0x00, 0x03,	/*tcp header */
	0x94, 0x47, 0x50, 0x10, 0x10, 0x00, 0xE1, 0x13, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	/*data */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static u8 ipv6_extensions_udp[] = {
	0x00, 0x00, 0x01, 0x00, 0x00, 0x01,	/*mac */
	0x00, 0x10, 0x94, 0x00, 0x00, 0x02,
	0x86, 0xDD,		/*type */
	0x60, 0x00, 0x00, 0x00, 0x00, 0x8E, 0x00, 0xFF, 0x20, 0x00,	/*ip header */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x3C, 0x00, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00,	/*next extension:hop */
	0x2B, 0x00, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00,	/*next extension:Destination */
	0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	/*next extension:Routing */
	0x04, 0x00, 0x00, 0x00, 0x00, 0x76, 0xBA, 0xFF, 0x00, 0x00,	/*udp header */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static u8 ipv6_extensions_tcp[] = {
	0x00, 0x00, 0x01, 0x00, 0x00, 0x01,	/*mac */
	0x00, 0x10, 0x94, 0x00, 0x00, 0x02,
	0x86, 0xDD,		/*type */
	0x60, 0x00, 0x00, 0x00, 0x00, 0x8E, 0x00, 0xFF, 0x20, 0x00,	/*ip header */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x3C, 0x00, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00,	/*next extension:hop */
	0x2B, 0x00, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00,	/*next extension:Destination */
	0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	/*next extension:Routing */
	0x04, 0x00, 0x04, 0x00, 0x00, 0x01, 0xE2, 0x40, 0x00, 0x03,	/*tcp header */
	0x94, 0x47, 0x50, 0x10, 0x10, 0x00, 0xE0, 0xE3, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static u8 rd6_ip4_ip6_udp[] = {
	0x00, 0x00, 0x01, 0x00, 0x00, 0x01,	/*mac */
	0x00, 0x10, 0x94, 0x00, 0x00, 0x02,
	0x08, 0x00,		/*type */
	0x45, 0x00, 0x00, 0x6E, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x29,	/*ip4 header */
	0x3A, 0x0E, 0xC0, 0x55, 0x01, 0x02, 0xC0, 0x00, 0x00, 0x01,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x32, 0x11, 0xFF, 0x20, 0x00,	/*ip6 header */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x04, 0x00, 0x00, 0x00, 0x00, 0x32, 0xBB, 0x87, 0x00, 0x00,	/*udp header */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static u8 rd6_ip4_ip6_tcp[] = {
	0x00, 0x00, 0x01, 0x00, 0x00, 0x01,	/*mac */
	0x00, 0x10, 0x94, 0x00, 0x00, 0x02,
	0x08, 0x00,		/*type */
	0x45, 0x00, 0x00, 0x6E, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x29,	/*ip4 header */
	0x3A, 0x0E, 0xC0, 0x55, 0x01, 0x02, 0xC0, 0x00, 0x00, 0x01,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x32, 0x06, 0xFF, 0x20, 0x00,	/*ip6 header */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x04, 0x00, 0x04, 0x00, 0x00, 0x01, 0xE2, 0x40, 0x00, 0x03,	/*tcp header */
	0x94, 0x47, 0x50, 0x10, 0x10, 0x00, 0xE1, 0x27, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static u8 dslite_ip6_ip4_udp[] = {
	0x00, 0x00, 0x01, 0x00, 0x00, 0x01,	/*mac */
	0x00, 0x10, 0x94, 0x00, 0x00, 0x02,
	0x86, 0xDD,		/*type */
	0x60, 0x00, 0x00, 0x00, 0x00, 0x46, 0x04, 0xFF, 0x20, 0x00,	/*ip6 header */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x45, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x11,	/*ip4 header */
	0x3A, 0x4E, 0xC0, 0x55, 0x01, 0x02, 0xC0, 0x00, 0x00, 0x01,
	0x04, 0x00, 0x00, 0x00, 0x00, 0x32, 0x7A, 0x31, 0x00, 0x00,	/*udp header */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static u8 dslite_ip6_ip4_tcp[] = {
	0x00, 0x00, 0x01, 0x00, 0x00, 0x01,	/*mac */
	0x00, 0x10, 0x94, 0x00, 0x00, 0x02,
	0x86, 0xDD,		/*type */
	0x60, 0x00, 0x00, 0x00, 0x00, 0x46, 0x04, 0xFF, 0x20, 0x00,	/*ip6 header */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x45, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x06,	/*ip4 header */
	0x3A, 0x59, 0xC0, 0x55, 0x01, 0x02, 0xC0, 0x00, 0x00, 0x01,
	0x04, 0x00, 0x04, 0x00, 0x00, 0x01, 0xE2, 0x40, 0x00, 0x03,	/*tcp header */
	0x94, 0x47, 0x50, 0x10, 0x10, 0x00, 0x9F, 0xD1, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static int checksm_mode = 2;
void proc_checksum_read(struct seq_file *s)
{
	char *devname = "eth0_4";

	if (!checksm_mode) {
		SEQ_PRINTF(s,
			   "\nsend pmac checksum ipv4_plain_udp new via %s\n",
			   devname);
		dp_send_packet(ipv4_plain_udp, sizeof(ipv4_plain_udp),
			       devname, DP_TX_CAL_CHKSUM);

		SEQ_PRINTF(s,
			   "\nsend pmac checksum ipv4_plain_tcp new via %s\n",
			   devname);
		dp_send_packet(ipv4_plain_tcp, sizeof(ipv4_plain_tcp),
			       devname, DP_TX_CAL_CHKSUM);

		SEQ_PRINTF(s,
			   "\nsend pmac checksum ipv6_plain_udp new via %s\n",
			   devname);
		dp_send_packet(ipv6_plain_udp, sizeof(ipv6_plain_udp),
			       devname, DP_TX_CAL_CHKSUM);

		SEQ_PRINTF(s,
			   "\nsend pmac checksum ipv6_plain_tcp new via %s\n",
			   devname);
		dp_send_packet(ipv6_plain_tcp, sizeof(ipv6_plain_tcp),
			       devname, DP_TX_CAL_CHKSUM);

		SEQ_PRINTF(s,
			   "\nsend pmac checksum ipv6_extensions_udp new via %s\n",
			   devname);
		dp_send_packet(ipv6_extensions_udp,
			       sizeof(ipv6_extensions_udp), devname,
			       DP_TX_CAL_CHKSUM);

		SEQ_PRINTF(s,
			   "\nsend pmac checksum ipv6_extensions_tcp via %s\n",
			   devname);
		dp_send_packet(ipv6_extensions_tcp,
			       sizeof(ipv6_extensions_tcp), devname,
			       DP_TX_CAL_CHKSUM);

		SEQ_PRINTF(s, "\nsend pmac checksum rd6_ip4_ip6_udp via %s\n",
			   devname);
		dp_send_packet(rd6_ip4_ip6_udp, sizeof(rd6_ip4_ip6_udp),
			       devname, DP_TX_CAL_CHKSUM);

		SEQ_PRINTF(s, "\nsend pmac checksum rd6_ip4_ip6_tcp via %s\n",
			   devname);
		dp_send_packet(rd6_ip4_ip6_tcp, sizeof(rd6_ip4_ip6_tcp),
			       devname, DP_TX_CAL_CHKSUM);

		SEQ_PRINTF(s,
			   "\nsend pmac checksum dslite_ip6_ip4_udp via %s\n",
			   devname);
		dp_send_packet(dslite_ip6_ip4_udp, sizeof(dslite_ip6_ip4_udp),
			       devname, DP_TX_CAL_CHKSUM);

		SEQ_PRINTF(s,
			   "\nsend pmac checksum dslite_ip6_ip4_tcp via %s\n",
			   devname);
		dp_send_packet(dslite_ip6_ip4_tcp, sizeof(dslite_ip6_ip4_tcp),
			       devname, DP_TX_CAL_CHKSUM);
	} else if (checksm_mode == 1) {
#define MOD_V  32
		int offset = 14 /*mac */ + 20 /*ip */ + 20 /*tcp */;
#define IP_LEN_OFFSET 16
		int i;
		int numbytes = jiffies % 1515;

		if (numbytes < 64)
			numbytes = 64;
		else if (numbytes >= 1514)
			numbytes = 1514;

		for (i = 0; i < sizeof(ipv4_plain_tcp) - offset; i++) {
			if (i < (numbytes - offset))
				ipv4_plain_tcp[offset + i] = (i % MOD_V) + 1;
			else
				ipv4_plain_tcp[offset + i] = 0;
		}
		*(unsigned short *)&ipv4_plain_tcp[IP_LEN_OFFSET] =
		    numbytes - 14 /*MAC HDR */;

		dp_send_packet(ipv4_plain_tcp, numbytes, devname,
			       DP_TX_CAL_CHKSUM);
	} else if (checksm_mode == 2) {
#define MOD_V  32
		int offset = 14 /*mac */  + 20 /*ip */  + 20 /*tcp */;
		int i;
		int numbytes = offset + 2 /*2 bytes payload */;

		for (i = 0; i < sizeof(ipv4_plain_tcp) - offset; i++) {
			if (i < (numbytes - offset))
				ipv4_plain_tcp[offset + i] = (i % MOD_V) + 1;
			else
				ipv4_plain_tcp[offset + i] = 0;
		}
		*(unsigned short *)&ipv4_plain_tcp[IP_LEN_OFFSET] =
		    numbytes - 14 /*MAC HDR */;
		dp_send_packet(ipv4_plain_tcp, numbytes, devname,
			       DP_TX_CAL_CHKSUM);
	}
}

ssize_t proc_checksum_write(struct file *file, const char *buf, size_t count,
			    loff_t *ppos)
{
	int len;
	char str[64];
	int num;
	char *param_list[2];

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));
	if ((dp_strcmpi(param_list[0], "help") == 0) ||
	    (dp_strcmpi(param_list[0], "h") == 0)) {
		goto help;
	} else
		checksm_mode = dp_atoi(param_list[0]);

	PRINTK("new checksm_mode=%d\n", checksm_mode);
	return count;

 help:
	PRINTK("checksm_mode usage: current value=%d\n", checksm_mode);
	PRINTK(" 0: common protocol test with HW checksum\n");
	PRINTK(" 1: TCP random size with HW checksum\n");
	PRINTK(" 2: 2 Bytes TCP packet with HW checksum\n");
	PRINTK(" others: not supported value\n");

	return count;
}

int proc_dport_dump(struct seq_file *s, int pos)
{
	int i;
	cbm_dq_port_res_t res;
	uint32_t flag = 0;
	memset(&res, 0, sizeof(cbm_dq_port_res_t));

	if (cbm_dequeue_port_resources_get(pos, &res, flag) == 0) {
		SEQ_PRINTF(s, "Dequeue port=%d free_base=0x%x\n", pos,
			   (u32) res.cbm_buf_free_base);

		for (i = 0; i < res.num_deq_ports; i++) {
			SEQ_PRINTF(s,
				   "%d:deq_port_base=0x%x num_desc=%d port = %d tx chan %d\n",
				   i, (u32) res.deq_info[i].cbm_dq_port_base,
				   res.deq_info[i].num_desc,
				   res.deq_info[i].port_no,
				   res.deq_info[i].dma_tx_chan);
		}

		if (res.deq_info)
			kfree(res.deq_info);
	}

	pos++;

	if (pos >= PMAC_MAX_NUM)
		pos = -1;	/*end of the loop */

	return pos;
}

int proc_common_cmd_start(void)
{
	return 0;
}

int proc_common_cmd_dump(struct seq_file *s, int pos)
{
	struct cmd_list {
		char *description;
		char *cmd;
	};
	int res = 0;
	static struct cmd_list cmd_list[] = {
		{"Pecostat",
		 "CPU utilization: pecostat -c pic0=0,pic1=1:EXL,K,S,U,IE 1"},
		{"Check gsw consumed buffer seg",
		 "dev=1; switch_cli  dev=$dev GSW_REGISTER_GET nRegAddr=0x47"},
		{"Check gsw default Multicast map",
		 "dev=1; switch_cli  dev=$dev GSW_REGISTER_GET nRegAddr=0x454"},
		{"Check gsw default unknown map",
		 "dev=1; switch_cli  dev=$dev GSW_REGISTER_GET nRegAddr=0x455"},
		{"Check gsw backpressue", "\n     dev=0 \
			\n     tx_channel=0   #DMA TX Channel, Range: 0~15 \
			\n     switch_cli dev=$dev GSW_REGISTER_SET nRegAddr=0xD45 nData=$tx_channel \
			\n     switch_cli dev=$dev GSW_REGISTER_SET nRegAddr=0xD46 nData=0x8000 \
			\n     switch_cli dev=$dev GSW_REGISTER_GET nRegAddr=0xD42 #RX Port Congestion Mask (bit 15:0)\
			\n     switch_cli dev=$dev GSW_REGISTER_GET nRegAddr=0xD43 #TX Queue Congestion Mask (bit 15:0)\
			\n     switch_cli dev=$dev GSW_REGISTER_GET nRegAddr=0xD44 #TX Queue Congestion Mask (bit 31:16)"},
		{"Disable gsw backpressue", "\n     dev=0 \
			\n     tx_channel=0   #DMA TX Channel, Range: 0~15 \
			\n     switch_cli dev=$dev GSW_REGISTER_SET nRegAddr=0xD42 nData=0x0     \
			\n     switch_cli dev=$dev GSW_REGISTER_SET nRegAddr=0xD43 nData=0x0     \
			\n     switch_cli dev=$dev GSW_REGISTER_SET nRegAddr=0xD44 nData=0x0     \
			\n     switch_cli dev=$dev GSW_REGISTER_SET nRegAddr=0xD45 nData=$tx_channel \
			\n     switch_cli dev=$dev GSW_REGISTER_SET nRegAddr=0xD46 nData=0x8020"},
		{"Check egress pmac", "\n    dev=1 \
			\n    nTrafficClass=1 #Range:0~15 \
			\n    nFlowIDMsb=1 #Range:0~3 \
			\n    nDestPortId=15 #Range:0~15 \
			\n    switch_cli dev=$dev GSW_PMAC_EG_CFG_GET nTrafficClass=$nTrafficClass nFlowIDMsb=$nFlowIDMsb nDestPortId=$nDestPortId"},
		{"Check ingress pmac", "\n    dev=1; \
			\n    nTxDmaChanId=1 #Range:0~15 \
			\n    switch_cli dev=$dev GSW_PMAC_IG_CFG_GET nTxDmaChanId=$nTxDmaChanId"},
		{"Check gsw link status",
		 "dev=0; nPortId=6; switch_cli dev=$dev GSW_PORT_LINK_CFG_GET nPortId=$nPortId"},
		{"Check gsw queue map",
		 "dev=1; nPortId=6; switch_cli  dev=$dev GSW_QOS_QUEUE_PORT_GET nPortId=$nPortId"},
		{"Add static MAC addres",
		 "\n    dev=1; nPortId=7; switch_cli dev=$dev GSW_MAC_TABLE_ENTRY_ADD nPortId=$nPortId nSubIfId=0x100 bStaticEntry=1 nMAC=00:10:94:00:00:01"},
		{"Disable PMAC Checksum",
		 "dev=0; switch_cli  dev=$dev GSW_REGISTER_SET nRegAddr=0xD03 nData=0x0C0"},
		{"Debugger example",
		 "\n    d.load.elf \"X:\\tmp2\\shaoguoh\\project\\ugw61_grx500\\openwrt\\core\\kernel_tree\\vmlinux\" /gnu /strippart \"/tmp2\" /path \"X:\\tmp2\" /nocode"},
		{"While example 1",
		 "\n     i=0 \n     while [ $i -lt 16 ] \n     do \n       xxxx \n     i=$((i+1)) \n     done"},
		 {"While example 2",
		 "\n     while true\n     do\n       xxxx\n     done\n"},
		{"For example",
		 "\n     for i in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 \n     do  \n       xxxxxx \n    done"},

		/*last place holder */
		{NULL, NULL}
	};

	if (!cmd_list[pos].description)
		return -1;

	if (!cmd_list[pos].cmd) {
		SEQ_PRINTF(s,
			   "key in wrongly cmd for pos=%d with command: %s\n",
			   pos, cmd_list[pos].cmd);
	} else if (dp_strncmpi(cmd_list[pos].cmd, "\n    ", 5) == 0) {
		res = SEQ_PRINTF(s, "---%s: ", cmd_list[pos].description);
		res = SEQ_PRINTF(s, "%s\n\n", cmd_list[pos].cmd);
	} else
		res =
		    SEQ_PRINTF(s, "---%s: %s\n\n", cmd_list[pos].description,
			       cmd_list[pos].cmd);
	if (res)
		return pos;	/*repeat since proc buffer not enough*/

	pos++;

	if (pos >= ARRAY_SIZE(cmd_list))
		pos = -1;	/*end of the loop */

	return pos;
}

struct dp_skb_info {
	struct list_head list;
	struct sk_buff *skb;
};

static int cbm_skb_num;	/* for cbm buffer testing purpose */
static struct dp_skb_info skb_list;
#define get_val(val, mask, offset) (((val) & (mask)) >> (offset))
int cbm_get_free_buf(int fsqm_index, uint32_t *free_num, uint32_t *head,
		     uint32_t *tail)
{
	unsigned char *base;

	if (!fsqm_index)
		base = (unsigned char *)(FSQM0_MODULE_BASE + 0xa0000000);
	else
		base = (unsigned char *)(FSQM1_MODULE_BASE + 0xa0000000);
	if (free_num)
		*free_num =
		    get_val(*(uint32_t *) (base + OFSC), OFSC_FSC_MASK,
			    OFSC_FSC_POS);
	if (head)
		*head =
		    get_val(*(uint32_t *) (base + OFSQ), OFSQ_HEAD_MASK,
			    OFSQ_HEAD_POS);
	if (tail)
		*tail =
		    get_val(*(uint32_t *) (base + OFSQ), OFSQ_TAIL_MASK,
			    OFSQ_TAIL_POS);

	return 0;
}

void proc_cbm_buf_read(struct seq_file *s)
{
	uint32_t free_fsqm_num[2], fsqm_head[2], fsqm_tail[2];
	int i;

	for (i = 0; i < 2; i++)
		cbm_get_free_buf(i, &free_fsqm_num[i], &fsqm_head[i],
				 &fsqm_tail[i]);
	for (i = 0; i < 2; i++)
		SEQ_PRINTF(s,
			   "FSQM%d: free buffer-%04d, head-%04d, tail-%04d\n",
			   i, free_fsqm_num[i], fsqm_head[i], fsqm_tail[i]);
	if (cbm_skb_num)
		SEQ_PRINTF(s,
			   "Overall %d CBM buffer is allocated for testing purpose !\n",
			   cbm_skb_num);
	SEQ_PRINTF(s,
		   "Note: use echo to display other commans: echo help > /proc/dp/%s\n",
		   DP_PROC_FILE_CBM_BUF_TEST);
}

ssize_t proc_cbm_buf_write(struct file *file, const char *buf, size_t count,
			   loff_t *ppos)
{
	int len;
	char str[64];
	char *param_list[2] = { 0 };
	unsigned int num;
	struct dp_skb_info *tmp = NULL;
	uint32_t free_fsqm_num[2], fsqm_head[2], fsqm_tail[2], *check_list;
	int i;
	uint32_t idx, buf_ptr, val, head, tail, bits;
	const uint32_t fsqm_buf_len[] = {9216, 1024};
	void __iomem *base;

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;

	if (dp_split_buffer(str, param_list, ARRAY_SIZE(param_list)) < 2)
		goto help;
	if (!cbm_skb_num)
		INIT_LIST_HEAD(&skb_list.list);
	if (list_empty(&skb_list.list) && (cbm_skb_num != 0)) {
		PR_ERR
		    ("Why skb_list is empty but recorded value of cbm_skb_num=%d\n",
		     cbm_skb_num);
		goto exit;
	}
	if (cbm_skb_num < 0) {
		PR_ERR("Why cbm_skb_num(%d) less than zero\n", cbm_skb_num);
		goto exit;
	}
	num = dp_atoi(param_list[1]);
	if (dp_strcmpi(param_list[0], "alloc") == 0) {
		if (num == 0)
			goto exit;
			do {
				tmp =
				    (struct dp_skb_info *)
				    kmalloc(sizeof(struct dp_skb_info), GFP_ATOMIC);
				if (!tmp) {
					PR_ERR("kmalloc failed\n");
					goto exit;
				}
				INIT_LIST_HEAD(&tmp->list);
				tmp->skb = cbm_alloc_skb(1000, 0);
				if (!tmp->skb) {
					goto exit;
					kfree(tmp);
				}
				DP_DEBUG(DP_DBG_FLAG_CBM_BUF,
					 "cbm_alloc_skb ok: %p (node=%p buffer=%p)\n",
					 tmp->skb, &tmp->list, tmp);
				list_add(&(tmp->list), &(skb_list.list));
				num--;
				cbm_skb_num++;
			} while (num);
	} else if (dp_strcmpi(param_list[0], "free") == 0) {
		struct list_head *pos, *n;
		struct dp_skb_info *p;
		if (cbm_skb_num == 0 || num == 0)
			goto exit;
		list_for_each_safe(pos, n, &skb_list.list) {
			if (!pos)
				break;
			p = list_entry(pos, struct dp_skb_info, list);
			if (!pos) {
				PR_ERR("why P NULL ???\n");
				goto exit;
			}
			if (p->skb) {
				if (!check_ptr_validation
				    ((uint32_t) p->skb->data))
					PR_ERR
					    ("Wrong Free skb %p(node=%p buffer=%p) not CBM bffer\n",
					     p->skb, pos, p);
				else
					DP_DEBUG(DP_DBG_FLAG_CBM_BUF,
						 "Free skb %p(node=%p buffer=%p)\n",
						 p->skb, pos, p);
				dev_kfree_skb(p->skb);

				p->skb = NULL;
			} else
				PR_ERR("why p->skb NULL ???\n");
			list_del(pos);
			kfree(p);
			num--;
			cbm_skb_num--;
			if (!num)
				break;
		}
	} else if (dp_strcmpi(param_list[0], "check") == 0) {
		int good = 1;
		idx = dp_atoi(param_list[1]);
		if (idx >= 2) {
			PR_ERR("FSQM idx must be 0 or 1\n");
			return count;
		}
		num = idx;
		PR_INFO("\nCBM link list check can only be performed with NO traffic on CBM!\n");
		check_list = kmalloc(fsqm_buf_len[idx] >> 3, GFP_KERNEL);
		if (!check_list) {
			PR_ERR("Failed to allocate check list buffer\n");
			return count;
		}
		memset(check_list, 0, fsqm_buf_len[idx] >> 3);
		if (idx == 0) /* Fixme: Hardcoded mapping address */
			base = (void __iomem *)FSQM0_MODULE_BASE + 0xa0000000;
		else
			base = (void __iomem *)FSQM1_MODULE_BASE + 0xa0000000;
		val = readl(base + OFSQ);
		head = val & 0x7FFF;
		tail = (val >> 16) & 0x7FFF;
		PR_INFO("FSQM Head: 0x%x, Tail: 0x%x\n", head, tail);
		for (i = 0, buf_ptr = head;
			i < fsqm_buf_len[num] && buf_ptr != tail;
			i++) {
			idx = buf_ptr / 32;
			bits = buf_ptr % 32;
			if (!(check_list[idx] & BIT(bits))) {
				check_list[idx] |= BIT(bits);
			} else {
				PRINTK("FSQM[%d] ERROR: PTR:[0x%4x] is duplicated\n", num, buf_ptr);
				good = 0;
			}
			buf_ptr = readl(base + RAM + (buf_ptr << 2));
		}
		PRINTK("Total freed buffers in link list: 0x%x, free buffer cnt in CBM OFSC REG: 0x%x---%s\n",
			i + 1, readl(base + OFSC),
			good ? "In Good State" : "In Wrong State");
		kfree(check_list);
		return count;
	} else
		goto help;

 exit:
	for (i = 0; i < 2; i++)
		cbm_get_free_buf(i, &free_fsqm_num[i], &fsqm_head[i],
				 &fsqm_tail[i]);
	if (cbm_skb_num)
		PR_ERR
		    ("Overall %d CBM buffer is allocated for testing purpose\n",
		     cbm_skb_num);
	else
		PR_ERR("All buffer already returned to CBM\n");
	for (i = 0; i < 2; i++)
		PR_ERR("FSQM%d: free buffer-%04d, head-%04d, tail-%04d\n", i,
		       free_fsqm_num[i], fsqm_head[i], fsqm_tail[i]);
	return count;

 help:
	PRINTK("usage: echo alloc [cbm buffer num] > /proc/dp/%s\n",
	       DP_PROC_FILE_CBM_BUF_TEST);
	PRINTK("usage: echo free  [cbm buffer num] > /proc/dp/%s\n",
	       DP_PROC_FILE_CBM_BUF_TEST);
	PRINTK("Check CBM buffer link list: echo check <fqsm_idx> > /proc/dp/%s\n",
	       DP_PROC_FILE_CBM_BUF_TEST);
	return count;
}

static int proc_gsw_pce_dump(struct seq_file *s, int pos)
{
	GSW_API_HANDLE gsw_handle = 0;
	GSW_PCE_rule_t *rule;
	int ret = 0, i;

	rule =
	    (GSW_PCE_rule_t *) kmalloc(sizeof(GSW_PCE_rule_t) + 1,
				       GFP_ATOMIC);
	if (!rule) {
		PR_ERR("Failed to allocate %d bytes buffer\n",
		       sizeof(GSW_PCE_rule_t) + 1);
		pos = -1;
		return pos;
	}

	/*read gswip-r rmon counter */
	gsw_handle = gsw_api_kopen((char *)GSWIP_R_DEV_NAME);
	if (gsw_handle == 0) {
		PR_ERR("Open GSWIP-R device FAILED !\n");
		pos = -1;
		kfree(rule);
		return pos;
	}
	rule->pattern.nIndex = pos;
	ret =
	    gsw_api_kioctl(gsw_handle, GSW_PCE_RULE_READ, (unsigned int)rule);
	if (ret != GSW_statusOk) {
		/*dbg("read_pce_entry returned Failure for index=%d\n", rule.index);*/
		pos = -1;
		return pos;
	}
	if (!rule->pattern.bEnable)
		goto EXIT;

	SEQ_PRINTF(s, "Pattern[%d]:-----\n", rule->pattern.nIndex);
	if (rule->pattern.bPortIdEnable) {
		SEQ_PRINTF(s, "  bPortIdEnable           =   %d\n",
			   rule->pattern.bPortIdEnable);
		SEQ_PRINTF(s, "  nPortId                 =   %d\n",
			   rule->pattern.nPortId);
		SEQ_PRINTF(s, "  bPortId_Exclude         =   %d\n",
			   rule->pattern.bPortId_Exclude);
	}
	if (rule->pattern.bSubIfIdEnable) {
		SEQ_PRINTF(s, "  bSubIfIdEnable          =   %d\n",
			   rule->pattern.bSubIfIdEnable);
		SEQ_PRINTF(s, "  nSubIfId                =   %d\n",
			   rule->pattern.nSubIfId);
		SEQ_PRINTF(s, "  bSubIfId_Exclude        =   %d\n",
			   rule->pattern.bSubIfId_Exclude);
	}
	if (rule->pattern.bDSCP_Enable) {
		SEQ_PRINTF(s, "  bDSCP_Enable            =   %d\n",
			   rule->pattern.bDSCP_Enable);
		SEQ_PRINTF(s, "  nDSCP                   =   %d\n",
			   rule->pattern.nDSCP);
		SEQ_PRINTF(s, "  bDSCP_Exclude           =   %d\n",
			   rule->pattern.bDSCP_Exclude);
	}
	if (rule->pattern.bInner_DSCP_Enable) {
		SEQ_PRINTF(s, "  bInner_DSCP_Enable      =   %d\n",
			   rule->pattern.bInner_DSCP_Enable);
		SEQ_PRINTF(s, "  nInnerDSCP              =   %d\n",
			   rule->pattern.nInnerDSCP);
		SEQ_PRINTF(s, "  bInnerDSCP_Exclude      =   %d\n",
			   rule->pattern.bInnerDSCP_Exclude);
	}
	if (rule->pattern.bPCP_Enable) {
		SEQ_PRINTF(s, "  bPCP_Enable             =   %d\n",
			   rule->pattern.bPCP_Enable);
		SEQ_PRINTF(s, "  nPCP                    =   %d\n",
			   rule->pattern.nPCP);
		SEQ_PRINTF(s, "  bCTAG_PCP_DEI_Exclude   =   %d\n",
			   rule->pattern.bCTAG_PCP_DEI_Exclude);
	}
	if (rule->pattern.bSTAG_PCP_DEI_Enable) {
		SEQ_PRINTF(s, "  bSTAG_PCP_DEI_Enable    =   %d\n",
			   rule->pattern.bSTAG_PCP_DEI_Enable);
		SEQ_PRINTF(s, "  nSTAG_PCP_DEI           =   %d\n",
			   rule->pattern.nSTAG_PCP_DEI);
		SEQ_PRINTF(s, "  bSTAG_PCP_DEI_Exclude   =   %d\n",
			   rule->pattern.bSTAG_PCP_DEI_Exclude);
	}
	if (rule->pattern.bPktLngEnable) {
		SEQ_PRINTF(s, "  bPktLngEnable           =   %d\n",
			   rule->pattern.bPktLngEnable);
		SEQ_PRINTF(s, "  nPktLng                 =   %d\n",
			   rule->pattern.nPktLng);
		SEQ_PRINTF(s, "  nPktLngRange            =   %d\n",
			   rule->pattern.nPktLngRange);
		SEQ_PRINTF(s, "  bPktLng_Exclude         =   %d\n",
			   rule->pattern.bPktLng_Exclude);
	}
	if (rule->pattern.bMAC_DstEnable) {
		SEQ_PRINTF(s, "  bMAC_DstEnable          =   %d\n",
			   rule->pattern.bMAC_DstEnable);
		SEQ_PRINTF(s,
			   "  nMAC_Dst                =   %02x:%2x:%2x:%2x:%2x:%2x\n",
			   rule->pattern.nMAC_Dst[0],
			   rule->pattern.nMAC_Dst[1],
			   rule->pattern.nMAC_Dst[2],
			   rule->pattern.nMAC_Dst[3],
			   rule->pattern.nMAC_Dst[4],
			   rule->pattern.nMAC_Dst[5]);
		SEQ_PRINTF(s, "  nMAC_DstMask            =   %x\n",
			   rule->pattern.nMAC_DstMask);
		SEQ_PRINTF(s, "  bDstMAC_Exclude         =   %d\n",
			   rule->pattern.bDstMAC_Exclude);
	}
	if (rule->pattern.bMAC_SrcEnable) {
		SEQ_PRINTF(s, "  bMAC_SrcEnable          =   %d\n",
			   rule->pattern.bMAC_SrcEnable);
		SEQ_PRINTF(s,
			   "  nMAC_Src                =   %02x:%2x:%2x:%2x:%2x:%2x\n",
			   rule->pattern.nMAC_Src[0],
			   rule->pattern.nMAC_Src[1],
			   rule->pattern.nMAC_Src[2],
			   rule->pattern.nMAC_Src[3],
			   rule->pattern.nMAC_Src[4],
			   rule->pattern.nMAC_Src[5]);
		SEQ_PRINTF(s, "  nMAC_SrcMask            =   %x\n",
			   rule->pattern.nMAC_SrcMask);
		SEQ_PRINTF(s, "  bSrcMAC_Exclude         =   %d\n",
			   rule->pattern.bSrcMAC_Exclude);
	}
	if (rule->pattern.bAppDataMSB_Enable) {
		SEQ_PRINTF(s, "  bAppDataMSB_Enable      =   %d\n",
			   rule->pattern.bAppDataMSB_Enable);
		SEQ_PRINTF(s, "  nAppDataMSB             =   %x\n",
			   rule->pattern.nAppDataMSB);
		SEQ_PRINTF(s, "  bAppMaskRangeMSB_Select =   %d\n",
			   rule->pattern.bAppMaskRangeMSB_Select);
		SEQ_PRINTF(s, "  nAppMaskRangeMSB        =   %x\n",
			   rule->pattern.nAppMaskRangeMSB);
		SEQ_PRINTF(s, "  bAppMSB_Exclude         =   %d\n",
			   rule->pattern.bAppMSB_Exclude);
	}
	if (rule->pattern.bAppDataLSB_Enable) {
		SEQ_PRINTF(s, "  bAppDataLSB_Enable      =   %d\n",
			   rule->pattern.bAppDataLSB_Enable);
		SEQ_PRINTF(s, "  nAppDataLSB             =   %x\n",
			   rule->pattern.nAppDataLSB);
		SEQ_PRINTF(s, "  bAppMaskRangeLSB_Select =   %d\n",
			   rule->pattern.bAppMaskRangeLSB_Select);
		SEQ_PRINTF(s, "  nAppMaskRangeLSB        =   %x\n",
			   rule->pattern.nAppMaskRangeLSB);
		SEQ_PRINTF(s, "  bAppLSB_Exclude         =   %d\n",
			   rule->pattern.bAppLSB_Exclude);
	}
	if (rule->pattern.eDstIP_Select) {
		SEQ_PRINTF(s, "  eDstIP_Select           =   %d\n",
			   rule->pattern.eDstIP_Select);
		SEQ_PRINTF(s, "  nDstIP                  =   %08x ",
			   rule->pattern.nDstIP.nIPv4);
		if (rule->pattern.eDstIP_Select == 2)
			for (i = 2; i < 8; i++)
				SEQ_PRINTF(s, "%04x ",
					   rule->pattern.nDstIP.nIPv6[i]);
		SEQ_PRINTF(s, "\n");
		SEQ_PRINTF(s, "  nDstIP_Mask             =   %x\n",
			   rule->pattern.nDstIP_Mask);
		SEQ_PRINTF(s, "  bDstIP_Exclude          =   %d\n",
			   rule->pattern.bDstIP_Exclude);
	}
	if (rule->pattern.eInnerDstIP_Select) {
		SEQ_PRINTF(s, "  eInnerDstIP_Select      =   %d\n",
			   rule->pattern.eInnerDstIP_Select);
		SEQ_PRINTF(s, "  nInnerDstIP             =   %x\n",
			   rule->pattern.nInnerDstIP.nIPv4);
		SEQ_PRINTF(s, "  nInnerDstIP_Mask        =   %x\n",
			   rule->pattern.nInnerDstIP_Mask);
		SEQ_PRINTF(s, "  bInnerDstIP_Exclude     =   %d\n",
			   rule->pattern.bInnerDstIP_Exclude);
	}
	if (rule->pattern.eSrcIP_Select) {
		SEQ_PRINTF(s, "  eSrcIP_Select           =   %d\n",
			   rule->pattern.eSrcIP_Select);
		SEQ_PRINTF(s, "  nSrcIP                  =   %x\n",
			   rule->pattern.nSrcIP.nIPv4);
		SEQ_PRINTF(s, "  nSrcIP_Mask             =   %x\n",
			   rule->pattern.nSrcIP_Mask);
		SEQ_PRINTF(s, "  bSrcIP_Exclude          =   %d\n",
			   rule->pattern.bSrcIP_Exclude);
	}
	if (rule->pattern.eInnerSrcIP_Select) {
		SEQ_PRINTF(s, "  eInnerSrcIP_Select      =   %d\n",
			   rule->pattern.eInnerSrcIP_Select);
		SEQ_PRINTF(s, "  nInnerSrcIP             =   %x\n",
			   rule->pattern.nInnerSrcIP.nIPv4);
		SEQ_PRINTF(s, "  nInnerSrcIP_Mask        =   %x\n",
			   rule->pattern.nInnerSrcIP_Mask);
		SEQ_PRINTF(s, "  bInnerSrcIP_Exclude     =   %d\n",
			   rule->pattern.bInnerSrcIP_Exclude);
	}
	if (rule->pattern.bEtherTypeEnable) {
		SEQ_PRINTF(s, "  bEtherTypeEnable        =   %d\n",
			   rule->pattern.bEtherTypeEnable);
		SEQ_PRINTF(s, "  nEtherType              =   %x\n",
			   rule->pattern.nEtherType);
		SEQ_PRINTF(s, "  nEtherTypeMask          =   %x\n",
			   rule->pattern.nEtherTypeMask);
		SEQ_PRINTF(s, "  bEtherType_Exclude      =   %d\n",
			   rule->pattern.bEtherType_Exclude);
	}
	if (rule->pattern.bProtocolEnable) {
		SEQ_PRINTF(s, "  bProtocolEnable         =   %d\n",
			   rule->pattern.bProtocolEnable);
		SEQ_PRINTF(s, "  nProtocol               =   %x\n",
			   rule->pattern.nProtocol);
		SEQ_PRINTF(s, "  nProtocolMask           =   %x\n",
			   rule->pattern.nProtocolMask);
		SEQ_PRINTF(s, "  bProtocol_Exclude       =   %d\n",
			   rule->pattern.bProtocol_Exclude);
	}
	if (rule->pattern.bInnerProtocolEnable) {
		SEQ_PRINTF(s, "  bInnerProtocolEnable    =   %d\n",
			   rule->pattern.bInnerProtocolEnable);
		SEQ_PRINTF(s, "  nInnerProtocol          =   %x\n",
			   rule->pattern.nInnerProtocol);
		SEQ_PRINTF(s, "  nInnerProtocolMask      =   %x\n",
			   rule->pattern.nInnerProtocolMask);
		SEQ_PRINTF(s, "  bInnerProtocol_Exclude  =   %d\n",
			   rule->pattern.bInnerProtocol_Exclude);
	}
	if (rule->pattern.bSessionIdEnable) {
		SEQ_PRINTF(s, "  bSessionIdEnable        =   %d\n",
			   rule->pattern.bSessionIdEnable);
		SEQ_PRINTF(s, "  nSessionId              =   %x\n",
			   rule->pattern.nSessionId);
		SEQ_PRINTF(s, "  bSessionId_Exclude      =   %d\n",
			   rule->pattern.bSessionId_Exclude);
	}
	if (rule->pattern.bPPP_ProtocolEnable) {
		SEQ_PRINTF(s, "  bPPP_ProtocolEnable     =   %d\n",
			   rule->pattern.bPPP_ProtocolEnable);
		SEQ_PRINTF(s, "  nPPP_Protocol           =   %x\n",
			   rule->pattern.nPPP_Protocol);
		SEQ_PRINTF(s, "  nPPP_ProtocolMask       =   %x\n",
			   rule->pattern.nPPP_ProtocolMask);
		SEQ_PRINTF(s, "  bPPP_Protocol_Exclude   =   %d\n",
			   rule->pattern.bPPP_Protocol_Exclude);
	}
	if (rule->pattern.bVid) {
		SEQ_PRINTF(s, "  bVid                    =   %d\n",
			   rule->pattern.bVid);
		SEQ_PRINTF(s, "  nVid                    =   %d\n",
			   rule->pattern.nVid);
		SEQ_PRINTF(s, "  bVid_Exclude            =   %d\n",
			   rule->pattern.bVid_Exclude);
	}
	if (rule->pattern.bSLAN_Vid) {
		SEQ_PRINTF(s, "  bSLAN_Vid               =    %d\n",
			   rule->pattern.bSLAN_Vid);
		SEQ_PRINTF(s, "  nSLAN_Vid               =    %d\n",
			   rule->pattern.nSLAN_Vid);
		SEQ_PRINTF(s, "  bSLANVid_Exclude        =    %d\n",
			   rule->pattern.bSLANVid_Exclude);
	}
	if (rule->pattern.bPayload1_SrcEnable) {
		SEQ_PRINTF(s, "  bPayload1_SrcEnable     =   %d\n",
			   rule->pattern.bPayload1_SrcEnable);
		SEQ_PRINTF(s, "  nPayload1               =   %x\n",
			   rule->pattern.nPayload1);
		SEQ_PRINTF(s, "  nPayload1_Mask          =   %x\n",
			   rule->pattern.nPayload1_Mask);
		SEQ_PRINTF(s, "  bPayload1_Exclude       =   %d\n",
			   rule->pattern.bPayload1_Exclude);
	}
	if (rule->pattern.bPayload2_SrcEnable) {
		SEQ_PRINTF(s, "  bPayload2_SrcEnable     =   %d\n",
			   rule->pattern.bPayload2_SrcEnable);
		SEQ_PRINTF(s, "  nPayload2               =   %x\n",
			   rule->pattern.nPayload2);
		SEQ_PRINTF(s, "  nPayload2_Mask          =   %x\n",
			   rule->pattern.nPayload2_Mask);
		SEQ_PRINTF(s, "  bPayload2_Exclude       =   %d\n",
			   rule->pattern.bPayload2_Exclude);
	}
	if (rule->pattern.bParserFlagLSB_Enable) {
		SEQ_PRINTF(s, "  bParserFlagLSB_Enable   =   %d\n",
			   rule->pattern.bParserFlagLSB_Enable);
		SEQ_PRINTF(s, "  nParserFlagLSB          =   %x\n",
			   rule->pattern.nParserFlagLSB);
		SEQ_PRINTF(s, "  nParserFlagLSB_Mask     =   %x\n",
			   rule->pattern.nParserFlagLSB_Mask);
		SEQ_PRINTF(s, "  bParserFlagLSB_Exclude  =   %d\n",
			   rule->pattern.bParserFlagLSB_Exclude);
	}
	if (rule->pattern.bParserFlagMSB_Enable) {
		SEQ_PRINTF(s, "  bParserFlagMSB_Enable   =   %d\n",
			   rule->pattern.bParserFlagMSB_Enable);
		SEQ_PRINTF(s, "  nParserFlagMSB          =   %x\n",
			   rule->pattern.nParserFlagMSB);
		SEQ_PRINTF(s, "  nParserFlagMSB_Mask     =   %x\n",
			   rule->pattern.nParserFlagMSB_Mask);
		SEQ_PRINTF(s, "  bParserFlagMSB_Exclude  =   %d\n",
			   rule->pattern.bParserFlagMSB_Exclude);
	}

	SEQ_PRINTF(s, "Action:\n");
	if (rule->action.eTrafficClassAction) {
		SEQ_PRINTF(s, "  eTrafficClassAction      =   %d\n",
			   rule->action.eTrafficClassAction);
		SEQ_PRINTF(s, "  nTrafficClassAlternate   =   %d\n",
			   rule->action.nTrafficClassAlternate);
	}
	if (rule->action.eSnoopingTypeAction)
		SEQ_PRINTF(s, "  eSnoopingTypeAction      =   %d\n",
			   rule->action.eSnoopingTypeAction);
	if (rule->action.eLearningAction)
		SEQ_PRINTF(s, "  eLearningAction          =   %d\n",
			   rule->action.eLearningAction);
	if (rule->action.eIrqAction)
		SEQ_PRINTF(s, "  eIrqAction               =   %d\n",
			   rule->action.eIrqAction);
	if (rule->action.eCrossStateAction)
		SEQ_PRINTF(s, "  eCrossStateAction        =   %d\n",
			   rule->action.eCrossStateAction);
	if (rule->action.eCritFrameAction)
		SEQ_PRINTF(s, "  eCritFrameAction         =   %d\n",
			   rule->action.eCritFrameAction);
	if (rule->action.eTimestampAction) {
		SEQ_PRINTF(s, "  eTimestampAction         =   %d\n",
			   rule->action.eTimestampAction);
	}
	if (rule->action.ePortMapAction) {
		SEQ_PRINTF(s, "  ePortMapAction           =   %d\n",
			   rule->action.ePortMapAction);
		SEQ_PRINTF(s, "  nForwardPortMap          =   %d\n",
			   rule->action.nForwardPortMap);
		SEQ_PRINTF(s, "  nForwardSubIfId          =   %d\n",
			   rule->action.nForwardSubIfId);
	}
	if (rule->action.bRemarkAction)
		SEQ_PRINTF(s, "  bRemarkAction            =   %d\n",
			   rule->action.bRemarkAction);
	if (rule->action.bRemarkPCP)
		SEQ_PRINTF(s, "  bRemarkPCP               =   %d\n",
			   rule->action.bRemarkPCP);
	if (rule->action.bRemarkSTAG_PCP)
		SEQ_PRINTF(s, "  bRemarkSTAG_PCP          =   %d\n",
			   rule->action.bRemarkSTAG_PCP);
	if (rule->action.bRemarkSTAG_DEI)
		SEQ_PRINTF(s, "  bRemarkSTAG_DEI          =   %d\n",
			   rule->action.bRemarkSTAG_DEI);
	if (rule->action.bRemarkDSCP)
		SEQ_PRINTF(s, "  bRemarkDSCP              =   %d\n",
			   rule->action.bRemarkDSCP);
	if (rule->action.bRemarkClass) {
		SEQ_PRINTF(s, "  bRemarkClass             =   %d\n",
			   rule->action.bRemarkClass);
	}
	if (rule->action.eMeterAction) {
		SEQ_PRINTF(s, "  eMeterAction             =   %d\n",
			   rule->action.eMeterAction);
		SEQ_PRINTF(s, "  nMeterId                 =   %d\n",
			   rule->action.nMeterId);
	}
	if (rule->action.bRMON_Action) {
		SEQ_PRINTF(s, "  bRMON_Action             =   %d\n",
			   rule->action.bRMON_Action);
		SEQ_PRINTF(s, "  nRMON_Id                 =   %d\n",
			   rule->action.nRMON_Id);
	}
	if (rule->action.eVLAN_Action) {
		SEQ_PRINTF(s, "  eVLAN_Action             =   %d\n",
			   rule->action.eVLAN_Action);
		SEQ_PRINTF(s, "  nVLAN_Id                 =   %d\n",
			   rule->action.nVLAN_Id);
	}
	if (rule->action.eSVLAN_Action) {
		SEQ_PRINTF(s, "  eSVLAN_Action            =   %d\n",
			   rule->action.eSVLAN_Action);
		SEQ_PRINTF(s, "  nSVLAN_Id                =   %d\n",
			   rule->action.nSVLAN_Id);
	}
	if (rule->action.eVLAN_CrossAction)
		SEQ_PRINTF(s, "  eVLAN_CrossAction        =   %d\n",
			   rule->action.eVLAN_CrossAction);
	if (rule->action.nFId)
		SEQ_PRINTF(s, "  nFId                     =   %d\n",
			   rule->action.nFId);
	if (rule->action.bPortBitMapMuxControl)
		SEQ_PRINTF(s, "  bPortBitMapMuxControl    =   %d\n",
			   rule->action.bPortBitMapMuxControl);
	if (rule->action.bPortTrunkAction)
		SEQ_PRINTF(s, "  bPortTrunkAction         =   %d\n",
			   rule->action.bPortTrunkAction);
	if (rule->action.bPortLinkSelection)
		SEQ_PRINTF(s, "  bPortLinkSelection       =   %d\n",
			   rule->action.bPortLinkSelection);
	if (rule->action.bCVLAN_Ignore_Control)
		SEQ_PRINTF(s, "  bCVLAN_Ignore_Control    =   %d\n",
			   rule->action.bCVLAN_Ignore_Control);
	if (rule->action.bFlowID_Action) {
		SEQ_PRINTF(s, "  bFlowID_Action           =   %d\n",
			   rule->action.bFlowID_Action);
		SEQ_PRINTF(s, "  nFlowID                  =   %d\n",
			   rule->action.nFlowID);
	}
	if (rule->action.bRoutExtId_Action) {
		SEQ_PRINTF(s, "  bRoutExtId_Action        =   %d\n",
			   rule->action.bRoutExtId_Action);
		SEQ_PRINTF(s, "  nRoutExtId               =   %d\n",
			   rule->action.nRoutExtId);
	}
	if (rule->action.bRtDstPortMaskCmp_Action)
		SEQ_PRINTF(s, "  bRtDstPortMaskCmp_Action =   %d\n",
			   rule->action.bRtDstPortMaskCmp_Action);
	if (rule->action.bRtSrcPortMaskCmp_Action)
		SEQ_PRINTF(s, "  bRtSrcPortMaskCmp_Action =   %d\n",
			   rule->action.bRtSrcPortMaskCmp_Action);
	if (rule->action.bRtDstIpMaskCmp_Action)
		SEQ_PRINTF(s, "  bRtDstIpMaskCmp_Action   =   %d\n",
			   rule->action.bRtDstIpMaskCmp_Action);
	if (rule->action.bRtSrcIpMaskCmp_Action)
		SEQ_PRINTF(s, "  bRtSrcIpMaskCmp_Action   =   %d\n",
			   rule->action.bRtSrcIpMaskCmp_Action);
	if (rule->action.bRtInnerIPasKey_Action)
		SEQ_PRINTF(s, "  bRtInnerIPasKey_Action   =   %d\n",
			   rule->action.bRtInnerIPasKey_Action);
	if (rule->action.bRtAccelEna_Action)
		SEQ_PRINTF(s, "  bRtAccelEna_Action       =   %d\n",
			   rule->action.bRtAccelEna_Action);
	if (rule->action.bRtCtrlEna_Action)
		SEQ_PRINTF(s, "  bRtCtrlEna_Action        =   %d\n",
			   rule->action.bRtCtrlEna_Action);
	if (rule->action.eProcessPath_Action)
		SEQ_PRINTF(s, "  eProcessPath_Action      =   %d\n",
			   rule->action.eProcessPath_Action);
	if (rule->action.ePortFilterType_Action)
		SEQ_PRINTF(s, "  ePortFilterType_Action   =   %d\n",
			   rule->action.ePortFilterType_Action);
	ret = SEQ_PRINTF(s, "\n");
	if (ret) {
		kfree(rule);
		return pos;
	}
 EXIT:
	kfree(rule);
	pos++;

	return pos;
}

int proc_gsw_pce_start(void)
{
	return 0;
}

#define NS_INT16SZ	 2
#define NS_INADDRSZ	 4
#define NS_IN6ADDRSZ	16

/* int
 * inet_pton(af, src, dst)
 *	convert from presentation format (which usually means ASCII printable)
 *	to network format (which is usually some kind of binary format).
 * return:
 *	4 if the address was valid and it is IPV4 format
 *  6 if the address was valid and it is IPV6 format
 *	0 if some other error occurred (`dst' is untouched in this case, too)
 * author:
 *	Paul Vixie, 1996.
 */
int pton(const char *src, void *dst)
{
	int ip_v = 0;

	if (strstr(src, ":")) {	/* IPV6 */
		if (inet_pton6(src, dst) == 1) {
			ip_v = 6;
			return ip_v;
		}

	} else {
		if (inet_pton4(src, dst) == 1) {
			ip_v = 4;
			return ip_v;
		}
	}

	return ip_v;
}

/* int
 * inet_pton4(src, dst)
 *	like inet_aton() but without all the hexadecimal and shorthand.
 * return:
 *	1 if `src' is a valid dotted quad, else 0.
 * notice:
 *	does not touch `dst' unless it's returning 1.
 * author:
 *	Paul Vixie, 1996.
 */
static int inet_pton4(const char *src, u_char *dst)
{
	static const char digits[] = "0123456789";
	int saw_digit, octets, ch;
	u_char tmp[NS_INADDRSZ], *tp;

	saw_digit = 0;
	octets = 0;
	*(tp = tmp) = 0;
	while ((ch = *src++) != '\0') {
		const char *pch;

		pch = strchr(digits, ch);
		if (pch != NULL) {
			u_int new = *tp * 10 + (u_int) (pch - digits);

			if (new > 255)
				return 0;
			*tp = (u_char) new;
			if (!saw_digit) {
				if (++octets > 4)
					return 0;
				saw_digit = 1;
			}
		} else if (ch == '.' && saw_digit) {
			if (octets == 4)
				return 0;
			*++tp = 0;
			saw_digit = 0;
		} else
			return 0;
	}
	if (octets < 4)
		return 0;

	memcpy(dst, tmp, NS_INADDRSZ);
	return 1;
}

/* int
 * inet_pton6(src, dst)
 *	convert presentation level address to network order binary form.
 * return:
 *	1 if `src' is a valid [RFC1884 2.2] address, else 0.
 * notice:
 *	(1) does not touch `dst' unless it's returning 1.
 *	(2) :: in a full address is silently ignored.
 * credit:
 *	inspired by Mark Andrews.
 * author:
 *	Paul Vixie, 1996.
 */
static int inet_pton6(const char *src, u_char *dst)
{
	static const char xdigits_l[] = "0123456789abcdef", xdigits_u[] =
	    "0123456789ABCDEF";
	u_char tmp[NS_IN6ADDRSZ], *tp, *endp, *colonp;
	const char *xdigits, *curtok;
	int ch, saw_xdigit;
	u_int val;

	memset((tp = tmp), '\0', NS_IN6ADDRSZ);
	endp = tp + NS_IN6ADDRSZ;
	colonp = NULL;
	/* Leading :: requires some special handling. */
	if (*src == ':')
		if (*++src != ':')
			return 0;
	curtok = src;
	saw_xdigit = 0;
	val = 0;
	while ((ch = *src++) != '\0') {
		const char *pch;

		pch = strchr((xdigits = xdigits_l), ch);
		if (pch == NULL)
			pch = strchr((xdigits = xdigits_u), ch);
		if (pch != NULL) {
			val <<= 4;
			val |= (pch - xdigits);
			if (val > 0xffff)
				return 0;
			saw_xdigit = 1;
			continue;
		}
		if (ch == ':') {
			curtok = src;
			if (!saw_xdigit) {
				if (colonp)
					return 0;
				colonp = tp;
				continue;
			}
			if (tp + NS_INT16SZ > endp)
				return 0;
			*tp++ = (u_char) (val >> 8) & 0xff;
			*tp++ = (u_char) val & 0xff;
			saw_xdigit = 0;
			val = 0;
			continue;
		}
		if (ch == '.' && ((tp + NS_INADDRSZ) <= endp) &&
		    inet_pton4(curtok, tp) > 0) {
			tp += NS_INADDRSZ;
			saw_xdigit = 0;
			break;	/* '\0' was seen by inet_pton4(). */
		}
		return 0;
	}
	if (saw_xdigit) {
		if (tp + NS_INT16SZ > endp)
			return 0;
		*tp++ = (u_char) (val >> 8) & 0xff;
		*tp++ = (u_char) val & 0xff;
	}
	if (colonp != NULL) {
		/*
		 * Since some memmove()'s erroneously fail to handle
		 * overlapping regions, we'll do the shift by hand.
		 */
		const int n = (int)(tp - colonp);
		int i;

		for (i = 1; i <= n; i++) {
			endp[-i] = colonp[n - i];
			colonp[n - i] = 0;
		}
		tp = endp;
	}
	if (tp != endp)
		return 0;
	memcpy(dst, tmp, NS_IN6ADDRSZ);
	return 1;
}

static int mac_stob(const char *mac, u8 bytes[6])
{
	int values[6];
	int i;
	int f = 0;

	if (!mac || !bytes)
		return -1;

	if (6 ==
	    sscanf(mac, "%x:%x:%x:%x:%x:%x", &values[0], &values[1],
		   &values[2], &values[3], &values[4], &values[5]))
		f = 1;
	else if (6 ==
		 sscanf(mac, "%x-%x-%x-%x-%x-%x", &values[0], &values[1],
			&values[2], &values[3], &values[4], &values[5]))
		f = 1;

	if (f) {		/* convert to uint8_t */
		for (i = 0; i < 6; ++i)
			bytes[i] = (u8) values[i];
		return 0;
	}
	return -1;

}

char *get_pae_ip_type(int type)
{
	if (type == GSW_RT_IP_V4)
		return "IPV4";
	if (type == GSW_RT_IP_V6)
		return "IPV6";
	return "Unknown";
}

char *get_pae_tunnel_type(int type)
{
	if (type == GSW_ROUTE_TUNL_NULL)
		return "NULL";
	if (type == GSW_ROUTE_TUNL_6RD)
		return "6RD";
	if (type == GSW_ROUTE_TUNL_DSLITE)
		return "Dslite";
	if (type == GSW_ROUTE_TUNL_L2TP)
		return "L2TP";
	if (type == GSW_ROUTE_TUNL_IPSEC)
		return "IPSEC";
	return "Unknown";
}

char *get_pae_ext_type(int type)
{
	if (type == 100)
		return "UDP";
	if (type == 0)
		return "TCP";
	return "Unknown";
}

char *get_pae_dir_type(int type)
{
	if (type == GSW_ROUTE_DIRECTION_DNSTREAM)
		return "DownStream";
	if (type == GSW_ROUTE_DIRECTION_UPSTREAM)
		return "UpStream";
	return "Unknown";
}

char *get_pae_pppoe_mode_type(int type)
{
	if (type == 0)
		return "Transparent";
	if (type == GSW_ROUTE_DIRECTION_UPSTREAM)
		return "Termination";
	return "Unknown";
}

char *get_pae_route_mode_type(int type)
{
	if (type == GSW_ROUTE_MODE_NULL)
		return "NULL";
	if (type == GSW_ROUTE_MODE_ROUTING)
		return "Basic Routing";
	if (type == GSW_ROUTE_MODE_NAT)
		return "NAT";
	if (type == GSW_ROUTE_MODE_NAPT)
		return "NAPT";
	return "Unknown";
}

char *get_pae_out_dscp_type(int type)
{
	if (type == GSW_ROUTE_OUT_DSCP_NULL)
		return "No Outer DSCP Marking";
	if (type == GSW_ROUTE_OUT_DSCP_INNER)
		return "Outer DSCP from Inner IP header";
	if (type == GSW_ROUTE_OUT_DSCP_SESSION)
		return "Outer DSCP from Session action";
	return "Unknown";
}


/* For proc only, no protection */

char *get_pae_port_list(u32 port)
{
	int i, k;
	static char list[PMAC_MAX_NUM * 2 + 1];

	k = 0;
	list[0] = 0;
	for (i = 0; i < PMAC_MAX_NUM; i++) {
		if (port & (1 << i)) {
			if (k)
				sprintf(list + strlen(list), "/");
			sprintf(list + strlen(list), "%d", i);
			k++;
		}
	}
	return list;
}

/* return 0 -- ok,
   return 1 -- buffer overfolow */
static int dp_route_dump(struct seq_file *seq, GSW_ROUTE_Entry_t *rt_entry)
{
	smart_proc_printf(seq, "Index[%04d] Hash=%u: %s(%u)\n",
			  rt_entry->nRtIndex, rt_entry->nHashVal,
			  (rt_entry->routeEntry.pattern.bValid ==
			   LTQ_TRUE) ? "Valid" : "Not Valid",
			  rt_entry->routeEntry.pattern.bValid);
	smart_proc_printf(seq, " Compare:\n");
	smart_proc_printf(seq, "   IP Type         = %d (%s)\n",
			  rt_entry->routeEntry.pattern.eIpType,
			  get_pae_ip_type(rt_entry->routeEntry.pattern.
					  eIpType));
	if (rt_entry->routeEntry.action.eIpType == GSW_RT_IP_V6)
		smart_proc_printf(seq, "   Src IP          = %pI6\n",
				  rt_entry->routeEntry.pattern.nSrcIP.nIPv6);
	else
		smart_proc_printf(seq, "   Src IP          = %pI4\n",
				  &rt_entry->routeEntry.pattern.nSrcIP.nIPv4);

	if (rt_entry->routeEntry.pattern.eIpType == GSW_RT_IP_V6)
		smart_proc_printf(seq, "   Dest IP         = %pI6\n",
				  rt_entry->routeEntry.pattern.nDstIP.nIPv6);
	else
		smart_proc_printf(seq, "   Dest IP         = %pI4\n",
				  &rt_entry->routeEntry.pattern.nDstIP.nIPv4);

	smart_proc_printf(seq, "   Src Port        = %d\n",
			  rt_entry->routeEntry.pattern.nSrcPort);
	smart_proc_printf(seq, "   Dest Port       = %d\n",
			  rt_entry->routeEntry.pattern.nDstPort);
	smart_proc_printf(seq, "   Extn Id         = %d (%s)\n",
			  rt_entry->routeEntry.pattern.nRoutExtId,
			  get_pae_ext_type(rt_entry->routeEntry.pattern.
					   nRoutExtId));
	smart_proc_printf(seq, " Action:\n");
	smart_proc_printf(seq, "   Dst PMAC List   = 0x%0x (%s)\n",
			  rt_entry->routeEntry.action.nDstPortMap,
			  get_pae_port_list(rt_entry->routeEntry.action.
					    nDstPortMap));
	smart_proc_printf(seq, "   Subif           = 0x%0x\n",
			  rt_entry->routeEntry.action.nDstSubIfId);
	smart_proc_printf(seq, "   IP Type         = %d (%s)\n",
			  rt_entry->routeEntry.action.eIpType,
			  get_pae_ip_type(rt_entry->routeEntry.action.
					  eIpType));
	if (rt_entry->routeEntry.action.eIpType == GSW_RT_IP_V6)
		smart_proc_printf(seq, "   NAT IP          = %pI6\n",
				  rt_entry->routeEntry.action.nNATIPaddr.
				  nIPv6);
	else
		smart_proc_printf(seq, "   NAT IP          = %pI4\n",
				  &rt_entry->routeEntry.action.nNATIPaddr.
				  nIPv4);
	smart_proc_printf(seq, "   NAT Port        = %d\n",
			  rt_entry->routeEntry.action.nTcpUdpPort);
	smart_proc_printf(seq, "   MTU             = %d\n",
			  rt_entry->routeEntry.action.nMTUvalue);
	smart_proc_printf(seq, "   Src MAC         = %pM (%s)\n",
			  rt_entry->routeEntry.action.nSrcMAC,
			  rt_entry->routeEntry.action.
			  bMAC_SrcEnable ? "Enabled" : "Disabled");
	smart_proc_printf(seq, "   Dst MAC         = %pM (%s)\n",
			  rt_entry->routeEntry.action.nDstMAC,
			  rt_entry->routeEntry.action.
			  bMAC_DstEnable ? "Enabled" : "Disabled");
	smart_proc_printf(seq, "   PPPoE Mode      = %u (%s)\n",
			  rt_entry->routeEntry.action.bPPPoEmode,
			  get_pae_pppoe_mode_type(rt_entry->routeEntry.action.
						  bPPPoEmode));
	smart_proc_printf(seq, "   PPPoE SessID    = %u\n",
			  rt_entry->routeEntry.action.nPPPoESessId);
	smart_proc_printf(seq, "   Dir             = %u (%s)\n",
			  rt_entry->routeEntry.action.eSessDirection,
			  get_pae_dir_type(rt_entry->routeEntry.action.
					   eSessDirection));
	smart_proc_printf(seq, "   Class           = %u (%s)\n",
			  rt_entry->routeEntry.action.nTrafficClass,
			  rt_entry->routeEntry.action.
			  bTCremarking ? "Enabled" : "Disabled");
	smart_proc_printf(seq, "   Routing Mode    = %u (%s)\n",
			  rt_entry->routeEntry.action.eSessRoutingMode,
			  get_pae_route_mode_type(rt_entry->routeEntry.action.
						  eSessRoutingMode));
	smart_proc_printf(seq, "   Tunnel Type     = %u (%s: %s\n",
			  rt_entry->routeEntry.action.eTunType,
			  get_pae_tunnel_type(rt_entry->routeEntry.action.
					      eTunType),
			  rt_entry->routeEntry.action.
			  bTunnel_Enable ? "Enabled" : "Disabled");
	smart_proc_printf(seq, "   Tunnel Index    = %u\n",
			  rt_entry->routeEntry.action.nTunnelIndex);
	smart_proc_printf(seq, "   MeterID         = %u (%s)\n",
			  rt_entry->routeEntry.action.nMeterId,
			  rt_entry->routeEntry.action.
			  bMeterAssign ? "Enabled" : "Disabled");
	smart_proc_printf(seq, "   TTL  Decrease   = %u (%s)\n",
			  rt_entry->routeEntry.action.bTTLDecrement,
			  rt_entry->routeEntry.action.
			  bTTLDecrement ? "Enabled" : "Disabled");
	smart_proc_printf(seq, "   OutDSCP         = %u (%s)\n",
			  rt_entry->routeEntry.action.eOutDSCPAction,
			  get_pae_out_dscp_type(rt_entry->routeEntry.action.eOutDSCPAction));
	smart_proc_printf(seq, "   InDSCP          = %u (%s)\n",
			  rt_entry->routeEntry.action.bInnerDSCPRemark,
			  rt_entry->routeEntry.action.
			  bInnerDSCPRemark ? "Enabled" : "Disabled");
	smart_proc_printf(seq, "   DSCP            = %u\n",
			  rt_entry->routeEntry.action.nDSCP);
	smart_proc_printf(seq, "   RTP             = %s (seq=%u roll=%u)\n",
			  rt_entry->routeEntry.action.
			  bRTPMeasEna ? "Enabled" : "Disabled",
			  rt_entry->routeEntry.action.nRTPSeqNumber,
			  rt_entry->routeEntry.action.nRTPSessionPktCnt);
	smart_proc_printf(seq, "   FID             = %u\n",
			  rt_entry->routeEntry.action.nFID);
	smart_proc_printf(seq, "   Flow ID         = %u\n",
			  rt_entry->routeEntry.action.nFlowId);
	smart_proc_printf(seq, "   Hit Status      = %u\n",
			  rt_entry->routeEntry.action.bHitStatus);
	smart_proc_printf(seq, "   Session Counters= %u\n",
			  rt_entry->routeEntry.action.nSessionCtrs);
	if (seq && SEQ_PRINTF(seq, "\n"))
		return 1;

	return 0;
}

#define EXT1_UP   "ext(up)  : echo add SrcIP 192.168.1.100 DstIP 192.168.0.100 SrcPort 1024 DstPort 1024 ExtId 100 SrcMac 11:11:11:11:11:11 DstMac 11:11:11:11:11:22 NatIP 192.168.0.1   NatPort 3000 MTU 1500 DstPmac 0x8000 subif 0 dir 1"
#define EXT2_DOWN "ext(down): echo add SrcIP 192.168.0.100 DstIP 192.168.0.1   SrcPort 1024 DstPort 3000 ExtId 100 SrcMac 11:11:11:11:11:33 DstMac 11:11:11:11:11:44 NatIP 192.168.1.100 NatPort 1024 MTU 1500 DstPmac 0x2    subif 0 dir 0"

ssize_t proc_gsw_route_write(struct file *file, const char *buf,
			     size_t count, loff_t *ppos)
{
	u16 len, i, tmp, start_param;
	int ret = 0;
	char *str = NULL;
	char *param_list[30 * 2];
	unsigned int num;
	GSW_ROUTE_Entry_t *rt_entry;
	GSW_API_HANDLE gsw_handle;
	u8 dscp_f = 0;

	gsw_handle = gsw_api_kopen((char *)GSWIP_R_DEV_NAME);
	if (gsw_handle == 0) {
		PR_ERR("Open GSWIP-R device FAILED !\n");
		return count;
	}
	str = kmalloc(count + 1, GFP_ATOMIC);
	if (!str) {
		PR_ERR("Failed to allocate %d bytes buffer\n", count);
		return count;
	}
	rt_entry = (GSW_ROUTE_Entry_t *) kmalloc(sizeof(GSW_ROUTE_Entry_t) + 1,
					  GFP_ATOMIC);
	if (!rt_entry) {
		PR_ERR("Failed to allocate %d bytes buffer\n",
			sizeof(GSW_ROUTE_Entry_t) + 1);
		kfree(str);
		return count;
	}

	len = count;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;

	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));
	if (num < 2) {
		PRINTK("parameter %d not enough. count=%d\n", num, count);
		goto help;
	}
	if (dp_strcmpi(param_list[0], "help") == 0)	/* help */
		goto help;

	/* delete an entry */
	if (dp_strcmpi(param_list[0], "del") == 0) {
		rt_entry->nRtIndex = dp_atoi(param_list[1]);
		DP_DEBUG(DP_DBG_FLAG_PAE, "Delete pae entry %d\n",
			 rt_entry->nRtIndex);
		ret =
		    gsw_api_kioctl(gsw_handle, GSW_ROUTE_ENTRY_DELETE,
				   (unsigned int)rt_entry);
		if (ret != GSW_statusOk) {
			PR_ERR("GSW_ROUTE_ENTRY_DELETE returned failure\n");
			goto exit;
		}
		kfree(str);
		kfree(rt_entry);
		return count;
	}

	/* dump an entry */
	if (dp_strcmpi(param_list[0], "dump") == 0) {
		rt_entry->nRtIndex = dp_atoi(param_list[1]);
		DP_DEBUG(DP_DBG_FLAG_PAE, "Dump pae entry %d\n",
			 rt_entry->nRtIndex);
		ret = gsw_api_kioctl(gsw_handle, GSW_ROUTE_ENTRY_READ,
				(unsigned int)rt_entry);
		if (ret != GSW_statusOk) {
			PR_ERR("GSW_ROUTE_ENTRY_DELETE returned failure\n");
			goto exit;
		}
		dp_route_dump(NULL, rt_entry);
		kfree(str);
		kfree(rt_entry);
		return count;
	}

	/* Modify an entry */
	if (dp_strcmpi(param_list[0], "modify") == 0) {
		rt_entry->nRtIndex = dp_atoi(param_list[1]);
		/*read back before delete and add it new */
		DP_DEBUG(DP_DBG_FLAG_PAE, "Dump pae entry %d\n",
			 rt_entry->nRtIndex);
		ret = gsw_api_kioctl(gsw_handle, GSW_ROUTE_ENTRY_READ,
			(unsigned int)rt_entry);
		if (ret != GSW_statusOk) {
			PR_ERR("GSW_ROUTE_ENTRY_DELETE returned failure\n");
			goto exit;
		}
		ret =
		    gsw_api_kioctl(gsw_handle, GSW_ROUTE_ENTRY_DELETE,
				   (unsigned int)rt_entry);
		if (ret != GSW_statusOk) {
			PR_ERR("GSW_ROUTE_ENTRY_DELETE returned failure\n");
			goto exit;
		}
		rt_entry->nHashVal = -1;	/*since GSWAPI no modify support, here switch to add command */
		start_param = 2;
		goto ADD_Modify_BOTH;
	}

	/* add a new entry */
	if (dp_strcmpi(param_list[0], "add") != 0) {
		PR_ERR("wrong command: %s\n", param_list[0]);
		goto help;
	}
	memset(rt_entry, 0, sizeof(*rt_entry));
	rt_entry->nHashVal = -1;
	rt_entry->bPrio = 1;
	rt_entry->routeEntry.action.nMTUvalue = 1501;
	rt_entry->routeEntry.pattern.bValid = LTQ_TRUE;
	start_param = 1;
 ADD_Modify_BOTH:
	for (i = start_param; i < num; i += 2) {
		/*compare table */
		if (dp_strcmpi(param_list[i], "SrcIP") == 0) {
			tmp =
			    pton(param_list[i + 1],
				 &rt_entry->routeEntry.pattern.nSrcIP);
			if (tmp == 4)
				rt_entry->routeEntry.pattern.eIpType =
				    GSW_RT_IP_V4;
			else if (tmp == 6)
				rt_entry->routeEntry.pattern.eIpType =
				    GSW_RT_IP_V6;
			else {
				PRINTK("Wong IP format for SrcIP\n");
				goto exit;
			}
		} else if (dp_strcmpi(param_list[i], "DstIP") == 0) {
			tmp =
			    pton(param_list[i + 1],
				 &rt_entry->routeEntry.pattern.nDstIP);
			if (tmp == 4)
				rt_entry->routeEntry.pattern.eIpType =
				    GSW_RT_IP_V4;
			else if (tmp == 6)
				rt_entry->routeEntry.pattern.eIpType =
				    GSW_RT_IP_V6;
			else {
				PRINTK("Wong IP format for DstIP\n");
				goto exit;
			}
		} else if (dp_strcmpi(param_list[i], "SrcPort") == 0)
			rt_entry->routeEntry.pattern.nSrcPort =
			    dp_atoi(param_list[i + 1]);
		else if (dp_strcmpi(param_list[i], "DstPort") == 0)
			rt_entry->routeEntry.pattern.nDstPort =
			    dp_atoi(param_list[i + 1]);
		else if (dp_strcmpi(param_list[i], "ExtId") == 0)
			rt_entry->routeEntry.pattern.nRoutExtId =
			    dp_atoi(param_list[i + 1]);
		else if (dp_strcmpi(param_list[i], "SrcMac") == 0) {
			rt_entry->routeEntry.action.bMAC_SrcEnable = 1;
			mac_stob(param_list[i + 1],
				 rt_entry->routeEntry.action.nSrcMAC);

			if (rt_entry->routeEntry.action.eSessRoutingMode < GSW_ROUTE_MODE_ROUTING)	/*normally only routing mode will change mac */
				rt_entry->routeEntry.action.eSessRoutingMode =
				    GSW_ROUTE_MODE_ROUTING;
		} /*below is all actions */
		else if (dp_strcmpi(param_list[i], "DstMac") == 0) {
			rt_entry->routeEntry.action.bMAC_DstEnable = 1;
			mac_stob(param_list[i + 1],
				 rt_entry->routeEntry.action.nDstMAC);

			if (rt_entry->routeEntry.action.eSessRoutingMode < GSW_ROUTE_MODE_ROUTING)	/*normally only routing mode will change mac */
				rt_entry->routeEntry.action.eSessRoutingMode =
				    GSW_ROUTE_MODE_ROUTING;
		} else if (dp_strcmpi(param_list[i], "NatIP") == 0) {

			tmp =
			    pton(param_list[i + 1],
				 &rt_entry->routeEntry.action.nNATIPaddr);
			if (tmp == 4)
				rt_entry->routeEntry.action.eIpType =
				    GSW_RT_IP_V4;
			else if (tmp == 6)
				rt_entry->routeEntry.action.eIpType =
				    GSW_RT_IP_V6;
			else {
				PRINTK("Wong IP format for NatIP\n");
				goto exit;
			}
			if (rt_entry->routeEntry.action.eSessRoutingMode <
			    GSW_ROUTE_MODE_NAT)
				rt_entry->routeEntry.action.eSessRoutingMode = GSW_ROUTE_MODE_NAT;	/* NAT */
		} else if (dp_strcmpi(param_list[i], "NatPort") == 0) {
			rt_entry->routeEntry.action.nTcpUdpPort =
			    dp_atoi(param_list[i + 1]);
			if (rt_entry->routeEntry.action.eSessRoutingMode <
			    GSW_ROUTE_MODE_NAPT)
				rt_entry->routeEntry.action.eSessRoutingMode = GSW_ROUTE_MODE_NAPT;	/* NAPT */
		} else if (dp_strcmpi(param_list[i], "MTU") == 0)
			rt_entry->routeEntry.action.nMTUvalue =
			    dp_atoi(param_list[i + 1]);
		else if (dp_strcmpi(param_list[i], "PPPoEmode") == 0)
			rt_entry->routeEntry.action.bPPPoEmode =
			    dp_atoi(param_list[i + 1]);
		else if (dp_strcmpi(param_list[i], "PPPoEId") == 0)
			rt_entry->routeEntry.action.nPPPoESessId =
			    dp_atoi(param_list[i + 1]);
		else if (dp_strcmpi(param_list[i], "TunType") == 0) {
			rt_entry->routeEntry.action.bTunnel_Enable = 1;
			rt_entry->routeEntry.action.eTunType =
			    dp_atoi(param_list[i + 1]);
		} else if (dp_strcmpi(param_list[i], "TunIndex") == 0) {
			rt_entry->routeEntry.action.bTunnel_Enable = 1;
			rt_entry->routeEntry.action.eTunType =
			    dp_atoi(param_list[i + 1]);

		} else if (dp_strcmpi(param_list[i], "MeterId") == 0) {
			rt_entry->routeEntry.action.bMeterAssign = 1;
			rt_entry->routeEntry.action.nMeterId =
			    dp_atoi(param_list[i + 1]);

		} else if (dp_strcmpi(param_list[i], "FID") == 0)
			rt_entry->routeEntry.action.nFID =
			    dp_atoi(param_list[i + 1]);
		else if (dp_strcmpi(param_list[i], "FlowId") == 0)
			rt_entry->routeEntry.action.nFlowId =
			    dp_atoi(param_list[i + 1]);
		else if (dp_strcmpi(param_list[i], "OutDscp") == 0)
			rt_entry->routeEntry.action.eOutDSCPAction =
			    dp_atoi(param_list[i + 1]);
		else if (dp_strcmpi(param_list[i], "InDscp") == 0)
			rt_entry->routeEntry.action.bInnerDSCPRemark =
			    dp_atoi(param_list[i + 1]);
		else if (dp_strcmpi(param_list[i], "Dscp") == 0) {
			rt_entry->routeEntry.action.nDSCP =
			    dp_atoi(param_list[i + 1]);
			dscp_f = 1;
		} else if (dp_strcmpi(param_list[i], "class") == 0) {
			rt_entry->routeEntry.action.bTCremarking = 1;
			rt_entry->routeEntry.action.nTrafficClass =
			    dp_atoi(param_list[i + 1]);
		} else if (dp_strcmpi(param_list[i], "ttl") == 0)
			rt_entry->routeEntry.action.bTTLDecrement =
			    dp_atoi(param_list[i + 1]);
		else if (dp_strcmpi(param_list[i], "dir") == 0)
			rt_entry->routeEntry.action.eSessDirection =
			    dp_atoi(param_list[i + 1]);
		else if (dp_strcmpi(param_list[i], "DstPmac") == 0)
			rt_entry->routeEntry.action.nDstPortMap =
			    dp_atoi(param_list[i + 1]);
		else if (dp_strcmpi(param_list[i], "Subif") == 0)
			rt_entry->routeEntry.action.nDstSubIfId =
			    dp_atoi(param_list[i + 1]);
		else {
			PRINTK("wrong parameter[%d]: %s\n", i, param_list[i]);
			goto exit;
		}

		if (!rt_entry->routeEntry.action.bTTLDecrement &&
		    (rt_entry->routeEntry.action.eSessRoutingMode >
		     GSW_ROUTE_MODE_NULL))
			rt_entry->routeEntry.action.bTTLDecrement = 1;

		/* if key in dscp but no inner/outer dscp action enabled,
		* then auto enable indscp action
		*/
		if (dscp_f &&
			!rt_entry->routeEntry.action.eOutDSCPAction &&
			!rt_entry->routeEntry.action.bInnerDSCPRemark)
			rt_entry->routeEntry.action.bInnerDSCPRemark = 1;
		/*nSessionCtrs ??*/
	}
	ret =
	    gsw_api_kioctl(gsw_handle, GSW_ROUTE_ENTRY_ADD,
			   (unsigned int)rt_entry);
	if (ret < GSW_statusOk) {
		PR_ERR("GSW_ROUTE_ENTRY_ADD returned failure\n");
		goto exit;
	}
	DP_DEBUG(DP_DBG_FLAG_PAE, "pae entry %d updated\n",
		 rt_entry->nRtIndex);

	dp_route_dump(NULL, rt_entry);

 exit:
	kfree(str);
	kfree(rt_entry);
	return count;

 help:
	PRINTK("usage:\n");
	PRINTK("  echo del	<entry-index> > /prooc/dp/%s\n",
	       DP_PROC_FILE_ROUTE);
	PRINTK("  echo show <entry-index> > /prooc/dp/%s\n",
	       DP_PROC_FILE_ROUTE);
	PRINTK
	    ("  echo add [SrcIP] [IP-value] [DstIP] [IP-value] [SrcPort] [Port-value] [DstPort] [Port-value] [ExtId] [ExtId-value]\n");
	PRINTK
	    ("           [dir] [dir-value] [SrcMAC] [MAC-value] [DstMAC] [MAC-value] [NatIP] [IP-value] [NatPort] [Port-value]\n");
	PRINTK
	    ("           [MTU] [MTU-value] [PPPoEmode] [PPPoEmode-value] [PPPoEId] [PPPoEId-value] [TunType] [Tunnel-value]\n");
	PRINTK
	    ("           [TunIndex] [Tunnel-index-value] [MeterId] [MeterId-value] [FID] [FID-value] [FlowId] [FlowId-value]\n");
	PRINTK
	    ("           [InDscp] [InDscp-value] [Dscp] [Dscp-value] [OutDscp] [OutDscp-value] [class] [class-value]\n");
	PRINTK
	    ("           [DstPmac] [DstPmac-value] [Subif] [Subif-value]\n");
	PRINTK("		> /prooc/dp/%s\n", DP_PROC_FILE_ROUTE);
	PRINTK
	    ("  echo modify <entry-index> [followed by paramers as add command] > /prooc/dp/%s\n",
	     DP_PROC_FILE_ROUTE);

	PRINTK(" Take note:\n");
	PRINTK
	    ("     Only MAC address learned session will be accelerated by HW\n");
	PRINTK
	    ("     After modify entry, its entry index maybe changed to new one for lower API limitation\n");
	PRINTK("     ExtId: %d(%s)/%d(%s)\n", 0, get_pae_ext_type(0), 100,
	       get_pae_ext_type(100));
	PRINTK("     Dir: %d(%s)/%d(%s)\n", GSW_ROUTE_DIRECTION_DNSTREAM,
	       get_pae_dir_type(GSW_ROUTE_DIRECTION_DNSTREAM),
	       GSW_ROUTE_DIRECTION_UPSTREAM,
	       get_pae_dir_type(GSW_ROUTE_DIRECTION_UPSTREAM));
	PRINTK("     OutDscp: %d(%s)/%d(%s)/%d(%s)\n",
		GSW_ROUTE_OUT_DSCP_NULL,
		get_pae_dir_type(GSW_ROUTE_OUT_DSCP_NULL),
		GSW_ROUTE_OUT_DSCP_INNER,
		get_pae_out_dscp_type(GSW_ROUTE_OUT_DSCP_INNER),
		GSW_ROUTE_OUT_DSCP_SESSION,
		get_pae_out_dscp_type(GSW_ROUTE_OUT_DSCP_SESSION));
	PRINTK("     Tunnel: %d(%s)/%d(%s)/%d(%s)/%d(%s)/%d(%s)\n",
	       GSW_ROUTE_TUNL_NULL, get_pae_tunnel_type(GSW_ROUTE_TUNL_NULL),
	       GSW_ROUTE_TUNL_6RD, get_pae_tunnel_type(GSW_ROUTE_TUNL_6RD),
	       GSW_ROUTE_TUNL_DSLITE,
	       get_pae_tunnel_type(GSW_ROUTE_TUNL_DSLITE),
	       GSW_ROUTE_TUNL_L2TP, get_pae_tunnel_type(GSW_ROUTE_TUNL_L2TP),
	       GSW_ROUTE_TUNL_IPSEC,
	       get_pae_tunnel_type(GSW_ROUTE_TUNL_IPSEC));
	PRINTK("     PPPoEmode: %d(%s)/%d(%s)\n", 0,
	       get_pae_pppoe_mode_type(0), 1, get_pae_pppoe_mode_type(1));
	PRINTK
	    ("     TTL/Route Mode/IPV4/6 type will be auto handled inside the proc\n");
	PRINTK("     DstPmac: bit 0 for pmac port 0, bit 1 for pmac port 1 and so on\n");
	PRINTK("     Subif(ATM bit format): ATM-QID[6:3] Mpoa_pt[2] Mpoa_mode[1:0]\n");


	PRINTK("     %s > /proc/dp/%s\n", EXT1_UP, DP_PROC_FILE_ROUTE);
	PRINTK("     %s > /proc/dp/%s\n", EXT2_DOWN, DP_PROC_FILE_ROUTE);

	goto exit;
}

int proc_gsw_route_dump(struct seq_file *seq, int pos)
{
	GSW_API_HANDLE gsw_handle;
	GSW_ROUTE_Entry_t *rt_entry;
	int ret = 0;

	rt_entry =
	    (GSW_ROUTE_Entry_t *) kmalloc(sizeof(GSW_ROUTE_Entry_t) + 1,
					  GFP_ATOMIC);
	if (!rt_entry) {
		PR_ERR("Failed to allocate %d bytes buffer\n",
		       sizeof(GSW_ROUTE_Entry_t) + 1);
		return -1;
	}
	/* read gswip-r rmon counter */
	gsw_handle = gsw_api_kopen((char *)GSWIP_R_DEV_NAME);
	if (gsw_handle == 0) {
		PR_ERR("Open GSWIP-R device FAILED !\n");
		kfree(rt_entry);
		return -1;
	}
	memset(rt_entry, 0, sizeof(*rt_entry));
	rt_entry->nRtIndex = pos;
	ret =
	    gsw_api_kioctl(gsw_handle, GSW_ROUTE_ENTRY_READ,
			   (unsigned int)rt_entry);
	if (ret != GSW_statusOk) {
		PR_ERR("GSW_ROUTE_ENTRY_READ returned Failure for index=%d\n",
			rt_entry->nRtIndex);
		pos = -1;
		kfree(rt_entry);
		return pos;
	}
	if (rt_entry->routeEntry.pattern.bValid != LTQ_TRUE)
		goto EXIT;

	if (dp_route_dump(seq, rt_entry))
		return pos;	/*need report*/

 EXIT:
	pos++;
	kfree(rt_entry);
	if (pos >= 4096)	/*GSWIP API does not check the maximum entry and it will hang */
		pos = -1;
	return pos;
}

#define PMAC_EG_SET(x, y) (pmac.eg.x = dp_atoi(y))
#define PMAC_IG_SET(x, y) (pmac.ig.x = dp_atoi(y))

ssize_t proc_gsw_pmac_write(struct file *file, const char *buf,
			     size_t count, loff_t *ppos)
{
	u16 len, i, k, start_param;
	int ret = 0;
	char *str = NULL;
	char *param_list[20 * 2];
	unsigned int num;
	union {
		GSW_PMAC_Eg_Cfg_t eg;
		GSW_PMAC_Ig_Cfg_t ig;
	} pmac;
	GSW_API_HANDLE gsw_handle;
	int class_s = 0, class_e = 0;
	int flow_s = 0, flow_e = 0;
	char *gsw_name;

	str = kmalloc(count + 1, GFP_ATOMIC);
	if (!str) {
		PR_ERR("Failed to allocate %d bytes buffer\n", count);
		return count;
	}
	len = count;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;

	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));
	if (num < 2) {
		PRINTK("parameter %d not enough. count=%d\n", num, count);
		goto help;
	}
	if (dp_strcmpi(param_list[0], "help") == 0)	/* help */
		goto help;
	/* set pmac */
	if (dp_strcmpi(param_list[0], "set") != 0) {
		PR_ERR("wrong command: %s\n", param_list[0]);
		goto help;
	}
	if (dp_strcmpi(param_list[1], "L") == 0)
		gsw_name = GSWIP_L_DEV_NAME;
	else if (dp_strcmpi(param_list[1], "R") == 0)
		gsw_name = GSWIP_R_DEV_NAME;
	else {
		PR_ERR("wrong switch: %s. It should be L or R only\n", param_list[0]);
		goto exit;
	}
	gsw_handle = gsw_api_kopen(gsw_name);
	if (gsw_handle == 0) {
		PR_ERR("Open GSWIP-R device FAILED !\n");
		goto exit;
	}
	memset(&pmac, 0, sizeof(pmac));
	start_param = 3;

	if (dp_strcmpi(param_list[start_param - 1], "EG") == 0) { /*ingress pmac */
		ret = gsw_api_kioctl(gsw_handle,
			GSW_PMAC_EG_CFG_GET, (unsigned int)&pmac);
		for (i = start_param; i < num; i += 2) {
			if (dp_strcmpi(param_list[i], "Class") == 0) {
				char *p = param_list[i+1];
				char *tail = p + strlen(p);
				char *tmp;
				tmp = strstr(p, ":");
				if (!tmp || (tmp >= tail)) {
					PRINTK("Wrong format for Class, it should be like xx:xx\n");
					goto exit;
				}
				*tmp = 0;
				class_s = dp_atoi(p);
				class_e = dp_atoi(tmp + 1);
			} else if (dp_strcmpi(param_list[i], "FlowID") == 0) {
				char *p = param_list[i+1];
				char *tail = p + strlen(p);
				char *tmp;
				tmp = strstr(p, ":");
				if (!tmp || (tmp >= tail)) {
					PRINTK("Wrong format for FlowID, it should be like xx:xx\n");
					goto exit;
				}
				*tmp = 0;
				flow_s = dp_atoi(p);
				flow_e = dp_atoi(tmp + 1);
			} else if (dp_strcmpi(param_list[i], "DestPort") == 0)
				PMAC_EG_SET(nDestPortId, param_list[i + 1]);
			else if (dp_strcmpi(param_list[i], "RxDmaCH") == 0)
				PMAC_EG_SET(nRxDmaChanId, param_list[i + 1]);
			/*
			else if (dp_strcmpi(param_list[i], "MPE1") == 0)
				PMAC_EG_SET(bMpe1Flag, param_list[i + 1]);
			else if (dp_strcmpi(param_list[i], "MPE2") == 0)
				PMAC_EG_SET(bMpe2Flag, param_list[i + 1]);
			else if (dp_strcmpi(param_list[i], "DEC") == 0)
				PMAC_EG_SET(bDecFlag, param_list[i + 1]);
			else if (dp_strcmpi(param_list[i], "ENC") == 0)
				PMAC_EG_SET(bEncFlag, param_list[i + 1]); */
			/*else if (dp_strcmpi(param_list[i], "ProcFlag") == 0)
				PMAC_EG_SET(bProcFlagsSelect, param_list[i + 1]);*/ /*global flag*/
			else if (dp_strcmpi(param_list[i], "RemL2Hdr") == 0)
				PMAC_EG_SET(bRemL2Hdr, param_list[i + 1]);
			else if (dp_strcmpi(param_list[i], "RemNum") == 0)
				PMAC_EG_SET(numBytesRem, param_list[i + 1]);
			else if (dp_strcmpi(param_list[i], "FCS") == 0)
				PMAC_EG_SET(bFcsEna, param_list[i + 1]);
			else if (dp_strcmpi(param_list[i], "PmacEna") == 0)
				PMAC_EG_SET(bPmacEna, param_list[i + 1]);
			else if (dp_strcmpi(param_list[i], "TcEnable") == 0)
				PMAC_EG_SET(bTCEnable, param_list[i + 1]);
			else {
				PRINTK("wrong parameter[%d]: %s\n", i, param_list[i]);
				goto exit;
			}
		}
		for (i = class_s; i < class_e; i++) {
			for (k = flow_s; k < flow_e; k++) {
				pmac.eg.nTrafficClass = i;
				/*Note: bProcFlagsSelect zero, just nTrafficClass,
				else use MPE1/2/ENC/DEC flag instead */
				pmac.eg.bMpe1Flag = (pmac.eg.nTrafficClass >> 0) & 1;
				pmac.eg.bMpe2Flag = (pmac.eg.nTrafficClass >> 1) & 1;
				pmac.eg.bEncFlag = (pmac.eg.nTrafficClass >> 2) & 1;
				pmac.eg.bDecFlag = (pmac.eg.nTrafficClass >> 3) & 1;
				pmac.eg.nFlowIDMsb = k;
				ret = gsw_api_kioctl(gsw_handle,
					GSW_PMAC_EG_CFG_SET, (unsigned int)&pmac);
			}
		}
		if (ret < GSW_statusOk) {
			PR_ERR("GSW_PMAC_EG_CFG_SET returned failure\n");
			goto exit;
		}
	} else if (dp_strcmpi(param_list[start_param - 1], "IG") == 0) { /*ingress pmac1 */
		ret = gsw_api_kioctl(gsw_handle,
			GSW_PMAC_IG_CFG_GET, (unsigned int)&pmac);
		for (i = start_param; i < num; i += 2) {
			if (dp_strcmpi(param_list[i], "TxDmaCH") == 0)
				PMAC_IG_SET(nTxDmaChanId, param_list[i + 1]);
			else if (dp_strcmpi(param_list[i], "ErrDrop") == 0)
				PMAC_IG_SET(bErrPktsDisc, param_list[i + 1]);
			else if (dp_strcmpi(param_list[i], "ClassEna") == 0) /*class from*/
				PMAC_IG_SET(bClassEna, param_list[i + 1]);
			else if (dp_strcmpi(param_list[i], "ClassDefault") == 0)
				PMAC_IG_SET(bClassDefault, param_list[i + 1]);
			else if (dp_strcmpi(param_list[i], "PmacEna") == 0) /*pmac from*/
				PMAC_IG_SET(bPmapEna, param_list[i + 1]);
			else if (dp_strcmpi(param_list[i], "PmacDefault") == 0)
				PMAC_IG_SET(bPmapDefault, param_list[i + 1]);
			else if (dp_strcmpi(param_list[i], "SubIdDefault") == 0)
				PMAC_IG_SET(bSubIdDefault, param_list[i + 1]);
			else if (dp_strcmpi(param_list[i], "SpIdDefault") == 0)
				PMAC_IG_SET(bSpIdDefault, param_list[i + 1]);
			else if (dp_strcmpi(param_list[i], "PmacPresent") == 0)
				PMAC_IG_SET(bPmacPresent, param_list[i + 1]);
			else if (dp_strcmpi(param_list[i], "DefaultPmacHdr") == 0) {
				char *p = param_list[i+1];
				char *tail = p + strlen(p);
				char *tmp;
				for (k = 0; k < 8; k++) {
					if (k < (8 - 1)) {
						tmp = strstr(p, ":");
						if (!tmp || ((u32)tmp >= (u32)tail)) {
							PRINTK("Wrong format for DefaultPmacHdr, it should be like xx:xx:xx:xx:xx:xx:xx:xx\n");
							goto exit;
						}
						*tmp = 0; /*replace : with zero */
					}
					pmac.ig.defPmacHdr[k] = dp_atoi(p);

					p = tmp + 1; /* move to next value */
				}
			} else {
				PRINTK("wrong parameter[%d]: %s\n", i, param_list[i]);
				goto exit;
			}
		}
		ret = gsw_api_kioctl(gsw_handle,
			GSW_PMAC_IG_CFG_SET, (unsigned int)&pmac);
		if (ret < GSW_statusOk) {
			PR_ERR("GSW_PMAC_IG_CFG_SET returned failure\n");
			goto exit;
		}
	} else {
		PRINTK("wrong parameter not supported: %s\n", param_list[start_param - 1]);
		goto exit;
	}
exit:
	kfree(str);
	return count;

 help:
	PRINTK("usage:\n");
	PRINTK("  echo set <L/R> EG\n");
	PRINTK("    [DestPort] [Dst-PMAC-Port-value]\n");
	PRINTK("    [Class] [Class-start:end](0~15)\n");
	PRINTK("    [FlowID] [FlowID-start:end](0~3)\n");
	PRINTK("\n");
	PRINTK("    [PmacEna] [Enable PMAC HDR (1) or not(0)]\n");
	PRINTK("    [RxDmaCH] [RxDmaCH-value]\n");
	/* PRINTK("    [MPE1] [value] [MPE2] [value] [DEC] [value] [ENC] [value]\n");*/
	PRINTK("    [TcEnable] [TcEnable-value(0/1)] [FCS] [FCS-value(0/1]\n");
	PRINTK("    [RemL2Hdr] [RemL2Hdr-value(0/1)] [RemNum] [RemNum-value]\n");
	PRINTK("     > /prooc/dp/%s\n", DP_PROC_FILE_PMAC);
	/*
	PRINTK("Note: only if bProcFlagsSelect is 1, then MPE1/2/DEC/ENC will be used.\n");
	PRINTK("      Otherwise it use Class only (Defautl setting)\n");*/

	PRINTK("  echo set IG [TxDmaCH] [TX-DMA-CH-value]\n");
	PRINTK("\n");
	PRINTK("    [ErrDrop] [Error-Drop-value(0/1)]\n");
	PRINTK("    [ClassEna] [Class Enable info from default PMAC header(1) or incoming PMAC header(0)]\n");
	PRINTK("    [ClassDefault] [Class Default info from default PMAC header(1) or incoming PMAC header(0)]\n");
	PRINTK("    [PmacEna] [Port Map Enable info from default PMAC header(1) or incoming PMAC header(0)]\n");
	PRINTK("    [PmacDefault] [Port Map info from default PMAC header(1) or incoming PMAC header(0)]\n");
	PRINTK("    [SubIdDefault] [Sub_Interface Id Info from default PMAC header(1) or in packet descriptor (0)]\n");
	PRINTK("    [SpIdDefault] [Source port id from default PMAC header(1) or incoming PMAC header (False)]\n");
	PRINTK("    [PmacPresent] [Packet PMAC header is present (1) or not (0)]\n");
	PRINTK("    [DefaultPmacHdr] [Default PMAC HDR(8 bytes: xx:xx:xx:xx:xx:xx:xx:xx]\n");
	PRINTK("     > /prooc/dp/%s\n", DP_PROC_FILE_PMAC);
	PRINTK("  ext1: echo set R IG TxDmaCH 1 ErrDrop 0 PmacDefault 0 PmacEna 0 ClassEna 1 ClassDefault 1 SubIdDefault 1 SpIdDefault 1 PmacPresent 0  DefaultPmacHdr 0x11:0x22:0x33:0x44:0x55:0x66:0x77:0x88 > /proc/dp/pmac\n");
	PRINTK("  ext2: echo set R EG DestPort 15 class 0:15 FlowID 0:3 RxDmaCH 4 TcEnable 1 RemL2Hdr 1 RemNum 8 PmacEna 0 FCS 1 > /proc/dp/pmac\n");
	goto exit;
}

ssize_t proc_dt_write(struct file *file, const char *buf,
			     size_t count, loff_t *ppos)
{
	u16 len;
	char str[64];
	char *param_list[20 * 2];
	unsigned int num;
	struct device_node *node;

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));
	if (!param_list[0] || (strlen(param_list[0]) <= 1))
		param_list[0] = "/";
	node = of_find_node_by_path(param_list[0]);
	if (!node)
		node = of_find_node_by_name(NULL, param_list[0]);
	if (!node) {
		PRINTK("Cannot find node for %s\n", param_list[0]);
		return count;
	}
	print_device_tree_node(node, 0);
	return count;
}

struct property_info {
	char *name;
	int type;
};

enum PROPERTY_TYPE{
	PROP_UNKNOWN = 0,
	PROP_STRING,
	PROP_REG,
	PROP_RANGER,
	PROP_U32_OCT,
	PROP_U32_HEX,
	PROP_HANDLE,
	PROP_REFERENCE
};

struct property_info prop_info[] = {
	{"compatible", PROP_STRING},
	{"status", PROP_STRING},
	{"name", PROP_STRING},
	{"label", PROP_STRING},
	{"model", PROP_STRING},
	{"reg-names", PROP_STRING},
	{"reg", PROP_REG},
	{"interrupts", PROP_U32_OCT},
	{"ranges", PROP_RANGER},
	{"dma-ranges", PROP_RANGER},
	{"phandle", PROP_HANDLE},
	{"interrupt-parent", PROP_HANDLE}

};

int get_property_info(char *name)
{
	int i;

	if (!name)
		return PROP_UNKNOWN;
	for (i = 0; i < ARRAY_SIZE(prop_info); i++) {
		if (dp_strcmpi(name, prop_info[i].name) == 0)
			return prop_info[i].type;
	}

	return PROP_UNKNOWN;
}

/*0--not string
  1--is string
*/
/*#define LOCAL_STRING_PARSE*/
int is_print_string(char *p, int len)
{
	int i;

	if (!p || !len)
		return 0;
	if (p[len-1] != 0)
		return 0;
	for (i = 0; i < len - 1; i++) {
#ifdef LOCAL_STRING_PARSE
		if (!(((p[i] >= 'a') && (p[i] <= 'z')) ||
			((p[i] >= 'A') && (p[i] <= 'Z')) ||
			((p[i] >= '0') && (p[i] <= '9')) ||
			(p[i] == '.') ||
			(p[i] == '/')))
#else
		if (!isprint(p[i]) && p[i] != 0) /*string list */
#endif
			return 0;
	}
	if (p[0] == 0)
		return 0;

	return 1;
}

#define INDENT_BASE 3 /*3 Space */
void print_property(struct device_node *node, struct property *p, char *indent)
{
	int type;
	int k, times, i;

	if (!p || !node)
		return;
	type = get_property_info(p->name);
	if (type == PROP_UNKNOWN) {
		if (is_print_string(p->value, p->length))
			/*PRINTK("%s  %s=%s (length=%d)\n", indent, p->name,
				(char *)p->value, p->length);*/
			type = PROP_STRING;
		else if ((p->length % 4) == 0)
			type = PROP_U32_OCT;
	}
	if (type == PROP_STRING) {
		char *s = (char *)p->value;
		int k = 0;
		PRINTK("%s  %s=", indent, p->name);
		do {
			PRINTK("\"%s\"", s);
			k += strlen(s) + 1;
			if (k < p->length) {
				s += strlen(s) + 1;
				PRINTK(",");
				continue;
			}
			PRINTK("\n");
			break;
		} while (1);
	} else if (type == PROP_U32_OCT) { /*each item is 4 bytes*/
		PRINTK("%s  %s=<", indent, p->name);
		times = p->length / 4;
		if (times) {
			for (k = 0; k < times; k++)
				PRINTK("%d ", *(int *)(p->value + k * 4));
		}
		PRINTK(">\n");
	} else if (type == PROP_U32_HEX) { /*each item is 4 bytes*/
		PRINTK("%s  %s=<", indent, p->name);
		times = p->length / 4;
		if (times) {
			for (k = 0; k < times; k++)
				PRINTK("0x%x ", *(int *)(p->value + k * 4));
		}
		PRINTK(">\n");
	} else if (type == PROP_REG) {/*two tuple: address and size */
		int n = (of_n_addr_cells(node) + of_n_size_cells(node));
		int j;
		PRINTK("%s  %s=<", indent, p->name);
		times = p->length / (4 * n);
		if (times) {
			for (k = 0; k < times; k++) {
				if (k)
					PRINTK("%s    ", indent);
				for (j = 0; j < n; j++)
					PRINTK("0x%x ",
					  *(int *)(p->value + k * 8 + 4 * j));
				if (k != (times - 1))
					PRINTK("\n");
			}
		}
		PRINTK(">\n");
	} else if (type == PROP_RANGER) {
		/*triple: child-bus-address, parent-bus-address, length */
		PRINTK("%s  %s=<", indent, p->name);
		times = p->length / (4 * 3);
		if (times) {
			for (k = 0; k < times; k++) {
				if (!k)
					PRINTK("0x%x 0x%x 0x%x",
						*(int *)(p->value + k * 8),
						*(int *)(p->value + k * 8 + 4),
						*(int *)(p->value + k * 8 + 8));
				else
					PRINTK("%s    0x%x 0x%x 0x%x", indent,
						*(int *)(p->value + k * 8),
						*(int *)(p->value + k * 8 + 4),
						*(int *)(p->value + k * 8 + 8));
				if (k != (times - 1))
					PRINTK("\n");
			}
		}
		PRINTK(">\n");
	} else if (type == PROP_HANDLE) {
		struct device_node *tmp = of_find_node_by_phandle(be32_to_cpup((u32 *)p->value));
		int offset = 0;
		if (tmp) {
			PRINTK("%s  %s=<&%s", indent, p->name, tmp->name);
			offset = 1;
		} else
			PRINTK("%s  %s=<", indent, p->name);
		if (p->length >= 4) {
			int times = p->length / 4;
			if (times) {
				for (k = offset; k < times; k++)
					PRINTK("%d ",
						*(int *)(p->value + k * 4));
			}
		}
		PRINTK(">\n");
	} else {
		PRINTK("%s  %s length=%d\n", indent, p->name, p->length);
		if (p->length) {
			char *s = (unsigned char *) p->value;
			PRINTK("%s   ", indent);
			for (i = 0; i < p->length; i++)
				PRINTK("0x%02x ", s[i]);
			PRINTK("\n");
		}
	}
}


void print_device_tree_node(struct device_node *node, int depth)
{
	int i = 0, len;
	struct device_node *child;
	struct property *p;
	char *indent;

	if (!node)
		return;
	len = (depth + 1) * 3;
	indent = kmalloc(len, GFP_KERNEL);
	if (!indent)
		return;
	for (i = 0; i < depth * 3; i++)
		indent[i] = ' ';
	indent[i] = '\0';
	++depth;
	PRINTK("%s{%s(%s)_cell addr/size=%d/%d\n",
		indent, node->name, node->full_name,
		of_n_addr_cells(node), of_n_size_cells(node));
	for_each_property_of_node(node, p)
		print_property(node, p, indent);
	for_each_child_of_node(node, child)
		print_device_tree_node(child, depth);

	PRINTK("%s}\n", indent);
	kfree(indent);
}