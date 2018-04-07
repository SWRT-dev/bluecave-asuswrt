#include <linux/netdevice.h>
#include <linux/types.h>
#include <switch-api/lantiq_ethsw_api.h>

#include "stat_helper_platform.h"

void _stat_helper_get_switch_rmon_counters(int8_t *switch_dev, uint8_t port, stat_helper_dev_stats64_t *stats)
{
	LTQ_ETHSW_API_HANDLE switch_api_fd;
	IFX_ETHSW_RMON_cnt_t param;

	memset(&param, 0, sizeof(IFX_ETHSW_RMON_cnt_t));

	switch_api_fd = ltq_ethsw_api_kopen(switch_dev);

	param.nPortId = port;
	ltq_ethsw_api_kioctl(switch_api_fd, IFX_ETHSW_RMON_GET, (unsigned int)&param);

	stats->rx_packets += (uint64_t)param.nRxGoodPkts;
	stats->tx_packets += (uint64_t)param.nTxGoodPkts;
	stats->rx_bytes += param.nRxGoodBytes;
	stats->tx_bytes += param.nTxGoodBytes;
	stats->rx_errors += param.nRxFCSErrorPkts + param.nRxUnderSizeErrorPkts + param.nRxOversizeErrorPkts + param.nRxAlignErrorPkts;
	stats->rx_dropped += (uint64_t)param.nRxDroppedPkts;
	stats->tx_dropped += (uint64_t)param.nTxDroppedPkts;
	stats->multicast += (uint64_t)param.nRxMulticastPkts;

	ltq_ethsw_api_kclose(switch_api_fd);
}
