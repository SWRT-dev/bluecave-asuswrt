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

/*             Supported functions           */
#define DEBUG
#define pr_fmt(fmt) KBUILD_MODNAME ":%s : " fmt, __func__

#include <linux/module.h>
#include <linux/init.h>
#include <linux/atomic.h>
#include <linux/uaccess.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/wait.h>
#include <linux/seq_file.h>
#include <linux/printk.h>
#include <linux/etherdevice.h>
#include <linux/atmdev.h>
#include <linux/atmioc.h>
#include <linux/skbuff.h>
#include <net/dsl_tc.h>
#include <net/datapath_proc_api.h>
#include <linux/atm.h>
#include <net/datapath_api.h>
#include "inc/tc_main.h"
#include "inc/reg_addr.h"
#include "inc/tc_common.h"
#include "inc/tc_api.h"
#include "inc/reg_addr.h"
#include "inc/atm_tc.h"
#include "inc/tc_proc.h"
#include "inc/fw/unified_qos_ds_base_vrx518_be.h"
#include "inc/fw/vrx518_a1plus_addr_def.h"
#include "inc/fw/vrx518_ds_be.h"
#include "inc/fw/vrx518_ppe_fw.h"
#include "inc/platform.h"
#include <net/dc_ep.h>

static void do_oam_tasklet(unsigned long);
static DECLARE_TASKLET(g_oam_tasklet, do_oam_tasklet, 0);
static struct atm_priv *g_atm_tc;
static const char *g_atm_dev_name = "atm-device-0";
/* tracking the number of ATM devices */
#define CGU_BASE	0x3000
#define CGU_CLKFSR	(CGU_BASE + 0x10)
#define MAX_DATA_LEN	(DMA_PACKET_SZ - 32)


static inline unsigned int pvc_num(void)
{
	return ATM_PVC_NUMBER;
}

/*Note: FW RXQ number is 16, TXQ number is 15 */
static inline unsigned int txq_num(void)
{
	return ATM_PRIO_Q_NUM;
}

/* QSB in VRX518 is fixed as System CLK */
static u32 get_qsb_clk(struct atm_priv *priv)
{
	u32 fpi_dvsn;
	u32 sys_clk, ppe_clk;

	fpi_dvsn = (tc_r32(CGU_CLKFSR) >> 28) & 0x03;
	priv->ep->hw_ops->clk_get(priv->ep, &sys_clk, &ppe_clk);
	sys_clk = sys_clk >> fpi_dvsn;

	return sys_clk;
}

static void set_qsb_clk(struct atm_priv *priv, u32 div)
{
	u32 clk_val;
	u32 divisor = 0;

	if (div > 8) {
		tc_dbg(priv->tc_priv, MSG_INIT,
			"QSB CLK divisor invailid: %d\n", div);
		return;
	}

	clk_val = tc_r32(CGU_CLKFSR);
	for (; div > 1; div >>= 1)
		divisor += 1;

	clk_val |= divisor << 28;
	tc_w32(clk_val, CGU_CLKFSR);
}

static void atm_free_tx_skb_vcc(struct sk_buff *skb)
{
	struct atm_vcc *vcc;

	vcc = ATM_SKB(skb)->vcc;

	if (vcc != NULL && vcc->pop != NULL)
		vcc->pop(vcc, skb);
	else
		dev_kfree_skb_any(skb);
}

/* Description:
 *    Enable/Disable HTU entries to capture OAM cell.
 *  Input:
 *    none
 *  Output:
 *    none
 */
static void validate_oam_htu_entry(struct atm_priv *priv, int en)
{
	struct htu_entry entry;
	int i;

	for (i = OAM_F4_SEG_HTU_ENTRY; i <= OAM_F5_HTU_ENTRY; i++) {
		tc_mem_read(priv, &entry, fpi_addr(HTU_ENTRY_TBL(i)),
			sizeof(entry));
		entry.vld = en;
		tc_mem_write(priv, fpi_addr(HTU_ENTRY_TBL(i)), &entry,
			sizeof(entry));
	}
	/*  idle for a while to finish running HTU search   */
	udelay(10);
}

/* Description:
 *    Remove one entry from HTU table.
 *  Input:
 *    conn --- unsigned int, connection ID
 *  Output:
 *    none
 */
static void clear_htu_entry(struct atm_priv *priv, unsigned int conn)
{
	struct htu_entry entry;

	tc_mem_read(priv, &entry,
		fpi_addr(HTU_ENTRY_TBL(conn + OAM_HTU_ENTRY_NUMBER)),
		sizeof(entry));
	entry.vld = 0;
	tc_mem_write(priv, fpi_addr(HTU_ENTRY_TBL(conn + OAM_HTU_ENTRY_NUMBER)),
		&entry,	sizeof(entry));
}

/**
 * valid- return 1
 * invalid - return 0
 */
int conn_valid(unsigned int conn)
{
	if (conn < pvc_num())
		return 1;
	else
		return 0;
}

/**
 * Description:
 *   Loop up for connection ID with virtual path ID.
 * Input:
 *   vpi --- unsigned int, virtual path ID
 * Output:
 *   total number of PVC: failed
 *   connection ID: Success
 */
unsigned int find_vpi(unsigned int vpi)
{
	unsigned int i;
	struct atm_pvc *conn = g_atm_tc->conn;
	unsigned int max_pvc = pvc_num();

	for (i = 0; i < max_pvc; i++)
		if ((g_atm_tc->pvc_tbl & (1 << i)) != 0
			&& conn[i].vcc != NULL
			&& vpi == conn[i].vcc->vpi)
			return i;

	return max_pvc;
}
/**
 *  Description:
 *    Loop up for vpi and vci from given queue id.
 *  Input:
 *    vpi --- unsigned int, virtual path ID
 *    vci --- unsigned int, virtual channel ID
 *  Output:
 *    success: 0
 *    failed: -1
 */
unsigned int find_vpivci_from_queue_id(
	unsigned int queue_id,
	unsigned int *vpi,
	unsigned int *vci)
{
	unsigned int i;
	struct atm_pvc *conn = g_atm_tc->conn;
	unsigned int max_pvc = pvc_num();

	for (i = 0; i < max_pvc; i++)
		if ((g_atm_tc->pvc_tbl & BIT(i))
			&& conn[i].vcc != NULL &&
			conn[i].sw_txq_tbl == BIT(queue_id)) {
			*vpi = conn[i].vcc->vpi;
			*vci = conn[i].vcc->vci;
			return 0;
		}
	return -1;
}
/**
 *  Description:
 *    Loop up for connection ID with virtual path ID and virtual channel ID.
 *  Input:
 *    vpi --- unsigned int, virtual path ID
 *    vci --- unsigned int, virtual channel ID
 *  Output:
 *    total number of PVC: failed
 *    connection ID: Success
 */
unsigned int find_vpivci(unsigned int vpi, unsigned int vci)
{
	unsigned int i;
	struct atm_pvc *conn = g_atm_tc->conn;
	unsigned int max_pvc = pvc_num();

	for (i = 0; i < max_pvc; i++)
		if ((g_atm_tc->pvc_tbl & BIT(i))
			&& conn[i].vcc != NULL
			&& vpi == conn[i].vcc->vpi
			&& vci == conn[i].vcc->vci)
			return i;

	return max_pvc;
}

/*  Description:
 *    Loop up for connection ID with atm_vcc structure.
 *  Input:
 *    vcc --- struct atm_vcc *, atm_vcc structure of opened connection
 *  Output:
 *    total number of PVC: failed
 *    connection ID: Success
 */
static int find_vcc(struct atm_vcc *vcc)
{
	unsigned int i;
	struct atm_priv *priv;
	struct atm_pvc *conn;
	unsigned int max_pvc = pvc_num();

	if (!vcc) {
		pr_err("%s : Invalid argument\n", __func__);
		return max_pvc;
	}

	priv = g_atm_tc;
	conn = priv->conn;

	for (i = 0; i < max_pvc; i++)
		if ((priv->pvc_tbl & (1 << i))
			&& conn[i].vcc == vcc)
			return i;

	return max_pvc;
}

static inline void qsb_param_dbg(struct atm_priv *priv,
		unsigned int qsb_clk,
		union qsb_queue_parameter_table *q_parm_tbl,
		union qsb_queue_vbr_parameter_table *q_vbr_parm_tbl)
{
	tc_dbg(priv->tc_priv, MSG_INIT,
		"qsb_clk = %lu\n", (unsigned long)qsb_clk);
	tc_dbg(priv->tc_priv, MSG_INIT,
		"qsb_queue_parameter_table.bit.tp       = %d\n",
			(int)q_parm_tbl->bit.tp);
	tc_dbg(priv->tc_priv, MSG_INIT,
		"qsb_queue_parameter_table.bit.wfqf     = %d (0x%08X)\n",
			(int)q_parm_tbl->bit.wfqf,
			(int)q_parm_tbl->bit.wfqf);
	tc_dbg(priv->tc_priv, MSG_INIT,
		"qsb_queue_parameter_table.bit.vbr      = %d\n",
			(int)q_parm_tbl->bit.vbr);
	tc_dbg(priv->tc_priv, MSG_INIT,
		"qsb_queue_parameter_table.dword        = 0x%08X\n",
			(int)q_parm_tbl->dword);
	tc_dbg(priv->tc_priv, MSG_INIT,
		"qsb_queue_vbr_parameter_table.bit.ts   = %d\n",
			(int)q_vbr_parm_tbl->bit.ts);
	tc_dbg(priv->tc_priv, MSG_INIT,
		"qsb_queue_vbr_parameter_table.bit.taus = %d\n",
			(int)q_vbr_parm_tbl->bit.taus);
	tc_dbg(priv->tc_priv, MSG_INIT,
		"qsb_queue_vbr_parameter_table.dword    = 0x%08X\n",
				(int)q_vbr_parm_tbl->dword);
}

/* Description:
 *    Setup QSB queue.
 *  Input:
 *    vcc        --- struct atm_vcc *, structure of an opened connection
 *    qos        --- struct atm_qos *, QoS parameter of the connection
 *    connection --- unsigned int, QSB queue ID, which is same as connection ID
 *  Output:
 *    none
 */
static void set_qsb(struct atm_priv *priv, struct atm_vcc *vcc,
		struct atm_qos *qos, unsigned int conn)
{
	union qsb_queue_parameter_table q_parm_tbl = { { 0 } };
	union qsb_queue_vbr_parameter_table q_vbr_parm_tbl = { { 0 } };
	unsigned int tmp, reg_val;
	unsigned int qsb_clk;
	struct tc_param *param;
	int port;
	unsigned int max_pcr;

	if (!priv || !vcc || !qos) {
		pr_err("%s : Invalid VCC/QoS/priv\n", __func__);
		return;
	}

	qsb_clk = get_qsb_clk(priv);
	param = &priv->tc_priv->param;
	/* QSB cell delay variation due to concurrency */
	param->qsb_tau   = 1;
	/*  QSB scheduler burst length */
	param->qsb_srvm  = 0x0F;
	param->qsb_tstep = 4;
	tc_dbg(priv->tc_priv, MSG_INIT, "%s\n", __func__);
	/* qsb_qos_dbg(qos); */
	conn += QSB_QUEUE_NUMBER_BASE; /* qsb qid = firmware qid + 1 */

	/* Peak Cell Rate (PCR) Limiter */
	if (qos->txtp.max_pcr == 0)
		q_parm_tbl.bit.tp = 0; /* disable PCR limiter */
	else {
		/*  peak cell rate would be slightly lower than requested
		 [maximum_rate / pcr = (qsb_clock / 8) * (time_step / 4) / pcr]
		*/
		tmp = ((qsb_clk * param->qsb_tstep) >> 5) / qos->txtp.max_pcr
			+ 1;
		/*  check if overflow takes place   */
		q_parm_tbl.bit.tp
			= tmp > QSB_TP_TS_MAX ? QSB_TP_TS_MAX : tmp;
	}

	/* A funny issue. Create two PVCs, one UBR and one UBR with max_pcr.
	* Send packets to these two PVCs at same time,
	* it triggers strange behavior.
	* In A1, RAM from 0x80000000 to 0x0x8007FFFF was corrupted
	* with fixed pattern 0x00000000 0x40000000.
	* In A4, PPE firmware keep emiting unknown cell
	* and no more response to driver.
	* To work around, create UBR always with max_pcr.
	* If user want to create UBR without max_pcr,
	* we give a default one larger than line-rate.
	*/
	if (qos->txtp.traffic_class == ATM_UBR &&
		q_parm_tbl.bit.tp == 0) {
		port = priv->conn[conn - QSB_QUEUE_NUMBER_BASE].port;
		max_pcr = priv->port[port].tx_max_cell_rate + 1000;

		tmp = ((qsb_clk * param->qsb_tstep) >> 5) / max_pcr + 1;
		if (tmp > QSB_TP_TS_MAX)
			tmp = QSB_TP_TS_MAX;
		else if (tmp < 1)
			tmp = 1;
		q_parm_tbl.bit.tp = tmp;
	}

	/* Weighted Fair Queueing Factor (WFQF) */
	switch (qos->txtp.traffic_class) {
	case ATM_CBR:
	case ATM_VBR_RT:
		/*  real time queue gets weighted fair queueing bypass  */
		q_parm_tbl.bit.wfqf = 0;
		break;
	case ATM_VBR_NRT:
	case ATM_UBR_PLUS:
		/*  WFQF calculation here is based on virtual cell rates,
		      to reduce granularity for high rates
		*/
		/*  WFQF is maximum cell rate / garenteed cell rate */
		/*  wfqf = qsb_minimum_cell_rate * QSB_WFQ_NONUBR_MAX /
			requested_minimum_peak_cell_rate
		*/
		if (qos->txtp.min_pcr == 0)
			q_parm_tbl.bit.wfqf = QSB_WFQ_NONUBR_MAX;
		else {
			tmp = QSB_GCR_MIN * QSB_WFQ_NONUBR_MAX /
				qos->txtp.min_pcr;
			if (tmp == 0)
				q_parm_tbl.bit.wfqf = 1;
			else if (tmp > QSB_WFQ_NONUBR_MAX)
				q_parm_tbl.bit.wfqf
					= QSB_WFQ_NONUBR_MAX;
			else
				q_parm_tbl.bit.wfqf = tmp;
		}
		break;

	case ATM_UBR:
	default:
		q_parm_tbl.bit.wfqf = QSB_WFQ_UBR_BYPASS;
		break;
	}

	/* Sustained Cell Rate (SCR) Leaky Bucket Shaper VBR.0/VBR.1 */
	if (qos->txtp.traffic_class == ATM_VBR_RT ||
		qos->txtp.traffic_class == ATM_VBR_NRT) {
		if (qos->txtp.scr == 0) {
			/* disable shaper */
			q_vbr_parm_tbl.bit.taus = 0;
			q_vbr_parm_tbl.bit.ts = 0;
		} else {
			/* Cell Loss Priority  (CLP) */
			if ((vcc->atm_options & ATM_ATMOPT_CLP))
				/* CLP1 */
				q_parm_tbl.bit.vbr = 1;
			else
				/* CLP0 */
				q_parm_tbl.bit.vbr = 0;
			/* Rate Shaper Parameter (TS) and
			    Burst Tolerance Parameter for SCR (tauS)
			*/
			tmp = ((qsb_clk * param->qsb_tstep) >> 5) /
					qos->txtp.scr + 1;
			q_vbr_parm_tbl.bit.ts
				= tmp > QSB_TP_TS_MAX ? QSB_TP_TS_MAX : tmp;
			tmp = (qos->txtp.mbs - 1) *
				(q_vbr_parm_tbl.bit.ts -
					q_parm_tbl.bit.tp) / 64;
			if (tmp == 0)
				q_vbr_parm_tbl.bit.taus = 1;
			else if (tmp > QSB_TAUS_MAX)
				q_vbr_parm_tbl.bit.taus
					= QSB_TAUS_MAX;
			else
				q_vbr_parm_tbl.bit.taus = tmp;
		}
	} else {
		q_vbr_parm_tbl.bit.taus = 0;
		q_vbr_parm_tbl.bit.ts = 0;
	}

	/* Queue Parameter Table (QPT) */
	tc_w32(QSB_QPT_SET_MASK, QSB_RTM);
	tc_w32(q_parm_tbl.dword, QSB_RTD);
	reg_val = QSB_RAMAC_RW_SET(QSB_RAMAC_RW_WRITE) |
			QSB_RAMAC_TSEL_SET(QSB_RAMAC_TSEL_QPT) |
			QSB_RAMAC_LH_SET(QSB_RAMAC_LH_LOW) |
			QSB_RAMAC_TESEL_SET(conn);
	tc_w32(reg_val, QSB_RAMAC);

	/* Queue VBR Paramter Table (QVPT) */
	tc_w32(QSB_QVPT_SET_MASK, QSB_RTM);
	tc_w32(q_vbr_parm_tbl.dword, QSB_RTD);
	reg_val = QSB_RAMAC_RW_SET(QSB_RAMAC_RW_WRITE) |
			QSB_RAMAC_TSEL_SET(QSB_RAMAC_TSEL_VBR) |
			QSB_RAMAC_LH_SET(QSB_RAMAC_LH_LOW) |
			QSB_RAMAC_TESEL_SET(conn);
	tc_w32(reg_val, QSB_RAMAC);

	qsb_param_dbg(priv, qsb_clk, &q_parm_tbl, &q_vbr_parm_tbl);
	tc_dbg(priv->tc_priv, MSG_INIT,
		"QSB setting for conn: %d is done\n", conn - 1);
}

static void do_oam_tasklet(unsigned long arg)
{
	unsigned long sys_flag;
	rx_descriptor_t desc;
	struct uni_cell_header *header;
	int ep_id;
	struct atm_vcc *vcc;
	desq_cfg_ctxt_t ds_oam_cfg_ctxt, local_oam_cfg_ctxt;
	dma_addr_t phy_addr;
	struct atm_priv *priv;
	u32 conn, ds_oam_dbase, oam_idx, oam_max_num;
	u32 mbox1_ier, rx_cnt, dq_cnt, *oam_dlist;

	priv = g_atm_tc;
	if (!priv) {
		pr_err("%s : Invalid argument\n", __func__);
		return;
	}
	ep_id = priv->ep_id;
	oam_dlist = priv->oam_llst.oam_des_list;
	oam_idx = priv->oam_llst.oam_idx;
	oam_max_num = priv->oam_llst.oam_num;

	tc_mem_read(priv, &ds_oam_cfg_ctxt,
		fpi_addr(__DS_OAM_DESQ_CFG_CTXT), sizeof(ds_oam_cfg_ctxt));
	ds_oam_dbase = ds_oam_cfg_ctxt.des_base_addr;
	rx_cnt = ds_oam_cfg_ctxt.enq_pkt_cnt;
	dq_cnt = ds_oam_cfg_ctxt.deq_pkt_cnt;

	while (1) {
		tc_mem_read(priv, &desc, fpi_addr(ds_oam_dbase + oam_idx * 2),
				sizeof(desc));
		if (desc.own == DS_OAM_DES_OWN) { /* desc not belong to CPU */
			/* Update idx as will exit loop */
			priv->oam_llst.oam_idx = oam_idx;
			/* probe if there's still availble oam packet */
			tc_mem_read(priv, &ds_oam_cfg_ctxt,
				fpi_addr(__DS_OAM_DESQ_CFG_CTXT),
				sizeof(ds_oam_cfg_ctxt));
			tc_mem_write(priv,
				fpi_addr(__DS_OAM_DESQ_CFG_CTXT +
					offsetof(desq_cfg_ctxt_t,
					deq_pkt_cnt) / sizeof(u32)),
					&dq_cnt, sizeof(dq_cnt));
			tc_mem_read(priv, &local_oam_cfg_ctxt,
				fpi_addr(__DS_TC_OAM_LOCAL_Q_CFG_CTXT),
				sizeof(local_oam_cfg_ctxt));
			if ((local_oam_cfg_ctxt.enq_pkt_cnt !=
					ds_oam_cfg_ctxt.enq_pkt_cnt) ||
				(ds_oam_cfg_ctxt.enq_pkt_cnt !=
					ds_oam_cfg_ctxt.deq_pkt_cnt))
				tasklet_schedule(&g_oam_tasklet);
			else {
				/* No more oam pkt, should enable irq again */
				spin_lock_irqsave(&priv->oam_lock, sys_flag);
				mbox1_ier = tc_r32(MBOX_IGU_IER(MBOX_IGU1));
				mbox_set_ier(priv, MBOX_IGU1,
					mbox1_ier | MBOX_OAM_RX);
				spin_unlock_irqrestore(&priv->oam_lock,
							sys_flag);
			}
			break;
		}

		phy_addr = dma_map_single(priv->pdev,
				(void *)oam_dlist[oam_idx],
				ATM_OAM_SIZE, DMA_FROM_DEVICE);

		dma_unmap_single(priv->pdev, phy_addr,
				ATM_OAM_SIZE, DMA_FROM_DEVICE);

		header = (struct uni_cell_header *)oam_dlist[oam_idx];

		if (header->pti == ATM_PTI_SEGF5 ||
			header->pti == ATM_PTI_E2EF5)
			conn = find_vpivci(header->vpi, header->vci);
		else if (header->vci == 0x03 || header->vci == 0x04)
			conn = find_vpi(header->vpi);
		else
			conn = -1; /* invalid */

		if (conn_valid(conn) && priv->conn[conn].vcc != NULL) {
			vcc = priv->conn[conn].vcc;
			priv->conn[conn].access_time = current_kernel_time();

			tc_dbg(priv->tc_priv, MSG_OAM_RX, "conn=%d, vpi: %d, vci:%d\n",
				conn, header->vpi, header->vci);
			/* dump_oam_cell(header, 1); */
			if (vcc->push_oam != NULL)
				vcc->push_oam(vcc, header);
			else {
				#if IS_ENABLED(CONFIG_LTQ_OAM)
					ifx_push_oam((unsigned char *)header);
				#else
					tc_dbg(priv->tc_priv, MSG_OAM_RX,
						"Receive OAM packet\n");
				#endif
			}
			priv->conn[conn].stats.oam_rx_pkts++;
			priv->conn[conn].stats.oam_rx_bytes
						+= desc.data_len;
			priv->stats.oam_rx_pkts++;
			priv->stats.oam_rx_bytes += desc.data_len;
		} else {
			priv->stats.oam_rx_errors++;
		}

		/* update desc */
		desc.c = 0;
		desc.own = DS_OAM_DES_OWN;
		desc.data_len = ATM_OAM_SIZE;
		tc_mem_write(priv, fpi_addr(ds_oam_dbase + oam_idx * 2), &desc,
				sizeof(desc));

		oam_idx = (oam_idx + 1) % oam_max_num;
		dq_cnt++;
	}
}

static void atm_umt_start(struct atm_priv *priv)
{
	priv->tc_priv->tc_ops.umt_start(0);
}

static void atm_aca_init(struct atm_priv *priv)
{
	struct aca_param param;
	struct aca_modem_param mdm;
	struct aca_cfg_param *txin;
	struct aca_cfg_param *txout;
	struct aca_cfg_param *rxout;
	struct soc_cfg *cfg;
	u32 phybase = priv->ep->phy_membase;
	u32 stop;

	memset(&param, 0, sizeof(param));
	priv->tc_priv->tc_ops.soc_cfg_get(&priv->tc_priv->cfg, 0);
	cfg = &priv->tc_priv->cfg;

	txin = &param.aca_txin;
	txin->byteswap = 1;
	txin->hd_size_in_dw = cfg->desc_dw_sz;
	txin->pd_desc_base = SB_XBAR_ADDR(__ACA_TX_IN_PD_LIST_BASE);
	txin->pd_desc_num = __ACA_TX_IN_PD_LIST_NUM;
	txin->pd_size_in_dw = DESC_DWSZ;
	txin->soc_desc_base = cfg->txin_dbase;
	txin->soc_desc_num = cfg->txin_dnum;
	txin->pp_buf_desc_num = ACA_TXIN_HD_DESC_NUM;

	tc_dbg(priv->tc_priv, MSG_INIT,
		"txin: bswp: %d, hdsz:%d, pd: dbase(0x%x), dnum(%d)\n",
		txin->byteswap, txin->hd_size_in_dw, txin->pd_desc_base,
		txin->pd_desc_num);
	tc_dbg(priv->tc_priv, MSG_INIT,
		"\t\tsz_indw(%d), soc_dbase:0x%x, soc_dnum:0x%x\n",
		txin->pd_size_in_dw, txin->soc_desc_base, txin->soc_desc_num);
	txout = &param.aca_txout;
	txout->byteswap = 1;
	txout->hd_size_in_dw = 1;
	txout->pd_desc_base = SB_XBAR_ADDR(__ACA_TX_OUT_PD_LIST_BASE);
	txout->pd_desc_num = __ACA_TX_OUT_PD_LIST_NUM;
	txout->pd_size_in_dw = DESC_DWSZ;
	txout->soc_desc_base = cfg->txout_dbase;
	txout->soc_desc_num = cfg->txout_dnum;
	txout->pp_buf_desc_num = ACA_TXOUT_HD_DESC_NUM;

	tc_dbg(priv->tc_priv, MSG_INIT,
		"txout: bswp: %d, hdsz:%d, pd: dbase(0x%x), dnum(%d)\n",
		txout->byteswap, txout->hd_size_in_dw, txout->pd_desc_base,
		txout->pd_desc_num);
	tc_dbg(priv->tc_priv, MSG_INIT,
		"\tsz_indw(%d), soc_dbase:0x%x, soc_dnum:0x%x\n",
		txout->pd_size_in_dw, txout->soc_desc_base,
		txout->soc_desc_num);
	rxout = &param.aca_rxout;
	rxout->byteswap = 1;
	rxout->hd_size_in_dw = cfg->desc_dw_sz;
	rxout->pd_desc_base = SB_XBAR_ADDR(__ACA_RX_OUT_PD_LIST_BASE);
	rxout->pd_desc_num = __ACA_RX_OUT_PD_LIST_NUM;
	rxout->pd_size_in_dw = DESC_DWSZ;
	rxout->soc_desc_base = cfg->rxout_dbase;
	rxout->soc_desc_num = cfg->rxout_dnum;
	rxout->pp_buf_desc_num = ACA_RXOUT_HD_DESC_NUM;

	tc_dbg(priv->tc_priv, MSG_INIT,
		"rxout: bswp: %d, hdsz:%d, pd: dbase(0x%x), dnum(%d)\n",
		rxout->byteswap, rxout->hd_size_in_dw, rxout->pd_desc_base,
		rxout->pd_desc_num);
	tc_dbg(priv->tc_priv, MSG_INIT,
		"\tsz_indw(%d), soc_dbase:0x%x, soc_dnum:0x%x\n",
		rxout->pd_size_in_dw,
		rxout->soc_desc_base, rxout->soc_desc_num);

	mdm.mdm_txout.stat
		= SB_XBAR_ADDR(__TX_OUT_ACA_ACCUM_STATUS) | phybase;
	mdm.mdm_txout.pd
		= SB_XBAR_ADDR(__TX_OUT_QUEUE_PD_BASE_ADDR_OFFSET) | phybase;
	mdm.mdm_txout.acc_cnt
		= SB_XBAR_ADDR(__TX_OUT_ACA_ACCUM_COUNT) | phybase;

	mdm.mdm_rxout.stat
		= SB_XBAR_ADDR(__RX_OUT_ACA_ACCUM_STATUS) | phybase;
	mdm.mdm_rxout.pd
		= SB_XBAR_ADDR(__RX_OUT_QUEUE_PD_BASE_ADDR_OFFSET) | phybase;
	mdm.mdm_rxout.acc_cnt
		= SB_XBAR_ADDR(__RX_OUT_ACA_ACCUM_COUNT) | phybase;

	mdm.mdm_rxin.stat
		= SB_XBAR_ADDR(__RX_IN_ACA_ACCUM_STATUS) | phybase;
	mdm.mdm_rxin.pd
		= SB_XBAR_ADDR(__RX_IN_QUEUE_PD_BASE_ADDR_OFFSET) | phybase;
	mdm.mdm_rxin.acc_cnt
		= SB_XBAR_ADDR(__RX_IN_ACA_ACCUM_COUNT) | phybase;

	tc_dbg(priv->tc_priv, MSG_INIT,
		"txout: (stat:0x%x, pd: 0x%x, cnt: 0x%x)\n",
		mdm.mdm_txout.stat, mdm.mdm_txout.pd, mdm.mdm_txout.acc_cnt);

	tc_dbg(priv->tc_priv, MSG_INIT,
		"rxout: (stat:0x%x, pd: 0x%x, cnt: 0x%x)\n",
		mdm.mdm_rxout.stat, mdm.mdm_rxout.pd, mdm.mdm_rxout.acc_cnt);

	tc_dbg(priv->tc_priv, MSG_INIT,
		"rxin: (stat:0x%x, pd: 0x%x, cnt: 0x%x)\n",
		mdm.mdm_rxin.stat, mdm.mdm_rxin.pd, mdm.mdm_rxin.acc_cnt);

	priv->ep->hw_ops->aca_init(priv->ep, &param, &mdm);
	stop = ACA_ALL_EN; /* ACA FW started all by default */
	priv->ep->hw_ops->aca_stop(priv->ep, &stop, 0);
	priv->ep->hw_ops->aca_start(priv->ep,
		ACA_TXOUT_EN | ACA_RXIN_EN | ACA_RXOUT_EN, 1);
}

static int print_datetime(char *buffer, const struct timespec *datetime)
{
	struct timeval tv;
	struct tm nowtm;
	char tmbuf[64];
	s64 nsec;

	if (buffer == NULL || datetime == NULL) {
		pr_err("%s : Invalid arguments\n", __func__);
		return -1;
	}
	nsec = timespec_to_ns(datetime);
	tv = ns_to_timeval(nsec);
	time_to_tm(tv.tv_sec, 0, &nowtm);
	memset(tmbuf, 0, 64);

	snprintf(tmbuf, sizeof(tmbuf), "%ld-%d-%d %d:%d:%d",
			1900 + nowtm.tm_year,
			1 + nowtm.tm_mon,
			nowtm.tm_mday,
			nowtm.tm_hour,
			nowtm.tm_min,
			nowtm.tm_sec);
	snprintf(buffer, sizeof(buffer), "%s.%06d", tmbuf, (int)tv.tv_usec);

	return 0;
}

void print_stat_mib(struct seq_file *seq, struct atm_stats *stat)
{
	seq_printf(seq, "AAL5 RX PKTs:\t\t %llu\n", stat->aal5_rx_pkts);
	seq_printf(seq, "AAL5 RX Bytes:\t\t %llu\n", stat->aal5_rx_bytes);
	seq_printf(seq, "AAL5 RX Err:\t\t %llu\n", stat->aal5_rx_errors);
	seq_printf(seq, "AAL5 RX Drop:\t\t %llu\n", stat->aal5_rx_dropped);

	seq_printf(seq, "OAM  RX PKTs:\t\t %llu\n", stat->oam_rx_pkts);
	seq_printf(seq, "OAM  RX Bytes:\t\t %llu\n", stat->oam_rx_bytes);
	seq_printf(seq, "OAM  RX Err:\t\t %llu\n", stat->oam_rx_errors);
	seq_printf(seq, "OAM  RX Drop:\t\t %llu\n", stat->oam_rx_dropped);

	seq_printf(seq, "AAL5 TX PKTs:\t\t %llu\n", stat->aal5_tx_pkts);
	seq_printf(seq, "AAL5 TX Bytes:\t\t %llu\n", stat->aal5_tx_bytes);
	seq_printf(seq, "AAL5 TX Err:\t\t %llu\n", stat->aal5_tx_errors);
	seq_printf(seq, "AAL5 TX Drop:\t\t %llu\n", stat->aal5_tx_dropped);

	seq_printf(seq, "OAM  TX PKTs:\t\t %llu\n", stat->oam_tx_pkts);
	seq_printf(seq, "OAM  TX Bytes:\t\t %llu\n", stat->oam_tx_bytes);
	seq_printf(seq, "OAM  TX Err:\t\t %llu\n", stat->oam_tx_errors);
	seq_printf(seq, "OAM  TX Drop:\t\t %llu\n\n", stat->oam_tx_dropped);
}

void print_drv_mib(struct seq_file *seq, struct atm_priv *priv)
{
	seq_puts(seq, "Driver Total MIB:\n");
	print_stat_mib(seq, &(priv->stats));
}
static int atm_tc_stats(struct atm_priv *priv,
	struct intel_tc_stats *stats
)
{
	int i, vpi, vci;
	struct intel_tc_atm_stats *atm_stats32;
	if (!priv) {
		pr_err("priv pointer is NULL!!!\n");
		return -EINVAL;
	}
	atm_stats32 = &(stats->stats.atm_tc_stats);
	stats->tc_info = TC_ATM_SL_MODE;
	atm_stats32->wrx_drophtu_cell = sb_r32(__WRX_DROPHTU_CELL);
	atm_stats32->wrx_dropdes_pdu = sb_r32(__WRX_DROPDES_PDU);
	atm_stats32->wrx_correct_pdu = sb_r32(__WRX_CORRECT_PDU);
	atm_stats32->wrx_err_pdu = sb_r32(__WRX_ERR_PDU);
	atm_stats32->wrx_dropdes_cell = sb_r32(__WRX_DROPDES_CELL);
	atm_stats32->wrx_correct_cell = sb_r32(__WRX_CORRECT_CELL);
	atm_stats32->wrx_err_cell = sb_r32(__WRX_ERR_CELL);
	atm_stats32->wrx_total_byte = sb_r32(__WRX_TOTAL_BYTE);
	atm_stats32->wtx_total_pdu = sb_r32(__WTX_TOTAL_PDU);
	atm_stats32->wtx_total_cell = sb_r32(__WTX_TOTAL_CELL);
	atm_stats32->wtx_total_byte = sb_r32(__WTX_TOTAL_BYTE);
	for (i = 0; i < txq_num(); i++) {
		atm_stats32->rx_mib[i].valid =
			priv->sw_txq_tbl & BIT(i);
		if (atm_stats32->rx_mib[i].valid)
			if (find_vpivci_from_queue_id(
				i,
				&vpi,
				&vci) == 0) {
				atm_stats32->rx_mib[i].vpi = vpi;
				atm_stats32->rx_mib[i].vci = vci;
			}

		atm_stats32->rx_mib[i].idx = i;
		atm_stats32->rx_mib[i].pdu = sb_r32(DSL_Q_RX_MIB_TBL(i));
		atm_stats32->rx_mib[i].bytes =
			sb_r32(DSL_Q_RX_MIB_TBL(i) + 1);
	}
	for (i = 0; i < txq_num(); i++) {
		atm_stats32->tx_mib[i].valid =
			priv->sw_txq_tbl & BIT(i);
		if (atm_stats32->tx_mib[i].valid)
			if (find_vpivci_from_queue_id(
				i,
				&vpi,
				&vci) == 0) {
				atm_stats32->tx_mib[i].vpi = vpi;
				atm_stats32->tx_mib[i].vci = vci;
			}
		atm_stats32->tx_mib[i].idx = i;
		atm_stats32->tx_mib[i].pdu = sb_r32(DSL_Q_TX_MIB_TBL(i));
		atm_stats32->tx_mib[i].bytes =
			sb_r32(DSL_Q_TX_MIB_TBL(i) + 1);
	}

	atm_stats32->aal5_rx_pkts = priv->stats.aal5_rx_pkts;
	atm_stats32->aal5_rx_bytes = priv->stats.aal5_rx_bytes;
	atm_stats32->aal5_rx_errors = priv->stats.aal5_rx_errors;
	atm_stats32->aal5_rx_dropped = priv->stats.aal5_rx_dropped;
	atm_stats32->oam_rx_pkts = priv->stats.oam_rx_pkts;
	atm_stats32->oam_rx_bytes = priv->stats.oam_rx_bytes;
	atm_stats32->oam_rx_errors = priv->stats.oam_rx_errors;
	atm_stats32->oam_rx_dropped = priv->stats.oam_rx_dropped;
	atm_stats32->aal5_tx_pkts = priv->stats.aal5_tx_pkts;
	atm_stats32->aal5_tx_bytes = priv->stats.aal5_tx_bytes;
	atm_stats32->aal5_tx_errors = priv->stats.aal5_tx_errors;
	atm_stats32->aal5_tx_dropped = priv->stats.aal5_tx_dropped;
	atm_stats32->oam_tx_pkts = priv->stats.oam_tx_pkts;
	atm_stats32->oam_tx_bytes = priv->stats.oam_tx_bytes;
	atm_stats32->oam_tx_errors = priv->stats.oam_tx_errors;
	atm_stats32->oam_tx_dropped = priv->stats.oam_tx_dropped;
	return 0;
}
int proc_read_atm_wanmib(struct seq_file *seq, void *v)
{
	struct intel_tc_stats stats;
	struct intel_tc_atm_stats *atm_stat;
	struct atm_priv *priv = (struct atm_priv *)seq->private;
	int i;

	if (!priv) {
		pr_err("priv pointer is NULL!!!\n");
		return -EINVAL;
	}
	atm_tc_stats(priv, &stats);
	atm_stat = &(stats.stats.atm_tc_stats);
	seq_puts(seq, "DSL WAN MIB:\n");
	seq_printf(seq, "  wrx_drophtu_cell: %u\n", atm_stat->wrx_drophtu_cell);
	seq_printf(seq, "  wrx_dropdes_pdu:  %u\n", atm_stat->wrx_dropdes_pdu);
	seq_printf(seq, "  wrx_correct_pdu:  %u\n", atm_stat->wrx_correct_pdu);
	seq_printf(seq, "  wrx_err_pdu:      %u\n", atm_stat->wrx_err_pdu);
	seq_printf(seq, "  wrx_dropdes_cell: %u\n", atm_stat->wrx_dropdes_cell);
	seq_printf(seq, "  wrx_correct_cell: %u\n", atm_stat->wrx_correct_cell);
	seq_printf(seq, "  wrx_err_cell:     %u\n", atm_stat->wrx_err_cell);
	seq_printf(seq, "  wrx_total_byte:   %u\n", atm_stat->wrx_total_byte);
	seq_printf(seq, "  wtx_total_pdu:    %u\n", atm_stat->wtx_total_pdu);
	seq_printf(seq, "  wtx_total_cell:   %u\n", atm_stat->wtx_total_cell);
	seq_printf(seq, "  wtx_total_byte:   %u\n", atm_stat->wtx_total_byte);
	seq_puts(seq, "DSL RX QUEUE MIB:\n");
	seq_puts(seq, "  idx     pdu       bytes\n");
	for (i = 0; i < txq_num(); i++)
		seq_printf(seq, "   %2d %10u %10u\n",
			atm_stat->rx_mib[i].idx,
			atm_stat->rx_mib[i].pdu,
			atm_stat->rx_mib[i].bytes);

	seq_puts(seq, "DSL TX QUEUE MIB:\n");
	seq_puts(seq, "  idx     pdu       bytes\n");

	for (i = 0; i < txq_num(); i++)
		seq_printf(seq, "   %2d %10u %10u\n",
			atm_stat->tx_mib[i].idx,
			atm_stat->tx_mib[i].pdu,
			atm_stat->tx_mib[i].bytes
		);

	print_drv_mib(seq, priv);

	return 0;
}

int proc_clear_atm_wanmib(struct atm_priv *priv)
{
	int i, ep_id;

	if (!priv) {
		pr_err("<%s>: priv pointer is NULL!!!\n", __func__);
		return -EINVAL;
	}

	ep_id = priv->ep_id;

	for (i = 0; i < sizeof(struct dsl_wan_mib_table) / 4; i++)
		sb_w32(0, DSL_WAN_MIB_TBL + i);

	for (i = 0; i < QOSQ_NUM; i++) {
		tc_memset(priv,
			fpi_addr(DSL_Q_RX_MIB_TBL(i)),
			0,
			sizeof(struct dsl_queue_mib));
		tc_memset(priv,
			fpi_addr(DSL_Q_TX_MIB_TBL(i)),
			0,
			sizeof(struct dsl_queue_mib));
	}

	spin_lock_bh(&priv->atm_lock);
	for (i = 0; i < pvc_num(); i++) {
		if (priv->pvc_tbl & BIT(i)) {
			memset(&priv->conn[i].stats, 0,
				sizeof(priv->conn[i].stats));
			memset(&priv->conn[i].prio_tx_packets, 0,
				sizeof(priv->conn[i].prio_tx_packets));
		}
		memset(&priv->stats, 0, sizeof(priv->stats));
	}
	spin_unlock_bh(&priv->atm_lock);

	return 0;
}

/*             Exported functions            */
void show_atm_pvc(struct seq_file *seq, const struct atm_pvc *pvc)
{
	char buf[64];

	seq_printf(seq, "\tNet device: %s\n", pvc->dev->name);
	print_datetime(buf, &(pvc->access_time));
	seq_printf(seq, "\tLast user cell: %s\n", buf);
	seq_printf(seq, "\tPort: %d\n", pvc->port);
	seq_printf(seq, "\tSoftware TX Queue: %u\n", pvc->sw_txq_tbl);
}

int proc_read_queue(struct seq_file *seq, void *v)
{

	static const char *mpoa_type_str[]
		= {"EoA w/o FCS", "EoA w FCS", "PPPoA", "IPoA"};

	struct wrx_queue_config_t rx;
	struct wtx_queue_config_t tx;
	char qmap_str[64];
	char qmap_flag;
	int qmap_str_len;
	int i, k;
	unsigned int bit;
	struct atm_priv *priv;

	if (!seq) {
		pr_err("%s : invalid seq data\n", __func__);
		return 0;
	}

	priv = (struct atm_priv *)seq->private;
	if (!priv) {
		pr_err("%s : invalid atm private\n", __func__);
		return 0;
	}
	seq_printf(seq, "RX Queue Config (0x%08X):\n", WRX_Q_CFG(0));

	for (i = 0; i < txq_num(); i++) {
		tc_mem_read(priv, &rx, fpi_addr(WRX_Q_CFG(i)),
				sizeof(rx));
		seq_printf(seq, "\t%d: MPoA type - %s", i,
				mpoa_type_str[rx.mpoa_type]);
		seq_puts(seq, ", MPoA mode - ");
		seq_printf(seq, "%s", rx.mpoa_mode ? "LLC" : "VC mux");
		seq_printf(seq, ", IP version %d", rx.ip_ver ? 6 : 4);
		seq_printf(seq, "\t\tOversize - %d", rx.oversize);
		seq_printf(seq, ", Undersize - %d", rx.undersize);
		seq_printf(seq, ", Max Frame size - %d\n", rx.mfs);
		seq_printf(seq, "\t\tuu mask - 0x%02X", rx.uumask);
		seq_printf(seq, ", cpi mask - 0x%02X\n", rx.cpimask);
		seq_printf(seq, "\t\tuu exp - 0x%02X", rx.uuexp);
		seq_printf(seq, ", cpi exp - 0x%02X\n", rx.cpiexp);
		if (rx.vlan_ins)
			seq_printf(seq, "     new_vlan = 0x%08X\n",
				rx.new_vlan);
	}

	seq_printf(seq, "TX Queue Config (0x%08X):\n", WTX_Q_CFG(0));

	for (i = 0; i < txq_num(); i++) {
		tc_mem_read(priv, &tx, fpi_addr(WTX_Q_CFG(i)),
				sizeof(tx));
		qmap_flag = 0;
		qmap_str_len = 0;
		for (k = 0, bit = 1; k < 15; k++, bit <<= 1)
			if (tx.same_vc_qmap & bit) {
				if (qmap_flag++)
					qmap_str_len +=
					sprintf(qmap_str + qmap_str_len,
								", ");
				qmap_str_len +=
					sprintf(qmap_str + qmap_str_len,
					"%d", k);
			}
		seq_printf(seq, "\t%d: uu - 0x%02X, cpi - 0x%02X\n", i,
			tx.uu, tx.cpi);
		seq_puts(seq, "\t\tsame VC queue map - ");
		seq_printf(seq, "%s\n", qmap_flag ? qmap_str : "null");
		seq_puts(seq, "\t\tbearer channel - ");
		seq_printf(seq, "%d, QSB ID - %d\n", tx.sbid, tx.qsb_vcid);
		seq_puts(seq, "\t\tMPoA mode - ");
		seq_printf(seq, "%s\n", tx.mpoa_mode ? "LLC" : "VC mux");
		seq_printf(seq, "\t\tATM header - 0x%08X\n", tx.atm_header);
	}

	return 0;
}

/* Main functions */
static int ppe_open(struct atm_vcc *vcc)
{
	int vpi = vcc->vpi;
	int vci = vcc->vci;
	struct atm_priv *priv = g_atm_tc;
	struct atm_port *port = &priv->port[(int)vcc->dev->phy_data];
	int sw_txq, i, ret;
	unsigned int conn;

	tc_dbg(priv->tc_priv, MSG_INIT, "%s\n", __func__);

	if (priv->tc_priv->tc_stat != TC_RUN
		|| priv->tc_priv->tc_mode != TC_ATM_SL_MODE) {
		tc_dbg(priv->tc_priv, MSG_INIT, "PPE Open fail! TC Status: %d, TC Mode: %d\n",
			(u32)priv->tc_priv->tc_stat,
			(u32)priv->tc_priv->tc_mode);
		return -EACCES;
	}

	if (vcc->qos.aal != ATM_AAL5 && vcc->qos.aal != ATM_AAL0) {
		tc_dbg(priv->tc_priv, MSG_INIT, "aal proto not supported: %d\n",
			vcc->qos.aal);
		return -EPROTONOSUPPORT;
	}

	/* check bandwidth */
	if ((vcc->qos.txtp.traffic_class == ATM_CBR &&
		vcc->qos.txtp.max_pcr >
			(port->tx_max_cell_rate - port->tx_used_cell_rate))
		|| (vcc->qos.txtp.traffic_class == ATM_VBR_RT &&
		vcc->qos.txtp.max_pcr >
			(port->tx_max_cell_rate - port->tx_used_cell_rate))
		|| (vcc->qos.txtp.traffic_class == ATM_VBR_NRT &&
		vcc->qos.txtp.scr >
			(port->tx_max_cell_rate - port->tx_used_cell_rate))
		|| (vcc->qos.txtp.traffic_class == ATM_UBR_PLUS &&
		vcc->qos.txtp.min_pcr >
			(port->tx_max_cell_rate - port->tx_used_cell_rate))) {
		tc_dbg(priv->tc_priv, MSG_INIT, "exceed TX line rate\n");
		return -EINVAL;
	}

	tc_dbg(priv->tc_priv, MSG_INIT, "PVC (%d.%d)\n", vpi, vci);

	/*  check existing vpi,vci  */
	conn = find_vpivci(vpi, vci);
	if (conn_valid(conn)) {
		tc_dbg(priv->tc_priv, MSG_INIT,
				"PVC (%d.%d) has been in used\n", vpi, vci);
		return -EADDRINUSE;
	}

	spin_lock_bh(&priv->atm_lock);
	/*  allocate software TX queue  */
	for (sw_txq = 0; sw_txq < txq_num(); sw_txq++)
		if (!(priv->sw_txq_tbl & BIT(sw_txq)))
			break;

	if (sw_txq >= txq_num()) {
		ret = -ENOSPC;
		tc_dbg(priv->tc_priv, MSG_INIT, "no free TX queue\n");
		goto OPEN_ERR;
	}

	if (!priv->pvc_tbl)
		validate_oam_htu_entry(priv, 1);

	/* allocate PVC */
	for (conn = 0; conn < pvc_num(); conn++)
		if (!(priv->pvc_tbl & BIT(conn))) {
			priv->sw_txq_tbl |= BIT(sw_txq);
			priv->conn[conn].vcc = vcc;
			priv->conn[conn].port
				= (unsigned int)vcc->dev->phy_data;
			priv->conn[conn].sw_txq_tbl = BIT(sw_txq);

			for (i = 0; i < txq_num(); i++)
				priv->conn[conn].prio_queue_map[i] = sw_txq;
			priv->pvc_tbl |= BIT(conn);
			break;
		}

	if (!conn_valid(conn)) {
		ret = -ENOSPC;
		tc_dbg(priv->tc_priv, MSG_INIT, "exceed PVC limit\n");
		goto OPEN_ERR;
	}
	spin_unlock_bh(&priv->atm_lock);

	/*  reserve bandwidth */
	switch (vcc->qos.txtp.traffic_class) {
	case ATM_CBR:
	case ATM_VBR_RT:
		port->tx_used_cell_rate += vcc->qos.txtp.max_pcr;
		break;
	case ATM_VBR_NRT:
		port->tx_used_cell_rate += vcc->qos.txtp.scr;
		break;
	case ATM_UBR_PLUS:
		port->tx_used_cell_rate += vcc->qos.txtp.min_pcr;
		break;
	}

	/* update atm_vcc structure */
	vcc->itf = (int)vcc->dev->phy_data;
	set_bit(ATM_VF_ADDR, &vcc->flags);
	set_bit(ATM_VF_READY, &vcc->flags);

	tc_dbg(priv->tc_priv, MSG_INIT,
		"ppe_open(%d.%d): conn = %d, ep_id: %d\n",
		vcc->vpi, vcc->vci, conn, priv->ep_id);

	return 0;

OPEN_ERR:
	spin_unlock_bh(&priv->atm_lock);
	return ret;
}

static void ppe_close(struct atm_vcc *vcc)
{
	u32 cid;
	struct atm_port *port;
	struct atm_pvc *conn;
	struct atm_priv *priv;
	struct net_device *dev;
	char dev_name[32];

	if (!vcc) {
		pr_err("%s: VCC ptr invalid\n", __func__);
		return;
	}

	priv = vcc->dev->dev_data;
	if (!priv) {
		pr_err("%s: priv ptr invalid\n", __func__);
		return;
	}

	if (priv->tc_priv->tc_stat != TC_RUN
		|| priv->tc_priv->tc_mode != TC_ATM_SL_MODE) {
		tc_err(priv->tc_priv, MSG_INIT, "PPE Close fail! TC Status: %d, TC Mode: %d\n",
			priv->tc_priv->tc_stat, priv->tc_priv->tc_mode);
		return;
	}

	tc_dbg(priv->tc_priv, MSG_INIT, "ppe close, vpi:%d, vci:%d\n",
		vcc->vpi, vcc->vci);

	/*  get connection id   */
	cid = find_vcc(vcc);
	if (!conn_valid(cid)) {
		tc_err(priv->tc_priv, MSG_INIT, "can't find vcc\n");
		return;
	}

	conn = &priv->conn[cid];
	port = &priv->port[conn->port];

	clear_bit(ATM_VF_READY, &vcc->flags);
	clear_bit(ATM_VF_ADDR, &vcc->flags);

	/* clear htu */
	clear_htu_entry(priv, cid);

	/*  release connection  */
	spin_lock_bh(&priv->atm_lock);
	priv->pvc_tbl &= ~(BIT(cid));
	priv->sw_txq_tbl &= ~(conn->sw_txq_tbl);
	dev = conn->dev;
	if (!dev)
		sprintf(dev_name, "atm_%d%d",
			conn->vcc->vpi, conn->vcc->vci);

	memset(conn, 0, sizeof(*conn));
	if (priv->pvc_tbl == 0)
		validate_oam_htu_entry(priv, 0);
	spin_unlock_bh(&priv->atm_lock);

	priv->tc_priv->tc_ops.dev_unreg(dev, (!dev) ? dev_name : dev->name,
					priv->conn[cid].subif_id, 0);

	/* release bandwidth */
	switch (vcc->qos.txtp.traffic_class) {
	case ATM_CBR:
	case ATM_VBR_RT:
		port->tx_used_cell_rate -= vcc->qos.txtp.max_pcr;
		break;
	case ATM_VBR_NRT:
		port->tx_used_cell_rate -= vcc->qos.txtp.scr;
		break;
	case ATM_UBR_PLUS:
		port->tx_used_cell_rate -= vcc->qos.txtp.min_pcr;
		break;
	}
	/* idle for a while to let parallel operation finish */
	udelay(100);

	return;
}

static int ppe_ioctl(struct atm_dev *dev, unsigned int cmd, void __user *arg)
{
	int ret = 0;
	struct intel_tc_stats stats;
	struct atm_priv *priv = g_atm_tc;
	switch (cmd) {
	case INTEL_ATM_TC_IOCTL_STATS:
		if (atm_tc_stats(priv, &stats) == 0)
			ret = copy_to_user(arg,
					(void *)&stats,
					sizeof(stats));
		break;
	default:
		ret = -ENOIOCTLCMD;
		break;
	}
	return ret;
}

static int ppe_send(struct atm_vcc *vcc, struct sk_buff *skb)
{
	int ret, qid, mpoa_pt, mpoa_type, vid;
	unsigned int prio, conn;
	struct atm_priv *priv;

	if (!vcc) {
		pr_err("%s: Invalid VCC PTR\n", __func__);
		return -EINVAL;
	}

	priv = vcc->dev->dev_data;
	if (!priv) {
		pr_err("%s: Invalid private data\n", __func__);
		return -EINVAL;
	}
	if (!skb || !vcc || vcc->qos.aal != ATM_AAL5)
		goto PARAM_ERR;

	ATM_SKB(skb)->vcc = vcc;
	if (priv->tc_priv->tc_stat != TC_RUN
		|| priv->tc_priv->tc_mode != TC_ATM_SL_MODE) {
		tc_dbg(priv->tc_priv, MSG_TX, "Drop packet due to tc_stat: %d, tc_mode: %d, ep_id: %d\n",
			(u32)priv->tc_priv->tc_stat,
			(u32)priv->tc_priv->tc_mode,
			priv->ep_id);
		atm_free_tx_skb_vcc(skb);
		return -EACCES;
	}

	skb_get(skb);
	atm_free_tx_skb_vcc(skb);
	ATM_SKB(skb)->vcc = NULL;

	conn = find_vcc(vcc);
	if (!conn_valid(conn)) {
		tc_dbg(priv->tc_priv, MSG_TX, "Cannot find vcc: vpi/vci: %d/%d\n",
			vcc->vpi, vcc->vci);
		ret = -EINVAL;
		goto FIND_VCC_FAIL;
	}

	if (!showtime_stat(priv->tc_priv)) {
		tc_dbg(priv->tc_priv, MSG_TX, "DSL not in showtime\n");
		ret = -EIO;
		goto CHECK_SHOWTIME_FAIL;
	}

	dump_skb_info(priv->tc_priv, skb, (MSG_TX | MSG_TXDATA));
	prio = skb->priority >= ATM_PRIO_Q_NUM ?
			ATM_PRIO_Q_NUM - 1 : skb->priority;
	mpoa_pt = 1;
	mpoa_type = 0; /* Ignore by FW if mpoa_pt set */
	qid = priv->conn[conn].prio_queue_map[prio];
	vid = priv->conn[conn].subif_id;
	qid = (vid & (~0x7f)) |
		ATM_DESC_SUBIF_ID(qid, mpoa_pt, mpoa_type);

	tc_dbg(priv->tc_priv, MSG_TX, "vid: 0x%x, qid: 0x%x\n",
			vid, qid);

	if (priv->tc_priv->tc_ops.send(priv->dev,
			skb, qid, ATM_SL_PKT) == 0) {
		priv->stats.aal5_tx_pkts++;
		priv->stats.aal5_tx_bytes += skb->len;
		priv->conn[conn].stats.aal5_tx_pkts++;
		priv->conn[conn].stats.aal5_tx_bytes += skb->len;
		priv->conn[conn].prio_tx_packets[prio]++;
	} else {
		tc_dbg(priv->tc_priv, MSG_TX, "ATM: TX fail\n");
		priv->stats.aal5_tx_dropped++;
		priv->conn[conn].stats.aal5_tx_dropped++;
	}

	if (vcc->stats)
		atomic_inc(&vcc->stats->tx);

	return 0;

PARAM_ERR:
	if (!skb) {
		tc_err(priv->tc_priv, MSG_TX, "SKB is NULL\n");
		return -EINVAL;
	} else if (!vcc) {
		tc_err(priv->tc_priv, MSG_TX, "VCC is NULL\n");
		return -EINVAL;
	} else if (vcc->qos.aal != ATM_AAL5) {
		tc_err(priv->tc_priv, MSG_TX, "AAL protocol is not ATM_ALL5\n");
		return -EPROTONOSUPPORT;
	}

	return -EINVAL;

FIND_VCC_FAIL:
	if (!vcc) {
		atm_free_tx_skb_vcc(skb);
		return ret;
	} else if (vcc->stats)
		atomic_inc(&vcc->stats->tx_err);

	if (vcc->qos.aal == ATM_AAL5)
		priv->stats.aal5_tx_errors++;
	atm_free_tx_skb_vcc(skb);
	return ret;

CHECK_SHOWTIME_FAIL:
	if (vcc->stats)
		atomic_inc(&vcc->stats->tx_err);
	if (vcc->qos.aal == ATM_AAL5) {
		priv->stats.aal5_tx_dropped++;
		priv->conn[conn].stats.aal5_tx_dropped++;
	}
	atm_free_tx_skb_vcc(skb);
	return ret;
}

static struct sk_buff *alloc_skb_tx(int len)
{
	struct sk_buff *skb;
	len = (len + TCPRIV_ALIGN - 1) & ~(TCPRIV_ALIGN - 1);
	skb = dev_alloc_skb(len);
	if (skb)
		ATM_SKB(skb)->vcc = NULL;
	return skb;
}

int ppe_send_oam(struct atm_vcc *vcc, void *cell, int flags)
{
	int vid, qid, mpoa_pt, mpoa_type;
	struct uni_cell_header *cell_header = (struct uni_cell_header *)cell;
	struct atm_priv *priv = g_atm_tc;
	struct sk_buff *skb;
	unsigned int conn;
	dp_subif_t dp_id;

	if (!vcc)
		return -EINVAL;

	if (!showtime_stat(priv->tc_priv)) {
		tc_dbg(priv->tc_priv, MSG_TX, "ATM not in showtime\n");
		return -EIO;
	}

	/* dump_oam_cell(cell, 0); */
	if (((cell_header->pti == ATM_PTI_SEGF5 ||
		cell_header->pti == ATM_PTI_E2EF5)
		&& !conn_valid(find_vpivci(cell_header->vpi, cell_header->vci)))
		|| ((cell_header->vci == 0x03 || cell_header->vci == 0x04)
		&& !conn_valid(find_vpi(cell_header->vpi)))) {

		tc_err(priv->tc_priv, MSG_TX, "Cell pti/vpi/vci not match: %d/%d/%d\n",
			cell_header->pti, cell_header->vpi, cell_header->vci);
		return -EINVAL;
	}

	/*  find queue ID   */
	conn = find_vcc(vcc);
	if (!conn_valid(conn)) {
		tc_err(priv->tc_priv, MSG_TX,
			"VCC is not invalid: VPI/VCI: %d/%d\n",
			vcc->vpi, vcc->vci);
		return -EINVAL;
	}
	mpoa_pt = 1;
	mpoa_type = 0; /* Ignore by FW  */
	qid = priv->conn[conn].prio_queue_map[priv->tc_priv->param.oam_prio];
	vid = priv->conn[conn].subif_id;
	dp_id.subif = (vid & (~0x7f)) |
		ATM_DESC_SUBIF_ID(qid, mpoa_pt, mpoa_type);

	/*  allocate sk_buff    */
	skb = (struct sk_buff *)alloc_skb_tx(MAX_DATA_LEN);
	if (skb == NULL) {
		tc_err(priv->tc_priv, MSG_TX,
				"%s: allocate cbm buffer fail\n", __func__);
		return -ENOMEM;
	}

	/*  copy data   */
	skb_put(skb, CELL_SIZE);
	memcpy(skb->data, cell, CELL_SIZE);
	/* SET SUBIFID */
	skb->DW0 = (skb->DW0 & ~0x7FFF) | dp_id.subif;
	skb->dev = priv->conn[conn].dev;

	tc_dbg(priv->tc_priv, MSG_TX, "conn: %d, dev name: %s, qid: 0x%x len:%d\n",
		conn, skb->dev->name, dp_id.subif, skb->len);
	if (priv->tc_priv->tc_ops.send(priv->dev,
			skb, qid, ATM_OAM_PKT) == 0) {
		priv->stats.oam_tx_pkts++;
		priv->stats.oam_tx_bytes += skb->len;
		priv->conn[conn].stats.oam_tx_pkts++;
		priv->conn[conn].stats.oam_tx_bytes += skb->len;
	} else {
		priv->stats.oam_tx_dropped++;
		priv->conn[conn].stats.oam_tx_dropped++;
	}

	return 0;
}

static int ppe_change_qos(struct atm_vcc *vcc, struct atm_qos *qos, int flags)
{
	int conn;

	if (vcc == NULL || qos == NULL)
		return -EINVAL;

	conn = find_vcc(vcc);
	if (conn < 0)
		return -EINVAL;

	set_qsb((struct atm_priv *)vcc->dev->dev_data, vcc, qos, conn);

	return 0;
}

static struct atmdev_ops g_ppe_atm_ops = {
	.owner =	THIS_MODULE,
	.open =		ppe_open,
	.close =	ppe_close,
	.ioctl =	ppe_ioctl,
	.send =		ppe_send,
	.send_oam =	ppe_send_oam,
	.change_qos =	ppe_change_qos,
};

/**
 * Disable Powersaving
 * Enable MBOX1 Interrupt (MBOX_FRAME_STOP)
 * Send MBOX5 Interrupt to wake up PPE FW
 */
static void atm_ppe_wakeup(struct atm_priv *priv)
{
	u32 mbox_ier;

	if (priv->tc_priv->param.ps) {
		atm_power_saving(priv, 0, 1);
		priv->tc_priv->param.ps = 1;
	}

	mbox_ier = tc_r32(MBOX_IGU_IER(MBOX_IGU1));
	mbox_ier |= MBOX_FRAME_STOP;
	mbox_set_ier(priv, MBOX_IGU1, mbox_ier);

	if (priv->tc_priv->param.ps)
		mbox_set_isr(priv, PPE_FW_MBOX0, MBOX_FW_WAKEUP);
}


/**
 * Restore Powersaving setting.
 */
static void atm_ppe_ps_restore(struct atm_priv *priv)
{
	u32 mbox_ier;
	mbox_ier = tc_r32(MBOX_IGU_IER(MBOX_IGU1));
	mbox_ier &= ~MBOX_FRAME_STOP;
	mbox_set_ier(priv, MBOX_IGU1, mbox_ier);

	if (priv->tc_priv->param.ps)
		atm_power_saving(priv, 1, 1);
}

/* Note, must lock oam lock if change the ier other than OAM_INT bit */
static irqreturn_t atm_irq_handler(int irq, void *dev_id)
{
	u32 mbox1_isr, mbox1_ier, mbox1_isrc;
	struct atm_priv *priv = dev_id;

	mbox1_ier = tc_r32(MBOX_IGU_IER(MBOX_IGU1));
	mbox1_isrc = tc_r32(MBOX_IGU_ISR(MBOX_IGU1));
	mbox1_isr = mbox1_isrc & mbox1_ier;

	/* Any other bit except OAM_INT_BIT, need lock oam_lock */
	if (mbox1_isr & MBOX_OAM_RX) {
		mbox1_ier &= ~MBOX_OAM_RX;
		mbox_set_ier(priv, MBOX_IGU1, mbox1_ier);
		tasklet_schedule(&g_oam_tasklet);
	}

	if (mbox1_isr & MBOX_FRAME_STOP) {
		ttha_disable(priv, 0);
		tc_dbg(priv->tc_priv, MSG_EVENT,
			"Disable TTHA\n"
		);
	}

#ifdef CONFIG_LTQ_VRX318_CPUFREQ
	if (mbox1_isr & MBOX_PKT_RX) {
		mbox1_ier &= ~MBOX_PKT_RX;
		spin_lock(&priv->oam_lock);
		mbox_set_ier(priv, MBOX_IGU1, mbox1_ier);
		spin_unlock(&priv->oam_lock);
		tasklet_schedule(&g_coc_tasklet);
	}
#endif

	/**
	* Clear TX interrupt at this moment.
	* Implement flow control mechansim if there is specific requirement.
	*/
	mbox_clr_isr(priv, MBOX_IGU1, mbox1_isrc);

	return IRQ_HANDLED;
}

static void atm_umt_init(struct atm_priv *priv)
{
	struct tc_priv *tcpriv;
	u32 dst;
	u32 period;

	tcpriv = priv->tc_priv;
	dst = priv->ep->hw_ops->umt_msg_addr(priv->ep);
	dst |= priv->ep->phy_membase;
	period = tcpriv->param.umt_period;

	tc_dbg(priv->tc_priv, MSG_INIT, "\tUMT period: %d, dst: 0x%x\n",
		period, dst);
	tcpriv->tc_ops.umt_init(0, period, dst);
}

/**
*  Load ATM firmware to corresponding PP32
**/
static void atm_fw_load(struct atm_priv *priv)
{
	struct ppe_fw *fw = &priv->fw;
	struct fw_bin *binfw = &priv->tc_priv->fw;

	memcpy(&fw->atm_ver, &binfw->fw_hdr.atm_ver, sizeof(fw->atm_ver));
	fw->atm_fw.size[PP32_TX] =
		binfw->fw_hdr.fw_info[FW_ATM].fw_size;
	fw->atm_fw.data[PP32_TX] =
		(const u32 *)binfw->fw_ptr[FW_ATM];

	tc_dbg(priv->tc_priv, MSG_INIT, "%s: Firmware size[%d] ptr[%p]\n",
			__func__,
			fw->atm_fw.size[PP32_TX],
			fw->atm_fw.data[PP32_TX]);
}

static inline void atm_pdma_init(struct atm_priv *priv)
{
	u32 reg;

	tc_w32(0x00000008, PDMA_CFG);
	tc_w32(0x00203580, SAR_PDMA_RX_CMDBUF_CFG);
	tc_w32(0x004035A0, SAR_PDMA_RX_FW_CMDBUF_CFG);

	/* Enable RX idle cell */
	reg = tc_r32(SFSM_CFG0);
	reg |= BIT(15);
	tc_w32(reg, SFSM_CFG0);
	reg = tc_r32(SFSM_CFG1);
	reg |= BIT(15);
	tc_w32(reg, SFSM_CFG1);
	return;
}

/**
* Configure ATM's Firmware
* vrx318_atm_fw_init
*/
static void atm_fw_init(struct atm_priv *priv)
{
	qos_cfg_t qos_cfg;
	struct psave_cfg ps_cfg;

	sb_w32(0x3C30, __VRX218_FLOW_CTRL_CFG);

	memset((void *)&qos_cfg, 0, sizeof(qos_cfg));
	/* PPE FW use this timer to wake up in the sleep mode.
	    It's the only way to wake up FW in the ATM mode.
	*/
	/* cgu_get_pp32_clock() / 62500 / 3 */
	qos_cfg.time_tick = 2304; /* tc_ppe_clkget(atm_priv->ep); */
	tc_mem_write(priv, fpi_addr(__QOS_CFG), &qos_cfg, sizeof(qos_cfg));
	memset((void *)&ps_cfg, 0, sizeof(ps_cfg));
	ps_cfg.start_state = 0;
	ps_cfg.sleep_en = priv->tc_priv->param.ps;
	tc_mem_write(priv, fpi_addr(__PSAVE_CFG), &ps_cfg, sizeof(ps_cfg));

	sb_w32(15 + 3, __CFG_WRX_HTUTS);
	sb_w32(0x03, __CFG_WRX_DMACH_ON);
	sb_w32(4, __CFG_WRX_HUNT_BITTH);
	sb_w32(1, __SOC_FAMILY);

	return;
}

/* vrx318_atm_wrx_queue_cfg_init */
static void atm_wrx_queue_cfg_init(struct atm_priv *priv)
{
	int i;
	struct wrx_queue_config_t wrx_queue_config;
	memset((void *)&wrx_queue_config, 0, sizeof(struct wrx_queue_config_t));

	wrx_queue_config.new_vlan  = 0;
	wrx_queue_config.vlan_ins  = 0;
	/* 0: EoA without FCS, 1: EoA with FCS, 2: PPPoA, 3:IPoA */
	wrx_queue_config.mpoa_type	= MPOA_IPOA;
	/* 0: IPv4, 1: IPv6 */
	wrx_queue_config.ip_ver		= ATM_IP_IPV4;
	/* 0: VCmux, 1: LLC */
	wrx_queue_config.mpoa_mode	= MPOA_VCMUX_MODE;
	wrx_queue_config.oversize	= MAX_DATA_LEN;
	wrx_queue_config.undersize	= 0;
	/* Max fragment size */
	wrx_queue_config.mfs		= MAX_DATA_LEN;
	wrx_queue_config.uumask		= 0xFF;
	wrx_queue_config.cpimask	= 0xFF;
	wrx_queue_config.uuexp		= 0;
	wrx_queue_config.cpiexp		= 0;

	for (i = 0; i < 2; i++)
		tc_mem_write(priv, fpi_addr(WRX_Q_CFG(i)),
			&wrx_queue_config,
			sizeof(struct wrx_queue_config_t));

	return;
}

/* vrx318_atm_wtx_queue_cfg_init */
static void atm_wtx_queue_cfg_init(struct atm_priv *priv, int qsb_en)
{
	int i;
	struct wtx_queue_config_t wtx_queue_config;

	memset((void *)&wtx_queue_config, 0, sizeof(struct wtx_queue_config_t));
	wtx_queue_config.same_vc_qmap = 0;
	wtx_queue_config.uu           = 0;
	wtx_queue_config.cpi          = 0;
	wtx_queue_config.sbid         = 0;
	/*  Which QSB queue (VCID) does this TX queue map to. */
	wtx_queue_config.qsb_vcid     = 0;
	wtx_queue_config.mpoa_mode    = 0; /*  0: VCmux, 1: LLC */
	wtx_queue_config.qsben        = qsb_en;
	wtx_queue_config.atm_header   = 0;

	for (i = 0; i < txq_num(); i++) {
		tc_mem_write(priv, fpi_addr(WTX_Q_CFG(i)),
			&wtx_queue_config,
			sizeof(struct wtx_queue_config_t)
			);
	}

	return;
}

/* vrx318_atm_wtx_port_cfg_init */
static void atm_wtx_port_cfg_init(struct atm_priv *priv,
		int qsb_en)
{
	struct wtx_port_config_t wtx_port_config;

	memset((void *)&wtx_port_config, 0, sizeof(struct wtx_port_config_t));
	wtx_port_config.qsben = qsb_en;
	/* BC0: Use TX Queue 0 - if QSB is Disabled */
	wtx_port_config.qid = 0;
	tc_mem_write(priv, fpi_addr(__WTX_PORT_CONFIG0),
			&wtx_port_config,
		sizeof(struct wtx_port_config_t));
	/* BC1: Use TX Queue 1 - if QSB is Disabled */
	wtx_port_config.qid = 1;
	tc_mem_write(priv, fpi_addr(__WTX_PORT_CONFIG1), &wtx_port_config,
		sizeof(struct wtx_port_config_t));

	return;
}

/* vrx318_atm_htu_init */
static void atm_htu_init(struct atm_priv *priv)
{
	/* OAM_F4_SEG_HTU_ENTRY */
	sb_w32(0x00000031, HTU_ENTRY_TBL(OAM_F4_SEG_HTU_ENTRY));
	sb_w32(0xfff0000e, HTU_MASK_TBL(OAM_F4_SEG_HTU_ENTRY));
	sb_w32(0x00000600, HTU_RESULT_TBL(OAM_F4_SEG_HTU_ENTRY));

	/* OAM_F4_TOT_HTU_ENTRY */
	sb_w32(0x00000041, HTU_ENTRY_TBL(OAM_F4_TOT_HTU_ENTRY));
	sb_w32(0xfff0000e, HTU_MASK_TBL(OAM_F4_TOT_HTU_ENTRY));
	sb_w32(0x00000600, HTU_RESULT_TBL(OAM_F4_TOT_HTU_ENTRY));

	/* OAM_F5_HTU_ENTRY */
	sb_w32(0x00000009, HTU_ENTRY_TBL(OAM_F5_HTU_ENTRY));
	sb_w32(0xfffffff2, HTU_MASK_TBL(OAM_F5_HTU_ENTRY));
	sb_w32(0x00000600, HTU_RESULT_TBL(OAM_F5_HTU_ENTRY));

	return;
}

/* ATM Downstream initialization
* vrx318_ds_aal5_desq_cfg_ctxt_init
* - RXIN
* - RXOUT
*/
static int atm_ds_aal5_desq_cfg_ctxt_init(struct atm_priv *priv)
{
	int i;
	rx_descriptor_t rx_descriptor;
	dma_addr_t phy_addr;
	desq_cfg_ctxt_t ds_desq_cfg_ctxt;

	memset((void *)&ds_desq_cfg_ctxt, 0, sizeof(ds_desq_cfg_ctxt));
	/* Initialize Downstream Descriptor Queue Config/Context */
	ds_desq_cfg_ctxt.des_in_own_val		= 0;
	ds_desq_cfg_ctxt.fast_path		= 0;
	ds_desq_cfg_ctxt.mbox_int_en		= 0;
	ds_desq_cfg_ctxt.des_sync_needed	= 0;
	ds_desq_cfg_ctxt.des_num = ACA_RXOUT_HD_DESC_NUM;
	ds_desq_cfg_ctxt.des_base_addr		= __DS_PKT_DES_LIST_BASE;
	tc_mem_write(priv, fpi_addr(__DS_PKT_DESQ_CFG_CTXT),
		&ds_desq_cfg_ctxt, sizeof(desq_cfg_ctxt_t));

	memset((void *)&rx_descriptor, 0, sizeof(rx_descriptor));
	rx_descriptor.own = 1;/*!ds_desq_cfg_ctxt.des_in_own_val;*/
	rx_descriptor.sop = 0;
	rx_descriptor.eop = 0;
	rx_descriptor.data_len = MAX_DATA_LEN;
	/* Initialize DS Descriptors */
	for (i = 0; i < ds_desq_cfg_ctxt.des_num; i++) {
		if (!tc_buf_alloc(priv, DMA_PACKET_SZ,
				&phy_addr, DS_DIR, priv->pdev)) {
			set_tc_status(priv->tc_priv, TC_ERR);
			return -ENOMEM;
		}
		rx_descriptor.data_ptr
			= (unsigned int)phy_addr;
		tc_mem_write(priv,
			fpi_addr(ds_desq_cfg_ctxt.des_base_addr + (i * 2)),
			&rx_descriptor, sizeof(rx_descriptor));
	}

	return 0;
}

/* vrx318_ds_oam_desq_cfg_ctxt_init */
static int atm_ds_oam_desq_cfg_ctxt_init(struct atm_priv *priv)
{
	int i;
	desq_cfg_ctxt_t oam_desq_cfg_ctxt;
	rx_descriptor_t rx_descriptor;
	dma_addr_t phy_addr;

	/* Initialize Downstream OAM Descriptor Queue Config/Context */
	memset((void *)&oam_desq_cfg_ctxt, 0, sizeof(oam_desq_cfg_ctxt));
	oam_desq_cfg_ctxt.des_in_own_val	= 0;
	oam_desq_cfg_ctxt.fast_path		= 0;
	oam_desq_cfg_ctxt.mbox_int_en		= 0;

	/*FW does not it copy to SoC, driver launch a tasklet
	* to directly fetch from vrx318 des list
	*/
	oam_desq_cfg_ctxt.des_sync_needed	= 0;
	oam_desq_cfg_ctxt.des_num		= DS_OAM_DES_NUM;
	oam_desq_cfg_ctxt.des_base_addr		= __DS_OAM_DES_LIST_BASE;
	tc_mem_write(priv, fpi_addr(__DS_OAM_DESQ_CFG_CTXT),
		&oam_desq_cfg_ctxt, sizeof(oam_desq_cfg_ctxt));

	priv->oam_llst.oam_num = oam_desq_cfg_ctxt.des_num;
	priv->oam_llst.oam_des_list
		= kzalloc(priv->oam_llst.oam_num * sizeof(unsigned int),
			GFP_KERNEL);
	if (!priv->oam_llst.oam_des_list) {
		tc_err(priv->tc_priv, MSG_INIT, "Invalid OAM DES List\n");
		return -ENOMEM;
	}
	priv->oam_llst.oam_idx = 0;

	/* Init OWN bit of all the Descriptors in Shadow DS Descriptor List */
	rx_descriptor.own = !oam_desq_cfg_ctxt.des_in_own_val;
	rx_descriptor.sop = 0;
	rx_descriptor.eop = 0;
	/*0 EDMA doesn't require the data_len */
	rx_descriptor.data_len = ATM_OAM_SIZE;
	for (i = 0; i < oam_desq_cfg_ctxt.des_num; i++) {
		priv->oam_llst.oam_des_list[i] =
			(u32)tc_buf_alloc(priv, ATM_OAM_SIZE, &phy_addr,
			DS_DIR, priv->pdev);
		if (!priv->oam_llst.oam_des_list[i]) {
			tc_err(priv->tc_priv,
				MSG_INIT,
				"Failed to allocate buffer for DS OAM\n");
			set_tc_status(priv->tc_priv, TC_ERR);
			return -ENOMEM;
		}
		rx_descriptor.data_ptr = phy_addr;
		tc_mem_write(priv,
			fpi_addr(oam_desq_cfg_ctxt.des_base_addr + (i * 2)),
			&rx_descriptor, sizeof(rx_descriptor));
	}

	return 0;
}

static void atm_us_fastpath_desq_cfg_ctxt_init(struct atm_priv *priv)
{
	int i, own;
	desq_cfg_ctxt_t us_fp_desq_cfg_ctxt;

	memset(&us_fp_desq_cfg_ctxt, 0, sizeof(us_fp_desq_cfg_ctxt));
	/* Initialize Up-Stream Fast-Path Descriptor Queue Config/Context */
	/*1 - owned by SoC CDMA, 0 - owned by VRX318 */
	us_fp_desq_cfg_ctxt.des_in_own_val	= 1;
	us_fp_desq_cfg_ctxt.fast_path		= 1;
	us_fp_desq_cfg_ctxt.mbox_int_en		= 0;
	us_fp_desq_cfg_ctxt.des_sync_needed	= 0;/* 318 is 1*/
	us_fp_desq_cfg_ctxt.des_num		= __US_FAST_PATH_DES_LIST_NUM;
	us_fp_desq_cfg_ctxt.des_base_addr	= __US_FAST_PATH_DES_LIST_BASE;

	tc_mem_write(priv, fpi_addr(__US_FP_INQ_DES_CFG_CTXT),
		&us_fp_desq_cfg_ctxt, sizeof(desq_cfg_ctxt_t));

	/* Init OWN bit of all the Descriptors in Shadow Fast Path Des List */
	own = !us_fp_desq_cfg_ctxt.des_in_own_val;
	own = (own & 1) << 31;

	for (i = 0; i < us_fp_desq_cfg_ctxt.des_num; i++)
		sb_w32(own,	us_fp_desq_cfg_ctxt.des_base_addr + (i * 2));

	return;
}

/* Initialize QoS related configuration for
* vrx318_atm_us_qos_cfg_init
* Initializes the below PPE FW Data Structures
* 1. INQ_QoS_CFG
* 2. QoSQ_CFG_CTXT
* 4. SHAPING_WFQ_CFG
* 5. QOSQ_MIB
* 6. QOSQ_FLOW_CTRL_CFG
* 7. STD_DES_CFG
*/
static void atm_us_qos_cfg_init(struct atm_priv *priv)
{
	int i, offset;
	qosq_flow_ctrl_cfg_t qosq_flow_ctrl_cfg;
	std_des_cfg_t std_des_cfg;
	inq_qos_cfg_t fp_qos_cfg, cpu_qos_cfg;
	qosq_cfg_ctxt_t qosq_cfg_ctxt;

	/* Initialize QOSQ_FLOW_CTRL_CFG */
	memset(&qosq_flow_ctrl_cfg, 0, sizeof(qosq_flow_ctrl_cfg));
	qosq_flow_ctrl_cfg.large_frame_size = 1024;
	qosq_flow_ctrl_cfg.large_frame_drop_th = 28;
	tc_mem_write(priv, fpi_addr(__QOSQ_FLOW_CTRL_CFG),
		&qosq_flow_ctrl_cfg, sizeof(qosq_flow_ctrl_cfg));

	/* Initialize STD_DES_CFG */
	memset(&std_des_cfg, 0, sizeof(std_des_cfg));
	std_des_cfg.byte_off = 0;
	std_des_cfg.data_len = MAX_DATA_LEN;
	tc_mem_write(priv, fpi_addr(__STD_DES_CFG),
		&std_des_cfg, sizeof(std_des_cfg));

	/* Setup INQ_QoS_CFG for Fast-Path & CPU-Path */
	memset(&fp_qos_cfg, 0, sizeof(fp_qos_cfg));
	memset(&cpu_qos_cfg, 0, sizeof(cpu_qos_cfg));

	/* Fast path */
	fp_qos_cfg.qos_en         = 1;
	fp_qos_cfg.qid_mask       = 0xF;
	fp_qos_cfg.qosq_base_qid  = 0;
	fp_qos_cfg.desq_cfg_ctxt  = __US_FP_INQ_DES_CFG_CTXT;
	tc_mem_write(priv, INQ_QOS_CFG_BASE(0), &fp_qos_cfg,
			sizeof(fp_qos_cfg));

	/* Setup Fast-Path DESQ_CFG_CTXT */
	atm_us_fastpath_desq_cfg_ctxt_init(priv);

	/* Setup QoSQ_CFG_CTXT */
	memset(&qosq_cfg_ctxt, 0, sizeof(qosq_cfg_ctxt));

	qosq_cfg_ctxt.threshold = 8;
	qosq_cfg_ctxt.des_num   = TOTAL_QOS_DES_NUM / txq_num();

	offset = 0;
	for (i = 0; i < txq_num(); i++) { /* txq_num() was 8*/
		qosq_cfg_ctxt.des_base_addr = __US_QOSQ_DES_LIST_BASE + offset;
		tc_mem_write(priv,
			fpi_addr((__QOSQ_CFG_CTXT_BASE
				+ (i * __QOSQ_CFG_CTXT_SIZE))),
			&qosq_cfg_ctxt, sizeof(qosq_cfg_ctxt));
		offset += qosq_cfg_ctxt.des_num * 2;
	}

	return;
}

/** vrx318_atm_us_qos_des_cfg_ctxt_init
 * vrx218_atm_us_qos_des_init: should be 512 descriptor
 */
static int atm_us_qos_des_cfg_ctxt_init(struct atm_priv *priv)
{
	int i, offset;
	tx_descriptor_t tx_descriptor;
	desq_cfg_ctxt_t desq_cfg_ctxt;
	u32 dst_addr;
	dma_addr_t phy_addr;

	/* Setup QoSQ PSEUDO DES_CFG_CTXT */
	memset(&desq_cfg_ctxt, 0, sizeof(desq_cfg_ctxt));

	desq_cfg_ctxt.des_in_own_val = 1;
	desq_cfg_ctxt.mbox_int_en = 0;
	desq_cfg_ctxt.des_sync_needed = 0;
	desq_cfg_ctxt.des_num = TOTAL_QOS_DES_NUM / txq_num();
	offset = 0;

	for (i = 0; i < txq_num(); i++) {
		desq_cfg_ctxt.des_base_addr = __US_QOSQ_DES_LIST_BASE + offset;
		dst_addr = fpi_addr(__QOSQ_PSEUDO_DES_CFG_BASE +
			(i * sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int)));
		tc_mem_write(priv, dst_addr, &desq_cfg_ctxt,
			sizeof(desq_cfg_ctxt));
		offset += desq_cfg_ctxt.des_num * 2;
	}

	memset(&tx_descriptor, 0, sizeof(tx_descriptor));
	tx_descriptor.own = !desq_cfg_ctxt.des_in_own_val;
	tx_descriptor.data_len = MAX_DATA_LEN;

	/* Initialize QoSQ Descriptors */
	for (i = 0; i < TOTAL_QOS_DES_NUM; i++) {
		tx_descriptor.data_ptr =
			(u32)tc_buf_alloc(priv, tx_descriptor.data_len,
				&phy_addr, US_DIR, priv->pdev);
		if (!tx_descriptor.data_ptr) {
			tc_err(priv->tc_priv, MSG_INIT, "Failed alloc buffer for QoSQ\n");
			set_tc_status(priv->tc_priv, TC_ERR);
			return -ENOMEM;
		}
		tx_descriptor.data_ptr = phy_addr;
		tc_mem_write(priv, fpi_addr(__US_QOSQ_DES_LIST_BASE + (i * 2)),
			&tx_descriptor, sizeof(tx_descriptor));
	}

	return 0;
}

#define PDBRAM_RX_PKT_BUFFER_END	0x22BFFF
#define PDBRAM_RX_OAM_BUFFER_BASE	(PDBRAM_RX_PKT_BUFFER_END + 1)
/** vrx318_atm_local_des_cfg_ctxt_init
* Initialize VRX218 ATM-TC Local Descriptor List
* Performs below actions
* (1) Allocate and Initialize TC UpStream Local Descriptors
* (2) Allocate and Initialize TC DownStream Local Descriptors
*/
static void atm_local_des_cfg_ctxt_init(struct atm_priv *priv)
{
	int i, des_cnt;
	tx_descriptor_t tx_descriptor;
	rx_descriptor_t rx_descriptor;
	desq_cfg_ctxt_t desq_cfg_ctxt;
	u32 ds_addr, dst_addr;
	cdma_ch_ctxt_t cdma_ch_ctxt;

	/* Setup the Local DESQ Configuration/Context for 16 UpStream Queues */
	memset(&desq_cfg_ctxt, 0, sizeof(desq_cfg_ctxt));
	desq_cfg_ctxt.des_in_own_val = 0;
	des_cnt = 0;
	for (i = 0; i < txq_num(); i++) {
		if (i == 0)
			desq_cfg_ctxt.des_num
				= 3;
		else
			desq_cfg_ctxt.des_num = 2;
		desq_cfg_ctxt.des_base_addr
			= __US_TC_LOCAL_Q_DES_LIST_BASE + (des_cnt * 2);
		dst_addr = fpi_addr(__US_TC_LOCAL_Q_CFG_CTXT_BASE
			+ i * (sizeof(desq_cfg_ctxt_t)/sizeof(unsigned int)));
		tc_mem_write(priv, dst_addr, &desq_cfg_ctxt,
				sizeof(desq_cfg_ctxt));
		des_cnt += desq_cfg_ctxt.des_num;
	}

	/* Initialize UpStream Descriptors */
	memset(&tx_descriptor, 0, sizeof(tx_descriptor));
	tx_descriptor.own = !desq_cfg_ctxt.des_in_own_val;
	tx_descriptor.data_len = MAX_DATA_LEN;
	ds_addr = PDBRAM_PPE_BASE;
	/* Note: ATM US PDBRAM pointer need convert to DWORD pointer */
	for (i = 0; i < ATM_PDBRAM_US_BUF_LEN; i++) {
		tx_descriptor.data_ptr = priv->ep->phy_membase
			+ (ds_addr + (i * tx_descriptor.data_len));
		dst_addr = fpi_addr(__US_TC_LOCAL_Q_DES_LIST_BASE + (i * 2));
		tc_mem_write(priv, dst_addr, &tx_descriptor,
				sizeof(tx_descriptor));
	}

	if (priv->tc_priv->param.sharing_cdma_en) {
		tc_dbg(priv->tc_priv, MSG_INIT,
			"Configure sharing CDMA context\n");
		memset(&rx_descriptor, 0, sizeof(rx_descriptor));
		/* rx_descriptor.own = 1; */
		rx_descriptor.data_len = MAX_DATA_LEN;

		/* Initialize UpStream Descriptors */
		for (i = 0; i < __US_LOCAL_Q_SHARED_CDMA_NUM; i++) {
			dst_addr = (u32)fpi_addr((__US_LOCAL_Q_SHARED_CDMA_BASE
				+ (i * 2)));
			tc_mem_write(priv, dst_addr,
				&rx_descriptor,
				sizeof(rx_descriptor));
		}

		memset(&cdma_ch_ctxt, 0, sizeof(cdma_ch_ctxt));
		cdma_ch_ctxt.cdma_num = __US_QOS_Q_SHARED_CDMA_NUM;
		cdma_ch_ctxt.cdma_tx_sb_base = __US_QOS_Q_SHARED_CDMA_BASE;
		cdma_ch_ctxt.cdma_ext_sb_base = __US_CDMA_EXT_SB_BASE;
		cdma_ch_ctxt.cdma_rx_sb_base = __US_LOCAL_Q_SHARED_CDMA_BASE;
		dst_addr = (u32)fpi_addr(__US_CDMA_READ_CH_CFG_CTXT);
		tc_mem_write(priv, dst_addr, &cdma_ch_ctxt,
			sizeof(cdma_ch_ctxt));
	}

	/* Setup the Local DESQ Configuration/Context for AAL5 DownStream Q*/
	memset(&desq_cfg_ctxt, 0, sizeof(desq_cfg_ctxt));
	desq_cfg_ctxt.des_in_own_val = 1;
	desq_cfg_ctxt.des_num
		= __DS_TC_LOCAL_AAL5Q_DES_LIST_NUM;
	desq_cfg_ctxt.des_base_addr = __DS_TC_LOCAL_AAL5Q_DES_LIST_BASE;
	tc_mem_write(priv, fpi_addr(__DS_TC_AAL5_LOCAL_Q_CFG_CTXT),
			&desq_cfg_ctxt, sizeof(desq_cfg_ctxt));

	memset(&rx_descriptor, 0, sizeof(rx_descriptor));
	rx_descriptor.own = 0;/* !desq_cfg_ctxt.des_in_own_val; */
	rx_descriptor.data_len = MAX_DATA_LEN;
	ds_addr = PDBRAM_PPE_BASE + ATM_PDBRAM_US_BUF_LEN * DMA_PACKET_SZ;
	/* Initialize DownStream AAL5 Descriptors */
	/* for (i = 0; i < ATM_PDBRAM_DS_BUF_LEN; i++) { */
	for (i = 0; i < __DS_TC_LOCAL_AAL5Q_DES_LIST_NUM; i++) {
		rx_descriptor.data_ptr = priv->ep->phy_membase +
				(ds_addr +	(i * DMA_PACKET_SZ));
		dst_addr = fpi_addr(__DS_TC_LOCAL_AAL5Q_DES_LIST_BASE +
				(i * 2));
		tc_mem_write(priv, dst_addr, &rx_descriptor,
				sizeof(rx_descriptor));
	}

	/* Setup the Local DESQ Configuration/Context for OAM DownStream Q */
	memset(&desq_cfg_ctxt, 0, sizeof(desq_cfg_ctxt));
	desq_cfg_ctxt.des_in_own_val = 1;
	desq_cfg_ctxt.des_num = ATM_PDBRAM_DS_OAM_LEN;
	desq_cfg_ctxt.des_base_addr = __DS_TC_LOCAL_OAMQ_DES_LIST_BASE;
	tc_mem_write(priv, fpi_addr(__DS_TC_OAM_LOCAL_Q_CFG_CTXT),
			&desq_cfg_ctxt, sizeof(desq_cfg_ctxt));

	memset(&rx_descriptor, 0, sizeof(rx_descriptor));
	rx_descriptor.own = 0;/* !desq_cfg_ctxt.des_in_own_val; */
	rx_descriptor.data_len = __US_LOCAL_Q_SHARED_CDMA_NUM;

	/* Initialize DownStream OAM Descriptors */
	/* for (i = 0; i < ARRAY_SIZE(oam_sb_addr); i++) { */
	for (i = 0; i < 10; i++) {
		/* rx_descriptor.data_ptr = fpi_addr(oam_sb_addr[i]); */
		rx_descriptor.data_ptr = (PDBRAM_RX_OAM_BUFFER_BASE +
			(i * rx_descriptor.data_len)) | priv->ep->phy_membase;
		dst_addr = fpi_addr(__DS_TC_LOCAL_OAMQ_DES_LIST_BASE +
					(i * 2));
		tc_mem_write(priv, dst_addr, &rx_descriptor,
				sizeof(rx_descriptor));
	}

	return;
}

static void atm_ppe_aca_cfg(struct atm_priv *priv)
{
	us_aca_cfg_ctxt_t us_ctxt;
	ds_aca_cfg_ctxt_t ds_ctxt;
	struct aca_event_reg_addr aca;
	u32 phybase;

	sb_w32(1, __TX_IN_ACA_ACCUM_STATUS);
	sb_w32(1, __TX_OUT_ACA_ACCUM_STATUS);
	sb_w32(1, __RX_IN_ACA_ACCUM_STATUS);
	sb_w32(1, __RX_OUT_ACA_ACCUM_STATUS);

	memset(&us_ctxt, 0, sizeof(us_ctxt));
	memset(&ds_ctxt, 0, sizeof(ds_ctxt));

	phybase = priv->ep->phy_membase;
	priv->ep->hw_ops->aca_event_addr_get(priv->ep, &aca);
	us_ctxt.tx_in_aca_msg_ptr = aca.txin_acc_sub | phybase;
	us_ctxt.tx_out_aca_msg_ptr = aca.txout_acc_add | phybase;
	us_ctxt.tx_out_aca_status_ptr
		= SB_XBAR_ADDR(__TX_OUT_ACA_ACCUM_STATUS) | phybase;
	ds_ctxt.rx_in_aca_msg_ptr = aca.rxin_acc_sub | phybase;
	ds_ctxt.rx_out_aca_msg_ptr = aca.rxout_acc_add | phybase;
	ds_ctxt.rx_in_aca_status_ptr
		= SB_XBAR_ADDR(__RX_IN_ACA_ACCUM_STATUS) | phybase;
	ds_ctxt.rx_out_aca_status_ptr
		= SB_XBAR_ADDR(__RX_OUT_ACA_ACCUM_STATUS) | phybase;
	tc_mem_write(priv, fpi_addr(__US_ACA_CFG_CTXT_BASE),
		&us_ctxt, sizeof(us_ctxt));
	tc_mem_write(priv, fpi_addr(__DS_ACA_CFG_CTXT_BASE),
		&ds_ctxt, sizeof(ds_ctxt));

	sb_w32(phybase, __EP_ADDR_BAR);
}

/* vrx218_atm_cdma_copy_ch_init */
static void atm_cdma_copy_ch_init(struct atm_priv *priv)
{
	cdma_copy_ch_cfg_t cpch_cfg;
	unsigned int dst_addr, i;

	/* Setup 8 UpStream CDMA Copy Channel */
	cpch_cfg.srcq_ctxt_ptr = __QOSQ_PSEUDO_DES_CFG_BASE;
	cpch_cfg.dstq_ctxt_ptr = __US_TC_LOCAL_Q_CFG_CTXT_BASE;
	dst_addr = fpi_addr(__US_CDMA_COPY_CH_CFG);
	tc_mem_write(priv, dst_addr, &cpch_cfg, sizeof(cpch_cfg));

	for (i = 1; i < txq_num(); i++) {
		cpch_cfg.srcq_ctxt_ptr +=
				sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int);
		cpch_cfg.dstq_ctxt_ptr +=
				sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int);
		dst_addr += sizeof(cdma_copy_ch_cfg_t);
		tc_mem_write(priv, dst_addr, &cpch_cfg, sizeof(cpch_cfg));
	}

	/* Setup AAL5 DownStream eDMA Copy Channel */
	cpch_cfg.srcq_ctxt_ptr = __DS_TC_AAL5_LOCAL_Q_CFG_CTXT;
	cpch_cfg.dstq_ctxt_ptr = __DS_PKT_DESQ_CFG_CTXT;
	dst_addr = fpi_addr(__DS_CDMA_COPY_CH_CFG);
	tc_mem_write(priv, dst_addr, &cpch_cfg, sizeof(cpch_cfg));

	/* Setup OAM DownStream eDMA Copy Channel */
	cpch_cfg.srcq_ctxt_ptr = __DS_TC_OAM_LOCAL_Q_CFG_CTXT;
	cpch_cfg.dstq_ctxt_ptr = __DS_OAM_DESQ_CFG_CTXT;
	dst_addr += sizeof(cdma_copy_ch_cfg_t);
	tc_mem_write(priv, dst_addr, &cpch_cfg, sizeof(cpch_cfg));

	return;
}

static void atm_cdma_init(struct atm_priv *priv)
{
	u32 rxbase, txbase;
	u32 rxnum, txnum;
	u32 phybase;

	phybase = priv->ep->phy_membase;
	/* Initialize CDMA control */
	tc_cdma_init(priv);
	/* Enable CDMA channel for US */
	if (priv->tc_priv->param.sharing_cdma_en) {
		tc_dbg(priv->tc_priv, MSG_INIT,
			"Configure CDMA in sharing mode\n");
		/* Channel 0 and 1 set to US */
		/* Setup RX Channel desc base address and len */
		rxbase = phybase +
			SB_XBAR_ADDR(__US_LOCAL_Q_SHARED_CDMA_BASE);
		rxnum = __US_QOS_Q_SHARED_CDMA_NUM;

		/* Setup TX Channel desc base address and len */
		txbase = phybase +
			SB_XBAR_ADDR(__US_QOS_Q_SHARED_CDMA_BASE);
		txnum = __US_QOS_Q_SHARED_CDMA_NUM;
		/* configure CDMA channel sharing */
		cdma_ch_cfg(priv, ATM_US_DMA_RXCH, ATM_US_DMA_TXCH,
			rxbase, rxnum, txbase, txnum);
		cdma_ch_on(priv, ATM_US_DMA_RXCH, ATM_US_DMA_TXCH);
	}

	/* Channel 16, 17 set to DS AAL5 */
	rxbase = phybase + SB_XBAR_ADDR(__DS_PKT_DES_LIST_BASE);
	txbase = phybase + SB_XBAR_ADDR(__DS_TC_LOCAL_AAL5Q_DES_LIST_BASE);
	rxnum = __DS_PKT_DES_LIST_NUM;
	txnum = __DS_TC_LOCAL_AAL5Q_DES_LIST_NUM;
	cdma_ch_cfg(priv, DS_AAL5_DMA_RXCH, DS_AAL5_DMA_TXCH,
		rxbase, rxnum, txbase, txnum);
	cdma_ch_on(priv, DS_AAL5_DMA_RXCH, DS_AAL5_DMA_TXCH);
	/* Channel 18, 19 set to DS OAM*/
	rxbase = phybase + SB_XBAR_ADDR(__DS_OAM_DES_LIST_BASE);
	txbase = phybase + SB_XBAR_ADDR(__DS_TC_LOCAL_OAMQ_DES_LIST_BASE);
	rxnum = __DS_OAM_DES_LIST_NUM;
	txnum = __DS_TC_LOCAL_OAMQ_DES_LIST_NUM;
	cdma_ch_cfg(priv, DS_OAM_DMA_RXCH, DS_OAM_DMA_TXCH,
		rxbase, rxnum, txbase, txnum);
	cdma_ch_on(priv, DS_OAM_DMA_RXCH, DS_OAM_DMA_TXCH);

	return;
}

static void atm_cdma_exit(struct atm_priv *priv)
{
	cdma_ch_off(priv, ATM_US_DMA_RXCH, ATM_US_DMA_TXCH);
	cdma_ch_off(priv, DS_AAL5_DMA_RXCH, DS_AAL5_DMA_TXCH);
	cdma_ch_off(priv, DS_OAM_DMA_RXCH, DS_OAM_DMA_TXCH);
}

/**
* ATM TC HW and FW Initialization: vrx318_ppe_atm_init
*/
static void ppe_atm_fw_hw_init(struct atm_priv *priv,
				u32 ep_id, int qsb_en)
{
	/* Clear PPE SB */
	ppe_sb_clear(priv);
	/* Clear PDBRAM */
	pdbram_clear(priv);
	/* Configure DMA */
	atm_pdma_init(priv);
	/* Configure ATM firmware */
	atm_fw_init(priv);
	atm_htu_init(priv);

	/* always enable qsb */
	atm_wtx_queue_cfg_init(priv, qsb_en);
	atm_wtx_port_cfg_init(priv, qsb_en);
	atm_wrx_queue_cfg_init(priv);
	atm_ds_aal5_desq_cfg_ctxt_init(priv);

	/* Initialize Downstream OAM Descriptors and desc config context */
	atm_ds_oam_desq_cfg_ctxt_init(priv);
	atm_us_qos_cfg_init(priv);
	atm_us_qos_des_cfg_ctxt_init(priv);
	atm_local_des_cfg_ctxt_init(priv);

	atm_cdma_init(priv);
	atm_cdma_copy_ch_init(priv);
	atm_ppe_aca_cfg(priv);
	/* Enable TTHA */
	ttha_enable(priv);
}

static int atm_dev_init(struct atm_priv *atm_priv, int ep_id)
{
	int i, err;

	for (i = 0; i < ATM_PORT_NUMBER; i++) {
		if (atm_priv->port[i].dev)
			continue;
		atm_priv->port[i].tx_max_cell_rate = DEFAULT_CELL_RATE;
		atm_priv->port[i].dev = atm_dev_register(g_atm_dev_name,
				atm_priv->tc_priv->ep_dev[ep_id].dev,
				&g_ppe_atm_ops, -1, NULL);
		if (!atm_priv->port[i].dev) {
			err = -EIO;
			goto ATM_DEV_REGISTER_FAIL;
		} else {
			atm_priv->port[i].dev->ci_range.vpi_bits = 8;
			atm_priv->port[i].dev->ci_range.vci_bits = 16;
			/* assume 3200 cell rate
			 * before get real information
			 */
			atm_priv->port[i].dev->link_rate =
				DEFAULT_CELL_RATE;
			atm_priv->port[i].dev->dev_data = atm_priv;
			atm_priv->port[i].dev->phy_data =
				(void *)(unsigned long)i;
		}
	}

	return 0;

ATM_DEV_REGISTER_FAIL:
	pr_err("ATM device registration fail\n");
	for (i = 0; i < ATM_PORT_NUMBER; i++)
		if (atm_priv->port[i].dev)
			atm_dev_deregister(atm_priv->port[i].dev);

	return err;
}

static int atm_dev_deinit(struct atm_priv *atm_priv)
{
	int i;

	for (i = 0; i < ATM_PORT_NUMBER; i++)
		if (atm_priv->port[i].dev)
			atm_dev_deregister(atm_priv->port[i].dev);

	return 0;
}

/* Description: Setup QSB. */
static void qsb_global_set(struct atm_priv *priv)
{
	u32 qsb_clk = get_qsb_clk(priv);
	int i;
	u32 tmp1, tmp2, tmp3;
	u32 reg_val;
	struct tc_param *param = &priv->tc_priv->param;

	/* QSB cell delay variation due to concurrency */
	param->qsb_tau   = 1;
	/* QSB scheduler burst length */
	param->qsb_srvm  = 0x0F;
	param->qsb_tstep = 4;
	tc_w32(param->qsb_tau, QSB_ICDV);
	tc_w32(param->qsb_srvm, QSB_SBL);
	tc_w32(param->qsb_tstep >> 1, QSB_CFG);

	/* set SCT and SPT per port */
	for (i = 0; i < ATM_PORT_NUMBER; i++)
		if (priv->port[i].tx_max_cell_rate != 0) {
			tmp1 = ((qsb_clk *
					param->qsb_tstep) >> 1) /
					priv->port[i].tx_max_cell_rate;
			tmp2 = tmp1 >> 6; /* integer value of Tsb */
			/* fractional part of Tsb */
			tmp3 = (tmp1 & ((1 << 6) - 1)) + 1;
			/*  carry over to integer part (?)  */
			if (tmp3 == (1 << 6)) {
				tmp3 = 0;
				tmp2++;
			}
			if (tmp2 == 0)
				tmp2 = tmp3 = 1;
			/*  1. set mask  */
			/*  2. write value to data transfer register */
			/*  3. start the tranfer */
			/*  SCT (FracRate)  */
			tc_w32(QSB_SET_SCT_MASK, QSB_RTM);
			tc_w32(tmp3, QSB_RTD);
			reg_val = QSB_RAMAC_RW_SET(QSB_RAMAC_RW_WRITE) |
					QSB_RAMAC_TSEL_SET(QSB_RAMAC_TSEL_SCT) |
					QSB_RAMAC_LH_SET(QSB_RAMAC_LH_LOW) |
					QSB_RAMAC_TESEL_SET(i & 0x01);
			tc_w32(reg_val, QSB_RAMAC);

			/*  SPT (SBV + PN + IntRage)    */
			tc_w32(QSB_SET_SPT_MASK, QSB_RTM);
			reg_val = QSB_RTD_TTV_SET(QSB_SPT_SBV_VALID) |
					QSB_SPT_PN_SET(i & 0x01) |
					QSB_SPT_INTRATE_SET(tmp2);
			tc_w32(reg_val, QSB_RTD);
			reg_val = QSB_RAMAC_RW_SET(QSB_RAMAC_RW_WRITE) |
					QSB_RAMAC_TSEL_SET(QSB_RAMAC_TSEL_SPT) |
					QSB_RAMAC_LH_SET(QSB_RAMAC_LH_LOW) |
					QSB_RAMAC_TESEL_SET(i & 0x01);
			tc_w32(reg_val, QSB_RAMAC);
		}
}

static int print_htu(struct seq_file *seq, int i)
{
	struct htu_entry entry;
	struct htu_mask mask;
	struct htu_result result;

	struct atm_priv *priv = (struct atm_priv *)seq->private;

	if (!priv) {
		pr_err("<%s>: Invalid private data\n", __func__);
		return -EINVAL;
	}

	tc_mem_read(priv, &entry, fpi_addr(HTU_ENTRY_TBL(i)), sizeof(entry));
	tc_mem_read(priv, &mask, fpi_addr(HTU_MASK_TBL(i)), sizeof(mask));
	tc_mem_read(priv, &result, fpi_addr(HTU_RESULT_TBL(i)), sizeof(result));

	if (entry.vld) {
		seq_printf(seq, "%2d. valid\n", i);
		seq_printf(seq,  "    entry  0x%08x - pid %01x, vpi %02x",
			*(u32 *)&entry, entry.pid, entry.vpi);
		seq_printf(seq,  ", vci %04x, pti %01x\n",
					entry.vci, entry.pti);
		seq_printf(seq,  "    mask   0x%08x - pid %01x, vpi %02x",
			*(u32 *)&mask, mask.pid_mask, mask.vpi_mask);
		seq_printf(seq,  ", vci %04x, pti %01x\n",
			mask.vci_mask, mask.pti_mask);
		seq_printf(seq,  "    result 0x%08x - type: %s, qid: %d",
			*(u32 *)&result, result.type ? "cell" : "AAL5",
			result.qid);
		if (result.type)
			seq_printf(seq,  ", cell id: %d, verification: %s",
				result.cellid, result.ven ? "on" : "off");
		seq_puts(seq,  "\n");
	} else
		seq_printf(seq, "%2d. invalid\n", i);

	return 0;
}

int proc_read_htu(struct seq_file *seq, void *v)
{
	int htuts;
	int i;

	struct atm_priv *priv = (struct atm_priv *)seq->private;

	if (!priv) {
		pr_err("<%s>: Invalid private data\n", __func__);
		return -EINVAL;
	}
	htuts = sb_r32(__CFG_WRX_HTUTS);

	for (i = 0; i < htuts; i++)
		print_htu(seq, i);

	return 0;
}

/* Description:
 *    Add one entry to HTU table.
 *  Input:
 *    vpi        --- unsigned int, virtual path ID
 *    vci        --- unsigned int, virtual channel ID
 *    connection --- unsigned int, connection ID
 *    aal5       --- int, 0 means AAL0, else means AAL5
 *  Output:
 *    none
 */
static void set_htu_entry(struct atm_priv *priv, unsigned int vpi,
			unsigned int vci, unsigned int conn, int aal5)
{
	struct htu_entry htu_entry = {
		.res1 = 0x00,
		.pid  = priv->conn[conn].port & 0x01,
		.vpi  = vpi,
		.vci  = vci,
		.pti  = 0x00,
		.vld  = 0x01};

	struct htu_mask htu_mask = {
		.set      = 0x03,
		.pid_mask = 0x02,
		.vpi_mask = 0x00,
		.vci_mask = 0x0000,
		.pti_mask = 0x03, /* 0xx, user data */
		.clear    = 0x00};

	struct htu_result htu_result = {
		.res1   = 0x00,
		.cellid = conn,
		.res2   = 0x00,
		.type   = aal5 ? 0x00 : 0x01,
		.ven    = 0x01,
		.res3   = 0x00,
		.qid    = conn};

	tc_mem_write(priv,
		fpi_addr(HTU_RESULT_TBL(conn + OAM_HTU_ENTRY_NUMBER)),
		&htu_result, sizeof(htu_result));
	tc_mem_write(priv,
		fpi_addr(HTU_MASK_TBL(conn + OAM_HTU_ENTRY_NUMBER)),
		&htu_mask, sizeof(htu_mask));
	tc_mem_write(priv,
		fpi_addr(HTU_ENTRY_TBL(conn + OAM_HTU_ENTRY_NUMBER)),
		&htu_entry, sizeof(htu_entry));
}

static void mpoa_setup_sync(struct atm_priv *priv, unsigned int conn)
{
	unsigned int sw_txq_tbl;
	int sw_tx_queue;
	struct wrx_queue_config_t rx_qcfg;
	struct wtx_queue_config_t tx_qcfg;
	struct uni_cell_header *cell_header;
	struct atm_vcc *vcc;
	struct net_device *dev;
	char dev_name[32];

	tc_dbg(priv->tc_priv, MSG_INIT,
		"sync: conn: %d, vpi: %d, vci: %d, mpoa_type: %d, mpoa_mode: %d\n",
		conn, priv->conn[conn].vcc->vpi, priv->conn[conn].vcc->vci,
		priv->conn[conn].mpoa_type, priv->conn[conn].mpoa_mode);

	dev = priv->conn[conn].dev;

	if (!dev)
		sprintf(dev_name, "atm_%d%d",
			priv->conn[conn].vcc->vpi, priv->conn[conn].vcc->vci);

	priv->tc_priv->tc_ops.dev_reg(dev, (!dev) ? dev_name : dev->name,
				&priv->conn[conn].subif_id, 0);
	tc_dbg(priv->tc_priv, MSG_INIT,
		"conn[%d]subif_id[%x]", conn, priv->conn[conn].subif_id);
	vcc = priv->conn[conn].vcc;

	/*  set htu entry   */
	set_htu_entry(priv, vcc->vpi, vcc->vci, conn,
		vcc->qos.aal == ATM_AAL5 ? 1 : 0);

	memset(&rx_qcfg, 0, sizeof(struct wrx_queue_config_t));
	/* setup RX queue cfg and TX queue cfg */
	rx_qcfg.new_vlan	= 0; /* No Vlan insert for GRX500 */
	rx_qcfg.vlan_ins	= 0; /* No Vlan insert */
	rx_qcfg.mpoa_type	= priv->conn[conn].mpoa_type;
	rx_qcfg.ip_ver		= ATM_IP_IPV4; /* set IPv4 as default */
	rx_qcfg.mpoa_mode	= priv->conn[conn].mpoa_mode;
	rx_qcfg.oversize	= MAX_DATA_LEN;/*param->aal5r_max_pktsz;*/
	rx_qcfg.undersize	= 0;/*param->aal5r_min_pktsz;*/
	rx_qcfg.mfs		= MAX_DATA_LEN;/*param->aal5s_max_pktsz;*/
	rx_qcfg.cpimask		= 0xFF;
	rx_qcfg.uumask		= 0xFF;
	rx_qcfg.uuexp		= 0;
	rx_qcfg.cpiexp		= 0;

	tc_mem_write(priv, fpi_addr(WRX_Q_CFG(conn)), &rx_qcfg,
			sizeof(struct wrx_queue_config_t));

	memset(&tx_qcfg, 0, sizeof(struct wtx_queue_config_t));
	/* only one TX queue is assigned now */
	/* use ioctl/proc to add other TX queues */
	tx_qcfg.same_vc_qmap   = 0x00;
	tx_qcfg.sbid           = priv->conn[conn].port;
	/* qsb qid = firmware qid + 1 */
	tx_qcfg.qsb_vcid       = conn + QSB_QUEUE_NUMBER_BASE;
	tx_qcfg.mpoa_mode      = priv->conn[conn].mpoa_mode;
	/* reserved in A4, however put 1 for backward compatible */
	tx_qcfg.qsben          = 1;

	cell_header = (struct uni_cell_header *)((unsigned int *)&tx_qcfg + 2);
	cell_header->clp = (vcc->atm_options & ATM_ATMOPT_CLP) ? 1 : 0;
	cell_header->pti = ATM_PTI_US0;
	cell_header->vci = vcc->vci;
	cell_header->vpi = vcc->vpi;
	cell_header->gfc = 0;

	for (sw_txq_tbl = priv->conn[conn].sw_txq_tbl, sw_tx_queue = 0;
		sw_tx_queue < pvc_num();
		sw_tx_queue++) {
		if ((sw_txq_tbl & BIT(sw_tx_queue)) != 0) {
			tx_qcfg.same_vc_qmap
				= sw_txq_tbl & ~(BIT(sw_tx_queue));
			tc_mem_write(priv,
				fpi_addr(WTX_Q_CFG(sw_tx_queue)),
				&tx_qcfg, sizeof(struct wtx_queue_config_t));
		}
	}

	/*  set qsb */
	set_qsb(priv, vcc, &vcc->qos, conn);

}

static void mpoa_setup(struct atm_vcc *vcc, int mpoa_type,
			int f_llc, struct net_device *dev)
{
	unsigned int conn;
	struct atm_priv *priv;
	int ep_id;

	if (!vcc) {
		pr_err("%s: Invalid VCC or Invalid network device\n", __func__);
		return;
	}

	priv = (struct atm_priv *)vcc->dev->dev_data;
	if (!priv) {
		pr_err("%s : Invalid ATM private data\n", __func__);
		return;
	}
	tc_dbg(priv->tc_priv, MSG_EVENT, "Add VPC/VCI: mpoa_type: %d, f_llc: %d, dev: %s\n",
		mpoa_type, f_llc, dev->name);

	ep_id = priv->ep_id;

	spin_lock_bh(&priv->atm_lock);
	conn = find_vcc(vcc);
	if (!conn_valid(conn))
		goto err1;

	priv->conn[conn].dev = dev;
	priv->conn[conn].mpoa_mode = f_llc;
	priv->conn[conn].mpoa_type = mpoa_type;
	spin_unlock_bh(&priv->atm_lock);

	mpoa_setup_sync(priv, conn);

	return;

err1:
	spin_unlock_bh(&priv->atm_lock);
	return;
}

static void setup_oam_htu_entry(struct atm_priv *priv)
{
	struct htu_entry htu_entry = {0};
	struct htu_result htu_result = {0};
	struct htu_mask htu_mask = {0};
	int ven = 1;
	int i;
	int max_pvc = pvc_num();

	if (priv->tc_priv->param.dfe_loopback)
		ven = 0;
	else
		ven = 1;

	htu_mask.set = 0x03;

	/* Clear HTU Tables */
	for (i = 0; i < max_pvc; i++) {
		htu_result.qid = (unsigned int)i;
		tc_mem_write(priv,
			fpi_addr(HTU_ENTRY_TBL(i + OAM_HTU_ENTRY_NUMBER)),
			&htu_entry, sizeof(htu_entry));
		tc_mem_write(priv,
			fpi_addr(HTU_MASK_TBL(i + OAM_HTU_ENTRY_NUMBER)),
			&htu_mask, sizeof(htu_mask));
		tc_mem_write(priv,
			fpi_addr(HTU_RESULT_TBL(i + OAM_HTU_ENTRY_NUMBER)),
			&htu_result, sizeof(htu_result));
	}

	/*  OAM HTU Entry   */
	htu_entry.vci		= 0x03;
	htu_mask.pid_mask	= 0x03;
	htu_mask.vpi_mask	= 0xFF;
	htu_mask.vci_mask	= 0x0000;
	htu_mask.pti_mask	= 0x07;
	htu_result.cellid	= 0;
	htu_result.type		= 1;
	htu_result.ven		= ven;
	htu_result.qid		= 0;
	tc_mem_write(priv,
		fpi_addr(HTU_RESULT_TBL(OAM_F4_SEG_HTU_ENTRY)),
		&htu_result, sizeof(htu_result));
	tc_mem_write(priv,
		fpi_addr(HTU_MASK_TBL(OAM_F4_SEG_HTU_ENTRY)),
		&htu_mask, sizeof(htu_mask));
	tc_mem_write(priv,
		fpi_addr(HTU_ENTRY_TBL(OAM_F4_SEG_HTU_ENTRY)),
		&htu_entry, sizeof(htu_entry));

	htu_entry.vci		= 0x04;
	htu_result.cellid	= 0;
	htu_result.type		= 1;
	htu_result.ven		= ven;
	htu_result.qid		= 0;
	tc_mem_write(priv,
		fpi_addr(HTU_RESULT_TBL(OAM_F4_TOT_HTU_ENTRY)),
		&htu_result, sizeof(htu_result));
	tc_mem_write(priv,
		fpi_addr(HTU_MASK_TBL(OAM_F4_TOT_HTU_ENTRY)),
		&htu_mask, sizeof(htu_mask));
	tc_mem_write(priv,
		fpi_addr(HTU_ENTRY_TBL(OAM_F4_TOT_HTU_ENTRY)),
		&htu_entry, sizeof(htu_entry));

	htu_entry.vci		= 0x00;
	htu_entry.pti		= 0x04;
	htu_mask.vci_mask	= 0xFFFF;
	htu_mask.pti_mask	= 0x01;
	htu_result.cellid	= 0;
	htu_result.type		= 1;
	htu_result.ven		= ven;
	htu_result.qid		= 0;
	tc_mem_write(priv,
		fpi_addr(HTU_RESULT_TBL(OAM_F5_HTU_ENTRY)),
		&htu_result, sizeof(htu_result));
	tc_mem_write(priv,
		fpi_addr(HTU_MASK_TBL(OAM_F5_HTU_ENTRY)),
		&htu_mask, sizeof(htu_mask));
	tc_mem_write(priv,
		fpi_addr(HTU_ENTRY_TBL(OAM_F5_HTU_ENTRY)),
		&htu_entry, sizeof(htu_entry));
}

static unsigned int atm_get_pvc_id(struct sk_buff *skb)
{
	struct pmac_rx_hdr *pmac_hdr;

	if (!skb) {
		pr_err("%s : Invalid skb\n", __func__);
		return -EINVAL;
	}
	pmac_hdr = (struct pmac_rx_hdr *)skb->data;
	if (!pmac_hdr) {
		pr_err("%s : skb'data is NULL\n", __func__);
		return -EINVAL;
	}
	return (pmac_hdr->src_sub_inf_id2 >> 3) & 0xF;
}

static int atm_get_qid_by_vcc(struct net_device *dev, struct sk_buff *skb,
		void *vcc, uint32_t flags)
{
	struct atm_priv *priv;
	unsigned int conn;
	int qid, type, prio;

	if (!dev || !vcc) {
		pr_err("%s: Invalid dev/skb/vcc\n", __func__);
		return -EINVAL;
	}

	priv = g_atm_tc;
	if (!priv) {
		pr_err("%s: ATM not init\n", __func__);
		return -EINVAL;
	}

	spin_lock_bh(&priv->atm_lock);
	if (skb)
		prio = skb->priority < ATM_PRIO_Q_NUM ?
			skb->priority : ATM_PRIO_Q_NUM - 1;
	else
		prio = 0;
	conn = find_vcc(vcc);

	if (!conn_valid(conn)) {
		tc_dbg(priv->tc_priv, MSG_EVENT,
			"%s: given VCC is not attached\n", __func__);
		spin_unlock_bh(&priv->atm_lock);
		return -ENODEV;
	} else {
		qid = priv->conn[conn].prio_queue_map[prio];
		type = priv->conn[conn].mpoa_type;
		qid = priv->conn[conn].subif_id
				| ATM_DESC_SUBIF_ID(qid, 0, type);
	}

	spin_unlock_bh(&priv->atm_lock);

	return qid;
}

int sw_tx_queue_del(struct atm_priv *priv, int conn)
{
	int i;
	int qid, txq_id;
	wtx_queue_config_t wtx_q_cfg;
	u32 new_q_tbl, q_tbl;
	struct atm_pvc *connect;

	spin_lock_bh(&priv->atm_lock);
	connect = &priv->conn[conn];
	q_tbl = connect->sw_txq_tbl;

	qid = txq_id = -1;
	for (i = (pvc_num() - 1); i >= 0; i--) {
		if (q_tbl & BIT(i)) {
			if (qid == -1)
				qid = i;
			else {
				txq_id = i;
				break;
			}
		}
	}

	/* Last Queue or something wrong */
	if (qid < 0 || q_tbl == BIT(qid)
		|| ((q_tbl & BIT(qid)) == 0) || txq_id < 0) {
		spin_unlock_bh(&priv->atm_lock);
		return -EIO;
	}

	/* Update queue map table */
	new_q_tbl = connect->sw_txq_tbl & (~(BIT(qid)));

	connect->sw_txq_tbl = new_q_tbl;
	priv->sw_txq_tbl &= ~(BIT(qid));

	for (i = 0; i < ATM_PRIO_Q_NUM; i++) {
		if (connect->prio_queue_map[i] == qid)
			connect->prio_queue_map[i] = txq_id;
	}
	spin_unlock_bh(&priv->atm_lock);

	for (i = 0; i < qid; i++) {
		tc_mem_read(priv, &wtx_q_cfg, fpi_addr(WTX_Q_CFG(i)),
			sizeof(wtx_q_cfg));
		wtx_q_cfg.same_vc_qmap &= ~(BIT(qid));
		tc_mem_write(priv, fpi_addr(WTX_Q_CFG(i)),
			&wtx_q_cfg, sizeof(wtx_q_cfg));
	}
	tc_memset(priv, fpi_addr(WTX_Q_CFG(qid)), 0, sizeof(wtx_q_cfg));

	return 0;
}

int sw_tx_queue_add(struct atm_priv *priv, int conn)
{
	int i;
	int qid;
	wtx_queue_config_t wtx_q_cfg;
	u32 new_q_tbl, q_tbl;
	struct atm_pvc *connect;

	/* Get available Phyiscal Queue ID */
	spin_lock_bh(&priv->atm_lock);
	for (qid = 0; qid < pvc_num(); qid++) {
		if ((priv->sw_txq_tbl & BIT(qid)) == 0)
			break;
	}

	if (qid >= pvc_num()) {
		tc_err(priv->tc_priv, MSG_INIT, "No available physical queue\n");
		spin_unlock_bh(&priv->atm_lock);
		return -ENOSPC;
	}

	connect = &priv->conn[conn];
	/* Reconfigure Existing Queue's QMAP */
	q_tbl = connect->sw_txq_tbl;
	new_q_tbl = q_tbl | BIT(qid);

	/* Update local and global tx queue table */
	connect->sw_txq_tbl = new_q_tbl;
	priv->sw_txq_tbl |= BIT(qid);
	spin_unlock_bh(&priv->atm_lock);

	for (i = 0; i < pvc_num(); i++) {
		if (q_tbl & BIT(i)) {
			tc_mem_read(priv, &wtx_q_cfg, fpi_addr(WTX_Q_CFG(i)),
				sizeof(wtx_q_cfg));
			wtx_q_cfg.same_vc_qmap = new_q_tbl & (~(BIT(i)));
			tc_mem_write(priv, fpi_addr(WTX_Q_CFG(i)),
				&wtx_q_cfg, sizeof(wtx_q_cfg));
		}
	}
	/* Add new Queue Setting */
	wtx_q_cfg.same_vc_qmap = new_q_tbl & (~(BIT(qid)));
	tc_mem_write(priv, fpi_addr(WTX_Q_CFG(qid)),
		&wtx_q_cfg, sizeof(wtx_q_cfg));

	return 0;
}

static void atm_encapsulate_frame(struct sk_buff *skb, int conn)
{
	unsigned int proto_type;
	int mpoa_type, mpoa_mode;

	mpoa_type = g_atm_tc->conn[conn].mpoa_type;
	mpoa_mode = g_atm_tc->conn[conn].mpoa_mode;

	/* ETH packet, need recover ATM encapsulation  */
	if (mpoa_mode) {
		/*  LLC */
		switch (mpoa_type) {
		case 0: /* EoA w/o FCS */
			skb_push(skb, 10);
			((u32 *)skb->data)[0] = 0xAAAA0300;
			((u32 *)skb->data)[1] = 0x80C20007;
			((u16 *)skb->data)[4] = 0x0000;
			break;

		case 1: /* EoA w FCS */
			skb_push(skb, 10);
			((u32 *)skb->data)[0] = 0xAAAA0300;
			((u32 *)skb->data)[1] = 0x80C20001;
			((u16 *)skb->data)[4] = 0x0000;
			break;

		case 2: /* PPPoA */
			proto_type = ntohs(*(u16 *)(skb->data + 12));
			switch (proto_type) {
			case 0x0800:
				proto_type = 0x0021;
				break;
			case 0x86DD:
				proto_type = 0x0057;
				break;
			}
			skb_pull(skb, ETH_HLEN - 6);
			((u32 *)skb->data)[0] = 0xFEFE03CF;
			((u16 *)skb->data)[2] = (u16)proto_type;
			break;

		case 3: /*  IPoA */
			skb_pull(skb, ETH_HLEN - 8);
			((u32 *)skb->data)[0] = 0xAAAA0300;
			((u16 *)skb->data)[2] = 0x0000;
			break;
		}
	} else {
		/*  VC-mux  */
		switch (mpoa_type) {
		case 0: /* EoA w/o FCS */
			skb_push(skb, 2);
			*(u16 *)skb->data = 0x0000;
			break;

		case 1: /* EoA w FCS */
			skb_push(skb, 2);
			*(u16 *)skb->data = 0x0000;
			break;

		case 2: /* PPPoA */
			proto_type = ntohs(*(u16 *)(skb->data + 12));
			switch (proto_type) {
			case 0x0800:
				proto_type = 0x0021;
				break;
			case 0x86DD:
				proto_type = 0x0057;
				break;
			}
			skb_pull(skb, ETH_HLEN - 2);
			*(u16 *)skb->data = (u16)proto_type;
			break;
		case 3: /*  IPoA */
			skb_pull(skb, ETH_HLEN);
			break;
		}
	}
	dump_skb_info(g_atm_tc->tc_priv, skb, (MSG_RX | MSG_RXDATA));
}

static void atm_push(struct net_device *rxif,
		struct sk_buff *skb)
{
	unsigned int conn;
	struct atm_vcc *vcc = NULL;
	struct atm_priv *priv = g_atm_tc;

	if (!priv) {
		pr_err("%s : Invalid ATM private data\n", __func__);
		goto err;
	}
	conn = atm_get_pvc_id(skb);
	skb_pull(skb, PMAC_SIZE);
	if (unlikely(!conn_valid(conn)
		|| (priv->conn[conn].vcc == NULL)
		|| (priv->pvc_tbl & BIT(conn)) == 0)) {
		tc_dbg(priv->tc_priv, MSG_RX,
			"invalid conn: %d or NULL Vcc\n", conn);
	} else {
		vcc = priv->conn[conn].vcc;
		if (atm_charge(vcc, skb->truesize)) {

			if (vcc->stats)
				atomic_inc(&vcc->stats->rx);

			spin_lock_bh(&priv->atm_lock);
			if (vcc->qos.aal == ATM_AAL5) {
				priv->stats.aal5_rx_pkts++;
				priv->stats.aal5_rx_bytes += skb->len;
				priv->conn[conn].stats.aal5_rx_pkts++;
				priv->conn[conn].stats.aal5_rx_bytes
					+= skb->len;
			} else
				priv->stats.aal5_rx_errors++;
			priv->conn[conn].access_time = current_kernel_time();
			spin_unlock_bh(&priv->atm_lock);

			ATM_SKB(skb)->vcc = vcc;
			atm_encapsulate_frame(skb, conn);
			vcc->push(vcc, skb);

			return;
		} else {
			tc_dbg(priv->tc_priv, MSG_RX,
				"inactive qid %d\n", conn);

			if (vcc->stats)
				atomic_inc(&vcc->stats->rx_drop);

			spin_lock_bh(&priv->atm_lock);
			if (vcc->qos.aal == ATM_AAL5)
				priv->stats.aal5_rx_dropped++;
			priv->conn[conn].stats.aal5_rx_dropped++;
			spin_unlock_bh(&priv->atm_lock);
		}
	}
err:
	dev_kfree_skb_any(skb);
}

static void atm_irq_on(u32 irq_id)
{
	u32 irq_ier;
	struct atm_priv *priv = g_atm_tc;

	spin_lock_irq(&priv->oam_lock);
	irq_ier = tc_r32(MBOX_IGU_IER(MBOX_IGU1));
	irq_ier |= irq_id;
	mbox_set_ier(priv, MBOX_IGU1, irq_ier);
	spin_unlock_irq(&priv->oam_lock);
}

static void atm_irq_off(u32 irq_id)
{
	u32 irq_ier;
	struct atm_priv *priv = g_atm_tc;

	spin_lock_irq(&priv->oam_lock);
	irq_ier = tc_r32(MBOX_IGU_IER(MBOX_IGU1));
	irq_ier &= ~irq_id;
	mbox_set_ier(priv, MBOX_IGU1, irq_ier);
	spin_unlock_irq(&priv->oam_lock);
}

static int atm_showtime_enter(const unsigned char idx,
		struct port_cell_info *port_cell, void *xdata_addr)
{
	int i, j;
	struct atm_priv *priv = g_atm_tc;
	struct dc_ep_dev *ep;

	tc_dbg(priv->tc_priv, MSG_EVENT, "ATM Showtime: Enter\n");
	atm_ppe_ps_restore(priv);
	if (port_cell != NULL) {
		for (j = 0; j < ATM_PORT_NUMBER && j < port_cell->port_num;
				j++)
			if (port_cell->tx_link_rate[j] > 0)
				break;
		if (j >= ATM_PORT_NUMBER) {
			tc_err(priv->tc_priv, MSG_EVENT,
				"Invalid tx link rate: %d, %d\n",
				port_cell->tx_link_rate[0],
				port_cell->tx_link_rate[1]);
			return -EINVAL;
		}
		for (i = 0; i < ATM_PORT_NUMBER && i < port_cell->port_num;
			i++) {
			priv->port[i].tx_max_cell_rate
				= port_cell->tx_link_rate[i] > 0 ?
					port_cell->tx_link_rate[i] :
					port_cell->tx_link_rate[j];
			/* test QSB */
			tc_dbg(priv->tc_priv, MSG_EVENT,
				"ATM Showtime: max rate[%u] args[%u %u]\n",
				priv->port[i].tx_max_cell_rate,
				port_cell->tx_link_rate[i],
				port_cell->tx_link_rate[j]);
		}
	} else
		tc_err(priv->tc_priv, MSG_EVENT, "Invalid PORT CELL\n");


	if (idx != priv->ep_id) {
		tc_err(priv->tc_priv, MSG_EVENT,
			"Invalid Line [%d]#[%d]\n",
			idx, priv->ep_id);
		return 0;
	}

	/* qsb_global_set(priv); */

	for (i = 0; i < pvc_num(); i++)
		if ((priv->pvc_tbl & BIT(i)) && priv->conn[i].vcc != NULL)
			set_qsb(priv, priv->conn[i].vcc,
				&priv->conn[i].vcc->qos, i);
	ep = priv->ep;
	ep->hw_ops->aca_start(priv->ep, ACA_TXIN_EN, 0);
	tc_info(priv->tc_priv, MSG_EVENT,
		"ATM line[%d]:enter showtime, cell rate: 0 - %d, 1 - %d\n",
		idx, priv->port[0].tx_max_cell_rate,
		priv->port[1].tx_max_cell_rate);
	atm_dev_signal_change(priv->port[idx].dev, ATM_PHY_SIG_FOUND);
	return 0;
}

int atm_power_saving(struct atm_priv *priv, int ps_en, int flag)
{
	struct psave_cfg ps_cfg;

	memset(&ps_cfg, 0, sizeof(ps_cfg));

	if (!flag) {
		tc_mem_read(priv, &ps_cfg,
			fpi_addr(__PSAVE_CFG), sizeof(ps_cfg));
		return ps_cfg.sleep_en;
	} else {
		tc_mem_read(priv, &ps_cfg,
			fpi_addr(__PSAVE_CFG), sizeof(ps_cfg));
		ps_cfg.sleep_en = !!(ps_en);
		tc_mem_write(priv, fpi_addr(__PSAVE_CFG),
			&ps_cfg, sizeof(ps_cfg));
		priv->tc_priv->param.ps = ps_en;
	}

	return 0;
}

static int atm_showtime_exit(const unsigned char idx)
{
	struct atm_priv *priv = g_atm_tc;
	u32 stop = ACA_TXIN_EN;
	struct dc_ep_dev *ep;

	if (!g_atm_tc || g_atm_tc->ep_id != idx)
		return 0;

	tc_info(priv->tc_priv, MSG_EVENT, "Line[%d]: show time exit!\n", idx);

	atm_dev_signal_change(priv->port[idx].dev, ATM_PHY_SIG_LOST);
	ep = priv->ep;
	ep->hw_ops->aca_stop(ep, &stop, 0);
	/* Stop TXIN traffic, then wait for PPE FW signal */
	atm_ppe_wakeup(priv);

	return 0;
}

static int atm_framer_requst_en(const unsigned char idx)
{
	struct atm_priv *priv = g_atm_tc;

	ttha_enable(priv);
	tc_info(priv->tc_priv, MSG_EVENT, "Enable TTHA!\n");
	return 0;
}

static void atm_cb_setup(struct atm_priv *priv, int flags)
{
	spin_lock_bh(&priv->atm_lock);
	if (flags) {
		priv->tc_priv->tc_ops.recv = atm_push;
		priv->tc_priv->tc_ops.get_qid = atm_get_qid_by_vcc;
		priv->tc_priv->tc_ops.irq_on = atm_irq_on;
		priv->tc_priv->tc_ops.irq_off = atm_irq_off;
		priv->tc_priv->tc_ops.showtime_enter = atm_showtime_enter;
		priv->tc_priv->tc_ops.showtime_exit = atm_showtime_exit;
		priv->tc_priv->tc_ops.framer_request_en = atm_framer_requst_en;
	} else {
		priv->tc_priv->tc_ops.recv = NULL;
		priv->tc_priv->tc_ops.get_qid = NULL;
		priv->tc_priv->tc_ops.irq_on = NULL;
		priv->tc_priv->tc_ops.irq_off = NULL;
		priv->tc_priv->tc_ops.showtime_enter = NULL;
		priv->tc_priv->tc_ops.showtime_exit = NULL;
		priv->tc_priv->tc_ops.framer_request_en = NULL;
	}
	priv->tc_priv->tc_ops.erb_addr_get = NULL;
	spin_unlock_bh(&priv->atm_lock);
}

static int atm_datapath_init(struct atm_priv *priv)
{
	int err;
	int ep_id;
	int irq_id;

	ep_id = priv->ep_id;
	irq_id = priv->tc_priv->ep_dev[ep_id].irq;
	tc_dbg(priv->tc_priv, MSG_INIT,
		"%s : ep_id[%d] irq_id[%d]\n", __func__, ep_id, irq_id);

	/* request irq (enable by default) */
	err = devm_request_irq(priv->ep->dev, priv->ep->irq,
			atm_irq_handler, 0,
			g_atm_dev_name, (void *)priv);
	if (err) {
		tc_err(priv->tc_priv, MSG_SWITCH,
			"Failed to request PCIe MSI irq %u\n",
			irq_id);
		return err;
	}

	qsb_global_set(priv);
	setup_oam_htu_entry(priv);

	atm_hook_mpoa_setup = mpoa_setup;
	atm_cb_setup(priv, 1);

	return 0;
}

/* This API no lock protect, should only be called in initialization part */
static void atm_fw_cfg_init(struct atm_priv *priv)
{
	u32 conn;

	tc_dbg(priv->tc_priv, MSG_INIT, "ATM FW PVC init\n");

	if (priv->pvc_tbl)
		validate_oam_htu_entry(priv, 1);
	else {
		tc_dbg(priv->tc_priv, MSG_INIT,
			"%s : No PVC table\n", __func__);
		return;
	}

	for (conn = 0; conn < pvc_num(); conn++) {
		if ((priv->pvc_tbl & BIT(conn)) != 0)
			mpoa_setup_sync(priv, conn);
	}

}

static void ppe_stop(struct atm_priv *priv)
{
	pp32_stop(priv, PP32_TX);
}

static void ppe_start(struct atm_priv *priv)
{
	pp32_start(priv, PP32_TX);
}


/**
* Corresponding to vrx318_atm_tc_hw_fw_init
* Reset PP32
* Load firmware
* Configure PP32
* Configure registers
* Configure hardware modules
*/
static int atm_tc_hw_fw_init(struct atm_priv *priv)
{
	int ep_id;
	int i;

	ep_id = priv->ep_id;
	tc_dbg(priv->tc_priv, MSG_INIT, "port	  = %d\n", ep_id);
	tc_dbg(priv->tc_priv, MSG_INIT, "irq	  = %u\n",
			priv->tc_priv->ep_dev[ep_id].irq);
	tc_dbg(priv->tc_priv, MSG_INIT, "membase	  = 0x%08x\n",
		(unsigned int)priv->tc_priv->ep_dev[ep_id].membase);
	tc_dbg(priv->tc_priv, MSG_INIT, "phy_membase = 0x%08x\n",
		(unsigned int)priv->tc_priv->ep_dev[ep_id].phy_membase);

	/* Power on Modem component */
	tc_clkon(priv->ep, PMU_ATM);
	/* Reset PPE HW */
	tc_ppe_rst(priv->ep);
	/* Set VRX318 PPE clock 576MHz */
	tc_ppe_clkset(priv->ep, PPE_CLK_576MHZ);
	set_qsb_clk(priv, 0);
	/* Clear and disable mailbox   */
	for (i = MBOX_IGU0; i <= MBOX_IGU2; i++) {
		mbox_set_ier(priv, i, 0);
		mbox_clr_isr(priv, i, MBOX_MASK(i));
	}
	/* Load firmware to PP32 */
	ppe_stop(priv);
	ppe_atm_fw_hw_init(priv, ep_id, 1);
	atm_fw_load(priv);
	pp32_load(priv, &priv->fw, 1);

	/* init datapath */
	atm_datapath_init(priv);

	/* start hardware */
	setup_dfe_loopback(priv, 0);
	ppe_start(priv);
	dfe_loopback_linkup(priv);


	/* Init FW tx/rx queue /qsb configuration for pre-set PVCs */
	atm_fw_cfg_init(priv);

	/* Indicate DSL FW all configuration is ready */
	/* PPE FW is ready to receive/send packets */
	tc_stat_indicate(priv, priv->tc_priv->tc_mode, 1);

	/* enable mailbox Interrupts */
	mbox_set_ier(priv, MBOX_IGU0, 0);
	/* enable OAM, TC switch */
	mbox_set_ier(priv, MBOX_IGU1, MBOX_OAM_RX | MBOX_FRAME_STOP);
	tc_info(priv->tc_priv, MSG_SWITCH, "ATM TC init successfully\n");

	return 0;
}

int atm_tc_load(struct tc_priv *tc_priv,
		u32 ep_id, enum dsl_tc_mode tc_mode)
{
	int err;
	struct atm_priv *priv;

	if (tc_priv == NULL) {
		pr_err("%s : Invalid argument\n", __func__);
		return -EINVAL;
	}

	tc_dbg(tc_priv, MSG_INIT, "TC switch to ATM\n");
	if (!g_atm_tc) { /*First time initialization */
		priv = kzalloc(sizeof(struct atm_priv), GFP_KERNEL);
		if (!priv) {
			tc_err(tc_priv, MSG_INIT, "Failed to alloc priv buffer\n");
			err = -ENOMEM;
			goto err1;
		}
		priv->tc_priv = tc_priv;
		spin_lock_init(&priv->atm_lock);
		spin_lock_init(&priv->oam_lock);
		init_completion(&priv->comp);
		g_atm_tc = priv;
	} else
		priv = g_atm_tc;

	/* register atm device */
	err = atm_dev_init(priv, ep_id);
	if (err) {
		tc_err(tc_priv, MSG_INIT,
			"atm_dev_init failed\n");
		goto err2;
	}

	priv->ep = &tc_priv->ep_dev[ep_id];
	priv->ep_id = ep_id;
	priv->pdev = priv->ep->dev;

	/* PPE FW/HW init */
	err = atm_tc_hw_fw_init(priv);
	if (err) {
		tc_dbg(tc_priv, MSG_INIT, "failed\n");
		goto err2;
	}
	tc_priv->priv = (void *)priv;

	/* Proc Init */
	if (atm_tc_proc_init(priv) != 0) {
		tc_dbg(tc_priv, MSG_INIT, "failed\n");
		goto err2;
	}
	atm_umt_init(priv);
	atm_aca_init(priv);
	atm_umt_start(priv);

	if (priv->tc_priv->tc_ops.disable_us != NULL)
		priv->tc_priv->tc_ops.disable_us(0);

	/* Send userspace TC UP message */
	tc_ntlk_msg_send(tc_priv, 0, ATM_TC, TC_LOADED, 0, ep_id);

	tc_info(tc_priv, MSG_INIT, "ATM TC Loaded\n");
	return 0;

err2:
	kfree(priv);
	priv = NULL;
	g_atm_tc = NULL;
err1:
	return err;
}

static void atm_datapath_exit(struct atm_priv *priv)
{
	devm_free_irq(priv->pdev, priv->ep->irq, priv);
}

static void atm_free_mem(struct atm_priv *priv)
{
	int i;
	u32 phy_addr;
	for (i = 0; i < TOTAL_QOS_DES_NUM; i++) {
		phy_addr = sb_r32(__US_QOSQ_DES_LIST_BASE + (i << 1));
		priv->tc_priv->tc_ops.free(phy_addr, US_DIR);
	}
	for (i = 0; i < DS_OAM_DES_NUM; i++) {
		phy_addr = sb_r32(__DS_OAM_DES_LIST_BASE + (i << 1));
		priv->tc_priv->tc_ops.free(phy_addr, DS_DIR);
	}
	/* Free QoS queue buffers */
	for (i = 0; i < ACA_RXOUT_HD_DESC_NUM; i++) {
		phy_addr = sb_r32(__DS_PKT_DES_LIST_BASE + (i << 1));
		priv->tc_priv->tc_ops.free(phy_addr, DS_DIR);
	}
	kfree(priv->oam_llst.oam_des_list);
	return;
}

/**
* 1. Indicate DSL FW/PPE FW unload
* 2. Disable idle cell
* 3. Wait for PPE FW to complete stop
* 4. stop PPE and disable TTHA
* 5. CDMA channel off
* 6. Clear interrupt
* 7. Free pre-allocated SoC memory
*/
static void atm_ppe_exit(struct atm_priv *priv)
{
	int i;

	/* Indicate DSL FW TC Clear */
	tc_stat_indicate(priv, TC_ATM_SL_MODE, 0);

	/* Disable Idle cell for RX */
	rtha_idle_keep_disable(priv);

	/* Indicate PPE FW TC switch */
	sb_w32(1, __MODE_SW_CFG);

	/* Test only, without CBM flush, using PPE FW flush */
        priv->ep->hw_ops->aca_start(priv->ep, ACA_TXIN_EN, 0);

        /* Delay some time to allow PPE FW complete its task */
        msleep(1);

	/* Stop PPE FW */
	ppe_stop(priv);

	/* STOP DMA */
	atm_cdma_exit(priv);

	/* Kill all tasklets */
	tasklet_kill(&g_oam_tasklet);

	/* Disable and Clear all Interrupts */
	for (i = MBOX_IGU0; i < MBOX_IGU2; i++) {
		mbox_set_ier(priv, i, 0);
		mbox_clr_isr(priv, i, MBOX_MASK(i));
	}

	atm_datapath_exit(priv);


	/* free ptm pre-allocated memory */
	atm_free_mem(priv);

	return;
}


static void atm_aca_txin_poll(struct atm_priv *priv,
			u32 dbase, u32 *idx, u32 dnum, u32 *pkt_num)
{
	rx_descriptor_t desc;
	u32 off, cnt, dst, wrp_rd;

	off = *idx;
	cnt = 0;
	wrp_rd = dnum * DW_SZ(desc);

	tc_dbg(priv->tc_priv, MSG_SWITCH,
		"dbase: 0x%x, idx: %u, pending num: %u, wrap_round: %u\n",
		dbase, off, *pkt_num, wrp_rd);

	while (1) {
		dst = fpi_addr(dbase + off);
		tc_mem_read(priv, &desc, dst, sizeof(desc));
		if (desc.own == 1) {
			tc_dbg(priv->tc_priv, MSG_SWITCH,
				"addr: 0x%x, desc ptr: 0x%x\n",
				dbase + off, desc.data_ptr);
			priv->tc_priv->tc_ops.free(desc.data_ptr, US_DIR);
			desc.own = !desc.own;
			tc_mem_write(priv, dst, &desc, sizeof(desc));
			cnt++;
			off = (off + 2) % wrp_rd;
		} else
			break;
	}

	if (cnt != 0) {
		*idx = off;
		priv->ep->hw_ops->aca_txin_ack_sub(priv->ep, cnt);
		sb_w32(1, __TX_IN_ACA_ACCUM_STATUS);
		*pkt_num -= cnt;
	} else {
		tc_err(priv->tc_priv, MSG_SWITCH,
			"No ACA TXIN PENDING packets!!!\n");
		*pkt_num = 0;
	}

	tc_dbg(priv->tc_priv, MSG_SWITCH,
		"ACA TXIN POLL free %d packets\n", cnt);
}

static inline void aca_txin_wait(struct atm_priv *priv)
{
	msleep(ACA_TXIN_POLL_INTVAL);
}

static int atm_clear_aca_txin(struct atm_priv *priv,
			u32 dbase, u32 *idx, u32 dnum, u32 pkt_num)
{
	if (!sb_r32(__RX_IN_ACA_ACCUM_STATUS)) {
		tc_err(priv->tc_priv, MSG_SWITCH,
			"ACA RXIN not clean\n");
		return -EBUSY;
	}

	while (pkt_num > 0) {
		tc_dbg(priv->tc_priv, MSG_SWITCH,
			"pending pkt num: %d\n", pkt_num);
		aca_txin_wait(priv);
		atm_aca_txin_poll(priv, dbase, idx, dnum, &pkt_num);
	}
	return 0;
}

static void atm_aca_exit(struct atm_priv *priv, u32 pkt_num)
{
	struct dc_ep_dev *ep;
	u32 flag, idx;
	desq_cfg_ctxt_t desq_cfg;

	ep = priv->ep;
	tc_mem_read(priv, &desq_cfg,
		fpi_addr(__US_FP_INQ_DES_CFG_CTXT), sizeof(desq_cfg));
	idx = desq_cfg.deq_idx;
	atm_clear_aca_txin(priv, desq_cfg.des_base_addr,
			&idx, desq_cfg.des_num, pkt_num);

	flag = ACA_TXIN_EN | ACA_TXOUT_EN | ACA_RXIN_EN | ACA_RXOUT_EN;
	if (ep->hw_ops->aca_stop(ep, &flag, 1) < 0)
		tc_err(priv->tc_priv, MSG_SWITCH, "ACA is busy: %u!\n", flag);
}

static void atm_umt_exit(struct atm_priv *priv)
{
	struct tc_priv *tcpriv;

	tcpriv = priv->tc_priv;
	tcpriv->tc_ops.umt_exit(0);
}

/**
* Unload Sequence
* 1. Stop US traffic
* 2. Disable US/DS idle cell
* 3. Indicate PPE FW unload and Wait PPE to clear pending packets
* 4. PPE stop, CDMA chan off, TTHA disable, interrupt clear
* 5. free pre-allocated SoC memory
* 6. ACA stop
* 7. SoC DMA exit, UMT exit
* 8. power down PPE component including CDMA
* 9. reset soc port to start from base address
*/
void atm_tc_unload(void)
{
	struct atm_priv *priv = g_atm_tc;
	u32 pending_num;

	if (!priv) {
		pr_err("<%s>: Invalid ATM TC\n", __func__);
		return;
	}

	/* Disable US traffic */
	priv->tc_priv->tc_ops.disable_us(1);
	/* stop PPE */
	atm_ppe_exit(priv);
	/* Get pending ACA TXIN PKT num */
	/* pending_num = priv->ep->hw_ops->aca_txin_hd_cnt(priv->ep); */
	/* Enable ACA TXIN */
	/* priv->ep->hw_ops->aca_start(priv->ep, ACA_TXIN_EN, 0); */
	/* stop UMT */
	atm_umt_exit(priv);
	/* stop ACA */
	atm_aca_exit(priv, pending_num);

	/* atm_dev_deinit(priv); */
	/* modem module power off */
	tc_clkoff(priv->ep, PMU_ATM);
	/* remove proc */
	atm_tc_proc_exit(priv);
	/* remove ATM callback function */
	atm_cb_setup(priv, 0);
	/* message for TC DOWN */
	tc_ntlk_msg_send(priv->tc_priv, 0, ATM_TC, TC_UNLOADED, 0, priv->ep_id);

	tc_info(priv->tc_priv, MSG_SWITCH, "Unload ATM TC successfully\n");
}

void atm_exit(void)
{
	struct atm_priv *priv = g_atm_tc;

	if (!priv)
		return;
	atm_dev_deinit(priv);
	kfree(priv);
	g_atm_tc = NULL;

	pr_info("ATM TC exit!\n");
}
