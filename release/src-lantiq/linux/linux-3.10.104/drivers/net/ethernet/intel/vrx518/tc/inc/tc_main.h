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
#ifndef __TC_MAIN_H__
#define __TC_MAIN_H__

#include <net/dc_ep.h>
#include <net/dsl_tc.h>

#define EP_MAX_NUM	(DC_EP_MAX_PEER + 1)
#define TCPRIV_ALIGN	32
#define DMA_PACKET_SZ	2048
#define PMAC_SIZE		8
enum {
	MSG_RX		= BIT(0),
	MSG_TX		= BIT(1),
	MSG_EVENT	= BIT(2),
	MSG_RXDATA	= BIT(3),
	MSG_TXDATA	= BIT(4),
	MSG_INIT	= BIT(5),
	MSG_OAM_RX	= BIT(6),
	MSG_OAM_TX	= BIT(7),
	MSG_OAM_RXDATA	= BIT(8),
	MSG_OAM_TXDATA	= BIT(9),
	MSG_QOS		= BIT(10),
	MSG_SWITCH	= BIT(11),
	MSG_LOOPBACK	= BIT(12),
	MSG_COC		= BIT(13),
	MSG_MIB		= BIT(14),
};

#define tc_level(level, priv, type, fmt, args...)	\
do {							\
	if (priv->msg_enable & (type))			\
		pr_##level##_ratelimited(fmt, ##args);	\
} while (0)

#define tc_dbg(priv, type, fmt, args...)		\
	tc_level(debug, priv, type, fmt, ##args)
#define tc_err(priv, type, fmt, args...)		\
	tc_level(err, priv, type, fmt, ##args)
#define tc_info(priv, type, fmt, args...)		\
	tc_level(info, priv, type, fmt, ##args)

#define TC_DEF_DBG	(MSG_INIT | MSG_SWITCH | MSG_EVENT | MSG_LOOPBACK)
#define DUMP_HDR_SZ	32

enum tc_dir {
	US_DIR = 0,
	DS_DIR = 1,
};

enum tc_status {
	TC_RUN = 0,	/* TC loaded succesfully */
	TC_INIT,	/* in Initialization */
	NO_TC,		/* Init done, but NO TC loaded */
	TC_SWITCHING,	/* TC is swiching */
	TC_EXIT,	/* module is going to be unloadeds */
	TC_ERR,
};

enum dsl_tc_mode {
	TC_ATM_SL_MODE = 0, /* ATM Single line mode */
	TC_PTM_SL_MODE = 1, /* PTM Single line mode */
	TC_PTM_BND_MODE = 2, /* PTM Bonding mode */
	TC_NONE_MODE,
};

enum tc_pkt_type {
	ATM_SL_PKT = 0,
	PTM_SL_PKT,
	PTM_BOND_PKT,
	ATM_OAM_PKT,
};

struct soc_cfg {
	u32 txin_dbase;
	u32 txin_dnum;
	u32 txout_dbase;
	u32 txout_dnum;
	u32 rxin_dbase;
	u32 rxin_dnum;
	u32 rxout_dbase;
	u32 rxout_dnum;
	u32 desc_dw_sz;
};

struct tc_param {
	unsigned int p2p:1;
	unsigned int dfe_loopback:1;
	unsigned int bonding_en:1;
	unsigned int sharing_cdma_en:1;
	unsigned int cdma_desc_loc:2;
	unsigned int ps:1;
	unsigned int res0:25;

	unsigned int umt_period;
	unsigned int qsb_tstep;
	unsigned int qsb_tau;
	unsigned int qsb_srvm;
	unsigned int aal5r_max_pktsz;
	unsigned int aal5r_min_pktsz;
	unsigned int aal5s_max_pktsz;
	unsigned int oam_prio;
};

struct cdma {
	u32 chans;
};

struct tc_hw_ops {
	/*PTM/ATM/BONDING callback functions */
	void (*recv)(struct net_device *pdev, struct sk_buff *skb);
	int (*get_qid)(struct net_device *pdev, struct sk_buff *skb,
		void *vcc, uint32_t flags);
	void (*irq_on)(u32 irq_no);
	void (*irq_off)(u32 irq_no);
	int (*showtime_enter)(const unsigned char idx,
		struct port_cell_info *port_cell, void *xdata_addr);
	int (*showtime_exit)(const unsigned char idx);
	/*int (*tc_reset)(const unsigned char idx, u32 type);*/
	int (*erb_addr_get)(const unsigned char idx,
		unsigned int *data_addr, unsigned int *desc_addr);
	int (*framer_request_en)(const unsigned char idx);

	/*SoC callback functions */
	int (*send)(struct net_device *pdev, struct sk_buff *skb,
			int qid, enum tc_pkt_type type);
	void *(*alloc)(size_t size, enum tc_dir dir);
	void (*free)(dma_addr_t phyaddr, enum tc_dir dir);
	int (*dev_reg)(struct net_device *pdev, char *dev_name,
			int *subif_id, int flag);
	void (*dev_unreg)(struct net_device *pdev, char *dev_name,
			int subif_id, int flag);
	/*umt init/exit including the corresponding DMA init/exit */
	int (*umt_init)(u32 umt_id, u32 umt_period, u32 umt_dst);
	void (*umt_exit)(u32 umt_id);
	void (*umt_start)(u32 umt_id);
	int (*soc_cfg_get)(struct soc_cfg *cfg, u32 umt_id);
	void (*coc_req)(void);
	void (*disable_us)(int en);
	int (*get_mib)(struct net_device *pdev, struct rtnl_link_stats64 *stat);
};

enum fw_id {
	FW_TX = 0,
	FW_BONDING,
	FW_RX,
	FW_ATM,
	FW_MAX
};

struct fw_info {
	__be32 fw_id;
	__be32 fw_size;
};

struct fw_ver_id {
	/* DWORD 0 */
	unsigned int family:4;
	unsigned int package:4;
	unsigned int major:8;
	unsigned int mid:8;
	unsigned int minor:8;

	/* DWORD 1 */
	unsigned int features;
} __packed;

struct fw_hdr {
	/* header information */
	struct fw_ver_id ptm_ver;
	struct fw_ver_id atm_ver;
	__be32 comp_id;
	__be32 hdr_sz;
	__be32 date;
	__be32 res[9];
	/* firmware information */
	__be32 fw_num;
	struct fw_info fw_info[FW_MAX];
};

struct fw_bin {
	const struct firmware *fw;
	struct fw_hdr fw_hdr;
	const u8 *fw_ptr[FW_MAX];
};

struct tc_priv {
	struct dc_ep_dev ep_dev[EP_MAX_NUM]; /* EP info from EP driver */
	int ep_num; /* EP num, if two, Bonding feature will be enabled */
	int showtime[EP_MAX_NUM]; /* showtime status */
	enum tc_status tc_stat;
	enum dsl_tc_mode tc_mode;
	u32 tc_idx;
	u32 msg_enable;
	struct fw_bin fw;
	struct proc_dir_entry *proc_dir;
	struct tc_hw_ops tc_ops;
	struct tc_param param;
	struct soc_cfg cfg;
	struct cdma dma;
	void *priv; /* point to ATM/PTM TC structure */
	u32 switch_exist;
	spinlock_t tc_lock;
};

/**
 * Common share struct for ATM/PTM/Bonding priv structure
 */
struct tc_comm {
	struct dc_ep_dev *ep; /* This should be always the first one */
	struct tc_priv *tc_priv;
	u32 ep_id;
};

/* p is a pointer to ATM/PTM/Bonding priv structure */
#define to_tcpriv(p)	(((struct tc_comm __force *)p)->tc_priv)
#define to_epdev(p)	(((struct tc_comm __force *)p)->ep)


extern void ptm_tc_unload(enum dsl_tc_mode);
extern void ptm_tc_load(struct tc_priv *, u32, enum dsl_tc_mode);
extern void ptm_exit(void);
extern void tc_request(u32, enum dsl_tc_mode);
extern void tc_unload(struct tc_priv *);
extern void tc_load(struct tc_priv *, u32, enum dsl_tc_mode);
extern void tc_get_drv_version(const char **, const char **);

extern int atm_tc_load(struct tc_priv *, u32 , enum dsl_tc_mode);
extern void atm_tc_unload(void);
extern void atm_exit(void);
#endif /* __TC_MAIN_H__ */

