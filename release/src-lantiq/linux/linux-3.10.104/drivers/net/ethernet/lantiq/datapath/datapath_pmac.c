#include <linux/kernel.h>	/* printk() */
#include <linux/types.h>	/* size_t */
#include <linux/etherdevice.h>
#include <net/lantiq_cbm.h>
#include <net/lantiq_cbm_api.h>
#include <net/datapath_api.h>
#include "datapath.h"
#include <asm/gic.h>

int dp_gsw_kioctl(char *dev_name, u32 command, u32 arg)
{
	GSW_API_HANDLE handle;
	int res;

	handle = gsw_api_kopen(dev_name);
	if (!handle) {
		PR_ERR("Failed to call API gsw_api_kopen for %s\n", dev_name);
		return -1;
	}
	res = gsw_api_kioctl(handle, command, arg);
	if (res != GSW_statusOk) {
		PR_ERR("Failed to call gsw_api_kioctl with return value %d\n",
		       res);
		return -1;
	}
	return 0;
}

/*This API is only for GSWIP-R PMAC modification, not for GSWIP-L */
int32_t dp_pmac_set(uint32_t port, dp_pmac_cfg_t *pmac_cfg)
{
	GSW_PMAC_Eg_Cfg_t egcfg;
	GSW_PMAC_Ig_Cfg_t igcfg;
	int i, j, k;
	uint32_t flag = 0;
	cbm_dq_port_res_t dqport;
	int32_t ret;
	GSW_API_HANDLE gswr_r;
	GSW_PMAC_Glbl_Cfg_t pmac_glb;

	if (!pmac_cfg || !port) {
		PR_ERR("dp_pmac_set:wrong parameter(pmac_cfg/port NULL)\n");
		return -1;
	}

	if (!pmac_cfg->ig_pmac_flags && !pmac_cfg->eg_pmac_flags)
		return 0;

	memset(&dqport, 0, sizeof(cbm_dq_port_res_t));

	/* Get GSWIP device hander */
	if ((port >= 1) && (port <= 6))
		gswr_r = gsw_api_kopen("/dev/switch_api/0");
	else
		gswr_r = gsw_api_kopen("/dev/switch_api/1");

	if (gswr_r == 0) {
		PR_ERR("dp_pmac_set:Failed to open %s!!\n", GSWIP_R_DEV_NAME);
		return -1;
	}

	/*set ingress port via DMA tx channel */
	if (pmac_cfg->ig_pmac_flags) {

		/*Read back igcfg from gsw first */
		ret = cbm_dequeue_port_resources_get(port, &dqport, flag);

		if (ret == -1) {
			PR_ERR("cbm_dequeue_port_resources_get failed\n");
			return -1;
		}

		memset(&igcfg, 0, sizeof(GSW_PMAC_Ig_Cfg_t));

		for (i = 0; i < dqport.num_deq_ports; i++) {
			igcfg.nTxDmaChanId = dqport.deq_info[i].dma_tx_chan;
			if (igcfg.nTxDmaChanId == (u8)-1)
			  igcfg.nTxDmaChanId = pmac_cfg->ig_pmac.tx_dma_chan;
			gsw_api_kioctl(gswr_r, GSW_PMAC_IG_CFG_GET,
				       (u32) &igcfg);

			/*update igcfg and write back to gsw */
			if (pmac_cfg->ig_pmac_flags & IG_PMAC_F_ERR_DISC)
				igcfg.bErrPktsDisc =
				    pmac_cfg->ig_pmac.err_disc;

			if (pmac_cfg->ig_pmac_flags & IG_PMAC_F_PRESENT)
				igcfg.bPmacPresent = pmac_cfg->ig_pmac.pmac;

			if (pmac_cfg->ig_pmac_flags & IG_PMAC_F_SUBIF)
				igcfg.bSubIdDefault =
				    pmac_cfg->ig_pmac.def_pmac_subifid;

			if (pmac_cfg->ig_pmac_flags & IG_PMAC_F_SPID)
				igcfg.bSpIdDefault =
				    pmac_cfg->ig_pmac.def_pmac_src_port;

			if (pmac_cfg->ig_pmac_flags & IG_PMAC_F_CLASSENA)
				igcfg.bClassEna =
				    pmac_cfg->ig_pmac.def_pmac_en_tc;

			if (pmac_cfg->ig_pmac_flags & IG_PMAC_F_CLASS)
				igcfg.bClassDefault =
				    pmac_cfg->ig_pmac.def_pmac_tc;

			if (pmac_cfg->ig_pmac_flags & IG_PMAC_F_PMAPENA)
				igcfg.bPmapEna =
				    pmac_cfg->ig_pmac.def_pmac_en_pmap;

			if (pmac_cfg->ig_pmac_flags & IG_PMAC_F_PMAP)
				igcfg.bPmapDefault =
				    pmac_cfg->ig_pmac.def_pmac_pmap;

			if (pmac_cfg->ig_pmac_flags & IG_PMAC_F_PMACHDR1)
				igcfg.defPmacHdr[0] =
				    pmac_cfg->ig_pmac.def_pmac_hdr[0];

			if (pmac_cfg->ig_pmac_flags & IG_PMAC_F_PMACHDR2)
				igcfg.defPmacHdr[1] =
				    pmac_cfg->ig_pmac.def_pmac_hdr[1];

			if (pmac_cfg->ig_pmac_flags & IG_PMAC_F_PMACHDR3)
				igcfg.defPmacHdr[2] =
				    pmac_cfg->ig_pmac.def_pmac_hdr[2];

			if (pmac_cfg->ig_pmac_flags & IG_PMAC_F_PMACHDR4)
				igcfg.defPmacHdr[3] =
				    pmac_cfg->ig_pmac.def_pmac_hdr[3];

			if (pmac_cfg->ig_pmac_flags & IG_PMAC_F_PMACHDR5)
				igcfg.defPmacHdr[4] =
				    pmac_cfg->ig_pmac.def_pmac_hdr[4];

			if (pmac_cfg->ig_pmac_flags & IG_PMAC_F_PMACHDR6)
				igcfg.defPmacHdr[5] =
				    pmac_cfg->ig_pmac.def_pmac_hdr[5];

			if (pmac_cfg->ig_pmac_flags & IG_PMAC_F_PMACHDR7)
				igcfg.defPmacHdr[6] =
				    pmac_cfg->ig_pmac.def_pmac_hdr[6];

			if (pmac_cfg->ig_pmac_flags & IG_PMAC_F_PMACHDR8)
				igcfg.defPmacHdr[7] =
				    pmac_cfg->ig_pmac.def_pmac_hdr[7];

			DP_DEBUG(DP_DBG_FLAG_DBG,
				 "\nPMAC %d igcfg configuration:\n", port);
			DP_DEBUG(DP_DBG_FLAG_DBG, "igcfg.nTxDmaChanId=%d\n",
				 igcfg.nTxDmaChanId);
			DP_DEBUG(DP_DBG_FLAG_DBG, "igcfg.bErrPktsDisc=%d\n",
				 igcfg.bErrPktsDisc);
			DP_DEBUG(DP_DBG_FLAG_DBG, "igcfg.bPmapDefault=%d\n",
				 igcfg.bPmapDefault);
			DP_DEBUG(DP_DBG_FLAG_DBG, "igcfg.bPmapEna=%d\n",
				 igcfg.bPmapEna);
			DP_DEBUG(DP_DBG_FLAG_DBG, "igcfg.bClassDefault=%d\n",
				 igcfg.bClassDefault);
			DP_DEBUG(DP_DBG_FLAG_DBG, "igcfg.bClassEna=%d\n",
				 igcfg.bClassEna);
			DP_DEBUG(DP_DBG_FLAG_DBG, "igcfg.bSubIdDefault=%d\n",
				 igcfg.bSubIdDefault);
			DP_DEBUG(DP_DBG_FLAG_DBG, "igcfg.bSpIdDefault=%d\n",
				 igcfg.bSpIdDefault);
			DP_DEBUG(DP_DBG_FLAG_DBG, "igcfg.bPmacPresent=%d\n",
				 igcfg.bPmacPresent);
			DP_DEBUG(DP_DBG_FLAG_DBG, "igcfg.defPmacHdr=");

			for (k = 0;
			     k <
			     sizeof(igcfg.defPmacHdr) /
			     sizeof(igcfg.defPmacHdr[0]); k++)
				DP_DEBUG(DP_DBG_FLAG_DBG, "0x%x ",
					 igcfg.defPmacHdr[k]);

			DP_DEBUG(DP_DBG_FLAG_DBG, "\n");

			gsw_api_kioctl(gswr_r, GSW_PMAC_IG_CFG_SET,
				       (u32) &igcfg);
		}

		if (dqport.deq_info)
			kfree(dqport.deq_info);
	}

	/*set egress port via pmac port id */
	if (!pmac_cfg->eg_pmac_flags)
		return 0;

	for (i = 0; i <= 15; i++) {	/*traffic class */
		for (j = 0; j <= 3; j++) {	/* flow */
			/*read back egcfg first from gsw */
			memset(&egcfg, 0, sizeof(GSW_PMAC_Eg_Cfg_t));
			egcfg.nDestPortId = port;
			egcfg.nTrafficClass = i;
			egcfg.nFlowIDMsb = j;

			memset(&pmac_glb, 0, sizeof(pmac_glb));
			gsw_api_kioctl(gswr_r, GSW_PMAC_GLBL_CFG_GET,
				       (u32) &pmac_glb);
			egcfg.bProcFlagsSelect = pmac_glb.bProcFlagsEgCfgEna;
			DP_DEBUG(DP_DBG_FLAG_DBG, "bProcFlagsSelect=%u\n",
				 egcfg.bProcFlagsSelect);

			/*update egcfg and write back to gsw */
			if (pmac_cfg->eg_pmac_flags & EG_PMAC_F_FCS)
				egcfg.bFcsEna = pmac_cfg->eg_pmac.fcs;

			if (pmac_cfg->eg_pmac_flags & EG_PMAC_F_L2HDR_RM) {
				egcfg.bRemL2Hdr = pmac_cfg->eg_pmac.rm_l2hdr;
				egcfg.numBytesRem =
				    pmac_cfg->eg_pmac.num_l2hdr_bytes_rm;
			}

			if (pmac_cfg->eg_pmac_flags & EG_PMAC_F_PMAC)
				egcfg.bPmacEna = pmac_cfg->eg_pmac.pmac;

			if (pmac_cfg->eg_pmac_flags & EG_PMAC_F_RXID)
				egcfg.nRxDmaChanId =
				    pmac_cfg->eg_pmac.rx_dma_chan;

			if (pmac_cfg->eg_pmac_flags & EG_PMAC_F_RESDW1)
				egcfg.nResDW1 = pmac_cfg->eg_pmac.res_dw1;

			if (pmac_cfg->eg_pmac_flags & EG_PMAC_F_RES1DW0)
				egcfg.nRes1DW0 = pmac_cfg->eg_pmac.res1_dw0;

			if (pmac_cfg->eg_pmac_flags & EG_PMAC_F_RES2DW0)
				egcfg.nRes2DW0 = pmac_cfg->eg_pmac.res2_dw0;

			/*if (pmac_cfg->eg_pmac_flags & EG_PMAC_F_TCENA)
			*  egcfg.bTCEnable = pmac_cfg->eg_pmac.tc_enable;
			*/

			if (pmac_cfg->eg_pmac_flags & EG_PMAC_F_DECFLG)
				egcfg.bDecFlag = pmac_cfg->eg_pmac.dec_flag;

			if (pmac_cfg->eg_pmac_flags & EG_PMAC_F_ENCFLG)
				egcfg.bEncFlag = pmac_cfg->eg_pmac.enc_flag;

			if (pmac_cfg->eg_pmac_flags & EG_PMAC_F_MPE1FLG)
				egcfg.bMpe1Flag = pmac_cfg->eg_pmac.mpe1_flag;

			if (pmac_cfg->eg_pmac_flags & EG_PMAC_F_MPE2FLG)
				egcfg.bMpe2Flag = pmac_cfg->eg_pmac.mpe2_flag;
#if defined(CONFIG_LTQ_DATAPATH_DBG) && CONFIG_LTQ_DATAPATH_DBG
			if (dp_dbg_flag) {
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "\nPMAC %d egcfg configuration:\n",
					 port);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "egcfg.nRxDmaChanId=%d\n",
					 egcfg.nRxDmaChanId);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "egcfg.bRemL2Hdr=%d\n",
					 egcfg.bRemL2Hdr);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "egcfg.numBytesRem=%d\n",
					 egcfg.numBytesRem);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "egcfg.bFcsEna=%d\n", egcfg.bFcsEna);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "egcfg.bPmacEna=%d\n",
					 egcfg.bPmacEna);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "egcfg.nResDW1=%d\n", egcfg.nResDW1);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "egcfg.nRes1DW0=%d\n",
					 egcfg.nRes1DW0);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "egcfg.nRes2DW0=%d\n",
					 egcfg.nRes2DW0);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "egcfg.nDestPortId=%d\n",
					 egcfg.nDestPortId);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "egcfg.bTCEnable=%d\n",
					 egcfg.bTCEnable);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "egcfg.nTrafficClass=%d\n",
					 egcfg.nTrafficClass);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "egcfg.nFlowIDMsb=%d\n",
					 egcfg.nFlowIDMsb);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "egcfg.bDecFlag=%d\n",
					 egcfg.bDecFlag);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "egcfg.bEncFlag=%d\n",
					 egcfg.bEncFlag);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "egcfg.bMpe1Flag=%d\n",
					 egcfg.bMpe1Flag);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "egcfg.bMpe2Flag=%d\n",
					 egcfg.bMpe2Flag);
			}
#endif
			gsw_api_kioctl(gswr_r, GSW_PMAC_EG_CFG_SET,
				       (u32) &egcfg);
			;
		}
	}

	return 0;
}
EXPORT_SYMBOL(dp_pmac_set);

/*flag: bit 0 for cpu
		bit 1 for mpe1,bit 2 for mpe2, bit 3 for mpe3;
*/
#define GSW_L_BASE_ADDR        (0xBC000000)
#define GSW_R_BASE_ADDR        (0xBA000000)
#define FDMA_PASR_ADDR         (0xA47)

void dp_set_gsw_parser(uint8_t flag, uint8_t cpu, uint8_t mpe1, uint8_t mpe2,
		       uint8_t mpe3)
{
	/*later it should use gsw_api_kioctl(gswl, GSW_CPU_PORT_CFG_SET,..) */
	u32 v = REG32(GSW_R_BASE_ADDR + FDMA_PASR_ADDR * 4);

	DP_DEBUG(DP_DBG_FLAG_DBG,
		 "Old FDMA_PASR register (0x%x) value: 0x%x\n",
		 FDMA_PASR_ADDR, v);
	DP_DEBUG(DP_DBG_FLAG_DBG, "flag=0x%x cpu=%d mpe1/2/3=%d/%d/%d\n",
		 flag, cpu, mpe1, mpe2, mpe3);

	if (flag & F_MPE_NONE)
		v = (v & ~(0x3 << 0)) | (cpu << 0);

	if (flag & F_MPE1_ONLY)
		v = (v & ~(0x3 << 2)) | (mpe1 << 2);

	if (flag & F_MPE2_ONLY)
		v = (v & ~(0x3 << 4)) | (mpe2 << 4);

	if (flag & F_MPE1_MPE2)
		v = (v & ~(0x3 << 6)) | (mpe3 << 6);

	REG32(GSW_R_BASE_ADDR + FDMA_PASR_ADDR * 4) = v;
	DP_DEBUG(DP_DBG_FLAG_DBG,
		 "New FDMA_PASR register (0x%x) value: 0x%x\n",
		 FDMA_PASR_ADDR, v);

	dp_parser_info_refresh(v, 0);
}
EXPORT_SYMBOL(dp_set_gsw_parser);

void dp_get_gsw_parser(uint8_t *cpu, uint8_t *mpe1, uint8_t *mpe2,
		       uint8_t *mpe3)
{
	u32 v = REG32(GSW_R_BASE_ADDR + FDMA_PASR_ADDR * 4);

	DP_DEBUG(DP_DBG_FLAG_DBG, "FDMA_PASR register (0x%x) value: 0x%x\n",
		 FDMA_PASR_ADDR, v);
	dp_parser_info_refresh(v, 1);

	if (cpu) {
		*cpu = (v >> 0) & 0x3;
		DP_DEBUG(DP_DBG_FLAG_DBG, "  cpu=%d\n", *cpu);
	}

	if (mpe1) {
		*mpe1 = (v >> 2) & 0x3;
		DP_DEBUG(DP_DBG_FLAG_DBG, "  mpe1=%d\n", *mpe1);
	}

	if (mpe2) {
		*mpe2 = (v >> 4) & 0x3;
		DP_DEBUG(DP_DBG_FLAG_DBG, "  mpe2=%d\n", *mpe2);
	}

	if (mpe3) {
		*mpe3 = (v >> 6) & 0x3;
		DP_DEBUG(DP_DBG_FLAG_DBG, "  mpe3=%d\n", *mpe3);
	}
}
EXPORT_SYMBOL(dp_get_gsw_parser);

char *parser_flag_str(uint8_t f)
{
	if (f == DP_PARSER_F_DISABLE)
		return "No Parser";
	else if (f == DP_PARSER_F_HDR_ENABLE)
		return "Parser Flag only";
	else if (f == DP_PARSER_F_HDR_OFFSETS_ENABLE)
		return "Parser Full";
	else
		return "Reserved";
}

int gsw_mib_reset(int dev, u32 flag)
{
	GSW_API_HANDLE gsw_handle = 0;
	char *dev_name;
	int ret;
	GSW_RMON_clear_t rmon_clear;

	if (dev == 0)
		dev_name = GSWIP_L_DEV_NAME;

	else
		dev_name = GSWIP_R_DEV_NAME;

	gsw_handle = gsw_api_kopen(dev_name);

	if (gsw_handle == 0) {
		PR_ERR("Open %s FAILED !!\n", dev_name);
		return -1;
	}

	rmon_clear.eRmonType = GSW_RMON_ALL_TYPE;
	ret = gsw_api_kioctl(gsw_handle, GSW_RMON_CLEAR, (u32) &rmon_clear);

	if (ret != GSW_statusOk) {
		PR_ERR("R:GSW_RMON_CLEAR failed for GSW_RMON_ALL_TYPE\n");
		return -1;
	}
#if 0

	for (i = 0; i < portn; i++) {
		rmon_clear.eRmonType = GSW_RMON_PORT_TYPE;
		rmon_clear.nRmonId = i;
		ret =
		    gsw_api_kioctl(gsw_handle, GSW_RMON_CLEAR,
				   (u32) &rmon_clear);

		if (ret != GSW_statusOk) {
			PR_ERR("GSW_RMON_CLEAR failed:GSW_RMON_PORT_TYPE\n");
			return -1;
		}
	}

	GSW_RMON_ALL_TYPE rmon_clear.eRmonType = GSW_RMON_REDIRECT_TYPE;
	ret = gsw_api_kioctl(gsw_handle, GSW_RMON_CLEAR, (u32) &rmon_clear);

	if (ret != GSW_statusOk) {
		PR_ERR
		    ("R:GSW_RMON_CLEAR failed for GSW_RMON_REDIRECT_TYPE\n");
		return -1;
	}
#endif
	gsw_api_kclose(gsw_handle);
	return ret;
}
EXPORT_SYMBOL(gsw_mib_reset);
