#include <xway/switch-api/lantiq_gsw_api.h>

#include "directconnect_dp.h"

int32_t
_dc_dp_gsw_pmac_init(int32_t port_id, int32_t dma_cid)
{
	u8 i = 0, j = 0;
	GSW_PMAC_Eg_Cfg_t egCfg;
	GSW_PMAC_Ig_Cfg_t igCfg;
	GSW_register_t regCfg;
	GSW_API_HANDLE gswr;

	memset((void *)&egCfg, 0x00, sizeof(egCfg));
	memset((void *)&igCfg, 0x00, sizeof(igCfg));

	/* Do the GSW-R configuration */
	gswr = gsw_api_kopen("/dev/switch_api/1");
	if (gswr == 0) {
		DC_DP_ERROR("Open SWAPI device FAILED!!!\n");
		return -EIO;
	}

	/* GSWIP-R PMAC Egress Configuration Table */
	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "PMAC_EG_CFG_SET for GSW-R.\n");
	for (i = 0; i <= 15; i++) {
		for (j = 0; j <= 3; j++) {
			egCfg.nRxDmaChanId  = 0;
			egCfg.bPmacEna      = 0;
			egCfg.bFcsEna       = 0;
			egCfg.bRemL2Hdr     = 0;
			egCfg.numBytesRem   = 0;
			egCfg.nResDW1       = 0;
			egCfg.nRes1DW0      = 0;
			egCfg.nRes2DW0      = 0;
			egCfg.nDestPortId   = port_id;
			egCfg.nTrafficClass = i;
			egCfg.bMpe1Flag     = 0;
			egCfg.bMpe2Flag     = 0;
			egCfg.bEncFlag      = 0;
			egCfg.bDecFlag      = 0;
			egCfg.nFlowIDMsb    = j;
			egCfg.bTCEnable	    = 1;

			gsw_api_kioctl(gswr, GSW_PMAC_EG_CFG_SET, (unsigned int)&egCfg);		
		}
	}

	/* GSWIP-R PMAC Ingress Configuration Table */
	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "PMAC_IG_CFG_SET for GSW-R.\n");
	igCfg.nTxDmaChanId  = dma_cid;
	igCfg.bPmacPresent  = 0;
	igCfg.bSpIdDefault  = 1; 
	igCfg.bSubIdDefault = 0;
	igCfg.bClassDefault = 0; 
	igCfg.bClassEna     = 0; 
	igCfg.bErrPktsDisc  = 1; 

	igCfg.bPmapDefault  = 1;
	igCfg.bPmapEna      = 1;

	igCfg.defPmacHdr[0] = 0;
	igCfg.defPmacHdr[1] = 0;
	igCfg.defPmacHdr[2] = port_id << 4;
	igCfg.defPmacHdr[3] = 0x80;
	igCfg.defPmacHdr[4] = 0;
	igCfg.defPmacHdr[5] = 0;
	igCfg.defPmacHdr[6] = 0xFF;
	igCfg.defPmacHdr[7] = 0xFF;

	gsw_api_kioctl(gswr, GSW_PMAC_IG_CFG_SET, (unsigned int)&igCfg);

	/* Allow traffic from one VAP to any VAP */

	/* PCE_PCTRL_3 */
	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0x483 + (10 * port_id);
	gsw_api_kioctl(gswr, GSW_REGISTER_GET, (unsigned int)&regCfg);
	regCfg.nData |= 0x4000;
	gsw_api_kioctl(gswr, GSW_REGISTER_SET, (unsigned int)&regCfg);

	/* PCE_IGPTRM */
	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0x544 + (16 * port_id);
	gsw_api_kioctl(gswr, GSW_REGISTER_GET, (unsigned int)&regCfg);
	regCfg.nData |= 0xFFFF;
	gsw_api_kioctl(gswr, GSW_REGISTER_SET, (unsigned int)&regCfg);

	gsw_api_kclose(gswr);

	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "GSW PMAC Init Done.\n");
	return 0;
}
