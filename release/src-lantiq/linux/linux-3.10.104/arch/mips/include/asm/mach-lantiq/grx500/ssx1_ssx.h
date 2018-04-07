//-----------------------------------------------------------------------------
// LSD Generator
//-----------------------------------------------------------------------------
// Perl Package        : LSD::generator::targetC (v1.1)
// LSD Source          : D:/Users/shij/Perforce/l1033.grx500.win.v_shij.priv.dfv.grx500.dfv/ipg_lsd/lsd_sys/source/xml/reg_files/ssx1_ssx.xml
// Register File Name  : SSX1_SSX
// Register File Title : SSX1_ssx
// Register Width      : 64
// Note                : Doxygen compliant comments
//-----------------------------------------------------------------------------

#ifndef _SSX1_SSX_H
#define _SSX1_SSX_H

//! \defgroup SSX1_SSX Register File SSX1_SSX - SSX1_ssx
//! @{

//! Base Address of SSX1_SSX
#define SSX1_SSX_MODULE_BASE 0x1DF00000u

//! \defgroup TREG1_RT_COMPONENT Register TREG1_RT_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TREG1_RT_COMPONENT 0x0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_RT_COMPONENT 0x1DF00000u

//! Register Reset Value
#define TREG1_RT_COMPONENT_RST 0x0000000062003532u

//! Field REV - rev
#define TREG1_RT_COMPONENT_REV_POS 0
//! Field REV - rev
#define TREG1_RT_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TREG1_RT_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TREG1_RT_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TREG1_RT_NETWORK Register TREG1_RT_NETWORK - network
//! @{

//! Register Offset (relative)
#define TREG1_RT_NETWORK 0x10
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_RT_NETWORK 0x1DF00010u

//! Register Reset Value
#define TREG1_RT_NETWORK_RST 0x0000000000000000u

//! Field REV - rev
#define TREG1_RT_NETWORK_REV_POS 32
//! Field REV - rev
#define TREG1_RT_NETWORK_REV_MASK 0xFFFF00000000u

//! Field ID - id
#define TREG1_RT_NETWORK_ID_POS 48
//! Field ID - id
#define TREG1_RT_NETWORK_ID_MASK 0xFFFF000000000000u

//! @}

//! \defgroup TREG1_RT_INITID_READBACK Register TREG1_RT_INITID_READBACK - initid_readback
//! @{

//! Register Offset (relative)
#define TREG1_RT_INITID_READBACK 0x70
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_RT_INITID_READBACK 0x1DF00070u

//! Register Reset Value
#define TREG1_RT_INITID_READBACK_RST 0x0000000000000000u

//! Field INITID - initid
#define TREG1_RT_INITID_READBACK_INITID_POS 0
//! Field INITID - initid
#define TREG1_RT_INITID_READBACK_INITID_MASK 0xFFu

//! @}

//! \defgroup TREG1_RT_NETWORK_CONTROL Register TREG1_RT_NETWORK_CONTROL - network_control
//! @{

//! Register Offset (relative)
#define TREG1_RT_NETWORK_CONTROL 0x78
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_RT_NETWORK_CONTROL 0x1DF00078u

//! Register Reset Value
#define TREG1_RT_NETWORK_CONTROL_RST 0x0000000000000000u

//! Field TIMEOUT_BASE - timeout_base
#define TREG1_RT_NETWORK_CONTROL_TIMEOUT_BASE_POS 8
//! Field TIMEOUT_BASE - timeout_base
#define TREG1_RT_NETWORK_CONTROL_TIMEOUT_BASE_MASK 0x700u

//! Field CLOCK_GATE_DISABLE - clock_gate_disable
#define TREG1_RT_NETWORK_CONTROL_CLOCK_GATE_DISABLE_POS 56
//! Field CLOCK_GATE_DISABLE - clock_gate_disable
#define TREG1_RT_NETWORK_CONTROL_CLOCK_GATE_DISABLE_MASK 0x100000000000000u

//! @}

//! \defgroup TREG1_SI_CONTROL Register TREG1_SI_CONTROL - control
//! @{

//! Register Offset (relative)
#define TREG1_SI_CONTROL 0x420
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_SI_CONTROL 0x1DF00420u

//! Register Reset Value
#define TREG1_SI_CONTROL_RST 0x0000000000000000u

//! Field CLOCK_GATE_DISABLE - clock_gate_disable
#define TREG1_SI_CONTROL_CLOCK_GATE_DISABLE_POS 56
//! Field CLOCK_GATE_DISABLE - clock_gate_disable
#define TREG1_SI_CONTROL_CLOCK_GATE_DISABLE_MASK 0x100000000000000u

//! @}

//! \defgroup TREG1_SI_FLAG_STATUS_0 Register TREG1_SI_FLAG_STATUS_0 - flag_status_0
//! @{

//! Register Offset (relative)
#define TREG1_SI_FLAG_STATUS_0 0x510
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_SI_FLAG_STATUS_0 0x1DF00510u

//! Register Reset Value
#define TREG1_SI_FLAG_STATUS_0_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG1_SI_FLAG_STATUS_0_STATUS_POS 0
//! Field STATUS - status
#define TREG1_SI_FLAG_STATUS_0_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_SI_FLAG_STATUS_1 Register TREG1_SI_FLAG_STATUS_1 - flag_status_1
//! @{

//! Register Offset (relative)
#define TREG1_SI_FLAG_STATUS_1 0x530
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_SI_FLAG_STATUS_1 0x1DF00530u

//! Register Reset Value
#define TREG1_SI_FLAG_STATUS_1_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG1_SI_FLAG_STATUS_1_STATUS_POS 0
//! Field STATUS - status
#define TREG1_SI_FLAG_STATUS_1_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_SI_FLAG_STATUS_2 Register TREG1_SI_FLAG_STATUS_2 - flag_status_2
//! @{

//! Register Offset (relative)
#define TREG1_SI_FLAG_STATUS_2 0x550
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_SI_FLAG_STATUS_2 0x1DF00550u

//! Register Reset Value
#define TREG1_SI_FLAG_STATUS_2_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG1_SI_FLAG_STATUS_2_STATUS_POS 0
//! Field STATUS - status
#define TREG1_SI_FLAG_STATUS_2_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_SI_FLAG_STATUS_3 Register TREG1_SI_FLAG_STATUS_3 - flag_status_3
//! @{

//! Register Offset (relative)
#define TREG1_SI_FLAG_STATUS_3 0x570
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_SI_FLAG_STATUS_3 0x1DF00570u

//! Register Reset Value
#define TREG1_SI_FLAG_STATUS_3_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG1_SI_FLAG_STATUS_3_STATUS_POS 0
//! Field STATUS - status
#define TREG1_SI_FLAG_STATUS_3_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_SI_FLAG_STATUS_4 Register TREG1_SI_FLAG_STATUS_4 - flag_status_4
//! @{

//! Register Offset (relative)
#define TREG1_SI_FLAG_STATUS_4 0x590
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_SI_FLAG_STATUS_4 0x1DF00590u

//! Register Reset Value
#define TREG1_SI_FLAG_STATUS_4_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG1_SI_FLAG_STATUS_4_STATUS_POS 0
//! Field STATUS - status
#define TREG1_SI_FLAG_STATUS_4_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_SI_FLAG_STATUS_5 Register TREG1_SI_FLAG_STATUS_5 - flag_status_5
//! @{

//! Register Offset (relative)
#define TREG1_SI_FLAG_STATUS_5 0x5B0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_SI_FLAG_STATUS_5 0x1DF005B0u

//! Register Reset Value
#define TREG1_SI_FLAG_STATUS_5_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG1_SI_FLAG_STATUS_5_STATUS_POS 0
//! Field STATUS - status
#define TREG1_SI_FLAG_STATUS_5_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_SI_FLAG_STATUS_6 Register TREG1_SI_FLAG_STATUS_6 - flag_status_6
//! @{

//! Register Offset (relative)
#define TREG1_SI_FLAG_STATUS_6 0x5D0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_SI_FLAG_STATUS_6 0x1DF005D0u

//! Register Reset Value
#define TREG1_SI_FLAG_STATUS_6_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG1_SI_FLAG_STATUS_6_STATUS_POS 0
//! Field STATUS - status
#define TREG1_SI_FLAG_STATUS_6_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_SI_FLAG_STATUS_7 Register TREG1_SI_FLAG_STATUS_7 - flag_status_7
//! @{

//! Register Offset (relative)
#define TREG1_SI_FLAG_STATUS_7 0x5F0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_SI_FLAG_STATUS_7 0x1DF005F0u

//! Register Reset Value
#define TREG1_SI_FLAG_STATUS_7_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG1_SI_FLAG_STATUS_7_STATUS_POS 0
//! Field STATUS - status
#define TREG1_SI_FLAG_STATUS_7_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_TA_COMPONENT Register TDM2R_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TDM2R_TA_COMPONENT 0x1000
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_TA_COMPONENT 0x1DF01000u

//! Register Reset Value
#define TDM2R_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TDM2R_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TDM2R_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TDM2R_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TDM2R_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TDM2R_TA_CORE Register TDM2R_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TDM2R_TA_CORE 0x1018
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_TA_CORE 0x1DF01018u

//! Register Reset Value
#define TDM2R_TA_CORE_RST 0x000088C300140001u

//! Field REV_CODE - rev_code
#define TDM2R_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TDM2R_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TDM2R_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TDM2R_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TDM2R_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TDM2R_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TDM2R_TA_AGENT_CONTROL Register TDM2R_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TDM2R_TA_AGENT_CONTROL 0x1020
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_TA_AGENT_CONTROL 0x1DF01020u

//! Register Reset Value
#define TDM2R_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TDM2R_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TDM2R_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TDM2R_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TDM2R_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TDM2R_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TDM2R_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDM2R_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDM2R_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TDM2R_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TDM2R_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TDM2R_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TDM2R_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TDM2R_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TDM2R_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TDM2R_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TDM2R_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TDM2R_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TDM2R_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TDM2R_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TDM2R_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TDM2R_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TDM2R_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TDM2R_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TDM2R_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TDM2R_TA_AGENT_STATUS Register TDM2R_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TDM2R_TA_AGENT_STATUS 0x1028
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_TA_AGENT_STATUS 0x1DF01028u

//! Register Reset Value
#define TDM2R_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TDM2R_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TDM2R_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDM2R_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDM2R_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TDM2R_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TDM2R_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TDM2R_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TDM2R_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TDM2R_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TDM2R_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TDM2R_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TDM2R_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TDM2R_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TDM2R_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TDM2R_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TDM2R_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TDM2R_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TDM2R_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TDM2R_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TDM2R_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TDM2R_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TDM2R_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TDM2R_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TDM2R_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TDM2R_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TDM2R_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TDM2R_TA_ERROR_LOG Register TDM2R_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TDM2R_TA_ERROR_LOG 0x1058
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_TA_ERROR_LOG 0x1DF01058u

//! Register Reset Value
#define TDM2R_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TDM2R_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TDM2R_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TDM2R_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TDM2R_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TDM2R_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TDM2R_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TDM2R_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TDM2R_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TDM2R_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TDM2R_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TDM2R_TA_ERROR_LOG_ADDR Register TDM2R_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TDM2R_TA_ERROR_LOG_ADDR 0x1060
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_TA_ERROR_LOG_ADDR 0x1DF01060u

//! Register Reset Value
#define TDM2R_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TDM2R_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TDM2R_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_TA_BANDWIDTH_0 Register TDM2R_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TDM2R_TA_BANDWIDTH_0 0x1100
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_TA_BANDWIDTH_0 0x1DF01100u

//! Register Reset Value
#define TDM2R_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TDM2R_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TDM2R_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TDM2R_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TDM2R_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TDM2R_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TDM2R_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TDM2R_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TDM2R_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM2R_TA_BANDWIDTH_1 Register TDM2R_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TDM2R_TA_BANDWIDTH_1 0x1108
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_TA_BANDWIDTH_1 0x1DF01108u

//! Register Reset Value
#define TDM2R_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TDM2R_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TDM2R_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TDM2R_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TDM2R_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TDM2R_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TDM2R_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TDM2R_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TDM2R_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM2R_TA_BANDWIDTH_2 Register TDM2R_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TDM2R_TA_BANDWIDTH_2 0x1110
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_TA_BANDWIDTH_2 0x1DF01110u

//! Register Reset Value
#define TDM2R_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TDM2R_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TDM2R_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TDM2R_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TDM2R_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TDM2R_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TDM2R_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TDM2R_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TDM2R_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM2R_TA_BANDWIDTH_3 Register TDM2R_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TDM2R_TA_BANDWIDTH_3 0x1118
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_TA_BANDWIDTH_3 0x1DF01118u

//! Register Reset Value
#define TDM2R_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TDM2R_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TDM2R_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TDM2R_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TDM2R_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TDM2R_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TDM2R_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TDM2R_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TDM2R_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM2R_TA_ALLOC_LIMIT_0 Register TDM2R_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TDM2R_TA_ALLOC_LIMIT_0 0x1200
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_TA_ALLOC_LIMIT_0 0x1DF01200u

//! Register Reset Value
#define TDM2R_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TDM2R_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TDM2R_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TDM2R_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TDM2R_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TDM2R_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TDM2R_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TDM2R_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TDM2R_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TDM2R_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TDM2R_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TDM2R_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TDM2R_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TDM2R_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TDM2R_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TDM2R_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TDM2R_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM2R_TA_ALLOC_LIMIT_1 Register TDM2R_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TDM2R_TA_ALLOC_LIMIT_1 0x1208
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_TA_ALLOC_LIMIT_1 0x1DF01208u

//! Register Reset Value
#define TDM2R_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TDM2R_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TDM2R_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TDM2R_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TDM2R_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TDM2R_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TDM2R_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TDM2R_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TDM2R_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TDM2R_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TDM2R_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TDM2R_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TDM2R_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TDM2R_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TDM2R_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TDM2R_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TDM2R_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM2R_TA_ALLOC_LIMIT_2 Register TDM2R_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TDM2R_TA_ALLOC_LIMIT_2 0x1210
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_TA_ALLOC_LIMIT_2 0x1DF01210u

//! Register Reset Value
#define TDM2R_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TDM2R_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TDM2R_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TDM2R_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TDM2R_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TDM2R_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TDM2R_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TDM2R_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TDM2R_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TDM2R_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TDM2R_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TDM2R_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TDM2R_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TDM2R_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TDM2R_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TDM2R_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TDM2R_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM2R_TA_ALLOC_LIMIT_3 Register TDM2R_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TDM2R_TA_ALLOC_LIMIT_3 0x1218
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_TA_ALLOC_LIMIT_3 0x1DF01218u

//! Register Reset Value
#define TDM2R_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TDM2R_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TDM2R_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TDM2R_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TDM2R_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TDM2R_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TDM2R_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TDM2R_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TDM2R_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TDM2R_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TDM2R_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TDM2R_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TDM2R_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TDM2R_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TDM2R_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TDM2R_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TDM2R_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM2T_TA_COMPONENT Register TDM2T_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TDM2T_TA_COMPONENT 0x1400
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_TA_COMPONENT 0x1DF01400u

//! Register Reset Value
#define TDM2T_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TDM2T_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TDM2T_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TDM2T_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TDM2T_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TDM2T_TA_CORE Register TDM2T_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TDM2T_TA_CORE 0x1418
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_TA_CORE 0x1DF01418u

//! Register Reset Value
#define TDM2T_TA_CORE_RST 0x000088C300130001u

//! Field REV_CODE - rev_code
#define TDM2T_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TDM2T_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TDM2T_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TDM2T_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TDM2T_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TDM2T_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TDM2T_TA_AGENT_CONTROL Register TDM2T_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TDM2T_TA_AGENT_CONTROL 0x1420
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_TA_AGENT_CONTROL 0x1DF01420u

//! Register Reset Value
#define TDM2T_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TDM2T_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TDM2T_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TDM2T_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TDM2T_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TDM2T_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TDM2T_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDM2T_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDM2T_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TDM2T_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TDM2T_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TDM2T_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TDM2T_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TDM2T_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TDM2T_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TDM2T_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TDM2T_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TDM2T_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TDM2T_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TDM2T_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TDM2T_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TDM2T_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TDM2T_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TDM2T_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TDM2T_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TDM2T_TA_AGENT_STATUS Register TDM2T_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TDM2T_TA_AGENT_STATUS 0x1428
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_TA_AGENT_STATUS 0x1DF01428u

//! Register Reset Value
#define TDM2T_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TDM2T_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TDM2T_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDM2T_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDM2T_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TDM2T_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TDM2T_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TDM2T_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TDM2T_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TDM2T_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TDM2T_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TDM2T_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TDM2T_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TDM2T_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TDM2T_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TDM2T_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TDM2T_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TDM2T_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TDM2T_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TDM2T_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TDM2T_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TDM2T_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TDM2T_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TDM2T_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TDM2T_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TDM2T_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TDM2T_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TDM2T_TA_ERROR_LOG Register TDM2T_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TDM2T_TA_ERROR_LOG 0x1458
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_TA_ERROR_LOG 0x1DF01458u

//! Register Reset Value
#define TDM2T_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TDM2T_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TDM2T_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TDM2T_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TDM2T_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TDM2T_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TDM2T_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TDM2T_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TDM2T_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TDM2T_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TDM2T_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TDM2T_TA_ERROR_LOG_ADDR Register TDM2T_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TDM2T_TA_ERROR_LOG_ADDR 0x1460
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_TA_ERROR_LOG_ADDR 0x1DF01460u

//! Register Reset Value
#define TDM2T_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TDM2T_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TDM2T_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_TA_BANDWIDTH_0 Register TDM2T_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TDM2T_TA_BANDWIDTH_0 0x1500
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_TA_BANDWIDTH_0 0x1DF01500u

//! Register Reset Value
#define TDM2T_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TDM2T_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TDM2T_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TDM2T_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TDM2T_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TDM2T_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TDM2T_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TDM2T_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TDM2T_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM2T_TA_BANDWIDTH_1 Register TDM2T_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TDM2T_TA_BANDWIDTH_1 0x1508
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_TA_BANDWIDTH_1 0x1DF01508u

//! Register Reset Value
#define TDM2T_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TDM2T_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TDM2T_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TDM2T_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TDM2T_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TDM2T_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TDM2T_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TDM2T_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TDM2T_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM2T_TA_BANDWIDTH_2 Register TDM2T_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TDM2T_TA_BANDWIDTH_2 0x1510
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_TA_BANDWIDTH_2 0x1DF01510u

//! Register Reset Value
#define TDM2T_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TDM2T_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TDM2T_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TDM2T_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TDM2T_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TDM2T_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TDM2T_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TDM2T_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TDM2T_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM2T_TA_BANDWIDTH_3 Register TDM2T_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TDM2T_TA_BANDWIDTH_3 0x1518
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_TA_BANDWIDTH_3 0x1DF01518u

//! Register Reset Value
#define TDM2T_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TDM2T_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TDM2T_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TDM2T_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TDM2T_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TDM2T_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TDM2T_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TDM2T_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TDM2T_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM2T_TA_ALLOC_LIMIT_0 Register TDM2T_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TDM2T_TA_ALLOC_LIMIT_0 0x1600
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_TA_ALLOC_LIMIT_0 0x1DF01600u

//! Register Reset Value
#define TDM2T_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TDM2T_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TDM2T_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TDM2T_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TDM2T_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TDM2T_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TDM2T_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TDM2T_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TDM2T_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TDM2T_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TDM2T_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TDM2T_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TDM2T_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TDM2T_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TDM2T_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TDM2T_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TDM2T_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM2T_TA_ALLOC_LIMIT_1 Register TDM2T_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TDM2T_TA_ALLOC_LIMIT_1 0x1608
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_TA_ALLOC_LIMIT_1 0x1DF01608u

//! Register Reset Value
#define TDM2T_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TDM2T_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TDM2T_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TDM2T_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TDM2T_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TDM2T_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TDM2T_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TDM2T_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TDM2T_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TDM2T_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TDM2T_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TDM2T_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TDM2T_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TDM2T_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TDM2T_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TDM2T_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TDM2T_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM2T_TA_ALLOC_LIMIT_2 Register TDM2T_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TDM2T_TA_ALLOC_LIMIT_2 0x1610
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_TA_ALLOC_LIMIT_2 0x1DF01610u

//! Register Reset Value
#define TDM2T_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TDM2T_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TDM2T_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TDM2T_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TDM2T_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TDM2T_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TDM2T_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TDM2T_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TDM2T_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TDM2T_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TDM2T_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TDM2T_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TDM2T_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TDM2T_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TDM2T_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TDM2T_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TDM2T_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM2T_TA_ALLOC_LIMIT_3 Register TDM2T_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TDM2T_TA_ALLOC_LIMIT_3 0x1618
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_TA_ALLOC_LIMIT_3 0x1DF01618u

//! Register Reset Value
#define TDM2T_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TDM2T_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TDM2T_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TDM2T_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TDM2T_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TDM2T_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TDM2T_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TDM2T_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TDM2T_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TDM2T_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TDM2T_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TDM2T_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TDM2T_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TDM2T_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TDM2T_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TDM2T_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TDM2T_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TGWPL_TA_COMPONENT Register TGWPL_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TGWPL_TA_COMPONENT 0x1800
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_TA_COMPONENT 0x1DF01800u

//! Register Reset Value
#define TGWPL_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TGWPL_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TGWPL_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TGWPL_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TGWPL_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TGWPL_TA_CORE Register TGWPL_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TGWPL_TA_CORE 0x1818
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_TA_CORE 0x1DF01818u

//! Register Reset Value
#define TGWPL_TA_CORE_RST 0x000050C500120001u

//! Field REV_CODE - rev_code
#define TGWPL_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TGWPL_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TGWPL_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TGWPL_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TGWPL_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TGWPL_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TGWPL_TA_AGENT_CONTROL Register TGWPL_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TGWPL_TA_AGENT_CONTROL 0x1820
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_TA_AGENT_CONTROL 0x1DF01820u

//! Register Reset Value
#define TGWPL_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TGWPL_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TGWPL_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TGWPL_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TGWPL_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TGWPL_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TGWPL_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TGWPL_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TGWPL_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TGWPL_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TGWPL_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TGWPL_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TGWPL_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TGWPL_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TGWPL_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TGWPL_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TGWPL_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TGWPL_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TGWPL_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TGWPL_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TGWPL_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TGWPL_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TGWPL_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TGWPL_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TGWPL_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TGWPL_TA_AGENT_STATUS Register TGWPL_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TGWPL_TA_AGENT_STATUS 0x1828
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_TA_AGENT_STATUS 0x1DF01828u

//! Register Reset Value
#define TGWPL_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TGWPL_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TGWPL_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TGWPL_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TGWPL_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TGWPL_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TGWPL_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TGWPL_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TGWPL_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TGWPL_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TGWPL_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TGWPL_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TGWPL_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TGWPL_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TGWPL_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TGWPL_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TGWPL_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TGWPL_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TGWPL_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TGWPL_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TGWPL_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TGWPL_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TGWPL_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TGWPL_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TGWPL_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TGWPL_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TGWPL_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TGWPL_TA_ERROR_LOG Register TGWPL_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TGWPL_TA_ERROR_LOG 0x1858
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_TA_ERROR_LOG 0x1DF01858u

//! Register Reset Value
#define TGWPL_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TGWPL_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TGWPL_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TGWPL_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TGWPL_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TGWPL_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TGWPL_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TGWPL_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TGWPL_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TGWPL_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TGWPL_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TGWPL_TA_ERROR_LOG_ADDR Register TGWPL_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TGWPL_TA_ERROR_LOG_ADDR 0x1860
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_TA_ERROR_LOG_ADDR 0x1DF01860u

//! Register Reset Value
#define TGWPL_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TGWPL_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TGWPL_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_TA_BANDWIDTH_0 Register TGWPL_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TGWPL_TA_BANDWIDTH_0 0x1900
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_TA_BANDWIDTH_0 0x1DF01900u

//! Register Reset Value
#define TGWPL_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TGWPL_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TGWPL_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TGWPL_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TGWPL_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TGWPL_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TGWPL_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TGWPL_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TGWPL_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TGWPL_TA_BANDWIDTH_1 Register TGWPL_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TGWPL_TA_BANDWIDTH_1 0x1908
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_TA_BANDWIDTH_1 0x1DF01908u

//! Register Reset Value
#define TGWPL_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TGWPL_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TGWPL_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TGWPL_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TGWPL_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TGWPL_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TGWPL_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TGWPL_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TGWPL_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TGWPL_TA_BANDWIDTH_2 Register TGWPL_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TGWPL_TA_BANDWIDTH_2 0x1910
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_TA_BANDWIDTH_2 0x1DF01910u

//! Register Reset Value
#define TGWPL_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TGWPL_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TGWPL_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TGWPL_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TGWPL_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TGWPL_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TGWPL_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TGWPL_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TGWPL_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TGWPL_TA_BANDWIDTH_3 Register TGWPL_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TGWPL_TA_BANDWIDTH_3 0x1918
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_TA_BANDWIDTH_3 0x1DF01918u

//! Register Reset Value
#define TGWPL_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TGWPL_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TGWPL_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TGWPL_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TGWPL_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TGWPL_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TGWPL_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TGWPL_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TGWPL_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TGWPL_TA_ALLOC_LIMIT_0 Register TGWPL_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TGWPL_TA_ALLOC_LIMIT_0 0x1A00
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_TA_ALLOC_LIMIT_0 0x1DF01A00u

//! Register Reset Value
#define TGWPL_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TGWPL_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TGWPL_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TGWPL_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TGWPL_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TGWPL_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TGWPL_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TGWPL_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TGWPL_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TGWPL_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TGWPL_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TGWPL_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TGWPL_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TGWPL_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TGWPL_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TGWPL_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TGWPL_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TGWPL_TA_ALLOC_LIMIT_1 Register TGWPL_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TGWPL_TA_ALLOC_LIMIT_1 0x1A08
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_TA_ALLOC_LIMIT_1 0x1DF01A08u

//! Register Reset Value
#define TGWPL_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TGWPL_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TGWPL_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TGWPL_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TGWPL_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TGWPL_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TGWPL_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TGWPL_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TGWPL_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TGWPL_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TGWPL_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TGWPL_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TGWPL_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TGWPL_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TGWPL_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TGWPL_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TGWPL_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TGWPL_TA_ALLOC_LIMIT_2 Register TGWPL_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TGWPL_TA_ALLOC_LIMIT_2 0x1A10
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_TA_ALLOC_LIMIT_2 0x1DF01A10u

//! Register Reset Value
#define TGWPL_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TGWPL_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TGWPL_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TGWPL_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TGWPL_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TGWPL_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TGWPL_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TGWPL_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TGWPL_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TGWPL_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TGWPL_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TGWPL_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TGWPL_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TGWPL_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TGWPL_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TGWPL_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TGWPL_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TGWPL_TA_ALLOC_LIMIT_3 Register TGWPL_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TGWPL_TA_ALLOC_LIMIT_3 0x1A18
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_TA_ALLOC_LIMIT_3 0x1DF01A18u

//! Register Reset Value
#define TGWPL_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TGWPL_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TGWPL_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TGWPL_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TGWPL_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TGWPL_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TGWPL_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TGWPL_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TGWPL_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TGWPL_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TGWPL_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TGWPL_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TGWPL_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TGWPL_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TGWPL_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TGWPL_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TGWPL_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN10_TA_COMPONENT Register TLN10_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TLN10_TA_COMPONENT 0x1C00
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_TA_COMPONENT 0x1DF01C00u

//! Register Reset Value
#define TLN10_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TLN10_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TLN10_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TLN10_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TLN10_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TLN10_TA_CORE Register TLN10_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TLN10_TA_CORE 0x1C18
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_TA_CORE 0x1DF01C18u

//! Register Reset Value
#define TLN10_TA_CORE_RST 0x000050C50F100001u

//! Field REV_CODE - rev_code
#define TLN10_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TLN10_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TLN10_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TLN10_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TLN10_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TLN10_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TLN10_TA_AGENT_CONTROL Register TLN10_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TLN10_TA_AGENT_CONTROL 0x1C20
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_TA_AGENT_CONTROL 0x1DF01C20u

//! Register Reset Value
#define TLN10_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TLN10_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TLN10_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TLN10_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TLN10_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TLN10_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TLN10_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TLN10_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TLN10_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TLN10_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TLN10_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TLN10_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TLN10_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TLN10_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TLN10_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TLN10_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TLN10_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TLN10_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TLN10_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TLN10_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TLN10_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TLN10_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TLN10_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TLN10_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TLN10_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TLN10_TA_AGENT_STATUS Register TLN10_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TLN10_TA_AGENT_STATUS 0x1C28
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_TA_AGENT_STATUS 0x1DF01C28u

//! Register Reset Value
#define TLN10_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TLN10_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TLN10_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TLN10_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TLN10_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TLN10_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TLN10_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TLN10_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TLN10_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TLN10_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TLN10_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TLN10_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TLN10_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TLN10_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TLN10_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TLN10_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TLN10_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TLN10_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TLN10_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TLN10_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TLN10_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TLN10_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TLN10_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TLN10_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TLN10_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TLN10_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TLN10_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TLN10_TA_ERROR_LOG Register TLN10_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TLN10_TA_ERROR_LOG 0x1C58
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_TA_ERROR_LOG 0x1DF01C58u

//! Register Reset Value
#define TLN10_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TLN10_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TLN10_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TLN10_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TLN10_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TLN10_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TLN10_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TLN10_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TLN10_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TLN10_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TLN10_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TLN10_TA_ERROR_LOG_ADDR Register TLN10_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TLN10_TA_ERROR_LOG_ADDR 0x1C60
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_TA_ERROR_LOG_ADDR 0x1DF01C60u

//! Register Reset Value
#define TLN10_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TLN10_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TLN10_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_TA_BANDWIDTH_0 Register TLN10_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TLN10_TA_BANDWIDTH_0 0x1D00
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_TA_BANDWIDTH_0 0x1DF01D00u

//! Register Reset Value
#define TLN10_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TLN10_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TLN10_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TLN10_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TLN10_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TLN10_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TLN10_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TLN10_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TLN10_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN10_TA_BANDWIDTH_1 Register TLN10_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TLN10_TA_BANDWIDTH_1 0x1D08
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_TA_BANDWIDTH_1 0x1DF01D08u

//! Register Reset Value
#define TLN10_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TLN10_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TLN10_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TLN10_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TLN10_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TLN10_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TLN10_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TLN10_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TLN10_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN10_TA_BANDWIDTH_2 Register TLN10_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TLN10_TA_BANDWIDTH_2 0x1D10
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_TA_BANDWIDTH_2 0x1DF01D10u

//! Register Reset Value
#define TLN10_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TLN10_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TLN10_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TLN10_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TLN10_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TLN10_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TLN10_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TLN10_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TLN10_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN10_TA_BANDWIDTH_3 Register TLN10_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TLN10_TA_BANDWIDTH_3 0x1D18
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_TA_BANDWIDTH_3 0x1DF01D18u

//! Register Reset Value
#define TLN10_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TLN10_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TLN10_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TLN10_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TLN10_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TLN10_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TLN10_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TLN10_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TLN10_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN10_TA_ALLOC_LIMIT_0 Register TLN10_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TLN10_TA_ALLOC_LIMIT_0 0x1E00
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_TA_ALLOC_LIMIT_0 0x1DF01E00u

//! Register Reset Value
#define TLN10_TA_ALLOC_LIMIT_0_RST 0x0000000001010101u

//! Field MIN_VALUE_0 - min_value_0
#define TLN10_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TLN10_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TLN10_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TLN10_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TLN10_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TLN10_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TLN10_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TLN10_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TLN10_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TLN10_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TLN10_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TLN10_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TLN10_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TLN10_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TLN10_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TLN10_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN10_TA_ALLOC_LIMIT_1 Register TLN10_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TLN10_TA_ALLOC_LIMIT_1 0x1E08
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_TA_ALLOC_LIMIT_1 0x1DF01E08u

//! Register Reset Value
#define TLN10_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TLN10_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TLN10_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TLN10_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TLN10_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TLN10_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TLN10_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TLN10_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TLN10_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TLN10_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TLN10_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TLN10_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TLN10_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TLN10_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TLN10_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TLN10_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TLN10_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN10_TA_ALLOC_LIMIT_2 Register TLN10_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TLN10_TA_ALLOC_LIMIT_2 0x1E10
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_TA_ALLOC_LIMIT_2 0x1DF01E10u

//! Register Reset Value
#define TLN10_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TLN10_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TLN10_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TLN10_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TLN10_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TLN10_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TLN10_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TLN10_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TLN10_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TLN10_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TLN10_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TLN10_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TLN10_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TLN10_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TLN10_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TLN10_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TLN10_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN10_TA_ALLOC_LIMIT_3 Register TLN10_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TLN10_TA_ALLOC_LIMIT_3 0x1E18
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_TA_ALLOC_LIMIT_3 0x1DF01E18u

//! Register Reset Value
#define TLN10_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TLN10_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TLN10_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TLN10_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TLN10_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TLN10_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TLN10_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TLN10_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TLN10_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TLN10_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TLN10_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TLN10_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TLN10_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TLN10_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TLN10_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TLN10_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TLN10_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM2R_IA_COMPONENT Register IDM2R_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define IDM2R_IA_COMPONENT 0x10000
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2R_IA_COMPONENT 0x1DF10000u

//! Register Reset Value
#define IDM2R_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define IDM2R_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define IDM2R_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define IDM2R_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define IDM2R_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup IDM2R_IA_CORE Register IDM2R_IA_CORE - core
//! @{

//! Register Offset (relative)
#define IDM2R_IA_CORE 0x10018
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2R_IA_CORE 0x1DF10018u

//! Register Reset Value
#define IDM2R_IA_CORE_RST 0x000050C500140001u

//! Field REV_CODE - rev_code
#define IDM2R_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define IDM2R_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define IDM2R_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define IDM2R_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define IDM2R_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define IDM2R_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup IDM2R_IA_AGENT_CONTROL Register IDM2R_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define IDM2R_IA_AGENT_CONTROL 0x10020
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2R_IA_AGENT_CONTROL 0x1DF10020u

//! Register Reset Value
#define IDM2R_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IDM2R_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IDM2R_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define IDM2R_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define IDM2R_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define IDM2R_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IDM2R_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define IDM2R_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IDM2R_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define IDM2R_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define IDM2R_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IDM2R_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IDM2R_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IDM2R_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IDM2R_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IDM2R_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IDM2R_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IDM2R_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IDM2R_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IDM2R_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IDM2R_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IDM2R_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IDM2R_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup IDM2R_IA_AGENT_STATUS Register IDM2R_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define IDM2R_IA_AGENT_STATUS 0x10028
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2R_IA_AGENT_STATUS 0x1DF10028u

//! Register Reset Value
#define IDM2R_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IDM2R_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IDM2R_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define IDM2R_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define IDM2R_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define IDM2R_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define IDM2R_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define IDM2R_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define IDM2R_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define IDM2R_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define IDM2R_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define IDM2R_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IDM2R_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define IDM2R_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define IDM2R_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define IDM2R_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IDM2R_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define IDM2R_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define IDM2R_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IDM2R_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IDM2R_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IDM2R_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IDM2R_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup IDM2R_IA_ERROR_LOG Register IDM2R_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define IDM2R_IA_ERROR_LOG 0x10058
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2R_IA_ERROR_LOG 0x1DF10058u

//! Register Reset Value
#define IDM2R_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define IDM2R_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define IDM2R_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define IDM2R_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define IDM2R_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define IDM2R_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define IDM2R_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define IDM2R_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define IDM2R_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define IDM2R_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define IDM2R_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define IDM2R_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define IDM2R_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define IDM2R_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define IDM2R_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define IDM2R_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define IDM2R_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define IDM2R_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define IDM2R_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup IDM2R_IA_ERROR_LOG_ADDR Register IDM2R_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define IDM2R_IA_ERROR_LOG_ADDR 0x10060
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2R_IA_ERROR_LOG_ADDR 0x1DF10060u

//! Register Reset Value
#define IDM2R_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define IDM2R_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define IDM2R_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup IDM2R_IA_CORE_FLAG Register IDM2R_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define IDM2R_IA_CORE_FLAG 0x10068
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2R_IA_CORE_FLAG 0x1DF10068u

//! Register Reset Value
#define IDM2R_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define IDM2R_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define IDM2R_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define IDM2R_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define IDM2R_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define IDM2R_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define IDM2R_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define IDM2R_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define IDM2R_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup IDM2R_IA_ADDR_FILL_IN Register IDM2R_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define IDM2R_IA_ADDR_FILL_IN 0x10070
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2R_IA_ADDR_FILL_IN 0x1DF10070u

//! Register Reset Value
#define IDM2R_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define IDM2R_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define IDM2R_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup IDM2R_IA_BANDWIDTH_0 Register IDM2R_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define IDM2R_IA_BANDWIDTH_0 0x10100
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2R_IA_BANDWIDTH_0 0x1DF10100u

//! Register Reset Value
#define IDM2R_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define IDM2R_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define IDM2R_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define IDM2R_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define IDM2R_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define IDM2R_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define IDM2R_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define IDM2R_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define IDM2R_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define IDM2R_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define IDM2R_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define IDM2R_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define IDM2R_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define IDM2R_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define IDM2R_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define IDM2R_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define IDM2R_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM2R_IA_BANDWIDTH_1 Register IDM2R_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define IDM2R_IA_BANDWIDTH_1 0x10108
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2R_IA_BANDWIDTH_1 0x1DF10108u

//! Register Reset Value
#define IDM2R_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define IDM2R_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define IDM2R_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define IDM2R_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define IDM2R_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define IDM2R_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define IDM2R_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define IDM2R_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define IDM2R_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define IDM2R_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define IDM2R_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define IDM2R_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define IDM2R_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define IDM2R_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define IDM2R_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define IDM2R_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define IDM2R_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM2R_IA_BANDWIDTH_2 Register IDM2R_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define IDM2R_IA_BANDWIDTH_2 0x10110
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2R_IA_BANDWIDTH_2 0x1DF10110u

//! Register Reset Value
#define IDM2R_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define IDM2R_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define IDM2R_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define IDM2R_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define IDM2R_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define IDM2R_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define IDM2R_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define IDM2R_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define IDM2R_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define IDM2R_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define IDM2R_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define IDM2R_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define IDM2R_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define IDM2R_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define IDM2R_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define IDM2R_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define IDM2R_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM2R_IA_BANDWIDTH_3 Register IDM2R_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define IDM2R_IA_BANDWIDTH_3 0x10118
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2R_IA_BANDWIDTH_3 0x1DF10118u

//! Register Reset Value
#define IDM2R_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define IDM2R_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define IDM2R_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define IDM2R_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define IDM2R_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define IDM2R_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define IDM2R_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define IDM2R_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define IDM2R_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define IDM2R_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define IDM2R_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define IDM2R_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define IDM2R_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define IDM2R_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define IDM2R_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define IDM2R_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define IDM2R_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM2R_IA_BANDWIDTH_4 Register IDM2R_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define IDM2R_IA_BANDWIDTH_4 0x10120
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2R_IA_BANDWIDTH_4 0x1DF10120u

//! Register Reset Value
#define IDM2R_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define IDM2R_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define IDM2R_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define IDM2R_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define IDM2R_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define IDM2R_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define IDM2R_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define IDM2R_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define IDM2R_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define IDM2R_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define IDM2R_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define IDM2R_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define IDM2R_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define IDM2R_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define IDM2R_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define IDM2R_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define IDM2R_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM2R_IA_BANDWIDTH_5 Register IDM2R_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define IDM2R_IA_BANDWIDTH_5 0x10128
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2R_IA_BANDWIDTH_5 0x1DF10128u

//! Register Reset Value
#define IDM2R_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define IDM2R_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define IDM2R_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define IDM2R_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define IDM2R_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define IDM2R_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define IDM2R_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define IDM2R_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define IDM2R_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define IDM2R_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define IDM2R_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define IDM2R_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define IDM2R_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define IDM2R_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define IDM2R_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define IDM2R_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define IDM2R_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM2R_IA_BANDWIDTH_6 Register IDM2R_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define IDM2R_IA_BANDWIDTH_6 0x10130
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2R_IA_BANDWIDTH_6 0x1DF10130u

//! Register Reset Value
#define IDM2R_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define IDM2R_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define IDM2R_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define IDM2R_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define IDM2R_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define IDM2R_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define IDM2R_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define IDM2R_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define IDM2R_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define IDM2R_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define IDM2R_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define IDM2R_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define IDM2R_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define IDM2R_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define IDM2R_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define IDM2R_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define IDM2R_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM2R_IA_BANDWIDTH_7 Register IDM2R_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define IDM2R_IA_BANDWIDTH_7 0x10138
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2R_IA_BANDWIDTH_7 0x1DF10138u

//! Register Reset Value
#define IDM2R_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define IDM2R_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define IDM2R_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define IDM2R_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define IDM2R_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define IDM2R_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define IDM2R_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define IDM2R_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define IDM2R_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define IDM2R_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define IDM2R_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define IDM2R_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define IDM2R_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define IDM2R_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define IDM2R_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define IDM2R_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define IDM2R_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM2T_IA_COMPONENT Register IDM2T_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define IDM2T_IA_COMPONENT 0x10400
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2T_IA_COMPONENT 0x1DF10400u

//! Register Reset Value
#define IDM2T_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define IDM2T_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define IDM2T_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define IDM2T_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define IDM2T_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup IDM2T_IA_CORE Register IDM2T_IA_CORE - core
//! @{

//! Register Offset (relative)
#define IDM2T_IA_CORE 0x10418
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2T_IA_CORE 0x1DF10418u

//! Register Reset Value
#define IDM2T_IA_CORE_RST 0x000050C500130001u

//! Field REV_CODE - rev_code
#define IDM2T_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define IDM2T_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define IDM2T_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define IDM2T_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define IDM2T_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define IDM2T_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup IDM2T_IA_AGENT_CONTROL Register IDM2T_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define IDM2T_IA_AGENT_CONTROL 0x10420
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2T_IA_AGENT_CONTROL 0x1DF10420u

//! Register Reset Value
#define IDM2T_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IDM2T_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IDM2T_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define IDM2T_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define IDM2T_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define IDM2T_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IDM2T_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define IDM2T_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IDM2T_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define IDM2T_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define IDM2T_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IDM2T_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IDM2T_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IDM2T_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IDM2T_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IDM2T_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IDM2T_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IDM2T_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IDM2T_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IDM2T_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IDM2T_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IDM2T_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IDM2T_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup IDM2T_IA_AGENT_STATUS Register IDM2T_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define IDM2T_IA_AGENT_STATUS 0x10428
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2T_IA_AGENT_STATUS 0x1DF10428u

//! Register Reset Value
#define IDM2T_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IDM2T_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IDM2T_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define IDM2T_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define IDM2T_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define IDM2T_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define IDM2T_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define IDM2T_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define IDM2T_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define IDM2T_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define IDM2T_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define IDM2T_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IDM2T_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define IDM2T_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define IDM2T_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define IDM2T_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IDM2T_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define IDM2T_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define IDM2T_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IDM2T_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IDM2T_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IDM2T_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IDM2T_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup IDM2T_IA_ERROR_LOG Register IDM2T_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define IDM2T_IA_ERROR_LOG 0x10458
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2T_IA_ERROR_LOG 0x1DF10458u

//! Register Reset Value
#define IDM2T_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define IDM2T_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define IDM2T_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define IDM2T_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define IDM2T_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define IDM2T_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define IDM2T_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define IDM2T_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define IDM2T_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define IDM2T_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define IDM2T_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define IDM2T_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define IDM2T_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define IDM2T_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define IDM2T_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define IDM2T_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define IDM2T_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define IDM2T_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define IDM2T_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup IDM2T_IA_ERROR_LOG_ADDR Register IDM2T_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define IDM2T_IA_ERROR_LOG_ADDR 0x10460
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2T_IA_ERROR_LOG_ADDR 0x1DF10460u

//! Register Reset Value
#define IDM2T_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define IDM2T_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define IDM2T_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup IDM2T_IA_CORE_FLAG Register IDM2T_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define IDM2T_IA_CORE_FLAG 0x10468
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2T_IA_CORE_FLAG 0x1DF10468u

//! Register Reset Value
#define IDM2T_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define IDM2T_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define IDM2T_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define IDM2T_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define IDM2T_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define IDM2T_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define IDM2T_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define IDM2T_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define IDM2T_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup IDM2T_IA_ADDR_FILL_IN Register IDM2T_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define IDM2T_IA_ADDR_FILL_IN 0x10470
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2T_IA_ADDR_FILL_IN 0x1DF10470u

//! Register Reset Value
#define IDM2T_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define IDM2T_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define IDM2T_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup IDM2T_IA_BANDWIDTH_0 Register IDM2T_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define IDM2T_IA_BANDWIDTH_0 0x10500
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2T_IA_BANDWIDTH_0 0x1DF10500u

//! Register Reset Value
#define IDM2T_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define IDM2T_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define IDM2T_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define IDM2T_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define IDM2T_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define IDM2T_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define IDM2T_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define IDM2T_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define IDM2T_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define IDM2T_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define IDM2T_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define IDM2T_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define IDM2T_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define IDM2T_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define IDM2T_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define IDM2T_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define IDM2T_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM2T_IA_BANDWIDTH_1 Register IDM2T_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define IDM2T_IA_BANDWIDTH_1 0x10508
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2T_IA_BANDWIDTH_1 0x1DF10508u

//! Register Reset Value
#define IDM2T_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define IDM2T_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define IDM2T_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define IDM2T_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define IDM2T_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define IDM2T_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define IDM2T_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define IDM2T_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define IDM2T_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define IDM2T_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define IDM2T_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define IDM2T_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define IDM2T_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define IDM2T_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define IDM2T_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define IDM2T_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define IDM2T_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM2T_IA_BANDWIDTH_2 Register IDM2T_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define IDM2T_IA_BANDWIDTH_2 0x10510
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2T_IA_BANDWIDTH_2 0x1DF10510u

//! Register Reset Value
#define IDM2T_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define IDM2T_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define IDM2T_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define IDM2T_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define IDM2T_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define IDM2T_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define IDM2T_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define IDM2T_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define IDM2T_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define IDM2T_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define IDM2T_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define IDM2T_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define IDM2T_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define IDM2T_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define IDM2T_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define IDM2T_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define IDM2T_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM2T_IA_BANDWIDTH_3 Register IDM2T_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define IDM2T_IA_BANDWIDTH_3 0x10518
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2T_IA_BANDWIDTH_3 0x1DF10518u

//! Register Reset Value
#define IDM2T_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define IDM2T_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define IDM2T_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define IDM2T_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define IDM2T_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define IDM2T_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define IDM2T_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define IDM2T_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define IDM2T_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define IDM2T_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define IDM2T_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define IDM2T_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define IDM2T_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define IDM2T_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define IDM2T_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define IDM2T_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define IDM2T_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM2T_IA_BANDWIDTH_4 Register IDM2T_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define IDM2T_IA_BANDWIDTH_4 0x10520
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2T_IA_BANDWIDTH_4 0x1DF10520u

//! Register Reset Value
#define IDM2T_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define IDM2T_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define IDM2T_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define IDM2T_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define IDM2T_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define IDM2T_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define IDM2T_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define IDM2T_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define IDM2T_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define IDM2T_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define IDM2T_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define IDM2T_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define IDM2T_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define IDM2T_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define IDM2T_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define IDM2T_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define IDM2T_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM2T_IA_BANDWIDTH_5 Register IDM2T_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define IDM2T_IA_BANDWIDTH_5 0x10528
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2T_IA_BANDWIDTH_5 0x1DF10528u

//! Register Reset Value
#define IDM2T_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define IDM2T_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define IDM2T_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define IDM2T_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define IDM2T_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define IDM2T_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define IDM2T_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define IDM2T_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define IDM2T_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define IDM2T_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define IDM2T_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define IDM2T_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define IDM2T_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define IDM2T_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define IDM2T_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define IDM2T_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define IDM2T_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM2T_IA_BANDWIDTH_6 Register IDM2T_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define IDM2T_IA_BANDWIDTH_6 0x10530
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2T_IA_BANDWIDTH_6 0x1DF10530u

//! Register Reset Value
#define IDM2T_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define IDM2T_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define IDM2T_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define IDM2T_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define IDM2T_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define IDM2T_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define IDM2T_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define IDM2T_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define IDM2T_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define IDM2T_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define IDM2T_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define IDM2T_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define IDM2T_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define IDM2T_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define IDM2T_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define IDM2T_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define IDM2T_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM2T_IA_BANDWIDTH_7 Register IDM2T_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define IDM2T_IA_BANDWIDTH_7 0x10538
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IDM2T_IA_BANDWIDTH_7 0x1DF10538u

//! Register Reset Value
#define IDM2T_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define IDM2T_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define IDM2T_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define IDM2T_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define IDM2T_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define IDM2T_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define IDM2T_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define IDM2T_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define IDM2T_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define IDM2T_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define IDM2T_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define IDM2T_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define IDM2T_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define IDM2T_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define IDM2T_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define IDM2T_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define IDM2T_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM2_IA_COMPONENT Register IXCM2_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define IXCM2_IA_COMPONENT 0x10800
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM2_IA_COMPONENT 0x1DF10800u

//! Register Reset Value
#define IXCM2_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define IXCM2_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define IXCM2_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define IXCM2_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define IXCM2_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup IXCM2_IA_CORE Register IXCM2_IA_CORE - core
//! @{

//! Register Offset (relative)
#define IXCM2_IA_CORE 0x10818
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM2_IA_CORE 0x1DF10818u

//! Register Reset Value
#define IXCM2_IA_CORE_RST 0x000088C3001B0001u

//! Field REV_CODE - rev_code
#define IXCM2_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define IXCM2_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define IXCM2_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define IXCM2_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define IXCM2_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define IXCM2_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup IXCM2_IA_AGENT_CONTROL Register IXCM2_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define IXCM2_IA_AGENT_CONTROL 0x10820
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM2_IA_AGENT_CONTROL 0x1DF10820u

//! Register Reset Value
#define IXCM2_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IXCM2_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IXCM2_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define IXCM2_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define IXCM2_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define IXCM2_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IXCM2_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define IXCM2_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IXCM2_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define IXCM2_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define IXCM2_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IXCM2_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IXCM2_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IXCM2_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IXCM2_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IXCM2_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IXCM2_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IXCM2_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IXCM2_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IXCM2_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IXCM2_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IXCM2_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IXCM2_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup IXCM2_IA_AGENT_STATUS Register IXCM2_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define IXCM2_IA_AGENT_STATUS 0x10828
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM2_IA_AGENT_STATUS 0x1DF10828u

//! Register Reset Value
#define IXCM2_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IXCM2_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IXCM2_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define IXCM2_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define IXCM2_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define IXCM2_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define IXCM2_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define IXCM2_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define IXCM2_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define IXCM2_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define IXCM2_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define IXCM2_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IXCM2_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define IXCM2_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define IXCM2_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define IXCM2_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IXCM2_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define IXCM2_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define IXCM2_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IXCM2_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IXCM2_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IXCM2_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IXCM2_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup IXCM2_IA_ERROR_LOG Register IXCM2_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define IXCM2_IA_ERROR_LOG 0x10858
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM2_IA_ERROR_LOG 0x1DF10858u

//! Register Reset Value
#define IXCM2_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define IXCM2_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define IXCM2_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define IXCM2_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define IXCM2_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define IXCM2_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define IXCM2_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define IXCM2_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define IXCM2_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define IXCM2_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define IXCM2_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define IXCM2_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define IXCM2_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define IXCM2_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define IXCM2_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define IXCM2_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define IXCM2_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define IXCM2_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define IXCM2_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup IXCM2_IA_ERROR_LOG_ADDR Register IXCM2_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define IXCM2_IA_ERROR_LOG_ADDR 0x10860
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM2_IA_ERROR_LOG_ADDR 0x1DF10860u

//! Register Reset Value
#define IXCM2_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define IXCM2_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define IXCM2_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup IXCM2_IA_CORE_FLAG Register IXCM2_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define IXCM2_IA_CORE_FLAG 0x10868
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM2_IA_CORE_FLAG 0x1DF10868u

//! Register Reset Value
#define IXCM2_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define IXCM2_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define IXCM2_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define IXCM2_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define IXCM2_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define IXCM2_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define IXCM2_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define IXCM2_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define IXCM2_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup IXCM2_IA_ADDR_FILL_IN Register IXCM2_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define IXCM2_IA_ADDR_FILL_IN 0x10870
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM2_IA_ADDR_FILL_IN 0x1DF10870u

//! Register Reset Value
#define IXCM2_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define IXCM2_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define IXCM2_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup IXCM2_IA_BANDWIDTH_0 Register IXCM2_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define IXCM2_IA_BANDWIDTH_0 0x10900
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM2_IA_BANDWIDTH_0 0x1DF10900u

//! Register Reset Value
#define IXCM2_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define IXCM2_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define IXCM2_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define IXCM2_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define IXCM2_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define IXCM2_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define IXCM2_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define IXCM2_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define IXCM2_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define IXCM2_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define IXCM2_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define IXCM2_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define IXCM2_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define IXCM2_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define IXCM2_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define IXCM2_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define IXCM2_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM2_IA_BANDWIDTH_1 Register IXCM2_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define IXCM2_IA_BANDWIDTH_1 0x10908
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM2_IA_BANDWIDTH_1 0x1DF10908u

//! Register Reset Value
#define IXCM2_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define IXCM2_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define IXCM2_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define IXCM2_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define IXCM2_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define IXCM2_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define IXCM2_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define IXCM2_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define IXCM2_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define IXCM2_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define IXCM2_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define IXCM2_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define IXCM2_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define IXCM2_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define IXCM2_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define IXCM2_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define IXCM2_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM2_IA_BANDWIDTH_2 Register IXCM2_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define IXCM2_IA_BANDWIDTH_2 0x10910
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM2_IA_BANDWIDTH_2 0x1DF10910u

//! Register Reset Value
#define IXCM2_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define IXCM2_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define IXCM2_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define IXCM2_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define IXCM2_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define IXCM2_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define IXCM2_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define IXCM2_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define IXCM2_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define IXCM2_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define IXCM2_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define IXCM2_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define IXCM2_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define IXCM2_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define IXCM2_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define IXCM2_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define IXCM2_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM2_IA_BANDWIDTH_3 Register IXCM2_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define IXCM2_IA_BANDWIDTH_3 0x10918
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM2_IA_BANDWIDTH_3 0x1DF10918u

//! Register Reset Value
#define IXCM2_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define IXCM2_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define IXCM2_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define IXCM2_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define IXCM2_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define IXCM2_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define IXCM2_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define IXCM2_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define IXCM2_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define IXCM2_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define IXCM2_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define IXCM2_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define IXCM2_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define IXCM2_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define IXCM2_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define IXCM2_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define IXCM2_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM2_IA_BANDWIDTH_4 Register IXCM2_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define IXCM2_IA_BANDWIDTH_4 0x10920
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM2_IA_BANDWIDTH_4 0x1DF10920u

//! Register Reset Value
#define IXCM2_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define IXCM2_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define IXCM2_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define IXCM2_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define IXCM2_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define IXCM2_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define IXCM2_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define IXCM2_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define IXCM2_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define IXCM2_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define IXCM2_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define IXCM2_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define IXCM2_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define IXCM2_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define IXCM2_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define IXCM2_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define IXCM2_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM2_IA_BANDWIDTH_5 Register IXCM2_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define IXCM2_IA_BANDWIDTH_5 0x10928
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM2_IA_BANDWIDTH_5 0x1DF10928u

//! Register Reset Value
#define IXCM2_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define IXCM2_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define IXCM2_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define IXCM2_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define IXCM2_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define IXCM2_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define IXCM2_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define IXCM2_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define IXCM2_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define IXCM2_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define IXCM2_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define IXCM2_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define IXCM2_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define IXCM2_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define IXCM2_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define IXCM2_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define IXCM2_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM2_IA_BANDWIDTH_6 Register IXCM2_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define IXCM2_IA_BANDWIDTH_6 0x10930
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM2_IA_BANDWIDTH_6 0x1DF10930u

//! Register Reset Value
#define IXCM2_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define IXCM2_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define IXCM2_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define IXCM2_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define IXCM2_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define IXCM2_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define IXCM2_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define IXCM2_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define IXCM2_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define IXCM2_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define IXCM2_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define IXCM2_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define IXCM2_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define IXCM2_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define IXCM2_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define IXCM2_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define IXCM2_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM2_IA_BANDWIDTH_7 Register IXCM2_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define IXCM2_IA_BANDWIDTH_7 0x10938
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM2_IA_BANDWIDTH_7 0x1DF10938u

//! Register Reset Value
#define IXCM2_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define IXCM2_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define IXCM2_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define IXCM2_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define IXCM2_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define IXCM2_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define IXCM2_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define IXCM2_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define IXCM2_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define IXCM2_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define IXCM2_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define IXCM2_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define IXCM2_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define IXCM2_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define IXCM2_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define IXCM2_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define IXCM2_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM3_IA_COMPONENT Register IXCM3_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define IXCM3_IA_COMPONENT 0x10C00
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM3_IA_COMPONENT 0x1DF10C00u

//! Register Reset Value
#define IXCM3_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define IXCM3_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define IXCM3_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define IXCM3_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define IXCM3_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup IXCM3_IA_CORE Register IXCM3_IA_CORE - core
//! @{

//! Register Offset (relative)
#define IXCM3_IA_CORE 0x10C18
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM3_IA_CORE 0x1DF10C18u

//! Register Reset Value
#define IXCM3_IA_CORE_RST 0x000088C3001C0001u

//! Field REV_CODE - rev_code
#define IXCM3_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define IXCM3_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define IXCM3_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define IXCM3_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define IXCM3_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define IXCM3_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup IXCM3_IA_AGENT_CONTROL Register IXCM3_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define IXCM3_IA_AGENT_CONTROL 0x10C20
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM3_IA_AGENT_CONTROL 0x1DF10C20u

//! Register Reset Value
#define IXCM3_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IXCM3_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IXCM3_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define IXCM3_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define IXCM3_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define IXCM3_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IXCM3_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define IXCM3_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IXCM3_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define IXCM3_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define IXCM3_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IXCM3_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IXCM3_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IXCM3_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IXCM3_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IXCM3_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IXCM3_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IXCM3_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IXCM3_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IXCM3_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IXCM3_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IXCM3_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IXCM3_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup IXCM3_IA_AGENT_STATUS Register IXCM3_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define IXCM3_IA_AGENT_STATUS 0x10C28
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM3_IA_AGENT_STATUS 0x1DF10C28u

//! Register Reset Value
#define IXCM3_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IXCM3_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IXCM3_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define IXCM3_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define IXCM3_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define IXCM3_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define IXCM3_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define IXCM3_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define IXCM3_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define IXCM3_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define IXCM3_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define IXCM3_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IXCM3_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define IXCM3_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define IXCM3_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define IXCM3_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IXCM3_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define IXCM3_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define IXCM3_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IXCM3_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IXCM3_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IXCM3_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IXCM3_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup IXCM3_IA_ERROR_LOG Register IXCM3_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define IXCM3_IA_ERROR_LOG 0x10C58
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM3_IA_ERROR_LOG 0x1DF10C58u

//! Register Reset Value
#define IXCM3_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define IXCM3_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define IXCM3_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define IXCM3_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define IXCM3_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define IXCM3_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define IXCM3_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define IXCM3_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define IXCM3_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define IXCM3_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define IXCM3_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define IXCM3_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define IXCM3_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define IXCM3_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define IXCM3_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define IXCM3_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define IXCM3_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define IXCM3_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define IXCM3_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup IXCM3_IA_ERROR_LOG_ADDR Register IXCM3_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define IXCM3_IA_ERROR_LOG_ADDR 0x10C60
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM3_IA_ERROR_LOG_ADDR 0x1DF10C60u

//! Register Reset Value
#define IXCM3_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define IXCM3_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define IXCM3_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup IXCM3_IA_CORE_FLAG Register IXCM3_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define IXCM3_IA_CORE_FLAG 0x10C68
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM3_IA_CORE_FLAG 0x1DF10C68u

//! Register Reset Value
#define IXCM3_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define IXCM3_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define IXCM3_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define IXCM3_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define IXCM3_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define IXCM3_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define IXCM3_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define IXCM3_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define IXCM3_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup IXCM3_IA_ADDR_FILL_IN Register IXCM3_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define IXCM3_IA_ADDR_FILL_IN 0x10C70
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM3_IA_ADDR_FILL_IN 0x1DF10C70u

//! Register Reset Value
#define IXCM3_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define IXCM3_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define IXCM3_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup IXCM3_IA_BANDWIDTH_0 Register IXCM3_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define IXCM3_IA_BANDWIDTH_0 0x10D00
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM3_IA_BANDWIDTH_0 0x1DF10D00u

//! Register Reset Value
#define IXCM3_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define IXCM3_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define IXCM3_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define IXCM3_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define IXCM3_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define IXCM3_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define IXCM3_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define IXCM3_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define IXCM3_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define IXCM3_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define IXCM3_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define IXCM3_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define IXCM3_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define IXCM3_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define IXCM3_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define IXCM3_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define IXCM3_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM3_IA_BANDWIDTH_1 Register IXCM3_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define IXCM3_IA_BANDWIDTH_1 0x10D08
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM3_IA_BANDWIDTH_1 0x1DF10D08u

//! Register Reset Value
#define IXCM3_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define IXCM3_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define IXCM3_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define IXCM3_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define IXCM3_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define IXCM3_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define IXCM3_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define IXCM3_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define IXCM3_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define IXCM3_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define IXCM3_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define IXCM3_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define IXCM3_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define IXCM3_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define IXCM3_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define IXCM3_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define IXCM3_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM3_IA_BANDWIDTH_2 Register IXCM3_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define IXCM3_IA_BANDWIDTH_2 0x10D10
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM3_IA_BANDWIDTH_2 0x1DF10D10u

//! Register Reset Value
#define IXCM3_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define IXCM3_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define IXCM3_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define IXCM3_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define IXCM3_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define IXCM3_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define IXCM3_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define IXCM3_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define IXCM3_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define IXCM3_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define IXCM3_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define IXCM3_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define IXCM3_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define IXCM3_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define IXCM3_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define IXCM3_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define IXCM3_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM3_IA_BANDWIDTH_3 Register IXCM3_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define IXCM3_IA_BANDWIDTH_3 0x10D18
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM3_IA_BANDWIDTH_3 0x1DF10D18u

//! Register Reset Value
#define IXCM3_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define IXCM3_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define IXCM3_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define IXCM3_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define IXCM3_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define IXCM3_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define IXCM3_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define IXCM3_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define IXCM3_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define IXCM3_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define IXCM3_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define IXCM3_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define IXCM3_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define IXCM3_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define IXCM3_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define IXCM3_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define IXCM3_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM3_IA_BANDWIDTH_4 Register IXCM3_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define IXCM3_IA_BANDWIDTH_4 0x10D20
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM3_IA_BANDWIDTH_4 0x1DF10D20u

//! Register Reset Value
#define IXCM3_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define IXCM3_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define IXCM3_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define IXCM3_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define IXCM3_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define IXCM3_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define IXCM3_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define IXCM3_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define IXCM3_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define IXCM3_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define IXCM3_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define IXCM3_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define IXCM3_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define IXCM3_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define IXCM3_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define IXCM3_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define IXCM3_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM3_IA_BANDWIDTH_5 Register IXCM3_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define IXCM3_IA_BANDWIDTH_5 0x10D28
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM3_IA_BANDWIDTH_5 0x1DF10D28u

//! Register Reset Value
#define IXCM3_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define IXCM3_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define IXCM3_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define IXCM3_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define IXCM3_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define IXCM3_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define IXCM3_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define IXCM3_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define IXCM3_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define IXCM3_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define IXCM3_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define IXCM3_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define IXCM3_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define IXCM3_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define IXCM3_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define IXCM3_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define IXCM3_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM3_IA_BANDWIDTH_6 Register IXCM3_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define IXCM3_IA_BANDWIDTH_6 0x10D30
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM3_IA_BANDWIDTH_6 0x1DF10D30u

//! Register Reset Value
#define IXCM3_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define IXCM3_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define IXCM3_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define IXCM3_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define IXCM3_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define IXCM3_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define IXCM3_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define IXCM3_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define IXCM3_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define IXCM3_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define IXCM3_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define IXCM3_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define IXCM3_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define IXCM3_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define IXCM3_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define IXCM3_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define IXCM3_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM3_IA_BANDWIDTH_7 Register IXCM3_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define IXCM3_IA_BANDWIDTH_7 0x10D38
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM3_IA_BANDWIDTH_7 0x1DF10D38u

//! Register Reset Value
#define IXCM3_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define IXCM3_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define IXCM3_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define IXCM3_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define IXCM3_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define IXCM3_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define IXCM3_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define IXCM3_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define IXCM3_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define IXCM3_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define IXCM3_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define IXCM3_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define IXCM3_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define IXCM3_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define IXCM3_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define IXCM3_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define IXCM3_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM4_IA_COMPONENT Register IXCM4_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define IXCM4_IA_COMPONENT 0x11000
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM4_IA_COMPONENT 0x1DF11000u

//! Register Reset Value
#define IXCM4_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define IXCM4_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define IXCM4_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define IXCM4_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define IXCM4_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup IXCM4_IA_CORE Register IXCM4_IA_CORE - core
//! @{

//! Register Offset (relative)
#define IXCM4_IA_CORE 0x11018
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM4_IA_CORE 0x1DF11018u

//! Register Reset Value
#define IXCM4_IA_CORE_RST 0x000088C3001D0001u

//! Field REV_CODE - rev_code
#define IXCM4_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define IXCM4_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define IXCM4_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define IXCM4_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define IXCM4_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define IXCM4_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup IXCM4_IA_AGENT_CONTROL Register IXCM4_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define IXCM4_IA_AGENT_CONTROL 0x11020
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM4_IA_AGENT_CONTROL 0x1DF11020u

//! Register Reset Value
#define IXCM4_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IXCM4_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IXCM4_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define IXCM4_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define IXCM4_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define IXCM4_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IXCM4_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define IXCM4_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IXCM4_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define IXCM4_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define IXCM4_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IXCM4_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IXCM4_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IXCM4_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IXCM4_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IXCM4_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IXCM4_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IXCM4_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IXCM4_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IXCM4_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IXCM4_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IXCM4_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IXCM4_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup IXCM4_IA_AGENT_STATUS Register IXCM4_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define IXCM4_IA_AGENT_STATUS 0x11028
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM4_IA_AGENT_STATUS 0x1DF11028u

//! Register Reset Value
#define IXCM4_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IXCM4_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IXCM4_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define IXCM4_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define IXCM4_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define IXCM4_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define IXCM4_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define IXCM4_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define IXCM4_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define IXCM4_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define IXCM4_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define IXCM4_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IXCM4_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define IXCM4_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define IXCM4_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define IXCM4_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IXCM4_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define IXCM4_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define IXCM4_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IXCM4_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IXCM4_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IXCM4_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IXCM4_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup IXCM4_IA_ERROR_LOG Register IXCM4_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define IXCM4_IA_ERROR_LOG 0x11058
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM4_IA_ERROR_LOG 0x1DF11058u

//! Register Reset Value
#define IXCM4_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define IXCM4_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define IXCM4_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define IXCM4_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define IXCM4_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define IXCM4_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define IXCM4_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define IXCM4_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define IXCM4_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define IXCM4_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define IXCM4_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define IXCM4_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define IXCM4_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define IXCM4_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define IXCM4_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define IXCM4_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define IXCM4_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define IXCM4_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define IXCM4_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup IXCM4_IA_ERROR_LOG_ADDR Register IXCM4_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define IXCM4_IA_ERROR_LOG_ADDR 0x11060
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM4_IA_ERROR_LOG_ADDR 0x1DF11060u

//! Register Reset Value
#define IXCM4_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define IXCM4_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define IXCM4_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup IXCM4_IA_CORE_FLAG Register IXCM4_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define IXCM4_IA_CORE_FLAG 0x11068
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM4_IA_CORE_FLAG 0x1DF11068u

//! Register Reset Value
#define IXCM4_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define IXCM4_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define IXCM4_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define IXCM4_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define IXCM4_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define IXCM4_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define IXCM4_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define IXCM4_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define IXCM4_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup IXCM4_IA_ADDR_FILL_IN Register IXCM4_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define IXCM4_IA_ADDR_FILL_IN 0x11070
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM4_IA_ADDR_FILL_IN 0x1DF11070u

//! Register Reset Value
#define IXCM4_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define IXCM4_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define IXCM4_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup IXCM4_IA_BANDWIDTH_0 Register IXCM4_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define IXCM4_IA_BANDWIDTH_0 0x11100
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM4_IA_BANDWIDTH_0 0x1DF11100u

//! Register Reset Value
#define IXCM4_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define IXCM4_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define IXCM4_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define IXCM4_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define IXCM4_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define IXCM4_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define IXCM4_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define IXCM4_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define IXCM4_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define IXCM4_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define IXCM4_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define IXCM4_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define IXCM4_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define IXCM4_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define IXCM4_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define IXCM4_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define IXCM4_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM4_IA_BANDWIDTH_1 Register IXCM4_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define IXCM4_IA_BANDWIDTH_1 0x11108
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM4_IA_BANDWIDTH_1 0x1DF11108u

//! Register Reset Value
#define IXCM4_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define IXCM4_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define IXCM4_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define IXCM4_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define IXCM4_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define IXCM4_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define IXCM4_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define IXCM4_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define IXCM4_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define IXCM4_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define IXCM4_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define IXCM4_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define IXCM4_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define IXCM4_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define IXCM4_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define IXCM4_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define IXCM4_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM4_IA_BANDWIDTH_2 Register IXCM4_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define IXCM4_IA_BANDWIDTH_2 0x11110
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM4_IA_BANDWIDTH_2 0x1DF11110u

//! Register Reset Value
#define IXCM4_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define IXCM4_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define IXCM4_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define IXCM4_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define IXCM4_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define IXCM4_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define IXCM4_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define IXCM4_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define IXCM4_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define IXCM4_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define IXCM4_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define IXCM4_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define IXCM4_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define IXCM4_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define IXCM4_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define IXCM4_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define IXCM4_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM4_IA_BANDWIDTH_3 Register IXCM4_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define IXCM4_IA_BANDWIDTH_3 0x11118
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM4_IA_BANDWIDTH_3 0x1DF11118u

//! Register Reset Value
#define IXCM4_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define IXCM4_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define IXCM4_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define IXCM4_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define IXCM4_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define IXCM4_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define IXCM4_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define IXCM4_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define IXCM4_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define IXCM4_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define IXCM4_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define IXCM4_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define IXCM4_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define IXCM4_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define IXCM4_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define IXCM4_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define IXCM4_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM4_IA_BANDWIDTH_4 Register IXCM4_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define IXCM4_IA_BANDWIDTH_4 0x11120
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM4_IA_BANDWIDTH_4 0x1DF11120u

//! Register Reset Value
#define IXCM4_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define IXCM4_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define IXCM4_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define IXCM4_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define IXCM4_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define IXCM4_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define IXCM4_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define IXCM4_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define IXCM4_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define IXCM4_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define IXCM4_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define IXCM4_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define IXCM4_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define IXCM4_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define IXCM4_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define IXCM4_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define IXCM4_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM4_IA_BANDWIDTH_5 Register IXCM4_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define IXCM4_IA_BANDWIDTH_5 0x11128
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM4_IA_BANDWIDTH_5 0x1DF11128u

//! Register Reset Value
#define IXCM4_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define IXCM4_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define IXCM4_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define IXCM4_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define IXCM4_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define IXCM4_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define IXCM4_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define IXCM4_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define IXCM4_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define IXCM4_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define IXCM4_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define IXCM4_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define IXCM4_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define IXCM4_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define IXCM4_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define IXCM4_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define IXCM4_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM4_IA_BANDWIDTH_6 Register IXCM4_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define IXCM4_IA_BANDWIDTH_6 0x11130
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM4_IA_BANDWIDTH_6 0x1DF11130u

//! Register Reset Value
#define IXCM4_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define IXCM4_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define IXCM4_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define IXCM4_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define IXCM4_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define IXCM4_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define IXCM4_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define IXCM4_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define IXCM4_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define IXCM4_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define IXCM4_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define IXCM4_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define IXCM4_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define IXCM4_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define IXCM4_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define IXCM4_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define IXCM4_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM4_IA_BANDWIDTH_7 Register IXCM4_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define IXCM4_IA_BANDWIDTH_7 0x11138
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM4_IA_BANDWIDTH_7 0x1DF11138u

//! Register Reset Value
#define IXCM4_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define IXCM4_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define IXCM4_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define IXCM4_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define IXCM4_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define IXCM4_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define IXCM4_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define IXCM4_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define IXCM4_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define IXCM4_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define IXCM4_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define IXCM4_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define IXCM4_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define IXCM4_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define IXCM4_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define IXCM4_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define IXCM4_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM5_IA_COMPONENT Register IXCM5_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define IXCM5_IA_COMPONENT 0x11400
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM5_IA_COMPONENT 0x1DF11400u

//! Register Reset Value
#define IXCM5_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define IXCM5_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define IXCM5_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define IXCM5_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define IXCM5_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup IXCM5_IA_CORE Register IXCM5_IA_CORE - core
//! @{

//! Register Offset (relative)
#define IXCM5_IA_CORE 0x11418
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM5_IA_CORE 0x1DF11418u

//! Register Reset Value
#define IXCM5_IA_CORE_RST 0x000088C3001E0001u

//! Field REV_CODE - rev_code
#define IXCM5_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define IXCM5_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define IXCM5_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define IXCM5_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define IXCM5_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define IXCM5_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup IXCM5_IA_AGENT_CONTROL Register IXCM5_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define IXCM5_IA_AGENT_CONTROL 0x11420
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM5_IA_AGENT_CONTROL 0x1DF11420u

//! Register Reset Value
#define IXCM5_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IXCM5_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IXCM5_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define IXCM5_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define IXCM5_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define IXCM5_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IXCM5_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define IXCM5_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IXCM5_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define IXCM5_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define IXCM5_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IXCM5_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IXCM5_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IXCM5_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IXCM5_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IXCM5_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IXCM5_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IXCM5_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IXCM5_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IXCM5_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IXCM5_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IXCM5_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IXCM5_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup IXCM5_IA_AGENT_STATUS Register IXCM5_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define IXCM5_IA_AGENT_STATUS 0x11428
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM5_IA_AGENT_STATUS 0x1DF11428u

//! Register Reset Value
#define IXCM5_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IXCM5_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IXCM5_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define IXCM5_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define IXCM5_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define IXCM5_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define IXCM5_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define IXCM5_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define IXCM5_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define IXCM5_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define IXCM5_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define IXCM5_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IXCM5_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define IXCM5_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define IXCM5_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define IXCM5_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IXCM5_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define IXCM5_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define IXCM5_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IXCM5_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IXCM5_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IXCM5_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IXCM5_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup IXCM5_IA_ERROR_LOG Register IXCM5_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define IXCM5_IA_ERROR_LOG 0x11458
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM5_IA_ERROR_LOG 0x1DF11458u

//! Register Reset Value
#define IXCM5_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define IXCM5_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define IXCM5_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define IXCM5_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define IXCM5_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define IXCM5_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define IXCM5_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define IXCM5_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define IXCM5_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define IXCM5_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define IXCM5_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define IXCM5_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define IXCM5_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define IXCM5_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define IXCM5_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define IXCM5_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define IXCM5_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define IXCM5_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define IXCM5_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup IXCM5_IA_ERROR_LOG_ADDR Register IXCM5_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define IXCM5_IA_ERROR_LOG_ADDR 0x11460
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM5_IA_ERROR_LOG_ADDR 0x1DF11460u

//! Register Reset Value
#define IXCM5_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define IXCM5_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define IXCM5_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup IXCM5_IA_CORE_FLAG Register IXCM5_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define IXCM5_IA_CORE_FLAG 0x11468
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM5_IA_CORE_FLAG 0x1DF11468u

//! Register Reset Value
#define IXCM5_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define IXCM5_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define IXCM5_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define IXCM5_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define IXCM5_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define IXCM5_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define IXCM5_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define IXCM5_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define IXCM5_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup IXCM5_IA_ADDR_FILL_IN Register IXCM5_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define IXCM5_IA_ADDR_FILL_IN 0x11470
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM5_IA_ADDR_FILL_IN 0x1DF11470u

//! Register Reset Value
#define IXCM5_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define IXCM5_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define IXCM5_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup IXCM5_IA_BANDWIDTH_0 Register IXCM5_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define IXCM5_IA_BANDWIDTH_0 0x11500
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM5_IA_BANDWIDTH_0 0x1DF11500u

//! Register Reset Value
#define IXCM5_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define IXCM5_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define IXCM5_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define IXCM5_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define IXCM5_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define IXCM5_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define IXCM5_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define IXCM5_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define IXCM5_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define IXCM5_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define IXCM5_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define IXCM5_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define IXCM5_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define IXCM5_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define IXCM5_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define IXCM5_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define IXCM5_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM5_IA_BANDWIDTH_1 Register IXCM5_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define IXCM5_IA_BANDWIDTH_1 0x11508
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM5_IA_BANDWIDTH_1 0x1DF11508u

//! Register Reset Value
#define IXCM5_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define IXCM5_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define IXCM5_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define IXCM5_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define IXCM5_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define IXCM5_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define IXCM5_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define IXCM5_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define IXCM5_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define IXCM5_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define IXCM5_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define IXCM5_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define IXCM5_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define IXCM5_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define IXCM5_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define IXCM5_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define IXCM5_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM5_IA_BANDWIDTH_2 Register IXCM5_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define IXCM5_IA_BANDWIDTH_2 0x11510
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM5_IA_BANDWIDTH_2 0x1DF11510u

//! Register Reset Value
#define IXCM5_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define IXCM5_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define IXCM5_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define IXCM5_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define IXCM5_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define IXCM5_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define IXCM5_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define IXCM5_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define IXCM5_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define IXCM5_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define IXCM5_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define IXCM5_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define IXCM5_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define IXCM5_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define IXCM5_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define IXCM5_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define IXCM5_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM5_IA_BANDWIDTH_3 Register IXCM5_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define IXCM5_IA_BANDWIDTH_3 0x11518
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM5_IA_BANDWIDTH_3 0x1DF11518u

//! Register Reset Value
#define IXCM5_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define IXCM5_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define IXCM5_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define IXCM5_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define IXCM5_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define IXCM5_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define IXCM5_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define IXCM5_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define IXCM5_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define IXCM5_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define IXCM5_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define IXCM5_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define IXCM5_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define IXCM5_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define IXCM5_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define IXCM5_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define IXCM5_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM5_IA_BANDWIDTH_4 Register IXCM5_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define IXCM5_IA_BANDWIDTH_4 0x11520
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM5_IA_BANDWIDTH_4 0x1DF11520u

//! Register Reset Value
#define IXCM5_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define IXCM5_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define IXCM5_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define IXCM5_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define IXCM5_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define IXCM5_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define IXCM5_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define IXCM5_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define IXCM5_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define IXCM5_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define IXCM5_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define IXCM5_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define IXCM5_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define IXCM5_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define IXCM5_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define IXCM5_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define IXCM5_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM5_IA_BANDWIDTH_5 Register IXCM5_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define IXCM5_IA_BANDWIDTH_5 0x11528
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM5_IA_BANDWIDTH_5 0x1DF11528u

//! Register Reset Value
#define IXCM5_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define IXCM5_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define IXCM5_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define IXCM5_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define IXCM5_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define IXCM5_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define IXCM5_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define IXCM5_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define IXCM5_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define IXCM5_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define IXCM5_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define IXCM5_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define IXCM5_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define IXCM5_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define IXCM5_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define IXCM5_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define IXCM5_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM5_IA_BANDWIDTH_6 Register IXCM5_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define IXCM5_IA_BANDWIDTH_6 0x11530
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM5_IA_BANDWIDTH_6 0x1DF11530u

//! Register Reset Value
#define IXCM5_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define IXCM5_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define IXCM5_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define IXCM5_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define IXCM5_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define IXCM5_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define IXCM5_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define IXCM5_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define IXCM5_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define IXCM5_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define IXCM5_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define IXCM5_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define IXCM5_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define IXCM5_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define IXCM5_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define IXCM5_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define IXCM5_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCM5_IA_BANDWIDTH_7 Register IXCM5_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define IXCM5_IA_BANDWIDTH_7 0x11538
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_IXCM5_IA_BANDWIDTH_7 0x1DF11538u

//! Register Reset Value
#define IXCM5_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define IXCM5_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define IXCM5_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define IXCM5_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define IXCM5_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define IXCM5_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define IXCM5_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define IXCM5_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define IXCM5_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define IXCM5_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define IXCM5_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define IXCM5_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define IXCM5_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define IXCM5_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define IXCM5_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define IXCM5_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define IXCM5_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN01_IA_COMPONENT Register ILN01_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define ILN01_IA_COMPONENT 0x11800
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_ILN01_IA_COMPONENT 0x1DF11800u

//! Register Reset Value
#define ILN01_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define ILN01_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define ILN01_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define ILN01_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define ILN01_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup ILN01_IA_CORE Register ILN01_IA_CORE - core
//! @{

//! Register Offset (relative)
#define ILN01_IA_CORE 0x11818
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_ILN01_IA_CORE 0x1DF11818u

//! Register Reset Value
#define ILN01_IA_CORE_RST 0x000050C50F100001u

//! Field REV_CODE - rev_code
#define ILN01_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define ILN01_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define ILN01_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define ILN01_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define ILN01_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define ILN01_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup ILN01_IA_AGENT_CONTROL Register ILN01_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define ILN01_IA_AGENT_CONTROL 0x11820
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_ILN01_IA_AGENT_CONTROL 0x1DF11820u

//! Register Reset Value
#define ILN01_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define ILN01_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define ILN01_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define ILN01_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define ILN01_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define ILN01_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define ILN01_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define ILN01_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define ILN01_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define ILN01_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define ILN01_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define ILN01_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define ILN01_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define ILN01_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define ILN01_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define ILN01_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define ILN01_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define ILN01_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define ILN01_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define ILN01_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define ILN01_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define ILN01_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define ILN01_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup ILN01_IA_AGENT_STATUS Register ILN01_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define ILN01_IA_AGENT_STATUS 0x11828
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_ILN01_IA_AGENT_STATUS 0x1DF11828u

//! Register Reset Value
#define ILN01_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define ILN01_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define ILN01_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define ILN01_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define ILN01_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define ILN01_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define ILN01_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define ILN01_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define ILN01_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define ILN01_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define ILN01_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define ILN01_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define ILN01_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define ILN01_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define ILN01_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define ILN01_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define ILN01_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define ILN01_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define ILN01_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define ILN01_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define ILN01_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define ILN01_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define ILN01_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup ILN01_IA_ERROR_LOG Register ILN01_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define ILN01_IA_ERROR_LOG 0x11858
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_ILN01_IA_ERROR_LOG 0x1DF11858u

//! Register Reset Value
#define ILN01_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define ILN01_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define ILN01_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define ILN01_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define ILN01_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define ILN01_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define ILN01_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define ILN01_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define ILN01_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define ILN01_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define ILN01_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define ILN01_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define ILN01_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define ILN01_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define ILN01_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define ILN01_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define ILN01_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define ILN01_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define ILN01_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup ILN01_IA_ERROR_LOG_ADDR Register ILN01_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define ILN01_IA_ERROR_LOG_ADDR 0x11860
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_ILN01_IA_ERROR_LOG_ADDR 0x1DF11860u

//! Register Reset Value
#define ILN01_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define ILN01_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define ILN01_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup ILN01_IA_CORE_FLAG Register ILN01_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define ILN01_IA_CORE_FLAG 0x11868
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_ILN01_IA_CORE_FLAG 0x1DF11868u

//! Register Reset Value
#define ILN01_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define ILN01_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define ILN01_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define ILN01_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define ILN01_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define ILN01_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define ILN01_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define ILN01_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define ILN01_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup ILN01_IA_ADDR_FILL_IN Register ILN01_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define ILN01_IA_ADDR_FILL_IN 0x11870
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_ILN01_IA_ADDR_FILL_IN 0x1DF11870u

//! Register Reset Value
#define ILN01_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define ILN01_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define ILN01_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup ILN01_IA_BANDWIDTH_0 Register ILN01_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define ILN01_IA_BANDWIDTH_0 0x11900
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_ILN01_IA_BANDWIDTH_0 0x1DF11900u

//! Register Reset Value
#define ILN01_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define ILN01_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define ILN01_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define ILN01_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define ILN01_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define ILN01_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define ILN01_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define ILN01_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define ILN01_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define ILN01_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define ILN01_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define ILN01_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define ILN01_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define ILN01_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define ILN01_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define ILN01_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define ILN01_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN01_IA_BANDWIDTH_1 Register ILN01_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define ILN01_IA_BANDWIDTH_1 0x11908
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_ILN01_IA_BANDWIDTH_1 0x1DF11908u

//! Register Reset Value
#define ILN01_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define ILN01_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define ILN01_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define ILN01_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define ILN01_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define ILN01_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define ILN01_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define ILN01_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define ILN01_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define ILN01_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define ILN01_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define ILN01_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define ILN01_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define ILN01_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define ILN01_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define ILN01_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define ILN01_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN01_IA_BANDWIDTH_2 Register ILN01_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define ILN01_IA_BANDWIDTH_2 0x11910
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_ILN01_IA_BANDWIDTH_2 0x1DF11910u

//! Register Reset Value
#define ILN01_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define ILN01_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define ILN01_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define ILN01_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define ILN01_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define ILN01_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define ILN01_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define ILN01_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define ILN01_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define ILN01_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define ILN01_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define ILN01_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define ILN01_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define ILN01_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define ILN01_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define ILN01_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define ILN01_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN01_IA_BANDWIDTH_3 Register ILN01_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define ILN01_IA_BANDWIDTH_3 0x11918
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_ILN01_IA_BANDWIDTH_3 0x1DF11918u

//! Register Reset Value
#define ILN01_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define ILN01_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define ILN01_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define ILN01_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define ILN01_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define ILN01_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define ILN01_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define ILN01_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define ILN01_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define ILN01_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define ILN01_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define ILN01_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define ILN01_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define ILN01_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define ILN01_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define ILN01_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define ILN01_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN01_IA_BANDWIDTH_4 Register ILN01_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define ILN01_IA_BANDWIDTH_4 0x11920
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_ILN01_IA_BANDWIDTH_4 0x1DF11920u

//! Register Reset Value
#define ILN01_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define ILN01_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define ILN01_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define ILN01_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define ILN01_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define ILN01_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define ILN01_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define ILN01_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define ILN01_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define ILN01_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define ILN01_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define ILN01_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define ILN01_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define ILN01_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define ILN01_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define ILN01_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define ILN01_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN01_IA_BANDWIDTH_5 Register ILN01_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define ILN01_IA_BANDWIDTH_5 0x11928
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_ILN01_IA_BANDWIDTH_5 0x1DF11928u

//! Register Reset Value
#define ILN01_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define ILN01_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define ILN01_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define ILN01_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define ILN01_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define ILN01_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define ILN01_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define ILN01_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define ILN01_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define ILN01_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define ILN01_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define ILN01_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define ILN01_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define ILN01_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define ILN01_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define ILN01_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define ILN01_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN01_IA_BANDWIDTH_6 Register ILN01_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define ILN01_IA_BANDWIDTH_6 0x11930
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_ILN01_IA_BANDWIDTH_6 0x1DF11930u

//! Register Reset Value
#define ILN01_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define ILN01_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define ILN01_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define ILN01_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define ILN01_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define ILN01_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define ILN01_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define ILN01_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define ILN01_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define ILN01_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define ILN01_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define ILN01_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define ILN01_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define ILN01_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define ILN01_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define ILN01_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define ILN01_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN01_IA_BANDWIDTH_7 Register ILN01_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define ILN01_IA_BANDWIDTH_7 0x11938
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_ILN01_IA_BANDWIDTH_7 0x1DF11938u

//! Register Reset Value
#define ILN01_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define ILN01_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define ILN01_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define ILN01_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define ILN01_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define ILN01_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define ILN01_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define ILN01_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define ILN01_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define ILN01_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define ILN01_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define ILN01_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define ILN01_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define ILN01_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define ILN01_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define ILN01_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define ILN01_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup TREG1_PM_ERROR_LOG Register TREG1_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TREG1_PM_ERROR_LOG 0x80020
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ERROR_LOG 0x1DF80020u

//! Register Reset Value
#define TREG1_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TREG1_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TREG1_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TREG1_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TREG1_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TREG1_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TREG1_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TREG1_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TREG1_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TREG1_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TREG1_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TREG1_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TREG1_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TREG1_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TREG1_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TREG1_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TREG1_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TREG1_PM_CONTROL Register TREG1_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TREG1_PM_CONTROL 0x80028
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_CONTROL 0x1DF80028u

//! Register Reset Value
#define TREG1_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TREG1_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TREG1_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TREG1_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TREG1_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TREG1_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TREG1_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TREG1_PM_ERROR_CLEAR_SINGLE Register TREG1_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TREG1_PM_ERROR_CLEAR_SINGLE 0x80030
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ERROR_CLEAR_SINGLE 0x1DF80030u

//! Register Reset Value
#define TREG1_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TREG1_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TREG1_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TREG1_PM_ERROR_CLEAR_MULTI Register TREG1_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TREG1_PM_ERROR_CLEAR_MULTI 0x80038
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ERROR_CLEAR_MULTI 0x1DF80038u

//! Register Reset Value
#define TREG1_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TREG1_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TREG1_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TREG1_PM_REQ_INFO_PERMISSION_0 Register TREG1_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TREG1_PM_REQ_INFO_PERMISSION_0 0x80048
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_REQ_INFO_PERMISSION_0 0x1DF80048u

//! Register Reset Value
#define TREG1_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_READ_PERMISSION_0 Register TREG1_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TREG1_PM_READ_PERMISSION_0 0x80050
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_READ_PERMISSION_0 0x1DF80050u

//! Register Reset Value
#define TREG1_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TREG1_PM_WRITE_PERMISSION_0 Register TREG1_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TREG1_PM_WRITE_PERMISSION_0 0x80058
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_WRITE_PERMISSION_0 0x1DF80058u

//! Register Reset Value
#define TREG1_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TREG1_PM_ADDR_MATCH_1 Register TREG1_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TREG1_PM_ADDR_MATCH_1 0x80060
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ADDR_MATCH_1 0x1DF80060u

//! Register Reset Value
#define TREG1_PM_ADDR_MATCH_1_RST 0x0000000000080050u

//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG1_PM_REQ_INFO_PERMISSION_1 Register TREG1_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TREG1_PM_REQ_INFO_PERMISSION_1 0x80068
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_REQ_INFO_PERMISSION_1 0x1DF80068u

//! Register Reset Value
#define TREG1_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_READ_PERMISSION_1 Register TREG1_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TREG1_PM_READ_PERMISSION_1 0x80070
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_READ_PERMISSION_1 0x1DF80070u

//! Register Reset Value
#define TREG1_PM_READ_PERMISSION_1_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TREG1_PM_WRITE_PERMISSION_1 Register TREG1_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TREG1_PM_WRITE_PERMISSION_1 0x80078
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_WRITE_PERMISSION_1 0x1DF80078u

//! Register Reset Value
#define TREG1_PM_WRITE_PERMISSION_1_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TREG1_PM_ADDR_MATCH_2 Register TREG1_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TREG1_PM_ADDR_MATCH_2 0x80080
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ADDR_MATCH_2 0x1DF80080u

//! Register Reset Value
#define TREG1_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG1_PM_REQ_INFO_PERMISSION_2 Register TREG1_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TREG1_PM_REQ_INFO_PERMISSION_2 0x80088
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_REQ_INFO_PERMISSION_2 0x1DF80088u

//! Register Reset Value
#define TREG1_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_READ_PERMISSION_2 Register TREG1_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TREG1_PM_READ_PERMISSION_2 0x80090
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_READ_PERMISSION_2 0x1DF80090u

//! Register Reset Value
#define TREG1_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_WRITE_PERMISSION_2 Register TREG1_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TREG1_PM_WRITE_PERMISSION_2 0x80098
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_WRITE_PERMISSION_2 0x1DF80098u

//! Register Reset Value
#define TREG1_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_ADDR_MATCH_3 Register TREG1_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TREG1_PM_ADDR_MATCH_3 0x800A0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ADDR_MATCH_3 0x1DF800A0u

//! Register Reset Value
#define TREG1_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG1_PM_REQ_INFO_PERMISSION_3 Register TREG1_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TREG1_PM_REQ_INFO_PERMISSION_3 0x800A8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_REQ_INFO_PERMISSION_3 0x1DF800A8u

//! Register Reset Value
#define TREG1_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_READ_PERMISSION_3 Register TREG1_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TREG1_PM_READ_PERMISSION_3 0x800B0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_READ_PERMISSION_3 0x1DF800B0u

//! Register Reset Value
#define TREG1_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_WRITE_PERMISSION_3 Register TREG1_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TREG1_PM_WRITE_PERMISSION_3 0x800B8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_WRITE_PERMISSION_3 0x1DF800B8u

//! Register Reset Value
#define TREG1_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_ADDR_MATCH_4 Register TREG1_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TREG1_PM_ADDR_MATCH_4 0x800C0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ADDR_MATCH_4 0x1DF800C0u

//! Register Reset Value
#define TREG1_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG1_PM_REQ_INFO_PERMISSION_4 Register TREG1_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TREG1_PM_REQ_INFO_PERMISSION_4 0x800C8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_REQ_INFO_PERMISSION_4 0x1DF800C8u

//! Register Reset Value
#define TREG1_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_READ_PERMISSION_4 Register TREG1_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TREG1_PM_READ_PERMISSION_4 0x800D0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_READ_PERMISSION_4 0x1DF800D0u

//! Register Reset Value
#define TREG1_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_WRITE_PERMISSION_4 Register TREG1_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TREG1_PM_WRITE_PERMISSION_4 0x800D8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_WRITE_PERMISSION_4 0x1DF800D8u

//! Register Reset Value
#define TREG1_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_ADDR_MATCH_5 Register TREG1_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TREG1_PM_ADDR_MATCH_5 0x800E0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ADDR_MATCH_5 0x1DF800E0u

//! Register Reset Value
#define TREG1_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG1_PM_REQ_INFO_PERMISSION_5 Register TREG1_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TREG1_PM_REQ_INFO_PERMISSION_5 0x800E8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_REQ_INFO_PERMISSION_5 0x1DF800E8u

//! Register Reset Value
#define TREG1_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_READ_PERMISSION_5 Register TREG1_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TREG1_PM_READ_PERMISSION_5 0x800F0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_READ_PERMISSION_5 0x1DF800F0u

//! Register Reset Value
#define TREG1_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_WRITE_PERMISSION_5 Register TREG1_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TREG1_PM_WRITE_PERMISSION_5 0x800F8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_WRITE_PERMISSION_5 0x1DF800F8u

//! Register Reset Value
#define TREG1_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_ADDR_MATCH_6 Register TREG1_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TREG1_PM_ADDR_MATCH_6 0x80100
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ADDR_MATCH_6 0x1DF80100u

//! Register Reset Value
#define TREG1_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG1_PM_REQ_INFO_PERMISSION_6 Register TREG1_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TREG1_PM_REQ_INFO_PERMISSION_6 0x80108
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_REQ_INFO_PERMISSION_6 0x1DF80108u

//! Register Reset Value
#define TREG1_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_READ_PERMISSION_6 Register TREG1_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TREG1_PM_READ_PERMISSION_6 0x80110
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_READ_PERMISSION_6 0x1DF80110u

//! Register Reset Value
#define TREG1_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_WRITE_PERMISSION_6 Register TREG1_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TREG1_PM_WRITE_PERMISSION_6 0x80118
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_WRITE_PERMISSION_6 0x1DF80118u

//! Register Reset Value
#define TREG1_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_ADDR_MATCH_7 Register TREG1_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TREG1_PM_ADDR_MATCH_7 0x80120
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ADDR_MATCH_7 0x1DF80120u

//! Register Reset Value
#define TREG1_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG1_PM_REQ_INFO_PERMISSION_7 Register TREG1_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TREG1_PM_REQ_INFO_PERMISSION_7 0x80128
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_REQ_INFO_PERMISSION_7 0x1DF80128u

//! Register Reset Value
#define TREG1_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_READ_PERMISSION_7 Register TREG1_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TREG1_PM_READ_PERMISSION_7 0x80130
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_READ_PERMISSION_7 0x1DF80130u

//! Register Reset Value
#define TREG1_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_WRITE_PERMISSION_7 Register TREG1_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TREG1_PM_WRITE_PERMISSION_7 0x80138
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_WRITE_PERMISSION_7 0x1DF80138u

//! Register Reset Value
#define TREG1_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_ADDR_MATCH_8 Register TREG1_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TREG1_PM_ADDR_MATCH_8 0x80140
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ADDR_MATCH_8 0x1DF80140u

//! Register Reset Value
#define TREG1_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG1_PM_REQ_INFO_PERMISSION_8 Register TREG1_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TREG1_PM_REQ_INFO_PERMISSION_8 0x80148
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_REQ_INFO_PERMISSION_8 0x1DF80148u

//! Register Reset Value
#define TREG1_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_READ_PERMISSION_8 Register TREG1_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TREG1_PM_READ_PERMISSION_8 0x80150
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_READ_PERMISSION_8 0x1DF80150u

//! Register Reset Value
#define TREG1_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_WRITE_PERMISSION_8 Register TREG1_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TREG1_PM_WRITE_PERMISSION_8 0x80158
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_WRITE_PERMISSION_8 0x1DF80158u

//! Register Reset Value
#define TREG1_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_ADDR_MATCH_9 Register TREG1_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TREG1_PM_ADDR_MATCH_9 0x80160
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ADDR_MATCH_9 0x1DF80160u

//! Register Reset Value
#define TREG1_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG1_PM_REQ_INFO_PERMISSION_9 Register TREG1_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TREG1_PM_REQ_INFO_PERMISSION_9 0x80168
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_REQ_INFO_PERMISSION_9 0x1DF80168u

//! Register Reset Value
#define TREG1_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_READ_PERMISSION_9 Register TREG1_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TREG1_PM_READ_PERMISSION_9 0x80170
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_READ_PERMISSION_9 0x1DF80170u

//! Register Reset Value
#define TREG1_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_WRITE_PERMISSION_9 Register TREG1_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TREG1_PM_WRITE_PERMISSION_9 0x80178
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_WRITE_PERMISSION_9 0x1DF80178u

//! Register Reset Value
#define TREG1_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_ADDR_MATCH_10 Register TREG1_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TREG1_PM_ADDR_MATCH_10 0x80180
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ADDR_MATCH_10 0x1DF80180u

//! Register Reset Value
#define TREG1_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG1_PM_REQ_INFO_PERMISSION_10 Register TREG1_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TREG1_PM_REQ_INFO_PERMISSION_10 0x80188
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_REQ_INFO_PERMISSION_10 0x1DF80188u

//! Register Reset Value
#define TREG1_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_READ_PERMISSION_10 Register TREG1_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TREG1_PM_READ_PERMISSION_10 0x80190
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_READ_PERMISSION_10 0x1DF80190u

//! Register Reset Value
#define TREG1_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_WRITE_PERMISSION_10 Register TREG1_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TREG1_PM_WRITE_PERMISSION_10 0x80198
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_WRITE_PERMISSION_10 0x1DF80198u

//! Register Reset Value
#define TREG1_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_ADDR_MATCH_11 Register TREG1_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TREG1_PM_ADDR_MATCH_11 0x801A0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ADDR_MATCH_11 0x1DF801A0u

//! Register Reset Value
#define TREG1_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG1_PM_REQ_INFO_PERMISSION_11 Register TREG1_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TREG1_PM_REQ_INFO_PERMISSION_11 0x801A8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_REQ_INFO_PERMISSION_11 0x1DF801A8u

//! Register Reset Value
#define TREG1_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_READ_PERMISSION_11 Register TREG1_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TREG1_PM_READ_PERMISSION_11 0x801B0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_READ_PERMISSION_11 0x1DF801B0u

//! Register Reset Value
#define TREG1_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_WRITE_PERMISSION_11 Register TREG1_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TREG1_PM_WRITE_PERMISSION_11 0x801B8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_WRITE_PERMISSION_11 0x1DF801B8u

//! Register Reset Value
#define TREG1_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_ADDR_MATCH_12 Register TREG1_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TREG1_PM_ADDR_MATCH_12 0x801C0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ADDR_MATCH_12 0x1DF801C0u

//! Register Reset Value
#define TREG1_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG1_PM_REQ_INFO_PERMISSION_12 Register TREG1_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TREG1_PM_REQ_INFO_PERMISSION_12 0x801C8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_REQ_INFO_PERMISSION_12 0x1DF801C8u

//! Register Reset Value
#define TREG1_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_READ_PERMISSION_12 Register TREG1_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TREG1_PM_READ_PERMISSION_12 0x801D0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_READ_PERMISSION_12 0x1DF801D0u

//! Register Reset Value
#define TREG1_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_WRITE_PERMISSION_12 Register TREG1_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TREG1_PM_WRITE_PERMISSION_12 0x801D8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_WRITE_PERMISSION_12 0x1DF801D8u

//! Register Reset Value
#define TREG1_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_ADDR_MATCH_13 Register TREG1_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TREG1_PM_ADDR_MATCH_13 0x801E0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ADDR_MATCH_13 0x1DF801E0u

//! Register Reset Value
#define TREG1_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG1_PM_REQ_INFO_PERMISSION_13 Register TREG1_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TREG1_PM_REQ_INFO_PERMISSION_13 0x801E8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_REQ_INFO_PERMISSION_13 0x1DF801E8u

//! Register Reset Value
#define TREG1_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_READ_PERMISSION_13 Register TREG1_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TREG1_PM_READ_PERMISSION_13 0x801F0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_READ_PERMISSION_13 0x1DF801F0u

//! Register Reset Value
#define TREG1_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_WRITE_PERMISSION_13 Register TREG1_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TREG1_PM_WRITE_PERMISSION_13 0x801F8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_WRITE_PERMISSION_13 0x1DF801F8u

//! Register Reset Value
#define TREG1_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_ADDR_MATCH_14 Register TREG1_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TREG1_PM_ADDR_MATCH_14 0x80200
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ADDR_MATCH_14 0x1DF80200u

//! Register Reset Value
#define TREG1_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG1_PM_REQ_INFO_PERMISSION_14 Register TREG1_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TREG1_PM_REQ_INFO_PERMISSION_14 0x80208
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_REQ_INFO_PERMISSION_14 0x1DF80208u

//! Register Reset Value
#define TREG1_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_READ_PERMISSION_14 Register TREG1_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TREG1_PM_READ_PERMISSION_14 0x80210
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_READ_PERMISSION_14 0x1DF80210u

//! Register Reset Value
#define TREG1_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_WRITE_PERMISSION_14 Register TREG1_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TREG1_PM_WRITE_PERMISSION_14 0x80218
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_WRITE_PERMISSION_14 0x1DF80218u

//! Register Reset Value
#define TREG1_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_ADDR_MATCH_15 Register TREG1_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TREG1_PM_ADDR_MATCH_15 0x80220
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_ADDR_MATCH_15 0x1DF80220u

//! Register Reset Value
#define TREG1_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG1_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TREG1_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TREG1_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG1_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG1_PM_REQ_INFO_PERMISSION_15 Register TREG1_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TREG1_PM_REQ_INFO_PERMISSION_15 0x80228
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_REQ_INFO_PERMISSION_15 0x1DF80228u

//! Register Reset Value
#define TREG1_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG1_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_READ_PERMISSION_15 Register TREG1_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TREG1_PM_READ_PERMISSION_15 0x80230
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_READ_PERMISSION_15 0x1DF80230u

//! Register Reset Value
#define TREG1_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG1_PM_WRITE_PERMISSION_15 Register TREG1_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TREG1_PM_WRITE_PERMISSION_15 0x80238
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TREG1_PM_WRITE_PERMISSION_15 0x1DF80238u

//! Register Reset Value
#define TREG1_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG1_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_ERROR_LOG Register TDM2R_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ERROR_LOG 0x80420
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ERROR_LOG 0x1DF80420u

//! Register Reset Value
#define TDM2R_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TDM2R_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TDM2R_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TDM2R_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TDM2R_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TDM2R_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TDM2R_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TDM2R_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TDM2R_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TDM2R_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TDM2R_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TDM2R_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TDM2R_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TDM2R_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TDM2R_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TDM2R_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TDM2R_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TDM2R_PM_CONTROL Register TDM2R_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TDM2R_PM_CONTROL 0x80428
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_CONTROL 0x1DF80428u

//! Register Reset Value
#define TDM2R_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TDM2R_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TDM2R_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TDM2R_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TDM2R_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TDM2R_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TDM2R_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TDM2R_PM_ERROR_CLEAR_SINGLE Register TDM2R_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ERROR_CLEAR_SINGLE 0x80430
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ERROR_CLEAR_SINGLE 0x1DF80430u

//! Register Reset Value
#define TDM2R_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TDM2R_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TDM2R_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TDM2R_PM_ERROR_CLEAR_MULTI Register TDM2R_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ERROR_CLEAR_MULTI 0x80438
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ERROR_CLEAR_MULTI 0x1DF80438u

//! Register Reset Value
#define TDM2R_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TDM2R_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TDM2R_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TDM2R_PM_REQ_INFO_PERMISSION_0 Register TDM2R_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TDM2R_PM_REQ_INFO_PERMISSION_0 0x80448
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_REQ_INFO_PERMISSION_0 0x1DF80448u

//! Register Reset Value
#define TDM2R_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_READ_PERMISSION_0 Register TDM2R_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TDM2R_PM_READ_PERMISSION_0 0x80450
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_READ_PERMISSION_0 0x1DF80450u

//! Register Reset Value
#define TDM2R_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TDM2R_PM_WRITE_PERMISSION_0 Register TDM2R_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TDM2R_PM_WRITE_PERMISSION_0 0x80458
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_WRITE_PERMISSION_0 0x1DF80458u

//! Register Reset Value
#define TDM2R_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TDM2R_PM_ADDR_MATCH_1 Register TDM2R_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ADDR_MATCH_1 0x80460
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ADDR_MATCH_1 0x1DF80460u

//! Register Reset Value
#define TDM2R_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2R_PM_REQ_INFO_PERMISSION_1 Register TDM2R_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TDM2R_PM_REQ_INFO_PERMISSION_1 0x80468
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_REQ_INFO_PERMISSION_1 0x1DF80468u

//! Register Reset Value
#define TDM2R_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_READ_PERMISSION_1 Register TDM2R_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TDM2R_PM_READ_PERMISSION_1 0x80470
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_READ_PERMISSION_1 0x1DF80470u

//! Register Reset Value
#define TDM2R_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_WRITE_PERMISSION_1 Register TDM2R_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TDM2R_PM_WRITE_PERMISSION_1 0x80478
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_WRITE_PERMISSION_1 0x1DF80478u

//! Register Reset Value
#define TDM2R_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_ADDR_MATCH_2 Register TDM2R_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ADDR_MATCH_2 0x80480
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ADDR_MATCH_2 0x1DF80480u

//! Register Reset Value
#define TDM2R_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2R_PM_REQ_INFO_PERMISSION_2 Register TDM2R_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TDM2R_PM_REQ_INFO_PERMISSION_2 0x80488
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_REQ_INFO_PERMISSION_2 0x1DF80488u

//! Register Reset Value
#define TDM2R_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_READ_PERMISSION_2 Register TDM2R_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TDM2R_PM_READ_PERMISSION_2 0x80490
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_READ_PERMISSION_2 0x1DF80490u

//! Register Reset Value
#define TDM2R_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_WRITE_PERMISSION_2 Register TDM2R_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TDM2R_PM_WRITE_PERMISSION_2 0x80498
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_WRITE_PERMISSION_2 0x1DF80498u

//! Register Reset Value
#define TDM2R_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_ADDR_MATCH_3 Register TDM2R_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ADDR_MATCH_3 0x804A0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ADDR_MATCH_3 0x1DF804A0u

//! Register Reset Value
#define TDM2R_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2R_PM_REQ_INFO_PERMISSION_3 Register TDM2R_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TDM2R_PM_REQ_INFO_PERMISSION_3 0x804A8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_REQ_INFO_PERMISSION_3 0x1DF804A8u

//! Register Reset Value
#define TDM2R_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_READ_PERMISSION_3 Register TDM2R_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TDM2R_PM_READ_PERMISSION_3 0x804B0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_READ_PERMISSION_3 0x1DF804B0u

//! Register Reset Value
#define TDM2R_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_WRITE_PERMISSION_3 Register TDM2R_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TDM2R_PM_WRITE_PERMISSION_3 0x804B8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_WRITE_PERMISSION_3 0x1DF804B8u

//! Register Reset Value
#define TDM2R_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_ADDR_MATCH_4 Register TDM2R_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ADDR_MATCH_4 0x804C0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ADDR_MATCH_4 0x1DF804C0u

//! Register Reset Value
#define TDM2R_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2R_PM_REQ_INFO_PERMISSION_4 Register TDM2R_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TDM2R_PM_REQ_INFO_PERMISSION_4 0x804C8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_REQ_INFO_PERMISSION_4 0x1DF804C8u

//! Register Reset Value
#define TDM2R_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_READ_PERMISSION_4 Register TDM2R_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TDM2R_PM_READ_PERMISSION_4 0x804D0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_READ_PERMISSION_4 0x1DF804D0u

//! Register Reset Value
#define TDM2R_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_WRITE_PERMISSION_4 Register TDM2R_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TDM2R_PM_WRITE_PERMISSION_4 0x804D8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_WRITE_PERMISSION_4 0x1DF804D8u

//! Register Reset Value
#define TDM2R_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_ADDR_MATCH_5 Register TDM2R_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ADDR_MATCH_5 0x804E0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ADDR_MATCH_5 0x1DF804E0u

//! Register Reset Value
#define TDM2R_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2R_PM_REQ_INFO_PERMISSION_5 Register TDM2R_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TDM2R_PM_REQ_INFO_PERMISSION_5 0x804E8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_REQ_INFO_PERMISSION_5 0x1DF804E8u

//! Register Reset Value
#define TDM2R_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_READ_PERMISSION_5 Register TDM2R_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TDM2R_PM_READ_PERMISSION_5 0x804F0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_READ_PERMISSION_5 0x1DF804F0u

//! Register Reset Value
#define TDM2R_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_WRITE_PERMISSION_5 Register TDM2R_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TDM2R_PM_WRITE_PERMISSION_5 0x804F8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_WRITE_PERMISSION_5 0x1DF804F8u

//! Register Reset Value
#define TDM2R_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_ADDR_MATCH_6 Register TDM2R_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ADDR_MATCH_6 0x80500
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ADDR_MATCH_6 0x1DF80500u

//! Register Reset Value
#define TDM2R_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2R_PM_REQ_INFO_PERMISSION_6 Register TDM2R_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TDM2R_PM_REQ_INFO_PERMISSION_6 0x80508
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_REQ_INFO_PERMISSION_6 0x1DF80508u

//! Register Reset Value
#define TDM2R_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_READ_PERMISSION_6 Register TDM2R_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TDM2R_PM_READ_PERMISSION_6 0x80510
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_READ_PERMISSION_6 0x1DF80510u

//! Register Reset Value
#define TDM2R_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_WRITE_PERMISSION_6 Register TDM2R_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TDM2R_PM_WRITE_PERMISSION_6 0x80518
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_WRITE_PERMISSION_6 0x1DF80518u

//! Register Reset Value
#define TDM2R_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_ADDR_MATCH_7 Register TDM2R_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ADDR_MATCH_7 0x80520
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ADDR_MATCH_7 0x1DF80520u

//! Register Reset Value
#define TDM2R_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2R_PM_REQ_INFO_PERMISSION_7 Register TDM2R_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TDM2R_PM_REQ_INFO_PERMISSION_7 0x80528
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_REQ_INFO_PERMISSION_7 0x1DF80528u

//! Register Reset Value
#define TDM2R_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_READ_PERMISSION_7 Register TDM2R_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TDM2R_PM_READ_PERMISSION_7 0x80530
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_READ_PERMISSION_7 0x1DF80530u

//! Register Reset Value
#define TDM2R_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_WRITE_PERMISSION_7 Register TDM2R_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TDM2R_PM_WRITE_PERMISSION_7 0x80538
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_WRITE_PERMISSION_7 0x1DF80538u

//! Register Reset Value
#define TDM2R_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_ADDR_MATCH_8 Register TDM2R_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ADDR_MATCH_8 0x80540
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ADDR_MATCH_8 0x1DF80540u

//! Register Reset Value
#define TDM2R_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2R_PM_REQ_INFO_PERMISSION_8 Register TDM2R_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TDM2R_PM_REQ_INFO_PERMISSION_8 0x80548
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_REQ_INFO_PERMISSION_8 0x1DF80548u

//! Register Reset Value
#define TDM2R_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_READ_PERMISSION_8 Register TDM2R_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TDM2R_PM_READ_PERMISSION_8 0x80550
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_READ_PERMISSION_8 0x1DF80550u

//! Register Reset Value
#define TDM2R_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_WRITE_PERMISSION_8 Register TDM2R_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TDM2R_PM_WRITE_PERMISSION_8 0x80558
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_WRITE_PERMISSION_8 0x1DF80558u

//! Register Reset Value
#define TDM2R_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_ADDR_MATCH_9 Register TDM2R_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ADDR_MATCH_9 0x80560
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ADDR_MATCH_9 0x1DF80560u

//! Register Reset Value
#define TDM2R_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2R_PM_REQ_INFO_PERMISSION_9 Register TDM2R_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TDM2R_PM_REQ_INFO_PERMISSION_9 0x80568
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_REQ_INFO_PERMISSION_9 0x1DF80568u

//! Register Reset Value
#define TDM2R_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_READ_PERMISSION_9 Register TDM2R_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TDM2R_PM_READ_PERMISSION_9 0x80570
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_READ_PERMISSION_9 0x1DF80570u

//! Register Reset Value
#define TDM2R_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_WRITE_PERMISSION_9 Register TDM2R_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TDM2R_PM_WRITE_PERMISSION_9 0x80578
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_WRITE_PERMISSION_9 0x1DF80578u

//! Register Reset Value
#define TDM2R_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_ADDR_MATCH_10 Register TDM2R_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ADDR_MATCH_10 0x80580
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ADDR_MATCH_10 0x1DF80580u

//! Register Reset Value
#define TDM2R_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2R_PM_REQ_INFO_PERMISSION_10 Register TDM2R_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TDM2R_PM_REQ_INFO_PERMISSION_10 0x80588
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_REQ_INFO_PERMISSION_10 0x1DF80588u

//! Register Reset Value
#define TDM2R_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_READ_PERMISSION_10 Register TDM2R_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TDM2R_PM_READ_PERMISSION_10 0x80590
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_READ_PERMISSION_10 0x1DF80590u

//! Register Reset Value
#define TDM2R_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_WRITE_PERMISSION_10 Register TDM2R_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TDM2R_PM_WRITE_PERMISSION_10 0x80598
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_WRITE_PERMISSION_10 0x1DF80598u

//! Register Reset Value
#define TDM2R_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_ADDR_MATCH_11 Register TDM2R_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ADDR_MATCH_11 0x805A0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ADDR_MATCH_11 0x1DF805A0u

//! Register Reset Value
#define TDM2R_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2R_PM_REQ_INFO_PERMISSION_11 Register TDM2R_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TDM2R_PM_REQ_INFO_PERMISSION_11 0x805A8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_REQ_INFO_PERMISSION_11 0x1DF805A8u

//! Register Reset Value
#define TDM2R_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_READ_PERMISSION_11 Register TDM2R_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TDM2R_PM_READ_PERMISSION_11 0x805B0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_READ_PERMISSION_11 0x1DF805B0u

//! Register Reset Value
#define TDM2R_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_WRITE_PERMISSION_11 Register TDM2R_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TDM2R_PM_WRITE_PERMISSION_11 0x805B8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_WRITE_PERMISSION_11 0x1DF805B8u

//! Register Reset Value
#define TDM2R_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_ADDR_MATCH_12 Register TDM2R_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ADDR_MATCH_12 0x805C0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ADDR_MATCH_12 0x1DF805C0u

//! Register Reset Value
#define TDM2R_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2R_PM_REQ_INFO_PERMISSION_12 Register TDM2R_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TDM2R_PM_REQ_INFO_PERMISSION_12 0x805C8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_REQ_INFO_PERMISSION_12 0x1DF805C8u

//! Register Reset Value
#define TDM2R_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_READ_PERMISSION_12 Register TDM2R_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TDM2R_PM_READ_PERMISSION_12 0x805D0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_READ_PERMISSION_12 0x1DF805D0u

//! Register Reset Value
#define TDM2R_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_WRITE_PERMISSION_12 Register TDM2R_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TDM2R_PM_WRITE_PERMISSION_12 0x805D8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_WRITE_PERMISSION_12 0x1DF805D8u

//! Register Reset Value
#define TDM2R_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_ADDR_MATCH_13 Register TDM2R_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ADDR_MATCH_13 0x805E0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ADDR_MATCH_13 0x1DF805E0u

//! Register Reset Value
#define TDM2R_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2R_PM_REQ_INFO_PERMISSION_13 Register TDM2R_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TDM2R_PM_REQ_INFO_PERMISSION_13 0x805E8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_REQ_INFO_PERMISSION_13 0x1DF805E8u

//! Register Reset Value
#define TDM2R_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_READ_PERMISSION_13 Register TDM2R_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TDM2R_PM_READ_PERMISSION_13 0x805F0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_READ_PERMISSION_13 0x1DF805F0u

//! Register Reset Value
#define TDM2R_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_WRITE_PERMISSION_13 Register TDM2R_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TDM2R_PM_WRITE_PERMISSION_13 0x805F8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_WRITE_PERMISSION_13 0x1DF805F8u

//! Register Reset Value
#define TDM2R_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_ADDR_MATCH_14 Register TDM2R_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ADDR_MATCH_14 0x80600
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ADDR_MATCH_14 0x1DF80600u

//! Register Reset Value
#define TDM2R_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2R_PM_REQ_INFO_PERMISSION_14 Register TDM2R_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TDM2R_PM_REQ_INFO_PERMISSION_14 0x80608
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_REQ_INFO_PERMISSION_14 0x1DF80608u

//! Register Reset Value
#define TDM2R_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_READ_PERMISSION_14 Register TDM2R_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TDM2R_PM_READ_PERMISSION_14 0x80610
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_READ_PERMISSION_14 0x1DF80610u

//! Register Reset Value
#define TDM2R_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_WRITE_PERMISSION_14 Register TDM2R_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TDM2R_PM_WRITE_PERMISSION_14 0x80618
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_WRITE_PERMISSION_14 0x1DF80618u

//! Register Reset Value
#define TDM2R_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_ADDR_MATCH_15 Register TDM2R_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TDM2R_PM_ADDR_MATCH_15 0x80620
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_ADDR_MATCH_15 0x1DF80620u

//! Register Reset Value
#define TDM2R_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2R_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TDM2R_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2R_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2R_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2R_PM_REQ_INFO_PERMISSION_15 Register TDM2R_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TDM2R_PM_REQ_INFO_PERMISSION_15 0x80628
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_REQ_INFO_PERMISSION_15 0x1DF80628u

//! Register Reset Value
#define TDM2R_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2R_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_READ_PERMISSION_15 Register TDM2R_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TDM2R_PM_READ_PERMISSION_15 0x80630
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_READ_PERMISSION_15 0x1DF80630u

//! Register Reset Value
#define TDM2R_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2R_PM_WRITE_PERMISSION_15 Register TDM2R_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TDM2R_PM_WRITE_PERMISSION_15 0x80638
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2R_PM_WRITE_PERMISSION_15 0x1DF80638u

//! Register Reset Value
#define TDM2R_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2R_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_ERROR_LOG Register TDM2T_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ERROR_LOG 0x80820
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ERROR_LOG 0x1DF80820u

//! Register Reset Value
#define TDM2T_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TDM2T_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TDM2T_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TDM2T_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TDM2T_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TDM2T_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TDM2T_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TDM2T_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TDM2T_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TDM2T_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TDM2T_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TDM2T_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TDM2T_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TDM2T_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TDM2T_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TDM2T_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TDM2T_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TDM2T_PM_CONTROL Register TDM2T_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TDM2T_PM_CONTROL 0x80828
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_CONTROL 0x1DF80828u

//! Register Reset Value
#define TDM2T_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TDM2T_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TDM2T_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TDM2T_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TDM2T_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TDM2T_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TDM2T_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TDM2T_PM_ERROR_CLEAR_SINGLE Register TDM2T_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ERROR_CLEAR_SINGLE 0x80830
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ERROR_CLEAR_SINGLE 0x1DF80830u

//! Register Reset Value
#define TDM2T_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TDM2T_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TDM2T_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TDM2T_PM_ERROR_CLEAR_MULTI Register TDM2T_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ERROR_CLEAR_MULTI 0x80838
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ERROR_CLEAR_MULTI 0x1DF80838u

//! Register Reset Value
#define TDM2T_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TDM2T_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TDM2T_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TDM2T_PM_REQ_INFO_PERMISSION_0 Register TDM2T_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TDM2T_PM_REQ_INFO_PERMISSION_0 0x80848
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_REQ_INFO_PERMISSION_0 0x1DF80848u

//! Register Reset Value
#define TDM2T_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_READ_PERMISSION_0 Register TDM2T_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TDM2T_PM_READ_PERMISSION_0 0x80850
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_READ_PERMISSION_0 0x1DF80850u

//! Register Reset Value
#define TDM2T_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TDM2T_PM_WRITE_PERMISSION_0 Register TDM2T_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TDM2T_PM_WRITE_PERMISSION_0 0x80858
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_WRITE_PERMISSION_0 0x1DF80858u

//! Register Reset Value
#define TDM2T_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TDM2T_PM_ADDR_MATCH_1 Register TDM2T_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ADDR_MATCH_1 0x80860
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ADDR_MATCH_1 0x1DF80860u

//! Register Reset Value
#define TDM2T_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2T_PM_REQ_INFO_PERMISSION_1 Register TDM2T_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TDM2T_PM_REQ_INFO_PERMISSION_1 0x80868
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_REQ_INFO_PERMISSION_1 0x1DF80868u

//! Register Reset Value
#define TDM2T_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_READ_PERMISSION_1 Register TDM2T_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TDM2T_PM_READ_PERMISSION_1 0x80870
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_READ_PERMISSION_1 0x1DF80870u

//! Register Reset Value
#define TDM2T_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_WRITE_PERMISSION_1 Register TDM2T_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TDM2T_PM_WRITE_PERMISSION_1 0x80878
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_WRITE_PERMISSION_1 0x1DF80878u

//! Register Reset Value
#define TDM2T_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_ADDR_MATCH_2 Register TDM2T_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ADDR_MATCH_2 0x80880
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ADDR_MATCH_2 0x1DF80880u

//! Register Reset Value
#define TDM2T_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2T_PM_REQ_INFO_PERMISSION_2 Register TDM2T_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TDM2T_PM_REQ_INFO_PERMISSION_2 0x80888
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_REQ_INFO_PERMISSION_2 0x1DF80888u

//! Register Reset Value
#define TDM2T_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_READ_PERMISSION_2 Register TDM2T_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TDM2T_PM_READ_PERMISSION_2 0x80890
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_READ_PERMISSION_2 0x1DF80890u

//! Register Reset Value
#define TDM2T_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_WRITE_PERMISSION_2 Register TDM2T_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TDM2T_PM_WRITE_PERMISSION_2 0x80898
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_WRITE_PERMISSION_2 0x1DF80898u

//! Register Reset Value
#define TDM2T_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_ADDR_MATCH_3 Register TDM2T_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ADDR_MATCH_3 0x808A0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ADDR_MATCH_3 0x1DF808A0u

//! Register Reset Value
#define TDM2T_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2T_PM_REQ_INFO_PERMISSION_3 Register TDM2T_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TDM2T_PM_REQ_INFO_PERMISSION_3 0x808A8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_REQ_INFO_PERMISSION_3 0x1DF808A8u

//! Register Reset Value
#define TDM2T_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_READ_PERMISSION_3 Register TDM2T_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TDM2T_PM_READ_PERMISSION_3 0x808B0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_READ_PERMISSION_3 0x1DF808B0u

//! Register Reset Value
#define TDM2T_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_WRITE_PERMISSION_3 Register TDM2T_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TDM2T_PM_WRITE_PERMISSION_3 0x808B8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_WRITE_PERMISSION_3 0x1DF808B8u

//! Register Reset Value
#define TDM2T_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_ADDR_MATCH_4 Register TDM2T_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ADDR_MATCH_4 0x808C0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ADDR_MATCH_4 0x1DF808C0u

//! Register Reset Value
#define TDM2T_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2T_PM_REQ_INFO_PERMISSION_4 Register TDM2T_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TDM2T_PM_REQ_INFO_PERMISSION_4 0x808C8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_REQ_INFO_PERMISSION_4 0x1DF808C8u

//! Register Reset Value
#define TDM2T_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_READ_PERMISSION_4 Register TDM2T_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TDM2T_PM_READ_PERMISSION_4 0x808D0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_READ_PERMISSION_4 0x1DF808D0u

//! Register Reset Value
#define TDM2T_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_WRITE_PERMISSION_4 Register TDM2T_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TDM2T_PM_WRITE_PERMISSION_4 0x808D8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_WRITE_PERMISSION_4 0x1DF808D8u

//! Register Reset Value
#define TDM2T_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_ADDR_MATCH_5 Register TDM2T_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ADDR_MATCH_5 0x808E0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ADDR_MATCH_5 0x1DF808E0u

//! Register Reset Value
#define TDM2T_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2T_PM_REQ_INFO_PERMISSION_5 Register TDM2T_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TDM2T_PM_REQ_INFO_PERMISSION_5 0x808E8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_REQ_INFO_PERMISSION_5 0x1DF808E8u

//! Register Reset Value
#define TDM2T_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_READ_PERMISSION_5 Register TDM2T_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TDM2T_PM_READ_PERMISSION_5 0x808F0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_READ_PERMISSION_5 0x1DF808F0u

//! Register Reset Value
#define TDM2T_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_WRITE_PERMISSION_5 Register TDM2T_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TDM2T_PM_WRITE_PERMISSION_5 0x808F8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_WRITE_PERMISSION_5 0x1DF808F8u

//! Register Reset Value
#define TDM2T_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_ADDR_MATCH_6 Register TDM2T_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ADDR_MATCH_6 0x80900
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ADDR_MATCH_6 0x1DF80900u

//! Register Reset Value
#define TDM2T_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2T_PM_REQ_INFO_PERMISSION_6 Register TDM2T_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TDM2T_PM_REQ_INFO_PERMISSION_6 0x80908
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_REQ_INFO_PERMISSION_6 0x1DF80908u

//! Register Reset Value
#define TDM2T_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_READ_PERMISSION_6 Register TDM2T_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TDM2T_PM_READ_PERMISSION_6 0x80910
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_READ_PERMISSION_6 0x1DF80910u

//! Register Reset Value
#define TDM2T_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_WRITE_PERMISSION_6 Register TDM2T_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TDM2T_PM_WRITE_PERMISSION_6 0x80918
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_WRITE_PERMISSION_6 0x1DF80918u

//! Register Reset Value
#define TDM2T_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_ADDR_MATCH_7 Register TDM2T_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ADDR_MATCH_7 0x80920
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ADDR_MATCH_7 0x1DF80920u

//! Register Reset Value
#define TDM2T_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2T_PM_REQ_INFO_PERMISSION_7 Register TDM2T_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TDM2T_PM_REQ_INFO_PERMISSION_7 0x80928
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_REQ_INFO_PERMISSION_7 0x1DF80928u

//! Register Reset Value
#define TDM2T_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_READ_PERMISSION_7 Register TDM2T_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TDM2T_PM_READ_PERMISSION_7 0x80930
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_READ_PERMISSION_7 0x1DF80930u

//! Register Reset Value
#define TDM2T_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_WRITE_PERMISSION_7 Register TDM2T_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TDM2T_PM_WRITE_PERMISSION_7 0x80938
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_WRITE_PERMISSION_7 0x1DF80938u

//! Register Reset Value
#define TDM2T_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_ADDR_MATCH_8 Register TDM2T_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ADDR_MATCH_8 0x80940
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ADDR_MATCH_8 0x1DF80940u

//! Register Reset Value
#define TDM2T_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2T_PM_REQ_INFO_PERMISSION_8 Register TDM2T_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TDM2T_PM_REQ_INFO_PERMISSION_8 0x80948
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_REQ_INFO_PERMISSION_8 0x1DF80948u

//! Register Reset Value
#define TDM2T_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_READ_PERMISSION_8 Register TDM2T_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TDM2T_PM_READ_PERMISSION_8 0x80950
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_READ_PERMISSION_8 0x1DF80950u

//! Register Reset Value
#define TDM2T_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_WRITE_PERMISSION_8 Register TDM2T_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TDM2T_PM_WRITE_PERMISSION_8 0x80958
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_WRITE_PERMISSION_8 0x1DF80958u

//! Register Reset Value
#define TDM2T_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_ADDR_MATCH_9 Register TDM2T_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ADDR_MATCH_9 0x80960
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ADDR_MATCH_9 0x1DF80960u

//! Register Reset Value
#define TDM2T_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2T_PM_REQ_INFO_PERMISSION_9 Register TDM2T_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TDM2T_PM_REQ_INFO_PERMISSION_9 0x80968
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_REQ_INFO_PERMISSION_9 0x1DF80968u

//! Register Reset Value
#define TDM2T_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_READ_PERMISSION_9 Register TDM2T_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TDM2T_PM_READ_PERMISSION_9 0x80970
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_READ_PERMISSION_9 0x1DF80970u

//! Register Reset Value
#define TDM2T_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_WRITE_PERMISSION_9 Register TDM2T_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TDM2T_PM_WRITE_PERMISSION_9 0x80978
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_WRITE_PERMISSION_9 0x1DF80978u

//! Register Reset Value
#define TDM2T_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_ADDR_MATCH_10 Register TDM2T_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ADDR_MATCH_10 0x80980
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ADDR_MATCH_10 0x1DF80980u

//! Register Reset Value
#define TDM2T_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2T_PM_REQ_INFO_PERMISSION_10 Register TDM2T_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TDM2T_PM_REQ_INFO_PERMISSION_10 0x80988
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_REQ_INFO_PERMISSION_10 0x1DF80988u

//! Register Reset Value
#define TDM2T_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_READ_PERMISSION_10 Register TDM2T_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TDM2T_PM_READ_PERMISSION_10 0x80990
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_READ_PERMISSION_10 0x1DF80990u

//! Register Reset Value
#define TDM2T_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_WRITE_PERMISSION_10 Register TDM2T_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TDM2T_PM_WRITE_PERMISSION_10 0x80998
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_WRITE_PERMISSION_10 0x1DF80998u

//! Register Reset Value
#define TDM2T_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_ADDR_MATCH_11 Register TDM2T_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ADDR_MATCH_11 0x809A0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ADDR_MATCH_11 0x1DF809A0u

//! Register Reset Value
#define TDM2T_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2T_PM_REQ_INFO_PERMISSION_11 Register TDM2T_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TDM2T_PM_REQ_INFO_PERMISSION_11 0x809A8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_REQ_INFO_PERMISSION_11 0x1DF809A8u

//! Register Reset Value
#define TDM2T_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_READ_PERMISSION_11 Register TDM2T_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TDM2T_PM_READ_PERMISSION_11 0x809B0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_READ_PERMISSION_11 0x1DF809B0u

//! Register Reset Value
#define TDM2T_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_WRITE_PERMISSION_11 Register TDM2T_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TDM2T_PM_WRITE_PERMISSION_11 0x809B8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_WRITE_PERMISSION_11 0x1DF809B8u

//! Register Reset Value
#define TDM2T_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_ADDR_MATCH_12 Register TDM2T_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ADDR_MATCH_12 0x809C0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ADDR_MATCH_12 0x1DF809C0u

//! Register Reset Value
#define TDM2T_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2T_PM_REQ_INFO_PERMISSION_12 Register TDM2T_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TDM2T_PM_REQ_INFO_PERMISSION_12 0x809C8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_REQ_INFO_PERMISSION_12 0x1DF809C8u

//! Register Reset Value
#define TDM2T_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_READ_PERMISSION_12 Register TDM2T_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TDM2T_PM_READ_PERMISSION_12 0x809D0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_READ_PERMISSION_12 0x1DF809D0u

//! Register Reset Value
#define TDM2T_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_WRITE_PERMISSION_12 Register TDM2T_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TDM2T_PM_WRITE_PERMISSION_12 0x809D8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_WRITE_PERMISSION_12 0x1DF809D8u

//! Register Reset Value
#define TDM2T_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_ADDR_MATCH_13 Register TDM2T_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ADDR_MATCH_13 0x809E0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ADDR_MATCH_13 0x1DF809E0u

//! Register Reset Value
#define TDM2T_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2T_PM_REQ_INFO_PERMISSION_13 Register TDM2T_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TDM2T_PM_REQ_INFO_PERMISSION_13 0x809E8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_REQ_INFO_PERMISSION_13 0x1DF809E8u

//! Register Reset Value
#define TDM2T_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_READ_PERMISSION_13 Register TDM2T_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TDM2T_PM_READ_PERMISSION_13 0x809F0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_READ_PERMISSION_13 0x1DF809F0u

//! Register Reset Value
#define TDM2T_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_WRITE_PERMISSION_13 Register TDM2T_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TDM2T_PM_WRITE_PERMISSION_13 0x809F8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_WRITE_PERMISSION_13 0x1DF809F8u

//! Register Reset Value
#define TDM2T_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_ADDR_MATCH_14 Register TDM2T_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ADDR_MATCH_14 0x80A00
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ADDR_MATCH_14 0x1DF80A00u

//! Register Reset Value
#define TDM2T_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2T_PM_REQ_INFO_PERMISSION_14 Register TDM2T_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TDM2T_PM_REQ_INFO_PERMISSION_14 0x80A08
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_REQ_INFO_PERMISSION_14 0x1DF80A08u

//! Register Reset Value
#define TDM2T_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_READ_PERMISSION_14 Register TDM2T_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TDM2T_PM_READ_PERMISSION_14 0x80A10
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_READ_PERMISSION_14 0x1DF80A10u

//! Register Reset Value
#define TDM2T_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_WRITE_PERMISSION_14 Register TDM2T_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TDM2T_PM_WRITE_PERMISSION_14 0x80A18
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_WRITE_PERMISSION_14 0x1DF80A18u

//! Register Reset Value
#define TDM2T_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_ADDR_MATCH_15 Register TDM2T_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TDM2T_PM_ADDR_MATCH_15 0x80A20
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_ADDR_MATCH_15 0x1DF80A20u

//! Register Reset Value
#define TDM2T_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM2T_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TDM2T_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TDM2T_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM2T_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM2T_PM_REQ_INFO_PERMISSION_15 Register TDM2T_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TDM2T_PM_REQ_INFO_PERMISSION_15 0x80A28
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_REQ_INFO_PERMISSION_15 0x1DF80A28u

//! Register Reset Value
#define TDM2T_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM2T_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_READ_PERMISSION_15 Register TDM2T_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TDM2T_PM_READ_PERMISSION_15 0x80A30
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_READ_PERMISSION_15 0x1DF80A30u

//! Register Reset Value
#define TDM2T_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM2T_PM_WRITE_PERMISSION_15 Register TDM2T_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TDM2T_PM_WRITE_PERMISSION_15 0x80A38
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TDM2T_PM_WRITE_PERMISSION_15 0x1DF80A38u

//! Register Reset Value
#define TDM2T_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM2T_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_ERROR_LOG Register TGWPL_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ERROR_LOG 0x80C20
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ERROR_LOG 0x1DF80C20u

//! Register Reset Value
#define TGWPL_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TGWPL_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TGWPL_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TGWPL_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TGWPL_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TGWPL_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TGWPL_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TGWPL_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TGWPL_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TGWPL_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TGWPL_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TGWPL_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TGWPL_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TGWPL_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TGWPL_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TGWPL_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TGWPL_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TGWPL_PM_CONTROL Register TGWPL_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TGWPL_PM_CONTROL 0x80C28
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_CONTROL 0x1DF80C28u

//! Register Reset Value
#define TGWPL_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TGWPL_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TGWPL_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TGWPL_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TGWPL_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TGWPL_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TGWPL_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TGWPL_PM_ERROR_CLEAR_SINGLE Register TGWPL_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ERROR_CLEAR_SINGLE 0x80C30
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ERROR_CLEAR_SINGLE 0x1DF80C30u

//! Register Reset Value
#define TGWPL_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TGWPL_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TGWPL_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TGWPL_PM_ERROR_CLEAR_MULTI Register TGWPL_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ERROR_CLEAR_MULTI 0x80C38
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ERROR_CLEAR_MULTI 0x1DF80C38u

//! Register Reset Value
#define TGWPL_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TGWPL_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TGWPL_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TGWPL_PM_REQ_INFO_PERMISSION_0 Register TGWPL_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TGWPL_PM_REQ_INFO_PERMISSION_0 0x80C48
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_REQ_INFO_PERMISSION_0 0x1DF80C48u

//! Register Reset Value
#define TGWPL_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_READ_PERMISSION_0 Register TGWPL_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TGWPL_PM_READ_PERMISSION_0 0x80C50
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_READ_PERMISSION_0 0x1DF80C50u

//! Register Reset Value
#define TGWPL_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TGWPL_PM_WRITE_PERMISSION_0 Register TGWPL_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TGWPL_PM_WRITE_PERMISSION_0 0x80C58
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_WRITE_PERMISSION_0 0x1DF80C58u

//! Register Reset Value
#define TGWPL_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TGWPL_PM_ADDR_MATCH_1 Register TGWPL_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ADDR_MATCH_1 0x80C60
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ADDR_MATCH_1 0x1DF80C60u

//! Register Reset Value
#define TGWPL_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPL_PM_REQ_INFO_PERMISSION_1 Register TGWPL_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TGWPL_PM_REQ_INFO_PERMISSION_1 0x80C68
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_REQ_INFO_PERMISSION_1 0x1DF80C68u

//! Register Reset Value
#define TGWPL_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_READ_PERMISSION_1 Register TGWPL_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TGWPL_PM_READ_PERMISSION_1 0x80C70
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_READ_PERMISSION_1 0x1DF80C70u

//! Register Reset Value
#define TGWPL_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_WRITE_PERMISSION_1 Register TGWPL_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TGWPL_PM_WRITE_PERMISSION_1 0x80C78
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_WRITE_PERMISSION_1 0x1DF80C78u

//! Register Reset Value
#define TGWPL_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_ADDR_MATCH_2 Register TGWPL_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ADDR_MATCH_2 0x80C80
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ADDR_MATCH_2 0x1DF80C80u

//! Register Reset Value
#define TGWPL_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPL_PM_REQ_INFO_PERMISSION_2 Register TGWPL_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TGWPL_PM_REQ_INFO_PERMISSION_2 0x80C88
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_REQ_INFO_PERMISSION_2 0x1DF80C88u

//! Register Reset Value
#define TGWPL_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_READ_PERMISSION_2 Register TGWPL_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TGWPL_PM_READ_PERMISSION_2 0x80C90
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_READ_PERMISSION_2 0x1DF80C90u

//! Register Reset Value
#define TGWPL_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_WRITE_PERMISSION_2 Register TGWPL_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TGWPL_PM_WRITE_PERMISSION_2 0x80C98
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_WRITE_PERMISSION_2 0x1DF80C98u

//! Register Reset Value
#define TGWPL_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_ADDR_MATCH_3 Register TGWPL_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ADDR_MATCH_3 0x80CA0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ADDR_MATCH_3 0x1DF80CA0u

//! Register Reset Value
#define TGWPL_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPL_PM_REQ_INFO_PERMISSION_3 Register TGWPL_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TGWPL_PM_REQ_INFO_PERMISSION_3 0x80CA8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_REQ_INFO_PERMISSION_3 0x1DF80CA8u

//! Register Reset Value
#define TGWPL_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_READ_PERMISSION_3 Register TGWPL_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TGWPL_PM_READ_PERMISSION_3 0x80CB0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_READ_PERMISSION_3 0x1DF80CB0u

//! Register Reset Value
#define TGWPL_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_WRITE_PERMISSION_3 Register TGWPL_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TGWPL_PM_WRITE_PERMISSION_3 0x80CB8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_WRITE_PERMISSION_3 0x1DF80CB8u

//! Register Reset Value
#define TGWPL_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_ADDR_MATCH_4 Register TGWPL_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ADDR_MATCH_4 0x80CC0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ADDR_MATCH_4 0x1DF80CC0u

//! Register Reset Value
#define TGWPL_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPL_PM_REQ_INFO_PERMISSION_4 Register TGWPL_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TGWPL_PM_REQ_INFO_PERMISSION_4 0x80CC8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_REQ_INFO_PERMISSION_4 0x1DF80CC8u

//! Register Reset Value
#define TGWPL_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_READ_PERMISSION_4 Register TGWPL_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TGWPL_PM_READ_PERMISSION_4 0x80CD0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_READ_PERMISSION_4 0x1DF80CD0u

//! Register Reset Value
#define TGWPL_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_WRITE_PERMISSION_4 Register TGWPL_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TGWPL_PM_WRITE_PERMISSION_4 0x80CD8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_WRITE_PERMISSION_4 0x1DF80CD8u

//! Register Reset Value
#define TGWPL_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_ADDR_MATCH_5 Register TGWPL_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ADDR_MATCH_5 0x80CE0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ADDR_MATCH_5 0x1DF80CE0u

//! Register Reset Value
#define TGWPL_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPL_PM_REQ_INFO_PERMISSION_5 Register TGWPL_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TGWPL_PM_REQ_INFO_PERMISSION_5 0x80CE8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_REQ_INFO_PERMISSION_5 0x1DF80CE8u

//! Register Reset Value
#define TGWPL_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_READ_PERMISSION_5 Register TGWPL_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TGWPL_PM_READ_PERMISSION_5 0x80CF0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_READ_PERMISSION_5 0x1DF80CF0u

//! Register Reset Value
#define TGWPL_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_WRITE_PERMISSION_5 Register TGWPL_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TGWPL_PM_WRITE_PERMISSION_5 0x80CF8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_WRITE_PERMISSION_5 0x1DF80CF8u

//! Register Reset Value
#define TGWPL_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_ADDR_MATCH_6 Register TGWPL_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ADDR_MATCH_6 0x80D00
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ADDR_MATCH_6 0x1DF80D00u

//! Register Reset Value
#define TGWPL_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPL_PM_REQ_INFO_PERMISSION_6 Register TGWPL_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TGWPL_PM_REQ_INFO_PERMISSION_6 0x80D08
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_REQ_INFO_PERMISSION_6 0x1DF80D08u

//! Register Reset Value
#define TGWPL_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_READ_PERMISSION_6 Register TGWPL_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TGWPL_PM_READ_PERMISSION_6 0x80D10
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_READ_PERMISSION_6 0x1DF80D10u

//! Register Reset Value
#define TGWPL_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_WRITE_PERMISSION_6 Register TGWPL_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TGWPL_PM_WRITE_PERMISSION_6 0x80D18
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_WRITE_PERMISSION_6 0x1DF80D18u

//! Register Reset Value
#define TGWPL_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_ADDR_MATCH_7 Register TGWPL_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ADDR_MATCH_7 0x80D20
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ADDR_MATCH_7 0x1DF80D20u

//! Register Reset Value
#define TGWPL_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPL_PM_REQ_INFO_PERMISSION_7 Register TGWPL_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TGWPL_PM_REQ_INFO_PERMISSION_7 0x80D28
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_REQ_INFO_PERMISSION_7 0x1DF80D28u

//! Register Reset Value
#define TGWPL_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_READ_PERMISSION_7 Register TGWPL_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TGWPL_PM_READ_PERMISSION_7 0x80D30
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_READ_PERMISSION_7 0x1DF80D30u

//! Register Reset Value
#define TGWPL_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_WRITE_PERMISSION_7 Register TGWPL_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TGWPL_PM_WRITE_PERMISSION_7 0x80D38
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_WRITE_PERMISSION_7 0x1DF80D38u

//! Register Reset Value
#define TGWPL_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_ADDR_MATCH_8 Register TGWPL_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ADDR_MATCH_8 0x80D40
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ADDR_MATCH_8 0x1DF80D40u

//! Register Reset Value
#define TGWPL_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPL_PM_REQ_INFO_PERMISSION_8 Register TGWPL_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TGWPL_PM_REQ_INFO_PERMISSION_8 0x80D48
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_REQ_INFO_PERMISSION_8 0x1DF80D48u

//! Register Reset Value
#define TGWPL_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_READ_PERMISSION_8 Register TGWPL_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TGWPL_PM_READ_PERMISSION_8 0x80D50
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_READ_PERMISSION_8 0x1DF80D50u

//! Register Reset Value
#define TGWPL_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_WRITE_PERMISSION_8 Register TGWPL_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TGWPL_PM_WRITE_PERMISSION_8 0x80D58
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_WRITE_PERMISSION_8 0x1DF80D58u

//! Register Reset Value
#define TGWPL_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_ADDR_MATCH_9 Register TGWPL_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ADDR_MATCH_9 0x80D60
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ADDR_MATCH_9 0x1DF80D60u

//! Register Reset Value
#define TGWPL_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPL_PM_REQ_INFO_PERMISSION_9 Register TGWPL_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TGWPL_PM_REQ_INFO_PERMISSION_9 0x80D68
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_REQ_INFO_PERMISSION_9 0x1DF80D68u

//! Register Reset Value
#define TGWPL_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_READ_PERMISSION_9 Register TGWPL_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TGWPL_PM_READ_PERMISSION_9 0x80D70
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_READ_PERMISSION_9 0x1DF80D70u

//! Register Reset Value
#define TGWPL_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_WRITE_PERMISSION_9 Register TGWPL_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TGWPL_PM_WRITE_PERMISSION_9 0x80D78
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_WRITE_PERMISSION_9 0x1DF80D78u

//! Register Reset Value
#define TGWPL_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_ADDR_MATCH_10 Register TGWPL_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ADDR_MATCH_10 0x80D80
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ADDR_MATCH_10 0x1DF80D80u

//! Register Reset Value
#define TGWPL_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPL_PM_REQ_INFO_PERMISSION_10 Register TGWPL_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TGWPL_PM_REQ_INFO_PERMISSION_10 0x80D88
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_REQ_INFO_PERMISSION_10 0x1DF80D88u

//! Register Reset Value
#define TGWPL_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_READ_PERMISSION_10 Register TGWPL_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TGWPL_PM_READ_PERMISSION_10 0x80D90
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_READ_PERMISSION_10 0x1DF80D90u

//! Register Reset Value
#define TGWPL_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_WRITE_PERMISSION_10 Register TGWPL_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TGWPL_PM_WRITE_PERMISSION_10 0x80D98
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_WRITE_PERMISSION_10 0x1DF80D98u

//! Register Reset Value
#define TGWPL_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_ADDR_MATCH_11 Register TGWPL_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ADDR_MATCH_11 0x80DA0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ADDR_MATCH_11 0x1DF80DA0u

//! Register Reset Value
#define TGWPL_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPL_PM_REQ_INFO_PERMISSION_11 Register TGWPL_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TGWPL_PM_REQ_INFO_PERMISSION_11 0x80DA8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_REQ_INFO_PERMISSION_11 0x1DF80DA8u

//! Register Reset Value
#define TGWPL_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_READ_PERMISSION_11 Register TGWPL_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TGWPL_PM_READ_PERMISSION_11 0x80DB0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_READ_PERMISSION_11 0x1DF80DB0u

//! Register Reset Value
#define TGWPL_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_WRITE_PERMISSION_11 Register TGWPL_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TGWPL_PM_WRITE_PERMISSION_11 0x80DB8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_WRITE_PERMISSION_11 0x1DF80DB8u

//! Register Reset Value
#define TGWPL_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_ADDR_MATCH_12 Register TGWPL_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ADDR_MATCH_12 0x80DC0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ADDR_MATCH_12 0x1DF80DC0u

//! Register Reset Value
#define TGWPL_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPL_PM_REQ_INFO_PERMISSION_12 Register TGWPL_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TGWPL_PM_REQ_INFO_PERMISSION_12 0x80DC8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_REQ_INFO_PERMISSION_12 0x1DF80DC8u

//! Register Reset Value
#define TGWPL_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_READ_PERMISSION_12 Register TGWPL_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TGWPL_PM_READ_PERMISSION_12 0x80DD0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_READ_PERMISSION_12 0x1DF80DD0u

//! Register Reset Value
#define TGWPL_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_WRITE_PERMISSION_12 Register TGWPL_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TGWPL_PM_WRITE_PERMISSION_12 0x80DD8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_WRITE_PERMISSION_12 0x1DF80DD8u

//! Register Reset Value
#define TGWPL_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_ADDR_MATCH_13 Register TGWPL_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ADDR_MATCH_13 0x80DE0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ADDR_MATCH_13 0x1DF80DE0u

//! Register Reset Value
#define TGWPL_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPL_PM_REQ_INFO_PERMISSION_13 Register TGWPL_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TGWPL_PM_REQ_INFO_PERMISSION_13 0x80DE8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_REQ_INFO_PERMISSION_13 0x1DF80DE8u

//! Register Reset Value
#define TGWPL_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_READ_PERMISSION_13 Register TGWPL_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TGWPL_PM_READ_PERMISSION_13 0x80DF0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_READ_PERMISSION_13 0x1DF80DF0u

//! Register Reset Value
#define TGWPL_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_WRITE_PERMISSION_13 Register TGWPL_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TGWPL_PM_WRITE_PERMISSION_13 0x80DF8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_WRITE_PERMISSION_13 0x1DF80DF8u

//! Register Reset Value
#define TGWPL_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_ADDR_MATCH_14 Register TGWPL_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ADDR_MATCH_14 0x80E00
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ADDR_MATCH_14 0x1DF80E00u

//! Register Reset Value
#define TGWPL_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPL_PM_REQ_INFO_PERMISSION_14 Register TGWPL_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TGWPL_PM_REQ_INFO_PERMISSION_14 0x80E08
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_REQ_INFO_PERMISSION_14 0x1DF80E08u

//! Register Reset Value
#define TGWPL_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_READ_PERMISSION_14 Register TGWPL_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TGWPL_PM_READ_PERMISSION_14 0x80E10
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_READ_PERMISSION_14 0x1DF80E10u

//! Register Reset Value
#define TGWPL_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_WRITE_PERMISSION_14 Register TGWPL_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TGWPL_PM_WRITE_PERMISSION_14 0x80E18
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_WRITE_PERMISSION_14 0x1DF80E18u

//! Register Reset Value
#define TGWPL_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_ADDR_MATCH_15 Register TGWPL_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TGWPL_PM_ADDR_MATCH_15 0x80E20
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_ADDR_MATCH_15 0x1DF80E20u

//! Register Reset Value
#define TGWPL_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPL_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TGWPL_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPL_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPL_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPL_PM_REQ_INFO_PERMISSION_15 Register TGWPL_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TGWPL_PM_REQ_INFO_PERMISSION_15 0x80E28
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_REQ_INFO_PERMISSION_15 0x1DF80E28u

//! Register Reset Value
#define TGWPL_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPL_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_READ_PERMISSION_15 Register TGWPL_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TGWPL_PM_READ_PERMISSION_15 0x80E30
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_READ_PERMISSION_15 0x1DF80E30u

//! Register Reset Value
#define TGWPL_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPL_PM_WRITE_PERMISSION_15 Register TGWPL_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TGWPL_PM_WRITE_PERMISSION_15 0x80E38
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TGWPL_PM_WRITE_PERMISSION_15 0x1DF80E38u

//! Register Reset Value
#define TGWPL_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPL_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_ERROR_LOG Register TLN10_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TLN10_PM_ERROR_LOG 0x81020
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ERROR_LOG 0x1DF81020u

//! Register Reset Value
#define TLN10_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TLN10_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TLN10_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TLN10_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TLN10_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TLN10_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TLN10_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TLN10_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TLN10_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TLN10_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TLN10_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TLN10_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TLN10_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TLN10_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TLN10_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TLN10_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TLN10_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TLN10_PM_CONTROL Register TLN10_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TLN10_PM_CONTROL 0x81028
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_CONTROL 0x1DF81028u

//! Register Reset Value
#define TLN10_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TLN10_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TLN10_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TLN10_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TLN10_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TLN10_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TLN10_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TLN10_PM_ERROR_CLEAR_SINGLE Register TLN10_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TLN10_PM_ERROR_CLEAR_SINGLE 0x81030
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ERROR_CLEAR_SINGLE 0x1DF81030u

//! Register Reset Value
#define TLN10_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TLN10_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TLN10_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TLN10_PM_ERROR_CLEAR_MULTI Register TLN10_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TLN10_PM_ERROR_CLEAR_MULTI 0x81038
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ERROR_CLEAR_MULTI 0x1DF81038u

//! Register Reset Value
#define TLN10_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TLN10_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TLN10_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TLN10_PM_REQ_INFO_PERMISSION_0 Register TLN10_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TLN10_PM_REQ_INFO_PERMISSION_0 0x81048
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_REQ_INFO_PERMISSION_0 0x1DF81048u

//! Register Reset Value
#define TLN10_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_READ_PERMISSION_0 Register TLN10_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TLN10_PM_READ_PERMISSION_0 0x81050
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_READ_PERMISSION_0 0x1DF81050u

//! Register Reset Value
#define TLN10_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1u

//! @}

//! \defgroup TLN10_PM_WRITE_PERMISSION_0 Register TLN10_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TLN10_PM_WRITE_PERMISSION_0 0x81058
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_WRITE_PERMISSION_0 0x1DF81058u

//! Register Reset Value
#define TLN10_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1u

//! @}

//! \defgroup TLN10_PM_ADDR_MATCH_1 Register TLN10_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TLN10_PM_ADDR_MATCH_1 0x81060
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ADDR_MATCH_1 0x1DF81060u

//! Register Reset Value
#define TLN10_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN10_PM_REQ_INFO_PERMISSION_1 Register TLN10_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TLN10_PM_REQ_INFO_PERMISSION_1 0x81068
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_REQ_INFO_PERMISSION_1 0x1DF81068u

//! Register Reset Value
#define TLN10_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_READ_PERMISSION_1 Register TLN10_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TLN10_PM_READ_PERMISSION_1 0x81070
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_READ_PERMISSION_1 0x1DF81070u

//! Register Reset Value
#define TLN10_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_WRITE_PERMISSION_1 Register TLN10_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TLN10_PM_WRITE_PERMISSION_1 0x81078
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_WRITE_PERMISSION_1 0x1DF81078u

//! Register Reset Value
#define TLN10_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_ADDR_MATCH_2 Register TLN10_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TLN10_PM_ADDR_MATCH_2 0x81080
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ADDR_MATCH_2 0x1DF81080u

//! Register Reset Value
#define TLN10_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN10_PM_REQ_INFO_PERMISSION_2 Register TLN10_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TLN10_PM_REQ_INFO_PERMISSION_2 0x81088
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_REQ_INFO_PERMISSION_2 0x1DF81088u

//! Register Reset Value
#define TLN10_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_READ_PERMISSION_2 Register TLN10_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TLN10_PM_READ_PERMISSION_2 0x81090
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_READ_PERMISSION_2 0x1DF81090u

//! Register Reset Value
#define TLN10_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_WRITE_PERMISSION_2 Register TLN10_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TLN10_PM_WRITE_PERMISSION_2 0x81098
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_WRITE_PERMISSION_2 0x1DF81098u

//! Register Reset Value
#define TLN10_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_ADDR_MATCH_3 Register TLN10_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TLN10_PM_ADDR_MATCH_3 0x810A0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ADDR_MATCH_3 0x1DF810A0u

//! Register Reset Value
#define TLN10_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN10_PM_REQ_INFO_PERMISSION_3 Register TLN10_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TLN10_PM_REQ_INFO_PERMISSION_3 0x810A8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_REQ_INFO_PERMISSION_3 0x1DF810A8u

//! Register Reset Value
#define TLN10_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_READ_PERMISSION_3 Register TLN10_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TLN10_PM_READ_PERMISSION_3 0x810B0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_READ_PERMISSION_3 0x1DF810B0u

//! Register Reset Value
#define TLN10_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_WRITE_PERMISSION_3 Register TLN10_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TLN10_PM_WRITE_PERMISSION_3 0x810B8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_WRITE_PERMISSION_3 0x1DF810B8u

//! Register Reset Value
#define TLN10_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_ADDR_MATCH_4 Register TLN10_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TLN10_PM_ADDR_MATCH_4 0x810C0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ADDR_MATCH_4 0x1DF810C0u

//! Register Reset Value
#define TLN10_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN10_PM_REQ_INFO_PERMISSION_4 Register TLN10_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TLN10_PM_REQ_INFO_PERMISSION_4 0x810C8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_REQ_INFO_PERMISSION_4 0x1DF810C8u

//! Register Reset Value
#define TLN10_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_READ_PERMISSION_4 Register TLN10_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TLN10_PM_READ_PERMISSION_4 0x810D0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_READ_PERMISSION_4 0x1DF810D0u

//! Register Reset Value
#define TLN10_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_WRITE_PERMISSION_4 Register TLN10_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TLN10_PM_WRITE_PERMISSION_4 0x810D8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_WRITE_PERMISSION_4 0x1DF810D8u

//! Register Reset Value
#define TLN10_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_ADDR_MATCH_5 Register TLN10_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TLN10_PM_ADDR_MATCH_5 0x810E0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ADDR_MATCH_5 0x1DF810E0u

//! Register Reset Value
#define TLN10_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN10_PM_REQ_INFO_PERMISSION_5 Register TLN10_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TLN10_PM_REQ_INFO_PERMISSION_5 0x810E8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_REQ_INFO_PERMISSION_5 0x1DF810E8u

//! Register Reset Value
#define TLN10_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_READ_PERMISSION_5 Register TLN10_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TLN10_PM_READ_PERMISSION_5 0x810F0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_READ_PERMISSION_5 0x1DF810F0u

//! Register Reset Value
#define TLN10_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_WRITE_PERMISSION_5 Register TLN10_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TLN10_PM_WRITE_PERMISSION_5 0x810F8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_WRITE_PERMISSION_5 0x1DF810F8u

//! Register Reset Value
#define TLN10_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_ADDR_MATCH_6 Register TLN10_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TLN10_PM_ADDR_MATCH_6 0x81100
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ADDR_MATCH_6 0x1DF81100u

//! Register Reset Value
#define TLN10_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN10_PM_REQ_INFO_PERMISSION_6 Register TLN10_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TLN10_PM_REQ_INFO_PERMISSION_6 0x81108
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_REQ_INFO_PERMISSION_6 0x1DF81108u

//! Register Reset Value
#define TLN10_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_READ_PERMISSION_6 Register TLN10_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TLN10_PM_READ_PERMISSION_6 0x81110
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_READ_PERMISSION_6 0x1DF81110u

//! Register Reset Value
#define TLN10_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_WRITE_PERMISSION_6 Register TLN10_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TLN10_PM_WRITE_PERMISSION_6 0x81118
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_WRITE_PERMISSION_6 0x1DF81118u

//! Register Reset Value
#define TLN10_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_ADDR_MATCH_7 Register TLN10_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TLN10_PM_ADDR_MATCH_7 0x81120
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ADDR_MATCH_7 0x1DF81120u

//! Register Reset Value
#define TLN10_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN10_PM_REQ_INFO_PERMISSION_7 Register TLN10_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TLN10_PM_REQ_INFO_PERMISSION_7 0x81128
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_REQ_INFO_PERMISSION_7 0x1DF81128u

//! Register Reset Value
#define TLN10_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_READ_PERMISSION_7 Register TLN10_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TLN10_PM_READ_PERMISSION_7 0x81130
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_READ_PERMISSION_7 0x1DF81130u

//! Register Reset Value
#define TLN10_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_WRITE_PERMISSION_7 Register TLN10_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TLN10_PM_WRITE_PERMISSION_7 0x81138
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_WRITE_PERMISSION_7 0x1DF81138u

//! Register Reset Value
#define TLN10_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_ADDR_MATCH_8 Register TLN10_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TLN10_PM_ADDR_MATCH_8 0x81140
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ADDR_MATCH_8 0x1DF81140u

//! Register Reset Value
#define TLN10_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN10_PM_REQ_INFO_PERMISSION_8 Register TLN10_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TLN10_PM_REQ_INFO_PERMISSION_8 0x81148
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_REQ_INFO_PERMISSION_8 0x1DF81148u

//! Register Reset Value
#define TLN10_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_READ_PERMISSION_8 Register TLN10_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TLN10_PM_READ_PERMISSION_8 0x81150
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_READ_PERMISSION_8 0x1DF81150u

//! Register Reset Value
#define TLN10_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_WRITE_PERMISSION_8 Register TLN10_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TLN10_PM_WRITE_PERMISSION_8 0x81158
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_WRITE_PERMISSION_8 0x1DF81158u

//! Register Reset Value
#define TLN10_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_ADDR_MATCH_9 Register TLN10_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TLN10_PM_ADDR_MATCH_9 0x81160
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ADDR_MATCH_9 0x1DF81160u

//! Register Reset Value
#define TLN10_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN10_PM_REQ_INFO_PERMISSION_9 Register TLN10_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TLN10_PM_REQ_INFO_PERMISSION_9 0x81168
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_REQ_INFO_PERMISSION_9 0x1DF81168u

//! Register Reset Value
#define TLN10_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_READ_PERMISSION_9 Register TLN10_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TLN10_PM_READ_PERMISSION_9 0x81170
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_READ_PERMISSION_9 0x1DF81170u

//! Register Reset Value
#define TLN10_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_WRITE_PERMISSION_9 Register TLN10_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TLN10_PM_WRITE_PERMISSION_9 0x81178
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_WRITE_PERMISSION_9 0x1DF81178u

//! Register Reset Value
#define TLN10_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_ADDR_MATCH_10 Register TLN10_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TLN10_PM_ADDR_MATCH_10 0x81180
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ADDR_MATCH_10 0x1DF81180u

//! Register Reset Value
#define TLN10_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN10_PM_REQ_INFO_PERMISSION_10 Register TLN10_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TLN10_PM_REQ_INFO_PERMISSION_10 0x81188
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_REQ_INFO_PERMISSION_10 0x1DF81188u

//! Register Reset Value
#define TLN10_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_READ_PERMISSION_10 Register TLN10_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TLN10_PM_READ_PERMISSION_10 0x81190
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_READ_PERMISSION_10 0x1DF81190u

//! Register Reset Value
#define TLN10_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_WRITE_PERMISSION_10 Register TLN10_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TLN10_PM_WRITE_PERMISSION_10 0x81198
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_WRITE_PERMISSION_10 0x1DF81198u

//! Register Reset Value
#define TLN10_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_ADDR_MATCH_11 Register TLN10_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TLN10_PM_ADDR_MATCH_11 0x811A0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ADDR_MATCH_11 0x1DF811A0u

//! Register Reset Value
#define TLN10_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN10_PM_REQ_INFO_PERMISSION_11 Register TLN10_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TLN10_PM_REQ_INFO_PERMISSION_11 0x811A8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_REQ_INFO_PERMISSION_11 0x1DF811A8u

//! Register Reset Value
#define TLN10_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_READ_PERMISSION_11 Register TLN10_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TLN10_PM_READ_PERMISSION_11 0x811B0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_READ_PERMISSION_11 0x1DF811B0u

//! Register Reset Value
#define TLN10_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_WRITE_PERMISSION_11 Register TLN10_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TLN10_PM_WRITE_PERMISSION_11 0x811B8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_WRITE_PERMISSION_11 0x1DF811B8u

//! Register Reset Value
#define TLN10_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_ADDR_MATCH_12 Register TLN10_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TLN10_PM_ADDR_MATCH_12 0x811C0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ADDR_MATCH_12 0x1DF811C0u

//! Register Reset Value
#define TLN10_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN10_PM_REQ_INFO_PERMISSION_12 Register TLN10_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TLN10_PM_REQ_INFO_PERMISSION_12 0x811C8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_REQ_INFO_PERMISSION_12 0x1DF811C8u

//! Register Reset Value
#define TLN10_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_READ_PERMISSION_12 Register TLN10_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TLN10_PM_READ_PERMISSION_12 0x811D0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_READ_PERMISSION_12 0x1DF811D0u

//! Register Reset Value
#define TLN10_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_WRITE_PERMISSION_12 Register TLN10_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TLN10_PM_WRITE_PERMISSION_12 0x811D8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_WRITE_PERMISSION_12 0x1DF811D8u

//! Register Reset Value
#define TLN10_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_ADDR_MATCH_13 Register TLN10_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TLN10_PM_ADDR_MATCH_13 0x811E0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ADDR_MATCH_13 0x1DF811E0u

//! Register Reset Value
#define TLN10_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN10_PM_REQ_INFO_PERMISSION_13 Register TLN10_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TLN10_PM_REQ_INFO_PERMISSION_13 0x811E8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_REQ_INFO_PERMISSION_13 0x1DF811E8u

//! Register Reset Value
#define TLN10_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_READ_PERMISSION_13 Register TLN10_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TLN10_PM_READ_PERMISSION_13 0x811F0
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_READ_PERMISSION_13 0x1DF811F0u

//! Register Reset Value
#define TLN10_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_WRITE_PERMISSION_13 Register TLN10_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TLN10_PM_WRITE_PERMISSION_13 0x811F8
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_WRITE_PERMISSION_13 0x1DF811F8u

//! Register Reset Value
#define TLN10_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_ADDR_MATCH_14 Register TLN10_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TLN10_PM_ADDR_MATCH_14 0x81200
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ADDR_MATCH_14 0x1DF81200u

//! Register Reset Value
#define TLN10_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN10_PM_REQ_INFO_PERMISSION_14 Register TLN10_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TLN10_PM_REQ_INFO_PERMISSION_14 0x81208
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_REQ_INFO_PERMISSION_14 0x1DF81208u

//! Register Reset Value
#define TLN10_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_READ_PERMISSION_14 Register TLN10_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TLN10_PM_READ_PERMISSION_14 0x81210
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_READ_PERMISSION_14 0x1DF81210u

//! Register Reset Value
#define TLN10_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_WRITE_PERMISSION_14 Register TLN10_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TLN10_PM_WRITE_PERMISSION_14 0x81218
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_WRITE_PERMISSION_14 0x1DF81218u

//! Register Reset Value
#define TLN10_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_ADDR_MATCH_15 Register TLN10_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TLN10_PM_ADDR_MATCH_15 0x81220
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_ADDR_MATCH_15 0x1DF81220u

//! Register Reset Value
#define TLN10_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN10_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TLN10_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TLN10_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN10_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN10_PM_REQ_INFO_PERMISSION_15 Register TLN10_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TLN10_PM_REQ_INFO_PERMISSION_15 0x81228
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_REQ_INFO_PERMISSION_15 0x1DF81228u

//! Register Reset Value
#define TLN10_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN10_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_READ_PERMISSION_15 Register TLN10_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TLN10_PM_READ_PERMISSION_15 0x81230
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_READ_PERMISSION_15 0x1DF81230u

//! Register Reset Value
#define TLN10_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN10_PM_WRITE_PERMISSION_15 Register TLN10_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TLN10_PM_WRITE_PERMISSION_15 0x81238
//! Register Offset (absolute) for 1st Instance SSX1_SSX
#define SSX1_SSX_TLN10_PM_WRITE_PERMISSION_15 0x1DF81238u

//! Register Reset Value
#define TLN10_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN10_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! @}

#endif
