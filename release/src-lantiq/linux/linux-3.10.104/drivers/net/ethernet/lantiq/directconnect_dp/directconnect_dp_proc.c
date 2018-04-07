/*
 * ####################################
 *              Head File
 * ####################################
 */

/*
 *  Common Head File
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/rculist.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include <linux/interrupt.h>
#include <lantiq_dmax.h>
#include <linux/ltq_hwmcpy.h>
#include "directconnect_dp.h"

#define DC_DP_PROC "dc_dp"
#define DC_DP_PROC_DBG "dbg"
#define DC_DP_PROC_DEV "dev"
#define DC_DP_PROC_STA "wave500_sta"

/*
 * ####################################
 *             Declaration
 * ####################################
 */

static int proc_read_dc_dp_dbg_seq_open(struct inode *, struct file *);
static ssize_t proc_write_dc_dp_dbg(struct file *, const char __user *, size_t , loff_t *);
static void *proc_read_dc_dp_dev_seq_start(struct seq_file *seq, loff_t *ppos);
static void proc_read_dc_dp_dev_seq_stop(struct seq_file *seq, void *v);
static void *proc_read_dc_dp_dev_seq_next(struct seq_file *seq, void *v, loff_t *ppos);
static int proc_read_dc_dp_dev_seq_show(struct seq_file *seq, void *v);
static int proc_read_dc_dp_dev_seq_open(struct inode *, struct file *);
static ssize_t proc_write_dc_dp_dev(struct file *, const char __user *, size_t , loff_t *);
static int proc_read_dc_dp_wave500_sta_seq_open(struct inode *, struct file *);
static int proc_read_dc_dp_wave500_sta(struct seq_file *seq, void *v);

/*
 * ####################################
 *           Global Variable
 * ####################################
 */

#undef DP_F_ENUM_OR_STRING
#define DP_F_ENUM_OR_STRING(name,value, short_name) short_name
static int8_t *g_dc_dp_port_type_str[] = {
	DP_F_FLAG_LIST
};
#undef DP_F_ENUM_OR_STRING

#undef DP_F_ENUM_OR_STRING
#define DP_F_ENUM_OR_STRING(name,value, short_name) value
static uint32_t g_dc_dp_port_flag[] = {
	DP_F_FLAG_LIST
};
#undef DP_F_ENUM_OR_STRING

static int8_t *g_dc_dp_port_status_str[] = {
	"PORT_FREE",
	"PORT_ALLOCATED",
	"PORT_DEV_REGISTERED",
	"PORT_SUBIF_REGISTERED",
	"Invalid"
};

static int8_t *g_dc_dp_dbg_flag_str[] = {
	"dbg",			/*DC_DP_DBG_FLAG_DBG */

	"rx",			/*DC_DP_DBG_FLAG_DUMP_RX */
	"rx_data",		/*DC_DP_DBG_FLAG_DUMP_RX_DATA*/
	"rx_desc",		/*DC_DP_DBG_FLAG_DUMP_RX_DESCRIPTOR */
	"rx_pmac",		/*DC_DP_DBG_FLAG_DUMP_RX_PMAC */

	"tx",			/*DC_DP_DBG_FLAG_DUMP_TX */
	"tx_data",		/*DC_DP_DBG_FLAG_DUMP_TX_DATA */
	"tx_desc",		/*DC_DP_DBG_FLAG_DUMP_TX_DESCRIPTOR */
	"tx_pmac",		/*DC_DP_DBG_FLAG_DUMP_TX_PMAC */

	/*the last one*/
	"err"
};

static uint32_t g_dc_dp_dbg_flag_list[] = {
	DC_DP_DBG_FLAG_DBG,

	DC_DP_DBG_FLAG_DUMP_RX,
	DC_DP_DBG_FLAG_DUMP_RX_DATA,
	DC_DP_DBG_FLAG_DUMP_RX_DESCRIPTOR,
	DC_DP_DBG_FLAG_DUMP_RX_PMAC,

	DC_DP_DBG_FLAG_DUMP_TX,
	DC_DP_DBG_FLAG_DUMP_TX_DATA,
	DC_DP_DBG_FLAG_DUMP_TX_DESCRIPTOR,
	DC_DP_DBG_FLAG_DUMP_TX_PMAC,

	/*The last one*/
	DC_DP_DBG_FLAG_ERR
};

static inline int32_t _dc_dp_get_port_type_str_size(void)
{
    return ARRAY_SIZE(g_dc_dp_port_type_str);
}   

static inline int32_t _dc_dp_get_dbg_flag_str_size(void)
{   
    return ARRAY_SIZE(g_dc_dp_dbg_flag_str);
}

static inline int32_t _dc_dp_get_port_status_str_size(void)
{   
    return ARRAY_SIZE(g_dc_dp_port_status_str);
}   

static struct proc_dir_entry *g_dc_dp_proc = NULL;

static struct file_operations g_dc_dp_dbg_proc_fops = {
    .owner      = THIS_MODULE,
    .open       = proc_read_dc_dp_dbg_seq_open,
    .read       = seq_read,
    .write      = proc_write_dc_dp_dbg,
    .llseek     = seq_lseek,
    .release    = seq_release,
};

static struct seq_operations g_proc_read_dc_dp_dev_seq_ops = {
    .start      = proc_read_dc_dp_dev_seq_start,
    .next       = proc_read_dc_dp_dev_seq_next,
    .stop       = proc_read_dc_dp_dev_seq_stop,
    .show       = proc_read_dc_dp_dev_seq_show,
};

static struct file_operations g_dc_dp_dev_proc_fops = {
    .owner      = THIS_MODULE,
    .open       = proc_read_dc_dp_dev_seq_open,
    .read       = seq_read,
    .write      = proc_write_dc_dp_dev,
    .llseek     = seq_lseek,
    .release    = seq_release,
};

static struct file_operations g_dc_dp_sta_proc_fops = {
    .owner      = THIS_MODULE,
    .open       = proc_read_dc_dp_wave500_sta_seq_open,
    .read       = seq_read,
    .write      = NULL,
    .llseek     = seq_lseek,
    .release    = seq_release,
};

/*
 * ####################################
 *           Extern Variable
 * ####################################
 */
extern spinlock_t g_dc_dp_lock;
extern dc_dp_priv_dev_info_t g_priv_info[DC_DP_MAX_DEV_NUM];
extern dc_dp_priv_sta_info_t g_sta_info;

/*
 * ####################################
 *            Extern Function
 * ####################################
 */

/*
 * ####################################
 *            Local Function
 * ####################################
 */
static void proc_write_dc_dp_dbg_usage(void)
{
	pr_info("usage:\n");
	pr_info("    echo [enable|disable] dbg tx tx_data tx_desc rx rx_data rx_desc > /proc/%s/%s\n", DC_DP_PROC, DC_DP_PROC_DBG);
}

static void proc_write_dc_dp_dev_usage(void)
{
	pr_info("usage:\n");
	//pr_info("    echo show dev <idx> > /proc/%s/%s\n", DC_DP_PROC, DC_DP_PROC_DEV);
	pr_info("    echo set dev <idx> umt_period <us> > /proc/%s/%s\n", DC_DP_PROC, DC_DP_PROC_DEV);
	pr_info("    echo set dev <idx> umt <0|1> > /proc/%s/%s\n", DC_DP_PROC, DC_DP_PROC_DEV);
	pr_info("    echo set dev <idx> dma <0|1> > /proc/%s/%s\n", DC_DP_PROC, DC_DP_PROC_DEV);
}

static int proc_read_dc_dp_dbg(struct seq_file *seq, void *v)
{
	int i;

	seq_printf(seq, "g_dc_dp_dbg_flag=0x%08x\n", g_dc_dp_dbg_flag);
	seq_printf(seq, "Supported Flags =%d\n",
		   _dc_dp_get_dbg_flag_str_size());
	seq_printf(seq, "Enabled Flags(0x%0x):", g_dc_dp_dbg_flag);

	for (i = 0; i < _dc_dp_get_dbg_flag_str_size(); i++)
		if ((g_dc_dp_dbg_flag & g_dc_dp_dbg_flag_list[i]) == g_dc_dp_dbg_flag_list[i])
			seq_printf(seq, "%s ", g_dc_dp_dbg_flag_str[i]);

	seq_printf(seq, "\n");

    return 0;
}

static int proc_read_dc_dp_dbg_seq_open(struct inode *inode, struct file *file)
{
    return single_open(file, proc_read_dc_dp_dbg, NULL);
}

static ssize_t proc_write_dc_dp_dbg(struct file *file, const char __user *buf, size_t count, loff_t *data)
{
    int len;
    char str[40];
    char *p;
	int f_enable;

    len = min(count, (size_t)(sizeof(str) - 1));
    len -= copy_from_user(str, buf, len);
    while ( len && str[len - 1] <= ' ' )
        len--;
    str[len] = 0;
    for ( p = str; *p && *p <= ' '; p++, len-- );
    if ( !*p ) {
		goto help;
	}

	if (!strncmp(p, "enable", 6)) {
		f_enable = 1;
	} else if (!strncmp(p, "disable", 7)) {
		f_enable = 0;
	} else {
		goto help;
	}

#if 0
	for (i = 1; i < num; i++) {
		for (j = 0; j < _dc_dp_get_dbg_flag_str_size(); j++) {
			if (!strncmp(param_list[i],
				       g_dc_dp_dbg_flag_str[j])) {
				set_ltq_dbg_flag(g_dc_dp_dbg_flag,
						 f_enable, g_dc_dp_dbg_flag_list[j]);
				break;
			}
		}
	}
#endif
    return len; 

help:
	proc_write_dc_dp_dbg_usage();
	return count;
}

static void *proc_read_dc_dp_dev_seq_start(struct seq_file *seq, loff_t *ppos)
{
#if 1
	int32_t dev_idx = (int32_t)*ppos;

	if (dev_idx < DC_DP_MAX_DEV_NUM) {
		return &g_priv_info[dev_idx];
	} else {
		return NULL;
	}
#else
	if (*ppos == 0) {
		return ppos;
	} else {
		return NULL;
	}
#endif
}

static void proc_read_dc_dp_dev_seq_stop(struct seq_file *seq, void *v)
{
	return;
}

static void *proc_read_dc_dp_dev_seq_next(struct seq_file *seq, void *v, loff_t *ppos)
{
#if 1
	int32_t dev_idx = ++(*ppos);

	if (dev_idx < DC_DP_MAX_DEV_NUM) {
		return &g_priv_info[dev_idx];
	} else {
		return NULL;
	}
#else
	loff_t *dev_idx_p = (loff_t *)v;

	(*dev_idx_p)++;
	if (*dev_idx_p < DC_DP_MAX_DEV_NUM && *dev_idx_p >= 0) {
		return v;
	} else {
		return NULL;
	}
#endif
}

static int proc_read_dc_dp_dev_seq_show(struct seq_file *seq, void *v)
{
	int i;
#if 1
	int32_t pos = 0;
	dc_dp_priv_dev_info_t *dev_info = (dc_dp_priv_dev_info_t *)v;
#else
	int32_t pos = (int32_t)(*(loff_t *)v);
	dc_dp_priv_dev_info_t *dev_info;

	if (pos >= DC_DP_MAX_DEV_NUM) {
		return -1;
	}

	dev_info = &g_priv_info[pos];
#endif

	DC_DP_LOCK(&g_dc_dp_lock);

	if (dev_info->flags == DC_DP_DEV_FREE) {
		DC_DP_UNLOCK(&g_dc_dp_lock);
		seq_printf(seq, "%d: Not registered\n", pos);
		return 0;
	}

	seq_printf(seq,
		   "%d: module=0x%08x(name:%8s) dev_port=%02d dp_port=%02d\n",
		   pos, (uint32_t)dev_info->owner, dev_info->owner->name,
		   dev_info->dev_port, dev_info->port_id);
	seq_printf(seq,  "    status:            %s\n",
		g_dc_dp_port_status_str[dev_info->flags]);

	seq_printf(seq, "    allocate_flags:    ");
	for (i = 0; i < _dc_dp_get_port_type_str_size(); i++) {
		if (dev_info->alloc_flags & g_dc_dp_port_flag[i])
			seq_printf(seq,  "%s ", g_dc_dp_port_type_str[i]);
	}
	seq_printf(seq, "\n");

	seq_printf(seq, "    cb->rx_fn:         0x%0x\n",
		(uint32_t) dev_info->cb.rx_fn);
	seq_printf(seq, "    cb->restart_fn:    0x%0x\n",
		(uint32_t) dev_info->cb.restart_fn);
	seq_printf(seq, "    cb->stop_fn:       0x%0x\n",
		(uint32_t) dev_info->cb.stop_fn);
	seq_printf(seq, "    cb->get_staid_fn:  0x%0x\n",
		(uint32_t) dev_info->cb.get_staid_fn);

	seq_printf(seq, "    cbm_pid:           %d\n",
		   dev_info->cbm_pid);

	seq_printf(seq, "    dma_ch:            %d\n",
		   _DMA_CHANNEL(dev_info->dma_ch));

	seq_printf(seq, "    num_bufpools:      %02d\n",
		   dev_info->num_bufpools);
	for (i = 0; i < dev_info->num_bufpools; i++) {
		seq_printf(seq, "    buflist %d:\n",
		   	(i + 1));
		seq_printf(seq, "      virtual range:    0x%p-0x%p (%d KB)\n",
		   	dev_info->virt_buflist_base[i].pool,
			((uint8_t *)dev_info->virt_buflist_base[i].pool + dev_info->virt_buflist_base[i].size),
			(dev_info->virt_buflist_base[i].size >> 10));
		seq_printf(seq, "      physical range:   0x%p-0x%p (%d KB)\n",
		   	(void *)virt_to_phys(dev_info->virt_buflist_base[i].pool),
			(void *)((uint8_t *)virt_to_phys(dev_info->virt_buflist_base[i].pool) + dev_info->virt_buflist_base[i].size),
			(dev_info->virt_buflist_base[i].size >> 10));
	}

	seq_printf(seq, "    umt_id:            %d\n",
		   dev_info->umt_id);
	seq_printf(seq, "    umt_period:        %d (in micro second)\n",
		   dev_info->umt_period);
	seq_printf(seq, "    class2prio:        ");
	for (i = 0; i < 16; i++) {
		seq_printf(seq, "[%d->%d],", i, dev_info->class2prio[i]);
	}
	seq_printf(seq, "\n");
	seq_printf(seq, "    prio2wmm:          ");
	for (i = 0; i < DC_DP_MAX_WIFI_CLASS; i++) {
		seq_printf(seq, "[%d->%d],", i, dev_info->prio2wmm[i]);
	}
	seq_printf(seq, "\n");

	seq_printf(seq, "    num_subif:         %02d\n",
		   dev_info->num_subif);
	for (i = 0; i < DC_DP_MAX_SUBIF_PER_DEV; i++) {
		if (dev_info->subif_info[i].flags) {
			seq_printf(seq,
				   "      [%02d]: subif=0x%04x(vap=%d) netif=0x%0x(name=%s), device_name=%s\n",
				   i, dev_info->subif_info[i].subif,
				   (dev_info->
				    subif_info[i].subif >> DC_DP_VAPID_OFFSET)
				   & DC_DP_VAPID_MASK, (uint32_t)dev_info->subif_info[i].netif,
				   dev_info->subif_info[i].netif ? dev_info->subif_info[i].netif->name : "NULL",
				   dev_info->subif_info[i].device_name);
		}
	}

	DC_DP_UNLOCK(&g_dc_dp_lock);

	return 0;
}

static int proc_read_dc_dp_dev_seq_open(struct inode *inode, struct file *file)
{
    return seq_open(file, &g_proc_read_dc_dp_dev_seq_ops);
}

static __always_inline char *parse_token(char **str, char *delim)
{
	*str = strim(*str);
	return strsep(str, delim);
}

static uint32_t btoi(char *str)
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

static int32_t dc_dp_atoi(uint8_t *str)
{
	uint32_t n = 0;
	int32_t i = 0;
	int32_t nega_sign = 0;

	if (!str)
		return 0;
	//dp_replace_ch(str, strlen(str), '.', 0);
	//dp_replace_ch(str, strlen(str), ' ', 0);
	//dp_replace_ch(str, strlen(str), '\r', 0);
	//dp_replace_ch(str, strlen(str), '\n', 0);
	if (str[0] == 0)
		return 0;

	if (str[0] == 'b' || str[0] == 'B') {	/*binary format */
		n = btoi(str + 1);
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
				DC_DP_ERROR("Wrong value:%u\n", str[i]);
			
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

static ssize_t proc_write_dc_dp_dev(struct file *file, const char __user *buf, size_t count, loff_t *data)
{
    int len;
    char str[40] = {0};
    char *p, *param_name = NULL, *param_val = NULL;
	int32_t dev_idx;
	char *delim = " \t\n\v\f\r";
	int32_t value;

    len = min(count, (size_t)(sizeof(str) - 1));
    len -= copy_from_user(str, buf, len);

	p = str;

	if (!strncmp(p, "set", 3)) {
		/* set ... */
		parse_token(&p, delim); //Skip command 'set'
		if (!p) {
			goto help;
		}
		param_name = parse_token(&p, delim);
		if (!p) {
			goto help;
		}
		if (strncmp(param_name, "dev", 3)) {
			goto help;
		}
		param_val = parse_token(&p, delim);
		if (!p) {
			goto help;
		}
		dev_idx = dc_dp_atoi(param_val);
		if (dev_idx >= DC_DP_MAX_DEV_NUM) {
			goto help;
		}
		param_name = parse_token(&p, delim);
		if (!p) {
			goto help;
		}
		param_val = parse_token(&p, delim);
		value = dc_dp_atoi(param_val);

		if (!strncmp(param_name, "umt_period", strlen("umt_period"))) {
			g_priv_info[dev_idx].umt_period = value;
#if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE
			ltq_umt_set_period(g_priv_info[dev_idx].umt_id, g_priv_info[dev_idx].port_id, value);
#else /* #if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE */
			ltq_umt_set_period(value);
#endif /* #else */

		} else if (!strncmp(param_name, "umt", 3)) {
			if (value == 0) {
#if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE
				ltq_umt_enable(g_priv_info[dev_idx].umt_id, g_priv_info[dev_idx].port_id, 0);
#else /* #if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE */
				ltq_umt_enable(0);
#endif /* #else */

			} else {
#if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE
				ltq_umt_enable(g_priv_info[dev_idx].umt_id, g_priv_info[dev_idx].port_id, 1);
#else /* #if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE */
				ltq_umt_enable(1);
#endif /* #else */
			}

		} else if (!strncmp(param_name, "dma", 3)) {
			if (value == 0) {
				ltq_dma_chan_off(g_priv_info[dev_idx].dma_ch);
			} else {
				ltq_dma_chan_on(g_priv_info[dev_idx].dma_ch);
			}
		} else {
			goto help;
		}
	} else {
		goto help;
	}

    return len; 

help:
	proc_write_dc_dp_dev_usage();
    return count;
}

static int proc_read_dc_dp_wave500_sta(struct seq_file *seq, void *v)
{
	int32_t i;

	seq_printf(seq, "mac addr             sta id    iface\n");

	spin_lock_bh(&g_sta_info.sta_hash_lock);
	for (i = 0; i < STA_HASH_SIZE; i++) {
		dc_dp_priv_sta_entry_t *sta_entry;

		hlist_for_each_entry_rcu(sta_entry, &g_sta_info.sta_hash[i], hlist) {
			seq_printf(seq, "%pM    %d        %s\n",
				sta_entry->mac_addr, sta_entry->sta_id,
				((sta_entry->netif) ? sta_entry->netif->name : NULL));
		}
	}
	spin_unlock_bh(&g_sta_info.sta_hash_lock);

    return 0;
}

static int proc_read_dc_dp_wave500_sta_seq_open(struct inode *inode, struct file *file)
{
    return single_open(file, proc_read_dc_dp_wave500_sta, NULL);
}

int32_t dc_dp_proc_init(void)
{
    struct proc_dir_entry *entry;
    
	g_dc_dp_proc = proc_mkdir(DC_DP_PROC, NULL);
	if (!g_dc_dp_proc)
		return -ENOMEM;

    entry = proc_create_data(DC_DP_PROC_DBG, 0, g_dc_dp_proc,
			&g_dc_dp_dbg_proc_fops, NULL);
	if (!entry)
		goto __dbg_proc_err;

    entry = proc_create_data(DC_DP_PROC_DEV, 0, g_dc_dp_proc,
            &g_dc_dp_dev_proc_fops, NULL);
	if (!entry)
		goto __dev_proc_err;

    entry = proc_create_data(DC_DP_PROC_STA, 0, g_dc_dp_proc,
            &g_dc_dp_sta_proc_fops, NULL);
	if (!entry)
		goto __sta_proc_err;

	return 0;

__sta_proc_err:
	remove_proc_entry(DC_DP_PROC_DEV, g_dc_dp_proc);

__dev_proc_err:
	remove_proc_entry(DC_DP_PROC_DBG, g_dc_dp_proc);

__dbg_proc_err:
	proc_remove(g_dc_dp_proc);
	return -ENOMEM;
}

void dc_dp_proc_exit(void)
{
	remove_proc_entry(DC_DP_PROC_STA, g_dc_dp_proc);
	remove_proc_entry(DC_DP_PROC_DEV, g_dc_dp_proc);
	remove_proc_entry(DC_DP_PROC_DBG, g_dc_dp_proc);
	proc_remove(g_dc_dp_proc);
	g_dc_dp_proc = NULL;
}
