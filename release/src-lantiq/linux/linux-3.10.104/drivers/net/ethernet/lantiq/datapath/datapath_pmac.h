#ifndef DATAPATH_PMAC_H_XXX
#define DATAPATH_PMAC_H_XXX

union gsw_var {
	GSW_register_t reg;
	GSW_QoS_meterCfg_t meter_cfg;
	GSW_QoS_meterPort_t meter_port;
	GSW_QoS_WRED_PortCfg_t wred_p;
	GSW_QoS_WRED_QueueCfg_t wred_q;
	GSW_QoS_WRED_Cfg_t wred_cfg;
	GSW_RMON_clear_t rmon;
	GSW_portCfg_t port_cfg;
	GSW_PCE_rule_t pce;
};
int dp_gsw_kioctl(char *dev_name, u32 command, u32 arg);

#endif
