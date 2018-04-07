/*
** =============================================================================
** FILE NAME     : ltq_vpe.h
** PROJECT       : LXDB
** MODULES       : LXDB
** DATE          : 24-03-2008
** AUTHOR        : LXDB Team
** DESCRIPTION   : This header file contains APIs for LTQ related enhancements
**		   and requirements in MIPS MT code.
** REFERENCES    : AR9 Product Specification
** COPYRIGHT     : Copyright (c) 2008
**                 Infineon Technologies AG,
**                 Am Campeon 1-12, 85579 Neubiberg, Germany
**
** Any use of this software is subject to the conclusion of a respective
** License agreement. Without such a License agreement no rights to the
** software are granted
**
** HISTORY       :
** $Date   $Author    $Comment
** 24-03-2008	LXDB	Initial version
** ============================================================================
*/

#ifndef _LTQ_VPE_H
#define _LTQ_VPE_H

/* For the explanation of the APIs please refer the section "MT APRP Kernel
 * Programming" in AR9 SW Architecutre Specification 
 */
int32_t vpe1_sw_start(void* sw_start_addr, uint32_t tcmask, uint32_t flags);
int32_t vpe1_sw_stop(uint32_t flags);
uint32_t vpe1_get_load_addr (uint32_t flags);
uint32_t vpe1_get_max_mem (uint32_t flags);

int32_t vpe1_set_boot_param(char *field, char *value, char flags);
int32_t vpe1_get_boot_param(char *field, char **value, char flags);

/* Watchdog APIs */
extern uint32_t vpe1_wdog_ctr;
extern uint32_t vpe1_wdog_timeout;

uint32_t vpe1_sw_wdog_start(uint32_t);
uint32_t vpe1_sw_wdog_stop(uint32_t);

typedef int (*VPE_SW_WDOG_RESET)(uint32_t wdog_cleared_ok_count);
int32_t vpe1_sw_wdog_register_reset_handler(VPE_SW_WDOG_RESET reset_fn);

#endif  
