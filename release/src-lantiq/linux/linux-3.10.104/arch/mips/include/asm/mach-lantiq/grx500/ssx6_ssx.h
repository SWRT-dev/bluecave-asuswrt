//-----------------------------------------------------------------------------
// LSD Generator
//-----------------------------------------------------------------------------
// Perl Package        : LSD::generator::targetC (v1.1)
// LSD Source          : D:/Users/shij/Perforce/l1033.grx500.win.v_shij.priv.dfv.grx500.dfv/ipg_lsd/lsd_sys/source/xml/reg_files/ssx6_ssx.xml
// Register File Name  : SSX6_SSX
// Register File Title : SSX6_ssx
// Register Width      : 64
// Note                : Doxygen compliant comments
//-----------------------------------------------------------------------------

#ifndef _SSX6_SSX_H
#define _SSX6_SSX_H

//! \defgroup SSX6_SSX Register File SSX6_SSX - SSX6_ssx
//! @{

//! Base Address of SSX6_SSX
#define SSX6_SSX_MODULE_BASE 0x13F00000u

//! \defgroup TREG6_RT_COMPONENT Register TREG6_RT_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TREG6_RT_COMPONENT 0x0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_RT_COMPONENT 0x13F00000u

//! Register Reset Value
#define TREG6_RT_COMPONENT_RST 0x0000000062003532u

//! Field REV - rev
#define TREG6_RT_COMPONENT_REV_POS 0
//! Field REV - rev
#define TREG6_RT_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TREG6_RT_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TREG6_RT_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TREG6_RT_NETWORK Register TREG6_RT_NETWORK - network
//! @{

//! Register Offset (relative)
#define TREG6_RT_NETWORK 0x10
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_RT_NETWORK 0x13F00010u

//! Register Reset Value
#define TREG6_RT_NETWORK_RST 0x0000000000000000u

//! Field REV - rev
#define TREG6_RT_NETWORK_REV_POS 32
//! Field REV - rev
#define TREG6_RT_NETWORK_REV_MASK 0xFFFF00000000u

//! Field ID - id
#define TREG6_RT_NETWORK_ID_POS 48
//! Field ID - id
#define TREG6_RT_NETWORK_ID_MASK 0xFFFF000000000000u

//! @}

//! \defgroup TREG6_RT_INITID_READBACK Register TREG6_RT_INITID_READBACK - initid_readback
//! @{

//! Register Offset (relative)
#define TREG6_RT_INITID_READBACK 0x70
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_RT_INITID_READBACK 0x13F00070u

//! Register Reset Value
#define TREG6_RT_INITID_READBACK_RST 0x0000000000000000u

//! Field INITID - initid
#define TREG6_RT_INITID_READBACK_INITID_POS 0
//! Field INITID - initid
#define TREG6_RT_INITID_READBACK_INITID_MASK 0xFFu

//! @}

//! \defgroup TREG6_RT_NETWORK_CONTROL Register TREG6_RT_NETWORK_CONTROL - network_control
//! @{

//! Register Offset (relative)
#define TREG6_RT_NETWORK_CONTROL 0x78
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_RT_NETWORK_CONTROL 0x13F00078u

//! Register Reset Value
#define TREG6_RT_NETWORK_CONTROL_RST 0x0000000000000000u

//! Field TIMEOUT_BASE - timeout_base
#define TREG6_RT_NETWORK_CONTROL_TIMEOUT_BASE_POS 8
//! Field TIMEOUT_BASE - timeout_base
#define TREG6_RT_NETWORK_CONTROL_TIMEOUT_BASE_MASK 0x700u

//! Field CLOCK_GATE_DISABLE - clock_gate_disable
#define TREG6_RT_NETWORK_CONTROL_CLOCK_GATE_DISABLE_POS 56
//! Field CLOCK_GATE_DISABLE - clock_gate_disable
#define TREG6_RT_NETWORK_CONTROL_CLOCK_GATE_DISABLE_MASK 0x100000000000000u

//! @}

//! \defgroup TREG6_SI_CONTROL Register TREG6_SI_CONTROL - control
//! @{

//! Register Offset (relative)
#define TREG6_SI_CONTROL 0x420
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_SI_CONTROL 0x13F00420u

//! Register Reset Value
#define TREG6_SI_CONTROL_RST 0x0000000000000000u

//! Field CLOCK_GATE_DISABLE - clock_gate_disable
#define TREG6_SI_CONTROL_CLOCK_GATE_DISABLE_POS 56
//! Field CLOCK_GATE_DISABLE - clock_gate_disable
#define TREG6_SI_CONTROL_CLOCK_GATE_DISABLE_MASK 0x100000000000000u

//! @}

//! \defgroup TREG6_SI_FLAG_STATUS_0 Register TREG6_SI_FLAG_STATUS_0 - flag_status_0
//! @{

//! Register Offset (relative)
#define TREG6_SI_FLAG_STATUS_0 0x510
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_SI_FLAG_STATUS_0 0x13F00510u

//! Register Reset Value
#define TREG6_SI_FLAG_STATUS_0_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG6_SI_FLAG_STATUS_0_STATUS_POS 0
//! Field STATUS - status
#define TREG6_SI_FLAG_STATUS_0_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_SI_FLAG_STATUS_1 Register TREG6_SI_FLAG_STATUS_1 - flag_status_1
//! @{

//! Register Offset (relative)
#define TREG6_SI_FLAG_STATUS_1 0x530
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_SI_FLAG_STATUS_1 0x13F00530u

//! Register Reset Value
#define TREG6_SI_FLAG_STATUS_1_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG6_SI_FLAG_STATUS_1_STATUS_POS 0
//! Field STATUS - status
#define TREG6_SI_FLAG_STATUS_1_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_SI_FLAG_STATUS_2 Register TREG6_SI_FLAG_STATUS_2 - flag_status_2
//! @{

//! Register Offset (relative)
#define TREG6_SI_FLAG_STATUS_2 0x550
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_SI_FLAG_STATUS_2 0x13F00550u

//! Register Reset Value
#define TREG6_SI_FLAG_STATUS_2_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG6_SI_FLAG_STATUS_2_STATUS_POS 0
//! Field STATUS - status
#define TREG6_SI_FLAG_STATUS_2_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_SI_FLAG_STATUS_3 Register TREG6_SI_FLAG_STATUS_3 - flag_status_3
//! @{

//! Register Offset (relative)
#define TREG6_SI_FLAG_STATUS_3 0x570
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_SI_FLAG_STATUS_3 0x13F00570u

//! Register Reset Value
#define TREG6_SI_FLAG_STATUS_3_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG6_SI_FLAG_STATUS_3_STATUS_POS 0
//! Field STATUS - status
#define TREG6_SI_FLAG_STATUS_3_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_SI_FLAG_STATUS_4 Register TREG6_SI_FLAG_STATUS_4 - flag_status_4
//! @{

//! Register Offset (relative)
#define TREG6_SI_FLAG_STATUS_4 0x590
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_SI_FLAG_STATUS_4 0x13F00590u

//! Register Reset Value
#define TREG6_SI_FLAG_STATUS_4_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG6_SI_FLAG_STATUS_4_STATUS_POS 0
//! Field STATUS - status
#define TREG6_SI_FLAG_STATUS_4_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_SI_FLAG_STATUS_5 Register TREG6_SI_FLAG_STATUS_5 - flag_status_5
//! @{

//! Register Offset (relative)
#define TREG6_SI_FLAG_STATUS_5 0x5B0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_SI_FLAG_STATUS_5 0x13F005B0u

//! Register Reset Value
#define TREG6_SI_FLAG_STATUS_5_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG6_SI_FLAG_STATUS_5_STATUS_POS 0
//! Field STATUS - status
#define TREG6_SI_FLAG_STATUS_5_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_SI_FLAG_STATUS_6 Register TREG6_SI_FLAG_STATUS_6 - flag_status_6
//! @{

//! Register Offset (relative)
#define TREG6_SI_FLAG_STATUS_6 0x5D0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_SI_FLAG_STATUS_6 0x13F005D0u

//! Register Reset Value
#define TREG6_SI_FLAG_STATUS_6_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG6_SI_FLAG_STATUS_6_STATUS_POS 0
//! Field STATUS - status
#define TREG6_SI_FLAG_STATUS_6_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_SI_FLAG_STATUS_7 Register TREG6_SI_FLAG_STATUS_7 - flag_status_7
//! @{

//! Register Offset (relative)
#define TREG6_SI_FLAG_STATUS_7 0x5F0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_SI_FLAG_STATUS_7 0x13F005F0u

//! Register Reset Value
#define TREG6_SI_FLAG_STATUS_7_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG6_SI_FLAG_STATUS_7_STATUS_POS 0
//! Field STATUS - status
#define TREG6_SI_FLAG_STATUS_7_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_TA_COMPONENT Register TIC_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TIC_TA_COMPONENT 0x1000
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_TA_COMPONENT 0x13F01000u

//! Register Reset Value
#define TIC_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TIC_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TIC_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TIC_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TIC_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TIC_TA_CORE Register TIC_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TIC_TA_CORE 0x1018
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_TA_CORE 0x13F01018u

//! Register Reset Value
#define TIC_TA_CORE_RST 0x000050C500620001u

//! Field REV_CODE - rev_code
#define TIC_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TIC_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TIC_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TIC_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TIC_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TIC_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TIC_TA_AGENT_CONTROL Register TIC_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TIC_TA_AGENT_CONTROL 0x1020
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_TA_AGENT_CONTROL 0x13F01020u

//! Register Reset Value
#define TIC_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TIC_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TIC_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TIC_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TIC_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TIC_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TIC_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TIC_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TIC_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TIC_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TIC_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TIC_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TIC_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TIC_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TIC_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TIC_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TIC_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TIC_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TIC_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TIC_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TIC_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TIC_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TIC_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TIC_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TIC_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TIC_TA_AGENT_STATUS Register TIC_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TIC_TA_AGENT_STATUS 0x1028
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_TA_AGENT_STATUS 0x13F01028u

//! Register Reset Value
#define TIC_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TIC_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TIC_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TIC_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TIC_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TIC_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TIC_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TIC_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TIC_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TIC_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TIC_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TIC_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TIC_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TIC_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TIC_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TIC_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TIC_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TIC_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TIC_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TIC_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TIC_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TIC_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TIC_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TIC_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TIC_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TIC_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TIC_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TIC_TA_ERROR_LOG Register TIC_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TIC_TA_ERROR_LOG 0x1058
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_TA_ERROR_LOG 0x13F01058u

//! Register Reset Value
#define TIC_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TIC_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TIC_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TIC_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TIC_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TIC_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TIC_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TIC_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TIC_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TIC_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TIC_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TIC_TA_ERROR_LOG_ADDR Register TIC_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TIC_TA_ERROR_LOG_ADDR 0x1060
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_TA_ERROR_LOG_ADDR 0x13F01060u

//! Register Reset Value
#define TIC_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TIC_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TIC_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_TA_BANDWIDTH_0 Register TIC_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TIC_TA_BANDWIDTH_0 0x1100
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_TA_BANDWIDTH_0 0x13F01100u

//! Register Reset Value
#define TIC_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TIC_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TIC_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TIC_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TIC_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TIC_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TIC_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TIC_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TIC_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TIC_TA_BANDWIDTH_1 Register TIC_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TIC_TA_BANDWIDTH_1 0x1108
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_TA_BANDWIDTH_1 0x13F01108u

//! Register Reset Value
#define TIC_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TIC_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TIC_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TIC_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TIC_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TIC_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TIC_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TIC_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TIC_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TIC_TA_BANDWIDTH_2 Register TIC_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TIC_TA_BANDWIDTH_2 0x1110
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_TA_BANDWIDTH_2 0x13F01110u

//! Register Reset Value
#define TIC_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TIC_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TIC_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TIC_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TIC_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TIC_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TIC_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TIC_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TIC_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TIC_TA_BANDWIDTH_3 Register TIC_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TIC_TA_BANDWIDTH_3 0x1118
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_TA_BANDWIDTH_3 0x13F01118u

//! Register Reset Value
#define TIC_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TIC_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TIC_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TIC_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TIC_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TIC_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TIC_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TIC_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TIC_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TIC_TA_ALLOC_LIMIT_0 Register TIC_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TIC_TA_ALLOC_LIMIT_0 0x1200
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_TA_ALLOC_LIMIT_0 0x13F01200u

//! Register Reset Value
#define TIC_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TIC_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TIC_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TIC_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TIC_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TIC_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TIC_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TIC_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TIC_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TIC_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TIC_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TIC_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TIC_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TIC_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TIC_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TIC_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TIC_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TIC_TA_ALLOC_LIMIT_1 Register TIC_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TIC_TA_ALLOC_LIMIT_1 0x1208
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_TA_ALLOC_LIMIT_1 0x13F01208u

//! Register Reset Value
#define TIC_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TIC_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TIC_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TIC_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TIC_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TIC_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TIC_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TIC_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TIC_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TIC_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TIC_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TIC_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TIC_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TIC_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TIC_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TIC_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TIC_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TIC_TA_ALLOC_LIMIT_2 Register TIC_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TIC_TA_ALLOC_LIMIT_2 0x1210
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_TA_ALLOC_LIMIT_2 0x13F01210u

//! Register Reset Value
#define TIC_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TIC_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TIC_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TIC_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TIC_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TIC_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TIC_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TIC_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TIC_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TIC_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TIC_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TIC_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TIC_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TIC_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TIC_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TIC_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TIC_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TIC_TA_ALLOC_LIMIT_3 Register TIC_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TIC_TA_ALLOC_LIMIT_3 0x1218
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_TA_ALLOC_LIMIT_3 0x13F01218u

//! Register Reset Value
#define TIC_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TIC_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TIC_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TIC_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TIC_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TIC_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TIC_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TIC_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TIC_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TIC_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TIC_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TIC_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TIC_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TIC_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TIC_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TIC_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TIC_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDDR_TA_COMPONENT Register TDDR_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TDDR_TA_COMPONENT 0x1400
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_TA_COMPONENT 0x13F01400u

//! Register Reset Value
#define TDDR_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TDDR_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TDDR_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TDDR_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TDDR_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TDDR_TA_CORE Register TDDR_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TDDR_TA_CORE 0x1418
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_TA_CORE 0x13F01418u

//! Register Reset Value
#define TDDR_TA_CORE_RST 0x000050C500650003u

//! Field REV_CODE - rev_code
#define TDDR_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TDDR_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TDDR_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TDDR_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TDDR_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TDDR_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TDDR_TA_AGENT_CONTROL Register TDDR_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TDDR_TA_AGENT_CONTROL 0x1420
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_TA_AGENT_CONTROL 0x13F01420u

//! Register Reset Value
#define TDDR_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TDDR_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TDDR_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TDDR_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TDDR_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TDDR_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TDDR_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDDR_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDDR_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TDDR_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TDDR_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TDDR_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TDDR_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TDDR_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TDDR_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TDDR_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TDDR_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TDDR_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TDDR_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TDDR_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TDDR_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TDDR_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TDDR_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TDDR_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TDDR_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TDDR_TA_AGENT_STATUS Register TDDR_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TDDR_TA_AGENT_STATUS 0x1428
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_TA_AGENT_STATUS 0x13F01428u

//! Register Reset Value
#define TDDR_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TDDR_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TDDR_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDDR_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDDR_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TDDR_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TDDR_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TDDR_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TDDR_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TDDR_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TDDR_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TDDR_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TDDR_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TDDR_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TDDR_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TDDR_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TDDR_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TDDR_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TDDR_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TDDR_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TDDR_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TDDR_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TDDR_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TDDR_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TDDR_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TDDR_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TDDR_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TDDR_TA_ERROR_LOG Register TDDR_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TDDR_TA_ERROR_LOG 0x1458
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_TA_ERROR_LOG 0x13F01458u

//! Register Reset Value
#define TDDR_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TDDR_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TDDR_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TDDR_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TDDR_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TDDR_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TDDR_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TDDR_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TDDR_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TDDR_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TDDR_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TDDR_TA_ERROR_LOG_ADDR Register TDDR_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TDDR_TA_ERROR_LOG_ADDR 0x1460
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_TA_ERROR_LOG_ADDR 0x13F01460u

//! Register Reset Value
#define TDDR_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TDDR_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TDDR_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDDR_TA_BANDWIDTH_0 Register TDDR_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TDDR_TA_BANDWIDTH_0 0x1500
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_TA_BANDWIDTH_0 0x13F01500u

//! Register Reset Value
#define TDDR_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TDDR_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TDDR_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TDDR_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TDDR_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TDDR_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TDDR_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TDDR_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TDDR_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDDR_TA_BANDWIDTH_1 Register TDDR_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TDDR_TA_BANDWIDTH_1 0x1508
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_TA_BANDWIDTH_1 0x13F01508u

//! Register Reset Value
#define TDDR_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TDDR_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TDDR_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TDDR_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TDDR_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TDDR_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TDDR_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TDDR_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TDDR_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDDR_TA_BANDWIDTH_2 Register TDDR_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TDDR_TA_BANDWIDTH_2 0x1510
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_TA_BANDWIDTH_2 0x13F01510u

//! Register Reset Value
#define TDDR_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TDDR_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TDDR_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TDDR_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TDDR_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TDDR_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TDDR_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TDDR_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TDDR_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDDR_TA_BANDWIDTH_3 Register TDDR_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TDDR_TA_BANDWIDTH_3 0x1518
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_TA_BANDWIDTH_3 0x13F01518u

//! Register Reset Value
#define TDDR_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TDDR_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TDDR_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TDDR_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TDDR_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TDDR_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TDDR_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TDDR_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TDDR_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDDR_TA_ALLOC_LIMIT_0 Register TDDR_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TDDR_TA_ALLOC_LIMIT_0 0x1600
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_TA_ALLOC_LIMIT_0 0x13F01600u

//! Register Reset Value
#define TDDR_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TDDR_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TDDR_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TDDR_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TDDR_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TDDR_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TDDR_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TDDR_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TDDR_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TDDR_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TDDR_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TDDR_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TDDR_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TDDR_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TDDR_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TDDR_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TDDR_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDDR_TA_ALLOC_LIMIT_1 Register TDDR_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TDDR_TA_ALLOC_LIMIT_1 0x1608
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_TA_ALLOC_LIMIT_1 0x13F01608u

//! Register Reset Value
#define TDDR_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TDDR_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TDDR_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TDDR_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TDDR_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TDDR_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TDDR_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TDDR_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TDDR_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TDDR_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TDDR_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TDDR_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TDDR_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TDDR_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TDDR_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TDDR_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TDDR_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDDR_TA_ALLOC_LIMIT_2 Register TDDR_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TDDR_TA_ALLOC_LIMIT_2 0x1610
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_TA_ALLOC_LIMIT_2 0x13F01610u

//! Register Reset Value
#define TDDR_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TDDR_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TDDR_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TDDR_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TDDR_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TDDR_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TDDR_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TDDR_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TDDR_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TDDR_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TDDR_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TDDR_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TDDR_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TDDR_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TDDR_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TDDR_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TDDR_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDDR_TA_ALLOC_LIMIT_3 Register TDDR_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TDDR_TA_ALLOC_LIMIT_3 0x1618
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_TA_ALLOC_LIMIT_3 0x13F01618u

//! Register Reset Value
#define TDDR_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TDDR_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TDDR_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TDDR_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TDDR_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TDDR_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TDDR_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TDDR_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TDDR_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TDDR_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TDDR_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TDDR_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TDDR_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TDDR_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TDDR_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TDDR_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TDDR_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TPCTL_TA_COMPONENT Register TPCTL_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TPCTL_TA_COMPONENT 0x1800
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_TA_COMPONENT 0x13F01800u

//! Register Reset Value
#define TPCTL_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TPCTL_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TPCTL_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TPCTL_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TPCTL_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TPCTL_TA_CORE Register TPCTL_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TPCTL_TA_CORE 0x1818
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_TA_CORE 0x13F01818u

//! Register Reset Value
#define TPCTL_TA_CORE_RST 0x000050C500630001u

//! Field REV_CODE - rev_code
#define TPCTL_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TPCTL_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TPCTL_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TPCTL_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TPCTL_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TPCTL_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TPCTL_TA_AGENT_CONTROL Register TPCTL_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TPCTL_TA_AGENT_CONTROL 0x1820
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_TA_AGENT_CONTROL 0x13F01820u

//! Register Reset Value
#define TPCTL_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TPCTL_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TPCTL_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TPCTL_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TPCTL_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TPCTL_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TPCTL_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TPCTL_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TPCTL_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TPCTL_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TPCTL_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TPCTL_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TPCTL_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TPCTL_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TPCTL_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TPCTL_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TPCTL_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TPCTL_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TPCTL_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TPCTL_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TPCTL_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TPCTL_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TPCTL_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TPCTL_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TPCTL_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TPCTL_TA_AGENT_STATUS Register TPCTL_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TPCTL_TA_AGENT_STATUS 0x1828
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_TA_AGENT_STATUS 0x13F01828u

//! Register Reset Value
#define TPCTL_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TPCTL_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TPCTL_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TPCTL_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TPCTL_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TPCTL_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TPCTL_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TPCTL_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TPCTL_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TPCTL_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TPCTL_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TPCTL_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TPCTL_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TPCTL_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TPCTL_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TPCTL_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TPCTL_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TPCTL_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TPCTL_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TPCTL_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TPCTL_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TPCTL_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TPCTL_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TPCTL_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TPCTL_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TPCTL_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TPCTL_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TPCTL_TA_ERROR_LOG Register TPCTL_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TPCTL_TA_ERROR_LOG 0x1858
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_TA_ERROR_LOG 0x13F01858u

//! Register Reset Value
#define TPCTL_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TPCTL_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TPCTL_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TPCTL_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TPCTL_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TPCTL_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TPCTL_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TPCTL_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TPCTL_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TPCTL_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TPCTL_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TPCTL_TA_ERROR_LOG_ADDR Register TPCTL_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TPCTL_TA_ERROR_LOG_ADDR 0x1860
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_TA_ERROR_LOG_ADDR 0x13F01860u

//! Register Reset Value
#define TPCTL_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TPCTL_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TPCTL_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_TA_BANDWIDTH_0 Register TPCTL_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TPCTL_TA_BANDWIDTH_0 0x1900
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_TA_BANDWIDTH_0 0x13F01900u

//! Register Reset Value
#define TPCTL_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TPCTL_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TPCTL_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TPCTL_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TPCTL_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TPCTL_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TPCTL_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TPCTL_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TPCTL_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TPCTL_TA_BANDWIDTH_1 Register TPCTL_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TPCTL_TA_BANDWIDTH_1 0x1908
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_TA_BANDWIDTH_1 0x13F01908u

//! Register Reset Value
#define TPCTL_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TPCTL_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TPCTL_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TPCTL_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TPCTL_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TPCTL_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TPCTL_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TPCTL_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TPCTL_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TPCTL_TA_BANDWIDTH_2 Register TPCTL_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TPCTL_TA_BANDWIDTH_2 0x1910
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_TA_BANDWIDTH_2 0x13F01910u

//! Register Reset Value
#define TPCTL_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TPCTL_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TPCTL_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TPCTL_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TPCTL_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TPCTL_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TPCTL_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TPCTL_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TPCTL_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TPCTL_TA_BANDWIDTH_3 Register TPCTL_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TPCTL_TA_BANDWIDTH_3 0x1918
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_TA_BANDWIDTH_3 0x13F01918u

//! Register Reset Value
#define TPCTL_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TPCTL_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TPCTL_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TPCTL_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TPCTL_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TPCTL_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TPCTL_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TPCTL_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TPCTL_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TPCTL_TA_ALLOC_LIMIT_0 Register TPCTL_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TPCTL_TA_ALLOC_LIMIT_0 0x1A00
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_TA_ALLOC_LIMIT_0 0x13F01A00u

//! Register Reset Value
#define TPCTL_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TPCTL_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TPCTL_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TPCTL_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TPCTL_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TPCTL_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TPCTL_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TPCTL_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TPCTL_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TPCTL_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TPCTL_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TPCTL_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TPCTL_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TPCTL_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TPCTL_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TPCTL_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TPCTL_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TPCTL_TA_ALLOC_LIMIT_1 Register TPCTL_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TPCTL_TA_ALLOC_LIMIT_1 0x1A08
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_TA_ALLOC_LIMIT_1 0x13F01A08u

//! Register Reset Value
#define TPCTL_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TPCTL_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TPCTL_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TPCTL_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TPCTL_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TPCTL_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TPCTL_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TPCTL_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TPCTL_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TPCTL_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TPCTL_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TPCTL_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TPCTL_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TPCTL_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TPCTL_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TPCTL_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TPCTL_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TPCTL_TA_ALLOC_LIMIT_2 Register TPCTL_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TPCTL_TA_ALLOC_LIMIT_2 0x1A10
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_TA_ALLOC_LIMIT_2 0x13F01A10u

//! Register Reset Value
#define TPCTL_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TPCTL_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TPCTL_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TPCTL_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TPCTL_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TPCTL_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TPCTL_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TPCTL_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TPCTL_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TPCTL_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TPCTL_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TPCTL_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TPCTL_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TPCTL_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TPCTL_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TPCTL_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TPCTL_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TPCTL_TA_ALLOC_LIMIT_3 Register TPCTL_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TPCTL_TA_ALLOC_LIMIT_3 0x1A18
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_TA_ALLOC_LIMIT_3 0x13F01A18u

//! Register Reset Value
#define TPCTL_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TPCTL_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TPCTL_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TPCTL_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TPCTL_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TPCTL_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TPCTL_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TPCTL_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TPCTL_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TPCTL_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TPCTL_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TPCTL_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TPCTL_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TPCTL_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TPCTL_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TPCTL_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TPCTL_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TPUB_TA_COMPONENT Register TPUB_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TPUB_TA_COMPONENT 0x1C00
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_TA_COMPONENT 0x13F01C00u

//! Register Reset Value
#define TPUB_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TPUB_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TPUB_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TPUB_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TPUB_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TPUB_TA_CORE Register TPUB_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TPUB_TA_CORE 0x1C18
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_TA_CORE 0x13F01C18u

//! Register Reset Value
#define TPUB_TA_CORE_RST 0x000050C500640001u

//! Field REV_CODE - rev_code
#define TPUB_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TPUB_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TPUB_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TPUB_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TPUB_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TPUB_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TPUB_TA_AGENT_CONTROL Register TPUB_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TPUB_TA_AGENT_CONTROL 0x1C20
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_TA_AGENT_CONTROL 0x13F01C20u

//! Register Reset Value
#define TPUB_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TPUB_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TPUB_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TPUB_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TPUB_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TPUB_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TPUB_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TPUB_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TPUB_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TPUB_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TPUB_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TPUB_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TPUB_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TPUB_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TPUB_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TPUB_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TPUB_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TPUB_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TPUB_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TPUB_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TPUB_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TPUB_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TPUB_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TPUB_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TPUB_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TPUB_TA_AGENT_STATUS Register TPUB_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TPUB_TA_AGENT_STATUS 0x1C28
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_TA_AGENT_STATUS 0x13F01C28u

//! Register Reset Value
#define TPUB_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TPUB_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TPUB_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TPUB_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TPUB_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TPUB_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TPUB_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TPUB_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TPUB_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TPUB_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TPUB_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TPUB_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TPUB_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TPUB_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TPUB_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TPUB_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TPUB_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TPUB_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TPUB_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TPUB_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TPUB_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TPUB_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TPUB_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TPUB_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TPUB_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TPUB_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TPUB_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TPUB_TA_ERROR_LOG Register TPUB_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TPUB_TA_ERROR_LOG 0x1C58
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_TA_ERROR_LOG 0x13F01C58u

//! Register Reset Value
#define TPUB_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TPUB_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TPUB_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TPUB_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TPUB_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TPUB_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TPUB_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TPUB_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TPUB_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TPUB_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TPUB_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TPUB_TA_ERROR_LOG_ADDR Register TPUB_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TPUB_TA_ERROR_LOG_ADDR 0x1C60
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_TA_ERROR_LOG_ADDR 0x13F01C60u

//! Register Reset Value
#define TPUB_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TPUB_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TPUB_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_TA_BANDWIDTH_0 Register TPUB_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TPUB_TA_BANDWIDTH_0 0x1D00
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_TA_BANDWIDTH_0 0x13F01D00u

//! Register Reset Value
#define TPUB_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TPUB_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TPUB_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TPUB_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TPUB_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TPUB_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TPUB_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TPUB_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TPUB_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TPUB_TA_BANDWIDTH_1 Register TPUB_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TPUB_TA_BANDWIDTH_1 0x1D08
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_TA_BANDWIDTH_1 0x13F01D08u

//! Register Reset Value
#define TPUB_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TPUB_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TPUB_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TPUB_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TPUB_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TPUB_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TPUB_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TPUB_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TPUB_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TPUB_TA_BANDWIDTH_2 Register TPUB_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TPUB_TA_BANDWIDTH_2 0x1D10
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_TA_BANDWIDTH_2 0x13F01D10u

//! Register Reset Value
#define TPUB_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TPUB_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TPUB_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TPUB_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TPUB_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TPUB_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TPUB_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TPUB_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TPUB_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TPUB_TA_BANDWIDTH_3 Register TPUB_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TPUB_TA_BANDWIDTH_3 0x1D18
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_TA_BANDWIDTH_3 0x13F01D18u

//! Register Reset Value
#define TPUB_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TPUB_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TPUB_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TPUB_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TPUB_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TPUB_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TPUB_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TPUB_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TPUB_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TPUB_TA_ALLOC_LIMIT_0 Register TPUB_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TPUB_TA_ALLOC_LIMIT_0 0x1E00
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_TA_ALLOC_LIMIT_0 0x13F01E00u

//! Register Reset Value
#define TPUB_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TPUB_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TPUB_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TPUB_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TPUB_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TPUB_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TPUB_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TPUB_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TPUB_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TPUB_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TPUB_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TPUB_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TPUB_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TPUB_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TPUB_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TPUB_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TPUB_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TPUB_TA_ALLOC_LIMIT_1 Register TPUB_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TPUB_TA_ALLOC_LIMIT_1 0x1E08
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_TA_ALLOC_LIMIT_1 0x13F01E08u

//! Register Reset Value
#define TPUB_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TPUB_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TPUB_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TPUB_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TPUB_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TPUB_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TPUB_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TPUB_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TPUB_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TPUB_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TPUB_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TPUB_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TPUB_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TPUB_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TPUB_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TPUB_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TPUB_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TPUB_TA_ALLOC_LIMIT_2 Register TPUB_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TPUB_TA_ALLOC_LIMIT_2 0x1E10
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_TA_ALLOC_LIMIT_2 0x13F01E10u

//! Register Reset Value
#define TPUB_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TPUB_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TPUB_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TPUB_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TPUB_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TPUB_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TPUB_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TPUB_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TPUB_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TPUB_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TPUB_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TPUB_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TPUB_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TPUB_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TPUB_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TPUB_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TPUB_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TPUB_TA_ALLOC_LIMIT_3 Register TPUB_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TPUB_TA_ALLOC_LIMIT_3 0x1E18
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_TA_ALLOC_LIMIT_3 0x13F01E18u

//! Register Reset Value
#define TPUB_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TPUB_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TPUB_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TPUB_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TPUB_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TPUB_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TPUB_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TPUB_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TPUB_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TPUB_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TPUB_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TPUB_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TPUB_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TPUB_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TPUB_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TPUB_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TPUB_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN60_TA_COMPONENT Register TLN60_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TLN60_TA_COMPONENT 0x2000
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_TA_COMPONENT 0x13F02000u

//! Register Reset Value
#define TLN60_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TLN60_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TLN60_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TLN60_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TLN60_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TLN60_TA_CORE Register TLN60_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TLN60_TA_CORE 0x2018
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_TA_CORE 0x13F02018u

//! Register Reset Value
#define TLN60_TA_CORE_RST 0x000050C50F600001u

//! Field REV_CODE - rev_code
#define TLN60_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TLN60_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TLN60_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TLN60_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TLN60_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TLN60_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TLN60_TA_AGENT_CONTROL Register TLN60_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TLN60_TA_AGENT_CONTROL 0x2020
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_TA_AGENT_CONTROL 0x13F02020u

//! Register Reset Value
#define TLN60_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TLN60_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TLN60_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TLN60_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TLN60_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TLN60_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TLN60_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TLN60_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TLN60_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TLN60_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TLN60_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TLN60_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TLN60_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TLN60_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TLN60_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TLN60_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TLN60_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TLN60_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TLN60_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TLN60_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TLN60_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TLN60_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TLN60_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TLN60_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TLN60_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TLN60_TA_AGENT_STATUS Register TLN60_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TLN60_TA_AGENT_STATUS 0x2028
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_TA_AGENT_STATUS 0x13F02028u

//! Register Reset Value
#define TLN60_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TLN60_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TLN60_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TLN60_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TLN60_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TLN60_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TLN60_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TLN60_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TLN60_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TLN60_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TLN60_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TLN60_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TLN60_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TLN60_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TLN60_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TLN60_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TLN60_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TLN60_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TLN60_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TLN60_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TLN60_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TLN60_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TLN60_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TLN60_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TLN60_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TLN60_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TLN60_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TLN60_TA_ERROR_LOG Register TLN60_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TLN60_TA_ERROR_LOG 0x2058
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_TA_ERROR_LOG 0x13F02058u

//! Register Reset Value
#define TLN60_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TLN60_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TLN60_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TLN60_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TLN60_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TLN60_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TLN60_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TLN60_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TLN60_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TLN60_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TLN60_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TLN60_TA_ERROR_LOG_ADDR Register TLN60_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TLN60_TA_ERROR_LOG_ADDR 0x2060
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_TA_ERROR_LOG_ADDR 0x13F02060u

//! Register Reset Value
#define TLN60_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TLN60_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TLN60_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_TA_BANDWIDTH_0 Register TLN60_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TLN60_TA_BANDWIDTH_0 0x2100
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_TA_BANDWIDTH_0 0x13F02100u

//! Register Reset Value
#define TLN60_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TLN60_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TLN60_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TLN60_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TLN60_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TLN60_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TLN60_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TLN60_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TLN60_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN60_TA_BANDWIDTH_1 Register TLN60_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TLN60_TA_BANDWIDTH_1 0x2108
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_TA_BANDWIDTH_1 0x13F02108u

//! Register Reset Value
#define TLN60_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TLN60_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TLN60_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TLN60_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TLN60_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TLN60_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TLN60_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TLN60_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TLN60_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN60_TA_BANDWIDTH_2 Register TLN60_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TLN60_TA_BANDWIDTH_2 0x2110
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_TA_BANDWIDTH_2 0x13F02110u

//! Register Reset Value
#define TLN60_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TLN60_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TLN60_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TLN60_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TLN60_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TLN60_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TLN60_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TLN60_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TLN60_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN60_TA_BANDWIDTH_3 Register TLN60_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TLN60_TA_BANDWIDTH_3 0x2118
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_TA_BANDWIDTH_3 0x13F02118u

//! Register Reset Value
#define TLN60_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TLN60_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TLN60_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TLN60_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TLN60_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TLN60_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TLN60_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TLN60_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TLN60_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN60_TA_ALLOC_LIMIT_0 Register TLN60_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TLN60_TA_ALLOC_LIMIT_0 0x2200
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_TA_ALLOC_LIMIT_0 0x13F02200u

//! Register Reset Value
#define TLN60_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TLN60_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TLN60_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TLN60_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TLN60_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TLN60_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TLN60_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TLN60_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TLN60_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TLN60_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TLN60_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TLN60_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TLN60_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TLN60_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TLN60_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TLN60_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TLN60_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN60_TA_ALLOC_LIMIT_1 Register TLN60_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TLN60_TA_ALLOC_LIMIT_1 0x2208
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_TA_ALLOC_LIMIT_1 0x13F02208u

//! Register Reset Value
#define TLN60_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TLN60_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TLN60_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TLN60_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TLN60_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TLN60_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TLN60_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TLN60_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TLN60_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TLN60_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TLN60_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TLN60_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TLN60_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TLN60_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TLN60_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TLN60_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TLN60_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN60_TA_ALLOC_LIMIT_2 Register TLN60_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TLN60_TA_ALLOC_LIMIT_2 0x2210
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_TA_ALLOC_LIMIT_2 0x13F02210u

//! Register Reset Value
#define TLN60_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TLN60_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TLN60_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TLN60_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TLN60_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TLN60_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TLN60_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TLN60_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TLN60_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TLN60_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TLN60_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TLN60_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TLN60_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TLN60_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TLN60_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TLN60_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TLN60_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN60_TA_ALLOC_LIMIT_3 Register TLN60_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TLN60_TA_ALLOC_LIMIT_3 0x2218
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_TA_ALLOC_LIMIT_3 0x13F02218u

//! Register Reset Value
#define TLN60_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TLN60_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TLN60_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TLN60_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TLN60_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TLN60_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TLN60_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TLN60_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TLN60_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TLN60_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TLN60_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TLN60_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TLN60_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TLN60_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TLN60_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TLN60_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TLN60_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IL2_IA_COMPONENT Register IL2_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define IL2_IA_COMPONENT 0x10000
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_IL2_IA_COMPONENT 0x13F10000u

//! Register Reset Value
#define IL2_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define IL2_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define IL2_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define IL2_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define IL2_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup IL2_IA_CORE Register IL2_IA_CORE - core
//! @{

//! Register Offset (relative)
#define IL2_IA_CORE 0x10018
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_IL2_IA_CORE 0x13F10018u

//! Register Reset Value
#define IL2_IA_CORE_RST 0x000050C5006A0001u

//! Field REV_CODE - rev_code
#define IL2_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define IL2_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define IL2_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define IL2_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define IL2_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define IL2_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup IL2_IA_AGENT_CONTROL Register IL2_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define IL2_IA_AGENT_CONTROL 0x10020
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_IL2_IA_AGENT_CONTROL 0x13F10020u

//! Register Reset Value
#define IL2_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IL2_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IL2_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define IL2_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define IL2_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define IL2_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IL2_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define IL2_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IL2_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define IL2_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define IL2_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IL2_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IL2_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IL2_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IL2_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IL2_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IL2_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IL2_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IL2_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IL2_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IL2_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IL2_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IL2_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup IL2_IA_AGENT_STATUS Register IL2_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define IL2_IA_AGENT_STATUS 0x10028
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_IL2_IA_AGENT_STATUS 0x13F10028u

//! Register Reset Value
#define IL2_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IL2_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IL2_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define IL2_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define IL2_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define IL2_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define IL2_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define IL2_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define IL2_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define IL2_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define IL2_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define IL2_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IL2_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define IL2_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define IL2_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define IL2_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IL2_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define IL2_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define IL2_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IL2_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IL2_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IL2_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IL2_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup IL2_IA_ERROR_LOG Register IL2_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define IL2_IA_ERROR_LOG 0x10058
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_IL2_IA_ERROR_LOG 0x13F10058u

//! Register Reset Value
#define IL2_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define IL2_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define IL2_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define IL2_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define IL2_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define IL2_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define IL2_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define IL2_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define IL2_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define IL2_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define IL2_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define IL2_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define IL2_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define IL2_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define IL2_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define IL2_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define IL2_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define IL2_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define IL2_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup IL2_IA_ERROR_LOG_ADDR Register IL2_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define IL2_IA_ERROR_LOG_ADDR 0x10060
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_IL2_IA_ERROR_LOG_ADDR 0x13F10060u

//! Register Reset Value
#define IL2_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define IL2_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define IL2_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup IL2_IA_CORE_FLAG Register IL2_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define IL2_IA_CORE_FLAG 0x10068
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_IL2_IA_CORE_FLAG 0x13F10068u

//! Register Reset Value
#define IL2_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define IL2_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define IL2_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define IL2_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define IL2_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define IL2_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define IL2_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define IL2_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define IL2_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup IL2_IA_ADDR_FILL_IN Register IL2_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define IL2_IA_ADDR_FILL_IN 0x10070
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_IL2_IA_ADDR_FILL_IN 0x13F10070u

//! Register Reset Value
#define IL2_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define IL2_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define IL2_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup IL2_IA_BANDWIDTH_0 Register IL2_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define IL2_IA_BANDWIDTH_0 0x10100
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_IL2_IA_BANDWIDTH_0 0x13F10100u

//! Register Reset Value
#define IL2_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define IL2_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define IL2_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define IL2_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define IL2_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define IL2_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define IL2_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define IL2_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define IL2_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define IL2_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define IL2_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define IL2_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define IL2_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define IL2_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define IL2_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define IL2_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define IL2_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup IL2_IA_BANDWIDTH_1 Register IL2_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define IL2_IA_BANDWIDTH_1 0x10108
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_IL2_IA_BANDWIDTH_1 0x13F10108u

//! Register Reset Value
#define IL2_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define IL2_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define IL2_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define IL2_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define IL2_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define IL2_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define IL2_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define IL2_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define IL2_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define IL2_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define IL2_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define IL2_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define IL2_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define IL2_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define IL2_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define IL2_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define IL2_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IL2_IA_BANDWIDTH_2 Register IL2_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define IL2_IA_BANDWIDTH_2 0x10110
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_IL2_IA_BANDWIDTH_2 0x13F10110u

//! Register Reset Value
#define IL2_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define IL2_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define IL2_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define IL2_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define IL2_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define IL2_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define IL2_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define IL2_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define IL2_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define IL2_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define IL2_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define IL2_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define IL2_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define IL2_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define IL2_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define IL2_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define IL2_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup IL2_IA_BANDWIDTH_3 Register IL2_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define IL2_IA_BANDWIDTH_3 0x10118
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_IL2_IA_BANDWIDTH_3 0x13F10118u

//! Register Reset Value
#define IL2_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define IL2_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define IL2_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define IL2_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define IL2_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define IL2_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define IL2_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define IL2_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define IL2_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define IL2_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define IL2_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define IL2_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define IL2_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define IL2_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define IL2_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define IL2_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define IL2_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup IL2_IA_BANDWIDTH_4 Register IL2_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define IL2_IA_BANDWIDTH_4 0x10120
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_IL2_IA_BANDWIDTH_4 0x13F10120u

//! Register Reset Value
#define IL2_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define IL2_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define IL2_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define IL2_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define IL2_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define IL2_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define IL2_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define IL2_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define IL2_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define IL2_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define IL2_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define IL2_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define IL2_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define IL2_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define IL2_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define IL2_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define IL2_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup IL2_IA_BANDWIDTH_5 Register IL2_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define IL2_IA_BANDWIDTH_5 0x10128
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_IL2_IA_BANDWIDTH_5 0x13F10128u

//! Register Reset Value
#define IL2_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define IL2_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define IL2_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define IL2_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define IL2_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define IL2_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define IL2_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define IL2_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define IL2_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define IL2_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define IL2_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define IL2_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define IL2_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define IL2_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define IL2_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define IL2_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define IL2_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup IL2_IA_BANDWIDTH_6 Register IL2_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define IL2_IA_BANDWIDTH_6 0x10130
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_IL2_IA_BANDWIDTH_6 0x13F10130u

//! Register Reset Value
#define IL2_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define IL2_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define IL2_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define IL2_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define IL2_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define IL2_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define IL2_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define IL2_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define IL2_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define IL2_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define IL2_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define IL2_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define IL2_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define IL2_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define IL2_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define IL2_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define IL2_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup IL2_IA_BANDWIDTH_7 Register IL2_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define IL2_IA_BANDWIDTH_7 0x10138
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_IL2_IA_BANDWIDTH_7 0x13F10138u

//! Register Reset Value
#define IL2_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define IL2_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define IL2_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define IL2_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define IL2_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define IL2_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define IL2_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define IL2_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define IL2_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define IL2_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define IL2_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define IL2_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define IL2_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define IL2_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define IL2_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define IL2_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define IL2_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup ITEP_IA_COMPONENT Register ITEP_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define ITEP_IA_COMPONENT 0x10400
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ITEP_IA_COMPONENT 0x13F10400u

//! Register Reset Value
#define ITEP_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define ITEP_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define ITEP_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define ITEP_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define ITEP_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup ITEP_IA_CORE Register ITEP_IA_CORE - core
//! @{

//! Register Offset (relative)
#define ITEP_IA_CORE 0x10418
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ITEP_IA_CORE 0x13F10418u

//! Register Reset Value
#define ITEP_IA_CORE_RST 0x0000CAFE006B0000u

//! Field REV_CODE - rev_code
#define ITEP_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define ITEP_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define ITEP_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define ITEP_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define ITEP_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define ITEP_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup ITEP_IA_AGENT_CONTROL Register ITEP_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define ITEP_IA_AGENT_CONTROL 0x10420
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ITEP_IA_AGENT_CONTROL 0x13F10420u

//! Register Reset Value
#define ITEP_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define ITEP_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define ITEP_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define ITEP_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define ITEP_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define ITEP_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define ITEP_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define ITEP_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define ITEP_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define ITEP_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define ITEP_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define ITEP_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define ITEP_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define ITEP_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define ITEP_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define ITEP_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define ITEP_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define ITEP_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define ITEP_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define ITEP_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define ITEP_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define ITEP_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define ITEP_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup ITEP_IA_AGENT_STATUS Register ITEP_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define ITEP_IA_AGENT_STATUS 0x10428
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ITEP_IA_AGENT_STATUS 0x13F10428u

//! Register Reset Value
#define ITEP_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define ITEP_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define ITEP_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define ITEP_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define ITEP_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define ITEP_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define ITEP_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define ITEP_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define ITEP_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define ITEP_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define ITEP_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define ITEP_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define ITEP_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define ITEP_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define ITEP_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define ITEP_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define ITEP_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define ITEP_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define ITEP_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define ITEP_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define ITEP_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define ITEP_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define ITEP_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup ITEP_IA_ERROR_LOG Register ITEP_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define ITEP_IA_ERROR_LOG 0x10458
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ITEP_IA_ERROR_LOG 0x13F10458u

//! Register Reset Value
#define ITEP_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define ITEP_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define ITEP_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define ITEP_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define ITEP_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define ITEP_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define ITEP_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define ITEP_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define ITEP_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define ITEP_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define ITEP_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define ITEP_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define ITEP_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define ITEP_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define ITEP_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define ITEP_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define ITEP_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define ITEP_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define ITEP_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup ITEP_IA_ERROR_LOG_ADDR Register ITEP_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define ITEP_IA_ERROR_LOG_ADDR 0x10460
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ITEP_IA_ERROR_LOG_ADDR 0x13F10460u

//! Register Reset Value
#define ITEP_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define ITEP_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define ITEP_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup ITEP_IA_CORE_FLAG Register ITEP_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define ITEP_IA_CORE_FLAG 0x10468
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ITEP_IA_CORE_FLAG 0x13F10468u

//! Register Reset Value
#define ITEP_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define ITEP_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define ITEP_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define ITEP_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define ITEP_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define ITEP_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define ITEP_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define ITEP_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define ITEP_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup ITEP_IA_ADDR_FILL_IN Register ITEP_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define ITEP_IA_ADDR_FILL_IN 0x10470
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ITEP_IA_ADDR_FILL_IN 0x13F10470u

//! Register Reset Value
#define ITEP_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define ITEP_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define ITEP_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup ITEP_IA_BANDWIDTH_0 Register ITEP_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define ITEP_IA_BANDWIDTH_0 0x10500
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ITEP_IA_BANDWIDTH_0 0x13F10500u

//! Register Reset Value
#define ITEP_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define ITEP_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define ITEP_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define ITEP_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define ITEP_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define ITEP_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define ITEP_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define ITEP_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define ITEP_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define ITEP_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define ITEP_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define ITEP_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define ITEP_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define ITEP_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define ITEP_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define ITEP_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define ITEP_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup ITEP_IA_BANDWIDTH_1 Register ITEP_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define ITEP_IA_BANDWIDTH_1 0x10508
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ITEP_IA_BANDWIDTH_1 0x13F10508u

//! Register Reset Value
#define ITEP_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define ITEP_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define ITEP_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define ITEP_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define ITEP_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define ITEP_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define ITEP_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define ITEP_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define ITEP_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define ITEP_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define ITEP_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define ITEP_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define ITEP_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define ITEP_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define ITEP_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define ITEP_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define ITEP_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup ITEP_IA_BANDWIDTH_2 Register ITEP_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define ITEP_IA_BANDWIDTH_2 0x10510
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ITEP_IA_BANDWIDTH_2 0x13F10510u

//! Register Reset Value
#define ITEP_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define ITEP_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define ITEP_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define ITEP_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define ITEP_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define ITEP_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define ITEP_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define ITEP_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define ITEP_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define ITEP_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define ITEP_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define ITEP_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define ITEP_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define ITEP_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define ITEP_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define ITEP_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define ITEP_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup ITEP_IA_BANDWIDTH_3 Register ITEP_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define ITEP_IA_BANDWIDTH_3 0x10518
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ITEP_IA_BANDWIDTH_3 0x13F10518u

//! Register Reset Value
#define ITEP_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define ITEP_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define ITEP_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define ITEP_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define ITEP_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define ITEP_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define ITEP_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define ITEP_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define ITEP_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define ITEP_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define ITEP_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define ITEP_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define ITEP_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define ITEP_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define ITEP_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define ITEP_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define ITEP_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup ITEP_IA_BANDWIDTH_4 Register ITEP_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define ITEP_IA_BANDWIDTH_4 0x10520
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ITEP_IA_BANDWIDTH_4 0x13F10520u

//! Register Reset Value
#define ITEP_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define ITEP_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define ITEP_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define ITEP_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define ITEP_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define ITEP_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define ITEP_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define ITEP_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define ITEP_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define ITEP_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define ITEP_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define ITEP_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define ITEP_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define ITEP_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define ITEP_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define ITEP_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define ITEP_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup ITEP_IA_BANDWIDTH_5 Register ITEP_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define ITEP_IA_BANDWIDTH_5 0x10528
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ITEP_IA_BANDWIDTH_5 0x13F10528u

//! Register Reset Value
#define ITEP_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define ITEP_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define ITEP_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define ITEP_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define ITEP_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define ITEP_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define ITEP_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define ITEP_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define ITEP_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define ITEP_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define ITEP_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define ITEP_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define ITEP_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define ITEP_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define ITEP_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define ITEP_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define ITEP_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup ITEP_IA_BANDWIDTH_6 Register ITEP_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define ITEP_IA_BANDWIDTH_6 0x10530
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ITEP_IA_BANDWIDTH_6 0x13F10530u

//! Register Reset Value
#define ITEP_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define ITEP_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define ITEP_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define ITEP_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define ITEP_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define ITEP_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define ITEP_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define ITEP_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define ITEP_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define ITEP_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define ITEP_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define ITEP_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define ITEP_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define ITEP_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define ITEP_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define ITEP_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define ITEP_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup ITEP_IA_BANDWIDTH_7 Register ITEP_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define ITEP_IA_BANDWIDTH_7 0x10538
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ITEP_IA_BANDWIDTH_7 0x13F10538u

//! Register Reset Value
#define ITEP_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define ITEP_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define ITEP_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define ITEP_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define ITEP_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define ITEP_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define ITEP_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define ITEP_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define ITEP_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define ITEP_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define ITEP_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define ITEP_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define ITEP_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define ITEP_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define ITEP_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define ITEP_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define ITEP_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN06_IA_COMPONENT Register ILN06_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define ILN06_IA_COMPONENT 0x10800
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ILN06_IA_COMPONENT 0x13F10800u

//! Register Reset Value
#define ILN06_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define ILN06_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define ILN06_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define ILN06_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define ILN06_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup ILN06_IA_CORE Register ILN06_IA_CORE - core
//! @{

//! Register Offset (relative)
#define ILN06_IA_CORE 0x10818
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ILN06_IA_CORE 0x13F10818u

//! Register Reset Value
#define ILN06_IA_CORE_RST 0x000050C50F600001u

//! Field REV_CODE - rev_code
#define ILN06_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define ILN06_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define ILN06_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define ILN06_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define ILN06_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define ILN06_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup ILN06_IA_AGENT_CONTROL Register ILN06_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define ILN06_IA_AGENT_CONTROL 0x10820
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ILN06_IA_AGENT_CONTROL 0x13F10820u

//! Register Reset Value
#define ILN06_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define ILN06_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define ILN06_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define ILN06_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define ILN06_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define ILN06_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define ILN06_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define ILN06_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define ILN06_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define ILN06_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define ILN06_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define ILN06_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define ILN06_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define ILN06_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define ILN06_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define ILN06_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define ILN06_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define ILN06_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define ILN06_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define ILN06_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define ILN06_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define ILN06_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define ILN06_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup ILN06_IA_AGENT_STATUS Register ILN06_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define ILN06_IA_AGENT_STATUS 0x10828
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ILN06_IA_AGENT_STATUS 0x13F10828u

//! Register Reset Value
#define ILN06_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define ILN06_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define ILN06_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define ILN06_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define ILN06_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define ILN06_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define ILN06_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define ILN06_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define ILN06_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define ILN06_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define ILN06_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define ILN06_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define ILN06_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define ILN06_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define ILN06_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define ILN06_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define ILN06_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define ILN06_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define ILN06_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define ILN06_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define ILN06_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define ILN06_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define ILN06_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup ILN06_IA_ERROR_LOG Register ILN06_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define ILN06_IA_ERROR_LOG 0x10858
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ILN06_IA_ERROR_LOG 0x13F10858u

//! Register Reset Value
#define ILN06_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define ILN06_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define ILN06_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define ILN06_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define ILN06_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define ILN06_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define ILN06_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define ILN06_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define ILN06_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define ILN06_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define ILN06_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define ILN06_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define ILN06_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define ILN06_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define ILN06_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define ILN06_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define ILN06_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define ILN06_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define ILN06_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup ILN06_IA_ERROR_LOG_ADDR Register ILN06_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define ILN06_IA_ERROR_LOG_ADDR 0x10860
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ILN06_IA_ERROR_LOG_ADDR 0x13F10860u

//! Register Reset Value
#define ILN06_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define ILN06_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define ILN06_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup ILN06_IA_CORE_FLAG Register ILN06_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define ILN06_IA_CORE_FLAG 0x10868
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ILN06_IA_CORE_FLAG 0x13F10868u

//! Register Reset Value
#define ILN06_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define ILN06_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define ILN06_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define ILN06_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define ILN06_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define ILN06_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define ILN06_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define ILN06_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define ILN06_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup ILN06_IA_ADDR_FILL_IN Register ILN06_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define ILN06_IA_ADDR_FILL_IN 0x10870
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ILN06_IA_ADDR_FILL_IN 0x13F10870u

//! Register Reset Value
#define ILN06_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define ILN06_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define ILN06_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup ILN06_IA_BANDWIDTH_0 Register ILN06_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define ILN06_IA_BANDWIDTH_0 0x10900
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ILN06_IA_BANDWIDTH_0 0x13F10900u

//! Register Reset Value
#define ILN06_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define ILN06_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define ILN06_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define ILN06_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define ILN06_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define ILN06_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define ILN06_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define ILN06_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define ILN06_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define ILN06_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define ILN06_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define ILN06_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define ILN06_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define ILN06_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define ILN06_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define ILN06_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define ILN06_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN06_IA_BANDWIDTH_1 Register ILN06_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define ILN06_IA_BANDWIDTH_1 0x10908
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ILN06_IA_BANDWIDTH_1 0x13F10908u

//! Register Reset Value
#define ILN06_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define ILN06_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define ILN06_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define ILN06_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define ILN06_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define ILN06_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define ILN06_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define ILN06_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define ILN06_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define ILN06_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define ILN06_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define ILN06_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define ILN06_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define ILN06_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define ILN06_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define ILN06_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define ILN06_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN06_IA_BANDWIDTH_2 Register ILN06_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define ILN06_IA_BANDWIDTH_2 0x10910
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ILN06_IA_BANDWIDTH_2 0x13F10910u

//! Register Reset Value
#define ILN06_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define ILN06_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define ILN06_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define ILN06_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define ILN06_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define ILN06_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define ILN06_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define ILN06_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define ILN06_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define ILN06_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define ILN06_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define ILN06_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define ILN06_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define ILN06_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define ILN06_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define ILN06_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define ILN06_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN06_IA_BANDWIDTH_3 Register ILN06_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define ILN06_IA_BANDWIDTH_3 0x10918
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ILN06_IA_BANDWIDTH_3 0x13F10918u

//! Register Reset Value
#define ILN06_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define ILN06_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define ILN06_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define ILN06_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define ILN06_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define ILN06_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define ILN06_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define ILN06_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define ILN06_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define ILN06_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define ILN06_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define ILN06_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define ILN06_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define ILN06_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define ILN06_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define ILN06_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define ILN06_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN06_IA_BANDWIDTH_4 Register ILN06_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define ILN06_IA_BANDWIDTH_4 0x10920
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ILN06_IA_BANDWIDTH_4 0x13F10920u

//! Register Reset Value
#define ILN06_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define ILN06_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define ILN06_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define ILN06_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define ILN06_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define ILN06_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define ILN06_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define ILN06_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define ILN06_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define ILN06_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define ILN06_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define ILN06_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define ILN06_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define ILN06_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define ILN06_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define ILN06_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define ILN06_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN06_IA_BANDWIDTH_5 Register ILN06_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define ILN06_IA_BANDWIDTH_5 0x10928
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ILN06_IA_BANDWIDTH_5 0x13F10928u

//! Register Reset Value
#define ILN06_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define ILN06_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define ILN06_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define ILN06_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define ILN06_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define ILN06_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define ILN06_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define ILN06_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define ILN06_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define ILN06_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define ILN06_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define ILN06_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define ILN06_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define ILN06_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define ILN06_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define ILN06_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define ILN06_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN06_IA_BANDWIDTH_6 Register ILN06_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define ILN06_IA_BANDWIDTH_6 0x10930
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ILN06_IA_BANDWIDTH_6 0x13F10930u

//! Register Reset Value
#define ILN06_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define ILN06_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define ILN06_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define ILN06_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define ILN06_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define ILN06_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define ILN06_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define ILN06_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define ILN06_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define ILN06_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define ILN06_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define ILN06_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define ILN06_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define ILN06_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define ILN06_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define ILN06_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define ILN06_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN06_IA_BANDWIDTH_7 Register ILN06_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define ILN06_IA_BANDWIDTH_7 0x10938
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_ILN06_IA_BANDWIDTH_7 0x13F10938u

//! Register Reset Value
#define ILN06_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define ILN06_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define ILN06_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define ILN06_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define ILN06_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define ILN06_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define ILN06_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define ILN06_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define ILN06_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define ILN06_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define ILN06_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define ILN06_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define ILN06_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define ILN06_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define ILN06_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define ILN06_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define ILN06_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup TREG6_PM_ERROR_LOG Register TREG6_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TREG6_PM_ERROR_LOG 0x80020
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ERROR_LOG 0x13F80020u

//! Register Reset Value
#define TREG6_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TREG6_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TREG6_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TREG6_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TREG6_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TREG6_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TREG6_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TREG6_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TREG6_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TREG6_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TREG6_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TREG6_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TREG6_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TREG6_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TREG6_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TREG6_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TREG6_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TREG6_PM_CONTROL Register TREG6_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TREG6_PM_CONTROL 0x80028
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_CONTROL 0x13F80028u

//! Register Reset Value
#define TREG6_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TREG6_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TREG6_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TREG6_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TREG6_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TREG6_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TREG6_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TREG6_PM_ERROR_CLEAR_SINGLE Register TREG6_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TREG6_PM_ERROR_CLEAR_SINGLE 0x80030
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ERROR_CLEAR_SINGLE 0x13F80030u

//! Register Reset Value
#define TREG6_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TREG6_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TREG6_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TREG6_PM_ERROR_CLEAR_MULTI Register TREG6_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TREG6_PM_ERROR_CLEAR_MULTI 0x80038
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ERROR_CLEAR_MULTI 0x13F80038u

//! Register Reset Value
#define TREG6_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TREG6_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TREG6_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TREG6_PM_REQ_INFO_PERMISSION_0 Register TREG6_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TREG6_PM_REQ_INFO_PERMISSION_0 0x80048
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_REQ_INFO_PERMISSION_0 0x13F80048u

//! Register Reset Value
#define TREG6_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_READ_PERMISSION_0 Register TREG6_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TREG6_PM_READ_PERMISSION_0 0x80050
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_READ_PERMISSION_0 0x13F80050u

//! Register Reset Value
#define TREG6_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TREG6_PM_WRITE_PERMISSION_0 Register TREG6_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TREG6_PM_WRITE_PERMISSION_0 0x80058
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_WRITE_PERMISSION_0 0x13F80058u

//! Register Reset Value
#define TREG6_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TREG6_PM_ADDR_MATCH_1 Register TREG6_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TREG6_PM_ADDR_MATCH_1 0x80060
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ADDR_MATCH_1 0x13F80060u

//! Register Reset Value
#define TREG6_PM_ADDR_MATCH_1_RST 0x0000000000080050u

//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG6_PM_REQ_INFO_PERMISSION_1 Register TREG6_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TREG6_PM_REQ_INFO_PERMISSION_1 0x80068
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_REQ_INFO_PERMISSION_1 0x13F80068u

//! Register Reset Value
#define TREG6_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_READ_PERMISSION_1 Register TREG6_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TREG6_PM_READ_PERMISSION_1 0x80070
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_READ_PERMISSION_1 0x13F80070u

//! Register Reset Value
#define TREG6_PM_READ_PERMISSION_1_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TREG6_PM_WRITE_PERMISSION_1 Register TREG6_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TREG6_PM_WRITE_PERMISSION_1 0x80078
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_WRITE_PERMISSION_1 0x13F80078u

//! Register Reset Value
#define TREG6_PM_WRITE_PERMISSION_1_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TREG6_PM_ADDR_MATCH_2 Register TREG6_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TREG6_PM_ADDR_MATCH_2 0x80080
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ADDR_MATCH_2 0x13F80080u

//! Register Reset Value
#define TREG6_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG6_PM_REQ_INFO_PERMISSION_2 Register TREG6_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TREG6_PM_REQ_INFO_PERMISSION_2 0x80088
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_REQ_INFO_PERMISSION_2 0x13F80088u

//! Register Reset Value
#define TREG6_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_READ_PERMISSION_2 Register TREG6_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TREG6_PM_READ_PERMISSION_2 0x80090
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_READ_PERMISSION_2 0x13F80090u

//! Register Reset Value
#define TREG6_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_WRITE_PERMISSION_2 Register TREG6_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TREG6_PM_WRITE_PERMISSION_2 0x80098
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_WRITE_PERMISSION_2 0x13F80098u

//! Register Reset Value
#define TREG6_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_ADDR_MATCH_3 Register TREG6_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TREG6_PM_ADDR_MATCH_3 0x800A0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ADDR_MATCH_3 0x13F800A0u

//! Register Reset Value
#define TREG6_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG6_PM_REQ_INFO_PERMISSION_3 Register TREG6_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TREG6_PM_REQ_INFO_PERMISSION_3 0x800A8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_REQ_INFO_PERMISSION_3 0x13F800A8u

//! Register Reset Value
#define TREG6_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_READ_PERMISSION_3 Register TREG6_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TREG6_PM_READ_PERMISSION_3 0x800B0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_READ_PERMISSION_3 0x13F800B0u

//! Register Reset Value
#define TREG6_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_WRITE_PERMISSION_3 Register TREG6_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TREG6_PM_WRITE_PERMISSION_3 0x800B8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_WRITE_PERMISSION_3 0x13F800B8u

//! Register Reset Value
#define TREG6_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_ADDR_MATCH_4 Register TREG6_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TREG6_PM_ADDR_MATCH_4 0x800C0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ADDR_MATCH_4 0x13F800C0u

//! Register Reset Value
#define TREG6_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG6_PM_REQ_INFO_PERMISSION_4 Register TREG6_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TREG6_PM_REQ_INFO_PERMISSION_4 0x800C8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_REQ_INFO_PERMISSION_4 0x13F800C8u

//! Register Reset Value
#define TREG6_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_READ_PERMISSION_4 Register TREG6_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TREG6_PM_READ_PERMISSION_4 0x800D0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_READ_PERMISSION_4 0x13F800D0u

//! Register Reset Value
#define TREG6_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_WRITE_PERMISSION_4 Register TREG6_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TREG6_PM_WRITE_PERMISSION_4 0x800D8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_WRITE_PERMISSION_4 0x13F800D8u

//! Register Reset Value
#define TREG6_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_ADDR_MATCH_5 Register TREG6_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TREG6_PM_ADDR_MATCH_5 0x800E0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ADDR_MATCH_5 0x13F800E0u

//! Register Reset Value
#define TREG6_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG6_PM_REQ_INFO_PERMISSION_5 Register TREG6_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TREG6_PM_REQ_INFO_PERMISSION_5 0x800E8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_REQ_INFO_PERMISSION_5 0x13F800E8u

//! Register Reset Value
#define TREG6_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_READ_PERMISSION_5 Register TREG6_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TREG6_PM_READ_PERMISSION_5 0x800F0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_READ_PERMISSION_5 0x13F800F0u

//! Register Reset Value
#define TREG6_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_WRITE_PERMISSION_5 Register TREG6_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TREG6_PM_WRITE_PERMISSION_5 0x800F8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_WRITE_PERMISSION_5 0x13F800F8u

//! Register Reset Value
#define TREG6_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_ADDR_MATCH_6 Register TREG6_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TREG6_PM_ADDR_MATCH_6 0x80100
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ADDR_MATCH_6 0x13F80100u

//! Register Reset Value
#define TREG6_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG6_PM_REQ_INFO_PERMISSION_6 Register TREG6_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TREG6_PM_REQ_INFO_PERMISSION_6 0x80108
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_REQ_INFO_PERMISSION_6 0x13F80108u

//! Register Reset Value
#define TREG6_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_READ_PERMISSION_6 Register TREG6_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TREG6_PM_READ_PERMISSION_6 0x80110
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_READ_PERMISSION_6 0x13F80110u

//! Register Reset Value
#define TREG6_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_WRITE_PERMISSION_6 Register TREG6_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TREG6_PM_WRITE_PERMISSION_6 0x80118
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_WRITE_PERMISSION_6 0x13F80118u

//! Register Reset Value
#define TREG6_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_ADDR_MATCH_7 Register TREG6_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TREG6_PM_ADDR_MATCH_7 0x80120
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ADDR_MATCH_7 0x13F80120u

//! Register Reset Value
#define TREG6_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG6_PM_REQ_INFO_PERMISSION_7 Register TREG6_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TREG6_PM_REQ_INFO_PERMISSION_7 0x80128
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_REQ_INFO_PERMISSION_7 0x13F80128u

//! Register Reset Value
#define TREG6_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_READ_PERMISSION_7 Register TREG6_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TREG6_PM_READ_PERMISSION_7 0x80130
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_READ_PERMISSION_7 0x13F80130u

//! Register Reset Value
#define TREG6_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_WRITE_PERMISSION_7 Register TREG6_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TREG6_PM_WRITE_PERMISSION_7 0x80138
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_WRITE_PERMISSION_7 0x13F80138u

//! Register Reset Value
#define TREG6_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_ADDR_MATCH_8 Register TREG6_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TREG6_PM_ADDR_MATCH_8 0x80140
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ADDR_MATCH_8 0x13F80140u

//! Register Reset Value
#define TREG6_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG6_PM_REQ_INFO_PERMISSION_8 Register TREG6_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TREG6_PM_REQ_INFO_PERMISSION_8 0x80148
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_REQ_INFO_PERMISSION_8 0x13F80148u

//! Register Reset Value
#define TREG6_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_READ_PERMISSION_8 Register TREG6_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TREG6_PM_READ_PERMISSION_8 0x80150
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_READ_PERMISSION_8 0x13F80150u

//! Register Reset Value
#define TREG6_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_WRITE_PERMISSION_8 Register TREG6_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TREG6_PM_WRITE_PERMISSION_8 0x80158
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_WRITE_PERMISSION_8 0x13F80158u

//! Register Reset Value
#define TREG6_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_ADDR_MATCH_9 Register TREG6_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TREG6_PM_ADDR_MATCH_9 0x80160
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ADDR_MATCH_9 0x13F80160u

//! Register Reset Value
#define TREG6_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG6_PM_REQ_INFO_PERMISSION_9 Register TREG6_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TREG6_PM_REQ_INFO_PERMISSION_9 0x80168
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_REQ_INFO_PERMISSION_9 0x13F80168u

//! Register Reset Value
#define TREG6_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_READ_PERMISSION_9 Register TREG6_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TREG6_PM_READ_PERMISSION_9 0x80170
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_READ_PERMISSION_9 0x13F80170u

//! Register Reset Value
#define TREG6_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_WRITE_PERMISSION_9 Register TREG6_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TREG6_PM_WRITE_PERMISSION_9 0x80178
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_WRITE_PERMISSION_9 0x13F80178u

//! Register Reset Value
#define TREG6_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_ADDR_MATCH_10 Register TREG6_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TREG6_PM_ADDR_MATCH_10 0x80180
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ADDR_MATCH_10 0x13F80180u

//! Register Reset Value
#define TREG6_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG6_PM_REQ_INFO_PERMISSION_10 Register TREG6_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TREG6_PM_REQ_INFO_PERMISSION_10 0x80188
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_REQ_INFO_PERMISSION_10 0x13F80188u

//! Register Reset Value
#define TREG6_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_READ_PERMISSION_10 Register TREG6_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TREG6_PM_READ_PERMISSION_10 0x80190
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_READ_PERMISSION_10 0x13F80190u

//! Register Reset Value
#define TREG6_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_WRITE_PERMISSION_10 Register TREG6_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TREG6_PM_WRITE_PERMISSION_10 0x80198
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_WRITE_PERMISSION_10 0x13F80198u

//! Register Reset Value
#define TREG6_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_ADDR_MATCH_11 Register TREG6_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TREG6_PM_ADDR_MATCH_11 0x801A0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ADDR_MATCH_11 0x13F801A0u

//! Register Reset Value
#define TREG6_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG6_PM_REQ_INFO_PERMISSION_11 Register TREG6_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TREG6_PM_REQ_INFO_PERMISSION_11 0x801A8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_REQ_INFO_PERMISSION_11 0x13F801A8u

//! Register Reset Value
#define TREG6_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_READ_PERMISSION_11 Register TREG6_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TREG6_PM_READ_PERMISSION_11 0x801B0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_READ_PERMISSION_11 0x13F801B0u

//! Register Reset Value
#define TREG6_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_WRITE_PERMISSION_11 Register TREG6_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TREG6_PM_WRITE_PERMISSION_11 0x801B8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_WRITE_PERMISSION_11 0x13F801B8u

//! Register Reset Value
#define TREG6_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_ADDR_MATCH_12 Register TREG6_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TREG6_PM_ADDR_MATCH_12 0x801C0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ADDR_MATCH_12 0x13F801C0u

//! Register Reset Value
#define TREG6_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG6_PM_REQ_INFO_PERMISSION_12 Register TREG6_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TREG6_PM_REQ_INFO_PERMISSION_12 0x801C8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_REQ_INFO_PERMISSION_12 0x13F801C8u

//! Register Reset Value
#define TREG6_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_READ_PERMISSION_12 Register TREG6_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TREG6_PM_READ_PERMISSION_12 0x801D0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_READ_PERMISSION_12 0x13F801D0u

//! Register Reset Value
#define TREG6_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_WRITE_PERMISSION_12 Register TREG6_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TREG6_PM_WRITE_PERMISSION_12 0x801D8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_WRITE_PERMISSION_12 0x13F801D8u

//! Register Reset Value
#define TREG6_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_ADDR_MATCH_13 Register TREG6_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TREG6_PM_ADDR_MATCH_13 0x801E0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ADDR_MATCH_13 0x13F801E0u

//! Register Reset Value
#define TREG6_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG6_PM_REQ_INFO_PERMISSION_13 Register TREG6_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TREG6_PM_REQ_INFO_PERMISSION_13 0x801E8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_REQ_INFO_PERMISSION_13 0x13F801E8u

//! Register Reset Value
#define TREG6_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_READ_PERMISSION_13 Register TREG6_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TREG6_PM_READ_PERMISSION_13 0x801F0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_READ_PERMISSION_13 0x13F801F0u

//! Register Reset Value
#define TREG6_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_WRITE_PERMISSION_13 Register TREG6_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TREG6_PM_WRITE_PERMISSION_13 0x801F8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_WRITE_PERMISSION_13 0x13F801F8u

//! Register Reset Value
#define TREG6_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_ADDR_MATCH_14 Register TREG6_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TREG6_PM_ADDR_MATCH_14 0x80200
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ADDR_MATCH_14 0x13F80200u

//! Register Reset Value
#define TREG6_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG6_PM_REQ_INFO_PERMISSION_14 Register TREG6_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TREG6_PM_REQ_INFO_PERMISSION_14 0x80208
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_REQ_INFO_PERMISSION_14 0x13F80208u

//! Register Reset Value
#define TREG6_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_READ_PERMISSION_14 Register TREG6_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TREG6_PM_READ_PERMISSION_14 0x80210
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_READ_PERMISSION_14 0x13F80210u

//! Register Reset Value
#define TREG6_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_WRITE_PERMISSION_14 Register TREG6_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TREG6_PM_WRITE_PERMISSION_14 0x80218
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_WRITE_PERMISSION_14 0x13F80218u

//! Register Reset Value
#define TREG6_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_ADDR_MATCH_15 Register TREG6_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TREG6_PM_ADDR_MATCH_15 0x80220
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_ADDR_MATCH_15 0x13F80220u

//! Register Reset Value
#define TREG6_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG6_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TREG6_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TREG6_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG6_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG6_PM_REQ_INFO_PERMISSION_15 Register TREG6_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TREG6_PM_REQ_INFO_PERMISSION_15 0x80228
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_REQ_INFO_PERMISSION_15 0x13F80228u

//! Register Reset Value
#define TREG6_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG6_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_READ_PERMISSION_15 Register TREG6_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TREG6_PM_READ_PERMISSION_15 0x80230
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_READ_PERMISSION_15 0x13F80230u

//! Register Reset Value
#define TREG6_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG6_PM_WRITE_PERMISSION_15 Register TREG6_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TREG6_PM_WRITE_PERMISSION_15 0x80238
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TREG6_PM_WRITE_PERMISSION_15 0x13F80238u

//! Register Reset Value
#define TREG6_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG6_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_ERROR_LOG Register TIC_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TIC_PM_ERROR_LOG 0x80420
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ERROR_LOG 0x13F80420u

//! Register Reset Value
#define TIC_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TIC_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TIC_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TIC_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TIC_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TIC_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TIC_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TIC_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TIC_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TIC_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TIC_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TIC_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TIC_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TIC_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TIC_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TIC_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TIC_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TIC_PM_CONTROL Register TIC_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TIC_PM_CONTROL 0x80428
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_CONTROL 0x13F80428u

//! Register Reset Value
#define TIC_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TIC_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TIC_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TIC_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TIC_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TIC_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TIC_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TIC_PM_ERROR_CLEAR_SINGLE Register TIC_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TIC_PM_ERROR_CLEAR_SINGLE 0x80430
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ERROR_CLEAR_SINGLE 0x13F80430u

//! Register Reset Value
#define TIC_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TIC_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TIC_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TIC_PM_ERROR_CLEAR_MULTI Register TIC_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TIC_PM_ERROR_CLEAR_MULTI 0x80438
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ERROR_CLEAR_MULTI 0x13F80438u

//! Register Reset Value
#define TIC_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TIC_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TIC_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TIC_PM_REQ_INFO_PERMISSION_0 Register TIC_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TIC_PM_REQ_INFO_PERMISSION_0 0x80448
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_REQ_INFO_PERMISSION_0 0x13F80448u

//! Register Reset Value
#define TIC_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TIC_PM_READ_PERMISSION_0 Register TIC_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TIC_PM_READ_PERMISSION_0 0x80450
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_READ_PERMISSION_0 0x13F80450u

//! Register Reset Value
#define TIC_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x3u

//! @}

//! \defgroup TIC_PM_WRITE_PERMISSION_0 Register TIC_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TIC_PM_WRITE_PERMISSION_0 0x80458
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_WRITE_PERMISSION_0 0x13F80458u

//! Register Reset Value
#define TIC_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x3u

//! @}

//! \defgroup TIC_PM_ADDR_MATCH_1 Register TIC_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TIC_PM_ADDR_MATCH_1 0x80460
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ADDR_MATCH_1 0x13F80460u

//! Register Reset Value
#define TIC_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TIC_PM_REQ_INFO_PERMISSION_1 Register TIC_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TIC_PM_REQ_INFO_PERMISSION_1 0x80468
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_REQ_INFO_PERMISSION_1 0x13F80468u

//! Register Reset Value
#define TIC_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TIC_PM_READ_PERMISSION_1 Register TIC_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TIC_PM_READ_PERMISSION_1 0x80470
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_READ_PERMISSION_1 0x13F80470u

//! Register Reset Value
#define TIC_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_WRITE_PERMISSION_1 Register TIC_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TIC_PM_WRITE_PERMISSION_1 0x80478
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_WRITE_PERMISSION_1 0x13F80478u

//! Register Reset Value
#define TIC_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_ADDR_MATCH_2 Register TIC_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TIC_PM_ADDR_MATCH_2 0x80480
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ADDR_MATCH_2 0x13F80480u

//! Register Reset Value
#define TIC_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TIC_PM_REQ_INFO_PERMISSION_2 Register TIC_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TIC_PM_REQ_INFO_PERMISSION_2 0x80488
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_REQ_INFO_PERMISSION_2 0x13F80488u

//! Register Reset Value
#define TIC_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TIC_PM_READ_PERMISSION_2 Register TIC_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TIC_PM_READ_PERMISSION_2 0x80490
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_READ_PERMISSION_2 0x13F80490u

//! Register Reset Value
#define TIC_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_WRITE_PERMISSION_2 Register TIC_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TIC_PM_WRITE_PERMISSION_2 0x80498
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_WRITE_PERMISSION_2 0x13F80498u

//! Register Reset Value
#define TIC_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_ADDR_MATCH_3 Register TIC_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TIC_PM_ADDR_MATCH_3 0x804A0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ADDR_MATCH_3 0x13F804A0u

//! Register Reset Value
#define TIC_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TIC_PM_REQ_INFO_PERMISSION_3 Register TIC_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TIC_PM_REQ_INFO_PERMISSION_3 0x804A8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_REQ_INFO_PERMISSION_3 0x13F804A8u

//! Register Reset Value
#define TIC_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TIC_PM_READ_PERMISSION_3 Register TIC_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TIC_PM_READ_PERMISSION_3 0x804B0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_READ_PERMISSION_3 0x13F804B0u

//! Register Reset Value
#define TIC_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_WRITE_PERMISSION_3 Register TIC_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TIC_PM_WRITE_PERMISSION_3 0x804B8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_WRITE_PERMISSION_3 0x13F804B8u

//! Register Reset Value
#define TIC_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_ADDR_MATCH_4 Register TIC_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TIC_PM_ADDR_MATCH_4 0x804C0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ADDR_MATCH_4 0x13F804C0u

//! Register Reset Value
#define TIC_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TIC_PM_REQ_INFO_PERMISSION_4 Register TIC_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TIC_PM_REQ_INFO_PERMISSION_4 0x804C8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_REQ_INFO_PERMISSION_4 0x13F804C8u

//! Register Reset Value
#define TIC_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TIC_PM_READ_PERMISSION_4 Register TIC_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TIC_PM_READ_PERMISSION_4 0x804D0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_READ_PERMISSION_4 0x13F804D0u

//! Register Reset Value
#define TIC_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_WRITE_PERMISSION_4 Register TIC_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TIC_PM_WRITE_PERMISSION_4 0x804D8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_WRITE_PERMISSION_4 0x13F804D8u

//! Register Reset Value
#define TIC_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_ADDR_MATCH_5 Register TIC_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TIC_PM_ADDR_MATCH_5 0x804E0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ADDR_MATCH_5 0x13F804E0u

//! Register Reset Value
#define TIC_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TIC_PM_REQ_INFO_PERMISSION_5 Register TIC_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TIC_PM_REQ_INFO_PERMISSION_5 0x804E8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_REQ_INFO_PERMISSION_5 0x13F804E8u

//! Register Reset Value
#define TIC_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TIC_PM_READ_PERMISSION_5 Register TIC_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TIC_PM_READ_PERMISSION_5 0x804F0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_READ_PERMISSION_5 0x13F804F0u

//! Register Reset Value
#define TIC_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_WRITE_PERMISSION_5 Register TIC_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TIC_PM_WRITE_PERMISSION_5 0x804F8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_WRITE_PERMISSION_5 0x13F804F8u

//! Register Reset Value
#define TIC_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_ADDR_MATCH_6 Register TIC_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TIC_PM_ADDR_MATCH_6 0x80500
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ADDR_MATCH_6 0x13F80500u

//! Register Reset Value
#define TIC_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TIC_PM_REQ_INFO_PERMISSION_6 Register TIC_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TIC_PM_REQ_INFO_PERMISSION_6 0x80508
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_REQ_INFO_PERMISSION_6 0x13F80508u

//! Register Reset Value
#define TIC_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TIC_PM_READ_PERMISSION_6 Register TIC_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TIC_PM_READ_PERMISSION_6 0x80510
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_READ_PERMISSION_6 0x13F80510u

//! Register Reset Value
#define TIC_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_WRITE_PERMISSION_6 Register TIC_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TIC_PM_WRITE_PERMISSION_6 0x80518
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_WRITE_PERMISSION_6 0x13F80518u

//! Register Reset Value
#define TIC_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_ADDR_MATCH_7 Register TIC_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TIC_PM_ADDR_MATCH_7 0x80520
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ADDR_MATCH_7 0x13F80520u

//! Register Reset Value
#define TIC_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TIC_PM_REQ_INFO_PERMISSION_7 Register TIC_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TIC_PM_REQ_INFO_PERMISSION_7 0x80528
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_REQ_INFO_PERMISSION_7 0x13F80528u

//! Register Reset Value
#define TIC_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TIC_PM_READ_PERMISSION_7 Register TIC_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TIC_PM_READ_PERMISSION_7 0x80530
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_READ_PERMISSION_7 0x13F80530u

//! Register Reset Value
#define TIC_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_WRITE_PERMISSION_7 Register TIC_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TIC_PM_WRITE_PERMISSION_7 0x80538
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_WRITE_PERMISSION_7 0x13F80538u

//! Register Reset Value
#define TIC_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_ADDR_MATCH_8 Register TIC_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TIC_PM_ADDR_MATCH_8 0x80540
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ADDR_MATCH_8 0x13F80540u

//! Register Reset Value
#define TIC_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TIC_PM_REQ_INFO_PERMISSION_8 Register TIC_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TIC_PM_REQ_INFO_PERMISSION_8 0x80548
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_REQ_INFO_PERMISSION_8 0x13F80548u

//! Register Reset Value
#define TIC_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TIC_PM_READ_PERMISSION_8 Register TIC_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TIC_PM_READ_PERMISSION_8 0x80550
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_READ_PERMISSION_8 0x13F80550u

//! Register Reset Value
#define TIC_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_WRITE_PERMISSION_8 Register TIC_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TIC_PM_WRITE_PERMISSION_8 0x80558
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_WRITE_PERMISSION_8 0x13F80558u

//! Register Reset Value
#define TIC_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_ADDR_MATCH_9 Register TIC_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TIC_PM_ADDR_MATCH_9 0x80560
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ADDR_MATCH_9 0x13F80560u

//! Register Reset Value
#define TIC_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TIC_PM_REQ_INFO_PERMISSION_9 Register TIC_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TIC_PM_REQ_INFO_PERMISSION_9 0x80568
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_REQ_INFO_PERMISSION_9 0x13F80568u

//! Register Reset Value
#define TIC_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TIC_PM_READ_PERMISSION_9 Register TIC_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TIC_PM_READ_PERMISSION_9 0x80570
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_READ_PERMISSION_9 0x13F80570u

//! Register Reset Value
#define TIC_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_WRITE_PERMISSION_9 Register TIC_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TIC_PM_WRITE_PERMISSION_9 0x80578
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_WRITE_PERMISSION_9 0x13F80578u

//! Register Reset Value
#define TIC_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_ADDR_MATCH_10 Register TIC_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TIC_PM_ADDR_MATCH_10 0x80580
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ADDR_MATCH_10 0x13F80580u

//! Register Reset Value
#define TIC_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TIC_PM_REQ_INFO_PERMISSION_10 Register TIC_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TIC_PM_REQ_INFO_PERMISSION_10 0x80588
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_REQ_INFO_PERMISSION_10 0x13F80588u

//! Register Reset Value
#define TIC_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TIC_PM_READ_PERMISSION_10 Register TIC_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TIC_PM_READ_PERMISSION_10 0x80590
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_READ_PERMISSION_10 0x13F80590u

//! Register Reset Value
#define TIC_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_WRITE_PERMISSION_10 Register TIC_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TIC_PM_WRITE_PERMISSION_10 0x80598
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_WRITE_PERMISSION_10 0x13F80598u

//! Register Reset Value
#define TIC_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_ADDR_MATCH_11 Register TIC_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TIC_PM_ADDR_MATCH_11 0x805A0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ADDR_MATCH_11 0x13F805A0u

//! Register Reset Value
#define TIC_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TIC_PM_REQ_INFO_PERMISSION_11 Register TIC_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TIC_PM_REQ_INFO_PERMISSION_11 0x805A8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_REQ_INFO_PERMISSION_11 0x13F805A8u

//! Register Reset Value
#define TIC_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TIC_PM_READ_PERMISSION_11 Register TIC_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TIC_PM_READ_PERMISSION_11 0x805B0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_READ_PERMISSION_11 0x13F805B0u

//! Register Reset Value
#define TIC_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_WRITE_PERMISSION_11 Register TIC_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TIC_PM_WRITE_PERMISSION_11 0x805B8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_WRITE_PERMISSION_11 0x13F805B8u

//! Register Reset Value
#define TIC_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_ADDR_MATCH_12 Register TIC_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TIC_PM_ADDR_MATCH_12 0x805C0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ADDR_MATCH_12 0x13F805C0u

//! Register Reset Value
#define TIC_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TIC_PM_REQ_INFO_PERMISSION_12 Register TIC_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TIC_PM_REQ_INFO_PERMISSION_12 0x805C8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_REQ_INFO_PERMISSION_12 0x13F805C8u

//! Register Reset Value
#define TIC_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TIC_PM_READ_PERMISSION_12 Register TIC_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TIC_PM_READ_PERMISSION_12 0x805D0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_READ_PERMISSION_12 0x13F805D0u

//! Register Reset Value
#define TIC_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_WRITE_PERMISSION_12 Register TIC_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TIC_PM_WRITE_PERMISSION_12 0x805D8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_WRITE_PERMISSION_12 0x13F805D8u

//! Register Reset Value
#define TIC_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_ADDR_MATCH_13 Register TIC_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TIC_PM_ADDR_MATCH_13 0x805E0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ADDR_MATCH_13 0x13F805E0u

//! Register Reset Value
#define TIC_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TIC_PM_REQ_INFO_PERMISSION_13 Register TIC_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TIC_PM_REQ_INFO_PERMISSION_13 0x805E8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_REQ_INFO_PERMISSION_13 0x13F805E8u

//! Register Reset Value
#define TIC_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TIC_PM_READ_PERMISSION_13 Register TIC_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TIC_PM_READ_PERMISSION_13 0x805F0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_READ_PERMISSION_13 0x13F805F0u

//! Register Reset Value
#define TIC_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_WRITE_PERMISSION_13 Register TIC_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TIC_PM_WRITE_PERMISSION_13 0x805F8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_WRITE_PERMISSION_13 0x13F805F8u

//! Register Reset Value
#define TIC_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_ADDR_MATCH_14 Register TIC_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TIC_PM_ADDR_MATCH_14 0x80600
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ADDR_MATCH_14 0x13F80600u

//! Register Reset Value
#define TIC_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TIC_PM_REQ_INFO_PERMISSION_14 Register TIC_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TIC_PM_REQ_INFO_PERMISSION_14 0x80608
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_REQ_INFO_PERMISSION_14 0x13F80608u

//! Register Reset Value
#define TIC_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TIC_PM_READ_PERMISSION_14 Register TIC_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TIC_PM_READ_PERMISSION_14 0x80610
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_READ_PERMISSION_14 0x13F80610u

//! Register Reset Value
#define TIC_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_WRITE_PERMISSION_14 Register TIC_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TIC_PM_WRITE_PERMISSION_14 0x80618
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_WRITE_PERMISSION_14 0x13F80618u

//! Register Reset Value
#define TIC_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_ADDR_MATCH_15 Register TIC_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TIC_PM_ADDR_MATCH_15 0x80620
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_ADDR_MATCH_15 0x13F80620u

//! Register Reset Value
#define TIC_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TIC_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TIC_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TIC_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TIC_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TIC_PM_REQ_INFO_PERMISSION_15 Register TIC_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TIC_PM_REQ_INFO_PERMISSION_15 0x80628
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_REQ_INFO_PERMISSION_15 0x13F80628u

//! Register Reset Value
#define TIC_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TIC_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TIC_PM_READ_PERMISSION_15 Register TIC_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TIC_PM_READ_PERMISSION_15 0x80630
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_READ_PERMISSION_15 0x13F80630u

//! Register Reset Value
#define TIC_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TIC_PM_WRITE_PERMISSION_15 Register TIC_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TIC_PM_WRITE_PERMISSION_15 0x80638
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TIC_PM_WRITE_PERMISSION_15 0x13F80638u

//! Register Reset Value
#define TIC_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TIC_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_ERROR_LOG Register TDDR_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TDDR_PM_ERROR_LOG 0x80820
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ERROR_LOG 0x13F80820u

//! Register Reset Value
#define TDDR_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TDDR_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TDDR_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TDDR_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TDDR_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TDDR_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TDDR_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TDDR_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TDDR_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TDDR_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TDDR_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TDDR_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TDDR_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TDDR_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TDDR_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TDDR_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TDDR_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TDDR_PM_CONTROL Register TDDR_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TDDR_PM_CONTROL 0x80828
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_CONTROL 0x13F80828u

//! Register Reset Value
#define TDDR_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TDDR_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TDDR_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TDDR_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TDDR_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TDDR_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TDDR_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TDDR_PM_ERROR_CLEAR_SINGLE Register TDDR_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TDDR_PM_ERROR_CLEAR_SINGLE 0x80830
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ERROR_CLEAR_SINGLE 0x13F80830u

//! Register Reset Value
#define TDDR_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TDDR_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TDDR_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TDDR_PM_ERROR_CLEAR_MULTI Register TDDR_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TDDR_PM_ERROR_CLEAR_MULTI 0x80838
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ERROR_CLEAR_MULTI 0x13F80838u

//! Register Reset Value
#define TDDR_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TDDR_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TDDR_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TDDR_PM_REQ_INFO_PERMISSION_0 Register TDDR_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TDDR_PM_REQ_INFO_PERMISSION_0 0x80848
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_REQ_INFO_PERMISSION_0 0x13F80848u

//! Register Reset Value
#define TDDR_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_READ_PERMISSION_0 Register TDDR_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TDDR_PM_READ_PERMISSION_0 0x80850
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_READ_PERMISSION_0 0x13F80850u

//! Register Reset Value
#define TDDR_PM_READ_PERMISSION_0_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TDDR_PM_WRITE_PERMISSION_0 Register TDDR_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TDDR_PM_WRITE_PERMISSION_0 0x80858
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_WRITE_PERMISSION_0 0x13F80858u

//! Register Reset Value
#define TDDR_PM_WRITE_PERMISSION_0_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TDDR_PM_ADDR_MATCH_1 Register TDDR_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TDDR_PM_ADDR_MATCH_1 0x80860
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ADDR_MATCH_1 0x13F80860u

//! Register Reset Value
#define TDDR_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDDR_PM_REQ_INFO_PERMISSION_1 Register TDDR_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TDDR_PM_REQ_INFO_PERMISSION_1 0x80868
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_REQ_INFO_PERMISSION_1 0x13F80868u

//! Register Reset Value
#define TDDR_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_READ_PERMISSION_1 Register TDDR_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TDDR_PM_READ_PERMISSION_1 0x80870
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_READ_PERMISSION_1 0x13F80870u

//! Register Reset Value
#define TDDR_PM_READ_PERMISSION_1_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TDDR_PM_WRITE_PERMISSION_1 Register TDDR_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TDDR_PM_WRITE_PERMISSION_1 0x80878
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_WRITE_PERMISSION_1 0x13F80878u

//! Register Reset Value
#define TDDR_PM_WRITE_PERMISSION_1_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TDDR_PM_ADDR_MATCH_2 Register TDDR_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TDDR_PM_ADDR_MATCH_2 0x80880
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ADDR_MATCH_2 0x13F80880u

//! Register Reset Value
#define TDDR_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDDR_PM_REQ_INFO_PERMISSION_2 Register TDDR_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TDDR_PM_REQ_INFO_PERMISSION_2 0x80888
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_REQ_INFO_PERMISSION_2 0x13F80888u

//! Register Reset Value
#define TDDR_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_READ_PERMISSION_2 Register TDDR_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TDDR_PM_READ_PERMISSION_2 0x80890
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_READ_PERMISSION_2 0x13F80890u

//! Register Reset Value
#define TDDR_PM_READ_PERMISSION_2_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TDDR_PM_WRITE_PERMISSION_2 Register TDDR_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TDDR_PM_WRITE_PERMISSION_2 0x80898
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_WRITE_PERMISSION_2 0x13F80898u

//! Register Reset Value
#define TDDR_PM_WRITE_PERMISSION_2_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TDDR_PM_ADDR_MATCH_3 Register TDDR_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TDDR_PM_ADDR_MATCH_3 0x808A0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ADDR_MATCH_3 0x13F808A0u

//! Register Reset Value
#define TDDR_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDDR_PM_REQ_INFO_PERMISSION_3 Register TDDR_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TDDR_PM_REQ_INFO_PERMISSION_3 0x808A8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_REQ_INFO_PERMISSION_3 0x13F808A8u

//! Register Reset Value
#define TDDR_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_READ_PERMISSION_3 Register TDDR_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TDDR_PM_READ_PERMISSION_3 0x808B0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_READ_PERMISSION_3 0x13F808B0u

//! Register Reset Value
#define TDDR_PM_READ_PERMISSION_3_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TDDR_PM_WRITE_PERMISSION_3 Register TDDR_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TDDR_PM_WRITE_PERMISSION_3 0x808B8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_WRITE_PERMISSION_3 0x13F808B8u

//! Register Reset Value
#define TDDR_PM_WRITE_PERMISSION_3_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TDDR_PM_ADDR_MATCH_4 Register TDDR_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TDDR_PM_ADDR_MATCH_4 0x808C0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ADDR_MATCH_4 0x13F808C0u

//! Register Reset Value
#define TDDR_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDDR_PM_REQ_INFO_PERMISSION_4 Register TDDR_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TDDR_PM_REQ_INFO_PERMISSION_4 0x808C8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_REQ_INFO_PERMISSION_4 0x13F808C8u

//! Register Reset Value
#define TDDR_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_READ_PERMISSION_4 Register TDDR_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TDDR_PM_READ_PERMISSION_4 0x808D0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_READ_PERMISSION_4 0x13F808D0u

//! Register Reset Value
#define TDDR_PM_READ_PERMISSION_4_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TDDR_PM_WRITE_PERMISSION_4 Register TDDR_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TDDR_PM_WRITE_PERMISSION_4 0x808D8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_WRITE_PERMISSION_4 0x13F808D8u

//! Register Reset Value
#define TDDR_PM_WRITE_PERMISSION_4_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TDDR_PM_ADDR_MATCH_5 Register TDDR_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TDDR_PM_ADDR_MATCH_5 0x808E0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ADDR_MATCH_5 0x13F808E0u

//! Register Reset Value
#define TDDR_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDDR_PM_REQ_INFO_PERMISSION_5 Register TDDR_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TDDR_PM_REQ_INFO_PERMISSION_5 0x808E8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_REQ_INFO_PERMISSION_5 0x13F808E8u

//! Register Reset Value
#define TDDR_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_READ_PERMISSION_5 Register TDDR_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TDDR_PM_READ_PERMISSION_5 0x808F0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_READ_PERMISSION_5 0x13F808F0u

//! Register Reset Value
#define TDDR_PM_READ_PERMISSION_5_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TDDR_PM_WRITE_PERMISSION_5 Register TDDR_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TDDR_PM_WRITE_PERMISSION_5 0x808F8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_WRITE_PERMISSION_5 0x13F808F8u

//! Register Reset Value
#define TDDR_PM_WRITE_PERMISSION_5_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TDDR_PM_ADDR_MATCH_6 Register TDDR_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TDDR_PM_ADDR_MATCH_6 0x80900
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ADDR_MATCH_6 0x13F80900u

//! Register Reset Value
#define TDDR_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDDR_PM_REQ_INFO_PERMISSION_6 Register TDDR_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TDDR_PM_REQ_INFO_PERMISSION_6 0x80908
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_REQ_INFO_PERMISSION_6 0x13F80908u

//! Register Reset Value
#define TDDR_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_READ_PERMISSION_6 Register TDDR_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TDDR_PM_READ_PERMISSION_6 0x80910
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_READ_PERMISSION_6 0x13F80910u

//! Register Reset Value
#define TDDR_PM_READ_PERMISSION_6_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TDDR_PM_WRITE_PERMISSION_6 Register TDDR_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TDDR_PM_WRITE_PERMISSION_6 0x80918
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_WRITE_PERMISSION_6 0x13F80918u

//! Register Reset Value
#define TDDR_PM_WRITE_PERMISSION_6_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TDDR_PM_ADDR_MATCH_7 Register TDDR_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TDDR_PM_ADDR_MATCH_7 0x80920
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ADDR_MATCH_7 0x13F80920u

//! Register Reset Value
#define TDDR_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDDR_PM_REQ_INFO_PERMISSION_7 Register TDDR_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TDDR_PM_REQ_INFO_PERMISSION_7 0x80928
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_REQ_INFO_PERMISSION_7 0x13F80928u

//! Register Reset Value
#define TDDR_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_READ_PERMISSION_7 Register TDDR_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TDDR_PM_READ_PERMISSION_7 0x80930
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_READ_PERMISSION_7 0x13F80930u

//! Register Reset Value
#define TDDR_PM_READ_PERMISSION_7_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TDDR_PM_WRITE_PERMISSION_7 Register TDDR_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TDDR_PM_WRITE_PERMISSION_7 0x80938
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_WRITE_PERMISSION_7 0x13F80938u

//! Register Reset Value
#define TDDR_PM_WRITE_PERMISSION_7_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0x3Fu

//! @}

//! \defgroup TDDR_PM_ADDR_MATCH_8 Register TDDR_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TDDR_PM_ADDR_MATCH_8 0x80940
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ADDR_MATCH_8 0x13F80940u

//! Register Reset Value
#define TDDR_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDDR_PM_REQ_INFO_PERMISSION_8 Register TDDR_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TDDR_PM_REQ_INFO_PERMISSION_8 0x80948
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_REQ_INFO_PERMISSION_8 0x13F80948u

//! Register Reset Value
#define TDDR_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_READ_PERMISSION_8 Register TDDR_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TDDR_PM_READ_PERMISSION_8 0x80950
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_READ_PERMISSION_8 0x13F80950u

//! Register Reset Value
#define TDDR_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_WRITE_PERMISSION_8 Register TDDR_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TDDR_PM_WRITE_PERMISSION_8 0x80958
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_WRITE_PERMISSION_8 0x13F80958u

//! Register Reset Value
#define TDDR_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_ADDR_MATCH_9 Register TDDR_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TDDR_PM_ADDR_MATCH_9 0x80960
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ADDR_MATCH_9 0x13F80960u

//! Register Reset Value
#define TDDR_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDDR_PM_REQ_INFO_PERMISSION_9 Register TDDR_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TDDR_PM_REQ_INFO_PERMISSION_9 0x80968
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_REQ_INFO_PERMISSION_9 0x13F80968u

//! Register Reset Value
#define TDDR_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_READ_PERMISSION_9 Register TDDR_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TDDR_PM_READ_PERMISSION_9 0x80970
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_READ_PERMISSION_9 0x13F80970u

//! Register Reset Value
#define TDDR_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_WRITE_PERMISSION_9 Register TDDR_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TDDR_PM_WRITE_PERMISSION_9 0x80978
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_WRITE_PERMISSION_9 0x13F80978u

//! Register Reset Value
#define TDDR_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_ADDR_MATCH_10 Register TDDR_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TDDR_PM_ADDR_MATCH_10 0x80980
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ADDR_MATCH_10 0x13F80980u

//! Register Reset Value
#define TDDR_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDDR_PM_REQ_INFO_PERMISSION_10 Register TDDR_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TDDR_PM_REQ_INFO_PERMISSION_10 0x80988
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_REQ_INFO_PERMISSION_10 0x13F80988u

//! Register Reset Value
#define TDDR_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_READ_PERMISSION_10 Register TDDR_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TDDR_PM_READ_PERMISSION_10 0x80990
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_READ_PERMISSION_10 0x13F80990u

//! Register Reset Value
#define TDDR_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_WRITE_PERMISSION_10 Register TDDR_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TDDR_PM_WRITE_PERMISSION_10 0x80998
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_WRITE_PERMISSION_10 0x13F80998u

//! Register Reset Value
#define TDDR_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_ADDR_MATCH_11 Register TDDR_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TDDR_PM_ADDR_MATCH_11 0x809A0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ADDR_MATCH_11 0x13F809A0u

//! Register Reset Value
#define TDDR_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDDR_PM_REQ_INFO_PERMISSION_11 Register TDDR_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TDDR_PM_REQ_INFO_PERMISSION_11 0x809A8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_REQ_INFO_PERMISSION_11 0x13F809A8u

//! Register Reset Value
#define TDDR_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_READ_PERMISSION_11 Register TDDR_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TDDR_PM_READ_PERMISSION_11 0x809B0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_READ_PERMISSION_11 0x13F809B0u

//! Register Reset Value
#define TDDR_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_WRITE_PERMISSION_11 Register TDDR_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TDDR_PM_WRITE_PERMISSION_11 0x809B8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_WRITE_PERMISSION_11 0x13F809B8u

//! Register Reset Value
#define TDDR_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_ADDR_MATCH_12 Register TDDR_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TDDR_PM_ADDR_MATCH_12 0x809C0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ADDR_MATCH_12 0x13F809C0u

//! Register Reset Value
#define TDDR_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDDR_PM_REQ_INFO_PERMISSION_12 Register TDDR_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TDDR_PM_REQ_INFO_PERMISSION_12 0x809C8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_REQ_INFO_PERMISSION_12 0x13F809C8u

//! Register Reset Value
#define TDDR_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_READ_PERMISSION_12 Register TDDR_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TDDR_PM_READ_PERMISSION_12 0x809D0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_READ_PERMISSION_12 0x13F809D0u

//! Register Reset Value
#define TDDR_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_WRITE_PERMISSION_12 Register TDDR_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TDDR_PM_WRITE_PERMISSION_12 0x809D8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_WRITE_PERMISSION_12 0x13F809D8u

//! Register Reset Value
#define TDDR_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_ADDR_MATCH_13 Register TDDR_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TDDR_PM_ADDR_MATCH_13 0x809E0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ADDR_MATCH_13 0x13F809E0u

//! Register Reset Value
#define TDDR_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDDR_PM_REQ_INFO_PERMISSION_13 Register TDDR_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TDDR_PM_REQ_INFO_PERMISSION_13 0x809E8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_REQ_INFO_PERMISSION_13 0x13F809E8u

//! Register Reset Value
#define TDDR_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_READ_PERMISSION_13 Register TDDR_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TDDR_PM_READ_PERMISSION_13 0x809F0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_READ_PERMISSION_13 0x13F809F0u

//! Register Reset Value
#define TDDR_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_WRITE_PERMISSION_13 Register TDDR_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TDDR_PM_WRITE_PERMISSION_13 0x809F8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_WRITE_PERMISSION_13 0x13F809F8u

//! Register Reset Value
#define TDDR_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_ADDR_MATCH_14 Register TDDR_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TDDR_PM_ADDR_MATCH_14 0x80A00
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ADDR_MATCH_14 0x13F80A00u

//! Register Reset Value
#define TDDR_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDDR_PM_REQ_INFO_PERMISSION_14 Register TDDR_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TDDR_PM_REQ_INFO_PERMISSION_14 0x80A08
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_REQ_INFO_PERMISSION_14 0x13F80A08u

//! Register Reset Value
#define TDDR_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_READ_PERMISSION_14 Register TDDR_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TDDR_PM_READ_PERMISSION_14 0x80A10
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_READ_PERMISSION_14 0x13F80A10u

//! Register Reset Value
#define TDDR_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_WRITE_PERMISSION_14 Register TDDR_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TDDR_PM_WRITE_PERMISSION_14 0x80A18
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_WRITE_PERMISSION_14 0x13F80A18u

//! Register Reset Value
#define TDDR_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_ADDR_MATCH_15 Register TDDR_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TDDR_PM_ADDR_MATCH_15 0x80A20
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_ADDR_MATCH_15 0x13F80A20u

//! Register Reset Value
#define TDDR_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDDR_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TDDR_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TDDR_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDDR_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDDR_PM_REQ_INFO_PERMISSION_15 Register TDDR_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TDDR_PM_REQ_INFO_PERMISSION_15 0x80A28
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_REQ_INFO_PERMISSION_15 0x13F80A28u

//! Register Reset Value
#define TDDR_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDDR_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_READ_PERMISSION_15 Register TDDR_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TDDR_PM_READ_PERMISSION_15 0x80A30
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_READ_PERMISSION_15 0x13F80A30u

//! Register Reset Value
#define TDDR_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDDR_PM_WRITE_PERMISSION_15 Register TDDR_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TDDR_PM_WRITE_PERMISSION_15 0x80A38
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TDDR_PM_WRITE_PERMISSION_15 0x13F80A38u

//! Register Reset Value
#define TDDR_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDDR_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_ERROR_LOG Register TPCTL_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ERROR_LOG 0x80C20
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ERROR_LOG 0x13F80C20u

//! Register Reset Value
#define TPCTL_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TPCTL_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TPCTL_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TPCTL_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TPCTL_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TPCTL_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TPCTL_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TPCTL_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TPCTL_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TPCTL_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TPCTL_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TPCTL_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TPCTL_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TPCTL_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TPCTL_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TPCTL_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TPCTL_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TPCTL_PM_CONTROL Register TPCTL_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TPCTL_PM_CONTROL 0x80C28
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_CONTROL 0x13F80C28u

//! Register Reset Value
#define TPCTL_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TPCTL_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TPCTL_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TPCTL_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TPCTL_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TPCTL_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TPCTL_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TPCTL_PM_ERROR_CLEAR_SINGLE Register TPCTL_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ERROR_CLEAR_SINGLE 0x80C30
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ERROR_CLEAR_SINGLE 0x13F80C30u

//! Register Reset Value
#define TPCTL_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TPCTL_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TPCTL_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TPCTL_PM_ERROR_CLEAR_MULTI Register TPCTL_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ERROR_CLEAR_MULTI 0x80C38
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ERROR_CLEAR_MULTI 0x13F80C38u

//! Register Reset Value
#define TPCTL_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TPCTL_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TPCTL_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TPCTL_PM_REQ_INFO_PERMISSION_0 Register TPCTL_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TPCTL_PM_REQ_INFO_PERMISSION_0 0x80C48
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_REQ_INFO_PERMISSION_0 0x13F80C48u

//! Register Reset Value
#define TPCTL_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_READ_PERMISSION_0 Register TPCTL_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TPCTL_PM_READ_PERMISSION_0 0x80C50
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_READ_PERMISSION_0 0x13F80C50u

//! Register Reset Value
#define TPCTL_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TPCTL_PM_WRITE_PERMISSION_0 Register TPCTL_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TPCTL_PM_WRITE_PERMISSION_0 0x80C58
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_WRITE_PERMISSION_0 0x13F80C58u

//! Register Reset Value
#define TPCTL_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TPCTL_PM_ADDR_MATCH_1 Register TPCTL_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ADDR_MATCH_1 0x80C60
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ADDR_MATCH_1 0x13F80C60u

//! Register Reset Value
#define TPCTL_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPCTL_PM_REQ_INFO_PERMISSION_1 Register TPCTL_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TPCTL_PM_REQ_INFO_PERMISSION_1 0x80C68
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_REQ_INFO_PERMISSION_1 0x13F80C68u

//! Register Reset Value
#define TPCTL_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_READ_PERMISSION_1 Register TPCTL_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TPCTL_PM_READ_PERMISSION_1 0x80C70
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_READ_PERMISSION_1 0x13F80C70u

//! Register Reset Value
#define TPCTL_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_WRITE_PERMISSION_1 Register TPCTL_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TPCTL_PM_WRITE_PERMISSION_1 0x80C78
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_WRITE_PERMISSION_1 0x13F80C78u

//! Register Reset Value
#define TPCTL_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_ADDR_MATCH_2 Register TPCTL_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ADDR_MATCH_2 0x80C80
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ADDR_MATCH_2 0x13F80C80u

//! Register Reset Value
#define TPCTL_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPCTL_PM_REQ_INFO_PERMISSION_2 Register TPCTL_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TPCTL_PM_REQ_INFO_PERMISSION_2 0x80C88
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_REQ_INFO_PERMISSION_2 0x13F80C88u

//! Register Reset Value
#define TPCTL_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_READ_PERMISSION_2 Register TPCTL_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TPCTL_PM_READ_PERMISSION_2 0x80C90
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_READ_PERMISSION_2 0x13F80C90u

//! Register Reset Value
#define TPCTL_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_WRITE_PERMISSION_2 Register TPCTL_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TPCTL_PM_WRITE_PERMISSION_2 0x80C98
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_WRITE_PERMISSION_2 0x13F80C98u

//! Register Reset Value
#define TPCTL_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_ADDR_MATCH_3 Register TPCTL_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ADDR_MATCH_3 0x80CA0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ADDR_MATCH_3 0x13F80CA0u

//! Register Reset Value
#define TPCTL_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPCTL_PM_REQ_INFO_PERMISSION_3 Register TPCTL_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TPCTL_PM_REQ_INFO_PERMISSION_3 0x80CA8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_REQ_INFO_PERMISSION_3 0x13F80CA8u

//! Register Reset Value
#define TPCTL_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_READ_PERMISSION_3 Register TPCTL_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TPCTL_PM_READ_PERMISSION_3 0x80CB0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_READ_PERMISSION_3 0x13F80CB0u

//! Register Reset Value
#define TPCTL_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_WRITE_PERMISSION_3 Register TPCTL_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TPCTL_PM_WRITE_PERMISSION_3 0x80CB8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_WRITE_PERMISSION_3 0x13F80CB8u

//! Register Reset Value
#define TPCTL_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_ADDR_MATCH_4 Register TPCTL_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ADDR_MATCH_4 0x80CC0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ADDR_MATCH_4 0x13F80CC0u

//! Register Reset Value
#define TPCTL_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPCTL_PM_REQ_INFO_PERMISSION_4 Register TPCTL_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TPCTL_PM_REQ_INFO_PERMISSION_4 0x80CC8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_REQ_INFO_PERMISSION_4 0x13F80CC8u

//! Register Reset Value
#define TPCTL_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_READ_PERMISSION_4 Register TPCTL_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TPCTL_PM_READ_PERMISSION_4 0x80CD0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_READ_PERMISSION_4 0x13F80CD0u

//! Register Reset Value
#define TPCTL_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_WRITE_PERMISSION_4 Register TPCTL_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TPCTL_PM_WRITE_PERMISSION_4 0x80CD8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_WRITE_PERMISSION_4 0x13F80CD8u

//! Register Reset Value
#define TPCTL_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_ADDR_MATCH_5 Register TPCTL_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ADDR_MATCH_5 0x80CE0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ADDR_MATCH_5 0x13F80CE0u

//! Register Reset Value
#define TPCTL_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPCTL_PM_REQ_INFO_PERMISSION_5 Register TPCTL_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TPCTL_PM_REQ_INFO_PERMISSION_5 0x80CE8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_REQ_INFO_PERMISSION_5 0x13F80CE8u

//! Register Reset Value
#define TPCTL_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_READ_PERMISSION_5 Register TPCTL_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TPCTL_PM_READ_PERMISSION_5 0x80CF0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_READ_PERMISSION_5 0x13F80CF0u

//! Register Reset Value
#define TPCTL_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_WRITE_PERMISSION_5 Register TPCTL_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TPCTL_PM_WRITE_PERMISSION_5 0x80CF8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_WRITE_PERMISSION_5 0x13F80CF8u

//! Register Reset Value
#define TPCTL_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_ADDR_MATCH_6 Register TPCTL_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ADDR_MATCH_6 0x80D00
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ADDR_MATCH_6 0x13F80D00u

//! Register Reset Value
#define TPCTL_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPCTL_PM_REQ_INFO_PERMISSION_6 Register TPCTL_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TPCTL_PM_REQ_INFO_PERMISSION_6 0x80D08
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_REQ_INFO_PERMISSION_6 0x13F80D08u

//! Register Reset Value
#define TPCTL_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_READ_PERMISSION_6 Register TPCTL_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TPCTL_PM_READ_PERMISSION_6 0x80D10
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_READ_PERMISSION_6 0x13F80D10u

//! Register Reset Value
#define TPCTL_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_WRITE_PERMISSION_6 Register TPCTL_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TPCTL_PM_WRITE_PERMISSION_6 0x80D18
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_WRITE_PERMISSION_6 0x13F80D18u

//! Register Reset Value
#define TPCTL_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_ADDR_MATCH_7 Register TPCTL_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ADDR_MATCH_7 0x80D20
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ADDR_MATCH_7 0x13F80D20u

//! Register Reset Value
#define TPCTL_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPCTL_PM_REQ_INFO_PERMISSION_7 Register TPCTL_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TPCTL_PM_REQ_INFO_PERMISSION_7 0x80D28
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_REQ_INFO_PERMISSION_7 0x13F80D28u

//! Register Reset Value
#define TPCTL_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_READ_PERMISSION_7 Register TPCTL_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TPCTL_PM_READ_PERMISSION_7 0x80D30
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_READ_PERMISSION_7 0x13F80D30u

//! Register Reset Value
#define TPCTL_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_WRITE_PERMISSION_7 Register TPCTL_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TPCTL_PM_WRITE_PERMISSION_7 0x80D38
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_WRITE_PERMISSION_7 0x13F80D38u

//! Register Reset Value
#define TPCTL_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_ADDR_MATCH_8 Register TPCTL_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ADDR_MATCH_8 0x80D40
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ADDR_MATCH_8 0x13F80D40u

//! Register Reset Value
#define TPCTL_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPCTL_PM_REQ_INFO_PERMISSION_8 Register TPCTL_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TPCTL_PM_REQ_INFO_PERMISSION_8 0x80D48
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_REQ_INFO_PERMISSION_8 0x13F80D48u

//! Register Reset Value
#define TPCTL_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_READ_PERMISSION_8 Register TPCTL_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TPCTL_PM_READ_PERMISSION_8 0x80D50
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_READ_PERMISSION_8 0x13F80D50u

//! Register Reset Value
#define TPCTL_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_WRITE_PERMISSION_8 Register TPCTL_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TPCTL_PM_WRITE_PERMISSION_8 0x80D58
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_WRITE_PERMISSION_8 0x13F80D58u

//! Register Reset Value
#define TPCTL_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_ADDR_MATCH_9 Register TPCTL_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ADDR_MATCH_9 0x80D60
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ADDR_MATCH_9 0x13F80D60u

//! Register Reset Value
#define TPCTL_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPCTL_PM_REQ_INFO_PERMISSION_9 Register TPCTL_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TPCTL_PM_REQ_INFO_PERMISSION_9 0x80D68
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_REQ_INFO_PERMISSION_9 0x13F80D68u

//! Register Reset Value
#define TPCTL_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_READ_PERMISSION_9 Register TPCTL_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TPCTL_PM_READ_PERMISSION_9 0x80D70
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_READ_PERMISSION_9 0x13F80D70u

//! Register Reset Value
#define TPCTL_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_WRITE_PERMISSION_9 Register TPCTL_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TPCTL_PM_WRITE_PERMISSION_9 0x80D78
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_WRITE_PERMISSION_9 0x13F80D78u

//! Register Reset Value
#define TPCTL_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_ADDR_MATCH_10 Register TPCTL_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ADDR_MATCH_10 0x80D80
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ADDR_MATCH_10 0x13F80D80u

//! Register Reset Value
#define TPCTL_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPCTL_PM_REQ_INFO_PERMISSION_10 Register TPCTL_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TPCTL_PM_REQ_INFO_PERMISSION_10 0x80D88
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_REQ_INFO_PERMISSION_10 0x13F80D88u

//! Register Reset Value
#define TPCTL_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_READ_PERMISSION_10 Register TPCTL_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TPCTL_PM_READ_PERMISSION_10 0x80D90
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_READ_PERMISSION_10 0x13F80D90u

//! Register Reset Value
#define TPCTL_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_WRITE_PERMISSION_10 Register TPCTL_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TPCTL_PM_WRITE_PERMISSION_10 0x80D98
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_WRITE_PERMISSION_10 0x13F80D98u

//! Register Reset Value
#define TPCTL_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_ADDR_MATCH_11 Register TPCTL_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ADDR_MATCH_11 0x80DA0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ADDR_MATCH_11 0x13F80DA0u

//! Register Reset Value
#define TPCTL_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPCTL_PM_REQ_INFO_PERMISSION_11 Register TPCTL_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TPCTL_PM_REQ_INFO_PERMISSION_11 0x80DA8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_REQ_INFO_PERMISSION_11 0x13F80DA8u

//! Register Reset Value
#define TPCTL_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_READ_PERMISSION_11 Register TPCTL_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TPCTL_PM_READ_PERMISSION_11 0x80DB0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_READ_PERMISSION_11 0x13F80DB0u

//! Register Reset Value
#define TPCTL_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_WRITE_PERMISSION_11 Register TPCTL_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TPCTL_PM_WRITE_PERMISSION_11 0x80DB8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_WRITE_PERMISSION_11 0x13F80DB8u

//! Register Reset Value
#define TPCTL_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_ADDR_MATCH_12 Register TPCTL_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ADDR_MATCH_12 0x80DC0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ADDR_MATCH_12 0x13F80DC0u

//! Register Reset Value
#define TPCTL_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPCTL_PM_REQ_INFO_PERMISSION_12 Register TPCTL_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TPCTL_PM_REQ_INFO_PERMISSION_12 0x80DC8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_REQ_INFO_PERMISSION_12 0x13F80DC8u

//! Register Reset Value
#define TPCTL_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_READ_PERMISSION_12 Register TPCTL_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TPCTL_PM_READ_PERMISSION_12 0x80DD0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_READ_PERMISSION_12 0x13F80DD0u

//! Register Reset Value
#define TPCTL_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_WRITE_PERMISSION_12 Register TPCTL_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TPCTL_PM_WRITE_PERMISSION_12 0x80DD8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_WRITE_PERMISSION_12 0x13F80DD8u

//! Register Reset Value
#define TPCTL_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_ADDR_MATCH_13 Register TPCTL_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ADDR_MATCH_13 0x80DE0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ADDR_MATCH_13 0x13F80DE0u

//! Register Reset Value
#define TPCTL_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPCTL_PM_REQ_INFO_PERMISSION_13 Register TPCTL_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TPCTL_PM_REQ_INFO_PERMISSION_13 0x80DE8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_REQ_INFO_PERMISSION_13 0x13F80DE8u

//! Register Reset Value
#define TPCTL_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_READ_PERMISSION_13 Register TPCTL_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TPCTL_PM_READ_PERMISSION_13 0x80DF0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_READ_PERMISSION_13 0x13F80DF0u

//! Register Reset Value
#define TPCTL_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_WRITE_PERMISSION_13 Register TPCTL_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TPCTL_PM_WRITE_PERMISSION_13 0x80DF8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_WRITE_PERMISSION_13 0x13F80DF8u

//! Register Reset Value
#define TPCTL_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_ADDR_MATCH_14 Register TPCTL_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ADDR_MATCH_14 0x80E00
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ADDR_MATCH_14 0x13F80E00u

//! Register Reset Value
#define TPCTL_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPCTL_PM_REQ_INFO_PERMISSION_14 Register TPCTL_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TPCTL_PM_REQ_INFO_PERMISSION_14 0x80E08
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_REQ_INFO_PERMISSION_14 0x13F80E08u

//! Register Reset Value
#define TPCTL_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_READ_PERMISSION_14 Register TPCTL_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TPCTL_PM_READ_PERMISSION_14 0x80E10
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_READ_PERMISSION_14 0x13F80E10u

//! Register Reset Value
#define TPCTL_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_WRITE_PERMISSION_14 Register TPCTL_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TPCTL_PM_WRITE_PERMISSION_14 0x80E18
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_WRITE_PERMISSION_14 0x13F80E18u

//! Register Reset Value
#define TPCTL_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_ADDR_MATCH_15 Register TPCTL_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TPCTL_PM_ADDR_MATCH_15 0x80E20
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_ADDR_MATCH_15 0x13F80E20u

//! Register Reset Value
#define TPCTL_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPCTL_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TPCTL_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TPCTL_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPCTL_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPCTL_PM_REQ_INFO_PERMISSION_15 Register TPCTL_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TPCTL_PM_REQ_INFO_PERMISSION_15 0x80E28
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_REQ_INFO_PERMISSION_15 0x13F80E28u

//! Register Reset Value
#define TPCTL_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPCTL_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_READ_PERMISSION_15 Register TPCTL_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TPCTL_PM_READ_PERMISSION_15 0x80E30
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_READ_PERMISSION_15 0x13F80E30u

//! Register Reset Value
#define TPCTL_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPCTL_PM_WRITE_PERMISSION_15 Register TPCTL_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TPCTL_PM_WRITE_PERMISSION_15 0x80E38
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPCTL_PM_WRITE_PERMISSION_15 0x13F80E38u

//! Register Reset Value
#define TPCTL_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPCTL_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_ERROR_LOG Register TPUB_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TPUB_PM_ERROR_LOG 0x81020
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ERROR_LOG 0x13F81020u

//! Register Reset Value
#define TPUB_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TPUB_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TPUB_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TPUB_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TPUB_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TPUB_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TPUB_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TPUB_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TPUB_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TPUB_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TPUB_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TPUB_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TPUB_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TPUB_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TPUB_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TPUB_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TPUB_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TPUB_PM_CONTROL Register TPUB_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TPUB_PM_CONTROL 0x81028
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_CONTROL 0x13F81028u

//! Register Reset Value
#define TPUB_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TPUB_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TPUB_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TPUB_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TPUB_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TPUB_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TPUB_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TPUB_PM_ERROR_CLEAR_SINGLE Register TPUB_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TPUB_PM_ERROR_CLEAR_SINGLE 0x81030
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ERROR_CLEAR_SINGLE 0x13F81030u

//! Register Reset Value
#define TPUB_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TPUB_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TPUB_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TPUB_PM_ERROR_CLEAR_MULTI Register TPUB_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TPUB_PM_ERROR_CLEAR_MULTI 0x81038
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ERROR_CLEAR_MULTI 0x13F81038u

//! Register Reset Value
#define TPUB_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TPUB_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TPUB_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TPUB_PM_REQ_INFO_PERMISSION_0 Register TPUB_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TPUB_PM_REQ_INFO_PERMISSION_0 0x81048
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_REQ_INFO_PERMISSION_0 0x13F81048u

//! Register Reset Value
#define TPUB_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_READ_PERMISSION_0 Register TPUB_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TPUB_PM_READ_PERMISSION_0 0x81050
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_READ_PERMISSION_0 0x13F81050u

//! Register Reset Value
#define TPUB_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TPUB_PM_WRITE_PERMISSION_0 Register TPUB_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TPUB_PM_WRITE_PERMISSION_0 0x81058
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_WRITE_PERMISSION_0 0x13F81058u

//! Register Reset Value
#define TPUB_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TPUB_PM_ADDR_MATCH_1 Register TPUB_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TPUB_PM_ADDR_MATCH_1 0x81060
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ADDR_MATCH_1 0x13F81060u

//! Register Reset Value
#define TPUB_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPUB_PM_REQ_INFO_PERMISSION_1 Register TPUB_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TPUB_PM_REQ_INFO_PERMISSION_1 0x81068
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_REQ_INFO_PERMISSION_1 0x13F81068u

//! Register Reset Value
#define TPUB_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_READ_PERMISSION_1 Register TPUB_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TPUB_PM_READ_PERMISSION_1 0x81070
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_READ_PERMISSION_1 0x13F81070u

//! Register Reset Value
#define TPUB_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_WRITE_PERMISSION_1 Register TPUB_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TPUB_PM_WRITE_PERMISSION_1 0x81078
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_WRITE_PERMISSION_1 0x13F81078u

//! Register Reset Value
#define TPUB_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_ADDR_MATCH_2 Register TPUB_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TPUB_PM_ADDR_MATCH_2 0x81080
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ADDR_MATCH_2 0x13F81080u

//! Register Reset Value
#define TPUB_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPUB_PM_REQ_INFO_PERMISSION_2 Register TPUB_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TPUB_PM_REQ_INFO_PERMISSION_2 0x81088
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_REQ_INFO_PERMISSION_2 0x13F81088u

//! Register Reset Value
#define TPUB_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_READ_PERMISSION_2 Register TPUB_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TPUB_PM_READ_PERMISSION_2 0x81090
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_READ_PERMISSION_2 0x13F81090u

//! Register Reset Value
#define TPUB_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_WRITE_PERMISSION_2 Register TPUB_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TPUB_PM_WRITE_PERMISSION_2 0x81098
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_WRITE_PERMISSION_2 0x13F81098u

//! Register Reset Value
#define TPUB_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_ADDR_MATCH_3 Register TPUB_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TPUB_PM_ADDR_MATCH_3 0x810A0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ADDR_MATCH_3 0x13F810A0u

//! Register Reset Value
#define TPUB_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPUB_PM_REQ_INFO_PERMISSION_3 Register TPUB_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TPUB_PM_REQ_INFO_PERMISSION_3 0x810A8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_REQ_INFO_PERMISSION_3 0x13F810A8u

//! Register Reset Value
#define TPUB_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_READ_PERMISSION_3 Register TPUB_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TPUB_PM_READ_PERMISSION_3 0x810B0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_READ_PERMISSION_3 0x13F810B0u

//! Register Reset Value
#define TPUB_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_WRITE_PERMISSION_3 Register TPUB_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TPUB_PM_WRITE_PERMISSION_3 0x810B8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_WRITE_PERMISSION_3 0x13F810B8u

//! Register Reset Value
#define TPUB_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_ADDR_MATCH_4 Register TPUB_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TPUB_PM_ADDR_MATCH_4 0x810C0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ADDR_MATCH_4 0x13F810C0u

//! Register Reset Value
#define TPUB_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPUB_PM_REQ_INFO_PERMISSION_4 Register TPUB_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TPUB_PM_REQ_INFO_PERMISSION_4 0x810C8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_REQ_INFO_PERMISSION_4 0x13F810C8u

//! Register Reset Value
#define TPUB_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_READ_PERMISSION_4 Register TPUB_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TPUB_PM_READ_PERMISSION_4 0x810D0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_READ_PERMISSION_4 0x13F810D0u

//! Register Reset Value
#define TPUB_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_WRITE_PERMISSION_4 Register TPUB_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TPUB_PM_WRITE_PERMISSION_4 0x810D8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_WRITE_PERMISSION_4 0x13F810D8u

//! Register Reset Value
#define TPUB_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_ADDR_MATCH_5 Register TPUB_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TPUB_PM_ADDR_MATCH_5 0x810E0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ADDR_MATCH_5 0x13F810E0u

//! Register Reset Value
#define TPUB_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPUB_PM_REQ_INFO_PERMISSION_5 Register TPUB_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TPUB_PM_REQ_INFO_PERMISSION_5 0x810E8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_REQ_INFO_PERMISSION_5 0x13F810E8u

//! Register Reset Value
#define TPUB_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_READ_PERMISSION_5 Register TPUB_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TPUB_PM_READ_PERMISSION_5 0x810F0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_READ_PERMISSION_5 0x13F810F0u

//! Register Reset Value
#define TPUB_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_WRITE_PERMISSION_5 Register TPUB_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TPUB_PM_WRITE_PERMISSION_5 0x810F8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_WRITE_PERMISSION_5 0x13F810F8u

//! Register Reset Value
#define TPUB_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_ADDR_MATCH_6 Register TPUB_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TPUB_PM_ADDR_MATCH_6 0x81100
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ADDR_MATCH_6 0x13F81100u

//! Register Reset Value
#define TPUB_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPUB_PM_REQ_INFO_PERMISSION_6 Register TPUB_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TPUB_PM_REQ_INFO_PERMISSION_6 0x81108
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_REQ_INFO_PERMISSION_6 0x13F81108u

//! Register Reset Value
#define TPUB_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_READ_PERMISSION_6 Register TPUB_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TPUB_PM_READ_PERMISSION_6 0x81110
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_READ_PERMISSION_6 0x13F81110u

//! Register Reset Value
#define TPUB_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_WRITE_PERMISSION_6 Register TPUB_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TPUB_PM_WRITE_PERMISSION_6 0x81118
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_WRITE_PERMISSION_6 0x13F81118u

//! Register Reset Value
#define TPUB_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_ADDR_MATCH_7 Register TPUB_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TPUB_PM_ADDR_MATCH_7 0x81120
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ADDR_MATCH_7 0x13F81120u

//! Register Reset Value
#define TPUB_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPUB_PM_REQ_INFO_PERMISSION_7 Register TPUB_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TPUB_PM_REQ_INFO_PERMISSION_7 0x81128
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_REQ_INFO_PERMISSION_7 0x13F81128u

//! Register Reset Value
#define TPUB_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_READ_PERMISSION_7 Register TPUB_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TPUB_PM_READ_PERMISSION_7 0x81130
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_READ_PERMISSION_7 0x13F81130u

//! Register Reset Value
#define TPUB_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_WRITE_PERMISSION_7 Register TPUB_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TPUB_PM_WRITE_PERMISSION_7 0x81138
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_WRITE_PERMISSION_7 0x13F81138u

//! Register Reset Value
#define TPUB_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_ADDR_MATCH_8 Register TPUB_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TPUB_PM_ADDR_MATCH_8 0x81140
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ADDR_MATCH_8 0x13F81140u

//! Register Reset Value
#define TPUB_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPUB_PM_REQ_INFO_PERMISSION_8 Register TPUB_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TPUB_PM_REQ_INFO_PERMISSION_8 0x81148
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_REQ_INFO_PERMISSION_8 0x13F81148u

//! Register Reset Value
#define TPUB_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_READ_PERMISSION_8 Register TPUB_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TPUB_PM_READ_PERMISSION_8 0x81150
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_READ_PERMISSION_8 0x13F81150u

//! Register Reset Value
#define TPUB_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_WRITE_PERMISSION_8 Register TPUB_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TPUB_PM_WRITE_PERMISSION_8 0x81158
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_WRITE_PERMISSION_8 0x13F81158u

//! Register Reset Value
#define TPUB_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_ADDR_MATCH_9 Register TPUB_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TPUB_PM_ADDR_MATCH_9 0x81160
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ADDR_MATCH_9 0x13F81160u

//! Register Reset Value
#define TPUB_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPUB_PM_REQ_INFO_PERMISSION_9 Register TPUB_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TPUB_PM_REQ_INFO_PERMISSION_9 0x81168
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_REQ_INFO_PERMISSION_9 0x13F81168u

//! Register Reset Value
#define TPUB_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_READ_PERMISSION_9 Register TPUB_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TPUB_PM_READ_PERMISSION_9 0x81170
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_READ_PERMISSION_9 0x13F81170u

//! Register Reset Value
#define TPUB_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_WRITE_PERMISSION_9 Register TPUB_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TPUB_PM_WRITE_PERMISSION_9 0x81178
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_WRITE_PERMISSION_9 0x13F81178u

//! Register Reset Value
#define TPUB_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_ADDR_MATCH_10 Register TPUB_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TPUB_PM_ADDR_MATCH_10 0x81180
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ADDR_MATCH_10 0x13F81180u

//! Register Reset Value
#define TPUB_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPUB_PM_REQ_INFO_PERMISSION_10 Register TPUB_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TPUB_PM_REQ_INFO_PERMISSION_10 0x81188
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_REQ_INFO_PERMISSION_10 0x13F81188u

//! Register Reset Value
#define TPUB_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_READ_PERMISSION_10 Register TPUB_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TPUB_PM_READ_PERMISSION_10 0x81190
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_READ_PERMISSION_10 0x13F81190u

//! Register Reset Value
#define TPUB_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_WRITE_PERMISSION_10 Register TPUB_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TPUB_PM_WRITE_PERMISSION_10 0x81198
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_WRITE_PERMISSION_10 0x13F81198u

//! Register Reset Value
#define TPUB_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_ADDR_MATCH_11 Register TPUB_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TPUB_PM_ADDR_MATCH_11 0x811A0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ADDR_MATCH_11 0x13F811A0u

//! Register Reset Value
#define TPUB_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPUB_PM_REQ_INFO_PERMISSION_11 Register TPUB_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TPUB_PM_REQ_INFO_PERMISSION_11 0x811A8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_REQ_INFO_PERMISSION_11 0x13F811A8u

//! Register Reset Value
#define TPUB_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_READ_PERMISSION_11 Register TPUB_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TPUB_PM_READ_PERMISSION_11 0x811B0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_READ_PERMISSION_11 0x13F811B0u

//! Register Reset Value
#define TPUB_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_WRITE_PERMISSION_11 Register TPUB_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TPUB_PM_WRITE_PERMISSION_11 0x811B8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_WRITE_PERMISSION_11 0x13F811B8u

//! Register Reset Value
#define TPUB_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_ADDR_MATCH_12 Register TPUB_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TPUB_PM_ADDR_MATCH_12 0x811C0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ADDR_MATCH_12 0x13F811C0u

//! Register Reset Value
#define TPUB_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPUB_PM_REQ_INFO_PERMISSION_12 Register TPUB_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TPUB_PM_REQ_INFO_PERMISSION_12 0x811C8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_REQ_INFO_PERMISSION_12 0x13F811C8u

//! Register Reset Value
#define TPUB_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_READ_PERMISSION_12 Register TPUB_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TPUB_PM_READ_PERMISSION_12 0x811D0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_READ_PERMISSION_12 0x13F811D0u

//! Register Reset Value
#define TPUB_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_WRITE_PERMISSION_12 Register TPUB_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TPUB_PM_WRITE_PERMISSION_12 0x811D8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_WRITE_PERMISSION_12 0x13F811D8u

//! Register Reset Value
#define TPUB_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_ADDR_MATCH_13 Register TPUB_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TPUB_PM_ADDR_MATCH_13 0x811E0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ADDR_MATCH_13 0x13F811E0u

//! Register Reset Value
#define TPUB_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPUB_PM_REQ_INFO_PERMISSION_13 Register TPUB_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TPUB_PM_REQ_INFO_PERMISSION_13 0x811E8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_REQ_INFO_PERMISSION_13 0x13F811E8u

//! Register Reset Value
#define TPUB_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_READ_PERMISSION_13 Register TPUB_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TPUB_PM_READ_PERMISSION_13 0x811F0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_READ_PERMISSION_13 0x13F811F0u

//! Register Reset Value
#define TPUB_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_WRITE_PERMISSION_13 Register TPUB_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TPUB_PM_WRITE_PERMISSION_13 0x811F8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_WRITE_PERMISSION_13 0x13F811F8u

//! Register Reset Value
#define TPUB_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_ADDR_MATCH_14 Register TPUB_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TPUB_PM_ADDR_MATCH_14 0x81200
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ADDR_MATCH_14 0x13F81200u

//! Register Reset Value
#define TPUB_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPUB_PM_REQ_INFO_PERMISSION_14 Register TPUB_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TPUB_PM_REQ_INFO_PERMISSION_14 0x81208
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_REQ_INFO_PERMISSION_14 0x13F81208u

//! Register Reset Value
#define TPUB_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_READ_PERMISSION_14 Register TPUB_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TPUB_PM_READ_PERMISSION_14 0x81210
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_READ_PERMISSION_14 0x13F81210u

//! Register Reset Value
#define TPUB_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_WRITE_PERMISSION_14 Register TPUB_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TPUB_PM_WRITE_PERMISSION_14 0x81218
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_WRITE_PERMISSION_14 0x13F81218u

//! Register Reset Value
#define TPUB_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_ADDR_MATCH_15 Register TPUB_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TPUB_PM_ADDR_MATCH_15 0x81220
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_ADDR_MATCH_15 0x13F81220u

//! Register Reset Value
#define TPUB_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TPUB_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TPUB_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TPUB_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TPUB_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TPUB_PM_REQ_INFO_PERMISSION_15 Register TPUB_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TPUB_PM_REQ_INFO_PERMISSION_15 0x81228
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_REQ_INFO_PERMISSION_15 0x13F81228u

//! Register Reset Value
#define TPUB_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TPUB_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_READ_PERMISSION_15 Register TPUB_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TPUB_PM_READ_PERMISSION_15 0x81230
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_READ_PERMISSION_15 0x13F81230u

//! Register Reset Value
#define TPUB_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TPUB_PM_WRITE_PERMISSION_15 Register TPUB_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TPUB_PM_WRITE_PERMISSION_15 0x81238
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TPUB_PM_WRITE_PERMISSION_15 0x13F81238u

//! Register Reset Value
#define TPUB_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TPUB_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_ERROR_LOG Register TLN60_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TLN60_PM_ERROR_LOG 0x81420
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ERROR_LOG 0x13F81420u

//! Register Reset Value
#define TLN60_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TLN60_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TLN60_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TLN60_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TLN60_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TLN60_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TLN60_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TLN60_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TLN60_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TLN60_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TLN60_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TLN60_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TLN60_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TLN60_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TLN60_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TLN60_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TLN60_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TLN60_PM_CONTROL Register TLN60_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TLN60_PM_CONTROL 0x81428
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_CONTROL 0x13F81428u

//! Register Reset Value
#define TLN60_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TLN60_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TLN60_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TLN60_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TLN60_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TLN60_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TLN60_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TLN60_PM_ERROR_CLEAR_SINGLE Register TLN60_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TLN60_PM_ERROR_CLEAR_SINGLE 0x81430
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ERROR_CLEAR_SINGLE 0x13F81430u

//! Register Reset Value
#define TLN60_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TLN60_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TLN60_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TLN60_PM_ERROR_CLEAR_MULTI Register TLN60_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TLN60_PM_ERROR_CLEAR_MULTI 0x81438
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ERROR_CLEAR_MULTI 0x13F81438u

//! Register Reset Value
#define TLN60_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TLN60_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TLN60_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TLN60_PM_REQ_INFO_PERMISSION_0 Register TLN60_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TLN60_PM_REQ_INFO_PERMISSION_0 0x81448
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_REQ_INFO_PERMISSION_0 0x13F81448u

//! Register Reset Value
#define TLN60_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_READ_PERMISSION_0 Register TLN60_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TLN60_PM_READ_PERMISSION_0 0x81450
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_READ_PERMISSION_0 0x13F81450u

//! Register Reset Value
#define TLN60_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TLN60_PM_WRITE_PERMISSION_0 Register TLN60_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TLN60_PM_WRITE_PERMISSION_0 0x81458
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_WRITE_PERMISSION_0 0x13F81458u

//! Register Reset Value
#define TLN60_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TLN60_PM_ADDR_MATCH_1 Register TLN60_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TLN60_PM_ADDR_MATCH_1 0x81460
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ADDR_MATCH_1 0x13F81460u

//! Register Reset Value
#define TLN60_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN60_PM_REQ_INFO_PERMISSION_1 Register TLN60_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TLN60_PM_REQ_INFO_PERMISSION_1 0x81468
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_REQ_INFO_PERMISSION_1 0x13F81468u

//! Register Reset Value
#define TLN60_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_READ_PERMISSION_1 Register TLN60_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TLN60_PM_READ_PERMISSION_1 0x81470
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_READ_PERMISSION_1 0x13F81470u

//! Register Reset Value
#define TLN60_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_WRITE_PERMISSION_1 Register TLN60_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TLN60_PM_WRITE_PERMISSION_1 0x81478
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_WRITE_PERMISSION_1 0x13F81478u

//! Register Reset Value
#define TLN60_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_ADDR_MATCH_2 Register TLN60_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TLN60_PM_ADDR_MATCH_2 0x81480
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ADDR_MATCH_2 0x13F81480u

//! Register Reset Value
#define TLN60_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN60_PM_REQ_INFO_PERMISSION_2 Register TLN60_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TLN60_PM_REQ_INFO_PERMISSION_2 0x81488
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_REQ_INFO_PERMISSION_2 0x13F81488u

//! Register Reset Value
#define TLN60_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_READ_PERMISSION_2 Register TLN60_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TLN60_PM_READ_PERMISSION_2 0x81490
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_READ_PERMISSION_2 0x13F81490u

//! Register Reset Value
#define TLN60_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_WRITE_PERMISSION_2 Register TLN60_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TLN60_PM_WRITE_PERMISSION_2 0x81498
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_WRITE_PERMISSION_2 0x13F81498u

//! Register Reset Value
#define TLN60_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_ADDR_MATCH_3 Register TLN60_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TLN60_PM_ADDR_MATCH_3 0x814A0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ADDR_MATCH_3 0x13F814A0u

//! Register Reset Value
#define TLN60_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN60_PM_REQ_INFO_PERMISSION_3 Register TLN60_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TLN60_PM_REQ_INFO_PERMISSION_3 0x814A8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_REQ_INFO_PERMISSION_3 0x13F814A8u

//! Register Reset Value
#define TLN60_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_READ_PERMISSION_3 Register TLN60_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TLN60_PM_READ_PERMISSION_3 0x814B0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_READ_PERMISSION_3 0x13F814B0u

//! Register Reset Value
#define TLN60_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_WRITE_PERMISSION_3 Register TLN60_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TLN60_PM_WRITE_PERMISSION_3 0x814B8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_WRITE_PERMISSION_3 0x13F814B8u

//! Register Reset Value
#define TLN60_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_ADDR_MATCH_4 Register TLN60_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TLN60_PM_ADDR_MATCH_4 0x814C0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ADDR_MATCH_4 0x13F814C0u

//! Register Reset Value
#define TLN60_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN60_PM_REQ_INFO_PERMISSION_4 Register TLN60_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TLN60_PM_REQ_INFO_PERMISSION_4 0x814C8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_REQ_INFO_PERMISSION_4 0x13F814C8u

//! Register Reset Value
#define TLN60_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_READ_PERMISSION_4 Register TLN60_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TLN60_PM_READ_PERMISSION_4 0x814D0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_READ_PERMISSION_4 0x13F814D0u

//! Register Reset Value
#define TLN60_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_WRITE_PERMISSION_4 Register TLN60_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TLN60_PM_WRITE_PERMISSION_4 0x814D8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_WRITE_PERMISSION_4 0x13F814D8u

//! Register Reset Value
#define TLN60_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_ADDR_MATCH_5 Register TLN60_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TLN60_PM_ADDR_MATCH_5 0x814E0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ADDR_MATCH_5 0x13F814E0u

//! Register Reset Value
#define TLN60_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN60_PM_REQ_INFO_PERMISSION_5 Register TLN60_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TLN60_PM_REQ_INFO_PERMISSION_5 0x814E8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_REQ_INFO_PERMISSION_5 0x13F814E8u

//! Register Reset Value
#define TLN60_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_READ_PERMISSION_5 Register TLN60_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TLN60_PM_READ_PERMISSION_5 0x814F0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_READ_PERMISSION_5 0x13F814F0u

//! Register Reset Value
#define TLN60_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_WRITE_PERMISSION_5 Register TLN60_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TLN60_PM_WRITE_PERMISSION_5 0x814F8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_WRITE_PERMISSION_5 0x13F814F8u

//! Register Reset Value
#define TLN60_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_ADDR_MATCH_6 Register TLN60_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TLN60_PM_ADDR_MATCH_6 0x81500
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ADDR_MATCH_6 0x13F81500u

//! Register Reset Value
#define TLN60_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN60_PM_REQ_INFO_PERMISSION_6 Register TLN60_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TLN60_PM_REQ_INFO_PERMISSION_6 0x81508
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_REQ_INFO_PERMISSION_6 0x13F81508u

//! Register Reset Value
#define TLN60_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_READ_PERMISSION_6 Register TLN60_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TLN60_PM_READ_PERMISSION_6 0x81510
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_READ_PERMISSION_6 0x13F81510u

//! Register Reset Value
#define TLN60_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_WRITE_PERMISSION_6 Register TLN60_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TLN60_PM_WRITE_PERMISSION_6 0x81518
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_WRITE_PERMISSION_6 0x13F81518u

//! Register Reset Value
#define TLN60_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_ADDR_MATCH_7 Register TLN60_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TLN60_PM_ADDR_MATCH_7 0x81520
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ADDR_MATCH_7 0x13F81520u

//! Register Reset Value
#define TLN60_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN60_PM_REQ_INFO_PERMISSION_7 Register TLN60_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TLN60_PM_REQ_INFO_PERMISSION_7 0x81528
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_REQ_INFO_PERMISSION_7 0x13F81528u

//! Register Reset Value
#define TLN60_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_READ_PERMISSION_7 Register TLN60_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TLN60_PM_READ_PERMISSION_7 0x81530
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_READ_PERMISSION_7 0x13F81530u

//! Register Reset Value
#define TLN60_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_WRITE_PERMISSION_7 Register TLN60_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TLN60_PM_WRITE_PERMISSION_7 0x81538
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_WRITE_PERMISSION_7 0x13F81538u

//! Register Reset Value
#define TLN60_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_ADDR_MATCH_8 Register TLN60_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TLN60_PM_ADDR_MATCH_8 0x81540
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ADDR_MATCH_8 0x13F81540u

//! Register Reset Value
#define TLN60_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN60_PM_REQ_INFO_PERMISSION_8 Register TLN60_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TLN60_PM_REQ_INFO_PERMISSION_8 0x81548
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_REQ_INFO_PERMISSION_8 0x13F81548u

//! Register Reset Value
#define TLN60_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_READ_PERMISSION_8 Register TLN60_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TLN60_PM_READ_PERMISSION_8 0x81550
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_READ_PERMISSION_8 0x13F81550u

//! Register Reset Value
#define TLN60_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_WRITE_PERMISSION_8 Register TLN60_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TLN60_PM_WRITE_PERMISSION_8 0x81558
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_WRITE_PERMISSION_8 0x13F81558u

//! Register Reset Value
#define TLN60_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_ADDR_MATCH_9 Register TLN60_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TLN60_PM_ADDR_MATCH_9 0x81560
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ADDR_MATCH_9 0x13F81560u

//! Register Reset Value
#define TLN60_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN60_PM_REQ_INFO_PERMISSION_9 Register TLN60_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TLN60_PM_REQ_INFO_PERMISSION_9 0x81568
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_REQ_INFO_PERMISSION_9 0x13F81568u

//! Register Reset Value
#define TLN60_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_READ_PERMISSION_9 Register TLN60_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TLN60_PM_READ_PERMISSION_9 0x81570
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_READ_PERMISSION_9 0x13F81570u

//! Register Reset Value
#define TLN60_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_WRITE_PERMISSION_9 Register TLN60_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TLN60_PM_WRITE_PERMISSION_9 0x81578
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_WRITE_PERMISSION_9 0x13F81578u

//! Register Reset Value
#define TLN60_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_ADDR_MATCH_10 Register TLN60_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TLN60_PM_ADDR_MATCH_10 0x81580
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ADDR_MATCH_10 0x13F81580u

//! Register Reset Value
#define TLN60_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN60_PM_REQ_INFO_PERMISSION_10 Register TLN60_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TLN60_PM_REQ_INFO_PERMISSION_10 0x81588
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_REQ_INFO_PERMISSION_10 0x13F81588u

//! Register Reset Value
#define TLN60_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_READ_PERMISSION_10 Register TLN60_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TLN60_PM_READ_PERMISSION_10 0x81590
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_READ_PERMISSION_10 0x13F81590u

//! Register Reset Value
#define TLN60_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_WRITE_PERMISSION_10 Register TLN60_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TLN60_PM_WRITE_PERMISSION_10 0x81598
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_WRITE_PERMISSION_10 0x13F81598u

//! Register Reset Value
#define TLN60_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_ADDR_MATCH_11 Register TLN60_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TLN60_PM_ADDR_MATCH_11 0x815A0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ADDR_MATCH_11 0x13F815A0u

//! Register Reset Value
#define TLN60_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN60_PM_REQ_INFO_PERMISSION_11 Register TLN60_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TLN60_PM_REQ_INFO_PERMISSION_11 0x815A8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_REQ_INFO_PERMISSION_11 0x13F815A8u

//! Register Reset Value
#define TLN60_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_READ_PERMISSION_11 Register TLN60_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TLN60_PM_READ_PERMISSION_11 0x815B0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_READ_PERMISSION_11 0x13F815B0u

//! Register Reset Value
#define TLN60_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_WRITE_PERMISSION_11 Register TLN60_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TLN60_PM_WRITE_PERMISSION_11 0x815B8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_WRITE_PERMISSION_11 0x13F815B8u

//! Register Reset Value
#define TLN60_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_ADDR_MATCH_12 Register TLN60_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TLN60_PM_ADDR_MATCH_12 0x815C0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ADDR_MATCH_12 0x13F815C0u

//! Register Reset Value
#define TLN60_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN60_PM_REQ_INFO_PERMISSION_12 Register TLN60_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TLN60_PM_REQ_INFO_PERMISSION_12 0x815C8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_REQ_INFO_PERMISSION_12 0x13F815C8u

//! Register Reset Value
#define TLN60_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_READ_PERMISSION_12 Register TLN60_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TLN60_PM_READ_PERMISSION_12 0x815D0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_READ_PERMISSION_12 0x13F815D0u

//! Register Reset Value
#define TLN60_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_WRITE_PERMISSION_12 Register TLN60_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TLN60_PM_WRITE_PERMISSION_12 0x815D8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_WRITE_PERMISSION_12 0x13F815D8u

//! Register Reset Value
#define TLN60_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_ADDR_MATCH_13 Register TLN60_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TLN60_PM_ADDR_MATCH_13 0x815E0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ADDR_MATCH_13 0x13F815E0u

//! Register Reset Value
#define TLN60_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN60_PM_REQ_INFO_PERMISSION_13 Register TLN60_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TLN60_PM_REQ_INFO_PERMISSION_13 0x815E8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_REQ_INFO_PERMISSION_13 0x13F815E8u

//! Register Reset Value
#define TLN60_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_READ_PERMISSION_13 Register TLN60_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TLN60_PM_READ_PERMISSION_13 0x815F0
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_READ_PERMISSION_13 0x13F815F0u

//! Register Reset Value
#define TLN60_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_WRITE_PERMISSION_13 Register TLN60_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TLN60_PM_WRITE_PERMISSION_13 0x815F8
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_WRITE_PERMISSION_13 0x13F815F8u

//! Register Reset Value
#define TLN60_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_ADDR_MATCH_14 Register TLN60_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TLN60_PM_ADDR_MATCH_14 0x81600
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ADDR_MATCH_14 0x13F81600u

//! Register Reset Value
#define TLN60_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN60_PM_REQ_INFO_PERMISSION_14 Register TLN60_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TLN60_PM_REQ_INFO_PERMISSION_14 0x81608
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_REQ_INFO_PERMISSION_14 0x13F81608u

//! Register Reset Value
#define TLN60_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_READ_PERMISSION_14 Register TLN60_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TLN60_PM_READ_PERMISSION_14 0x81610
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_READ_PERMISSION_14 0x13F81610u

//! Register Reset Value
#define TLN60_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_WRITE_PERMISSION_14 Register TLN60_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TLN60_PM_WRITE_PERMISSION_14 0x81618
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_WRITE_PERMISSION_14 0x13F81618u

//! Register Reset Value
#define TLN60_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_ADDR_MATCH_15 Register TLN60_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TLN60_PM_ADDR_MATCH_15 0x81620
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_ADDR_MATCH_15 0x13F81620u

//! Register Reset Value
#define TLN60_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN60_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TLN60_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TLN60_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN60_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN60_PM_REQ_INFO_PERMISSION_15 Register TLN60_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TLN60_PM_REQ_INFO_PERMISSION_15 0x81628
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_REQ_INFO_PERMISSION_15 0x13F81628u

//! Register Reset Value
#define TLN60_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN60_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_READ_PERMISSION_15 Register TLN60_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TLN60_PM_READ_PERMISSION_15 0x81630
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_READ_PERMISSION_15 0x13F81630u

//! Register Reset Value
#define TLN60_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN60_PM_WRITE_PERMISSION_15 Register TLN60_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TLN60_PM_WRITE_PERMISSION_15 0x81638
//! Register Offset (absolute) for 1st Instance SSX6_SSX
#define SSX6_SSX_TLN60_PM_WRITE_PERMISSION_15 0x13F81638u

//! Register Reset Value
#define TLN60_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN60_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! @}

#endif
