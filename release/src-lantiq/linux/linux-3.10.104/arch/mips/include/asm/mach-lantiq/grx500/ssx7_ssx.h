//-----------------------------------------------------------------------------
// LSD Generator
//-----------------------------------------------------------------------------
// Perl Package        : LSD::generator::targetC (v1.1)
// LSD Source          : D:/Users/shij/Perforce/l1033.grx500.win.v_shij.priv.dfv.grx500.dfv/ipg_lsd/lsd_sys/source/xml/reg_files/ssx7_ssx.xml
// Register File Name  : SSX7_SSX
// Register File Title : SSX7_ssx
// Register Width      : 64
// Note                : Doxygen compliant comments
//-----------------------------------------------------------------------------

#ifndef _SSX7_SSX_H
#define _SSX7_SSX_H

//! \defgroup SSX7_SSX Register File SSX7_SSX - SSX7_ssx
//! @{

//! Base Address of SSX7_SSX
#define SSX7_SSX_MODULE_BASE 0xA3000000u

//! \defgroup TREG7_RT_COMPONENT Register TREG7_RT_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TREG7_RT_COMPONENT 0x0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_RT_COMPONENT 0xA3000000u

//! Register Reset Value
#define TREG7_RT_COMPONENT_RST 0x0000000062003532u

//! Field REV - rev
#define TREG7_RT_COMPONENT_REV_POS 0
//! Field REV - rev
#define TREG7_RT_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TREG7_RT_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TREG7_RT_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TREG7_RT_NETWORK Register TREG7_RT_NETWORK - network
//! @{

//! Register Offset (relative)
#define TREG7_RT_NETWORK 0x10
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_RT_NETWORK 0xA3000010u

//! Register Reset Value
#define TREG7_RT_NETWORK_RST 0x0000000000000000u

//! Field REV - rev
#define TREG7_RT_NETWORK_REV_POS 32
//! Field REV - rev
#define TREG7_RT_NETWORK_REV_MASK 0xFFFF00000000u

//! Field ID - id
#define TREG7_RT_NETWORK_ID_POS 48
//! Field ID - id
#define TREG7_RT_NETWORK_ID_MASK 0xFFFF000000000000u

//! @}

//! \defgroup TREG7_RT_INITID_READBACK Register TREG7_RT_INITID_READBACK - initid_readback
//! @{

//! Register Offset (relative)
#define TREG7_RT_INITID_READBACK 0x70
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_RT_INITID_READBACK 0xA3000070u

//! Register Reset Value
#define TREG7_RT_INITID_READBACK_RST 0x0000000000000000u

//! Field INITID - initid
#define TREG7_RT_INITID_READBACK_INITID_POS 0
//! Field INITID - initid
#define TREG7_RT_INITID_READBACK_INITID_MASK 0xFFu

//! @}

//! \defgroup TREG7_RT_NETWORK_CONTROL Register TREG7_RT_NETWORK_CONTROL - network_control
//! @{

//! Register Offset (relative)
#define TREG7_RT_NETWORK_CONTROL 0x78
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_RT_NETWORK_CONTROL 0xA3000078u

//! Register Reset Value
#define TREG7_RT_NETWORK_CONTROL_RST 0x0000000000000000u

//! Field TIMEOUT_BASE - timeout_base
#define TREG7_RT_NETWORK_CONTROL_TIMEOUT_BASE_POS 8
//! Field TIMEOUT_BASE - timeout_base
#define TREG7_RT_NETWORK_CONTROL_TIMEOUT_BASE_MASK 0x700u

//! Field CLOCK_GATE_DISABLE - clock_gate_disable
#define TREG7_RT_NETWORK_CONTROL_CLOCK_GATE_DISABLE_POS 56
//! Field CLOCK_GATE_DISABLE - clock_gate_disable
#define TREG7_RT_NETWORK_CONTROL_CLOCK_GATE_DISABLE_MASK 0x100000000000000u

//! @}

//! \defgroup TREG7_SI_CONTROL Register TREG7_SI_CONTROL - control
//! @{

//! Register Offset (relative)
#define TREG7_SI_CONTROL 0x420
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_SI_CONTROL 0xA3000420u

//! Register Reset Value
#define TREG7_SI_CONTROL_RST 0x0000000000000000u

//! Field CLOCK_GATE_DISABLE - clock_gate_disable
#define TREG7_SI_CONTROL_CLOCK_GATE_DISABLE_POS 56
//! Field CLOCK_GATE_DISABLE - clock_gate_disable
#define TREG7_SI_CONTROL_CLOCK_GATE_DISABLE_MASK 0x100000000000000u

//! @}

//! \defgroup TREG7_SI_FLAG_STATUS_0 Register TREG7_SI_FLAG_STATUS_0 - flag_status_0
//! @{

//! Register Offset (relative)
#define TREG7_SI_FLAG_STATUS_0 0x510
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_SI_FLAG_STATUS_0 0xA3000510u

//! Register Reset Value
#define TREG7_SI_FLAG_STATUS_0_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG7_SI_FLAG_STATUS_0_STATUS_POS 0
//! Field STATUS - status
#define TREG7_SI_FLAG_STATUS_0_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_SI_FLAG_STATUS_1 Register TREG7_SI_FLAG_STATUS_1 - flag_status_1
//! @{

//! Register Offset (relative)
#define TREG7_SI_FLAG_STATUS_1 0x530
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_SI_FLAG_STATUS_1 0xA3000530u

//! Register Reset Value
#define TREG7_SI_FLAG_STATUS_1_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG7_SI_FLAG_STATUS_1_STATUS_POS 0
//! Field STATUS - status
#define TREG7_SI_FLAG_STATUS_1_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_SI_FLAG_STATUS_2 Register TREG7_SI_FLAG_STATUS_2 - flag_status_2
//! @{

//! Register Offset (relative)
#define TREG7_SI_FLAG_STATUS_2 0x550
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_SI_FLAG_STATUS_2 0xA3000550u

//! Register Reset Value
#define TREG7_SI_FLAG_STATUS_2_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG7_SI_FLAG_STATUS_2_STATUS_POS 0
//! Field STATUS - status
#define TREG7_SI_FLAG_STATUS_2_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_SI_FLAG_STATUS_3 Register TREG7_SI_FLAG_STATUS_3 - flag_status_3
//! @{

//! Register Offset (relative)
#define TREG7_SI_FLAG_STATUS_3 0x570
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_SI_FLAG_STATUS_3 0xA3000570u

//! Register Reset Value
#define TREG7_SI_FLAG_STATUS_3_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG7_SI_FLAG_STATUS_3_STATUS_POS 0
//! Field STATUS - status
#define TREG7_SI_FLAG_STATUS_3_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_SI_FLAG_STATUS_4 Register TREG7_SI_FLAG_STATUS_4 - flag_status_4
//! @{

//! Register Offset (relative)
#define TREG7_SI_FLAG_STATUS_4 0x590
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_SI_FLAG_STATUS_4 0xA3000590u

//! Register Reset Value
#define TREG7_SI_FLAG_STATUS_4_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG7_SI_FLAG_STATUS_4_STATUS_POS 0
//! Field STATUS - status
#define TREG7_SI_FLAG_STATUS_4_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_SI_FLAG_STATUS_5 Register TREG7_SI_FLAG_STATUS_5 - flag_status_5
//! @{

//! Register Offset (relative)
#define TREG7_SI_FLAG_STATUS_5 0x5B0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_SI_FLAG_STATUS_5 0xA30005B0u

//! Register Reset Value
#define TREG7_SI_FLAG_STATUS_5_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG7_SI_FLAG_STATUS_5_STATUS_POS 0
//! Field STATUS - status
#define TREG7_SI_FLAG_STATUS_5_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_SI_FLAG_STATUS_6 Register TREG7_SI_FLAG_STATUS_6 - flag_status_6
//! @{

//! Register Offset (relative)
#define TREG7_SI_FLAG_STATUS_6 0x5D0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_SI_FLAG_STATUS_6 0xA30005D0u

//! Register Reset Value
#define TREG7_SI_FLAG_STATUS_6_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG7_SI_FLAG_STATUS_6_STATUS_POS 0
//! Field STATUS - status
#define TREG7_SI_FLAG_STATUS_6_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_SI_FLAG_STATUS_7 Register TREG7_SI_FLAG_STATUS_7 - flag_status_7
//! @{

//! Register Offset (relative)
#define TREG7_SI_FLAG_STATUS_7 0x5F0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_SI_FLAG_STATUS_7 0xA30005F0u

//! Register Reset Value
#define TREG7_SI_FLAG_STATUS_7_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG7_SI_FLAG_STATUS_7_STATUS_POS 0
//! Field STATUS - status
#define TREG7_SI_FLAG_STATUS_7_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_TA_COMPONENT Register TMPE_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TMPE_TA_COMPONENT 0x800
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_TA_COMPONENT 0xA3000800u

//! Register Reset Value
#define TMPE_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TMPE_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TMPE_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TMPE_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TMPE_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TMPE_TA_CORE Register TMPE_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TMPE_TA_CORE 0x818
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_TA_CORE 0xA3000818u

//! Register Reset Value
#define TMPE_TA_CORE_RST 0x000050C500740001u

//! Field REV_CODE - rev_code
#define TMPE_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TMPE_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TMPE_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TMPE_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TMPE_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TMPE_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TMPE_TA_AGENT_CONTROL Register TMPE_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TMPE_TA_AGENT_CONTROL 0x820
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_TA_AGENT_CONTROL 0xA3000820u

//! Register Reset Value
#define TMPE_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TMPE_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TMPE_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TMPE_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TMPE_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TMPE_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TMPE_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TMPE_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TMPE_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TMPE_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TMPE_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TMPE_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TMPE_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TMPE_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TMPE_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TMPE_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TMPE_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TMPE_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TMPE_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TMPE_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TMPE_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TMPE_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TMPE_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TMPE_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TMPE_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TMPE_TA_AGENT_STATUS Register TMPE_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TMPE_TA_AGENT_STATUS 0x828
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_TA_AGENT_STATUS 0xA3000828u

//! Register Reset Value
#define TMPE_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TMPE_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TMPE_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TMPE_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TMPE_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TMPE_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TMPE_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TMPE_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TMPE_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TMPE_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TMPE_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TMPE_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TMPE_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TMPE_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TMPE_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TMPE_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TMPE_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TMPE_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TMPE_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TMPE_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TMPE_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TMPE_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TMPE_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TMPE_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TMPE_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TMPE_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TMPE_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TMPE_TA_ERROR_LOG Register TMPE_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TMPE_TA_ERROR_LOG 0x858
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_TA_ERROR_LOG 0xA3000858u

//! Register Reset Value
#define TMPE_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TMPE_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TMPE_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TMPE_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TMPE_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TMPE_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TMPE_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TMPE_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TMPE_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TMPE_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TMPE_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TMPE_TA_ERROR_LOG_ADDR Register TMPE_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TMPE_TA_ERROR_LOG_ADDR 0x860
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_TA_ERROR_LOG_ADDR 0xA3000860u

//! Register Reset Value
#define TMPE_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TMPE_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TMPE_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_TA_BANDWIDTH_0 Register TMPE_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TMPE_TA_BANDWIDTH_0 0x900
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_TA_BANDWIDTH_0 0xA3000900u

//! Register Reset Value
#define TMPE_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TMPE_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TMPE_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TMPE_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TMPE_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TMPE_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TMPE_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TMPE_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TMPE_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TMPE_TA_BANDWIDTH_1 Register TMPE_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TMPE_TA_BANDWIDTH_1 0x908
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_TA_BANDWIDTH_1 0xA3000908u

//! Register Reset Value
#define TMPE_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TMPE_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TMPE_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TMPE_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TMPE_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TMPE_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TMPE_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TMPE_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TMPE_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TMPE_TA_BANDWIDTH_2 Register TMPE_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TMPE_TA_BANDWIDTH_2 0x910
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_TA_BANDWIDTH_2 0xA3000910u

//! Register Reset Value
#define TMPE_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TMPE_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TMPE_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TMPE_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TMPE_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TMPE_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TMPE_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TMPE_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TMPE_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TMPE_TA_BANDWIDTH_3 Register TMPE_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TMPE_TA_BANDWIDTH_3 0x918
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_TA_BANDWIDTH_3 0xA3000918u

//! Register Reset Value
#define TMPE_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TMPE_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TMPE_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TMPE_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TMPE_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TMPE_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TMPE_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TMPE_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TMPE_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TMPE_TA_ALLOC_LIMIT_0 Register TMPE_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TMPE_TA_ALLOC_LIMIT_0 0xA00
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_TA_ALLOC_LIMIT_0 0xA3000A00u

//! Register Reset Value
#define TMPE_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TMPE_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TMPE_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TMPE_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TMPE_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TMPE_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TMPE_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TMPE_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TMPE_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TMPE_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TMPE_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TMPE_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TMPE_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TMPE_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TMPE_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TMPE_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TMPE_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TMPE_TA_ALLOC_LIMIT_1 Register TMPE_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TMPE_TA_ALLOC_LIMIT_1 0xA08
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_TA_ALLOC_LIMIT_1 0xA3000A08u

//! Register Reset Value
#define TMPE_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TMPE_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TMPE_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TMPE_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TMPE_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TMPE_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TMPE_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TMPE_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TMPE_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TMPE_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TMPE_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TMPE_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TMPE_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TMPE_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TMPE_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TMPE_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TMPE_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TMPE_TA_ALLOC_LIMIT_2 Register TMPE_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TMPE_TA_ALLOC_LIMIT_2 0xA10
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_TA_ALLOC_LIMIT_2 0xA3000A10u

//! Register Reset Value
#define TMPE_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TMPE_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TMPE_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TMPE_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TMPE_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TMPE_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TMPE_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TMPE_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TMPE_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TMPE_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TMPE_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TMPE_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TMPE_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TMPE_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TMPE_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TMPE_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TMPE_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TMPE_TA_ALLOC_LIMIT_3 Register TMPE_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TMPE_TA_ALLOC_LIMIT_3 0xA18
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_TA_ALLOC_LIMIT_3 0xA3000A18u

//! Register Reset Value
#define TMPE_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TMPE_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TMPE_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TMPE_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TMPE_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TMPE_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TMPE_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TMPE_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TMPE_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TMPE_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TMPE_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TMPE_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TMPE_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TMPE_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TMPE_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TMPE_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TMPE_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TMI2_TA_COMPONENT Register TMI2_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TMI2_TA_COMPONENT 0xC00
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_TA_COMPONENT 0xA3000C00u

//! Register Reset Value
#define TMI2_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TMI2_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TMI2_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TMI2_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TMI2_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TMI2_TA_CORE Register TMI2_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TMI2_TA_CORE 0xC18
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_TA_CORE 0xA3000C18u

//! Register Reset Value
#define TMI2_TA_CORE_RST 0x000050C500720001u

//! Field REV_CODE - rev_code
#define TMI2_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TMI2_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TMI2_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TMI2_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TMI2_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TMI2_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TMI2_TA_AGENT_CONTROL Register TMI2_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TMI2_TA_AGENT_CONTROL 0xC20
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_TA_AGENT_CONTROL 0xA3000C20u

//! Register Reset Value
#define TMI2_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TMI2_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TMI2_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TMI2_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TMI2_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TMI2_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TMI2_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TMI2_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TMI2_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TMI2_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TMI2_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TMI2_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TMI2_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TMI2_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TMI2_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TMI2_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TMI2_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TMI2_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TMI2_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TMI2_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TMI2_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TMI2_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TMI2_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TMI2_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TMI2_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TMI2_TA_AGENT_STATUS Register TMI2_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TMI2_TA_AGENT_STATUS 0xC28
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_TA_AGENT_STATUS 0xA3000C28u

//! Register Reset Value
#define TMI2_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TMI2_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TMI2_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TMI2_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TMI2_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TMI2_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TMI2_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TMI2_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TMI2_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TMI2_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TMI2_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TMI2_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TMI2_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TMI2_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TMI2_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TMI2_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TMI2_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TMI2_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TMI2_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TMI2_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TMI2_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TMI2_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TMI2_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TMI2_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TMI2_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TMI2_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TMI2_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TMI2_TA_ERROR_LOG Register TMI2_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TMI2_TA_ERROR_LOG 0xC58
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_TA_ERROR_LOG 0xA3000C58u

//! Register Reset Value
#define TMI2_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TMI2_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TMI2_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TMI2_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TMI2_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TMI2_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TMI2_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TMI2_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TMI2_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TMI2_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TMI2_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TMI2_TA_ERROR_LOG_ADDR Register TMI2_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TMI2_TA_ERROR_LOG_ADDR 0xC60
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_TA_ERROR_LOG_ADDR 0xA3000C60u

//! Register Reset Value
#define TMI2_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TMI2_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TMI2_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_TA_BANDWIDTH_0 Register TMI2_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TMI2_TA_BANDWIDTH_0 0xD00
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_TA_BANDWIDTH_0 0xA3000D00u

//! Register Reset Value
#define TMI2_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TMI2_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TMI2_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TMI2_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TMI2_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TMI2_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TMI2_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TMI2_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TMI2_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TMI2_TA_BANDWIDTH_1 Register TMI2_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TMI2_TA_BANDWIDTH_1 0xD08
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_TA_BANDWIDTH_1 0xA3000D08u

//! Register Reset Value
#define TMI2_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TMI2_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TMI2_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TMI2_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TMI2_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TMI2_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TMI2_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TMI2_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TMI2_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TMI2_TA_BANDWIDTH_2 Register TMI2_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TMI2_TA_BANDWIDTH_2 0xD10
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_TA_BANDWIDTH_2 0xA3000D10u

//! Register Reset Value
#define TMI2_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TMI2_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TMI2_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TMI2_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TMI2_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TMI2_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TMI2_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TMI2_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TMI2_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TMI2_TA_BANDWIDTH_3 Register TMI2_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TMI2_TA_BANDWIDTH_3 0xD18
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_TA_BANDWIDTH_3 0xA3000D18u

//! Register Reset Value
#define TMI2_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TMI2_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TMI2_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TMI2_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TMI2_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TMI2_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TMI2_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TMI2_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TMI2_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TMI2_TA_ALLOC_LIMIT_0 Register TMI2_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TMI2_TA_ALLOC_LIMIT_0 0xE00
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_TA_ALLOC_LIMIT_0 0xA3000E00u

//! Register Reset Value
#define TMI2_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TMI2_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TMI2_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TMI2_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TMI2_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TMI2_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TMI2_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TMI2_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TMI2_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TMI2_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TMI2_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TMI2_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TMI2_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TMI2_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TMI2_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TMI2_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TMI2_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TMI2_TA_ALLOC_LIMIT_1 Register TMI2_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TMI2_TA_ALLOC_LIMIT_1 0xE08
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_TA_ALLOC_LIMIT_1 0xA3000E08u

//! Register Reset Value
#define TMI2_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TMI2_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TMI2_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TMI2_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TMI2_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TMI2_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TMI2_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TMI2_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TMI2_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TMI2_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TMI2_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TMI2_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TMI2_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TMI2_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TMI2_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TMI2_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TMI2_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TMI2_TA_ALLOC_LIMIT_2 Register TMI2_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TMI2_TA_ALLOC_LIMIT_2 0xE10
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_TA_ALLOC_LIMIT_2 0xA3000E10u

//! Register Reset Value
#define TMI2_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TMI2_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TMI2_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TMI2_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TMI2_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TMI2_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TMI2_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TMI2_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TMI2_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TMI2_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TMI2_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TMI2_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TMI2_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TMI2_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TMI2_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TMI2_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TMI2_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TMI2_TA_ALLOC_LIMIT_3 Register TMI2_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TMI2_TA_ALLOC_LIMIT_3 0xE18
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_TA_ALLOC_LIMIT_3 0xA3000E18u

//! Register Reset Value
#define TMI2_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TMI2_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TMI2_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TMI2_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TMI2_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TMI2_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TMI2_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TMI2_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TMI2_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TMI2_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TMI2_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TMI2_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TMI2_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TMI2_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TMI2_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TMI2_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TMI2_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TTOE_TA_COMPONENT Register TTOE_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TTOE_TA_COMPONENT 0x1000
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_TA_COMPONENT 0xA3001000u

//! Register Reset Value
#define TTOE_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TTOE_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TTOE_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TTOE_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TTOE_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TTOE_TA_CORE Register TTOE_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TTOE_TA_CORE 0x1018
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_TA_CORE 0xA3001018u

//! Register Reset Value
#define TTOE_TA_CORE_RST 0x000050C500730001u

//! Field REV_CODE - rev_code
#define TTOE_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TTOE_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TTOE_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TTOE_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TTOE_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TTOE_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TTOE_TA_AGENT_CONTROL Register TTOE_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TTOE_TA_AGENT_CONTROL 0x1020
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_TA_AGENT_CONTROL 0xA3001020u

//! Register Reset Value
#define TTOE_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TTOE_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TTOE_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TTOE_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TTOE_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TTOE_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TTOE_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TTOE_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TTOE_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TTOE_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TTOE_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TTOE_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TTOE_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TTOE_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TTOE_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TTOE_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TTOE_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TTOE_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TTOE_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TTOE_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TTOE_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TTOE_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TTOE_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TTOE_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TTOE_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TTOE_TA_AGENT_STATUS Register TTOE_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TTOE_TA_AGENT_STATUS 0x1028
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_TA_AGENT_STATUS 0xA3001028u

//! Register Reset Value
#define TTOE_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TTOE_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TTOE_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TTOE_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TTOE_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TTOE_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TTOE_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TTOE_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TTOE_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TTOE_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TTOE_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TTOE_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TTOE_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TTOE_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TTOE_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TTOE_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TTOE_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TTOE_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TTOE_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TTOE_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TTOE_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TTOE_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TTOE_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TTOE_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TTOE_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TTOE_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TTOE_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TTOE_TA_ERROR_LOG Register TTOE_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TTOE_TA_ERROR_LOG 0x1058
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_TA_ERROR_LOG 0xA3001058u

//! Register Reset Value
#define TTOE_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TTOE_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TTOE_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TTOE_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TTOE_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TTOE_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TTOE_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TTOE_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TTOE_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TTOE_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TTOE_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TTOE_TA_ERROR_LOG_ADDR Register TTOE_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TTOE_TA_ERROR_LOG_ADDR 0x1060
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_TA_ERROR_LOG_ADDR 0xA3001060u

//! Register Reset Value
#define TTOE_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TTOE_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TTOE_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_TA_BANDWIDTH_0 Register TTOE_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TTOE_TA_BANDWIDTH_0 0x1100
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_TA_BANDWIDTH_0 0xA3001100u

//! Register Reset Value
#define TTOE_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TTOE_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TTOE_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TTOE_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TTOE_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TTOE_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TTOE_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TTOE_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TTOE_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TTOE_TA_BANDWIDTH_1 Register TTOE_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TTOE_TA_BANDWIDTH_1 0x1108
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_TA_BANDWIDTH_1 0xA3001108u

//! Register Reset Value
#define TTOE_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TTOE_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TTOE_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TTOE_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TTOE_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TTOE_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TTOE_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TTOE_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TTOE_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TTOE_TA_BANDWIDTH_2 Register TTOE_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TTOE_TA_BANDWIDTH_2 0x1110
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_TA_BANDWIDTH_2 0xA3001110u

//! Register Reset Value
#define TTOE_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TTOE_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TTOE_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TTOE_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TTOE_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TTOE_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TTOE_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TTOE_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TTOE_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TTOE_TA_BANDWIDTH_3 Register TTOE_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TTOE_TA_BANDWIDTH_3 0x1118
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_TA_BANDWIDTH_3 0xA3001118u

//! Register Reset Value
#define TTOE_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TTOE_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TTOE_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TTOE_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TTOE_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TTOE_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TTOE_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TTOE_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TTOE_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TTOE_TA_ALLOC_LIMIT_0 Register TTOE_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TTOE_TA_ALLOC_LIMIT_0 0x1200
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_TA_ALLOC_LIMIT_0 0xA3001200u

//! Register Reset Value
#define TTOE_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TTOE_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TTOE_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TTOE_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TTOE_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TTOE_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TTOE_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TTOE_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TTOE_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TTOE_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TTOE_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TTOE_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TTOE_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TTOE_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TTOE_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TTOE_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TTOE_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TTOE_TA_ALLOC_LIMIT_1 Register TTOE_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TTOE_TA_ALLOC_LIMIT_1 0x1208
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_TA_ALLOC_LIMIT_1 0xA3001208u

//! Register Reset Value
#define TTOE_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TTOE_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TTOE_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TTOE_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TTOE_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TTOE_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TTOE_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TTOE_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TTOE_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TTOE_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TTOE_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TTOE_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TTOE_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TTOE_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TTOE_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TTOE_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TTOE_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TTOE_TA_ALLOC_LIMIT_2 Register TTOE_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TTOE_TA_ALLOC_LIMIT_2 0x1210
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_TA_ALLOC_LIMIT_2 0xA3001210u

//! Register Reset Value
#define TTOE_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TTOE_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TTOE_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TTOE_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TTOE_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TTOE_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TTOE_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TTOE_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TTOE_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TTOE_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TTOE_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TTOE_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TTOE_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TTOE_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TTOE_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TTOE_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TTOE_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TTOE_TA_ALLOC_LIMIT_3 Register TTOE_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TTOE_TA_ALLOC_LIMIT_3 0x1218
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_TA_ALLOC_LIMIT_3 0xA3001218u

//! Register Reset Value
#define TTOE_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TTOE_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TTOE_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TTOE_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TTOE_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TTOE_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TTOE_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TTOE_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TTOE_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TTOE_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TTOE_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TTOE_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TTOE_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TTOE_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TTOE_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TTOE_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TTOE_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IMPE_IA_COMPONENT Register IMPE_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define IMPE_IA_COMPONENT 0x1400
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMPE_IA_COMPONENT 0xA3001400u

//! Register Reset Value
#define IMPE_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define IMPE_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define IMPE_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define IMPE_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define IMPE_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup IMPE_IA_CORE Register IMPE_IA_CORE - core
//! @{

//! Register Offset (relative)
#define IMPE_IA_CORE 0x1418
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMPE_IA_CORE 0xA3001418u

//! Register Reset Value
#define IMPE_IA_CORE_RST 0x000050C500740001u

//! Field REV_CODE - rev_code
#define IMPE_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define IMPE_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define IMPE_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define IMPE_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define IMPE_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define IMPE_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup IMPE_IA_AGENT_CONTROL Register IMPE_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define IMPE_IA_AGENT_CONTROL 0x1420
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMPE_IA_AGENT_CONTROL 0xA3001420u

//! Register Reset Value
#define IMPE_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IMPE_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IMPE_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define IMPE_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define IMPE_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define IMPE_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IMPE_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define IMPE_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IMPE_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define IMPE_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define IMPE_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IMPE_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IMPE_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IMPE_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IMPE_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IMPE_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IMPE_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IMPE_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IMPE_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IMPE_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IMPE_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IMPE_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IMPE_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup IMPE_IA_AGENT_STATUS Register IMPE_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define IMPE_IA_AGENT_STATUS 0x1428
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMPE_IA_AGENT_STATUS 0xA3001428u

//! Register Reset Value
#define IMPE_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IMPE_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IMPE_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define IMPE_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define IMPE_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define IMPE_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define IMPE_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define IMPE_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define IMPE_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define IMPE_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define IMPE_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define IMPE_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IMPE_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define IMPE_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define IMPE_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define IMPE_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IMPE_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define IMPE_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define IMPE_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IMPE_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IMPE_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IMPE_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IMPE_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup IMPE_IA_ERROR_LOG Register IMPE_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define IMPE_IA_ERROR_LOG 0x1458
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMPE_IA_ERROR_LOG 0xA3001458u

//! Register Reset Value
#define IMPE_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define IMPE_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define IMPE_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define IMPE_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define IMPE_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define IMPE_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define IMPE_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define IMPE_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define IMPE_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define IMPE_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define IMPE_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define IMPE_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define IMPE_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define IMPE_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define IMPE_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define IMPE_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define IMPE_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define IMPE_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define IMPE_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup IMPE_IA_ERROR_LOG_ADDR Register IMPE_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define IMPE_IA_ERROR_LOG_ADDR 0x1460
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMPE_IA_ERROR_LOG_ADDR 0xA3001460u

//! Register Reset Value
#define IMPE_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define IMPE_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define IMPE_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup IMPE_IA_CORE_FLAG Register IMPE_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define IMPE_IA_CORE_FLAG 0x1468
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMPE_IA_CORE_FLAG 0xA3001468u

//! Register Reset Value
#define IMPE_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define IMPE_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define IMPE_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define IMPE_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define IMPE_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define IMPE_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define IMPE_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define IMPE_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define IMPE_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup IMPE_IA_ADDR_FILL_IN Register IMPE_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define IMPE_IA_ADDR_FILL_IN 0x1470
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMPE_IA_ADDR_FILL_IN 0xA3001470u

//! Register Reset Value
#define IMPE_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define IMPE_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define IMPE_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup IMPE_IA_BANDWIDTH_0 Register IMPE_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define IMPE_IA_BANDWIDTH_0 0x1500
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMPE_IA_BANDWIDTH_0 0xA3001500u

//! Register Reset Value
#define IMPE_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define IMPE_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define IMPE_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define IMPE_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define IMPE_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define IMPE_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define IMPE_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define IMPE_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define IMPE_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define IMPE_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define IMPE_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define IMPE_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define IMPE_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define IMPE_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define IMPE_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define IMPE_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define IMPE_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup IMPE_IA_BANDWIDTH_1 Register IMPE_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define IMPE_IA_BANDWIDTH_1 0x1508
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMPE_IA_BANDWIDTH_1 0xA3001508u

//! Register Reset Value
#define IMPE_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define IMPE_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define IMPE_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define IMPE_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define IMPE_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define IMPE_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define IMPE_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define IMPE_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define IMPE_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define IMPE_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define IMPE_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define IMPE_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define IMPE_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define IMPE_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define IMPE_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define IMPE_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define IMPE_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IMPE_IA_BANDWIDTH_2 Register IMPE_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define IMPE_IA_BANDWIDTH_2 0x1510
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMPE_IA_BANDWIDTH_2 0xA3001510u

//! Register Reset Value
#define IMPE_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define IMPE_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define IMPE_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define IMPE_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define IMPE_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define IMPE_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define IMPE_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define IMPE_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define IMPE_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define IMPE_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define IMPE_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define IMPE_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define IMPE_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define IMPE_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define IMPE_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define IMPE_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define IMPE_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup IMPE_IA_BANDWIDTH_3 Register IMPE_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define IMPE_IA_BANDWIDTH_3 0x1518
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMPE_IA_BANDWIDTH_3 0xA3001518u

//! Register Reset Value
#define IMPE_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define IMPE_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define IMPE_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define IMPE_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define IMPE_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define IMPE_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define IMPE_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define IMPE_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define IMPE_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define IMPE_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define IMPE_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define IMPE_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define IMPE_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define IMPE_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define IMPE_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define IMPE_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define IMPE_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup IMPE_IA_BANDWIDTH_4 Register IMPE_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define IMPE_IA_BANDWIDTH_4 0x1520
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMPE_IA_BANDWIDTH_4 0xA3001520u

//! Register Reset Value
#define IMPE_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define IMPE_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define IMPE_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define IMPE_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define IMPE_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define IMPE_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define IMPE_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define IMPE_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define IMPE_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define IMPE_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define IMPE_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define IMPE_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define IMPE_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define IMPE_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define IMPE_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define IMPE_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define IMPE_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup IMPE_IA_BANDWIDTH_5 Register IMPE_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define IMPE_IA_BANDWIDTH_5 0x1528
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMPE_IA_BANDWIDTH_5 0xA3001528u

//! Register Reset Value
#define IMPE_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define IMPE_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define IMPE_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define IMPE_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define IMPE_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define IMPE_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define IMPE_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define IMPE_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define IMPE_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define IMPE_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define IMPE_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define IMPE_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define IMPE_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define IMPE_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define IMPE_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define IMPE_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define IMPE_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup IMPE_IA_BANDWIDTH_6 Register IMPE_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define IMPE_IA_BANDWIDTH_6 0x1530
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMPE_IA_BANDWIDTH_6 0xA3001530u

//! Register Reset Value
#define IMPE_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define IMPE_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define IMPE_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define IMPE_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define IMPE_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define IMPE_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define IMPE_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define IMPE_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define IMPE_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define IMPE_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define IMPE_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define IMPE_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define IMPE_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define IMPE_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define IMPE_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define IMPE_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define IMPE_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup IMPE_IA_BANDWIDTH_7 Register IMPE_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define IMPE_IA_BANDWIDTH_7 0x1538
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMPE_IA_BANDWIDTH_7 0xA3001538u

//! Register Reset Value
#define IMPE_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define IMPE_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define IMPE_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define IMPE_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define IMPE_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define IMPE_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define IMPE_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define IMPE_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define IMPE_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define IMPE_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define IMPE_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define IMPE_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define IMPE_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define IMPE_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define IMPE_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define IMPE_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define IMPE_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup IMO2_IA_COMPONENT Register IMO2_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define IMO2_IA_COMPONENT 0x1800
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMO2_IA_COMPONENT 0xA3001800u

//! Register Reset Value
#define IMO2_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define IMO2_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define IMO2_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define IMO2_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define IMO2_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup IMO2_IA_CORE Register IMO2_IA_CORE - core
//! @{

//! Register Offset (relative)
#define IMO2_IA_CORE 0x1818
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMO2_IA_CORE 0xA3001818u

//! Register Reset Value
#define IMO2_IA_CORE_RST 0x000050C500720001u

//! Field REV_CODE - rev_code
#define IMO2_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define IMO2_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define IMO2_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define IMO2_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define IMO2_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define IMO2_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup IMO2_IA_AGENT_CONTROL Register IMO2_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define IMO2_IA_AGENT_CONTROL 0x1820
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMO2_IA_AGENT_CONTROL 0xA3001820u

//! Register Reset Value
#define IMO2_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IMO2_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IMO2_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define IMO2_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define IMO2_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define IMO2_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IMO2_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define IMO2_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IMO2_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define IMO2_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define IMO2_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IMO2_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IMO2_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IMO2_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IMO2_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IMO2_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IMO2_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IMO2_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IMO2_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IMO2_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IMO2_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IMO2_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IMO2_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup IMO2_IA_AGENT_STATUS Register IMO2_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define IMO2_IA_AGENT_STATUS 0x1828
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMO2_IA_AGENT_STATUS 0xA3001828u

//! Register Reset Value
#define IMO2_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IMO2_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IMO2_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define IMO2_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define IMO2_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define IMO2_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define IMO2_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define IMO2_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define IMO2_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define IMO2_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define IMO2_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define IMO2_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IMO2_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define IMO2_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define IMO2_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define IMO2_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IMO2_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define IMO2_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define IMO2_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IMO2_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IMO2_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IMO2_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IMO2_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup IMO2_IA_ERROR_LOG Register IMO2_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define IMO2_IA_ERROR_LOG 0x1858
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMO2_IA_ERROR_LOG 0xA3001858u

//! Register Reset Value
#define IMO2_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define IMO2_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define IMO2_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define IMO2_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define IMO2_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define IMO2_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define IMO2_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define IMO2_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define IMO2_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define IMO2_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define IMO2_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define IMO2_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define IMO2_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define IMO2_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define IMO2_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define IMO2_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define IMO2_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define IMO2_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define IMO2_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup IMO2_IA_ERROR_LOG_ADDR Register IMO2_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define IMO2_IA_ERROR_LOG_ADDR 0x1860
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMO2_IA_ERROR_LOG_ADDR 0xA3001860u

//! Register Reset Value
#define IMO2_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define IMO2_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define IMO2_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup IMO2_IA_CORE_FLAG Register IMO2_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define IMO2_IA_CORE_FLAG 0x1868
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMO2_IA_CORE_FLAG 0xA3001868u

//! Register Reset Value
#define IMO2_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define IMO2_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define IMO2_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define IMO2_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define IMO2_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define IMO2_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define IMO2_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define IMO2_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define IMO2_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup IMO2_IA_ADDR_FILL_IN Register IMO2_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define IMO2_IA_ADDR_FILL_IN 0x1870
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMO2_IA_ADDR_FILL_IN 0xA3001870u

//! Register Reset Value
#define IMO2_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define IMO2_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define IMO2_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup IMO2_IA_BANDWIDTH_0 Register IMO2_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define IMO2_IA_BANDWIDTH_0 0x1900
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMO2_IA_BANDWIDTH_0 0xA3001900u

//! Register Reset Value
#define IMO2_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define IMO2_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define IMO2_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define IMO2_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define IMO2_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define IMO2_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define IMO2_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define IMO2_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define IMO2_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define IMO2_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define IMO2_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define IMO2_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define IMO2_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define IMO2_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define IMO2_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define IMO2_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define IMO2_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup IMO2_IA_BANDWIDTH_1 Register IMO2_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define IMO2_IA_BANDWIDTH_1 0x1908
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMO2_IA_BANDWIDTH_1 0xA3001908u

//! Register Reset Value
#define IMO2_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define IMO2_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define IMO2_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define IMO2_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define IMO2_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define IMO2_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define IMO2_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define IMO2_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define IMO2_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define IMO2_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define IMO2_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define IMO2_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define IMO2_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define IMO2_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define IMO2_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define IMO2_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define IMO2_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IMO2_IA_BANDWIDTH_2 Register IMO2_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define IMO2_IA_BANDWIDTH_2 0x1910
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMO2_IA_BANDWIDTH_2 0xA3001910u

//! Register Reset Value
#define IMO2_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define IMO2_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define IMO2_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define IMO2_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define IMO2_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define IMO2_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define IMO2_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define IMO2_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define IMO2_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define IMO2_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define IMO2_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define IMO2_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define IMO2_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define IMO2_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define IMO2_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define IMO2_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define IMO2_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup IMO2_IA_BANDWIDTH_3 Register IMO2_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define IMO2_IA_BANDWIDTH_3 0x1918
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMO2_IA_BANDWIDTH_3 0xA3001918u

//! Register Reset Value
#define IMO2_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define IMO2_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define IMO2_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define IMO2_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define IMO2_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define IMO2_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define IMO2_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define IMO2_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define IMO2_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define IMO2_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define IMO2_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define IMO2_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define IMO2_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define IMO2_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define IMO2_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define IMO2_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define IMO2_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup IMO2_IA_BANDWIDTH_4 Register IMO2_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define IMO2_IA_BANDWIDTH_4 0x1920
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMO2_IA_BANDWIDTH_4 0xA3001920u

//! Register Reset Value
#define IMO2_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define IMO2_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define IMO2_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define IMO2_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define IMO2_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define IMO2_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define IMO2_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define IMO2_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define IMO2_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define IMO2_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define IMO2_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define IMO2_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define IMO2_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define IMO2_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define IMO2_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define IMO2_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define IMO2_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup IMO2_IA_BANDWIDTH_5 Register IMO2_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define IMO2_IA_BANDWIDTH_5 0x1928
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMO2_IA_BANDWIDTH_5 0xA3001928u

//! Register Reset Value
#define IMO2_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define IMO2_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define IMO2_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define IMO2_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define IMO2_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define IMO2_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define IMO2_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define IMO2_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define IMO2_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define IMO2_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define IMO2_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define IMO2_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define IMO2_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define IMO2_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define IMO2_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define IMO2_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define IMO2_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup IMO2_IA_BANDWIDTH_6 Register IMO2_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define IMO2_IA_BANDWIDTH_6 0x1930
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMO2_IA_BANDWIDTH_6 0xA3001930u

//! Register Reset Value
#define IMO2_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define IMO2_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define IMO2_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define IMO2_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define IMO2_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define IMO2_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define IMO2_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define IMO2_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define IMO2_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define IMO2_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define IMO2_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define IMO2_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define IMO2_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define IMO2_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define IMO2_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define IMO2_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define IMO2_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup IMO2_IA_BANDWIDTH_7 Register IMO2_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define IMO2_IA_BANDWIDTH_7 0x1938
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_IMO2_IA_BANDWIDTH_7 0xA3001938u

//! Register Reset Value
#define IMO2_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define IMO2_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define IMO2_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define IMO2_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define IMO2_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define IMO2_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define IMO2_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define IMO2_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define IMO2_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define IMO2_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define IMO2_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define IMO2_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define IMO2_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define IMO2_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define IMO2_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define IMO2_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define IMO2_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup TMI2_PM_ERROR_LOG Register TMI2_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TMI2_PM_ERROR_LOG 0x80020
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ERROR_LOG 0xA3080020u

//! Register Reset Value
#define TMI2_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TMI2_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TMI2_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TMI2_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TMI2_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TMI2_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TMI2_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TMI2_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TMI2_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TMI2_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TMI2_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TMI2_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TMI2_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TMI2_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TMI2_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TMI2_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TMI2_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TMI2_PM_CONTROL Register TMI2_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TMI2_PM_CONTROL 0x80028
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_CONTROL 0xA3080028u

//! Register Reset Value
#define TMI2_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TMI2_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TMI2_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TMI2_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TMI2_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TMI2_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TMI2_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TMI2_PM_ERROR_CLEAR_SINGLE Register TMI2_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TMI2_PM_ERROR_CLEAR_SINGLE 0x80030
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ERROR_CLEAR_SINGLE 0xA3080030u

//! Register Reset Value
#define TMI2_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TMI2_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TMI2_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TMI2_PM_ERROR_CLEAR_MULTI Register TMI2_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TMI2_PM_ERROR_CLEAR_MULTI 0x80038
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ERROR_CLEAR_MULTI 0xA3080038u

//! Register Reset Value
#define TMI2_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TMI2_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TMI2_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TMI2_PM_REQ_INFO_PERMISSION_0 Register TMI2_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TMI2_PM_REQ_INFO_PERMISSION_0 0x80048
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_REQ_INFO_PERMISSION_0 0xA3080048u

//! Register Reset Value
#define TMI2_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_READ_PERMISSION_0 Register TMI2_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TMI2_PM_READ_PERMISSION_0 0x80050
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_READ_PERMISSION_0 0xA3080050u

//! Register Reset Value
#define TMI2_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_WRITE_PERMISSION_0 Register TMI2_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TMI2_PM_WRITE_PERMISSION_0 0x80058
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_WRITE_PERMISSION_0 0xA3080058u

//! Register Reset Value
#define TMI2_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_ADDR_MATCH_1 Register TMI2_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TMI2_PM_ADDR_MATCH_1 0x80060
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ADDR_MATCH_1 0xA3080060u

//! Register Reset Value
#define TMI2_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMI2_PM_REQ_INFO_PERMISSION_1 Register TMI2_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TMI2_PM_REQ_INFO_PERMISSION_1 0x80068
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_REQ_INFO_PERMISSION_1 0xA3080068u

//! Register Reset Value
#define TMI2_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_READ_PERMISSION_1 Register TMI2_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TMI2_PM_READ_PERMISSION_1 0x80070
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_READ_PERMISSION_1 0xA3080070u

//! Register Reset Value
#define TMI2_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_WRITE_PERMISSION_1 Register TMI2_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TMI2_PM_WRITE_PERMISSION_1 0x80078
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_WRITE_PERMISSION_1 0xA3080078u

//! Register Reset Value
#define TMI2_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_ADDR_MATCH_2 Register TMI2_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TMI2_PM_ADDR_MATCH_2 0x80080
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ADDR_MATCH_2 0xA3080080u

//! Register Reset Value
#define TMI2_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMI2_PM_REQ_INFO_PERMISSION_2 Register TMI2_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TMI2_PM_REQ_INFO_PERMISSION_2 0x80088
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_REQ_INFO_PERMISSION_2 0xA3080088u

//! Register Reset Value
#define TMI2_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_READ_PERMISSION_2 Register TMI2_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TMI2_PM_READ_PERMISSION_2 0x80090
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_READ_PERMISSION_2 0xA3080090u

//! Register Reset Value
#define TMI2_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_WRITE_PERMISSION_2 Register TMI2_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TMI2_PM_WRITE_PERMISSION_2 0x80098
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_WRITE_PERMISSION_2 0xA3080098u

//! Register Reset Value
#define TMI2_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_ADDR_MATCH_3 Register TMI2_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TMI2_PM_ADDR_MATCH_3 0x800A0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ADDR_MATCH_3 0xA30800A0u

//! Register Reset Value
#define TMI2_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMI2_PM_REQ_INFO_PERMISSION_3 Register TMI2_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TMI2_PM_REQ_INFO_PERMISSION_3 0x800A8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_REQ_INFO_PERMISSION_3 0xA30800A8u

//! Register Reset Value
#define TMI2_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_READ_PERMISSION_3 Register TMI2_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TMI2_PM_READ_PERMISSION_3 0x800B0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_READ_PERMISSION_3 0xA30800B0u

//! Register Reset Value
#define TMI2_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_WRITE_PERMISSION_3 Register TMI2_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TMI2_PM_WRITE_PERMISSION_3 0x800B8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_WRITE_PERMISSION_3 0xA30800B8u

//! Register Reset Value
#define TMI2_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_ADDR_MATCH_4 Register TMI2_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TMI2_PM_ADDR_MATCH_4 0x800C0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ADDR_MATCH_4 0xA30800C0u

//! Register Reset Value
#define TMI2_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMI2_PM_REQ_INFO_PERMISSION_4 Register TMI2_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TMI2_PM_REQ_INFO_PERMISSION_4 0x800C8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_REQ_INFO_PERMISSION_4 0xA30800C8u

//! Register Reset Value
#define TMI2_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_READ_PERMISSION_4 Register TMI2_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TMI2_PM_READ_PERMISSION_4 0x800D0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_READ_PERMISSION_4 0xA30800D0u

//! Register Reset Value
#define TMI2_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_WRITE_PERMISSION_4 Register TMI2_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TMI2_PM_WRITE_PERMISSION_4 0x800D8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_WRITE_PERMISSION_4 0xA30800D8u

//! Register Reset Value
#define TMI2_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_ADDR_MATCH_5 Register TMI2_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TMI2_PM_ADDR_MATCH_5 0x800E0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ADDR_MATCH_5 0xA30800E0u

//! Register Reset Value
#define TMI2_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMI2_PM_REQ_INFO_PERMISSION_5 Register TMI2_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TMI2_PM_REQ_INFO_PERMISSION_5 0x800E8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_REQ_INFO_PERMISSION_5 0xA30800E8u

//! Register Reset Value
#define TMI2_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_READ_PERMISSION_5 Register TMI2_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TMI2_PM_READ_PERMISSION_5 0x800F0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_READ_PERMISSION_5 0xA30800F0u

//! Register Reset Value
#define TMI2_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_WRITE_PERMISSION_5 Register TMI2_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TMI2_PM_WRITE_PERMISSION_5 0x800F8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_WRITE_PERMISSION_5 0xA30800F8u

//! Register Reset Value
#define TMI2_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_ADDR_MATCH_6 Register TMI2_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TMI2_PM_ADDR_MATCH_6 0x80100
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ADDR_MATCH_6 0xA3080100u

//! Register Reset Value
#define TMI2_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMI2_PM_REQ_INFO_PERMISSION_6 Register TMI2_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TMI2_PM_REQ_INFO_PERMISSION_6 0x80108
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_REQ_INFO_PERMISSION_6 0xA3080108u

//! Register Reset Value
#define TMI2_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_READ_PERMISSION_6 Register TMI2_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TMI2_PM_READ_PERMISSION_6 0x80110
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_READ_PERMISSION_6 0xA3080110u

//! Register Reset Value
#define TMI2_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_WRITE_PERMISSION_6 Register TMI2_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TMI2_PM_WRITE_PERMISSION_6 0x80118
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_WRITE_PERMISSION_6 0xA3080118u

//! Register Reset Value
#define TMI2_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_ADDR_MATCH_7 Register TMI2_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TMI2_PM_ADDR_MATCH_7 0x80120
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ADDR_MATCH_7 0xA3080120u

//! Register Reset Value
#define TMI2_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMI2_PM_REQ_INFO_PERMISSION_7 Register TMI2_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TMI2_PM_REQ_INFO_PERMISSION_7 0x80128
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_REQ_INFO_PERMISSION_7 0xA3080128u

//! Register Reset Value
#define TMI2_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_READ_PERMISSION_7 Register TMI2_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TMI2_PM_READ_PERMISSION_7 0x80130
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_READ_PERMISSION_7 0xA3080130u

//! Register Reset Value
#define TMI2_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_WRITE_PERMISSION_7 Register TMI2_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TMI2_PM_WRITE_PERMISSION_7 0x80138
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_WRITE_PERMISSION_7 0xA3080138u

//! Register Reset Value
#define TMI2_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_ADDR_MATCH_8 Register TMI2_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TMI2_PM_ADDR_MATCH_8 0x80140
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ADDR_MATCH_8 0xA3080140u

//! Register Reset Value
#define TMI2_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMI2_PM_REQ_INFO_PERMISSION_8 Register TMI2_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TMI2_PM_REQ_INFO_PERMISSION_8 0x80148
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_REQ_INFO_PERMISSION_8 0xA3080148u

//! Register Reset Value
#define TMI2_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_READ_PERMISSION_8 Register TMI2_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TMI2_PM_READ_PERMISSION_8 0x80150
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_READ_PERMISSION_8 0xA3080150u

//! Register Reset Value
#define TMI2_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_WRITE_PERMISSION_8 Register TMI2_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TMI2_PM_WRITE_PERMISSION_8 0x80158
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_WRITE_PERMISSION_8 0xA3080158u

//! Register Reset Value
#define TMI2_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_ADDR_MATCH_9 Register TMI2_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TMI2_PM_ADDR_MATCH_9 0x80160
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ADDR_MATCH_9 0xA3080160u

//! Register Reset Value
#define TMI2_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMI2_PM_REQ_INFO_PERMISSION_9 Register TMI2_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TMI2_PM_REQ_INFO_PERMISSION_9 0x80168
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_REQ_INFO_PERMISSION_9 0xA3080168u

//! Register Reset Value
#define TMI2_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_READ_PERMISSION_9 Register TMI2_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TMI2_PM_READ_PERMISSION_9 0x80170
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_READ_PERMISSION_9 0xA3080170u

//! Register Reset Value
#define TMI2_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_WRITE_PERMISSION_9 Register TMI2_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TMI2_PM_WRITE_PERMISSION_9 0x80178
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_WRITE_PERMISSION_9 0xA3080178u

//! Register Reset Value
#define TMI2_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_ADDR_MATCH_10 Register TMI2_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TMI2_PM_ADDR_MATCH_10 0x80180
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ADDR_MATCH_10 0xA3080180u

//! Register Reset Value
#define TMI2_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMI2_PM_REQ_INFO_PERMISSION_10 Register TMI2_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TMI2_PM_REQ_INFO_PERMISSION_10 0x80188
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_REQ_INFO_PERMISSION_10 0xA3080188u

//! Register Reset Value
#define TMI2_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_READ_PERMISSION_10 Register TMI2_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TMI2_PM_READ_PERMISSION_10 0x80190
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_READ_PERMISSION_10 0xA3080190u

//! Register Reset Value
#define TMI2_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_WRITE_PERMISSION_10 Register TMI2_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TMI2_PM_WRITE_PERMISSION_10 0x80198
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_WRITE_PERMISSION_10 0xA3080198u

//! Register Reset Value
#define TMI2_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_ADDR_MATCH_11 Register TMI2_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TMI2_PM_ADDR_MATCH_11 0x801A0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ADDR_MATCH_11 0xA30801A0u

//! Register Reset Value
#define TMI2_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMI2_PM_REQ_INFO_PERMISSION_11 Register TMI2_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TMI2_PM_REQ_INFO_PERMISSION_11 0x801A8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_REQ_INFO_PERMISSION_11 0xA30801A8u

//! Register Reset Value
#define TMI2_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_READ_PERMISSION_11 Register TMI2_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TMI2_PM_READ_PERMISSION_11 0x801B0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_READ_PERMISSION_11 0xA30801B0u

//! Register Reset Value
#define TMI2_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_WRITE_PERMISSION_11 Register TMI2_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TMI2_PM_WRITE_PERMISSION_11 0x801B8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_WRITE_PERMISSION_11 0xA30801B8u

//! Register Reset Value
#define TMI2_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_ADDR_MATCH_12 Register TMI2_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TMI2_PM_ADDR_MATCH_12 0x801C0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ADDR_MATCH_12 0xA30801C0u

//! Register Reset Value
#define TMI2_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMI2_PM_REQ_INFO_PERMISSION_12 Register TMI2_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TMI2_PM_REQ_INFO_PERMISSION_12 0x801C8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_REQ_INFO_PERMISSION_12 0xA30801C8u

//! Register Reset Value
#define TMI2_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_READ_PERMISSION_12 Register TMI2_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TMI2_PM_READ_PERMISSION_12 0x801D0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_READ_PERMISSION_12 0xA30801D0u

//! Register Reset Value
#define TMI2_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_WRITE_PERMISSION_12 Register TMI2_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TMI2_PM_WRITE_PERMISSION_12 0x801D8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_WRITE_PERMISSION_12 0xA30801D8u

//! Register Reset Value
#define TMI2_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_ADDR_MATCH_13 Register TMI2_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TMI2_PM_ADDR_MATCH_13 0x801E0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ADDR_MATCH_13 0xA30801E0u

//! Register Reset Value
#define TMI2_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMI2_PM_REQ_INFO_PERMISSION_13 Register TMI2_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TMI2_PM_REQ_INFO_PERMISSION_13 0x801E8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_REQ_INFO_PERMISSION_13 0xA30801E8u

//! Register Reset Value
#define TMI2_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_READ_PERMISSION_13 Register TMI2_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TMI2_PM_READ_PERMISSION_13 0x801F0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_READ_PERMISSION_13 0xA30801F0u

//! Register Reset Value
#define TMI2_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_WRITE_PERMISSION_13 Register TMI2_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TMI2_PM_WRITE_PERMISSION_13 0x801F8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_WRITE_PERMISSION_13 0xA30801F8u

//! Register Reset Value
#define TMI2_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_ADDR_MATCH_14 Register TMI2_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TMI2_PM_ADDR_MATCH_14 0x80200
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ADDR_MATCH_14 0xA3080200u

//! Register Reset Value
#define TMI2_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMI2_PM_REQ_INFO_PERMISSION_14 Register TMI2_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TMI2_PM_REQ_INFO_PERMISSION_14 0x80208
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_REQ_INFO_PERMISSION_14 0xA3080208u

//! Register Reset Value
#define TMI2_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_READ_PERMISSION_14 Register TMI2_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TMI2_PM_READ_PERMISSION_14 0x80210
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_READ_PERMISSION_14 0xA3080210u

//! Register Reset Value
#define TMI2_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_WRITE_PERMISSION_14 Register TMI2_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TMI2_PM_WRITE_PERMISSION_14 0x80218
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_WRITE_PERMISSION_14 0xA3080218u

//! Register Reset Value
#define TMI2_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_ADDR_MATCH_15 Register TMI2_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TMI2_PM_ADDR_MATCH_15 0x80220
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_ADDR_MATCH_15 0xA3080220u

//! Register Reset Value
#define TMI2_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMI2_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TMI2_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TMI2_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMI2_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMI2_PM_REQ_INFO_PERMISSION_15 Register TMI2_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TMI2_PM_REQ_INFO_PERMISSION_15 0x80228
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_REQ_INFO_PERMISSION_15 0xA3080228u

//! Register Reset Value
#define TMI2_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMI2_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_READ_PERMISSION_15 Register TMI2_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TMI2_PM_READ_PERMISSION_15 0x80230
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_READ_PERMISSION_15 0xA3080230u

//! Register Reset Value
#define TMI2_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMI2_PM_WRITE_PERMISSION_15 Register TMI2_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TMI2_PM_WRITE_PERMISSION_15 0x80238
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMI2_PM_WRITE_PERMISSION_15 0xA3080238u

//! Register Reset Value
#define TMI2_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMI2_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_ERROR_LOG Register TTOE_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TTOE_PM_ERROR_LOG 0x80420
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ERROR_LOG 0xA3080420u

//! Register Reset Value
#define TTOE_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TTOE_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TTOE_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TTOE_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TTOE_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TTOE_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TTOE_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TTOE_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TTOE_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TTOE_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TTOE_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TTOE_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TTOE_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TTOE_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TTOE_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TTOE_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TTOE_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TTOE_PM_CONTROL Register TTOE_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TTOE_PM_CONTROL 0x80428
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_CONTROL 0xA3080428u

//! Register Reset Value
#define TTOE_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TTOE_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TTOE_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TTOE_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TTOE_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TTOE_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TTOE_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TTOE_PM_ERROR_CLEAR_SINGLE Register TTOE_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TTOE_PM_ERROR_CLEAR_SINGLE 0x80430
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ERROR_CLEAR_SINGLE 0xA3080430u

//! Register Reset Value
#define TTOE_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TTOE_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TTOE_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TTOE_PM_ERROR_CLEAR_MULTI Register TTOE_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TTOE_PM_ERROR_CLEAR_MULTI 0x80438
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ERROR_CLEAR_MULTI 0xA3080438u

//! Register Reset Value
#define TTOE_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TTOE_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TTOE_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TTOE_PM_REQ_INFO_PERMISSION_0 Register TTOE_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TTOE_PM_REQ_INFO_PERMISSION_0 0x80448
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_REQ_INFO_PERMISSION_0 0xA3080448u

//! Register Reset Value
#define TTOE_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_READ_PERMISSION_0 Register TTOE_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TTOE_PM_READ_PERMISSION_0 0x80450
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_READ_PERMISSION_0 0xA3080450u

//! Register Reset Value
#define TTOE_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_WRITE_PERMISSION_0 Register TTOE_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TTOE_PM_WRITE_PERMISSION_0 0x80458
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_WRITE_PERMISSION_0 0xA3080458u

//! Register Reset Value
#define TTOE_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_ADDR_MATCH_1 Register TTOE_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TTOE_PM_ADDR_MATCH_1 0x80460
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ADDR_MATCH_1 0xA3080460u

//! Register Reset Value
#define TTOE_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TTOE_PM_REQ_INFO_PERMISSION_1 Register TTOE_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TTOE_PM_REQ_INFO_PERMISSION_1 0x80468
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_REQ_INFO_PERMISSION_1 0xA3080468u

//! Register Reset Value
#define TTOE_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_READ_PERMISSION_1 Register TTOE_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TTOE_PM_READ_PERMISSION_1 0x80470
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_READ_PERMISSION_1 0xA3080470u

//! Register Reset Value
#define TTOE_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_WRITE_PERMISSION_1 Register TTOE_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TTOE_PM_WRITE_PERMISSION_1 0x80478
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_WRITE_PERMISSION_1 0xA3080478u

//! Register Reset Value
#define TTOE_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_ADDR_MATCH_2 Register TTOE_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TTOE_PM_ADDR_MATCH_2 0x80480
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ADDR_MATCH_2 0xA3080480u

//! Register Reset Value
#define TTOE_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TTOE_PM_REQ_INFO_PERMISSION_2 Register TTOE_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TTOE_PM_REQ_INFO_PERMISSION_2 0x80488
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_REQ_INFO_PERMISSION_2 0xA3080488u

//! Register Reset Value
#define TTOE_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_READ_PERMISSION_2 Register TTOE_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TTOE_PM_READ_PERMISSION_2 0x80490
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_READ_PERMISSION_2 0xA3080490u

//! Register Reset Value
#define TTOE_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_WRITE_PERMISSION_2 Register TTOE_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TTOE_PM_WRITE_PERMISSION_2 0x80498
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_WRITE_PERMISSION_2 0xA3080498u

//! Register Reset Value
#define TTOE_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_ADDR_MATCH_3 Register TTOE_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TTOE_PM_ADDR_MATCH_3 0x804A0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ADDR_MATCH_3 0xA30804A0u

//! Register Reset Value
#define TTOE_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TTOE_PM_REQ_INFO_PERMISSION_3 Register TTOE_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TTOE_PM_REQ_INFO_PERMISSION_3 0x804A8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_REQ_INFO_PERMISSION_3 0xA30804A8u

//! Register Reset Value
#define TTOE_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_READ_PERMISSION_3 Register TTOE_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TTOE_PM_READ_PERMISSION_3 0x804B0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_READ_PERMISSION_3 0xA30804B0u

//! Register Reset Value
#define TTOE_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_WRITE_PERMISSION_3 Register TTOE_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TTOE_PM_WRITE_PERMISSION_3 0x804B8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_WRITE_PERMISSION_3 0xA30804B8u

//! Register Reset Value
#define TTOE_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_ADDR_MATCH_4 Register TTOE_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TTOE_PM_ADDR_MATCH_4 0x804C0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ADDR_MATCH_4 0xA30804C0u

//! Register Reset Value
#define TTOE_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TTOE_PM_REQ_INFO_PERMISSION_4 Register TTOE_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TTOE_PM_REQ_INFO_PERMISSION_4 0x804C8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_REQ_INFO_PERMISSION_4 0xA30804C8u

//! Register Reset Value
#define TTOE_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_READ_PERMISSION_4 Register TTOE_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TTOE_PM_READ_PERMISSION_4 0x804D0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_READ_PERMISSION_4 0xA30804D0u

//! Register Reset Value
#define TTOE_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_WRITE_PERMISSION_4 Register TTOE_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TTOE_PM_WRITE_PERMISSION_4 0x804D8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_WRITE_PERMISSION_4 0xA30804D8u

//! Register Reset Value
#define TTOE_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_ADDR_MATCH_5 Register TTOE_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TTOE_PM_ADDR_MATCH_5 0x804E0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ADDR_MATCH_5 0xA30804E0u

//! Register Reset Value
#define TTOE_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TTOE_PM_REQ_INFO_PERMISSION_5 Register TTOE_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TTOE_PM_REQ_INFO_PERMISSION_5 0x804E8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_REQ_INFO_PERMISSION_5 0xA30804E8u

//! Register Reset Value
#define TTOE_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_READ_PERMISSION_5 Register TTOE_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TTOE_PM_READ_PERMISSION_5 0x804F0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_READ_PERMISSION_5 0xA30804F0u

//! Register Reset Value
#define TTOE_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_WRITE_PERMISSION_5 Register TTOE_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TTOE_PM_WRITE_PERMISSION_5 0x804F8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_WRITE_PERMISSION_5 0xA30804F8u

//! Register Reset Value
#define TTOE_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_ADDR_MATCH_6 Register TTOE_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TTOE_PM_ADDR_MATCH_6 0x80500
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ADDR_MATCH_6 0xA3080500u

//! Register Reset Value
#define TTOE_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TTOE_PM_REQ_INFO_PERMISSION_6 Register TTOE_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TTOE_PM_REQ_INFO_PERMISSION_6 0x80508
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_REQ_INFO_PERMISSION_6 0xA3080508u

//! Register Reset Value
#define TTOE_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_READ_PERMISSION_6 Register TTOE_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TTOE_PM_READ_PERMISSION_6 0x80510
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_READ_PERMISSION_6 0xA3080510u

//! Register Reset Value
#define TTOE_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_WRITE_PERMISSION_6 Register TTOE_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TTOE_PM_WRITE_PERMISSION_6 0x80518
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_WRITE_PERMISSION_6 0xA3080518u

//! Register Reset Value
#define TTOE_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_ADDR_MATCH_7 Register TTOE_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TTOE_PM_ADDR_MATCH_7 0x80520
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ADDR_MATCH_7 0xA3080520u

//! Register Reset Value
#define TTOE_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TTOE_PM_REQ_INFO_PERMISSION_7 Register TTOE_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TTOE_PM_REQ_INFO_PERMISSION_7 0x80528
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_REQ_INFO_PERMISSION_7 0xA3080528u

//! Register Reset Value
#define TTOE_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_READ_PERMISSION_7 Register TTOE_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TTOE_PM_READ_PERMISSION_7 0x80530
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_READ_PERMISSION_7 0xA3080530u

//! Register Reset Value
#define TTOE_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_WRITE_PERMISSION_7 Register TTOE_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TTOE_PM_WRITE_PERMISSION_7 0x80538
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_WRITE_PERMISSION_7 0xA3080538u

//! Register Reset Value
#define TTOE_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_ADDR_MATCH_8 Register TTOE_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TTOE_PM_ADDR_MATCH_8 0x80540
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ADDR_MATCH_8 0xA3080540u

//! Register Reset Value
#define TTOE_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TTOE_PM_REQ_INFO_PERMISSION_8 Register TTOE_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TTOE_PM_REQ_INFO_PERMISSION_8 0x80548
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_REQ_INFO_PERMISSION_8 0xA3080548u

//! Register Reset Value
#define TTOE_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_READ_PERMISSION_8 Register TTOE_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TTOE_PM_READ_PERMISSION_8 0x80550
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_READ_PERMISSION_8 0xA3080550u

//! Register Reset Value
#define TTOE_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_WRITE_PERMISSION_8 Register TTOE_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TTOE_PM_WRITE_PERMISSION_8 0x80558
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_WRITE_PERMISSION_8 0xA3080558u

//! Register Reset Value
#define TTOE_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_ADDR_MATCH_9 Register TTOE_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TTOE_PM_ADDR_MATCH_9 0x80560
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ADDR_MATCH_9 0xA3080560u

//! Register Reset Value
#define TTOE_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TTOE_PM_REQ_INFO_PERMISSION_9 Register TTOE_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TTOE_PM_REQ_INFO_PERMISSION_9 0x80568
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_REQ_INFO_PERMISSION_9 0xA3080568u

//! Register Reset Value
#define TTOE_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_READ_PERMISSION_9 Register TTOE_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TTOE_PM_READ_PERMISSION_9 0x80570
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_READ_PERMISSION_9 0xA3080570u

//! Register Reset Value
#define TTOE_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_WRITE_PERMISSION_9 Register TTOE_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TTOE_PM_WRITE_PERMISSION_9 0x80578
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_WRITE_PERMISSION_9 0xA3080578u

//! Register Reset Value
#define TTOE_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_ADDR_MATCH_10 Register TTOE_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TTOE_PM_ADDR_MATCH_10 0x80580
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ADDR_MATCH_10 0xA3080580u

//! Register Reset Value
#define TTOE_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TTOE_PM_REQ_INFO_PERMISSION_10 Register TTOE_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TTOE_PM_REQ_INFO_PERMISSION_10 0x80588
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_REQ_INFO_PERMISSION_10 0xA3080588u

//! Register Reset Value
#define TTOE_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_READ_PERMISSION_10 Register TTOE_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TTOE_PM_READ_PERMISSION_10 0x80590
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_READ_PERMISSION_10 0xA3080590u

//! Register Reset Value
#define TTOE_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_WRITE_PERMISSION_10 Register TTOE_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TTOE_PM_WRITE_PERMISSION_10 0x80598
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_WRITE_PERMISSION_10 0xA3080598u

//! Register Reset Value
#define TTOE_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_ADDR_MATCH_11 Register TTOE_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TTOE_PM_ADDR_MATCH_11 0x805A0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ADDR_MATCH_11 0xA30805A0u

//! Register Reset Value
#define TTOE_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TTOE_PM_REQ_INFO_PERMISSION_11 Register TTOE_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TTOE_PM_REQ_INFO_PERMISSION_11 0x805A8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_REQ_INFO_PERMISSION_11 0xA30805A8u

//! Register Reset Value
#define TTOE_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_READ_PERMISSION_11 Register TTOE_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TTOE_PM_READ_PERMISSION_11 0x805B0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_READ_PERMISSION_11 0xA30805B0u

//! Register Reset Value
#define TTOE_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_WRITE_PERMISSION_11 Register TTOE_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TTOE_PM_WRITE_PERMISSION_11 0x805B8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_WRITE_PERMISSION_11 0xA30805B8u

//! Register Reset Value
#define TTOE_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_ADDR_MATCH_12 Register TTOE_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TTOE_PM_ADDR_MATCH_12 0x805C0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ADDR_MATCH_12 0xA30805C0u

//! Register Reset Value
#define TTOE_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TTOE_PM_REQ_INFO_PERMISSION_12 Register TTOE_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TTOE_PM_REQ_INFO_PERMISSION_12 0x805C8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_REQ_INFO_PERMISSION_12 0xA30805C8u

//! Register Reset Value
#define TTOE_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_READ_PERMISSION_12 Register TTOE_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TTOE_PM_READ_PERMISSION_12 0x805D0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_READ_PERMISSION_12 0xA30805D0u

//! Register Reset Value
#define TTOE_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_WRITE_PERMISSION_12 Register TTOE_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TTOE_PM_WRITE_PERMISSION_12 0x805D8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_WRITE_PERMISSION_12 0xA30805D8u

//! Register Reset Value
#define TTOE_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_ADDR_MATCH_13 Register TTOE_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TTOE_PM_ADDR_MATCH_13 0x805E0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ADDR_MATCH_13 0xA30805E0u

//! Register Reset Value
#define TTOE_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TTOE_PM_REQ_INFO_PERMISSION_13 Register TTOE_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TTOE_PM_REQ_INFO_PERMISSION_13 0x805E8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_REQ_INFO_PERMISSION_13 0xA30805E8u

//! Register Reset Value
#define TTOE_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_READ_PERMISSION_13 Register TTOE_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TTOE_PM_READ_PERMISSION_13 0x805F0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_READ_PERMISSION_13 0xA30805F0u

//! Register Reset Value
#define TTOE_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_WRITE_PERMISSION_13 Register TTOE_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TTOE_PM_WRITE_PERMISSION_13 0x805F8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_WRITE_PERMISSION_13 0xA30805F8u

//! Register Reset Value
#define TTOE_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_ADDR_MATCH_14 Register TTOE_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TTOE_PM_ADDR_MATCH_14 0x80600
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ADDR_MATCH_14 0xA3080600u

//! Register Reset Value
#define TTOE_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TTOE_PM_REQ_INFO_PERMISSION_14 Register TTOE_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TTOE_PM_REQ_INFO_PERMISSION_14 0x80608
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_REQ_INFO_PERMISSION_14 0xA3080608u

//! Register Reset Value
#define TTOE_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_READ_PERMISSION_14 Register TTOE_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TTOE_PM_READ_PERMISSION_14 0x80610
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_READ_PERMISSION_14 0xA3080610u

//! Register Reset Value
#define TTOE_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_WRITE_PERMISSION_14 Register TTOE_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TTOE_PM_WRITE_PERMISSION_14 0x80618
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_WRITE_PERMISSION_14 0xA3080618u

//! Register Reset Value
#define TTOE_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_ADDR_MATCH_15 Register TTOE_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TTOE_PM_ADDR_MATCH_15 0x80620
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_ADDR_MATCH_15 0xA3080620u

//! Register Reset Value
#define TTOE_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TTOE_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TTOE_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TTOE_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TTOE_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TTOE_PM_REQ_INFO_PERMISSION_15 Register TTOE_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TTOE_PM_REQ_INFO_PERMISSION_15 0x80628
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_REQ_INFO_PERMISSION_15 0xA3080628u

//! Register Reset Value
#define TTOE_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TTOE_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_READ_PERMISSION_15 Register TTOE_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TTOE_PM_READ_PERMISSION_15 0x80630
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_READ_PERMISSION_15 0xA3080630u

//! Register Reset Value
#define TTOE_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TTOE_PM_WRITE_PERMISSION_15 Register TTOE_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TTOE_PM_WRITE_PERMISSION_15 0x80638
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TTOE_PM_WRITE_PERMISSION_15 0xA3080638u

//! Register Reset Value
#define TTOE_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TTOE_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_ERROR_LOG Register TMPE_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TMPE_PM_ERROR_LOG 0x80820
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ERROR_LOG 0xA3080820u

//! Register Reset Value
#define TMPE_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TMPE_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TMPE_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TMPE_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TMPE_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TMPE_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TMPE_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TMPE_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TMPE_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TMPE_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TMPE_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TMPE_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TMPE_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TMPE_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TMPE_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TMPE_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TMPE_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TMPE_PM_CONTROL Register TMPE_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TMPE_PM_CONTROL 0x80828
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_CONTROL 0xA3080828u

//! Register Reset Value
#define TMPE_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TMPE_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TMPE_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TMPE_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TMPE_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TMPE_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TMPE_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TMPE_PM_ERROR_CLEAR_SINGLE Register TMPE_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TMPE_PM_ERROR_CLEAR_SINGLE 0x80830
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ERROR_CLEAR_SINGLE 0xA3080830u

//! Register Reset Value
#define TMPE_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TMPE_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TMPE_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TMPE_PM_ERROR_CLEAR_MULTI Register TMPE_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TMPE_PM_ERROR_CLEAR_MULTI 0x80838
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ERROR_CLEAR_MULTI 0xA3080838u

//! Register Reset Value
#define TMPE_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TMPE_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TMPE_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TMPE_PM_REQ_INFO_PERMISSION_0 Register TMPE_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TMPE_PM_REQ_INFO_PERMISSION_0 0x80848
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_REQ_INFO_PERMISSION_0 0xA3080848u

//! Register Reset Value
#define TMPE_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_READ_PERMISSION_0 Register TMPE_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TMPE_PM_READ_PERMISSION_0 0x80850
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_READ_PERMISSION_0 0xA3080850u

//! Register Reset Value
#define TMPE_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_WRITE_PERMISSION_0 Register TMPE_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TMPE_PM_WRITE_PERMISSION_0 0x80858
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_WRITE_PERMISSION_0 0xA3080858u

//! Register Reset Value
#define TMPE_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_ADDR_MATCH_1 Register TMPE_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TMPE_PM_ADDR_MATCH_1 0x80860
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ADDR_MATCH_1 0xA3080860u

//! Register Reset Value
#define TMPE_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMPE_PM_REQ_INFO_PERMISSION_1 Register TMPE_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TMPE_PM_REQ_INFO_PERMISSION_1 0x80868
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_REQ_INFO_PERMISSION_1 0xA3080868u

//! Register Reset Value
#define TMPE_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_READ_PERMISSION_1 Register TMPE_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TMPE_PM_READ_PERMISSION_1 0x80870
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_READ_PERMISSION_1 0xA3080870u

//! Register Reset Value
#define TMPE_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_WRITE_PERMISSION_1 Register TMPE_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TMPE_PM_WRITE_PERMISSION_1 0x80878
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_WRITE_PERMISSION_1 0xA3080878u

//! Register Reset Value
#define TMPE_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_ADDR_MATCH_2 Register TMPE_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TMPE_PM_ADDR_MATCH_2 0x80880
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ADDR_MATCH_2 0xA3080880u

//! Register Reset Value
#define TMPE_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMPE_PM_REQ_INFO_PERMISSION_2 Register TMPE_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TMPE_PM_REQ_INFO_PERMISSION_2 0x80888
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_REQ_INFO_PERMISSION_2 0xA3080888u

//! Register Reset Value
#define TMPE_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_READ_PERMISSION_2 Register TMPE_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TMPE_PM_READ_PERMISSION_2 0x80890
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_READ_PERMISSION_2 0xA3080890u

//! Register Reset Value
#define TMPE_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_WRITE_PERMISSION_2 Register TMPE_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TMPE_PM_WRITE_PERMISSION_2 0x80898
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_WRITE_PERMISSION_2 0xA3080898u

//! Register Reset Value
#define TMPE_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_ADDR_MATCH_3 Register TMPE_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TMPE_PM_ADDR_MATCH_3 0x808A0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ADDR_MATCH_3 0xA30808A0u

//! Register Reset Value
#define TMPE_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMPE_PM_REQ_INFO_PERMISSION_3 Register TMPE_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TMPE_PM_REQ_INFO_PERMISSION_3 0x808A8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_REQ_INFO_PERMISSION_3 0xA30808A8u

//! Register Reset Value
#define TMPE_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_READ_PERMISSION_3 Register TMPE_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TMPE_PM_READ_PERMISSION_3 0x808B0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_READ_PERMISSION_3 0xA30808B0u

//! Register Reset Value
#define TMPE_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_WRITE_PERMISSION_3 Register TMPE_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TMPE_PM_WRITE_PERMISSION_3 0x808B8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_WRITE_PERMISSION_3 0xA30808B8u

//! Register Reset Value
#define TMPE_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_ADDR_MATCH_4 Register TMPE_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TMPE_PM_ADDR_MATCH_4 0x808C0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ADDR_MATCH_4 0xA30808C0u

//! Register Reset Value
#define TMPE_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMPE_PM_REQ_INFO_PERMISSION_4 Register TMPE_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TMPE_PM_REQ_INFO_PERMISSION_4 0x808C8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_REQ_INFO_PERMISSION_4 0xA30808C8u

//! Register Reset Value
#define TMPE_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_READ_PERMISSION_4 Register TMPE_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TMPE_PM_READ_PERMISSION_4 0x808D0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_READ_PERMISSION_4 0xA30808D0u

//! Register Reset Value
#define TMPE_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_WRITE_PERMISSION_4 Register TMPE_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TMPE_PM_WRITE_PERMISSION_4 0x808D8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_WRITE_PERMISSION_4 0xA30808D8u

//! Register Reset Value
#define TMPE_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_ADDR_MATCH_5 Register TMPE_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TMPE_PM_ADDR_MATCH_5 0x808E0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ADDR_MATCH_5 0xA30808E0u

//! Register Reset Value
#define TMPE_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMPE_PM_REQ_INFO_PERMISSION_5 Register TMPE_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TMPE_PM_REQ_INFO_PERMISSION_5 0x808E8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_REQ_INFO_PERMISSION_5 0xA30808E8u

//! Register Reset Value
#define TMPE_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_READ_PERMISSION_5 Register TMPE_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TMPE_PM_READ_PERMISSION_5 0x808F0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_READ_PERMISSION_5 0xA30808F0u

//! Register Reset Value
#define TMPE_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_WRITE_PERMISSION_5 Register TMPE_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TMPE_PM_WRITE_PERMISSION_5 0x808F8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_WRITE_PERMISSION_5 0xA30808F8u

//! Register Reset Value
#define TMPE_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_ADDR_MATCH_6 Register TMPE_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TMPE_PM_ADDR_MATCH_6 0x80900
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ADDR_MATCH_6 0xA3080900u

//! Register Reset Value
#define TMPE_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMPE_PM_REQ_INFO_PERMISSION_6 Register TMPE_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TMPE_PM_REQ_INFO_PERMISSION_6 0x80908
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_REQ_INFO_PERMISSION_6 0xA3080908u

//! Register Reset Value
#define TMPE_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_READ_PERMISSION_6 Register TMPE_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TMPE_PM_READ_PERMISSION_6 0x80910
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_READ_PERMISSION_6 0xA3080910u

//! Register Reset Value
#define TMPE_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_WRITE_PERMISSION_6 Register TMPE_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TMPE_PM_WRITE_PERMISSION_6 0x80918
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_WRITE_PERMISSION_6 0xA3080918u

//! Register Reset Value
#define TMPE_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_ADDR_MATCH_7 Register TMPE_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TMPE_PM_ADDR_MATCH_7 0x80920
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ADDR_MATCH_7 0xA3080920u

//! Register Reset Value
#define TMPE_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMPE_PM_REQ_INFO_PERMISSION_7 Register TMPE_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TMPE_PM_REQ_INFO_PERMISSION_7 0x80928
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_REQ_INFO_PERMISSION_7 0xA3080928u

//! Register Reset Value
#define TMPE_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_READ_PERMISSION_7 Register TMPE_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TMPE_PM_READ_PERMISSION_7 0x80930
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_READ_PERMISSION_7 0xA3080930u

//! Register Reset Value
#define TMPE_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_WRITE_PERMISSION_7 Register TMPE_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TMPE_PM_WRITE_PERMISSION_7 0x80938
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_WRITE_PERMISSION_7 0xA3080938u

//! Register Reset Value
#define TMPE_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_ADDR_MATCH_8 Register TMPE_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TMPE_PM_ADDR_MATCH_8 0x80940
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ADDR_MATCH_8 0xA3080940u

//! Register Reset Value
#define TMPE_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMPE_PM_REQ_INFO_PERMISSION_8 Register TMPE_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TMPE_PM_REQ_INFO_PERMISSION_8 0x80948
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_REQ_INFO_PERMISSION_8 0xA3080948u

//! Register Reset Value
#define TMPE_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_READ_PERMISSION_8 Register TMPE_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TMPE_PM_READ_PERMISSION_8 0x80950
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_READ_PERMISSION_8 0xA3080950u

//! Register Reset Value
#define TMPE_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_WRITE_PERMISSION_8 Register TMPE_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TMPE_PM_WRITE_PERMISSION_8 0x80958
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_WRITE_PERMISSION_8 0xA3080958u

//! Register Reset Value
#define TMPE_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_ADDR_MATCH_9 Register TMPE_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TMPE_PM_ADDR_MATCH_9 0x80960
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ADDR_MATCH_9 0xA3080960u

//! Register Reset Value
#define TMPE_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMPE_PM_REQ_INFO_PERMISSION_9 Register TMPE_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TMPE_PM_REQ_INFO_PERMISSION_9 0x80968
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_REQ_INFO_PERMISSION_9 0xA3080968u

//! Register Reset Value
#define TMPE_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_READ_PERMISSION_9 Register TMPE_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TMPE_PM_READ_PERMISSION_9 0x80970
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_READ_PERMISSION_9 0xA3080970u

//! Register Reset Value
#define TMPE_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_WRITE_PERMISSION_9 Register TMPE_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TMPE_PM_WRITE_PERMISSION_9 0x80978
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_WRITE_PERMISSION_9 0xA3080978u

//! Register Reset Value
#define TMPE_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_ADDR_MATCH_10 Register TMPE_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TMPE_PM_ADDR_MATCH_10 0x80980
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ADDR_MATCH_10 0xA3080980u

//! Register Reset Value
#define TMPE_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMPE_PM_REQ_INFO_PERMISSION_10 Register TMPE_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TMPE_PM_REQ_INFO_PERMISSION_10 0x80988
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_REQ_INFO_PERMISSION_10 0xA3080988u

//! Register Reset Value
#define TMPE_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_READ_PERMISSION_10 Register TMPE_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TMPE_PM_READ_PERMISSION_10 0x80990
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_READ_PERMISSION_10 0xA3080990u

//! Register Reset Value
#define TMPE_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_WRITE_PERMISSION_10 Register TMPE_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TMPE_PM_WRITE_PERMISSION_10 0x80998
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_WRITE_PERMISSION_10 0xA3080998u

//! Register Reset Value
#define TMPE_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_ADDR_MATCH_11 Register TMPE_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TMPE_PM_ADDR_MATCH_11 0x809A0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ADDR_MATCH_11 0xA30809A0u

//! Register Reset Value
#define TMPE_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMPE_PM_REQ_INFO_PERMISSION_11 Register TMPE_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TMPE_PM_REQ_INFO_PERMISSION_11 0x809A8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_REQ_INFO_PERMISSION_11 0xA30809A8u

//! Register Reset Value
#define TMPE_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_READ_PERMISSION_11 Register TMPE_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TMPE_PM_READ_PERMISSION_11 0x809B0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_READ_PERMISSION_11 0xA30809B0u

//! Register Reset Value
#define TMPE_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_WRITE_PERMISSION_11 Register TMPE_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TMPE_PM_WRITE_PERMISSION_11 0x809B8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_WRITE_PERMISSION_11 0xA30809B8u

//! Register Reset Value
#define TMPE_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_ADDR_MATCH_12 Register TMPE_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TMPE_PM_ADDR_MATCH_12 0x809C0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ADDR_MATCH_12 0xA30809C0u

//! Register Reset Value
#define TMPE_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMPE_PM_REQ_INFO_PERMISSION_12 Register TMPE_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TMPE_PM_REQ_INFO_PERMISSION_12 0x809C8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_REQ_INFO_PERMISSION_12 0xA30809C8u

//! Register Reset Value
#define TMPE_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_READ_PERMISSION_12 Register TMPE_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TMPE_PM_READ_PERMISSION_12 0x809D0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_READ_PERMISSION_12 0xA30809D0u

//! Register Reset Value
#define TMPE_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_WRITE_PERMISSION_12 Register TMPE_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TMPE_PM_WRITE_PERMISSION_12 0x809D8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_WRITE_PERMISSION_12 0xA30809D8u

//! Register Reset Value
#define TMPE_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_ADDR_MATCH_13 Register TMPE_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TMPE_PM_ADDR_MATCH_13 0x809E0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ADDR_MATCH_13 0xA30809E0u

//! Register Reset Value
#define TMPE_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMPE_PM_REQ_INFO_PERMISSION_13 Register TMPE_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TMPE_PM_REQ_INFO_PERMISSION_13 0x809E8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_REQ_INFO_PERMISSION_13 0xA30809E8u

//! Register Reset Value
#define TMPE_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_READ_PERMISSION_13 Register TMPE_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TMPE_PM_READ_PERMISSION_13 0x809F0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_READ_PERMISSION_13 0xA30809F0u

//! Register Reset Value
#define TMPE_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_WRITE_PERMISSION_13 Register TMPE_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TMPE_PM_WRITE_PERMISSION_13 0x809F8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_WRITE_PERMISSION_13 0xA30809F8u

//! Register Reset Value
#define TMPE_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_ADDR_MATCH_14 Register TMPE_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TMPE_PM_ADDR_MATCH_14 0x80A00
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ADDR_MATCH_14 0xA3080A00u

//! Register Reset Value
#define TMPE_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMPE_PM_REQ_INFO_PERMISSION_14 Register TMPE_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TMPE_PM_REQ_INFO_PERMISSION_14 0x80A08
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_REQ_INFO_PERMISSION_14 0xA3080A08u

//! Register Reset Value
#define TMPE_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_READ_PERMISSION_14 Register TMPE_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TMPE_PM_READ_PERMISSION_14 0x80A10
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_READ_PERMISSION_14 0xA3080A10u

//! Register Reset Value
#define TMPE_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_WRITE_PERMISSION_14 Register TMPE_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TMPE_PM_WRITE_PERMISSION_14 0x80A18
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_WRITE_PERMISSION_14 0xA3080A18u

//! Register Reset Value
#define TMPE_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_ADDR_MATCH_15 Register TMPE_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TMPE_PM_ADDR_MATCH_15 0x80A20
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_ADDR_MATCH_15 0xA3080A20u

//! Register Reset Value
#define TMPE_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TMPE_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TMPE_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TMPE_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TMPE_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TMPE_PM_REQ_INFO_PERMISSION_15 Register TMPE_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TMPE_PM_REQ_INFO_PERMISSION_15 0x80A28
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_REQ_INFO_PERMISSION_15 0xA3080A28u

//! Register Reset Value
#define TMPE_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TMPE_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_READ_PERMISSION_15 Register TMPE_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TMPE_PM_READ_PERMISSION_15 0x80A30
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_READ_PERMISSION_15 0xA3080A30u

//! Register Reset Value
#define TMPE_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TMPE_PM_WRITE_PERMISSION_15 Register TMPE_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TMPE_PM_WRITE_PERMISSION_15 0x80A38
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TMPE_PM_WRITE_PERMISSION_15 0xA3080A38u

//! Register Reset Value
#define TMPE_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TMPE_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_ERROR_LOG Register TREG7_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TREG7_PM_ERROR_LOG 0x80C20
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ERROR_LOG 0xA3080C20u

//! Register Reset Value
#define TREG7_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TREG7_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TREG7_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TREG7_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TREG7_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TREG7_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TREG7_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TREG7_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TREG7_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TREG7_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TREG7_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TREG7_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TREG7_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TREG7_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TREG7_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TREG7_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TREG7_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TREG7_PM_CONTROL Register TREG7_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TREG7_PM_CONTROL 0x80C28
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_CONTROL 0xA3080C28u

//! Register Reset Value
#define TREG7_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TREG7_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TREG7_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TREG7_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TREG7_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TREG7_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TREG7_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TREG7_PM_ERROR_CLEAR_SINGLE Register TREG7_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TREG7_PM_ERROR_CLEAR_SINGLE 0x80C30
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ERROR_CLEAR_SINGLE 0xA3080C30u

//! Register Reset Value
#define TREG7_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TREG7_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TREG7_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TREG7_PM_ERROR_CLEAR_MULTI Register TREG7_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TREG7_PM_ERROR_CLEAR_MULTI 0x80C38
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ERROR_CLEAR_MULTI 0xA3080C38u

//! Register Reset Value
#define TREG7_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TREG7_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TREG7_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TREG7_PM_REQ_INFO_PERMISSION_0 Register TREG7_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TREG7_PM_REQ_INFO_PERMISSION_0 0x80C48
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_REQ_INFO_PERMISSION_0 0xA3080C48u

//! Register Reset Value
#define TREG7_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_READ_PERMISSION_0 Register TREG7_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TREG7_PM_READ_PERMISSION_0 0x80C50
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_READ_PERMISSION_0 0xA3080C50u

//! Register Reset Value
#define TREG7_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TREG7_PM_WRITE_PERMISSION_0 Register TREG7_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TREG7_PM_WRITE_PERMISSION_0 0x80C58
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_WRITE_PERMISSION_0 0xA3080C58u

//! Register Reset Value
#define TREG7_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TREG7_PM_ADDR_MATCH_1 Register TREG7_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TREG7_PM_ADDR_MATCH_1 0x80C60
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ADDR_MATCH_1 0xA3080C60u

//! Register Reset Value
#define TREG7_PM_ADDR_MATCH_1_RST 0x0000000000080050u

//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG7_PM_REQ_INFO_PERMISSION_1 Register TREG7_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TREG7_PM_REQ_INFO_PERMISSION_1 0x80C68
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_REQ_INFO_PERMISSION_1 0xA3080C68u

//! Register Reset Value
#define TREG7_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_READ_PERMISSION_1 Register TREG7_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TREG7_PM_READ_PERMISSION_1 0x80C70
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_READ_PERMISSION_1 0xA3080C70u

//! Register Reset Value
#define TREG7_PM_READ_PERMISSION_1_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TREG7_PM_WRITE_PERMISSION_1 Register TREG7_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TREG7_PM_WRITE_PERMISSION_1 0x80C78
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_WRITE_PERMISSION_1 0xA3080C78u

//! Register Reset Value
#define TREG7_PM_WRITE_PERMISSION_1_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TREG7_PM_ADDR_MATCH_2 Register TREG7_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TREG7_PM_ADDR_MATCH_2 0x80C80
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ADDR_MATCH_2 0xA3080C80u

//! Register Reset Value
#define TREG7_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG7_PM_REQ_INFO_PERMISSION_2 Register TREG7_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TREG7_PM_REQ_INFO_PERMISSION_2 0x80C88
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_REQ_INFO_PERMISSION_2 0xA3080C88u

//! Register Reset Value
#define TREG7_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_READ_PERMISSION_2 Register TREG7_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TREG7_PM_READ_PERMISSION_2 0x80C90
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_READ_PERMISSION_2 0xA3080C90u

//! Register Reset Value
#define TREG7_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_WRITE_PERMISSION_2 Register TREG7_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TREG7_PM_WRITE_PERMISSION_2 0x80C98
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_WRITE_PERMISSION_2 0xA3080C98u

//! Register Reset Value
#define TREG7_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_ADDR_MATCH_3 Register TREG7_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TREG7_PM_ADDR_MATCH_3 0x80CA0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ADDR_MATCH_3 0xA3080CA0u

//! Register Reset Value
#define TREG7_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG7_PM_REQ_INFO_PERMISSION_3 Register TREG7_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TREG7_PM_REQ_INFO_PERMISSION_3 0x80CA8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_REQ_INFO_PERMISSION_3 0xA3080CA8u

//! Register Reset Value
#define TREG7_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_READ_PERMISSION_3 Register TREG7_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TREG7_PM_READ_PERMISSION_3 0x80CB0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_READ_PERMISSION_3 0xA3080CB0u

//! Register Reset Value
#define TREG7_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_WRITE_PERMISSION_3 Register TREG7_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TREG7_PM_WRITE_PERMISSION_3 0x80CB8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_WRITE_PERMISSION_3 0xA3080CB8u

//! Register Reset Value
#define TREG7_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_ADDR_MATCH_4 Register TREG7_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TREG7_PM_ADDR_MATCH_4 0x80CC0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ADDR_MATCH_4 0xA3080CC0u

//! Register Reset Value
#define TREG7_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG7_PM_REQ_INFO_PERMISSION_4 Register TREG7_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TREG7_PM_REQ_INFO_PERMISSION_4 0x80CC8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_REQ_INFO_PERMISSION_4 0xA3080CC8u

//! Register Reset Value
#define TREG7_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_READ_PERMISSION_4 Register TREG7_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TREG7_PM_READ_PERMISSION_4 0x80CD0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_READ_PERMISSION_4 0xA3080CD0u

//! Register Reset Value
#define TREG7_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_WRITE_PERMISSION_4 Register TREG7_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TREG7_PM_WRITE_PERMISSION_4 0x80CD8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_WRITE_PERMISSION_4 0xA3080CD8u

//! Register Reset Value
#define TREG7_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_ADDR_MATCH_5 Register TREG7_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TREG7_PM_ADDR_MATCH_5 0x80CE0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ADDR_MATCH_5 0xA3080CE0u

//! Register Reset Value
#define TREG7_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG7_PM_REQ_INFO_PERMISSION_5 Register TREG7_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TREG7_PM_REQ_INFO_PERMISSION_5 0x80CE8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_REQ_INFO_PERMISSION_5 0xA3080CE8u

//! Register Reset Value
#define TREG7_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_READ_PERMISSION_5 Register TREG7_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TREG7_PM_READ_PERMISSION_5 0x80CF0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_READ_PERMISSION_5 0xA3080CF0u

//! Register Reset Value
#define TREG7_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_WRITE_PERMISSION_5 Register TREG7_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TREG7_PM_WRITE_PERMISSION_5 0x80CF8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_WRITE_PERMISSION_5 0xA3080CF8u

//! Register Reset Value
#define TREG7_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_ADDR_MATCH_6 Register TREG7_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TREG7_PM_ADDR_MATCH_6 0x80D00
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ADDR_MATCH_6 0xA3080D00u

//! Register Reset Value
#define TREG7_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG7_PM_REQ_INFO_PERMISSION_6 Register TREG7_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TREG7_PM_REQ_INFO_PERMISSION_6 0x80D08
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_REQ_INFO_PERMISSION_6 0xA3080D08u

//! Register Reset Value
#define TREG7_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_READ_PERMISSION_6 Register TREG7_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TREG7_PM_READ_PERMISSION_6 0x80D10
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_READ_PERMISSION_6 0xA3080D10u

//! Register Reset Value
#define TREG7_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_WRITE_PERMISSION_6 Register TREG7_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TREG7_PM_WRITE_PERMISSION_6 0x80D18
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_WRITE_PERMISSION_6 0xA3080D18u

//! Register Reset Value
#define TREG7_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_ADDR_MATCH_7 Register TREG7_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TREG7_PM_ADDR_MATCH_7 0x80D20
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ADDR_MATCH_7 0xA3080D20u

//! Register Reset Value
#define TREG7_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG7_PM_REQ_INFO_PERMISSION_7 Register TREG7_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TREG7_PM_REQ_INFO_PERMISSION_7 0x80D28
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_REQ_INFO_PERMISSION_7 0xA3080D28u

//! Register Reset Value
#define TREG7_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_READ_PERMISSION_7 Register TREG7_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TREG7_PM_READ_PERMISSION_7 0x80D30
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_READ_PERMISSION_7 0xA3080D30u

//! Register Reset Value
#define TREG7_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_WRITE_PERMISSION_7 Register TREG7_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TREG7_PM_WRITE_PERMISSION_7 0x80D38
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_WRITE_PERMISSION_7 0xA3080D38u

//! Register Reset Value
#define TREG7_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_ADDR_MATCH_8 Register TREG7_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TREG7_PM_ADDR_MATCH_8 0x80D40
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ADDR_MATCH_8 0xA3080D40u

//! Register Reset Value
#define TREG7_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG7_PM_REQ_INFO_PERMISSION_8 Register TREG7_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TREG7_PM_REQ_INFO_PERMISSION_8 0x80D48
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_REQ_INFO_PERMISSION_8 0xA3080D48u

//! Register Reset Value
#define TREG7_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_READ_PERMISSION_8 Register TREG7_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TREG7_PM_READ_PERMISSION_8 0x80D50
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_READ_PERMISSION_8 0xA3080D50u

//! Register Reset Value
#define TREG7_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_WRITE_PERMISSION_8 Register TREG7_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TREG7_PM_WRITE_PERMISSION_8 0x80D58
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_WRITE_PERMISSION_8 0xA3080D58u

//! Register Reset Value
#define TREG7_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_ADDR_MATCH_9 Register TREG7_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TREG7_PM_ADDR_MATCH_9 0x80D60
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ADDR_MATCH_9 0xA3080D60u

//! Register Reset Value
#define TREG7_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG7_PM_REQ_INFO_PERMISSION_9 Register TREG7_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TREG7_PM_REQ_INFO_PERMISSION_9 0x80D68
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_REQ_INFO_PERMISSION_9 0xA3080D68u

//! Register Reset Value
#define TREG7_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_READ_PERMISSION_9 Register TREG7_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TREG7_PM_READ_PERMISSION_9 0x80D70
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_READ_PERMISSION_9 0xA3080D70u

//! Register Reset Value
#define TREG7_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_WRITE_PERMISSION_9 Register TREG7_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TREG7_PM_WRITE_PERMISSION_9 0x80D78
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_WRITE_PERMISSION_9 0xA3080D78u

//! Register Reset Value
#define TREG7_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_ADDR_MATCH_10 Register TREG7_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TREG7_PM_ADDR_MATCH_10 0x80D80
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ADDR_MATCH_10 0xA3080D80u

//! Register Reset Value
#define TREG7_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG7_PM_REQ_INFO_PERMISSION_10 Register TREG7_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TREG7_PM_REQ_INFO_PERMISSION_10 0x80D88
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_REQ_INFO_PERMISSION_10 0xA3080D88u

//! Register Reset Value
#define TREG7_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_READ_PERMISSION_10 Register TREG7_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TREG7_PM_READ_PERMISSION_10 0x80D90
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_READ_PERMISSION_10 0xA3080D90u

//! Register Reset Value
#define TREG7_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_WRITE_PERMISSION_10 Register TREG7_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TREG7_PM_WRITE_PERMISSION_10 0x80D98
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_WRITE_PERMISSION_10 0xA3080D98u

//! Register Reset Value
#define TREG7_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_ADDR_MATCH_11 Register TREG7_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TREG7_PM_ADDR_MATCH_11 0x80DA0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ADDR_MATCH_11 0xA3080DA0u

//! Register Reset Value
#define TREG7_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG7_PM_REQ_INFO_PERMISSION_11 Register TREG7_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TREG7_PM_REQ_INFO_PERMISSION_11 0x80DA8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_REQ_INFO_PERMISSION_11 0xA3080DA8u

//! Register Reset Value
#define TREG7_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_READ_PERMISSION_11 Register TREG7_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TREG7_PM_READ_PERMISSION_11 0x80DB0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_READ_PERMISSION_11 0xA3080DB0u

//! Register Reset Value
#define TREG7_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_WRITE_PERMISSION_11 Register TREG7_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TREG7_PM_WRITE_PERMISSION_11 0x80DB8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_WRITE_PERMISSION_11 0xA3080DB8u

//! Register Reset Value
#define TREG7_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_ADDR_MATCH_12 Register TREG7_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TREG7_PM_ADDR_MATCH_12 0x80DC0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ADDR_MATCH_12 0xA3080DC0u

//! Register Reset Value
#define TREG7_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG7_PM_REQ_INFO_PERMISSION_12 Register TREG7_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TREG7_PM_REQ_INFO_PERMISSION_12 0x80DC8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_REQ_INFO_PERMISSION_12 0xA3080DC8u

//! Register Reset Value
#define TREG7_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_READ_PERMISSION_12 Register TREG7_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TREG7_PM_READ_PERMISSION_12 0x80DD0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_READ_PERMISSION_12 0xA3080DD0u

//! Register Reset Value
#define TREG7_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_WRITE_PERMISSION_12 Register TREG7_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TREG7_PM_WRITE_PERMISSION_12 0x80DD8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_WRITE_PERMISSION_12 0xA3080DD8u

//! Register Reset Value
#define TREG7_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_ADDR_MATCH_13 Register TREG7_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TREG7_PM_ADDR_MATCH_13 0x80DE0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ADDR_MATCH_13 0xA3080DE0u

//! Register Reset Value
#define TREG7_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG7_PM_REQ_INFO_PERMISSION_13 Register TREG7_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TREG7_PM_REQ_INFO_PERMISSION_13 0x80DE8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_REQ_INFO_PERMISSION_13 0xA3080DE8u

//! Register Reset Value
#define TREG7_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_READ_PERMISSION_13 Register TREG7_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TREG7_PM_READ_PERMISSION_13 0x80DF0
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_READ_PERMISSION_13 0xA3080DF0u

//! Register Reset Value
#define TREG7_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_WRITE_PERMISSION_13 Register TREG7_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TREG7_PM_WRITE_PERMISSION_13 0x80DF8
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_WRITE_PERMISSION_13 0xA3080DF8u

//! Register Reset Value
#define TREG7_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_ADDR_MATCH_14 Register TREG7_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TREG7_PM_ADDR_MATCH_14 0x80E00
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ADDR_MATCH_14 0xA3080E00u

//! Register Reset Value
#define TREG7_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG7_PM_REQ_INFO_PERMISSION_14 Register TREG7_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TREG7_PM_REQ_INFO_PERMISSION_14 0x80E08
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_REQ_INFO_PERMISSION_14 0xA3080E08u

//! Register Reset Value
#define TREG7_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_READ_PERMISSION_14 Register TREG7_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TREG7_PM_READ_PERMISSION_14 0x80E10
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_READ_PERMISSION_14 0xA3080E10u

//! Register Reset Value
#define TREG7_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_WRITE_PERMISSION_14 Register TREG7_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TREG7_PM_WRITE_PERMISSION_14 0x80E18
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_WRITE_PERMISSION_14 0xA3080E18u

//! Register Reset Value
#define TREG7_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_ADDR_MATCH_15 Register TREG7_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TREG7_PM_ADDR_MATCH_15 0x80E20
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_ADDR_MATCH_15 0xA3080E20u

//! Register Reset Value
#define TREG7_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG7_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TREG7_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TREG7_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG7_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG7_PM_REQ_INFO_PERMISSION_15 Register TREG7_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TREG7_PM_REQ_INFO_PERMISSION_15 0x80E28
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_REQ_INFO_PERMISSION_15 0xA3080E28u

//! Register Reset Value
#define TREG7_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG7_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_READ_PERMISSION_15 Register TREG7_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TREG7_PM_READ_PERMISSION_15 0x80E30
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_READ_PERMISSION_15 0xA3080E30u

//! Register Reset Value
#define TREG7_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG7_PM_WRITE_PERMISSION_15 Register TREG7_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TREG7_PM_WRITE_PERMISSION_15 0x80E38
//! Register Offset (absolute) for 1st Instance SSX7_SSX
#define SSX7_SSX_TREG7_PM_WRITE_PERMISSION_15 0xA3080E38u

//! Register Reset Value
#define TREG7_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG7_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! @}

#endif
