/******************************************************************************
**
** FILE NAME	: grx500_rcu.c
** PROJECT		: grx500 driver
** MODULES	   : RCU (Reset Control Unit)
**
** DATE	: 17 Sept 2014
** AUTHOR		: Jason Deng Yuhong
** DESCRIPTION	: RCU driver common source file
**
** HISTORY
** $Date		$Author		$Comment
******************************************************************************/

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/kallsyms.h>
#include <linux/delay.h>

/*
 *	Chip Specific Head File
 */
#include <lantiq_soc.h>
#include "ltq_rcu.h"

/*
 * ####################################
 *			RCU	Version No.
 *
 * LTQ_RCU_VER	 MAJOR:MID:MINOR
 * ####################################
 */

#define LTQ_RCU_VER_MAJOR				1
#define LTQ_RCU_VER_MID					1
#define LTQ_RCU_VER_MINOR				2

#define LTQ_RCU_MAJOR					241


/*
 * ####################################
 *			  Global Variable
 * ####################################
 */
/*
* must be locked before g_rcu_register_lock locked
* and be released after g_rcu_register_lock released
*/
spinlock_t g_rcu_lock;
/*
use spinlock rather than semaphore or mutex
because most functions run in user context
and they do not take much time to finish operation
*/
spinlock_t g_rcu_register_lock;
static LTQ_RCU_DECLARE_DOMAIN_NAME(g_rcu_domain_name);

/* static LTQ_RCU_DECLARE_MODULE_NAME(g_rcu_module_name); */

/* remapped base addr of the reset control unit */
static void __iomem *rcu_membase;
/* static struct device_node *ltq_rcu_np; */

static struct ltq_rcu_domain_t g_rcu_domains[LTQ_RCU_DOMAIN_MAX] = {
{
/* LTQ_RCU_DOMAIN_HRST */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_HRST,
.rst_req_value		= 1 << 0,
.rst_req_mask		= 1 << 0,
.rst_stat_mask		= 1 << 0,
.latch				= 1,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_TEP */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_TEP,
.rst_req_value		= 1 << 1,
.rst_req_mask		= 1 << 1,
.rst_stat_mask		= 1 << 1,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_CPU_CLUSTER_RST */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_CPU_CLUSTER_RST,
.rst_req_value		= 1 << 2,
.rst_req_mask		= 1 << 2,
.rst_stat_mask		= 1 << 2,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_CPU_CLUSTER_RLS */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_CPU_CLUSTER_RLS,
.rst_req_value		= 1 << 3,
.rst_req_mask		= 1 << 3,
.rst_stat_mask		= 1 << 3,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_HRST_CFG */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_HRST_CFG,
.rst_req_value		= 1 << 5,
.rst_req_mask		= 1 << 5,
.rst_stat_mask		= 1 << 5,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_CBM */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_CBM,
.rst_req_value		= 1 << 7,
.rst_req_mask		= 1 << 7,
.rst_stat_mask		= 1 << 7,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_PCIE_PHY3 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_PCIE_PHY3,
.rst_req_value		= 1 << 10,
.rst_req_mask		= 1 << 10,
.rst_stat_mask		= 1 << 10,
.latch				= 1, /* software will clear the reset bit */
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_PCIE_PHY1 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_PCIE_PHY1,
.rst_req_value		= 1 << 12,
.rst_req_mask		= 1 << 12,
.rst_stat_mask		= 1 << 24,
.latch				= 1,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_PCIE_PHY2 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_PCIE_PHY2,
.rst_req_value		= 1 << 13,
.rst_req_mask		= 1 << 13,
.rst_stat_mask		= 1 << 11,
.latch				= 1,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_XBAR6 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_XBAR6,
.rst_req_value		= 1 << 14,
.rst_req_mask		= 1 << 14,
.rst_stat_mask		= 1 << 14,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_GSWIP_L */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_GSWIP_L,
.rst_req_value		= 1 << 21,
.rst_req_mask		= 1 << 21,
.rst_stat_mask		= 1 << 16,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_WLAN */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_WLAN,
.rst_req_value		= 1 << 24,
.rst_req_mask		= 1 << 24,
.rst_stat_mask		= 1 << 21,
.latch				= 0,
.udelay				= 0,/* should we take some delay here */
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_PHY5 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_PHY5,
.rst_req_value		= 1 << 25,
.rst_req_mask		= 1 << 25,
.rst_stat_mask		= 1 << 25,
.latch				= 1,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_PHY4 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_PHY4,
.rst_req_value		= 1 << 26,
.rst_req_mask		= 1 << 26,
.rst_stat_mask		= 1 << 22,
.latch				= 1,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_PHY3 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_PHY3,
.rst_req_value		= 1 << 28,
.rst_req_mask		= 1 << 28,
.rst_stat_mask		= 1 << 23,
.latch				= 1,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_PHY2 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_PHY2,
.rst_req_value		= 1 << 29,
.rst_req_mask		= 1 << 29,
.rst_stat_mask		= 1 << 28,
.latch				= 1,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_GLB_SW */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_GLB_SW,
.rst_req_value		= 1 << 30,
.rst_req_mask		= 1 << 30,
.rst_stat_mask		= 1 << 29,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_PHY6F */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << LTQ_RCU_DOMAIN_PHY6F,
.rst_req_value		= 1 << 31,
.rst_req_mask		= 1 << 31,
.rst_stat_mask		= 1 << 30,
.latch				= 1,
.udelay				= 0,
.handlers			= NULL,
},
{
/* dummy : 18 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << 0,
.rst_req_value		= 1 << 0,
.rst_req_mask		= 1 << 0,
.rst_stat_mask		= 1 << 0,
.latch				= 0,
.udelay 			= 0,
.handlers			= NULL,
},
{
/* dummy : 19 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << 0,
.rst_req_value		= 1 << 0,
.rst_req_mask		= 1 << 0,
.rst_stat_mask		= 1 << 0,
.latch				= 0,
.udelay 			= 0,
.handlers			= NULL,
},
{
/* dummy : 20 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << 0,
.rst_req_value		= 1 << 0,
.rst_req_mask		= 1 << 0,
.rst_stat_mask		= 1 << 0,
.latch				= 0,
.udelay 			= 0,
.handlers			= NULL,
},
{
/* dummy : 21 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << 0,
.rst_req_value		= 1 << 0,
.rst_req_mask		= 1 << 0,
.rst_stat_mask		= 1 << 0,
.latch				= 0,
.udelay 			= 0,
.handlers			= NULL,
},
{
/* dummy : 22 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << 0,
.rst_req_value		= 1 << 0,
.rst_req_mask		= 1 << 0,
.rst_stat_mask		= 1 << 0,
.latch				= 0,
.udelay 			= 0,
.handlers			= NULL,
},
{
/* dummy : 23 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << 0,
.rst_req_value		= 1 << 0,
.rst_req_mask		= 1 << 0,
.rst_stat_mask		= 1 << 0,
.latch				= 0,
.udelay 			= 0,
.handlers			= NULL,
},
{
/* dummy : 24 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << 0,
.rst_req_value		= 1 << 0,
.rst_req_mask		= 1 << 0,
.rst_stat_mask		= 1 << 0,
.latch				= 0,
.udelay 			= 0,
.handlers			= NULL,
},
{
/* dummy : 25 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << 0,
.rst_req_value		= 1 << 0,
.rst_req_mask		= 1 << 0,
.rst_stat_mask		= 1 << 0,
.latch				= 0,
.udelay 			= 0,
.handlers			= NULL,
},
{
/* dummy : 26 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << 0,
.rst_req_value		= 1 << 0,
.rst_req_mask		= 1 << 0,
.rst_stat_mask		= 1 << 0,
.latch				= 0,
.udelay 			= 0,
.handlers			= NULL,
},
{
/* dummy : 27 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << 0,
.rst_req_value		= 1 << 0,
.rst_req_mask		= 1 << 0,
.rst_stat_mask		= 1 << 0,
.latch				= 0,
.udelay 			= 0,
.handlers			= NULL,
},
{
/* dummy : 28 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << 0,
.rst_req_value		= 1 << 0,
.rst_req_mask		= 1 << 0,
.rst_stat_mask		= 1 << 0,
.latch				= 0,
.udelay 			= 0,
.handlers			= NULL,
},
{
/* dummy : 29 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << 0,
.rst_req_value		= 1 << 0,
.rst_req_mask		= 1 << 0,
.rst_stat_mask		= 1 << 0,
.latch				= 0,
.udelay 			= 0,
.handlers			= NULL,
},
{
/* dummy : 30 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << 0,
.rst_req_value		= 1 << 0,
.rst_req_mask		= 1 << 0,
.rst_stat_mask		= 1 << 0,
.latch				= 0,
.udelay 			= 0,
.handlers			= NULL,
},
{
/* dummy : 31 */
.regblock			= RCU_REG_BLK0,
.affected_domains	= 1 << 0,
.rst_req_value		= 1 << 0,
.rst_req_mask		= 1 << 0,
.rst_stat_mask		= 1 << 0,
.latch				= 0,
.udelay 			= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_VCODEC */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_VCODEC - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 0,
.rst_req_mask		= 1 << 0,
.rst_stat_mask		= 1 << 0,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_OCP2SRAM */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_OCP2SRAM - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 1,
.rst_req_mask		= 1 << 1,
.rst_stat_mask		= 1 << 1,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_XBAR7 */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_XBAR7 - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 2,
.rst_req_mask		= 1 << 2,
.rst_stat_mask		= 1 << 2,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_SL40 */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_SL40 - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 3,
.rst_req_mask		= 1 << 3,
.rst_stat_mask		= 1 << 3,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_USB_PHY0 */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_USB_PHY0 - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 4,
.rst_req_mask		= 1 << 4,
.rst_stat_mask		= 1 << 4,
.latch				= 1,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_USB_PHY1 */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_USB_PHY1 - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 5,
.rst_req_mask		= 1 << 5,
.rst_stat_mask		= 1 << 5,
.latch				= 1,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_SR_PHYF */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_SR_PHYF - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 6,
.rst_req_mask		= 1 << 6,
.rst_stat_mask		= 1 << 6,
.latch				= 1,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_SR_PHY2 */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_SR_PHY2 - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 7,
.rst_req_mask		= 1 << 7,
.rst_stat_mask		= 1 << 7,
.latch				= 1,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_SR_PHY3 */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_SR_PHY3 - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 8,
.rst_req_mask		= 1 << 8,
.rst_stat_mask		= 1 << 8,
.latch				= 1,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_TEMPS */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_TEMPS - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 12,
.rst_req_mask		= 1 << 12,
.rst_stat_mask		= 1 << 11,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_SR_PHY5 */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_SR_PHY5 - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 15,
.rst_req_mask		= 1 << 15,
.rst_stat_mask		= 1 << 24,
.latch				= 1,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_SR_PHY4 */
.regblock			= RCU_REG_BLK1,
.affected_domains	= 1 << (LTQ_RCU_DOMAIN_SR_PHY4-LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 16,
.rst_req_mask		= 1 << 16,
.rst_stat_mask		= 1 << 23,
.latch				= 1,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_XBAR3 */
.regblock			= RCU_REG_BLK1,
.affected_domains	= 1 << (LTQ_RCU_DOMAIN_XBAR3-LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 18,
.rst_req_mask		= 1 << 18,
.rst_stat_mask		= 1 << 18,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_XBAR2 */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_XBAR2 - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 19,
.rst_req_mask		= 1 << 19,
.rst_stat_mask		= 1 << 19,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_XBAR1 */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_XBAR1 - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 20,
.rst_req_mask		= 1 << 20,
.rst_stat_mask		= 1 << 20,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_XBAR0 */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_XBAR0 - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 21,
.rst_req_mask		= 1 << 21,
.rst_stat_mask		= 1 << 21,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_GSWIP_R */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_GSWIP_R - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 22,
.rst_req_mask		= 1 << 22,
.rst_stat_mask		= 1 << 12,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_E123_ABORT_REQ */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_E123_ABORT_REQ - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 23,
.rst_req_mask		= 1 << 23,
.rst_stat_mask		= 1 << 16,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_E123_ABORT_ACK */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_E123_ABORT_ACK - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 24,
.rst_req_mask		= 1 << 24,
.rst_stat_mask		= 1 << 21,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_GPHY_CDB */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_GPHY_CDB - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 28,
.rst_req_mask		= 1 << 28,
.rst_stat_mask		= 1 << 28,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_DDR_CTL */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_DDR_CTL - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 29,
.rst_req_mask		= 1 << 29,
.rst_stat_mask		= 1 << 29,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_DDR_PUB */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_DDR_PUB - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 30,
.rst_req_mask		= 1 << 30,
.rst_stat_mask		= 1 << 30,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
{
/* LTQ_RCU_DOMAIN_DDR_PHY */
.regblock			= RCU_REG_BLK1,
.affected_domains = 1 << (LTQ_RCU_DOMAIN_DDR_PHY - LTQ_RCU_DOMAIN_HALF),
.rst_req_value		= 1 << 31,
.rst_req_mask		= 1 << 31,
.rst_stat_mask		= 1 << 31,
.latch				= 0,
.udelay				= 0,
.handlers			= NULL,
},
};
/*
 *	File Operations
 */
static int ltq_rcu_open(struct inode *inode, struct file *filep);
static int ltq_rcu_release(struct inode *inode, struct file *filelp);
static long ltq_rcu_ioctl(struct file *filp,
					unsigned int cmd,
					unsigned long arg);

static const struct file_operations g_rcu_fops = {
	.open			= ltq_rcu_open,
	.release		= ltq_rcu_release,
	.unlocked_ioctl = ltq_rcu_ioctl
};

/*
 *	Init/exit Functions
 */
static inline int ltq_rcu_version(char *);
static int __init ltq_rcu_pre_init(void);
static int ltq_rcu_probe(struct platform_device *pdev);

/*
 *	Register access function declaration
 */
void rcu_w32(uint32_t val, uint32_t reg_off);
uint32_t rcu_r32(uint32_t reg_off);
void rcu_w32_mask(uint32_t clr, uint32_t set, uint32_t reg_off);


/*
 * ####################################
 *			 RCU Register access APIs
 * ####################################
 */

void rcu_w32(uint32_t val, uint32_t reg_off)
{
	ltq_w32(val, rcu_membase + reg_off);
}

uint32_t rcu_r32(uint32_t reg_off)
{
	return ltq_r32(rcu_membase + reg_off);
}


void rcu_w32_mask(uint32_t clr, uint32_t set, uint32_t reg_off)
{
	rcu_w32((rcu_r32(reg_off) & ~(clr)) | (set), reg_off);
}


/*
 * ####################################
 *			 RCU Kernal APIs
 * ####################################
 */


/*!
\fn		int ltq_rcu_request(unsigned int reset_domain_id,
					unsigned int module_id,
					ltq_rcu_callbackfn callbackfn,
					unsigned long arg)
\brief	  Register ownership of one hardware module.

		User uses this function to register ownership of
		one hardware module and register
		callback function to receive reset event.
		When reset is triggered by other component,
		one pre-reset event is received through callback
		function before reset, and one post-reset event
		is received through callback function after reset.

\param	reset_domain_id	-	unsigned int, hardware module ID
\param	module_id	-	unsigned int, driver/component module ID
\param	callbackfn	-	ltq_rcu_callbackfn, callback function
							to receive event
\param	arg - unsigned long, parameter of callback function
\return   0		Register successfully.
\return   negative	 Register fail.
\ingroup  LTQ_RCU_API
 */
int ltq_rcu_request(unsigned int reset_domain_id,
					unsigned int module_id,
					ltq_rcu_callbackfn callbackfn,
					unsigned long arg)
{
	unsigned long sys_flags;
	struct ltq_rcu_handler_t *p_handler;
	struct ltq_rcu_handler_t *p_cur;

	if (reset_domain_id >= NUM_ENTITY(g_rcu_domains)
	|| module_id >= LTQ_RCU_MODULE_MAX || callbackfn == NULL)
		return -EINVAL;

	p_handler = (struct ltq_rcu_handler_t *)kmalloc(sizeof(*p_handler),
							GFP_KERNEL);
	if (p_handler == NULL)
		return -ENOMEM;

	p_handler->fn		= callbackfn;
	p_handler->arg		= arg;
	p_handler->module_id	= module_id;

	spin_lock_irqsave(&g_rcu_lock, sys_flags);
	for (p_cur = g_rcu_domains[reset_domain_id].handlers;
		p_cur != NULL;
		p_cur = p_cur->next)
		if (p_cur->module_id == module_id)
			break;
	if (p_cur != NULL) {
		p_cur->fn	= callbackfn;
		p_cur->arg	= arg;
	} else {
		p_handler->next = g_rcu_domains[reset_domain_id].handlers;
		g_rcu_domains[reset_domain_id].handlers = p_handler;
	}
	spin_unlock_irqrestore(&g_rcu_lock, sys_flags);

	if (p_cur != NULL)
		kfree(p_handler);

	return 0;
}
EXPORT_SYMBOL(ltq_rcu_request);

/*!
\fn		int ltq_rcu_free(unsigned int reset_domain_id,
			unsigned int module_id)
\brief	Release ownership of the reset domain for one hardware module.

		User uses this function to release ownership of one hardware module.
		Driver will not notify this component for reset event
		if this function return successfully.

\param	reset_domain_id - unsigned int, hardware module ID
\param	module_id		- unsigned int, driver/component module ID
\return	0			   Release successfully.
\return	negative	   Release fail.
\ingroup LTQ_RCU_API
 */
int ltq_rcu_free(unsigned int reset_domain_id, unsigned int module_id)
{
	unsigned long sys_flags;
	struct ltq_rcu_handler_t *p_cur, *p_prev;

	if (reset_domain_id >= NUM_ENTITY(g_rcu_domains)
		|| module_id >= LTQ_RCU_MODULE_MAX)
		return -EINVAL;

	spin_lock_irqsave(&g_rcu_lock, sys_flags);
	for (p_prev = NULL, p_cur = g_rcu_domains[reset_domain_id].handlers;
		p_cur != NULL;
		p_prev = p_cur, p_cur = p_cur->next)
		if (p_cur->module_id == module_id) {
			if (p_prev != NULL)
				p_prev->next = p_cur->next;
			else
				g_rcu_domains[reset_domain_id].handlers = p_cur->next;
			break;
		}
	spin_unlock_irqrestore(&g_rcu_lock, sys_flags);

	if (p_cur != NULL) {
		kfree(p_cur);
		return 0;
	} else
		return -EIO;
}
EXPORT_SYMBOL(ltq_rcu_free);

/*!
\fn		int ltq_rcu_rst(unsigned int reset_domain_id,
			unsigned int module_id)
\brief	Trigger reset of one hardware module.

		User uses this function to trigger reset of one
		hardware module. Driver will give notification
		to all drivers/components registering for reset
		of this hardware module.

\param	reset_domain_id - unsigned int, hardware module ID
\param	module_id - unsigned int, driver/component module ID
\return	0			Reset successfully.
\return	negative	   Reset fail.
\ingroup	LTQ_RCU_API
 */
int ltq_rcu_rst(unsigned int reset_domain_id, unsigned int module_id)
{
	unsigned long sys_flags;
	unsigned int domain_id_bits;
	int domain_id;
	struct ltq_rcu_handler_t *p_cur;
	spin_lock_irqsave(&g_rcu_lock, sys_flags);
	/* trigger pre-reset event, each handler must be as fast as possible,
	because interrupt is disabled */
	domain_id_bits = g_rcu_domains[reset_domain_id].affected_domains;
	printk(KERN_ALERT "Prepare reset = 0x%x ; domain = 0x%x\n",
		domain_id_bits, reset_domain_id);
	while (domain_id_bits != 0) {
		domain_id = clz(domain_id_bits);
		domain_id_bits ^= 1 << domain_id;
		printk(KERN_ALERT "Just b4 issue reset & domain_id = %d\n",
			domain_id);
		for (p_cur = g_rcu_domains[domain_id].handlers;
			p_cur != NULL; p_cur = p_cur->next)
			if (p_cur->module_id != module_id)
				p_cur->fn(domain_id, module_id, 0, p_cur->arg);
	}
	/* issue reset */
	printk(KERN_ALERT "Ready to issue reset, let's go ...\n");
	ltq_rcu_rst_req_write(g_rcu_domains[reset_domain_id].rst_req_value,
		g_rcu_domains[reset_domain_id].regblock,
		g_rcu_domains[reset_domain_id].rst_req_mask
		);
	printk(KERN_ALERT "Just after issue reset & reset_domain_id = 0x%x\n",
		reset_domain_id);
	if (g_rcu_domains[reset_domain_id].latch) {
		udelay(g_rcu_domains[reset_domain_id].udelay);
		ltq_rcu_rst_req_write(
			~g_rcu_domains[reset_domain_id].rst_req_value,
			g_rcu_domains[reset_domain_id].regblock,
			g_rcu_domains[reset_domain_id].rst_req_mask
			);
	} else {
		int max_count = 1000;
		while ((ltq_rcu_rst_stat_read(g_rcu_domains[reset_domain_id].regblock)
			& g_rcu_domains[reset_domain_id].rst_stat_mask) && max_count--)
			;
		if (max_count < 0) {
			/* err("timeout during reset domain - %s",
				g_rcu_domain_name[reset_domain_id]); */
			printk(KERN_ALERT "timeout during reset domain - %s",
				g_rcu_domain_name[reset_domain_id]);
		}
	}
	/* trigger post-reset event, each handler must be as fast as possible,
	* because interrupt is disabled */
	domain_id_bits = g_rcu_domains[reset_domain_id].affected_domains;
	while (domain_id_bits != 0) {
		domain_id = clz(domain_id_bits);
		domain_id_bits ^= 1 << domain_id;
		for (p_cur = g_rcu_domains[domain_id].handlers;
			p_cur != NULL; p_cur = p_cur->next)
			if (p_cur->module_id != module_id)
				p_cur->fn(domain_id, module_id, 1, p_cur->arg);
	}
	spin_unlock_irqrestore(&g_rcu_lock, sys_flags);
	return 0;
}
EXPORT_SYMBOL(ltq_rcu_rst);

/*
 * ####################################
 *			  Local Function
 * ####################################
 */

static int ltq_rcu_open(struct inode *inode, struct file *filep)
{
	return 0;
}

static int ltq_rcu_release(struct inode *inode, struct file *filelp)
{
	return 0;
}

static long ltq_rcu_ioctl(struct file *filp,
	unsigned int cmd,
	unsigned long arg)
{
	int ret;

	/* check magic number */
	if (_IOC_TYPE(cmd) != LTQ_RCU_IOC_MAGIC)
		return -EINVAL;

	/* check read/write right */
	if (((_IOC_DIR(cmd) & _IOC_WRITE)
			&& !access_ok(VERIFY_READ, arg, _IOC_SIZE(cmd)))
		|| ((_IOC_DIR(cmd) & _IOC_READ)
			&& !access_ok(VERIFY_WRITE, arg, _IOC_SIZE(cmd))))
		return -EINVAL;

	switch (cmd) {
	case LTQ_RCU_IOC_VERSION:
		{
			struct ltq_rcu_ioctl_version version = {
				.major = LTQ_RCU_VER_MAJOR,
				.mid   = LTQ_RCU_VER_MID,
				.minor = LTQ_RCU_VER_MINOR
			};
			ret = copy_to_user((void *)arg,
					(void *)&version,
					sizeof(version)
					);
		}
		break;
	case LTQ_RCU_IOC_QUERY_RST_DOMAIN:
		{
			struct ltq_rcu_ioctl_query_rst_domain query = {0};
			ret = copy_from_user((void *)&query,
					(void *)arg,
					sizeof(query)
					);
			if (ret == 0) {
				if (query.domain_id < NUM_ENTITY(g_rcu_domains)) {
					query.f_reset = ltq_rcu_stat_get(query.domain_id);
					ret = copy_to_user((void *)arg,
							(void *)&query,
							sizeof(query)
							);
				} else
					ret = -EIO;
			}
		}
		break;
	default:
		ret = -ENOTSUPP;
	}

	return ret;
}

int ltq_rcu_stat_get(unsigned int reset_domain_id)
{
	if (reset_domain_id >= NUM_ENTITY(g_rcu_domains))
		return 0;

	return (ltq_rcu_rst_stat_read(g_rcu_domains[reset_domain_id].regblock)
		& g_rcu_domains[reset_domain_id].rst_stat_mask) ? 1 : 0;
}
EXPORT_SYMBOL(ltq_rcu_stat_get);


void ltq_rcu_stat_clear(unsigned int reset_domain_id)
{
	unsigned long sys_flags;
	if (reset_domain_id >= NUM_ENTITY(g_rcu_domains))
		return;

	spin_lock_irqsave(&g_rcu_register_lock, sys_flags);
	/* write 0 to reset bit */
	ltq_rcu_rst_req_write(
		~g_rcu_domains[reset_domain_id].rst_req_value,
		g_rcu_domains[reset_domain_id].regblock,
		g_rcu_domains[reset_domain_id].rst_req_mask
		);
	/* read status again */
	ltq_rcu_stat_get(reset_domain_id);
	spin_unlock_irqrestore(&g_rcu_register_lock, sys_flags);
}
EXPORT_SYMBOL(ltq_rcu_stat_clear);

unsigned int ltq_rcu_rst_stat_read(unsigned int regblk)
{
	unsigned long sys_flags;
	unsigned int ret = 0;
	spin_lock_irqsave(&g_rcu_register_lock, sys_flags);
	ret = rcu_r32(RCU_REG_STAT_OFFSET(regblk));
	spin_unlock_irqrestore(&g_rcu_register_lock, sys_flags);
	return ret;
}
EXPORT_SYMBOL(ltq_rcu_rst_stat_read);


unsigned int ltq_rcu_rst_req_read(unsigned int regblk)
{
	unsigned long sys_flags;
	unsigned int ret;

	spin_lock_irqsave(&g_rcu_register_lock, sys_flags);
	ret = rcu_r32(RCU_REG_RST_OFFSET(regblk));
	spin_unlock_irqrestore(&g_rcu_register_lock, sys_flags);

	return ret;
}
EXPORT_SYMBOL(ltq_rcu_rst_req_read);

void ltq_rcu_rst_req_write(unsigned int value,
	unsigned int regblk,
	unsigned int mask)
{
	unsigned long sys_flags;
    printk(KERN_ALERT "%s: value[0x%x] regblk[0x%x] mask[0x%x]\n", __func__, value, regblk, mask);
	spin_lock_irqsave(&g_rcu_register_lock, sys_flags);
	rcu_w32_mask(mask, value & mask, RCU_REG_RST_OFFSET(regblk));
	spin_unlock_irqrestore(&g_rcu_register_lock, sys_flags);
}
EXPORT_SYMBOL(ltq_rcu_rst_req_write);

static const struct of_device_id grx500_rcu_match[] = {
	{ .compatible = "lantiq,rcu-grx500" },	/* rcu-grx500 */
	{ .compatible = "lantiq,rcu-xrx200" },	/* rcu-xrx200 */
	{},
};
MODULE_DEVICE_TABLE(of, grx500_rcu_match);

static struct platform_driver grx500_rcu_driver = {
	.probe = ltq_rcu_probe,
	.driver = {
		/* .name = "rcu-grx500", */
		.name = "rcu-xrx200",
		.owner = THIS_MODULE,
		.of_match_table = grx500_rcu_match,
	},
};


static inline int ltq_rcu_version(char *buf)
{
	return sprintf(buf, "Lantiq CPE GRX500 RCU driver version %d.%d.%d\n",
		LTQ_RCU_VER_MAJOR, LTQ_RCU_VER_MID, LTQ_RCU_VER_MINOR);
}

static int __init ltq_rcu_pre_init(void)
{
	int i;

	spin_lock_init(&g_rcu_lock);
	spin_lock_init(&g_rcu_register_lock);

	for (i = 0; i < NUM_ENTITY(g_rcu_domains); i++)
		g_rcu_domains[i].handlers = NULL;

	return 0;
}


static int ltq_rcu_probe(struct platform_device *pdev)
{
	struct resource *res;
	int ret_val = -ENOMEM;

	printk(KERN_ALERT "Welcome in RCU probe ...\n");
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res == NULL) {
		dev_err(&pdev->dev, "fail to get platform resource!\n");
		ret_val = -ENOENT;
		return ret_val;
	}
	printk(KERN_ALERT "Got resource from %s for RCU\n", res->name);

	/* remap rcu register range */

	/* rcu_membase = devm_ioremap_resource(&pdev->dev, res); */
	if (devm_request_mem_region(&pdev->dev,
			res->start,
			resource_size(res),
			res->name) < 0)
			pr_err("Failed to request rcu memory");

		rcu_membase = devm_ioremap(&pdev->dev,
						res->start,
						resource_size(res));

	if (!rcu_membase) {
		dev_err(&pdev->dev, "failed to remap memory region\n");
		ret_val = -ENXIO;
		return ret_val;
	}
	printk(KERN_ALERT "Successfully got remapped RCU membase @ 0x%p\n",
			rcu_membase);
	printk(KERN_ALERT "Initial value: rst_stat = %x  rst_req = %x\n",
			ltq_rcu_rst_stat_read(RCU_REG_BLK0),
			ltq_rcu_rst_req_read(RCU_REG_BLK0)
			);
	printk(KERN_ALERT "Second read value: rst_stat = %x  rst_req = %x\n",
			rcu_r32(LTQ_RCU_RST_STAT),
			rcu_r32(LTQ_RCU_RST_REQ)
			);
	ret_val = 0;
	return ret_val;
}


static int __init ltq_rcu_init(void)
{
	int ret = 0;
	char ver_str[64];

	printk(KERN_ALERT "GoodDay, trying to register RCU ...\n");

	/* register port device */
	ret = register_chrdev(LTQ_RCU_MAJOR, "ltq_rcu", &g_rcu_fops);
	if (ret != 0) {
		printk(KERN_ALERT "Can not register RCU device - %d", ret);
		return ret;
	}
	ret = platform_driver_register(&grx500_rcu_driver);
	if (ret != 0) {
		printk(KERN_ALERT "Can not register rcu-grx500 driver as a platform driver - %d", ret);
		return ret;
	}
	/* Clear the default callback to NULL for each reset domains */
	ret = ltq_rcu_pre_init();

	ltq_rcu_version(ver_str);
	printk(KERN_INFO "%s", ver_str);
	printk(KERN_ALERT "GRX500 RCU initialized successfully!\n");
	return 0;
}
static void __exit ltq_rcu_exit(void)
{
	struct ltq_rcu_handler_t *p_cur, *p_free;
	int i;

	unregister_chrdev(LTQ_RCU_MAJOR, "ltq_rcu");

	for (i = 0; i < NUM_ENTITY(g_rcu_domains); i++) {
		for (p_free = p_cur = g_rcu_domains[i].handlers;
			p_cur != NULL; p_free = p_cur) {
			p_cur = p_cur->next;
			kfree(p_free);
		}
	}

	platform_driver_unregister(&grx500_rcu_driver);
	printk(KERN_ALERT "GRX500 RCU Driver removed successfully!\n");

}
/*  to be called earlier than other drivers as well as kernel modules */
/* postcore_initcall(ltq_rcu_pre_init);	 */
module_init(ltq_rcu_init);
module_exit(ltq_rcu_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GRX500 RCU Driver Module");
MODULE_AUTHOR("Jason Deng yuhong");
