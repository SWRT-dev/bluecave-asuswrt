//-----------------------------------------------------------------------------
// LSD Generator
//-----------------------------------------------------------------------------
// Perl Package        : LSD::generator::targetC (v1.1)
// LSD Source          : D:/Users/shij/Perforce/l1033.grx500.win.v_shij.priv.dfv.grx500.dfv/ipg_lsd/lsd_sys/source/xml/reg_files/ssx2_ssx.xml
// Register File Name  : SSX2_SSX
// Register File Title : SSX2_ssx
// Register Width      : 64
// Note                : Doxygen compliant comments
//-----------------------------------------------------------------------------

#ifndef _SSX2_SSX_H
#define _SSX2_SSX_H

//! \defgroup SSX2_SSX Register File SSX2_SSX - SSX2_ssx
//! @{

//! Base Address of SSX2_SSX
#define SSX2_SSX_MODULE_BASE 0x1BF00000u

//! \defgroup TREG2_RT_COMPONENT Register TREG2_RT_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TREG2_RT_COMPONENT 0x0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_RT_COMPONENT 0x1BF00000u

//! Register Reset Value
#define TREG2_RT_COMPONENT_RST 0x0000000062003532u

//! Field REV - rev
#define TREG2_RT_COMPONENT_REV_POS 0
//! Field REV - rev
#define TREG2_RT_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TREG2_RT_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TREG2_RT_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TREG2_RT_NETWORK Register TREG2_RT_NETWORK - network
//! @{

//! Register Offset (relative)
#define TREG2_RT_NETWORK 0x10
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_RT_NETWORK 0x1BF00010u

//! Register Reset Value
#define TREG2_RT_NETWORK_RST 0x0000000000000000u

//! Field REV - rev
#define TREG2_RT_NETWORK_REV_POS 32
//! Field REV - rev
#define TREG2_RT_NETWORK_REV_MASK 0xFFFF00000000u

//! Field ID - id
#define TREG2_RT_NETWORK_ID_POS 48
//! Field ID - id
#define TREG2_RT_NETWORK_ID_MASK 0xFFFF000000000000u

//! @}

//! \defgroup TREG2_RT_INITID_READBACK Register TREG2_RT_INITID_READBACK - initid_readback
//! @{

//! Register Offset (relative)
#define TREG2_RT_INITID_READBACK 0x70
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_RT_INITID_READBACK 0x1BF00070u

//! Register Reset Value
#define TREG2_RT_INITID_READBACK_RST 0x0000000000000000u

//! Field INITID - initid
#define TREG2_RT_INITID_READBACK_INITID_POS 0
//! Field INITID - initid
#define TREG2_RT_INITID_READBACK_INITID_MASK 0xFFu

//! @}

//! \defgroup TREG2_RT_NETWORK_CONTROL Register TREG2_RT_NETWORK_CONTROL - network_control
//! @{

//! Register Offset (relative)
#define TREG2_RT_NETWORK_CONTROL 0x78
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_RT_NETWORK_CONTROL 0x1BF00078u

//! Register Reset Value
#define TREG2_RT_NETWORK_CONTROL_RST 0x0000000000000000u

//! Field TIMEOUT_BASE - timeout_base
#define TREG2_RT_NETWORK_CONTROL_TIMEOUT_BASE_POS 8
//! Field TIMEOUT_BASE - timeout_base
#define TREG2_RT_NETWORK_CONTROL_TIMEOUT_BASE_MASK 0x700u

//! Field CLOCK_GATE_DISABLE - clock_gate_disable
#define TREG2_RT_NETWORK_CONTROL_CLOCK_GATE_DISABLE_POS 56
//! Field CLOCK_GATE_DISABLE - clock_gate_disable
#define TREG2_RT_NETWORK_CONTROL_CLOCK_GATE_DISABLE_MASK 0x100000000000000u

//! @}

//! \defgroup TREG2_SI_CONTROL Register TREG2_SI_CONTROL - control
//! @{

//! Register Offset (relative)
#define TREG2_SI_CONTROL 0x420
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_SI_CONTROL 0x1BF00420u

//! Register Reset Value
#define TREG2_SI_CONTROL_RST 0x0000000000000000u

//! Field CLOCK_GATE_DISABLE - clock_gate_disable
#define TREG2_SI_CONTROL_CLOCK_GATE_DISABLE_POS 56
//! Field CLOCK_GATE_DISABLE - clock_gate_disable
#define TREG2_SI_CONTROL_CLOCK_GATE_DISABLE_MASK 0x100000000000000u

//! @}

//! \defgroup TREG2_SI_FLAG_STATUS_0 Register TREG2_SI_FLAG_STATUS_0 - flag_status_0
//! @{

//! Register Offset (relative)
#define TREG2_SI_FLAG_STATUS_0 0x510
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_SI_FLAG_STATUS_0 0x1BF00510u

//! Register Reset Value
#define TREG2_SI_FLAG_STATUS_0_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG2_SI_FLAG_STATUS_0_STATUS_POS 0
//! Field STATUS - status
#define TREG2_SI_FLAG_STATUS_0_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_SI_FLAG_STATUS_1 Register TREG2_SI_FLAG_STATUS_1 - flag_status_1
//! @{

//! Register Offset (relative)
#define TREG2_SI_FLAG_STATUS_1 0x530
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_SI_FLAG_STATUS_1 0x1BF00530u

//! Register Reset Value
#define TREG2_SI_FLAG_STATUS_1_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG2_SI_FLAG_STATUS_1_STATUS_POS 0
//! Field STATUS - status
#define TREG2_SI_FLAG_STATUS_1_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_SI_FLAG_STATUS_2 Register TREG2_SI_FLAG_STATUS_2 - flag_status_2
//! @{

//! Register Offset (relative)
#define TREG2_SI_FLAG_STATUS_2 0x550
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_SI_FLAG_STATUS_2 0x1BF00550u

//! Register Reset Value
#define TREG2_SI_FLAG_STATUS_2_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG2_SI_FLAG_STATUS_2_STATUS_POS 0
//! Field STATUS - status
#define TREG2_SI_FLAG_STATUS_2_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_SI_FLAG_STATUS_3 Register TREG2_SI_FLAG_STATUS_3 - flag_status_3
//! @{

//! Register Offset (relative)
#define TREG2_SI_FLAG_STATUS_3 0x570
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_SI_FLAG_STATUS_3 0x1BF00570u

//! Register Reset Value
#define TREG2_SI_FLAG_STATUS_3_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG2_SI_FLAG_STATUS_3_STATUS_POS 0
//! Field STATUS - status
#define TREG2_SI_FLAG_STATUS_3_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_SI_FLAG_STATUS_4 Register TREG2_SI_FLAG_STATUS_4 - flag_status_4
//! @{

//! Register Offset (relative)
#define TREG2_SI_FLAG_STATUS_4 0x590
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_SI_FLAG_STATUS_4 0x1BF00590u

//! Register Reset Value
#define TREG2_SI_FLAG_STATUS_4_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG2_SI_FLAG_STATUS_4_STATUS_POS 0
//! Field STATUS - status
#define TREG2_SI_FLAG_STATUS_4_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_SI_FLAG_STATUS_5 Register TREG2_SI_FLAG_STATUS_5 - flag_status_5
//! @{

//! Register Offset (relative)
#define TREG2_SI_FLAG_STATUS_5 0x5B0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_SI_FLAG_STATUS_5 0x1BF005B0u

//! Register Reset Value
#define TREG2_SI_FLAG_STATUS_5_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG2_SI_FLAG_STATUS_5_STATUS_POS 0
//! Field STATUS - status
#define TREG2_SI_FLAG_STATUS_5_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_SI_FLAG_STATUS_6 Register TREG2_SI_FLAG_STATUS_6 - flag_status_6
//! @{

//! Register Offset (relative)
#define TREG2_SI_FLAG_STATUS_6 0x5D0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_SI_FLAG_STATUS_6 0x1BF005D0u

//! Register Reset Value
#define TREG2_SI_FLAG_STATUS_6_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG2_SI_FLAG_STATUS_6_STATUS_POS 0
//! Field STATUS - status
#define TREG2_SI_FLAG_STATUS_6_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_SI_FLAG_STATUS_7 Register TREG2_SI_FLAG_STATUS_7 - flag_status_7
//! @{

//! Register Offset (relative)
#define TREG2_SI_FLAG_STATUS_7 0x5F0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_SI_FLAG_STATUS_7 0x1BF005F0u

//! Register Reset Value
#define TREG2_SI_FLAG_STATUS_7_RST 0x0000000000000000u

//! Field STATUS - status
#define TREG2_SI_FLAG_STATUS_7_STATUS_POS 0
//! Field STATUS - status
#define TREG2_SI_FLAG_STATUS_7_STATUS_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_TA_COMPONENT Register TUS0_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TUS0_TA_COMPONENT 0x1000
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_TA_COMPONENT 0x1BF01000u

//! Register Reset Value
#define TUS0_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TUS0_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TUS0_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TUS0_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TUS0_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TUS0_TA_CORE Register TUS0_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TUS0_TA_CORE 0x1018
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_TA_CORE 0x1BF01018u

//! Register Reset Value
#define TUS0_TA_CORE_RST 0x000050C500250001u

//! Field REV_CODE - rev_code
#define TUS0_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TUS0_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TUS0_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TUS0_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TUS0_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TUS0_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TUS0_TA_AGENT_CONTROL Register TUS0_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TUS0_TA_AGENT_CONTROL 0x1020
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_TA_AGENT_CONTROL 0x1BF01020u

//! Register Reset Value
#define TUS0_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TUS0_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TUS0_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TUS0_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TUS0_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TUS0_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TUS0_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TUS0_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TUS0_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TUS0_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TUS0_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TUS0_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TUS0_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TUS0_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TUS0_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TUS0_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TUS0_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TUS0_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TUS0_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TUS0_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TUS0_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TUS0_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TUS0_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TUS0_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TUS0_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TUS0_TA_AGENT_STATUS Register TUS0_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TUS0_TA_AGENT_STATUS 0x1028
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_TA_AGENT_STATUS 0x1BF01028u

//! Register Reset Value
#define TUS0_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TUS0_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TUS0_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TUS0_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TUS0_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TUS0_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TUS0_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TUS0_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TUS0_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TUS0_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TUS0_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TUS0_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TUS0_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TUS0_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TUS0_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TUS0_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TUS0_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TUS0_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TUS0_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TUS0_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TUS0_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TUS0_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TUS0_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TUS0_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TUS0_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TUS0_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TUS0_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TUS0_TA_ERROR_LOG Register TUS0_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TUS0_TA_ERROR_LOG 0x1058
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_TA_ERROR_LOG 0x1BF01058u

//! Register Reset Value
#define TUS0_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TUS0_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TUS0_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TUS0_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TUS0_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TUS0_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TUS0_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TUS0_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TUS0_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TUS0_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TUS0_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TUS0_TA_ERROR_LOG_ADDR Register TUS0_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TUS0_TA_ERROR_LOG_ADDR 0x1060
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_TA_ERROR_LOG_ADDR 0x1BF01060u

//! Register Reset Value
#define TUS0_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TUS0_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TUS0_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_TA_BANDWIDTH_0 Register TUS0_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TUS0_TA_BANDWIDTH_0 0x1100
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_TA_BANDWIDTH_0 0x1BF01100u

//! Register Reset Value
#define TUS0_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TUS0_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TUS0_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TUS0_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TUS0_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TUS0_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TUS0_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TUS0_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TUS0_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS0_TA_BANDWIDTH_1 Register TUS0_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TUS0_TA_BANDWIDTH_1 0x1108
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_TA_BANDWIDTH_1 0x1BF01108u

//! Register Reset Value
#define TUS0_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TUS0_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TUS0_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TUS0_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TUS0_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TUS0_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TUS0_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TUS0_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TUS0_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS0_TA_BANDWIDTH_2 Register TUS0_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TUS0_TA_BANDWIDTH_2 0x1110
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_TA_BANDWIDTH_2 0x1BF01110u

//! Register Reset Value
#define TUS0_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TUS0_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TUS0_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TUS0_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TUS0_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TUS0_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TUS0_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TUS0_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TUS0_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS0_TA_BANDWIDTH_3 Register TUS0_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TUS0_TA_BANDWIDTH_3 0x1118
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_TA_BANDWIDTH_3 0x1BF01118u

//! Register Reset Value
#define TUS0_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TUS0_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TUS0_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TUS0_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TUS0_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TUS0_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TUS0_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TUS0_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TUS0_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS0_TA_ALLOC_LIMIT_0 Register TUS0_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TUS0_TA_ALLOC_LIMIT_0 0x1200
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_TA_ALLOC_LIMIT_0 0x1BF01200u

//! Register Reset Value
#define TUS0_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TUS0_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TUS0_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TUS0_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TUS0_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TUS0_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TUS0_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TUS0_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TUS0_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TUS0_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TUS0_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TUS0_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TUS0_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TUS0_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TUS0_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TUS0_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TUS0_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS0_TA_ALLOC_LIMIT_1 Register TUS0_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TUS0_TA_ALLOC_LIMIT_1 0x1208
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_TA_ALLOC_LIMIT_1 0x1BF01208u

//! Register Reset Value
#define TUS0_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TUS0_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TUS0_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TUS0_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TUS0_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TUS0_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TUS0_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TUS0_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TUS0_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TUS0_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TUS0_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TUS0_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TUS0_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TUS0_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TUS0_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TUS0_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TUS0_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS0_TA_ALLOC_LIMIT_2 Register TUS0_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TUS0_TA_ALLOC_LIMIT_2 0x1210
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_TA_ALLOC_LIMIT_2 0x1BF01210u

//! Register Reset Value
#define TUS0_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TUS0_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TUS0_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TUS0_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TUS0_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TUS0_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TUS0_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TUS0_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TUS0_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TUS0_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TUS0_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TUS0_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TUS0_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TUS0_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TUS0_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TUS0_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TUS0_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS0_TA_ALLOC_LIMIT_3 Register TUS0_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TUS0_TA_ALLOC_LIMIT_3 0x1218
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_TA_ALLOC_LIMIT_3 0x1BF01218u

//! Register Reset Value
#define TUS0_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TUS0_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TUS0_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TUS0_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TUS0_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TUS0_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TUS0_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TUS0_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TUS0_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TUS0_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TUS0_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TUS0_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TUS0_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TUS0_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TUS0_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TUS0_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TUS0_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS0C_TA_COMPONENT Register TUS0C_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TUS0C_TA_COMPONENT 0x1400
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_TA_COMPONENT 0x1BF01400u

//! Register Reset Value
#define TUS0C_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TUS0C_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TUS0C_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TUS0C_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TUS0C_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TUS0C_TA_CORE Register TUS0C_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TUS0C_TA_CORE 0x1418
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_TA_CORE 0x1BF01418u

//! Register Reset Value
#define TUS0C_TA_CORE_RST 0x000088C300260001u

//! Field REV_CODE - rev_code
#define TUS0C_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TUS0C_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TUS0C_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TUS0C_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TUS0C_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TUS0C_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TUS0C_TA_AGENT_CONTROL Register TUS0C_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TUS0C_TA_AGENT_CONTROL 0x1420
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_TA_AGENT_CONTROL 0x1BF01420u

//! Register Reset Value
#define TUS0C_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TUS0C_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TUS0C_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TUS0C_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TUS0C_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TUS0C_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TUS0C_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TUS0C_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TUS0C_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TUS0C_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TUS0C_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TUS0C_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TUS0C_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TUS0C_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TUS0C_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TUS0C_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TUS0C_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TUS0C_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TUS0C_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TUS0C_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TUS0C_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TUS0C_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TUS0C_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TUS0C_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TUS0C_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TUS0C_TA_AGENT_STATUS Register TUS0C_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TUS0C_TA_AGENT_STATUS 0x1428
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_TA_AGENT_STATUS 0x1BF01428u

//! Register Reset Value
#define TUS0C_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TUS0C_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TUS0C_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TUS0C_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TUS0C_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TUS0C_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TUS0C_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TUS0C_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TUS0C_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TUS0C_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TUS0C_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TUS0C_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TUS0C_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TUS0C_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TUS0C_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TUS0C_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TUS0C_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TUS0C_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TUS0C_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TUS0C_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TUS0C_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TUS0C_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TUS0C_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TUS0C_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TUS0C_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TUS0C_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TUS0C_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TUS0C_TA_ERROR_LOG Register TUS0C_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TUS0C_TA_ERROR_LOG 0x1458
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_TA_ERROR_LOG 0x1BF01458u

//! Register Reset Value
#define TUS0C_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TUS0C_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TUS0C_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TUS0C_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TUS0C_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TUS0C_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TUS0C_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TUS0C_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TUS0C_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TUS0C_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TUS0C_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TUS0C_TA_ERROR_LOG_ADDR Register TUS0C_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TUS0C_TA_ERROR_LOG_ADDR 0x1460
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_TA_ERROR_LOG_ADDR 0x1BF01460u

//! Register Reset Value
#define TUS0C_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TUS0C_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TUS0C_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_TA_BANDWIDTH_0 Register TUS0C_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TUS0C_TA_BANDWIDTH_0 0x1500
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_TA_BANDWIDTH_0 0x1BF01500u

//! Register Reset Value
#define TUS0C_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TUS0C_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TUS0C_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TUS0C_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TUS0C_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TUS0C_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TUS0C_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TUS0C_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TUS0C_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS0C_TA_BANDWIDTH_1 Register TUS0C_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TUS0C_TA_BANDWIDTH_1 0x1508
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_TA_BANDWIDTH_1 0x1BF01508u

//! Register Reset Value
#define TUS0C_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TUS0C_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TUS0C_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TUS0C_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TUS0C_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TUS0C_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TUS0C_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TUS0C_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TUS0C_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS0C_TA_BANDWIDTH_2 Register TUS0C_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TUS0C_TA_BANDWIDTH_2 0x1510
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_TA_BANDWIDTH_2 0x1BF01510u

//! Register Reset Value
#define TUS0C_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TUS0C_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TUS0C_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TUS0C_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TUS0C_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TUS0C_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TUS0C_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TUS0C_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TUS0C_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS0C_TA_BANDWIDTH_3 Register TUS0C_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TUS0C_TA_BANDWIDTH_3 0x1518
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_TA_BANDWIDTH_3 0x1BF01518u

//! Register Reset Value
#define TUS0C_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TUS0C_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TUS0C_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TUS0C_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TUS0C_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TUS0C_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TUS0C_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TUS0C_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TUS0C_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS0C_TA_ALLOC_LIMIT_0 Register TUS0C_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TUS0C_TA_ALLOC_LIMIT_0 0x1600
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_TA_ALLOC_LIMIT_0 0x1BF01600u

//! Register Reset Value
#define TUS0C_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TUS0C_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TUS0C_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TUS0C_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TUS0C_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TUS0C_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TUS0C_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TUS0C_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TUS0C_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TUS0C_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TUS0C_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TUS0C_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TUS0C_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TUS0C_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TUS0C_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TUS0C_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TUS0C_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS0C_TA_ALLOC_LIMIT_1 Register TUS0C_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TUS0C_TA_ALLOC_LIMIT_1 0x1608
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_TA_ALLOC_LIMIT_1 0x1BF01608u

//! Register Reset Value
#define TUS0C_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TUS0C_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TUS0C_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TUS0C_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TUS0C_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TUS0C_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TUS0C_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TUS0C_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TUS0C_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TUS0C_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TUS0C_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TUS0C_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TUS0C_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TUS0C_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TUS0C_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TUS0C_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TUS0C_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS0C_TA_ALLOC_LIMIT_2 Register TUS0C_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TUS0C_TA_ALLOC_LIMIT_2 0x1610
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_TA_ALLOC_LIMIT_2 0x1BF01610u

//! Register Reset Value
#define TUS0C_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TUS0C_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TUS0C_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TUS0C_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TUS0C_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TUS0C_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TUS0C_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TUS0C_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TUS0C_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TUS0C_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TUS0C_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TUS0C_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TUS0C_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TUS0C_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TUS0C_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TUS0C_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TUS0C_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS0C_TA_ALLOC_LIMIT_3 Register TUS0C_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TUS0C_TA_ALLOC_LIMIT_3 0x1618
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_TA_ALLOC_LIMIT_3 0x1BF01618u

//! Register Reset Value
#define TUS0C_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TUS0C_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TUS0C_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TUS0C_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TUS0C_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TUS0C_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TUS0C_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TUS0C_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TUS0C_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TUS0C_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TUS0C_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TUS0C_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TUS0C_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TUS0C_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TUS0C_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TUS0C_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TUS0C_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS1_TA_COMPONENT Register TUS1_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TUS1_TA_COMPONENT 0x1800
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_TA_COMPONENT 0x1BF01800u

//! Register Reset Value
#define TUS1_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TUS1_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TUS1_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TUS1_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TUS1_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TUS1_TA_CORE Register TUS1_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TUS1_TA_CORE 0x1818
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_TA_CORE 0x1BF01818u

//! Register Reset Value
#define TUS1_TA_CORE_RST 0x000050C500270001u

//! Field REV_CODE - rev_code
#define TUS1_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TUS1_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TUS1_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TUS1_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TUS1_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TUS1_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TUS1_TA_AGENT_CONTROL Register TUS1_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TUS1_TA_AGENT_CONTROL 0x1820
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_TA_AGENT_CONTROL 0x1BF01820u

//! Register Reset Value
#define TUS1_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TUS1_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TUS1_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TUS1_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TUS1_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TUS1_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TUS1_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TUS1_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TUS1_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TUS1_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TUS1_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TUS1_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TUS1_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TUS1_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TUS1_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TUS1_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TUS1_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TUS1_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TUS1_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TUS1_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TUS1_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TUS1_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TUS1_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TUS1_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TUS1_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TUS1_TA_AGENT_STATUS Register TUS1_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TUS1_TA_AGENT_STATUS 0x1828
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_TA_AGENT_STATUS 0x1BF01828u

//! Register Reset Value
#define TUS1_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TUS1_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TUS1_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TUS1_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TUS1_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TUS1_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TUS1_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TUS1_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TUS1_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TUS1_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TUS1_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TUS1_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TUS1_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TUS1_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TUS1_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TUS1_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TUS1_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TUS1_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TUS1_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TUS1_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TUS1_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TUS1_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TUS1_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TUS1_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TUS1_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TUS1_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TUS1_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TUS1_TA_ERROR_LOG Register TUS1_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TUS1_TA_ERROR_LOG 0x1858
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_TA_ERROR_LOG 0x1BF01858u

//! Register Reset Value
#define TUS1_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TUS1_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TUS1_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TUS1_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TUS1_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TUS1_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TUS1_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TUS1_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TUS1_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TUS1_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TUS1_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TUS1_TA_ERROR_LOG_ADDR Register TUS1_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TUS1_TA_ERROR_LOG_ADDR 0x1860
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_TA_ERROR_LOG_ADDR 0x1BF01860u

//! Register Reset Value
#define TUS1_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TUS1_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TUS1_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_TA_BANDWIDTH_0 Register TUS1_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TUS1_TA_BANDWIDTH_0 0x1900
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_TA_BANDWIDTH_0 0x1BF01900u

//! Register Reset Value
#define TUS1_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TUS1_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TUS1_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TUS1_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TUS1_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TUS1_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TUS1_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TUS1_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TUS1_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS1_TA_BANDWIDTH_1 Register TUS1_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TUS1_TA_BANDWIDTH_1 0x1908
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_TA_BANDWIDTH_1 0x1BF01908u

//! Register Reset Value
#define TUS1_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TUS1_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TUS1_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TUS1_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TUS1_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TUS1_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TUS1_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TUS1_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TUS1_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS1_TA_BANDWIDTH_2 Register TUS1_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TUS1_TA_BANDWIDTH_2 0x1910
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_TA_BANDWIDTH_2 0x1BF01910u

//! Register Reset Value
#define TUS1_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TUS1_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TUS1_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TUS1_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TUS1_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TUS1_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TUS1_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TUS1_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TUS1_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS1_TA_BANDWIDTH_3 Register TUS1_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TUS1_TA_BANDWIDTH_3 0x1918
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_TA_BANDWIDTH_3 0x1BF01918u

//! Register Reset Value
#define TUS1_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TUS1_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TUS1_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TUS1_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TUS1_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TUS1_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TUS1_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TUS1_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TUS1_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS1_TA_ALLOC_LIMIT_0 Register TUS1_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TUS1_TA_ALLOC_LIMIT_0 0x1A00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_TA_ALLOC_LIMIT_0 0x1BF01A00u

//! Register Reset Value
#define TUS1_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TUS1_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TUS1_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TUS1_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TUS1_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TUS1_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TUS1_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TUS1_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TUS1_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TUS1_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TUS1_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TUS1_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TUS1_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TUS1_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TUS1_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TUS1_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TUS1_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS1_TA_ALLOC_LIMIT_1 Register TUS1_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TUS1_TA_ALLOC_LIMIT_1 0x1A08
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_TA_ALLOC_LIMIT_1 0x1BF01A08u

//! Register Reset Value
#define TUS1_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TUS1_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TUS1_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TUS1_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TUS1_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TUS1_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TUS1_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TUS1_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TUS1_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TUS1_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TUS1_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TUS1_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TUS1_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TUS1_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TUS1_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TUS1_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TUS1_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS1_TA_ALLOC_LIMIT_2 Register TUS1_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TUS1_TA_ALLOC_LIMIT_2 0x1A10
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_TA_ALLOC_LIMIT_2 0x1BF01A10u

//! Register Reset Value
#define TUS1_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TUS1_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TUS1_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TUS1_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TUS1_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TUS1_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TUS1_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TUS1_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TUS1_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TUS1_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TUS1_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TUS1_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TUS1_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TUS1_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TUS1_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TUS1_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TUS1_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS1_TA_ALLOC_LIMIT_3 Register TUS1_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TUS1_TA_ALLOC_LIMIT_3 0x1A18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_TA_ALLOC_LIMIT_3 0x1BF01A18u

//! Register Reset Value
#define TUS1_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TUS1_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TUS1_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TUS1_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TUS1_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TUS1_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TUS1_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TUS1_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TUS1_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TUS1_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TUS1_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TUS1_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TUS1_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TUS1_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TUS1_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TUS1_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TUS1_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS1C_TA_COMPONENT Register TUS1C_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TUS1C_TA_COMPONENT 0x1C00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_TA_COMPONENT 0x1BF01C00u

//! Register Reset Value
#define TUS1C_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TUS1C_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TUS1C_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TUS1C_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TUS1C_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TUS1C_TA_CORE Register TUS1C_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TUS1C_TA_CORE 0x1C18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_TA_CORE 0x1BF01C18u

//! Register Reset Value
#define TUS1C_TA_CORE_RST 0x000088C300280001u

//! Field REV_CODE - rev_code
#define TUS1C_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TUS1C_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TUS1C_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TUS1C_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TUS1C_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TUS1C_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TUS1C_TA_AGENT_CONTROL Register TUS1C_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TUS1C_TA_AGENT_CONTROL 0x1C20
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_TA_AGENT_CONTROL 0x1BF01C20u

//! Register Reset Value
#define TUS1C_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TUS1C_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TUS1C_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TUS1C_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TUS1C_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TUS1C_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TUS1C_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TUS1C_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TUS1C_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TUS1C_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TUS1C_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TUS1C_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TUS1C_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TUS1C_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TUS1C_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TUS1C_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TUS1C_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TUS1C_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TUS1C_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TUS1C_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TUS1C_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TUS1C_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TUS1C_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TUS1C_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TUS1C_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TUS1C_TA_AGENT_STATUS Register TUS1C_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TUS1C_TA_AGENT_STATUS 0x1C28
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_TA_AGENT_STATUS 0x1BF01C28u

//! Register Reset Value
#define TUS1C_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TUS1C_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TUS1C_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TUS1C_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TUS1C_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TUS1C_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TUS1C_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TUS1C_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TUS1C_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TUS1C_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TUS1C_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TUS1C_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TUS1C_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TUS1C_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TUS1C_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TUS1C_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TUS1C_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TUS1C_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TUS1C_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TUS1C_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TUS1C_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TUS1C_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TUS1C_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TUS1C_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TUS1C_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TUS1C_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TUS1C_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TUS1C_TA_ERROR_LOG Register TUS1C_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TUS1C_TA_ERROR_LOG 0x1C58
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_TA_ERROR_LOG 0x1BF01C58u

//! Register Reset Value
#define TUS1C_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TUS1C_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TUS1C_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TUS1C_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TUS1C_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TUS1C_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TUS1C_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TUS1C_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TUS1C_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TUS1C_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TUS1C_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TUS1C_TA_ERROR_LOG_ADDR Register TUS1C_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TUS1C_TA_ERROR_LOG_ADDR 0x1C60
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_TA_ERROR_LOG_ADDR 0x1BF01C60u

//! Register Reset Value
#define TUS1C_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TUS1C_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TUS1C_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_TA_BANDWIDTH_0 Register TUS1C_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TUS1C_TA_BANDWIDTH_0 0x1D00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_TA_BANDWIDTH_0 0x1BF01D00u

//! Register Reset Value
#define TUS1C_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TUS1C_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TUS1C_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TUS1C_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TUS1C_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TUS1C_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TUS1C_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TUS1C_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TUS1C_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS1C_TA_BANDWIDTH_1 Register TUS1C_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TUS1C_TA_BANDWIDTH_1 0x1D08
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_TA_BANDWIDTH_1 0x1BF01D08u

//! Register Reset Value
#define TUS1C_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TUS1C_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TUS1C_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TUS1C_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TUS1C_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TUS1C_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TUS1C_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TUS1C_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TUS1C_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS1C_TA_BANDWIDTH_2 Register TUS1C_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TUS1C_TA_BANDWIDTH_2 0x1D10
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_TA_BANDWIDTH_2 0x1BF01D10u

//! Register Reset Value
#define TUS1C_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TUS1C_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TUS1C_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TUS1C_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TUS1C_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TUS1C_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TUS1C_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TUS1C_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TUS1C_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS1C_TA_BANDWIDTH_3 Register TUS1C_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TUS1C_TA_BANDWIDTH_3 0x1D18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_TA_BANDWIDTH_3 0x1BF01D18u

//! Register Reset Value
#define TUS1C_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TUS1C_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TUS1C_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TUS1C_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TUS1C_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TUS1C_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TUS1C_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TUS1C_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TUS1C_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS1C_TA_ALLOC_LIMIT_0 Register TUS1C_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TUS1C_TA_ALLOC_LIMIT_0 0x1E00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_TA_ALLOC_LIMIT_0 0x1BF01E00u

//! Register Reset Value
#define TUS1C_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TUS1C_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TUS1C_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TUS1C_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TUS1C_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TUS1C_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TUS1C_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TUS1C_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TUS1C_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TUS1C_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TUS1C_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TUS1C_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TUS1C_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TUS1C_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TUS1C_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TUS1C_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TUS1C_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS1C_TA_ALLOC_LIMIT_1 Register TUS1C_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TUS1C_TA_ALLOC_LIMIT_1 0x1E08
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_TA_ALLOC_LIMIT_1 0x1BF01E08u

//! Register Reset Value
#define TUS1C_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TUS1C_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TUS1C_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TUS1C_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TUS1C_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TUS1C_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TUS1C_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TUS1C_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TUS1C_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TUS1C_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TUS1C_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TUS1C_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TUS1C_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TUS1C_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TUS1C_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TUS1C_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TUS1C_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS1C_TA_ALLOC_LIMIT_2 Register TUS1C_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TUS1C_TA_ALLOC_LIMIT_2 0x1E10
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_TA_ALLOC_LIMIT_2 0x1BF01E10u

//! Register Reset Value
#define TUS1C_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TUS1C_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TUS1C_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TUS1C_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TUS1C_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TUS1C_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TUS1C_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TUS1C_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TUS1C_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TUS1C_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TUS1C_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TUS1C_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TUS1C_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TUS1C_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TUS1C_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TUS1C_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TUS1C_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TUS1C_TA_ALLOC_LIMIT_3 Register TUS1C_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TUS1C_TA_ALLOC_LIMIT_3 0x1E18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_TA_ALLOC_LIMIT_3 0x1BF01E18u

//! Register Reset Value
#define TUS1C_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TUS1C_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TUS1C_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TUS1C_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TUS1C_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TUS1C_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TUS1C_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TUS1C_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TUS1C_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TUS1C_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TUS1C_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TUS1C_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TUS1C_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TUS1C_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TUS1C_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TUS1C_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TUS1C_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM1R_TA_COMPONENT Register TDM1R_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TDM1R_TA_COMPONENT 0x2000
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_TA_COMPONENT 0x1BF02000u

//! Register Reset Value
#define TDM1R_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TDM1R_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TDM1R_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TDM1R_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TDM1R_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TDM1R_TA_CORE Register TDM1R_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TDM1R_TA_CORE 0x2018
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_TA_CORE 0x1BF02018u

//! Register Reset Value
#define TDM1R_TA_CORE_RST 0x000088C300240001u

//! Field REV_CODE - rev_code
#define TDM1R_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TDM1R_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TDM1R_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TDM1R_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TDM1R_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TDM1R_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TDM1R_TA_AGENT_CONTROL Register TDM1R_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TDM1R_TA_AGENT_CONTROL 0x2020
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_TA_AGENT_CONTROL 0x1BF02020u

//! Register Reset Value
#define TDM1R_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TDM1R_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TDM1R_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TDM1R_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TDM1R_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TDM1R_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TDM1R_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDM1R_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDM1R_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TDM1R_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TDM1R_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TDM1R_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TDM1R_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TDM1R_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TDM1R_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TDM1R_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TDM1R_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TDM1R_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TDM1R_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TDM1R_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TDM1R_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TDM1R_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TDM1R_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TDM1R_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TDM1R_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TDM1R_TA_AGENT_STATUS Register TDM1R_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TDM1R_TA_AGENT_STATUS 0x2028
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_TA_AGENT_STATUS 0x1BF02028u

//! Register Reset Value
#define TDM1R_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TDM1R_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TDM1R_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDM1R_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDM1R_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TDM1R_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TDM1R_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TDM1R_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TDM1R_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TDM1R_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TDM1R_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TDM1R_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TDM1R_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TDM1R_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TDM1R_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TDM1R_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TDM1R_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TDM1R_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TDM1R_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TDM1R_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TDM1R_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TDM1R_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TDM1R_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TDM1R_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TDM1R_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TDM1R_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TDM1R_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TDM1R_TA_ERROR_LOG Register TDM1R_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TDM1R_TA_ERROR_LOG 0x2058
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_TA_ERROR_LOG 0x1BF02058u

//! Register Reset Value
#define TDM1R_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TDM1R_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TDM1R_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TDM1R_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TDM1R_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TDM1R_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TDM1R_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TDM1R_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TDM1R_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TDM1R_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TDM1R_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TDM1R_TA_ERROR_LOG_ADDR Register TDM1R_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TDM1R_TA_ERROR_LOG_ADDR 0x2060
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_TA_ERROR_LOG_ADDR 0x1BF02060u

//! Register Reset Value
#define TDM1R_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TDM1R_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TDM1R_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_TA_BANDWIDTH_0 Register TDM1R_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TDM1R_TA_BANDWIDTH_0 0x2100
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_TA_BANDWIDTH_0 0x1BF02100u

//! Register Reset Value
#define TDM1R_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TDM1R_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TDM1R_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TDM1R_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TDM1R_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TDM1R_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TDM1R_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TDM1R_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TDM1R_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM1R_TA_BANDWIDTH_1 Register TDM1R_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TDM1R_TA_BANDWIDTH_1 0x2108
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_TA_BANDWIDTH_1 0x1BF02108u

//! Register Reset Value
#define TDM1R_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TDM1R_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TDM1R_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TDM1R_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TDM1R_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TDM1R_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TDM1R_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TDM1R_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TDM1R_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM1R_TA_BANDWIDTH_2 Register TDM1R_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TDM1R_TA_BANDWIDTH_2 0x2110
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_TA_BANDWIDTH_2 0x1BF02110u

//! Register Reset Value
#define TDM1R_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TDM1R_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TDM1R_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TDM1R_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TDM1R_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TDM1R_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TDM1R_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TDM1R_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TDM1R_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM1R_TA_BANDWIDTH_3 Register TDM1R_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TDM1R_TA_BANDWIDTH_3 0x2118
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_TA_BANDWIDTH_3 0x1BF02118u

//! Register Reset Value
#define TDM1R_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TDM1R_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TDM1R_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TDM1R_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TDM1R_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TDM1R_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TDM1R_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TDM1R_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TDM1R_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM1R_TA_ALLOC_LIMIT_0 Register TDM1R_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TDM1R_TA_ALLOC_LIMIT_0 0x2200
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_TA_ALLOC_LIMIT_0 0x1BF02200u

//! Register Reset Value
#define TDM1R_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TDM1R_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TDM1R_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TDM1R_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TDM1R_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TDM1R_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TDM1R_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TDM1R_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TDM1R_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TDM1R_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TDM1R_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TDM1R_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TDM1R_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TDM1R_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TDM1R_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TDM1R_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TDM1R_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM1R_TA_ALLOC_LIMIT_1 Register TDM1R_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TDM1R_TA_ALLOC_LIMIT_1 0x2208
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_TA_ALLOC_LIMIT_1 0x1BF02208u

//! Register Reset Value
#define TDM1R_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TDM1R_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TDM1R_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TDM1R_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TDM1R_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TDM1R_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TDM1R_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TDM1R_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TDM1R_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TDM1R_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TDM1R_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TDM1R_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TDM1R_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TDM1R_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TDM1R_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TDM1R_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TDM1R_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM1R_TA_ALLOC_LIMIT_2 Register TDM1R_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TDM1R_TA_ALLOC_LIMIT_2 0x2210
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_TA_ALLOC_LIMIT_2 0x1BF02210u

//! Register Reset Value
#define TDM1R_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TDM1R_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TDM1R_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TDM1R_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TDM1R_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TDM1R_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TDM1R_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TDM1R_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TDM1R_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TDM1R_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TDM1R_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TDM1R_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TDM1R_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TDM1R_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TDM1R_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TDM1R_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TDM1R_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM1R_TA_ALLOC_LIMIT_3 Register TDM1R_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TDM1R_TA_ALLOC_LIMIT_3 0x2218
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_TA_ALLOC_LIMIT_3 0x1BF02218u

//! Register Reset Value
#define TDM1R_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TDM1R_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TDM1R_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TDM1R_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TDM1R_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TDM1R_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TDM1R_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TDM1R_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TDM1R_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TDM1R_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TDM1R_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TDM1R_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TDM1R_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TDM1R_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TDM1R_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TDM1R_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TDM1R_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM1T_TA_COMPONENT Register TDM1T_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TDM1T_TA_COMPONENT 0x2400
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_TA_COMPONENT 0x1BF02400u

//! Register Reset Value
#define TDM1T_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TDM1T_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TDM1T_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TDM1T_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TDM1T_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TDM1T_TA_CORE Register TDM1T_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TDM1T_TA_CORE 0x2418
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_TA_CORE 0x1BF02418u

//! Register Reset Value
#define TDM1T_TA_CORE_RST 0x000088C300230001u

//! Field REV_CODE - rev_code
#define TDM1T_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TDM1T_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TDM1T_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TDM1T_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TDM1T_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TDM1T_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TDM1T_TA_AGENT_CONTROL Register TDM1T_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TDM1T_TA_AGENT_CONTROL 0x2420
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_TA_AGENT_CONTROL 0x1BF02420u

//! Register Reset Value
#define TDM1T_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TDM1T_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TDM1T_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TDM1T_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TDM1T_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TDM1T_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TDM1T_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDM1T_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDM1T_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TDM1T_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TDM1T_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TDM1T_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TDM1T_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TDM1T_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TDM1T_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TDM1T_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TDM1T_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TDM1T_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TDM1T_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TDM1T_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TDM1T_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TDM1T_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TDM1T_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TDM1T_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TDM1T_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TDM1T_TA_AGENT_STATUS Register TDM1T_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TDM1T_TA_AGENT_STATUS 0x2428
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_TA_AGENT_STATUS 0x1BF02428u

//! Register Reset Value
#define TDM1T_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TDM1T_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TDM1T_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDM1T_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TDM1T_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TDM1T_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TDM1T_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TDM1T_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TDM1T_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TDM1T_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TDM1T_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TDM1T_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TDM1T_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TDM1T_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TDM1T_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TDM1T_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TDM1T_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TDM1T_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TDM1T_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TDM1T_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TDM1T_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TDM1T_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TDM1T_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TDM1T_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TDM1T_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TDM1T_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TDM1T_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TDM1T_TA_ERROR_LOG Register TDM1T_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TDM1T_TA_ERROR_LOG 0x2458
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_TA_ERROR_LOG 0x1BF02458u

//! Register Reset Value
#define TDM1T_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TDM1T_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TDM1T_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TDM1T_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TDM1T_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TDM1T_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TDM1T_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TDM1T_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TDM1T_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TDM1T_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TDM1T_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TDM1T_TA_ERROR_LOG_ADDR Register TDM1T_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TDM1T_TA_ERROR_LOG_ADDR 0x2460
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_TA_ERROR_LOG_ADDR 0x1BF02460u

//! Register Reset Value
#define TDM1T_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TDM1T_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TDM1T_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_TA_BANDWIDTH_0 Register TDM1T_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TDM1T_TA_BANDWIDTH_0 0x2500
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_TA_BANDWIDTH_0 0x1BF02500u

//! Register Reset Value
#define TDM1T_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TDM1T_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TDM1T_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TDM1T_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TDM1T_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TDM1T_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TDM1T_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TDM1T_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TDM1T_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM1T_TA_BANDWIDTH_1 Register TDM1T_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TDM1T_TA_BANDWIDTH_1 0x2508
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_TA_BANDWIDTH_1 0x1BF02508u

//! Register Reset Value
#define TDM1T_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TDM1T_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TDM1T_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TDM1T_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TDM1T_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TDM1T_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TDM1T_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TDM1T_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TDM1T_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM1T_TA_BANDWIDTH_2 Register TDM1T_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TDM1T_TA_BANDWIDTH_2 0x2510
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_TA_BANDWIDTH_2 0x1BF02510u

//! Register Reset Value
#define TDM1T_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TDM1T_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TDM1T_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TDM1T_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TDM1T_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TDM1T_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TDM1T_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TDM1T_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TDM1T_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM1T_TA_BANDWIDTH_3 Register TDM1T_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TDM1T_TA_BANDWIDTH_3 0x2518
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_TA_BANDWIDTH_3 0x1BF02518u

//! Register Reset Value
#define TDM1T_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TDM1T_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TDM1T_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TDM1T_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TDM1T_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TDM1T_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TDM1T_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TDM1T_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TDM1T_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM1T_TA_ALLOC_LIMIT_0 Register TDM1T_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TDM1T_TA_ALLOC_LIMIT_0 0x2600
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_TA_ALLOC_LIMIT_0 0x1BF02600u

//! Register Reset Value
#define TDM1T_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TDM1T_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TDM1T_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TDM1T_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TDM1T_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TDM1T_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TDM1T_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TDM1T_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TDM1T_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TDM1T_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TDM1T_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TDM1T_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TDM1T_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TDM1T_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TDM1T_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TDM1T_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TDM1T_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM1T_TA_ALLOC_LIMIT_1 Register TDM1T_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TDM1T_TA_ALLOC_LIMIT_1 0x2608
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_TA_ALLOC_LIMIT_1 0x1BF02608u

//! Register Reset Value
#define TDM1T_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TDM1T_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TDM1T_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TDM1T_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TDM1T_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TDM1T_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TDM1T_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TDM1T_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TDM1T_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TDM1T_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TDM1T_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TDM1T_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TDM1T_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TDM1T_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TDM1T_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TDM1T_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TDM1T_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM1T_TA_ALLOC_LIMIT_2 Register TDM1T_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TDM1T_TA_ALLOC_LIMIT_2 0x2610
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_TA_ALLOC_LIMIT_2 0x1BF02610u

//! Register Reset Value
#define TDM1T_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TDM1T_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TDM1T_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TDM1T_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TDM1T_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TDM1T_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TDM1T_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TDM1T_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TDM1T_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TDM1T_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TDM1T_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TDM1T_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TDM1T_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TDM1T_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TDM1T_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TDM1T_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TDM1T_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TDM1T_TA_ALLOC_LIMIT_3 Register TDM1T_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TDM1T_TA_ALLOC_LIMIT_3 0x2618
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_TA_ALLOC_LIMIT_3 0x1BF02618u

//! Register Reset Value
#define TDM1T_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TDM1T_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TDM1T_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TDM1T_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TDM1T_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TDM1T_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TDM1T_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TDM1T_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TDM1T_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TDM1T_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TDM1T_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TDM1T_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TDM1T_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TDM1T_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TDM1T_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TDM1T_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TDM1T_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TGWPR_TA_COMPONENT Register TGWPR_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TGWPR_TA_COMPONENT 0x2800
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_TA_COMPONENT 0x1BF02800u

//! Register Reset Value
#define TGWPR_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TGWPR_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TGWPR_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TGWPR_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TGWPR_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TGWPR_TA_CORE Register TGWPR_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TGWPR_TA_CORE 0x2818
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_TA_CORE 0x1BF02818u

//! Register Reset Value
#define TGWPR_TA_CORE_RST 0x000050C500220001u

//! Field REV_CODE - rev_code
#define TGWPR_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TGWPR_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TGWPR_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TGWPR_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TGWPR_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TGWPR_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TGWPR_TA_AGENT_CONTROL Register TGWPR_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TGWPR_TA_AGENT_CONTROL 0x2820
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_TA_AGENT_CONTROL 0x1BF02820u

//! Register Reset Value
#define TGWPR_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TGWPR_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TGWPR_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TGWPR_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TGWPR_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TGWPR_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TGWPR_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TGWPR_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TGWPR_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TGWPR_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TGWPR_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TGWPR_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TGWPR_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TGWPR_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TGWPR_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TGWPR_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TGWPR_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TGWPR_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TGWPR_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TGWPR_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TGWPR_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TGWPR_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TGWPR_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TGWPR_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TGWPR_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TGWPR_TA_AGENT_STATUS Register TGWPR_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TGWPR_TA_AGENT_STATUS 0x2828
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_TA_AGENT_STATUS 0x1BF02828u

//! Register Reset Value
#define TGWPR_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TGWPR_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TGWPR_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TGWPR_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TGWPR_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TGWPR_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TGWPR_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TGWPR_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TGWPR_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TGWPR_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TGWPR_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TGWPR_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TGWPR_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TGWPR_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TGWPR_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TGWPR_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TGWPR_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TGWPR_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TGWPR_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TGWPR_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TGWPR_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TGWPR_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TGWPR_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TGWPR_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TGWPR_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TGWPR_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TGWPR_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TGWPR_TA_ERROR_LOG Register TGWPR_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TGWPR_TA_ERROR_LOG 0x2858
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_TA_ERROR_LOG 0x1BF02858u

//! Register Reset Value
#define TGWPR_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TGWPR_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TGWPR_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TGWPR_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TGWPR_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TGWPR_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TGWPR_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TGWPR_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TGWPR_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TGWPR_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TGWPR_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TGWPR_TA_ERROR_LOG_ADDR Register TGWPR_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TGWPR_TA_ERROR_LOG_ADDR 0x2860
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_TA_ERROR_LOG_ADDR 0x1BF02860u

//! Register Reset Value
#define TGWPR_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TGWPR_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TGWPR_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_TA_BANDWIDTH_0 Register TGWPR_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TGWPR_TA_BANDWIDTH_0 0x2900
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_TA_BANDWIDTH_0 0x1BF02900u

//! Register Reset Value
#define TGWPR_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TGWPR_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TGWPR_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TGWPR_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TGWPR_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TGWPR_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TGWPR_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TGWPR_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TGWPR_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TGWPR_TA_BANDWIDTH_1 Register TGWPR_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TGWPR_TA_BANDWIDTH_1 0x2908
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_TA_BANDWIDTH_1 0x1BF02908u

//! Register Reset Value
#define TGWPR_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TGWPR_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TGWPR_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TGWPR_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TGWPR_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TGWPR_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TGWPR_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TGWPR_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TGWPR_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TGWPR_TA_BANDWIDTH_2 Register TGWPR_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TGWPR_TA_BANDWIDTH_2 0x2910
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_TA_BANDWIDTH_2 0x1BF02910u

//! Register Reset Value
#define TGWPR_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TGWPR_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TGWPR_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TGWPR_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TGWPR_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TGWPR_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TGWPR_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TGWPR_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TGWPR_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TGWPR_TA_BANDWIDTH_3 Register TGWPR_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TGWPR_TA_BANDWIDTH_3 0x2918
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_TA_BANDWIDTH_3 0x1BF02918u

//! Register Reset Value
#define TGWPR_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TGWPR_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TGWPR_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TGWPR_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TGWPR_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TGWPR_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TGWPR_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TGWPR_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TGWPR_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TGWPR_TA_ALLOC_LIMIT_0 Register TGWPR_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TGWPR_TA_ALLOC_LIMIT_0 0x2A00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_TA_ALLOC_LIMIT_0 0x1BF02A00u

//! Register Reset Value
#define TGWPR_TA_ALLOC_LIMIT_0_RST 0x0000000000000000u

//! Field MIN_VALUE_0 - min_value_0
#define TGWPR_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TGWPR_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TGWPR_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TGWPR_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TGWPR_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TGWPR_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TGWPR_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TGWPR_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TGWPR_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TGWPR_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TGWPR_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TGWPR_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TGWPR_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TGWPR_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TGWPR_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TGWPR_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TGWPR_TA_ALLOC_LIMIT_1 Register TGWPR_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TGWPR_TA_ALLOC_LIMIT_1 0x2A08
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_TA_ALLOC_LIMIT_1 0x1BF02A08u

//! Register Reset Value
#define TGWPR_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TGWPR_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TGWPR_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TGWPR_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TGWPR_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TGWPR_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TGWPR_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TGWPR_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TGWPR_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TGWPR_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TGWPR_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TGWPR_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TGWPR_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TGWPR_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TGWPR_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TGWPR_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TGWPR_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TGWPR_TA_ALLOC_LIMIT_2 Register TGWPR_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TGWPR_TA_ALLOC_LIMIT_2 0x2A10
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_TA_ALLOC_LIMIT_2 0x1BF02A10u

//! Register Reset Value
#define TGWPR_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TGWPR_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TGWPR_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TGWPR_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TGWPR_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TGWPR_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TGWPR_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TGWPR_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TGWPR_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TGWPR_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TGWPR_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TGWPR_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TGWPR_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TGWPR_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TGWPR_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TGWPR_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TGWPR_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TGWPR_TA_ALLOC_LIMIT_3 Register TGWPR_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TGWPR_TA_ALLOC_LIMIT_3 0x2A18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_TA_ALLOC_LIMIT_3 0x1BF02A18u

//! Register Reset Value
#define TGWPR_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TGWPR_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TGWPR_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TGWPR_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TGWPR_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TGWPR_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TGWPR_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TGWPR_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TGWPR_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TGWPR_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TGWPR_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TGWPR_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TGWPR_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TGWPR_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TGWPR_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TGWPR_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TGWPR_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN20_TA_COMPONENT Register TLN20_TA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define TLN20_TA_COMPONENT 0x2C00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_TA_COMPONENT 0x1BF02C00u

//! Register Reset Value
#define TLN20_TA_COMPONENT_RST 0x0000000060203532u

//! Field REV - rev
#define TLN20_TA_COMPONENT_REV_POS 0
//! Field REV - rev
#define TLN20_TA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define TLN20_TA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define TLN20_TA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup TLN20_TA_CORE Register TLN20_TA_CORE - core
//! @{

//! Register Offset (relative)
#define TLN20_TA_CORE 0x2C18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_TA_CORE 0x1BF02C18u

//! Register Reset Value
#define TLN20_TA_CORE_RST 0x000050C50F200001u

//! Field REV_CODE - rev_code
#define TLN20_TA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define TLN20_TA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define TLN20_TA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define TLN20_TA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define TLN20_TA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define TLN20_TA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup TLN20_TA_AGENT_CONTROL Register TLN20_TA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define TLN20_TA_AGENT_CONTROL 0x2C20
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_TA_AGENT_CONTROL 0x1BF02C20u

//! Register Reset Value
#define TLN20_TA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TLN20_TA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TLN20_TA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define TLN20_TA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define TLN20_TA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field REQ_TIMEOUT - req_timeout
#define TLN20_TA_AGENT_CONTROL_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TLN20_TA_AGENT_CONTROL_REQ_TIMEOUT_MASK 0x700u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TLN20_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_POS 16
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TLN20_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_MASK 0x70000u

//! Field SERROR_REP - serror_rep
#define TLN20_TA_AGENT_CONTROL_SERROR_REP_POS 24
//! Field SERROR_REP - serror_rep
#define TLN20_TA_AGENT_CONTROL_SERROR_REP_MASK 0x1000000u

//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TLN20_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_POS 25
//! Field REQ_TIMEOUT_REP - req_timeout_rep
#define TLN20_TA_AGENT_CONTROL_REQ_TIMEOUT_REP_MASK 0x2000000u

//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TLN20_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_POS 26
//! Field FUNCTIONAL_RESET_TIMEOUT_REP - functional_reset_timeout_rep
#define TLN20_TA_AGENT_CONTROL_FUNCTIONAL_RESET_TIMEOUT_REP_MASK 0x4000000u

//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TLN20_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_POS 27
//! Field AUTO_WAKEUP_RESP_CODE - auto_wakeup_resp_code
#define TLN20_TA_AGENT_CONTROL_AUTO_WAKEUP_RESP_CODE_MASK 0x8000000u

//! Field RFU0 - rfu0
#define TLN20_TA_AGENT_CONTROL_RFU0_POS 28
//! Field RFU0 - rfu0
#define TLN20_TA_AGENT_CONTROL_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TLN20_TA_AGENT_CONTROL_RFU1_POS 29
//! Field RFU1 - rfu1
#define TLN20_TA_AGENT_CONTROL_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TLN20_TA_AGENT_CONTROL_RFU2_POS 30
//! Field RFU2 - rfu2
#define TLN20_TA_AGENT_CONTROL_RFU2_MASK 0x40000000u

//! Field RFU3 - rfu3
#define TLN20_TA_AGENT_CONTROL_RFU3_POS 31
//! Field RFU3 - rfu3
#define TLN20_TA_AGENT_CONTROL_RFU3_MASK 0x80000000u

//! @}

//! \defgroup TLN20_TA_AGENT_STATUS Register TLN20_TA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define TLN20_TA_AGENT_STATUS 0x2C28
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_TA_AGENT_STATUS 0x1BF02C28u

//! Register Reset Value
#define TLN20_TA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define TLN20_TA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define TLN20_TA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TLN20_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_POS 3
//! Field FUNCTIONAL_RESET_TIMEOUT - functional_reset_timeout
#define TLN20_TA_AGENT_STATUS_FUNCTIONAL_RESET_TIMEOUT_MASK 0x8u

//! Field REQ_WAITING - req_waiting
#define TLN20_TA_AGENT_STATUS_REQ_WAITING_POS 4
//! Field REQ_WAITING - req_waiting
#define TLN20_TA_AGENT_STATUS_REQ_WAITING_MASK 0x10u

//! Field RESP_ACTIVE - resp_active
#define TLN20_TA_AGENT_STATUS_RESP_ACTIVE_POS 5
//! Field RESP_ACTIVE - resp_active
#define TLN20_TA_AGENT_STATUS_RESP_ACTIVE_MASK 0x20u

//! Field BURST - burst
#define TLN20_TA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define TLN20_TA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define TLN20_TA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define TLN20_TA_AGENT_STATUS_READEX_MASK 0x80u

//! Field REQ_TIMEOUT - req_timeout
#define TLN20_TA_AGENT_STATUS_REQ_TIMEOUT_POS 8
//! Field REQ_TIMEOUT - req_timeout
#define TLN20_TA_AGENT_STATUS_REQ_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define TLN20_TA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define TLN20_TA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_CLOSE - burst_close
#define TLN20_TA_AGENT_STATUS_BURST_CLOSE_POS 16
//! Field BURST_CLOSE - burst_close
#define TLN20_TA_AGENT_STATUS_BURST_CLOSE_MASK 0x10000u

//! Field SERROR - serror
#define TLN20_TA_AGENT_STATUS_SERROR_POS 24
//! Field SERROR - serror
#define TLN20_TA_AGENT_STATUS_SERROR_MASK 0x1000000u

//! Field RFU0 - rfu0
#define TLN20_TA_AGENT_STATUS_RFU0_POS 28
//! Field RFU0 - rfu0
#define TLN20_TA_AGENT_STATUS_RFU0_MASK 0x10000000u

//! Field RFU1 - rfu1
#define TLN20_TA_AGENT_STATUS_RFU1_POS 29
//! Field RFU1 - rfu1
#define TLN20_TA_AGENT_STATUS_RFU1_MASK 0x20000000u

//! Field RFU2 - rfu2
#define TLN20_TA_AGENT_STATUS_RFU2_POS 31
//! Field RFU2 - rfu2
#define TLN20_TA_AGENT_STATUS_RFU2_MASK 0x80000000u

//! @}

//! \defgroup TLN20_TA_ERROR_LOG Register TLN20_TA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TLN20_TA_ERROR_LOG 0x2C58
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_TA_ERROR_LOG 0x1BF02C58u

//! Register Reset Value
#define TLN20_TA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TLN20_TA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TLN20_TA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define TLN20_TA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TLN20_TA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field CODE - code
#define TLN20_TA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TLN20_TA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field MULTI - multi
#define TLN20_TA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TLN20_TA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define TLN20_TA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define TLN20_TA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup TLN20_TA_ERROR_LOG_ADDR Register TLN20_TA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define TLN20_TA_ERROR_LOG_ADDR 0x2C60
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_TA_ERROR_LOG_ADDR 0x1BF02C60u

//! Register Reset Value
#define TLN20_TA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define TLN20_TA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define TLN20_TA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_TA_BANDWIDTH_0 Register TLN20_TA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define TLN20_TA_BANDWIDTH_0 0x2D00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_TA_BANDWIDTH_0 0x1BF02D00u

//! Register Reset Value
#define TLN20_TA_BANDWIDTH_0_RST 0x0000000000000000u

//! Field FRACTION_0 - fraction_0
#define TLN20_TA_BANDWIDTH_0_FRACTION_0_POS 8
//! Field FRACTION_0 - fraction_0
#define TLN20_TA_BANDWIDTH_0_FRACTION_0_MASK 0xFF00u

//! Field FRACTION_1 - fraction_1
#define TLN20_TA_BANDWIDTH_0_FRACTION_1_POS 24
//! Field FRACTION_1 - fraction_1
#define TLN20_TA_BANDWIDTH_0_FRACTION_1_MASK 0xFF000000u

//! Field FRACTION_2 - fraction_2
#define TLN20_TA_BANDWIDTH_0_FRACTION_2_POS 40
//! Field FRACTION_2 - fraction_2
#define TLN20_TA_BANDWIDTH_0_FRACTION_2_MASK 0xFF0000000000u

//! Field FRACTION_3 - fraction_3
#define TLN20_TA_BANDWIDTH_0_FRACTION_3_POS 56
//! Field FRACTION_3 - fraction_3
#define TLN20_TA_BANDWIDTH_0_FRACTION_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN20_TA_BANDWIDTH_1 Register TLN20_TA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define TLN20_TA_BANDWIDTH_1 0x2D08
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_TA_BANDWIDTH_1 0x1BF02D08u

//! Register Reset Value
#define TLN20_TA_BANDWIDTH_1_RST 0x0000000000000000u

//! Field FRACTION_4 - fraction_4
#define TLN20_TA_BANDWIDTH_1_FRACTION_4_POS 8
//! Field FRACTION_4 - fraction_4
#define TLN20_TA_BANDWIDTH_1_FRACTION_4_MASK 0xFF00u

//! Field FRACTION_5 - fraction_5
#define TLN20_TA_BANDWIDTH_1_FRACTION_5_POS 24
//! Field FRACTION_5 - fraction_5
#define TLN20_TA_BANDWIDTH_1_FRACTION_5_MASK 0xFF000000u

//! Field FRACTION_6 - fraction_6
#define TLN20_TA_BANDWIDTH_1_FRACTION_6_POS 40
//! Field FRACTION_6 - fraction_6
#define TLN20_TA_BANDWIDTH_1_FRACTION_6_MASK 0xFF0000000000u

//! Field FRACTION_7 - fraction_7
#define TLN20_TA_BANDWIDTH_1_FRACTION_7_POS 56
//! Field FRACTION_7 - fraction_7
#define TLN20_TA_BANDWIDTH_1_FRACTION_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN20_TA_BANDWIDTH_2 Register TLN20_TA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define TLN20_TA_BANDWIDTH_2 0x2D10
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_TA_BANDWIDTH_2 0x1BF02D10u

//! Register Reset Value
#define TLN20_TA_BANDWIDTH_2_RST 0x0000000000000000u

//! Field FRACTION_8 - fraction_8
#define TLN20_TA_BANDWIDTH_2_FRACTION_8_POS 8
//! Field FRACTION_8 - fraction_8
#define TLN20_TA_BANDWIDTH_2_FRACTION_8_MASK 0xFF00u

//! Field FRACTION_9 - fraction_9
#define TLN20_TA_BANDWIDTH_2_FRACTION_9_POS 24
//! Field FRACTION_9 - fraction_9
#define TLN20_TA_BANDWIDTH_2_FRACTION_9_MASK 0xFF000000u

//! Field FRACTION_10 - fraction_10
#define TLN20_TA_BANDWIDTH_2_FRACTION_10_POS 40
//! Field FRACTION_10 - fraction_10
#define TLN20_TA_BANDWIDTH_2_FRACTION_10_MASK 0xFF0000000000u

//! Field FRACTION_11 - fraction_11
#define TLN20_TA_BANDWIDTH_2_FRACTION_11_POS 56
//! Field FRACTION_11 - fraction_11
#define TLN20_TA_BANDWIDTH_2_FRACTION_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN20_TA_BANDWIDTH_3 Register TLN20_TA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define TLN20_TA_BANDWIDTH_3 0x2D18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_TA_BANDWIDTH_3 0x1BF02D18u

//! Register Reset Value
#define TLN20_TA_BANDWIDTH_3_RST 0x0000000000000000u

//! Field FRACTION_12 - fraction_12
#define TLN20_TA_BANDWIDTH_3_FRACTION_12_POS 8
//! Field FRACTION_12 - fraction_12
#define TLN20_TA_BANDWIDTH_3_FRACTION_12_MASK 0xFF00u

//! Field FRACTION_13 - fraction_13
#define TLN20_TA_BANDWIDTH_3_FRACTION_13_POS 24
//! Field FRACTION_13 - fraction_13
#define TLN20_TA_BANDWIDTH_3_FRACTION_13_MASK 0xFF000000u

//! Field FRACTION_14 - fraction_14
#define TLN20_TA_BANDWIDTH_3_FRACTION_14_POS 40
//! Field FRACTION_14 - fraction_14
#define TLN20_TA_BANDWIDTH_3_FRACTION_14_MASK 0xFF0000000000u

//! Field FRACTION_15 - fraction_15
#define TLN20_TA_BANDWIDTH_3_FRACTION_15_POS 56
//! Field FRACTION_15 - fraction_15
#define TLN20_TA_BANDWIDTH_3_FRACTION_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN20_TA_ALLOC_LIMIT_0 Register TLN20_TA_ALLOC_LIMIT_0 - alloc_limit_0
//! @{

//! Register Offset (relative)
#define TLN20_TA_ALLOC_LIMIT_0 0x2E00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_TA_ALLOC_LIMIT_0 0x1BF02E00u

//! Register Reset Value
#define TLN20_TA_ALLOC_LIMIT_0_RST 0x0101010101010101u

//! Field MIN_VALUE_0 - min_value_0
#define TLN20_TA_ALLOC_LIMIT_0_MIN_VALUE_0_POS 0
//! Field MIN_VALUE_0 - min_value_0
#define TLN20_TA_ALLOC_LIMIT_0_MIN_VALUE_0_MASK 0xFFu

//! Field MAX_VALUE_0 - max_value_0
#define TLN20_TA_ALLOC_LIMIT_0_MAX_VALUE_0_POS 8
//! Field MAX_VALUE_0 - max_value_0
#define TLN20_TA_ALLOC_LIMIT_0_MAX_VALUE_0_MASK 0xFF00u

//! Field MIN_VALUE_1 - min_value_1
#define TLN20_TA_ALLOC_LIMIT_0_MIN_VALUE_1_POS 16
//! Field MIN_VALUE_1 - min_value_1
#define TLN20_TA_ALLOC_LIMIT_0_MIN_VALUE_1_MASK 0xFF0000u

//! Field MAX_VALUE_1 - max_value_1
#define TLN20_TA_ALLOC_LIMIT_0_MAX_VALUE_1_POS 24
//! Field MAX_VALUE_1 - max_value_1
#define TLN20_TA_ALLOC_LIMIT_0_MAX_VALUE_1_MASK 0xFF000000u

//! Field MIN_VALUE_2 - min_value_2
#define TLN20_TA_ALLOC_LIMIT_0_MIN_VALUE_2_POS 32
//! Field MIN_VALUE_2 - min_value_2
#define TLN20_TA_ALLOC_LIMIT_0_MIN_VALUE_2_MASK 0xFF00000000u

//! Field MAX_VALUE_2 - max_value_2
#define TLN20_TA_ALLOC_LIMIT_0_MAX_VALUE_2_POS 40
//! Field MAX_VALUE_2 - max_value_2
#define TLN20_TA_ALLOC_LIMIT_0_MAX_VALUE_2_MASK 0xFF0000000000u

//! Field MIN_VALUE_3 - min_value_3
#define TLN20_TA_ALLOC_LIMIT_0_MIN_VALUE_3_POS 48
//! Field MIN_VALUE_3 - min_value_3
#define TLN20_TA_ALLOC_LIMIT_0_MIN_VALUE_3_MASK 0xFF000000000000u

//! Field MAX_VALUE_3 - max_value_3
#define TLN20_TA_ALLOC_LIMIT_0_MAX_VALUE_3_POS 56
//! Field MAX_VALUE_3 - max_value_3
#define TLN20_TA_ALLOC_LIMIT_0_MAX_VALUE_3_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN20_TA_ALLOC_LIMIT_1 Register TLN20_TA_ALLOC_LIMIT_1 - alloc_limit_1
//! @{

//! Register Offset (relative)
#define TLN20_TA_ALLOC_LIMIT_1 0x2E08
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_TA_ALLOC_LIMIT_1 0x1BF02E08u

//! Register Reset Value
#define TLN20_TA_ALLOC_LIMIT_1_RST 0x0000000000000000u

//! Field MIN_VALUE_4 - min_value_4
#define TLN20_TA_ALLOC_LIMIT_1_MIN_VALUE_4_POS 0
//! Field MIN_VALUE_4 - min_value_4
#define TLN20_TA_ALLOC_LIMIT_1_MIN_VALUE_4_MASK 0xFFu

//! Field MAX_VALUE_4 - max_value_4
#define TLN20_TA_ALLOC_LIMIT_1_MAX_VALUE_4_POS 8
//! Field MAX_VALUE_4 - max_value_4
#define TLN20_TA_ALLOC_LIMIT_1_MAX_VALUE_4_MASK 0xFF00u

//! Field MIN_VALUE_5 - min_value_5
#define TLN20_TA_ALLOC_LIMIT_1_MIN_VALUE_5_POS 16
//! Field MIN_VALUE_5 - min_value_5
#define TLN20_TA_ALLOC_LIMIT_1_MIN_VALUE_5_MASK 0xFF0000u

//! Field MAX_VALUE_5 - max_value_5
#define TLN20_TA_ALLOC_LIMIT_1_MAX_VALUE_5_POS 24
//! Field MAX_VALUE_5 - max_value_5
#define TLN20_TA_ALLOC_LIMIT_1_MAX_VALUE_5_MASK 0xFF000000u

//! Field MIN_VALUE_6 - min_value_6
#define TLN20_TA_ALLOC_LIMIT_1_MIN_VALUE_6_POS 32
//! Field MIN_VALUE_6 - min_value_6
#define TLN20_TA_ALLOC_LIMIT_1_MIN_VALUE_6_MASK 0xFF00000000u

//! Field MAX_VALUE_6 - max_value_6
#define TLN20_TA_ALLOC_LIMIT_1_MAX_VALUE_6_POS 40
//! Field MAX_VALUE_6 - max_value_6
#define TLN20_TA_ALLOC_LIMIT_1_MAX_VALUE_6_MASK 0xFF0000000000u

//! Field MIN_VALUE_7 - min_value_7
#define TLN20_TA_ALLOC_LIMIT_1_MIN_VALUE_7_POS 48
//! Field MIN_VALUE_7 - min_value_7
#define TLN20_TA_ALLOC_LIMIT_1_MIN_VALUE_7_MASK 0xFF000000000000u

//! Field MAX_VALUE_7 - max_value_7
#define TLN20_TA_ALLOC_LIMIT_1_MAX_VALUE_7_POS 56
//! Field MAX_VALUE_7 - max_value_7
#define TLN20_TA_ALLOC_LIMIT_1_MAX_VALUE_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN20_TA_ALLOC_LIMIT_2 Register TLN20_TA_ALLOC_LIMIT_2 - alloc_limit_2
//! @{

//! Register Offset (relative)
#define TLN20_TA_ALLOC_LIMIT_2 0x2E10
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_TA_ALLOC_LIMIT_2 0x1BF02E10u

//! Register Reset Value
#define TLN20_TA_ALLOC_LIMIT_2_RST 0x0000000000000000u

//! Field MIN_VALUE_8 - min_value_8
#define TLN20_TA_ALLOC_LIMIT_2_MIN_VALUE_8_POS 0
//! Field MIN_VALUE_8 - min_value_8
#define TLN20_TA_ALLOC_LIMIT_2_MIN_VALUE_8_MASK 0xFFu

//! Field MAX_VALUE_8 - max_value_8
#define TLN20_TA_ALLOC_LIMIT_2_MAX_VALUE_8_POS 8
//! Field MAX_VALUE_8 - max_value_8
#define TLN20_TA_ALLOC_LIMIT_2_MAX_VALUE_8_MASK 0xFF00u

//! Field MIN_VALUE_9 - min_value_9
#define TLN20_TA_ALLOC_LIMIT_2_MIN_VALUE_9_POS 16
//! Field MIN_VALUE_9 - min_value_9
#define TLN20_TA_ALLOC_LIMIT_2_MIN_VALUE_9_MASK 0xFF0000u

//! Field MAX_VALUE_9 - max_value_9
#define TLN20_TA_ALLOC_LIMIT_2_MAX_VALUE_9_POS 24
//! Field MAX_VALUE_9 - max_value_9
#define TLN20_TA_ALLOC_LIMIT_2_MAX_VALUE_9_MASK 0xFF000000u

//! Field MIN_VALUE_10 - min_value_10
#define TLN20_TA_ALLOC_LIMIT_2_MIN_VALUE_10_POS 32
//! Field MIN_VALUE_10 - min_value_10
#define TLN20_TA_ALLOC_LIMIT_2_MIN_VALUE_10_MASK 0xFF00000000u

//! Field MAX_VALUE_10 - max_value_10
#define TLN20_TA_ALLOC_LIMIT_2_MAX_VALUE_10_POS 40
//! Field MAX_VALUE_10 - max_value_10
#define TLN20_TA_ALLOC_LIMIT_2_MAX_VALUE_10_MASK 0xFF0000000000u

//! Field MIN_VALUE_11 - min_value_11
#define TLN20_TA_ALLOC_LIMIT_2_MIN_VALUE_11_POS 48
//! Field MIN_VALUE_11 - min_value_11
#define TLN20_TA_ALLOC_LIMIT_2_MIN_VALUE_11_MASK 0xFF000000000000u

//! Field MAX_VALUE_11 - max_value_11
#define TLN20_TA_ALLOC_LIMIT_2_MAX_VALUE_11_POS 56
//! Field MAX_VALUE_11 - max_value_11
#define TLN20_TA_ALLOC_LIMIT_2_MAX_VALUE_11_MASK 0xFF00000000000000u

//! @}

//! \defgroup TLN20_TA_ALLOC_LIMIT_3 Register TLN20_TA_ALLOC_LIMIT_3 - alloc_limit_3
//! @{

//! Register Offset (relative)
#define TLN20_TA_ALLOC_LIMIT_3 0x2E18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_TA_ALLOC_LIMIT_3 0x1BF02E18u

//! Register Reset Value
#define TLN20_TA_ALLOC_LIMIT_3_RST 0x0000000000000000u

//! Field MIN_VALUE_12 - min_value_12
#define TLN20_TA_ALLOC_LIMIT_3_MIN_VALUE_12_POS 0
//! Field MIN_VALUE_12 - min_value_12
#define TLN20_TA_ALLOC_LIMIT_3_MIN_VALUE_12_MASK 0xFFu

//! Field MAX_VALUE_12 - max_value_12
#define TLN20_TA_ALLOC_LIMIT_3_MAX_VALUE_12_POS 8
//! Field MAX_VALUE_12 - max_value_12
#define TLN20_TA_ALLOC_LIMIT_3_MAX_VALUE_12_MASK 0xFF00u

//! Field MIN_VALUE_13 - min_value_13
#define TLN20_TA_ALLOC_LIMIT_3_MIN_VALUE_13_POS 16
//! Field MIN_VALUE_13 - min_value_13
#define TLN20_TA_ALLOC_LIMIT_3_MIN_VALUE_13_MASK 0xFF0000u

//! Field MAX_VALUE_13 - max_value_13
#define TLN20_TA_ALLOC_LIMIT_3_MAX_VALUE_13_POS 24
//! Field MAX_VALUE_13 - max_value_13
#define TLN20_TA_ALLOC_LIMIT_3_MAX_VALUE_13_MASK 0xFF000000u

//! Field MIN_VALUE_14 - min_value_14
#define TLN20_TA_ALLOC_LIMIT_3_MIN_VALUE_14_POS 32
//! Field MIN_VALUE_14 - min_value_14
#define TLN20_TA_ALLOC_LIMIT_3_MIN_VALUE_14_MASK 0xFF00000000u

//! Field MAX_VALUE_14 - max_value_14
#define TLN20_TA_ALLOC_LIMIT_3_MAX_VALUE_14_POS 40
//! Field MAX_VALUE_14 - max_value_14
#define TLN20_TA_ALLOC_LIMIT_3_MAX_VALUE_14_MASK 0xFF0000000000u

//! Field MIN_VALUE_15 - min_value_15
#define TLN20_TA_ALLOC_LIMIT_3_MIN_VALUE_15_POS 48
//! Field MIN_VALUE_15 - min_value_15
#define TLN20_TA_ALLOC_LIMIT_3_MIN_VALUE_15_MASK 0xFF000000000000u

//! Field MAX_VALUE_15 - max_value_15
#define TLN20_TA_ALLOC_LIMIT_3_MAX_VALUE_15_POS 56
//! Field MAX_VALUE_15 - max_value_15
#define TLN20_TA_ALLOC_LIMIT_3_MAX_VALUE_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS0W_IA_COMPONENT Register IUS0W_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define IUS0W_IA_COMPONENT 0x10000
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0W_IA_COMPONENT 0x1BF10000u

//! Register Reset Value
#define IUS0W_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define IUS0W_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define IUS0W_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define IUS0W_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define IUS0W_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup IUS0W_IA_CORE Register IUS0W_IA_CORE - core
//! @{

//! Register Offset (relative)
#define IUS0W_IA_CORE 0x10018
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0W_IA_CORE 0x1BF10018u

//! Register Reset Value
#define IUS0W_IA_CORE_RST 0x0000CAFE01250000u

//! Field REV_CODE - rev_code
#define IUS0W_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define IUS0W_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define IUS0W_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define IUS0W_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define IUS0W_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define IUS0W_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup IUS0W_IA_AGENT_CONTROL Register IUS0W_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define IUS0W_IA_AGENT_CONTROL 0x10020
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0W_IA_AGENT_CONTROL 0x1BF10020u

//! Register Reset Value
#define IUS0W_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IUS0W_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IUS0W_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define IUS0W_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define IUS0W_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define IUS0W_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IUS0W_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define IUS0W_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IUS0W_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define IUS0W_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define IUS0W_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IUS0W_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IUS0W_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IUS0W_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IUS0W_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IUS0W_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IUS0W_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IUS0W_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IUS0W_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IUS0W_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IUS0W_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IUS0W_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IUS0W_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup IUS0W_IA_AGENT_STATUS Register IUS0W_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define IUS0W_IA_AGENT_STATUS 0x10028
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0W_IA_AGENT_STATUS 0x1BF10028u

//! Register Reset Value
#define IUS0W_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IUS0W_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IUS0W_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define IUS0W_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define IUS0W_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define IUS0W_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define IUS0W_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define IUS0W_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define IUS0W_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define IUS0W_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define IUS0W_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define IUS0W_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IUS0W_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define IUS0W_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define IUS0W_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define IUS0W_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IUS0W_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define IUS0W_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define IUS0W_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IUS0W_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IUS0W_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IUS0W_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IUS0W_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup IUS0W_IA_ERROR_LOG Register IUS0W_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define IUS0W_IA_ERROR_LOG 0x10058
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0W_IA_ERROR_LOG 0x1BF10058u

//! Register Reset Value
#define IUS0W_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define IUS0W_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define IUS0W_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define IUS0W_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define IUS0W_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define IUS0W_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define IUS0W_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define IUS0W_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define IUS0W_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define IUS0W_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define IUS0W_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define IUS0W_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define IUS0W_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define IUS0W_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define IUS0W_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define IUS0W_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define IUS0W_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define IUS0W_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define IUS0W_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup IUS0W_IA_ERROR_LOG_ADDR Register IUS0W_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define IUS0W_IA_ERROR_LOG_ADDR 0x10060
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0W_IA_ERROR_LOG_ADDR 0x1BF10060u

//! Register Reset Value
#define IUS0W_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define IUS0W_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define IUS0W_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup IUS0W_IA_CORE_FLAG Register IUS0W_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define IUS0W_IA_CORE_FLAG 0x10068
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0W_IA_CORE_FLAG 0x1BF10068u

//! Register Reset Value
#define IUS0W_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define IUS0W_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define IUS0W_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define IUS0W_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define IUS0W_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define IUS0W_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define IUS0W_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define IUS0W_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define IUS0W_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup IUS0W_IA_ADDR_FILL_IN Register IUS0W_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define IUS0W_IA_ADDR_FILL_IN 0x10070
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0W_IA_ADDR_FILL_IN 0x1BF10070u

//! Register Reset Value
#define IUS0W_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define IUS0W_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define IUS0W_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup IUS0W_IA_BANDWIDTH_0 Register IUS0W_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define IUS0W_IA_BANDWIDTH_0 0x10100
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0W_IA_BANDWIDTH_0 0x1BF10100u

//! Register Reset Value
#define IUS0W_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define IUS0W_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define IUS0W_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define IUS0W_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define IUS0W_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define IUS0W_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define IUS0W_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define IUS0W_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define IUS0W_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define IUS0W_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define IUS0W_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define IUS0W_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define IUS0W_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define IUS0W_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define IUS0W_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define IUS0W_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define IUS0W_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS0W_IA_BANDWIDTH_1 Register IUS0W_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define IUS0W_IA_BANDWIDTH_1 0x10108
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0W_IA_BANDWIDTH_1 0x1BF10108u

//! Register Reset Value
#define IUS0W_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define IUS0W_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define IUS0W_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define IUS0W_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define IUS0W_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define IUS0W_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define IUS0W_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define IUS0W_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define IUS0W_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define IUS0W_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define IUS0W_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define IUS0W_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define IUS0W_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define IUS0W_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define IUS0W_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define IUS0W_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define IUS0W_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS0W_IA_BANDWIDTH_2 Register IUS0W_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define IUS0W_IA_BANDWIDTH_2 0x10110
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0W_IA_BANDWIDTH_2 0x1BF10110u

//! Register Reset Value
#define IUS0W_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define IUS0W_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define IUS0W_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define IUS0W_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define IUS0W_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define IUS0W_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define IUS0W_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define IUS0W_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define IUS0W_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define IUS0W_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define IUS0W_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define IUS0W_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define IUS0W_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define IUS0W_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define IUS0W_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define IUS0W_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define IUS0W_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS0W_IA_BANDWIDTH_3 Register IUS0W_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define IUS0W_IA_BANDWIDTH_3 0x10118
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0W_IA_BANDWIDTH_3 0x1BF10118u

//! Register Reset Value
#define IUS0W_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define IUS0W_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define IUS0W_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define IUS0W_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define IUS0W_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define IUS0W_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define IUS0W_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define IUS0W_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define IUS0W_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define IUS0W_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define IUS0W_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define IUS0W_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define IUS0W_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define IUS0W_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define IUS0W_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define IUS0W_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define IUS0W_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS0W_IA_BANDWIDTH_4 Register IUS0W_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define IUS0W_IA_BANDWIDTH_4 0x10120
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0W_IA_BANDWIDTH_4 0x1BF10120u

//! Register Reset Value
#define IUS0W_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define IUS0W_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define IUS0W_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define IUS0W_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define IUS0W_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define IUS0W_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define IUS0W_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define IUS0W_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define IUS0W_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define IUS0W_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define IUS0W_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define IUS0W_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define IUS0W_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define IUS0W_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define IUS0W_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define IUS0W_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define IUS0W_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS0W_IA_BANDWIDTH_5 Register IUS0W_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define IUS0W_IA_BANDWIDTH_5 0x10128
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0W_IA_BANDWIDTH_5 0x1BF10128u

//! Register Reset Value
#define IUS0W_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define IUS0W_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define IUS0W_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define IUS0W_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define IUS0W_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define IUS0W_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define IUS0W_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define IUS0W_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define IUS0W_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define IUS0W_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define IUS0W_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define IUS0W_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define IUS0W_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define IUS0W_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define IUS0W_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define IUS0W_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define IUS0W_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS0W_IA_BANDWIDTH_6 Register IUS0W_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define IUS0W_IA_BANDWIDTH_6 0x10130
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0W_IA_BANDWIDTH_6 0x1BF10130u

//! Register Reset Value
#define IUS0W_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define IUS0W_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define IUS0W_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define IUS0W_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define IUS0W_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define IUS0W_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define IUS0W_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define IUS0W_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define IUS0W_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define IUS0W_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define IUS0W_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define IUS0W_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define IUS0W_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define IUS0W_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define IUS0W_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define IUS0W_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define IUS0W_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS0W_IA_BANDWIDTH_7 Register IUS0W_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define IUS0W_IA_BANDWIDTH_7 0x10138
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0W_IA_BANDWIDTH_7 0x1BF10138u

//! Register Reset Value
#define IUS0W_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define IUS0W_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define IUS0W_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define IUS0W_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define IUS0W_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define IUS0W_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define IUS0W_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define IUS0W_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define IUS0W_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define IUS0W_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define IUS0W_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define IUS0W_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define IUS0W_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define IUS0W_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define IUS0W_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define IUS0W_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define IUS0W_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS0R_IA_COMPONENT Register IUS0R_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define IUS0R_IA_COMPONENT 0x10400
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0R_IA_COMPONENT 0x1BF10400u

//! Register Reset Value
#define IUS0R_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define IUS0R_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define IUS0R_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define IUS0R_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define IUS0R_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup IUS0R_IA_CORE Register IUS0R_IA_CORE - core
//! @{

//! Register Offset (relative)
#define IUS0R_IA_CORE 0x10418
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0R_IA_CORE 0x1BF10418u

//! Register Reset Value
#define IUS0R_IA_CORE_RST 0x0000CAFE00250000u

//! Field REV_CODE - rev_code
#define IUS0R_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define IUS0R_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define IUS0R_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define IUS0R_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define IUS0R_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define IUS0R_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup IUS0R_IA_AGENT_CONTROL Register IUS0R_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define IUS0R_IA_AGENT_CONTROL 0x10420
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0R_IA_AGENT_CONTROL 0x1BF10420u

//! Register Reset Value
#define IUS0R_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IUS0R_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IUS0R_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define IUS0R_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define IUS0R_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define IUS0R_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IUS0R_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define IUS0R_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IUS0R_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define IUS0R_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define IUS0R_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IUS0R_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IUS0R_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IUS0R_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IUS0R_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IUS0R_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IUS0R_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IUS0R_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IUS0R_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IUS0R_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IUS0R_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IUS0R_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IUS0R_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup IUS0R_IA_AGENT_STATUS Register IUS0R_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define IUS0R_IA_AGENT_STATUS 0x10428
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0R_IA_AGENT_STATUS 0x1BF10428u

//! Register Reset Value
#define IUS0R_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IUS0R_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IUS0R_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define IUS0R_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define IUS0R_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define IUS0R_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define IUS0R_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define IUS0R_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define IUS0R_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define IUS0R_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define IUS0R_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define IUS0R_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IUS0R_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define IUS0R_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define IUS0R_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define IUS0R_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IUS0R_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define IUS0R_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define IUS0R_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IUS0R_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IUS0R_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IUS0R_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IUS0R_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup IUS0R_IA_ERROR_LOG Register IUS0R_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define IUS0R_IA_ERROR_LOG 0x10458
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0R_IA_ERROR_LOG 0x1BF10458u

//! Register Reset Value
#define IUS0R_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define IUS0R_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define IUS0R_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define IUS0R_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define IUS0R_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define IUS0R_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define IUS0R_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define IUS0R_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define IUS0R_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define IUS0R_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define IUS0R_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define IUS0R_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define IUS0R_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define IUS0R_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define IUS0R_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define IUS0R_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define IUS0R_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define IUS0R_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define IUS0R_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup IUS0R_IA_ERROR_LOG_ADDR Register IUS0R_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define IUS0R_IA_ERROR_LOG_ADDR 0x10460
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0R_IA_ERROR_LOG_ADDR 0x1BF10460u

//! Register Reset Value
#define IUS0R_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define IUS0R_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define IUS0R_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup IUS0R_IA_CORE_FLAG Register IUS0R_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define IUS0R_IA_CORE_FLAG 0x10468
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0R_IA_CORE_FLAG 0x1BF10468u

//! Register Reset Value
#define IUS0R_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define IUS0R_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define IUS0R_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define IUS0R_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define IUS0R_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define IUS0R_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define IUS0R_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define IUS0R_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define IUS0R_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup IUS0R_IA_ADDR_FILL_IN Register IUS0R_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define IUS0R_IA_ADDR_FILL_IN 0x10470
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0R_IA_ADDR_FILL_IN 0x1BF10470u

//! Register Reset Value
#define IUS0R_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define IUS0R_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define IUS0R_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup IUS0R_IA_BANDWIDTH_0 Register IUS0R_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define IUS0R_IA_BANDWIDTH_0 0x10500
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0R_IA_BANDWIDTH_0 0x1BF10500u

//! Register Reset Value
#define IUS0R_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define IUS0R_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define IUS0R_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define IUS0R_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define IUS0R_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define IUS0R_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define IUS0R_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define IUS0R_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define IUS0R_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define IUS0R_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define IUS0R_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define IUS0R_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define IUS0R_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define IUS0R_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define IUS0R_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define IUS0R_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define IUS0R_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS0R_IA_BANDWIDTH_1 Register IUS0R_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define IUS0R_IA_BANDWIDTH_1 0x10508
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0R_IA_BANDWIDTH_1 0x1BF10508u

//! Register Reset Value
#define IUS0R_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define IUS0R_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define IUS0R_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define IUS0R_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define IUS0R_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define IUS0R_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define IUS0R_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define IUS0R_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define IUS0R_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define IUS0R_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define IUS0R_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define IUS0R_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define IUS0R_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define IUS0R_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define IUS0R_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define IUS0R_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define IUS0R_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS0R_IA_BANDWIDTH_2 Register IUS0R_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define IUS0R_IA_BANDWIDTH_2 0x10510
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0R_IA_BANDWIDTH_2 0x1BF10510u

//! Register Reset Value
#define IUS0R_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define IUS0R_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define IUS0R_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define IUS0R_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define IUS0R_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define IUS0R_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define IUS0R_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define IUS0R_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define IUS0R_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define IUS0R_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define IUS0R_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define IUS0R_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define IUS0R_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define IUS0R_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define IUS0R_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define IUS0R_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define IUS0R_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS0R_IA_BANDWIDTH_3 Register IUS0R_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define IUS0R_IA_BANDWIDTH_3 0x10518
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0R_IA_BANDWIDTH_3 0x1BF10518u

//! Register Reset Value
#define IUS0R_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define IUS0R_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define IUS0R_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define IUS0R_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define IUS0R_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define IUS0R_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define IUS0R_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define IUS0R_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define IUS0R_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define IUS0R_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define IUS0R_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define IUS0R_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define IUS0R_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define IUS0R_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define IUS0R_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define IUS0R_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define IUS0R_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS0R_IA_BANDWIDTH_4 Register IUS0R_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define IUS0R_IA_BANDWIDTH_4 0x10520
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0R_IA_BANDWIDTH_4 0x1BF10520u

//! Register Reset Value
#define IUS0R_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define IUS0R_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define IUS0R_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define IUS0R_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define IUS0R_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define IUS0R_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define IUS0R_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define IUS0R_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define IUS0R_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define IUS0R_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define IUS0R_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define IUS0R_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define IUS0R_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define IUS0R_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define IUS0R_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define IUS0R_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define IUS0R_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS0R_IA_BANDWIDTH_5 Register IUS0R_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define IUS0R_IA_BANDWIDTH_5 0x10528
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0R_IA_BANDWIDTH_5 0x1BF10528u

//! Register Reset Value
#define IUS0R_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define IUS0R_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define IUS0R_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define IUS0R_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define IUS0R_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define IUS0R_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define IUS0R_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define IUS0R_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define IUS0R_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define IUS0R_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define IUS0R_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define IUS0R_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define IUS0R_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define IUS0R_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define IUS0R_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define IUS0R_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define IUS0R_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS0R_IA_BANDWIDTH_6 Register IUS0R_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define IUS0R_IA_BANDWIDTH_6 0x10530
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0R_IA_BANDWIDTH_6 0x1BF10530u

//! Register Reset Value
#define IUS0R_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define IUS0R_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define IUS0R_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define IUS0R_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define IUS0R_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define IUS0R_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define IUS0R_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define IUS0R_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define IUS0R_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define IUS0R_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define IUS0R_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define IUS0R_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define IUS0R_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define IUS0R_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define IUS0R_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define IUS0R_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define IUS0R_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS0R_IA_BANDWIDTH_7 Register IUS0R_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define IUS0R_IA_BANDWIDTH_7 0x10538
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS0R_IA_BANDWIDTH_7 0x1BF10538u

//! Register Reset Value
#define IUS0R_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define IUS0R_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define IUS0R_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define IUS0R_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define IUS0R_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define IUS0R_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define IUS0R_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define IUS0R_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define IUS0R_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define IUS0R_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define IUS0R_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define IUS0R_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define IUS0R_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define IUS0R_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define IUS0R_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define IUS0R_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define IUS0R_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS1W_IA_COMPONENT Register IUS1W_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define IUS1W_IA_COMPONENT 0x10800
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1W_IA_COMPONENT 0x1BF10800u

//! Register Reset Value
#define IUS1W_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define IUS1W_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define IUS1W_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define IUS1W_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define IUS1W_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup IUS1W_IA_CORE Register IUS1W_IA_CORE - core
//! @{

//! Register Offset (relative)
#define IUS1W_IA_CORE 0x10818
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1W_IA_CORE 0x1BF10818u

//! Register Reset Value
#define IUS1W_IA_CORE_RST 0x0000CAFE01270000u

//! Field REV_CODE - rev_code
#define IUS1W_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define IUS1W_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define IUS1W_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define IUS1W_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define IUS1W_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define IUS1W_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup IUS1W_IA_AGENT_CONTROL Register IUS1W_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define IUS1W_IA_AGENT_CONTROL 0x10820
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1W_IA_AGENT_CONTROL 0x1BF10820u

//! Register Reset Value
#define IUS1W_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IUS1W_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IUS1W_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define IUS1W_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define IUS1W_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define IUS1W_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IUS1W_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define IUS1W_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IUS1W_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define IUS1W_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define IUS1W_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IUS1W_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IUS1W_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IUS1W_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IUS1W_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IUS1W_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IUS1W_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IUS1W_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IUS1W_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IUS1W_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IUS1W_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IUS1W_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IUS1W_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup IUS1W_IA_AGENT_STATUS Register IUS1W_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define IUS1W_IA_AGENT_STATUS 0x10828
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1W_IA_AGENT_STATUS 0x1BF10828u

//! Register Reset Value
#define IUS1W_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IUS1W_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IUS1W_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define IUS1W_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define IUS1W_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define IUS1W_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define IUS1W_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define IUS1W_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define IUS1W_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define IUS1W_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define IUS1W_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define IUS1W_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IUS1W_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define IUS1W_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define IUS1W_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define IUS1W_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IUS1W_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define IUS1W_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define IUS1W_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IUS1W_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IUS1W_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IUS1W_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IUS1W_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup IUS1W_IA_ERROR_LOG Register IUS1W_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define IUS1W_IA_ERROR_LOG 0x10858
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1W_IA_ERROR_LOG 0x1BF10858u

//! Register Reset Value
#define IUS1W_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define IUS1W_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define IUS1W_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define IUS1W_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define IUS1W_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define IUS1W_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define IUS1W_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define IUS1W_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define IUS1W_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define IUS1W_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define IUS1W_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define IUS1W_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define IUS1W_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define IUS1W_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define IUS1W_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define IUS1W_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define IUS1W_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define IUS1W_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define IUS1W_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup IUS1W_IA_ERROR_LOG_ADDR Register IUS1W_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define IUS1W_IA_ERROR_LOG_ADDR 0x10860
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1W_IA_ERROR_LOG_ADDR 0x1BF10860u

//! Register Reset Value
#define IUS1W_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define IUS1W_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define IUS1W_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup IUS1W_IA_CORE_FLAG Register IUS1W_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define IUS1W_IA_CORE_FLAG 0x10868
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1W_IA_CORE_FLAG 0x1BF10868u

//! Register Reset Value
#define IUS1W_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define IUS1W_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define IUS1W_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define IUS1W_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define IUS1W_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define IUS1W_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define IUS1W_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define IUS1W_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define IUS1W_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup IUS1W_IA_ADDR_FILL_IN Register IUS1W_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define IUS1W_IA_ADDR_FILL_IN 0x10870
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1W_IA_ADDR_FILL_IN 0x1BF10870u

//! Register Reset Value
#define IUS1W_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define IUS1W_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define IUS1W_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup IUS1W_IA_BANDWIDTH_0 Register IUS1W_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define IUS1W_IA_BANDWIDTH_0 0x10900
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1W_IA_BANDWIDTH_0 0x1BF10900u

//! Register Reset Value
#define IUS1W_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define IUS1W_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define IUS1W_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define IUS1W_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define IUS1W_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define IUS1W_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define IUS1W_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define IUS1W_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define IUS1W_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define IUS1W_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define IUS1W_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define IUS1W_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define IUS1W_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define IUS1W_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define IUS1W_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define IUS1W_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define IUS1W_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS1W_IA_BANDWIDTH_1 Register IUS1W_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define IUS1W_IA_BANDWIDTH_1 0x10908
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1W_IA_BANDWIDTH_1 0x1BF10908u

//! Register Reset Value
#define IUS1W_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define IUS1W_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define IUS1W_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define IUS1W_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define IUS1W_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define IUS1W_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define IUS1W_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define IUS1W_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define IUS1W_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define IUS1W_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define IUS1W_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define IUS1W_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define IUS1W_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define IUS1W_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define IUS1W_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define IUS1W_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define IUS1W_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS1W_IA_BANDWIDTH_2 Register IUS1W_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define IUS1W_IA_BANDWIDTH_2 0x10910
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1W_IA_BANDWIDTH_2 0x1BF10910u

//! Register Reset Value
#define IUS1W_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define IUS1W_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define IUS1W_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define IUS1W_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define IUS1W_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define IUS1W_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define IUS1W_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define IUS1W_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define IUS1W_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define IUS1W_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define IUS1W_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define IUS1W_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define IUS1W_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define IUS1W_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define IUS1W_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define IUS1W_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define IUS1W_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS1W_IA_BANDWIDTH_3 Register IUS1W_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define IUS1W_IA_BANDWIDTH_3 0x10918
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1W_IA_BANDWIDTH_3 0x1BF10918u

//! Register Reset Value
#define IUS1W_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define IUS1W_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define IUS1W_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define IUS1W_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define IUS1W_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define IUS1W_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define IUS1W_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define IUS1W_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define IUS1W_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define IUS1W_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define IUS1W_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define IUS1W_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define IUS1W_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define IUS1W_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define IUS1W_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define IUS1W_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define IUS1W_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS1W_IA_BANDWIDTH_4 Register IUS1W_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define IUS1W_IA_BANDWIDTH_4 0x10920
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1W_IA_BANDWIDTH_4 0x1BF10920u

//! Register Reset Value
#define IUS1W_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define IUS1W_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define IUS1W_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define IUS1W_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define IUS1W_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define IUS1W_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define IUS1W_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define IUS1W_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define IUS1W_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define IUS1W_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define IUS1W_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define IUS1W_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define IUS1W_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define IUS1W_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define IUS1W_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define IUS1W_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define IUS1W_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS1W_IA_BANDWIDTH_5 Register IUS1W_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define IUS1W_IA_BANDWIDTH_5 0x10928
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1W_IA_BANDWIDTH_5 0x1BF10928u

//! Register Reset Value
#define IUS1W_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define IUS1W_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define IUS1W_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define IUS1W_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define IUS1W_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define IUS1W_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define IUS1W_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define IUS1W_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define IUS1W_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define IUS1W_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define IUS1W_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define IUS1W_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define IUS1W_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define IUS1W_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define IUS1W_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define IUS1W_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define IUS1W_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS1W_IA_BANDWIDTH_6 Register IUS1W_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define IUS1W_IA_BANDWIDTH_6 0x10930
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1W_IA_BANDWIDTH_6 0x1BF10930u

//! Register Reset Value
#define IUS1W_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define IUS1W_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define IUS1W_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define IUS1W_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define IUS1W_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define IUS1W_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define IUS1W_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define IUS1W_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define IUS1W_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define IUS1W_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define IUS1W_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define IUS1W_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define IUS1W_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define IUS1W_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define IUS1W_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define IUS1W_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define IUS1W_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS1W_IA_BANDWIDTH_7 Register IUS1W_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define IUS1W_IA_BANDWIDTH_7 0x10938
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1W_IA_BANDWIDTH_7 0x1BF10938u

//! Register Reset Value
#define IUS1W_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define IUS1W_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define IUS1W_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define IUS1W_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define IUS1W_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define IUS1W_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define IUS1W_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define IUS1W_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define IUS1W_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define IUS1W_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define IUS1W_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define IUS1W_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define IUS1W_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define IUS1W_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define IUS1W_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define IUS1W_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define IUS1W_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS1R_IA_COMPONENT Register IUS1R_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define IUS1R_IA_COMPONENT 0x10C00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1R_IA_COMPONENT 0x1BF10C00u

//! Register Reset Value
#define IUS1R_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define IUS1R_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define IUS1R_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define IUS1R_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define IUS1R_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup IUS1R_IA_CORE Register IUS1R_IA_CORE - core
//! @{

//! Register Offset (relative)
#define IUS1R_IA_CORE 0x10C18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1R_IA_CORE 0x1BF10C18u

//! Register Reset Value
#define IUS1R_IA_CORE_RST 0x0000CAFE00270000u

//! Field REV_CODE - rev_code
#define IUS1R_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define IUS1R_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define IUS1R_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define IUS1R_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define IUS1R_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define IUS1R_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup IUS1R_IA_AGENT_CONTROL Register IUS1R_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define IUS1R_IA_AGENT_CONTROL 0x10C20
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1R_IA_AGENT_CONTROL 0x1BF10C20u

//! Register Reset Value
#define IUS1R_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IUS1R_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IUS1R_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define IUS1R_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define IUS1R_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define IUS1R_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IUS1R_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define IUS1R_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IUS1R_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define IUS1R_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define IUS1R_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IUS1R_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IUS1R_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IUS1R_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IUS1R_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IUS1R_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IUS1R_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IUS1R_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IUS1R_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IUS1R_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IUS1R_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IUS1R_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IUS1R_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup IUS1R_IA_AGENT_STATUS Register IUS1R_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define IUS1R_IA_AGENT_STATUS 0x10C28
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1R_IA_AGENT_STATUS 0x1BF10C28u

//! Register Reset Value
#define IUS1R_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IUS1R_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IUS1R_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define IUS1R_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define IUS1R_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define IUS1R_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define IUS1R_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define IUS1R_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define IUS1R_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define IUS1R_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define IUS1R_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define IUS1R_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IUS1R_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define IUS1R_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define IUS1R_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define IUS1R_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IUS1R_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define IUS1R_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define IUS1R_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IUS1R_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IUS1R_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IUS1R_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IUS1R_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup IUS1R_IA_ERROR_LOG Register IUS1R_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define IUS1R_IA_ERROR_LOG 0x10C58
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1R_IA_ERROR_LOG 0x1BF10C58u

//! Register Reset Value
#define IUS1R_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define IUS1R_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define IUS1R_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define IUS1R_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define IUS1R_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define IUS1R_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define IUS1R_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define IUS1R_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define IUS1R_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define IUS1R_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define IUS1R_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define IUS1R_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define IUS1R_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define IUS1R_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define IUS1R_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define IUS1R_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define IUS1R_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define IUS1R_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define IUS1R_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup IUS1R_IA_ERROR_LOG_ADDR Register IUS1R_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define IUS1R_IA_ERROR_LOG_ADDR 0x10C60
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1R_IA_ERROR_LOG_ADDR 0x1BF10C60u

//! Register Reset Value
#define IUS1R_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define IUS1R_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define IUS1R_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup IUS1R_IA_CORE_FLAG Register IUS1R_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define IUS1R_IA_CORE_FLAG 0x10C68
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1R_IA_CORE_FLAG 0x1BF10C68u

//! Register Reset Value
#define IUS1R_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define IUS1R_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define IUS1R_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define IUS1R_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define IUS1R_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define IUS1R_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define IUS1R_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define IUS1R_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define IUS1R_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup IUS1R_IA_ADDR_FILL_IN Register IUS1R_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define IUS1R_IA_ADDR_FILL_IN 0x10C70
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1R_IA_ADDR_FILL_IN 0x1BF10C70u

//! Register Reset Value
#define IUS1R_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define IUS1R_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define IUS1R_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup IUS1R_IA_BANDWIDTH_0 Register IUS1R_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define IUS1R_IA_BANDWIDTH_0 0x10D00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1R_IA_BANDWIDTH_0 0x1BF10D00u

//! Register Reset Value
#define IUS1R_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define IUS1R_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define IUS1R_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define IUS1R_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define IUS1R_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define IUS1R_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define IUS1R_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define IUS1R_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define IUS1R_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define IUS1R_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define IUS1R_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define IUS1R_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define IUS1R_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define IUS1R_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define IUS1R_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define IUS1R_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define IUS1R_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS1R_IA_BANDWIDTH_1 Register IUS1R_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define IUS1R_IA_BANDWIDTH_1 0x10D08
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1R_IA_BANDWIDTH_1 0x1BF10D08u

//! Register Reset Value
#define IUS1R_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define IUS1R_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define IUS1R_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define IUS1R_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define IUS1R_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define IUS1R_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define IUS1R_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define IUS1R_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define IUS1R_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define IUS1R_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define IUS1R_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define IUS1R_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define IUS1R_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define IUS1R_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define IUS1R_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define IUS1R_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define IUS1R_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS1R_IA_BANDWIDTH_2 Register IUS1R_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define IUS1R_IA_BANDWIDTH_2 0x10D10
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1R_IA_BANDWIDTH_2 0x1BF10D10u

//! Register Reset Value
#define IUS1R_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define IUS1R_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define IUS1R_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define IUS1R_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define IUS1R_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define IUS1R_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define IUS1R_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define IUS1R_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define IUS1R_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define IUS1R_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define IUS1R_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define IUS1R_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define IUS1R_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define IUS1R_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define IUS1R_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define IUS1R_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define IUS1R_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS1R_IA_BANDWIDTH_3 Register IUS1R_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define IUS1R_IA_BANDWIDTH_3 0x10D18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1R_IA_BANDWIDTH_3 0x1BF10D18u

//! Register Reset Value
#define IUS1R_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define IUS1R_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define IUS1R_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define IUS1R_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define IUS1R_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define IUS1R_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define IUS1R_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define IUS1R_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define IUS1R_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define IUS1R_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define IUS1R_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define IUS1R_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define IUS1R_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define IUS1R_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define IUS1R_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define IUS1R_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define IUS1R_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS1R_IA_BANDWIDTH_4 Register IUS1R_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define IUS1R_IA_BANDWIDTH_4 0x10D20
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1R_IA_BANDWIDTH_4 0x1BF10D20u

//! Register Reset Value
#define IUS1R_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define IUS1R_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define IUS1R_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define IUS1R_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define IUS1R_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define IUS1R_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define IUS1R_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define IUS1R_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define IUS1R_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define IUS1R_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define IUS1R_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define IUS1R_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define IUS1R_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define IUS1R_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define IUS1R_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define IUS1R_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define IUS1R_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS1R_IA_BANDWIDTH_5 Register IUS1R_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define IUS1R_IA_BANDWIDTH_5 0x10D28
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1R_IA_BANDWIDTH_5 0x1BF10D28u

//! Register Reset Value
#define IUS1R_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define IUS1R_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define IUS1R_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define IUS1R_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define IUS1R_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define IUS1R_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define IUS1R_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define IUS1R_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define IUS1R_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define IUS1R_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define IUS1R_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define IUS1R_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define IUS1R_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define IUS1R_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define IUS1R_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define IUS1R_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define IUS1R_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS1R_IA_BANDWIDTH_6 Register IUS1R_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define IUS1R_IA_BANDWIDTH_6 0x10D30
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1R_IA_BANDWIDTH_6 0x1BF10D30u

//! Register Reset Value
#define IUS1R_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define IUS1R_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define IUS1R_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define IUS1R_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define IUS1R_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define IUS1R_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define IUS1R_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define IUS1R_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define IUS1R_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define IUS1R_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define IUS1R_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define IUS1R_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define IUS1R_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define IUS1R_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define IUS1R_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define IUS1R_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define IUS1R_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup IUS1R_IA_BANDWIDTH_7 Register IUS1R_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define IUS1R_IA_BANDWIDTH_7 0x10D38
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IUS1R_IA_BANDWIDTH_7 0x1BF10D38u

//! Register Reset Value
#define IUS1R_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define IUS1R_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define IUS1R_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define IUS1R_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define IUS1R_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define IUS1R_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define IUS1R_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define IUS1R_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define IUS1R_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define IUS1R_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define IUS1R_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define IUS1R_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define IUS1R_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define IUS1R_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define IUS1R_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define IUS1R_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define IUS1R_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM1R_IA_COMPONENT Register IDM1R_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define IDM1R_IA_COMPONENT 0x11000
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1R_IA_COMPONENT 0x1BF11000u

//! Register Reset Value
#define IDM1R_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define IDM1R_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define IDM1R_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define IDM1R_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define IDM1R_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup IDM1R_IA_CORE Register IDM1R_IA_CORE - core
//! @{

//! Register Offset (relative)
#define IDM1R_IA_CORE 0x11018
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1R_IA_CORE 0x1BF11018u

//! Register Reset Value
#define IDM1R_IA_CORE_RST 0x000050C500240001u

//! Field REV_CODE - rev_code
#define IDM1R_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define IDM1R_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define IDM1R_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define IDM1R_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define IDM1R_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define IDM1R_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup IDM1R_IA_AGENT_CONTROL Register IDM1R_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define IDM1R_IA_AGENT_CONTROL 0x11020
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1R_IA_AGENT_CONTROL 0x1BF11020u

//! Register Reset Value
#define IDM1R_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IDM1R_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IDM1R_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define IDM1R_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define IDM1R_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define IDM1R_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IDM1R_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define IDM1R_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IDM1R_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define IDM1R_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define IDM1R_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IDM1R_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IDM1R_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IDM1R_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IDM1R_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IDM1R_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IDM1R_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IDM1R_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IDM1R_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IDM1R_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IDM1R_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IDM1R_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IDM1R_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup IDM1R_IA_AGENT_STATUS Register IDM1R_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define IDM1R_IA_AGENT_STATUS 0x11028
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1R_IA_AGENT_STATUS 0x1BF11028u

//! Register Reset Value
#define IDM1R_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IDM1R_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IDM1R_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define IDM1R_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define IDM1R_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define IDM1R_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define IDM1R_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define IDM1R_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define IDM1R_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define IDM1R_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define IDM1R_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define IDM1R_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IDM1R_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define IDM1R_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define IDM1R_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define IDM1R_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IDM1R_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define IDM1R_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define IDM1R_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IDM1R_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IDM1R_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IDM1R_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IDM1R_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup IDM1R_IA_ERROR_LOG Register IDM1R_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define IDM1R_IA_ERROR_LOG 0x11058
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1R_IA_ERROR_LOG 0x1BF11058u

//! Register Reset Value
#define IDM1R_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define IDM1R_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define IDM1R_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define IDM1R_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define IDM1R_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define IDM1R_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define IDM1R_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define IDM1R_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define IDM1R_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define IDM1R_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define IDM1R_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define IDM1R_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define IDM1R_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define IDM1R_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define IDM1R_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define IDM1R_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define IDM1R_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define IDM1R_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define IDM1R_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup IDM1R_IA_ERROR_LOG_ADDR Register IDM1R_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define IDM1R_IA_ERROR_LOG_ADDR 0x11060
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1R_IA_ERROR_LOG_ADDR 0x1BF11060u

//! Register Reset Value
#define IDM1R_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define IDM1R_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define IDM1R_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup IDM1R_IA_CORE_FLAG Register IDM1R_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define IDM1R_IA_CORE_FLAG 0x11068
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1R_IA_CORE_FLAG 0x1BF11068u

//! Register Reset Value
#define IDM1R_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define IDM1R_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define IDM1R_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define IDM1R_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define IDM1R_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define IDM1R_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define IDM1R_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define IDM1R_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define IDM1R_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup IDM1R_IA_ADDR_FILL_IN Register IDM1R_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define IDM1R_IA_ADDR_FILL_IN 0x11070
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1R_IA_ADDR_FILL_IN 0x1BF11070u

//! Register Reset Value
#define IDM1R_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define IDM1R_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define IDM1R_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup IDM1R_IA_BANDWIDTH_0 Register IDM1R_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define IDM1R_IA_BANDWIDTH_0 0x11100
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1R_IA_BANDWIDTH_0 0x1BF11100u

//! Register Reset Value
#define IDM1R_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define IDM1R_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define IDM1R_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define IDM1R_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define IDM1R_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define IDM1R_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define IDM1R_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define IDM1R_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define IDM1R_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define IDM1R_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define IDM1R_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define IDM1R_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define IDM1R_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define IDM1R_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define IDM1R_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define IDM1R_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define IDM1R_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM1R_IA_BANDWIDTH_1 Register IDM1R_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define IDM1R_IA_BANDWIDTH_1 0x11108
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1R_IA_BANDWIDTH_1 0x1BF11108u

//! Register Reset Value
#define IDM1R_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define IDM1R_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define IDM1R_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define IDM1R_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define IDM1R_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define IDM1R_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define IDM1R_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define IDM1R_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define IDM1R_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define IDM1R_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define IDM1R_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define IDM1R_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define IDM1R_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define IDM1R_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define IDM1R_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define IDM1R_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define IDM1R_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM1R_IA_BANDWIDTH_2 Register IDM1R_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define IDM1R_IA_BANDWIDTH_2 0x11110
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1R_IA_BANDWIDTH_2 0x1BF11110u

//! Register Reset Value
#define IDM1R_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define IDM1R_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define IDM1R_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define IDM1R_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define IDM1R_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define IDM1R_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define IDM1R_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define IDM1R_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define IDM1R_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define IDM1R_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define IDM1R_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define IDM1R_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define IDM1R_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define IDM1R_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define IDM1R_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define IDM1R_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define IDM1R_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM1R_IA_BANDWIDTH_3 Register IDM1R_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define IDM1R_IA_BANDWIDTH_3 0x11118
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1R_IA_BANDWIDTH_3 0x1BF11118u

//! Register Reset Value
#define IDM1R_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define IDM1R_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define IDM1R_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define IDM1R_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define IDM1R_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define IDM1R_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define IDM1R_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define IDM1R_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define IDM1R_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define IDM1R_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define IDM1R_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define IDM1R_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define IDM1R_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define IDM1R_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define IDM1R_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define IDM1R_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define IDM1R_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM1R_IA_BANDWIDTH_4 Register IDM1R_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define IDM1R_IA_BANDWIDTH_4 0x11120
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1R_IA_BANDWIDTH_4 0x1BF11120u

//! Register Reset Value
#define IDM1R_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define IDM1R_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define IDM1R_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define IDM1R_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define IDM1R_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define IDM1R_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define IDM1R_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define IDM1R_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define IDM1R_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define IDM1R_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define IDM1R_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define IDM1R_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define IDM1R_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define IDM1R_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define IDM1R_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define IDM1R_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define IDM1R_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM1R_IA_BANDWIDTH_5 Register IDM1R_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define IDM1R_IA_BANDWIDTH_5 0x11128
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1R_IA_BANDWIDTH_5 0x1BF11128u

//! Register Reset Value
#define IDM1R_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define IDM1R_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define IDM1R_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define IDM1R_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define IDM1R_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define IDM1R_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define IDM1R_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define IDM1R_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define IDM1R_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define IDM1R_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define IDM1R_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define IDM1R_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define IDM1R_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define IDM1R_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define IDM1R_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define IDM1R_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define IDM1R_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM1R_IA_BANDWIDTH_6 Register IDM1R_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define IDM1R_IA_BANDWIDTH_6 0x11130
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1R_IA_BANDWIDTH_6 0x1BF11130u

//! Register Reset Value
#define IDM1R_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define IDM1R_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define IDM1R_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define IDM1R_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define IDM1R_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define IDM1R_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define IDM1R_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define IDM1R_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define IDM1R_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define IDM1R_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define IDM1R_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define IDM1R_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define IDM1R_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define IDM1R_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define IDM1R_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define IDM1R_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define IDM1R_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM1R_IA_BANDWIDTH_7 Register IDM1R_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define IDM1R_IA_BANDWIDTH_7 0x11138
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1R_IA_BANDWIDTH_7 0x1BF11138u

//! Register Reset Value
#define IDM1R_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define IDM1R_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define IDM1R_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define IDM1R_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define IDM1R_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define IDM1R_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define IDM1R_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define IDM1R_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define IDM1R_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define IDM1R_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define IDM1R_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define IDM1R_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define IDM1R_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define IDM1R_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define IDM1R_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define IDM1R_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define IDM1R_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM1T_IA_COMPONENT Register IDM1T_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define IDM1T_IA_COMPONENT 0x11400
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1T_IA_COMPONENT 0x1BF11400u

//! Register Reset Value
#define IDM1T_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define IDM1T_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define IDM1T_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define IDM1T_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define IDM1T_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup IDM1T_IA_CORE Register IDM1T_IA_CORE - core
//! @{

//! Register Offset (relative)
#define IDM1T_IA_CORE 0x11418
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1T_IA_CORE 0x1BF11418u

//! Register Reset Value
#define IDM1T_IA_CORE_RST 0x000050C500230001u

//! Field REV_CODE - rev_code
#define IDM1T_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define IDM1T_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define IDM1T_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define IDM1T_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define IDM1T_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define IDM1T_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup IDM1T_IA_AGENT_CONTROL Register IDM1T_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define IDM1T_IA_AGENT_CONTROL 0x11420
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1T_IA_AGENT_CONTROL 0x1BF11420u

//! Register Reset Value
#define IDM1T_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IDM1T_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IDM1T_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define IDM1T_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define IDM1T_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define IDM1T_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IDM1T_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define IDM1T_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IDM1T_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define IDM1T_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define IDM1T_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IDM1T_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IDM1T_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IDM1T_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IDM1T_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IDM1T_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IDM1T_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IDM1T_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IDM1T_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IDM1T_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IDM1T_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IDM1T_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IDM1T_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup IDM1T_IA_AGENT_STATUS Register IDM1T_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define IDM1T_IA_AGENT_STATUS 0x11428
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1T_IA_AGENT_STATUS 0x1BF11428u

//! Register Reset Value
#define IDM1T_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IDM1T_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IDM1T_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define IDM1T_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define IDM1T_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define IDM1T_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define IDM1T_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define IDM1T_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define IDM1T_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define IDM1T_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define IDM1T_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define IDM1T_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IDM1T_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define IDM1T_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define IDM1T_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define IDM1T_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IDM1T_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define IDM1T_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define IDM1T_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IDM1T_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IDM1T_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IDM1T_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IDM1T_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup IDM1T_IA_ERROR_LOG Register IDM1T_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define IDM1T_IA_ERROR_LOG 0x11458
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1T_IA_ERROR_LOG 0x1BF11458u

//! Register Reset Value
#define IDM1T_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define IDM1T_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define IDM1T_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define IDM1T_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define IDM1T_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define IDM1T_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define IDM1T_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define IDM1T_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define IDM1T_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define IDM1T_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define IDM1T_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define IDM1T_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define IDM1T_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define IDM1T_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define IDM1T_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define IDM1T_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define IDM1T_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define IDM1T_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define IDM1T_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup IDM1T_IA_ERROR_LOG_ADDR Register IDM1T_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define IDM1T_IA_ERROR_LOG_ADDR 0x11460
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1T_IA_ERROR_LOG_ADDR 0x1BF11460u

//! Register Reset Value
#define IDM1T_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define IDM1T_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define IDM1T_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup IDM1T_IA_CORE_FLAG Register IDM1T_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define IDM1T_IA_CORE_FLAG 0x11468
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1T_IA_CORE_FLAG 0x1BF11468u

//! Register Reset Value
#define IDM1T_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define IDM1T_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define IDM1T_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define IDM1T_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define IDM1T_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define IDM1T_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define IDM1T_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define IDM1T_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define IDM1T_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup IDM1T_IA_ADDR_FILL_IN Register IDM1T_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define IDM1T_IA_ADDR_FILL_IN 0x11470
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1T_IA_ADDR_FILL_IN 0x1BF11470u

//! Register Reset Value
#define IDM1T_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define IDM1T_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define IDM1T_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup IDM1T_IA_BANDWIDTH_0 Register IDM1T_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define IDM1T_IA_BANDWIDTH_0 0x11500
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1T_IA_BANDWIDTH_0 0x1BF11500u

//! Register Reset Value
#define IDM1T_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define IDM1T_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define IDM1T_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define IDM1T_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define IDM1T_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define IDM1T_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define IDM1T_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define IDM1T_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define IDM1T_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define IDM1T_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define IDM1T_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define IDM1T_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define IDM1T_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define IDM1T_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define IDM1T_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define IDM1T_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define IDM1T_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM1T_IA_BANDWIDTH_1 Register IDM1T_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define IDM1T_IA_BANDWIDTH_1 0x11508
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1T_IA_BANDWIDTH_1 0x1BF11508u

//! Register Reset Value
#define IDM1T_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define IDM1T_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define IDM1T_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define IDM1T_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define IDM1T_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define IDM1T_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define IDM1T_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define IDM1T_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define IDM1T_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define IDM1T_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define IDM1T_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define IDM1T_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define IDM1T_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define IDM1T_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define IDM1T_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define IDM1T_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define IDM1T_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM1T_IA_BANDWIDTH_2 Register IDM1T_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define IDM1T_IA_BANDWIDTH_2 0x11510
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1T_IA_BANDWIDTH_2 0x1BF11510u

//! Register Reset Value
#define IDM1T_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define IDM1T_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define IDM1T_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define IDM1T_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define IDM1T_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define IDM1T_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define IDM1T_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define IDM1T_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define IDM1T_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define IDM1T_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define IDM1T_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define IDM1T_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define IDM1T_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define IDM1T_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define IDM1T_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define IDM1T_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define IDM1T_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM1T_IA_BANDWIDTH_3 Register IDM1T_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define IDM1T_IA_BANDWIDTH_3 0x11518
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1T_IA_BANDWIDTH_3 0x1BF11518u

//! Register Reset Value
#define IDM1T_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define IDM1T_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define IDM1T_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define IDM1T_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define IDM1T_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define IDM1T_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define IDM1T_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define IDM1T_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define IDM1T_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define IDM1T_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define IDM1T_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define IDM1T_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define IDM1T_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define IDM1T_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define IDM1T_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define IDM1T_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define IDM1T_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM1T_IA_BANDWIDTH_4 Register IDM1T_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define IDM1T_IA_BANDWIDTH_4 0x11520
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1T_IA_BANDWIDTH_4 0x1BF11520u

//! Register Reset Value
#define IDM1T_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define IDM1T_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define IDM1T_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define IDM1T_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define IDM1T_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define IDM1T_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define IDM1T_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define IDM1T_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define IDM1T_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define IDM1T_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define IDM1T_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define IDM1T_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define IDM1T_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define IDM1T_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define IDM1T_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define IDM1T_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define IDM1T_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM1T_IA_BANDWIDTH_5 Register IDM1T_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define IDM1T_IA_BANDWIDTH_5 0x11528
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1T_IA_BANDWIDTH_5 0x1BF11528u

//! Register Reset Value
#define IDM1T_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define IDM1T_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define IDM1T_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define IDM1T_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define IDM1T_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define IDM1T_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define IDM1T_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define IDM1T_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define IDM1T_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define IDM1T_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define IDM1T_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define IDM1T_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define IDM1T_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define IDM1T_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define IDM1T_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define IDM1T_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define IDM1T_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM1T_IA_BANDWIDTH_6 Register IDM1T_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define IDM1T_IA_BANDWIDTH_6 0x11530
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1T_IA_BANDWIDTH_6 0x1BF11530u

//! Register Reset Value
#define IDM1T_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define IDM1T_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define IDM1T_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define IDM1T_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define IDM1T_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define IDM1T_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define IDM1T_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define IDM1T_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define IDM1T_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define IDM1T_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define IDM1T_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define IDM1T_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define IDM1T_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define IDM1T_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define IDM1T_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define IDM1T_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define IDM1T_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup IDM1T_IA_BANDWIDTH_7 Register IDM1T_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define IDM1T_IA_BANDWIDTH_7 0x11538
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IDM1T_IA_BANDWIDTH_7 0x1BF11538u

//! Register Reset Value
#define IDM1T_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define IDM1T_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define IDM1T_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define IDM1T_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define IDM1T_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define IDM1T_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define IDM1T_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define IDM1T_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define IDM1T_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define IDM1T_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define IDM1T_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define IDM1T_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define IDM1T_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define IDM1T_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define IDM1T_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define IDM1T_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define IDM1T_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCMF_IA_COMPONENT Register IXCMF_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define IXCMF_IA_COMPONENT 0x11800
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IXCMF_IA_COMPONENT 0x1BF11800u

//! Register Reset Value
#define IXCMF_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define IXCMF_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define IXCMF_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define IXCMF_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define IXCMF_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup IXCMF_IA_CORE Register IXCMF_IA_CORE - core
//! @{

//! Register Offset (relative)
#define IXCMF_IA_CORE 0x11818
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IXCMF_IA_CORE 0x1BF11818u

//! Register Reset Value
#define IXCMF_IA_CORE_RST 0x000088C3002E0001u

//! Field REV_CODE - rev_code
#define IXCMF_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define IXCMF_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define IXCMF_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define IXCMF_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define IXCMF_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define IXCMF_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup IXCMF_IA_AGENT_CONTROL Register IXCMF_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define IXCMF_IA_AGENT_CONTROL 0x11820
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IXCMF_IA_AGENT_CONTROL 0x1BF11820u

//! Register Reset Value
#define IXCMF_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IXCMF_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IXCMF_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define IXCMF_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define IXCMF_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define IXCMF_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IXCMF_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define IXCMF_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IXCMF_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define IXCMF_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define IXCMF_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IXCMF_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define IXCMF_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IXCMF_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define IXCMF_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IXCMF_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define IXCMF_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IXCMF_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define IXCMF_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IXCMF_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define IXCMF_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IXCMF_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define IXCMF_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup IXCMF_IA_AGENT_STATUS Register IXCMF_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define IXCMF_IA_AGENT_STATUS 0x11828
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IXCMF_IA_AGENT_STATUS 0x1BF11828u

//! Register Reset Value
#define IXCMF_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define IXCMF_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define IXCMF_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define IXCMF_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define IXCMF_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define IXCMF_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define IXCMF_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define IXCMF_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define IXCMF_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define IXCMF_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define IXCMF_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define IXCMF_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define IXCMF_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define IXCMF_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define IXCMF_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define IXCMF_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define IXCMF_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define IXCMF_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define IXCMF_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IXCMF_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define IXCMF_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IXCMF_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define IXCMF_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup IXCMF_IA_ERROR_LOG Register IXCMF_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define IXCMF_IA_ERROR_LOG 0x11858
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IXCMF_IA_ERROR_LOG 0x1BF11858u

//! Register Reset Value
#define IXCMF_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define IXCMF_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define IXCMF_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define IXCMF_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define IXCMF_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define IXCMF_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define IXCMF_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define IXCMF_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define IXCMF_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define IXCMF_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define IXCMF_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define IXCMF_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define IXCMF_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define IXCMF_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define IXCMF_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define IXCMF_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define IXCMF_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define IXCMF_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define IXCMF_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup IXCMF_IA_ERROR_LOG_ADDR Register IXCMF_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define IXCMF_IA_ERROR_LOG_ADDR 0x11860
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IXCMF_IA_ERROR_LOG_ADDR 0x1BF11860u

//! Register Reset Value
#define IXCMF_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define IXCMF_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define IXCMF_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup IXCMF_IA_CORE_FLAG Register IXCMF_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define IXCMF_IA_CORE_FLAG 0x11868
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IXCMF_IA_CORE_FLAG 0x1BF11868u

//! Register Reset Value
#define IXCMF_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define IXCMF_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define IXCMF_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define IXCMF_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define IXCMF_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define IXCMF_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define IXCMF_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define IXCMF_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define IXCMF_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup IXCMF_IA_ADDR_FILL_IN Register IXCMF_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define IXCMF_IA_ADDR_FILL_IN 0x11870
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IXCMF_IA_ADDR_FILL_IN 0x1BF11870u

//! Register Reset Value
#define IXCMF_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define IXCMF_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define IXCMF_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup IXCMF_IA_BANDWIDTH_0 Register IXCMF_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define IXCMF_IA_BANDWIDTH_0 0x11900
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IXCMF_IA_BANDWIDTH_0 0x1BF11900u

//! Register Reset Value
#define IXCMF_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define IXCMF_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define IXCMF_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define IXCMF_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define IXCMF_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define IXCMF_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define IXCMF_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define IXCMF_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define IXCMF_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define IXCMF_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define IXCMF_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define IXCMF_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define IXCMF_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define IXCMF_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define IXCMF_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define IXCMF_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define IXCMF_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCMF_IA_BANDWIDTH_1 Register IXCMF_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define IXCMF_IA_BANDWIDTH_1 0x11908
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IXCMF_IA_BANDWIDTH_1 0x1BF11908u

//! Register Reset Value
#define IXCMF_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define IXCMF_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define IXCMF_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define IXCMF_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define IXCMF_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define IXCMF_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define IXCMF_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define IXCMF_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define IXCMF_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define IXCMF_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define IXCMF_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define IXCMF_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define IXCMF_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define IXCMF_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define IXCMF_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define IXCMF_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define IXCMF_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCMF_IA_BANDWIDTH_2 Register IXCMF_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define IXCMF_IA_BANDWIDTH_2 0x11910
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IXCMF_IA_BANDWIDTH_2 0x1BF11910u

//! Register Reset Value
#define IXCMF_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define IXCMF_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define IXCMF_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define IXCMF_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define IXCMF_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define IXCMF_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define IXCMF_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define IXCMF_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define IXCMF_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define IXCMF_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define IXCMF_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define IXCMF_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define IXCMF_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define IXCMF_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define IXCMF_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define IXCMF_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define IXCMF_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCMF_IA_BANDWIDTH_3 Register IXCMF_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define IXCMF_IA_BANDWIDTH_3 0x11918
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IXCMF_IA_BANDWIDTH_3 0x1BF11918u

//! Register Reset Value
#define IXCMF_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define IXCMF_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define IXCMF_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define IXCMF_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define IXCMF_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define IXCMF_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define IXCMF_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define IXCMF_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define IXCMF_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define IXCMF_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define IXCMF_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define IXCMF_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define IXCMF_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define IXCMF_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define IXCMF_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define IXCMF_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define IXCMF_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCMF_IA_BANDWIDTH_4 Register IXCMF_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define IXCMF_IA_BANDWIDTH_4 0x11920
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IXCMF_IA_BANDWIDTH_4 0x1BF11920u

//! Register Reset Value
#define IXCMF_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define IXCMF_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define IXCMF_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define IXCMF_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define IXCMF_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define IXCMF_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define IXCMF_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define IXCMF_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define IXCMF_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define IXCMF_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define IXCMF_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define IXCMF_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define IXCMF_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define IXCMF_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define IXCMF_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define IXCMF_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define IXCMF_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCMF_IA_BANDWIDTH_5 Register IXCMF_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define IXCMF_IA_BANDWIDTH_5 0x11928
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IXCMF_IA_BANDWIDTH_5 0x1BF11928u

//! Register Reset Value
#define IXCMF_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define IXCMF_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define IXCMF_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define IXCMF_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define IXCMF_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define IXCMF_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define IXCMF_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define IXCMF_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define IXCMF_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define IXCMF_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define IXCMF_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define IXCMF_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define IXCMF_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define IXCMF_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define IXCMF_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define IXCMF_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define IXCMF_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCMF_IA_BANDWIDTH_6 Register IXCMF_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define IXCMF_IA_BANDWIDTH_6 0x11930
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IXCMF_IA_BANDWIDTH_6 0x1BF11930u

//! Register Reset Value
#define IXCMF_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define IXCMF_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define IXCMF_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define IXCMF_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define IXCMF_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define IXCMF_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define IXCMF_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define IXCMF_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define IXCMF_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define IXCMF_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define IXCMF_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define IXCMF_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define IXCMF_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define IXCMF_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define IXCMF_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define IXCMF_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define IXCMF_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup IXCMF_IA_BANDWIDTH_7 Register IXCMF_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define IXCMF_IA_BANDWIDTH_7 0x11938
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_IXCMF_IA_BANDWIDTH_7 0x1BF11938u

//! Register Reset Value
#define IXCMF_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define IXCMF_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define IXCMF_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define IXCMF_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define IXCMF_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define IXCMF_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define IXCMF_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define IXCMF_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define IXCMF_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define IXCMF_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define IXCMF_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define IXCMF_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define IXCMF_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define IXCMF_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define IXCMF_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define IXCMF_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define IXCMF_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN02_IA_COMPONENT Register ILN02_IA_COMPONENT - component
//! @{

//! Register Offset (relative)
#define ILN02_IA_COMPONENT 0x11C00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_ILN02_IA_COMPONENT 0x1BF11C00u

//! Register Reset Value
#define ILN02_IA_COMPONENT_RST 0x0000000060103532u

//! Field REV - rev
#define ILN02_IA_COMPONENT_REV_POS 0
//! Field REV - rev
#define ILN02_IA_COMPONENT_REV_MASK 0xFFFFu

//! Field CODE - code
#define ILN02_IA_COMPONENT_CODE_POS 16
//! Field CODE - code
#define ILN02_IA_COMPONENT_CODE_MASK 0xFFFF0000u

//! @}

//! \defgroup ILN02_IA_CORE Register ILN02_IA_CORE - core
//! @{

//! Register Offset (relative)
#define ILN02_IA_CORE 0x11C18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_ILN02_IA_CORE 0x1BF11C18u

//! Register Reset Value
#define ILN02_IA_CORE_RST 0x000050C50F200001u

//! Field REV_CODE - rev_code
#define ILN02_IA_CORE_REV_CODE_POS 0
//! Field REV_CODE - rev_code
#define ILN02_IA_CORE_REV_CODE_MASK 0xFFFFu

//! Field CORE_CODE - core_code
#define ILN02_IA_CORE_CORE_CODE_POS 16
//! Field CORE_CODE - core_code
#define ILN02_IA_CORE_CORE_CODE_MASK 0xFFFF0000u

//! Field VENDOR_CODE - vendor_code
#define ILN02_IA_CORE_VENDOR_CODE_POS 32
//! Field VENDOR_CODE - vendor_code
#define ILN02_IA_CORE_VENDOR_CODE_MASK 0xFFFF00000000u

//! @}

//! \defgroup ILN02_IA_AGENT_CONTROL Register ILN02_IA_AGENT_CONTROL - agent_control
//! @{

//! Register Offset (relative)
#define ILN02_IA_AGENT_CONTROL 0x11C20
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_ILN02_IA_AGENT_CONTROL 0x1BF11C20u

//! Register Reset Value
#define ILN02_IA_AGENT_CONTROL_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define ILN02_IA_AGENT_CONTROL_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define ILN02_IA_AGENT_CONTROL_CORE_RESET_MASK 0x1u

//! Field REJECT - reject
#define ILN02_IA_AGENT_CONTROL_REJECT_POS 4
//! Field REJECT - reject
#define ILN02_IA_AGENT_CONTROL_REJECT_MASK 0x10u

//! Field RESP_TIMEOUT - resp_timeout
#define ILN02_IA_AGENT_CONTROL_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define ILN02_IA_AGENT_CONTROL_RESP_TIMEOUT_MASK 0x700u

//! Field BURST_TIMEOUT - burst_timeout
#define ILN02_IA_AGENT_CONTROL_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define ILN02_IA_AGENT_CONTROL_BURST_TIMEOUT_MASK 0x70000u

//! Field MERROR_REP - merror_rep
#define ILN02_IA_AGENT_CONTROL_MERROR_REP_POS 24
//! Field MERROR_REP - merror_rep
#define ILN02_IA_AGENT_CONTROL_MERROR_REP_MASK 0x1000000u

//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define ILN02_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_POS 25
//! Field RESP_TIMEOUT_REP - resp_timeout_rep
#define ILN02_IA_AGENT_CONTROL_RESP_TIMEOUT_REP_MASK 0x2000000u

//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define ILN02_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_POS 26
//! Field BURST_TIMEOUT_REP - burst_timeout_rep
#define ILN02_IA_AGENT_CONTROL_BURST_TIMEOUT_REP_MASK 0x4000000u

//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define ILN02_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_POS 27
//! Field ALL_INBAND_ERROR_REP - all_inband_error_rep
#define ILN02_IA_AGENT_CONTROL_ALL_INBAND_ERROR_REP_MASK 0x8000000u

//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define ILN02_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_POS 28
//! Field INBAND_ERROR_PRIMARY_REP - inband_error_primary_rep
#define ILN02_IA_AGENT_CONTROL_INBAND_ERROR_PRIMARY_REP_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define ILN02_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_POS 29
//! Field INBAND_ERROR_SECONDARY_REP - inband_error_secondary_rep
#define ILN02_IA_AGENT_CONTROL_INBAND_ERROR_SECONDARY_REP_MASK 0x20000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define ILN02_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define ILN02_IA_AGENT_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup ILN02_IA_AGENT_STATUS Register ILN02_IA_AGENT_STATUS - agent_status
//! @{

//! Register Offset (relative)
#define ILN02_IA_AGENT_STATUS 0x11C28
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_ILN02_IA_AGENT_STATUS 0x1BF11C28u

//! Register Reset Value
#define ILN02_IA_AGENT_STATUS_RST 0x0000000000000000u

//! Field CORE_RESET - core_reset
#define ILN02_IA_AGENT_STATUS_CORE_RESET_POS 0
//! Field CORE_RESET - core_reset
#define ILN02_IA_AGENT_STATUS_CORE_RESET_MASK 0x1u

//! Field REQ_ACTIVE - req_active
#define ILN02_IA_AGENT_STATUS_REQ_ACTIVE_POS 4
//! Field REQ_ACTIVE - req_active
#define ILN02_IA_AGENT_STATUS_REQ_ACTIVE_MASK 0x10u

//! Field RESP_WAITING - resp_waiting
#define ILN02_IA_AGENT_STATUS_RESP_WAITING_POS 5
//! Field RESP_WAITING - resp_waiting
#define ILN02_IA_AGENT_STATUS_RESP_WAITING_MASK 0x20u

//! Field BURST - burst
#define ILN02_IA_AGENT_STATUS_BURST_POS 6
//! Field BURST - burst
#define ILN02_IA_AGENT_STATUS_BURST_MASK 0x40u

//! Field READEX - readex
#define ILN02_IA_AGENT_STATUS_READEX_POS 7
//! Field READEX - readex
#define ILN02_IA_AGENT_STATUS_READEX_MASK 0x80u

//! Field RESP_TIMEOUT - resp_timeout
#define ILN02_IA_AGENT_STATUS_RESP_TIMEOUT_POS 8
//! Field RESP_TIMEOUT - resp_timeout
#define ILN02_IA_AGENT_STATUS_RESP_TIMEOUT_MASK 0x100u

//! Field TIMEBASE - timebase
#define ILN02_IA_AGENT_STATUS_TIMEBASE_POS 12
//! Field TIMEBASE - timebase
#define ILN02_IA_AGENT_STATUS_TIMEBASE_MASK 0xF000u

//! Field BURST_TIMEOUT - burst_timeout
#define ILN02_IA_AGENT_STATUS_BURST_TIMEOUT_POS 16
//! Field BURST_TIMEOUT - burst_timeout
#define ILN02_IA_AGENT_STATUS_BURST_TIMEOUT_MASK 0x10000u

//! Field MERROR - merror
#define ILN02_IA_AGENT_STATUS_MERROR_POS 24
//! Field MERROR - merror
#define ILN02_IA_AGENT_STATUS_MERROR_MASK 0x1000000u

//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define ILN02_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_POS 28
//! Field INBAND_ERROR_PRIMARY - inband_error_primary
#define ILN02_IA_AGENT_STATUS_INBAND_ERROR_PRIMARY_MASK 0x10000000u

//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define ILN02_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_POS 29
//! Field INBAND_ERROR_SECONDARY - inband_error_secondary
#define ILN02_IA_AGENT_STATUS_INBAND_ERROR_SECONDARY_MASK 0x20000000u

//! @}

//! \defgroup ILN02_IA_ERROR_LOG Register ILN02_IA_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define ILN02_IA_ERROR_LOG 0x11C58
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_ILN02_IA_ERROR_LOG 0x1BF11C58u

//! Register Reset Value
#define ILN02_IA_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define ILN02_IA_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define ILN02_IA_ERROR_LOG_CMD_MASK 0x7u

//! Field INITID - initid
#define ILN02_IA_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define ILN02_IA_ERROR_LOG_INITID_MASK 0xFF00u

//! Field RFU0 - rfu0
#define ILN02_IA_ERROR_LOG_RFU0_POS 16
//! Field RFU0 - rfu0
#define ILN02_IA_ERROR_LOG_RFU0_MASK 0x30000u

//! Field RFU1 - rfu1
#define ILN02_IA_ERROR_LOG_RFU1_POS 18
//! Field RFU1 - rfu1
#define ILN02_IA_ERROR_LOG_RFU1_MASK 0xC0000u

//! Field RFU2 - rfu2
#define ILN02_IA_ERROR_LOG_RFU2_POS 20
//! Field RFU2 - rfu2
#define ILN02_IA_ERROR_LOG_RFU2_MASK 0xF00000u

//! Field CODE - code
#define ILN02_IA_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define ILN02_IA_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define ILN02_IA_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define ILN02_IA_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define ILN02_IA_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define ILN02_IA_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field REQ_INFO - req_info
#define ILN02_IA_ERROR_LOG_REQ_INFO_POS 32
//! Field REQ_INFO - req_info
#define ILN02_IA_ERROR_LOG_REQ_INFO_MASK 0xFFFFFFFF00000000u

//! @}

//! \defgroup ILN02_IA_ERROR_LOG_ADDR Register ILN02_IA_ERROR_LOG_ADDR - error_log_addr
//! @{

//! Register Offset (relative)
#define ILN02_IA_ERROR_LOG_ADDR 0x11C60
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_ILN02_IA_ERROR_LOG_ADDR 0x1BF11C60u

//! Register Reset Value
#define ILN02_IA_ERROR_LOG_ADDR_RST 0x0000000000000000u

//! Field ADDR - addr
#define ILN02_IA_ERROR_LOG_ADDR_ADDR_POS 0
//! Field ADDR - addr
#define ILN02_IA_ERROR_LOG_ADDR_ADDR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup ILN02_IA_CORE_FLAG Register ILN02_IA_CORE_FLAG - core_flag
//! @{

//! Register Offset (relative)
#define ILN02_IA_CORE_FLAG 0x11C68
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_ILN02_IA_CORE_FLAG 0x1BF11C68u

//! Register Reset Value
#define ILN02_IA_CORE_FLAG_RST 0x0000000000000000u

//! Field ENABLE_0 - enable_0
#define ILN02_IA_CORE_FLAG_ENABLE_0_POS 0
//! Field ENABLE_0 - enable_0
#define ILN02_IA_CORE_FLAG_ENABLE_0_MASK 0x1u

//! Field ENABLE_1 - enable_1
#define ILN02_IA_CORE_FLAG_ENABLE_1_POS 1
//! Field ENABLE_1 - enable_1
#define ILN02_IA_CORE_FLAG_ENABLE_1_MASK 0x2u

//! Field ENABLE_2 - enable_2
#define ILN02_IA_CORE_FLAG_ENABLE_2_POS 2
//! Field ENABLE_2 - enable_2
#define ILN02_IA_CORE_FLAG_ENABLE_2_MASK 0x4u

//! Field ENABLE_3 - enable_3
#define ILN02_IA_CORE_FLAG_ENABLE_3_POS 3
//! Field ENABLE_3 - enable_3
#define ILN02_IA_CORE_FLAG_ENABLE_3_MASK 0x8u

//! @}

//! \defgroup ILN02_IA_ADDR_FILL_IN Register ILN02_IA_ADDR_FILL_IN - addr_fill_in
//! @{

//! Register Offset (relative)
#define ILN02_IA_ADDR_FILL_IN 0x11C70
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_ILN02_IA_ADDR_FILL_IN 0x1BF11C70u

//! Register Reset Value
#define ILN02_IA_ADDR_FILL_IN_RST 0x0000000000000000u

//! Field VALUE - value
#define ILN02_IA_ADDR_FILL_IN_VALUE_POS 10
//! Field VALUE - value
#define ILN02_IA_ADDR_FILL_IN_VALUE_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup ILN02_IA_BANDWIDTH_0 Register ILN02_IA_BANDWIDTH_0 - bandwidth_0
//! @{

//! Register Offset (relative)
#define ILN02_IA_BANDWIDTH_0 0x11D00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_ILN02_IA_BANDWIDTH_0 0x1BF11D00u

//! Register Reset Value
#define ILN02_IA_BANDWIDTH_0_RST 0x0101010101010101u

//! Field TARGET_GROUP_0 - target_group_0
#define ILN02_IA_BANDWIDTH_0_TARGET_GROUP_0_POS 0
//! Field TARGET_GROUP_0 - target_group_0
#define ILN02_IA_BANDWIDTH_0_TARGET_GROUP_0_MASK 0xFFu

//! Field TARGET_GROUP_1 - target_group_1
#define ILN02_IA_BANDWIDTH_0_TARGET_GROUP_1_POS 8
//! Field TARGET_GROUP_1 - target_group_1
#define ILN02_IA_BANDWIDTH_0_TARGET_GROUP_1_MASK 0xFF00u

//! Field TARGET_GROUP_2 - target_group_2
#define ILN02_IA_BANDWIDTH_0_TARGET_GROUP_2_POS 16
//! Field TARGET_GROUP_2 - target_group_2
#define ILN02_IA_BANDWIDTH_0_TARGET_GROUP_2_MASK 0xFF0000u

//! Field TARGET_GROUP_3 - target_group_3
#define ILN02_IA_BANDWIDTH_0_TARGET_GROUP_3_POS 24
//! Field TARGET_GROUP_3 - target_group_3
#define ILN02_IA_BANDWIDTH_0_TARGET_GROUP_3_MASK 0xFF000000u

//! Field TARGET_GROUP_4 - target_group_4
#define ILN02_IA_BANDWIDTH_0_TARGET_GROUP_4_POS 32
//! Field TARGET_GROUP_4 - target_group_4
#define ILN02_IA_BANDWIDTH_0_TARGET_GROUP_4_MASK 0xFF00000000u

//! Field TARGET_GROUP_5 - target_group_5
#define ILN02_IA_BANDWIDTH_0_TARGET_GROUP_5_POS 40
//! Field TARGET_GROUP_5 - target_group_5
#define ILN02_IA_BANDWIDTH_0_TARGET_GROUP_5_MASK 0xFF0000000000u

//! Field TARGET_GROUP_6 - target_group_6
#define ILN02_IA_BANDWIDTH_0_TARGET_GROUP_6_POS 48
//! Field TARGET_GROUP_6 - target_group_6
#define ILN02_IA_BANDWIDTH_0_TARGET_GROUP_6_MASK 0xFF000000000000u

//! Field TARGET_GROUP_7 - target_group_7
#define ILN02_IA_BANDWIDTH_0_TARGET_GROUP_7_POS 56
//! Field TARGET_GROUP_7 - target_group_7
#define ILN02_IA_BANDWIDTH_0_TARGET_GROUP_7_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN02_IA_BANDWIDTH_1 Register ILN02_IA_BANDWIDTH_1 - bandwidth_1
//! @{

//! Register Offset (relative)
#define ILN02_IA_BANDWIDTH_1 0x11D08
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_ILN02_IA_BANDWIDTH_1 0x1BF11D08u

//! Register Reset Value
#define ILN02_IA_BANDWIDTH_1_RST 0x0101010101010101u

//! Field TARGET_GROUP_8 - target_group_8
#define ILN02_IA_BANDWIDTH_1_TARGET_GROUP_8_POS 0
//! Field TARGET_GROUP_8 - target_group_8
#define ILN02_IA_BANDWIDTH_1_TARGET_GROUP_8_MASK 0xFFu

//! Field TARGET_GROUP_9 - target_group_9
#define ILN02_IA_BANDWIDTH_1_TARGET_GROUP_9_POS 8
//! Field TARGET_GROUP_9 - target_group_9
#define ILN02_IA_BANDWIDTH_1_TARGET_GROUP_9_MASK 0xFF00u

//! Field TARGET_GROUP_10 - target_group_10
#define ILN02_IA_BANDWIDTH_1_TARGET_GROUP_10_POS 16
//! Field TARGET_GROUP_10 - target_group_10
#define ILN02_IA_BANDWIDTH_1_TARGET_GROUP_10_MASK 0xFF0000u

//! Field TARGET_GROUP_11 - target_group_11
#define ILN02_IA_BANDWIDTH_1_TARGET_GROUP_11_POS 24
//! Field TARGET_GROUP_11 - target_group_11
#define ILN02_IA_BANDWIDTH_1_TARGET_GROUP_11_MASK 0xFF000000u

//! Field TARGET_GROUP_12 - target_group_12
#define ILN02_IA_BANDWIDTH_1_TARGET_GROUP_12_POS 32
//! Field TARGET_GROUP_12 - target_group_12
#define ILN02_IA_BANDWIDTH_1_TARGET_GROUP_12_MASK 0xFF00000000u

//! Field TARGET_GROUP_13 - target_group_13
#define ILN02_IA_BANDWIDTH_1_TARGET_GROUP_13_POS 40
//! Field TARGET_GROUP_13 - target_group_13
#define ILN02_IA_BANDWIDTH_1_TARGET_GROUP_13_MASK 0xFF0000000000u

//! Field TARGET_GROUP_14 - target_group_14
#define ILN02_IA_BANDWIDTH_1_TARGET_GROUP_14_POS 48
//! Field TARGET_GROUP_14 - target_group_14
#define ILN02_IA_BANDWIDTH_1_TARGET_GROUP_14_MASK 0xFF000000000000u

//! Field TARGET_GROUP_15 - target_group_15
#define ILN02_IA_BANDWIDTH_1_TARGET_GROUP_15_POS 56
//! Field TARGET_GROUP_15 - target_group_15
#define ILN02_IA_BANDWIDTH_1_TARGET_GROUP_15_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN02_IA_BANDWIDTH_2 Register ILN02_IA_BANDWIDTH_2 - bandwidth_2
//! @{

//! Register Offset (relative)
#define ILN02_IA_BANDWIDTH_2 0x11D10
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_ILN02_IA_BANDWIDTH_2 0x1BF11D10u

//! Register Reset Value
#define ILN02_IA_BANDWIDTH_2_RST 0x0101010101010101u

//! Field TARGET_GROUP_16 - target_group_16
#define ILN02_IA_BANDWIDTH_2_TARGET_GROUP_16_POS 0
//! Field TARGET_GROUP_16 - target_group_16
#define ILN02_IA_BANDWIDTH_2_TARGET_GROUP_16_MASK 0xFFu

//! Field TARGET_GROUP_17 - target_group_17
#define ILN02_IA_BANDWIDTH_2_TARGET_GROUP_17_POS 8
//! Field TARGET_GROUP_17 - target_group_17
#define ILN02_IA_BANDWIDTH_2_TARGET_GROUP_17_MASK 0xFF00u

//! Field TARGET_GROUP_18 - target_group_18
#define ILN02_IA_BANDWIDTH_2_TARGET_GROUP_18_POS 16
//! Field TARGET_GROUP_18 - target_group_18
#define ILN02_IA_BANDWIDTH_2_TARGET_GROUP_18_MASK 0xFF0000u

//! Field TARGET_GROUP_19 - target_group_19
#define ILN02_IA_BANDWIDTH_2_TARGET_GROUP_19_POS 24
//! Field TARGET_GROUP_19 - target_group_19
#define ILN02_IA_BANDWIDTH_2_TARGET_GROUP_19_MASK 0xFF000000u

//! Field TARGET_GROUP_20 - target_group_20
#define ILN02_IA_BANDWIDTH_2_TARGET_GROUP_20_POS 32
//! Field TARGET_GROUP_20 - target_group_20
#define ILN02_IA_BANDWIDTH_2_TARGET_GROUP_20_MASK 0xFF00000000u

//! Field TARGET_GROUP_21 - target_group_21
#define ILN02_IA_BANDWIDTH_2_TARGET_GROUP_21_POS 40
//! Field TARGET_GROUP_21 - target_group_21
#define ILN02_IA_BANDWIDTH_2_TARGET_GROUP_21_MASK 0xFF0000000000u

//! Field TARGET_GROUP_22 - target_group_22
#define ILN02_IA_BANDWIDTH_2_TARGET_GROUP_22_POS 48
//! Field TARGET_GROUP_22 - target_group_22
#define ILN02_IA_BANDWIDTH_2_TARGET_GROUP_22_MASK 0xFF000000000000u

//! Field TARGET_GROUP_23 - target_group_23
#define ILN02_IA_BANDWIDTH_2_TARGET_GROUP_23_POS 56
//! Field TARGET_GROUP_23 - target_group_23
#define ILN02_IA_BANDWIDTH_2_TARGET_GROUP_23_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN02_IA_BANDWIDTH_3 Register ILN02_IA_BANDWIDTH_3 - bandwidth_3
//! @{

//! Register Offset (relative)
#define ILN02_IA_BANDWIDTH_3 0x11D18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_ILN02_IA_BANDWIDTH_3 0x1BF11D18u

//! Register Reset Value
#define ILN02_IA_BANDWIDTH_3_RST 0x0101010101010101u

//! Field TARGET_GROUP_24 - target_group_24
#define ILN02_IA_BANDWIDTH_3_TARGET_GROUP_24_POS 0
//! Field TARGET_GROUP_24 - target_group_24
#define ILN02_IA_BANDWIDTH_3_TARGET_GROUP_24_MASK 0xFFu

//! Field TARGET_GROUP_25 - target_group_25
#define ILN02_IA_BANDWIDTH_3_TARGET_GROUP_25_POS 8
//! Field TARGET_GROUP_25 - target_group_25
#define ILN02_IA_BANDWIDTH_3_TARGET_GROUP_25_MASK 0xFF00u

//! Field TARGET_GROUP_26 - target_group_26
#define ILN02_IA_BANDWIDTH_3_TARGET_GROUP_26_POS 16
//! Field TARGET_GROUP_26 - target_group_26
#define ILN02_IA_BANDWIDTH_3_TARGET_GROUP_26_MASK 0xFF0000u

//! Field TARGET_GROUP_27 - target_group_27
#define ILN02_IA_BANDWIDTH_3_TARGET_GROUP_27_POS 24
//! Field TARGET_GROUP_27 - target_group_27
#define ILN02_IA_BANDWIDTH_3_TARGET_GROUP_27_MASK 0xFF000000u

//! Field TARGET_GROUP_28 - target_group_28
#define ILN02_IA_BANDWIDTH_3_TARGET_GROUP_28_POS 32
//! Field TARGET_GROUP_28 - target_group_28
#define ILN02_IA_BANDWIDTH_3_TARGET_GROUP_28_MASK 0xFF00000000u

//! Field TARGET_GROUP_29 - target_group_29
#define ILN02_IA_BANDWIDTH_3_TARGET_GROUP_29_POS 40
//! Field TARGET_GROUP_29 - target_group_29
#define ILN02_IA_BANDWIDTH_3_TARGET_GROUP_29_MASK 0xFF0000000000u

//! Field TARGET_GROUP_30 - target_group_30
#define ILN02_IA_BANDWIDTH_3_TARGET_GROUP_30_POS 48
//! Field TARGET_GROUP_30 - target_group_30
#define ILN02_IA_BANDWIDTH_3_TARGET_GROUP_30_MASK 0xFF000000000000u

//! Field TARGET_GROUP_31 - target_group_31
#define ILN02_IA_BANDWIDTH_3_TARGET_GROUP_31_POS 56
//! Field TARGET_GROUP_31 - target_group_31
#define ILN02_IA_BANDWIDTH_3_TARGET_GROUP_31_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN02_IA_BANDWIDTH_4 Register ILN02_IA_BANDWIDTH_4 - bandwidth_4
//! @{

//! Register Offset (relative)
#define ILN02_IA_BANDWIDTH_4 0x11D20
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_ILN02_IA_BANDWIDTH_4 0x1BF11D20u

//! Register Reset Value
#define ILN02_IA_BANDWIDTH_4_RST 0x0101010101010101u

//! Field TARGET_GROUP_32 - target_group_32
#define ILN02_IA_BANDWIDTH_4_TARGET_GROUP_32_POS 0
//! Field TARGET_GROUP_32 - target_group_32
#define ILN02_IA_BANDWIDTH_4_TARGET_GROUP_32_MASK 0xFFu

//! Field TARGET_GROUP_33 - target_group_33
#define ILN02_IA_BANDWIDTH_4_TARGET_GROUP_33_POS 8
//! Field TARGET_GROUP_33 - target_group_33
#define ILN02_IA_BANDWIDTH_4_TARGET_GROUP_33_MASK 0xFF00u

//! Field TARGET_GROUP_34 - target_group_34
#define ILN02_IA_BANDWIDTH_4_TARGET_GROUP_34_POS 16
//! Field TARGET_GROUP_34 - target_group_34
#define ILN02_IA_BANDWIDTH_4_TARGET_GROUP_34_MASK 0xFF0000u

//! Field TARGET_GROUP_35 - target_group_35
#define ILN02_IA_BANDWIDTH_4_TARGET_GROUP_35_POS 24
//! Field TARGET_GROUP_35 - target_group_35
#define ILN02_IA_BANDWIDTH_4_TARGET_GROUP_35_MASK 0xFF000000u

//! Field TARGET_GROUP_36 - target_group_36
#define ILN02_IA_BANDWIDTH_4_TARGET_GROUP_36_POS 32
//! Field TARGET_GROUP_36 - target_group_36
#define ILN02_IA_BANDWIDTH_4_TARGET_GROUP_36_MASK 0xFF00000000u

//! Field TARGET_GROUP_37 - target_group_37
#define ILN02_IA_BANDWIDTH_4_TARGET_GROUP_37_POS 40
//! Field TARGET_GROUP_37 - target_group_37
#define ILN02_IA_BANDWIDTH_4_TARGET_GROUP_37_MASK 0xFF0000000000u

//! Field TARGET_GROUP_38 - target_group_38
#define ILN02_IA_BANDWIDTH_4_TARGET_GROUP_38_POS 48
//! Field TARGET_GROUP_38 - target_group_38
#define ILN02_IA_BANDWIDTH_4_TARGET_GROUP_38_MASK 0xFF000000000000u

//! Field TARGET_GROUP_39 - target_group_39
#define ILN02_IA_BANDWIDTH_4_TARGET_GROUP_39_POS 56
//! Field TARGET_GROUP_39 - target_group_39
#define ILN02_IA_BANDWIDTH_4_TARGET_GROUP_39_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN02_IA_BANDWIDTH_5 Register ILN02_IA_BANDWIDTH_5 - bandwidth_5
//! @{

//! Register Offset (relative)
#define ILN02_IA_BANDWIDTH_5 0x11D28
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_ILN02_IA_BANDWIDTH_5 0x1BF11D28u

//! Register Reset Value
#define ILN02_IA_BANDWIDTH_5_RST 0x0101010101010101u

//! Field TARGET_GROUP_40 - target_group_40
#define ILN02_IA_BANDWIDTH_5_TARGET_GROUP_40_POS 0
//! Field TARGET_GROUP_40 - target_group_40
#define ILN02_IA_BANDWIDTH_5_TARGET_GROUP_40_MASK 0xFFu

//! Field TARGET_GROUP_41 - target_group_41
#define ILN02_IA_BANDWIDTH_5_TARGET_GROUP_41_POS 8
//! Field TARGET_GROUP_41 - target_group_41
#define ILN02_IA_BANDWIDTH_5_TARGET_GROUP_41_MASK 0xFF00u

//! Field TARGET_GROUP_42 - target_group_42
#define ILN02_IA_BANDWIDTH_5_TARGET_GROUP_42_POS 16
//! Field TARGET_GROUP_42 - target_group_42
#define ILN02_IA_BANDWIDTH_5_TARGET_GROUP_42_MASK 0xFF0000u

//! Field TARGET_GROUP_43 - target_group_43
#define ILN02_IA_BANDWIDTH_5_TARGET_GROUP_43_POS 24
//! Field TARGET_GROUP_43 - target_group_43
#define ILN02_IA_BANDWIDTH_5_TARGET_GROUP_43_MASK 0xFF000000u

//! Field TARGET_GROUP_44 - target_group_44
#define ILN02_IA_BANDWIDTH_5_TARGET_GROUP_44_POS 32
//! Field TARGET_GROUP_44 - target_group_44
#define ILN02_IA_BANDWIDTH_5_TARGET_GROUP_44_MASK 0xFF00000000u

//! Field TARGET_GROUP_45 - target_group_45
#define ILN02_IA_BANDWIDTH_5_TARGET_GROUP_45_POS 40
//! Field TARGET_GROUP_45 - target_group_45
#define ILN02_IA_BANDWIDTH_5_TARGET_GROUP_45_MASK 0xFF0000000000u

//! Field TARGET_GROUP_46 - target_group_46
#define ILN02_IA_BANDWIDTH_5_TARGET_GROUP_46_POS 48
//! Field TARGET_GROUP_46 - target_group_46
#define ILN02_IA_BANDWIDTH_5_TARGET_GROUP_46_MASK 0xFF000000000000u

//! Field TARGET_GROUP_47 - target_group_47
#define ILN02_IA_BANDWIDTH_5_TARGET_GROUP_47_POS 56
//! Field TARGET_GROUP_47 - target_group_47
#define ILN02_IA_BANDWIDTH_5_TARGET_GROUP_47_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN02_IA_BANDWIDTH_6 Register ILN02_IA_BANDWIDTH_6 - bandwidth_6
//! @{

//! Register Offset (relative)
#define ILN02_IA_BANDWIDTH_6 0x11D30
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_ILN02_IA_BANDWIDTH_6 0x1BF11D30u

//! Register Reset Value
#define ILN02_IA_BANDWIDTH_6_RST 0x0101010101010101u

//! Field TARGET_GROUP_48 - target_group_48
#define ILN02_IA_BANDWIDTH_6_TARGET_GROUP_48_POS 0
//! Field TARGET_GROUP_48 - target_group_48
#define ILN02_IA_BANDWIDTH_6_TARGET_GROUP_48_MASK 0xFFu

//! Field TARGET_GROUP_49 - target_group_49
#define ILN02_IA_BANDWIDTH_6_TARGET_GROUP_49_POS 8
//! Field TARGET_GROUP_49 - target_group_49
#define ILN02_IA_BANDWIDTH_6_TARGET_GROUP_49_MASK 0xFF00u

//! Field TARGET_GROUP_50 - target_group_50
#define ILN02_IA_BANDWIDTH_6_TARGET_GROUP_50_POS 16
//! Field TARGET_GROUP_50 - target_group_50
#define ILN02_IA_BANDWIDTH_6_TARGET_GROUP_50_MASK 0xFF0000u

//! Field TARGET_GROUP_51 - target_group_51
#define ILN02_IA_BANDWIDTH_6_TARGET_GROUP_51_POS 24
//! Field TARGET_GROUP_51 - target_group_51
#define ILN02_IA_BANDWIDTH_6_TARGET_GROUP_51_MASK 0xFF000000u

//! Field TARGET_GROUP_52 - target_group_52
#define ILN02_IA_BANDWIDTH_6_TARGET_GROUP_52_POS 32
//! Field TARGET_GROUP_52 - target_group_52
#define ILN02_IA_BANDWIDTH_6_TARGET_GROUP_52_MASK 0xFF00000000u

//! Field TARGET_GROUP_53 - target_group_53
#define ILN02_IA_BANDWIDTH_6_TARGET_GROUP_53_POS 40
//! Field TARGET_GROUP_53 - target_group_53
#define ILN02_IA_BANDWIDTH_6_TARGET_GROUP_53_MASK 0xFF0000000000u

//! Field TARGET_GROUP_54 - target_group_54
#define ILN02_IA_BANDWIDTH_6_TARGET_GROUP_54_POS 48
//! Field TARGET_GROUP_54 - target_group_54
#define ILN02_IA_BANDWIDTH_6_TARGET_GROUP_54_MASK 0xFF000000000000u

//! Field TARGET_GROUP_55 - target_group_55
#define ILN02_IA_BANDWIDTH_6_TARGET_GROUP_55_POS 56
//! Field TARGET_GROUP_55 - target_group_55
#define ILN02_IA_BANDWIDTH_6_TARGET_GROUP_55_MASK 0xFF00000000000000u

//! @}

//! \defgroup ILN02_IA_BANDWIDTH_7 Register ILN02_IA_BANDWIDTH_7 - bandwidth_7
//! @{

//! Register Offset (relative)
#define ILN02_IA_BANDWIDTH_7 0x11D38
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_ILN02_IA_BANDWIDTH_7 0x1BF11D38u

//! Register Reset Value
#define ILN02_IA_BANDWIDTH_7_RST 0x0101010101010101u

//! Field TARGET_GROUP_56 - target_group_56
#define ILN02_IA_BANDWIDTH_7_TARGET_GROUP_56_POS 0
//! Field TARGET_GROUP_56 - target_group_56
#define ILN02_IA_BANDWIDTH_7_TARGET_GROUP_56_MASK 0xFFu

//! Field TARGET_GROUP_57 - target_group_57
#define ILN02_IA_BANDWIDTH_7_TARGET_GROUP_57_POS 8
//! Field TARGET_GROUP_57 - target_group_57
#define ILN02_IA_BANDWIDTH_7_TARGET_GROUP_57_MASK 0xFF00u

//! Field TARGET_GROUP_58 - target_group_58
#define ILN02_IA_BANDWIDTH_7_TARGET_GROUP_58_POS 16
//! Field TARGET_GROUP_58 - target_group_58
#define ILN02_IA_BANDWIDTH_7_TARGET_GROUP_58_MASK 0xFF0000u

//! Field TARGET_GROUP_59 - target_group_59
#define ILN02_IA_BANDWIDTH_7_TARGET_GROUP_59_POS 24
//! Field TARGET_GROUP_59 - target_group_59
#define ILN02_IA_BANDWIDTH_7_TARGET_GROUP_59_MASK 0xFF000000u

//! Field TARGET_GROUP_60 - target_group_60
#define ILN02_IA_BANDWIDTH_7_TARGET_GROUP_60_POS 32
//! Field TARGET_GROUP_60 - target_group_60
#define ILN02_IA_BANDWIDTH_7_TARGET_GROUP_60_MASK 0xFF00000000u

//! Field TARGET_GROUP_61 - target_group_61
#define ILN02_IA_BANDWIDTH_7_TARGET_GROUP_61_POS 40
//! Field TARGET_GROUP_61 - target_group_61
#define ILN02_IA_BANDWIDTH_7_TARGET_GROUP_61_MASK 0xFF0000000000u

//! Field TARGET_GROUP_62 - target_group_62
#define ILN02_IA_BANDWIDTH_7_TARGET_GROUP_62_POS 48
//! Field TARGET_GROUP_62 - target_group_62
#define ILN02_IA_BANDWIDTH_7_TARGET_GROUP_62_MASK 0xFF000000000000u

//! Field TARGET_GROUP_63 - target_group_63
#define ILN02_IA_BANDWIDTH_7_TARGET_GROUP_63_POS 56
//! Field TARGET_GROUP_63 - target_group_63
#define ILN02_IA_BANDWIDTH_7_TARGET_GROUP_63_MASK 0xFF00000000000000u

//! @}

//! \defgroup TREG2_PM_ERROR_LOG Register TREG2_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TREG2_PM_ERROR_LOG 0x80020
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ERROR_LOG 0x1BF80020u

//! Register Reset Value
#define TREG2_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TREG2_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TREG2_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TREG2_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TREG2_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TREG2_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TREG2_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TREG2_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TREG2_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TREG2_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TREG2_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TREG2_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TREG2_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TREG2_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TREG2_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TREG2_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TREG2_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TREG2_PM_CONTROL Register TREG2_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TREG2_PM_CONTROL 0x80028
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_CONTROL 0x1BF80028u

//! Register Reset Value
#define TREG2_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TREG2_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TREG2_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TREG2_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TREG2_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TREG2_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TREG2_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TREG2_PM_ERROR_CLEAR_SINGLE Register TREG2_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TREG2_PM_ERROR_CLEAR_SINGLE 0x80030
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ERROR_CLEAR_SINGLE 0x1BF80030u

//! Register Reset Value
#define TREG2_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TREG2_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TREG2_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TREG2_PM_ERROR_CLEAR_MULTI Register TREG2_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TREG2_PM_ERROR_CLEAR_MULTI 0x80038
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ERROR_CLEAR_MULTI 0x1BF80038u

//! Register Reset Value
#define TREG2_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TREG2_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TREG2_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TREG2_PM_REQ_INFO_PERMISSION_0 Register TREG2_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TREG2_PM_REQ_INFO_PERMISSION_0 0x80048
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_REQ_INFO_PERMISSION_0 0x1BF80048u

//! Register Reset Value
#define TREG2_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_READ_PERMISSION_0 Register TREG2_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TREG2_PM_READ_PERMISSION_0 0x80050
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_READ_PERMISSION_0 0x1BF80050u

//! Register Reset Value
#define TREG2_PM_READ_PERMISSION_0_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TREG2_PM_WRITE_PERMISSION_0 Register TREG2_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TREG2_PM_WRITE_PERMISSION_0 0x80058
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_WRITE_PERMISSION_0 0x1BF80058u

//! Register Reset Value
#define TREG2_PM_WRITE_PERMISSION_0_RST 0x000000000000001Fu

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TREG2_PM_ADDR_MATCH_1 Register TREG2_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TREG2_PM_ADDR_MATCH_1 0x80060
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ADDR_MATCH_1 0x1BF80060u

//! Register Reset Value
#define TREG2_PM_ADDR_MATCH_1_RST 0x0000000000080050u

//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG2_PM_REQ_INFO_PERMISSION_1 Register TREG2_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TREG2_PM_REQ_INFO_PERMISSION_1 0x80068
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_REQ_INFO_PERMISSION_1 0x1BF80068u

//! Register Reset Value
#define TREG2_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_READ_PERMISSION_1 Register TREG2_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TREG2_PM_READ_PERMISSION_1 0x80070
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_READ_PERMISSION_1 0x1BF80070u

//! Register Reset Value
#define TREG2_PM_READ_PERMISSION_1_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TREG2_PM_WRITE_PERMISSION_1 Register TREG2_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TREG2_PM_WRITE_PERMISSION_1 0x80078
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_WRITE_PERMISSION_1 0x1BF80078u

//! Register Reset Value
#define TREG2_PM_WRITE_PERMISSION_1_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TREG2_PM_ADDR_MATCH_2 Register TREG2_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TREG2_PM_ADDR_MATCH_2 0x80080
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ADDR_MATCH_2 0x1BF80080u

//! Register Reset Value
#define TREG2_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG2_PM_REQ_INFO_PERMISSION_2 Register TREG2_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TREG2_PM_REQ_INFO_PERMISSION_2 0x80088
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_REQ_INFO_PERMISSION_2 0x1BF80088u

//! Register Reset Value
#define TREG2_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_READ_PERMISSION_2 Register TREG2_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TREG2_PM_READ_PERMISSION_2 0x80090
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_READ_PERMISSION_2 0x1BF80090u

//! Register Reset Value
#define TREG2_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_WRITE_PERMISSION_2 Register TREG2_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TREG2_PM_WRITE_PERMISSION_2 0x80098
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_WRITE_PERMISSION_2 0x1BF80098u

//! Register Reset Value
#define TREG2_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_ADDR_MATCH_3 Register TREG2_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TREG2_PM_ADDR_MATCH_3 0x800A0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ADDR_MATCH_3 0x1BF800A0u

//! Register Reset Value
#define TREG2_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG2_PM_REQ_INFO_PERMISSION_3 Register TREG2_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TREG2_PM_REQ_INFO_PERMISSION_3 0x800A8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_REQ_INFO_PERMISSION_3 0x1BF800A8u

//! Register Reset Value
#define TREG2_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_READ_PERMISSION_3 Register TREG2_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TREG2_PM_READ_PERMISSION_3 0x800B0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_READ_PERMISSION_3 0x1BF800B0u

//! Register Reset Value
#define TREG2_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_WRITE_PERMISSION_3 Register TREG2_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TREG2_PM_WRITE_PERMISSION_3 0x800B8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_WRITE_PERMISSION_3 0x1BF800B8u

//! Register Reset Value
#define TREG2_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_ADDR_MATCH_4 Register TREG2_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TREG2_PM_ADDR_MATCH_4 0x800C0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ADDR_MATCH_4 0x1BF800C0u

//! Register Reset Value
#define TREG2_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG2_PM_REQ_INFO_PERMISSION_4 Register TREG2_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TREG2_PM_REQ_INFO_PERMISSION_4 0x800C8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_REQ_INFO_PERMISSION_4 0x1BF800C8u

//! Register Reset Value
#define TREG2_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_READ_PERMISSION_4 Register TREG2_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TREG2_PM_READ_PERMISSION_4 0x800D0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_READ_PERMISSION_4 0x1BF800D0u

//! Register Reset Value
#define TREG2_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_WRITE_PERMISSION_4 Register TREG2_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TREG2_PM_WRITE_PERMISSION_4 0x800D8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_WRITE_PERMISSION_4 0x1BF800D8u

//! Register Reset Value
#define TREG2_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_ADDR_MATCH_5 Register TREG2_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TREG2_PM_ADDR_MATCH_5 0x800E0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ADDR_MATCH_5 0x1BF800E0u

//! Register Reset Value
#define TREG2_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG2_PM_REQ_INFO_PERMISSION_5 Register TREG2_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TREG2_PM_REQ_INFO_PERMISSION_5 0x800E8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_REQ_INFO_PERMISSION_5 0x1BF800E8u

//! Register Reset Value
#define TREG2_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_READ_PERMISSION_5 Register TREG2_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TREG2_PM_READ_PERMISSION_5 0x800F0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_READ_PERMISSION_5 0x1BF800F0u

//! Register Reset Value
#define TREG2_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_WRITE_PERMISSION_5 Register TREG2_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TREG2_PM_WRITE_PERMISSION_5 0x800F8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_WRITE_PERMISSION_5 0x1BF800F8u

//! Register Reset Value
#define TREG2_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_ADDR_MATCH_6 Register TREG2_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TREG2_PM_ADDR_MATCH_6 0x80100
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ADDR_MATCH_6 0x1BF80100u

//! Register Reset Value
#define TREG2_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG2_PM_REQ_INFO_PERMISSION_6 Register TREG2_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TREG2_PM_REQ_INFO_PERMISSION_6 0x80108
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_REQ_INFO_PERMISSION_6 0x1BF80108u

//! Register Reset Value
#define TREG2_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_READ_PERMISSION_6 Register TREG2_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TREG2_PM_READ_PERMISSION_6 0x80110
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_READ_PERMISSION_6 0x1BF80110u

//! Register Reset Value
#define TREG2_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_WRITE_PERMISSION_6 Register TREG2_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TREG2_PM_WRITE_PERMISSION_6 0x80118
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_WRITE_PERMISSION_6 0x1BF80118u

//! Register Reset Value
#define TREG2_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_ADDR_MATCH_7 Register TREG2_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TREG2_PM_ADDR_MATCH_7 0x80120
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ADDR_MATCH_7 0x1BF80120u

//! Register Reset Value
#define TREG2_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG2_PM_REQ_INFO_PERMISSION_7 Register TREG2_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TREG2_PM_REQ_INFO_PERMISSION_7 0x80128
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_REQ_INFO_PERMISSION_7 0x1BF80128u

//! Register Reset Value
#define TREG2_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_READ_PERMISSION_7 Register TREG2_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TREG2_PM_READ_PERMISSION_7 0x80130
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_READ_PERMISSION_7 0x1BF80130u

//! Register Reset Value
#define TREG2_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_WRITE_PERMISSION_7 Register TREG2_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TREG2_PM_WRITE_PERMISSION_7 0x80138
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_WRITE_PERMISSION_7 0x1BF80138u

//! Register Reset Value
#define TREG2_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_ADDR_MATCH_8 Register TREG2_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TREG2_PM_ADDR_MATCH_8 0x80140
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ADDR_MATCH_8 0x1BF80140u

//! Register Reset Value
#define TREG2_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG2_PM_REQ_INFO_PERMISSION_8 Register TREG2_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TREG2_PM_REQ_INFO_PERMISSION_8 0x80148
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_REQ_INFO_PERMISSION_8 0x1BF80148u

//! Register Reset Value
#define TREG2_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_READ_PERMISSION_8 Register TREG2_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TREG2_PM_READ_PERMISSION_8 0x80150
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_READ_PERMISSION_8 0x1BF80150u

//! Register Reset Value
#define TREG2_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_WRITE_PERMISSION_8 Register TREG2_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TREG2_PM_WRITE_PERMISSION_8 0x80158
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_WRITE_PERMISSION_8 0x1BF80158u

//! Register Reset Value
#define TREG2_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_ADDR_MATCH_9 Register TREG2_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TREG2_PM_ADDR_MATCH_9 0x80160
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ADDR_MATCH_9 0x1BF80160u

//! Register Reset Value
#define TREG2_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG2_PM_REQ_INFO_PERMISSION_9 Register TREG2_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TREG2_PM_REQ_INFO_PERMISSION_9 0x80168
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_REQ_INFO_PERMISSION_9 0x1BF80168u

//! Register Reset Value
#define TREG2_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_READ_PERMISSION_9 Register TREG2_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TREG2_PM_READ_PERMISSION_9 0x80170
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_READ_PERMISSION_9 0x1BF80170u

//! Register Reset Value
#define TREG2_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_WRITE_PERMISSION_9 Register TREG2_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TREG2_PM_WRITE_PERMISSION_9 0x80178
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_WRITE_PERMISSION_9 0x1BF80178u

//! Register Reset Value
#define TREG2_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_ADDR_MATCH_10 Register TREG2_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TREG2_PM_ADDR_MATCH_10 0x80180
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ADDR_MATCH_10 0x1BF80180u

//! Register Reset Value
#define TREG2_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG2_PM_REQ_INFO_PERMISSION_10 Register TREG2_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TREG2_PM_REQ_INFO_PERMISSION_10 0x80188
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_REQ_INFO_PERMISSION_10 0x1BF80188u

//! Register Reset Value
#define TREG2_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_READ_PERMISSION_10 Register TREG2_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TREG2_PM_READ_PERMISSION_10 0x80190
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_READ_PERMISSION_10 0x1BF80190u

//! Register Reset Value
#define TREG2_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_WRITE_PERMISSION_10 Register TREG2_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TREG2_PM_WRITE_PERMISSION_10 0x80198
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_WRITE_PERMISSION_10 0x1BF80198u

//! Register Reset Value
#define TREG2_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_ADDR_MATCH_11 Register TREG2_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TREG2_PM_ADDR_MATCH_11 0x801A0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ADDR_MATCH_11 0x1BF801A0u

//! Register Reset Value
#define TREG2_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG2_PM_REQ_INFO_PERMISSION_11 Register TREG2_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TREG2_PM_REQ_INFO_PERMISSION_11 0x801A8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_REQ_INFO_PERMISSION_11 0x1BF801A8u

//! Register Reset Value
#define TREG2_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_READ_PERMISSION_11 Register TREG2_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TREG2_PM_READ_PERMISSION_11 0x801B0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_READ_PERMISSION_11 0x1BF801B0u

//! Register Reset Value
#define TREG2_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_WRITE_PERMISSION_11 Register TREG2_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TREG2_PM_WRITE_PERMISSION_11 0x801B8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_WRITE_PERMISSION_11 0x1BF801B8u

//! Register Reset Value
#define TREG2_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_ADDR_MATCH_12 Register TREG2_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TREG2_PM_ADDR_MATCH_12 0x801C0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ADDR_MATCH_12 0x1BF801C0u

//! Register Reset Value
#define TREG2_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG2_PM_REQ_INFO_PERMISSION_12 Register TREG2_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TREG2_PM_REQ_INFO_PERMISSION_12 0x801C8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_REQ_INFO_PERMISSION_12 0x1BF801C8u

//! Register Reset Value
#define TREG2_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_READ_PERMISSION_12 Register TREG2_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TREG2_PM_READ_PERMISSION_12 0x801D0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_READ_PERMISSION_12 0x1BF801D0u

//! Register Reset Value
#define TREG2_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_WRITE_PERMISSION_12 Register TREG2_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TREG2_PM_WRITE_PERMISSION_12 0x801D8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_WRITE_PERMISSION_12 0x1BF801D8u

//! Register Reset Value
#define TREG2_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_ADDR_MATCH_13 Register TREG2_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TREG2_PM_ADDR_MATCH_13 0x801E0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ADDR_MATCH_13 0x1BF801E0u

//! Register Reset Value
#define TREG2_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG2_PM_REQ_INFO_PERMISSION_13 Register TREG2_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TREG2_PM_REQ_INFO_PERMISSION_13 0x801E8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_REQ_INFO_PERMISSION_13 0x1BF801E8u

//! Register Reset Value
#define TREG2_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_READ_PERMISSION_13 Register TREG2_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TREG2_PM_READ_PERMISSION_13 0x801F0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_READ_PERMISSION_13 0x1BF801F0u

//! Register Reset Value
#define TREG2_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_WRITE_PERMISSION_13 Register TREG2_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TREG2_PM_WRITE_PERMISSION_13 0x801F8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_WRITE_PERMISSION_13 0x1BF801F8u

//! Register Reset Value
#define TREG2_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_ADDR_MATCH_14 Register TREG2_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TREG2_PM_ADDR_MATCH_14 0x80200
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ADDR_MATCH_14 0x1BF80200u

//! Register Reset Value
#define TREG2_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG2_PM_REQ_INFO_PERMISSION_14 Register TREG2_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TREG2_PM_REQ_INFO_PERMISSION_14 0x80208
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_REQ_INFO_PERMISSION_14 0x1BF80208u

//! Register Reset Value
#define TREG2_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_READ_PERMISSION_14 Register TREG2_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TREG2_PM_READ_PERMISSION_14 0x80210
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_READ_PERMISSION_14 0x1BF80210u

//! Register Reset Value
#define TREG2_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_WRITE_PERMISSION_14 Register TREG2_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TREG2_PM_WRITE_PERMISSION_14 0x80218
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_WRITE_PERMISSION_14 0x1BF80218u

//! Register Reset Value
#define TREG2_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_ADDR_MATCH_15 Register TREG2_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TREG2_PM_ADDR_MATCH_15 0x80220
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_ADDR_MATCH_15 0x1BF80220u

//! Register Reset Value
#define TREG2_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TREG2_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TREG2_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TREG2_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TREG2_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TREG2_PM_REQ_INFO_PERMISSION_15 Register TREG2_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TREG2_PM_REQ_INFO_PERMISSION_15 0x80228
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_REQ_INFO_PERMISSION_15 0x1BF80228u

//! Register Reset Value
#define TREG2_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TREG2_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_READ_PERMISSION_15 Register TREG2_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TREG2_PM_READ_PERMISSION_15 0x80230
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_READ_PERMISSION_15 0x1BF80230u

//! Register Reset Value
#define TREG2_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TREG2_PM_WRITE_PERMISSION_15 Register TREG2_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TREG2_PM_WRITE_PERMISSION_15 0x80238
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TREG2_PM_WRITE_PERMISSION_15 0x1BF80238u

//! Register Reset Value
#define TREG2_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TREG2_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_ERROR_LOG Register TUS0_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TUS0_PM_ERROR_LOG 0x80420
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ERROR_LOG 0x1BF80420u

//! Register Reset Value
#define TUS0_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TUS0_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TUS0_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TUS0_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TUS0_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TUS0_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TUS0_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TUS0_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TUS0_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TUS0_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TUS0_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TUS0_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TUS0_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TUS0_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TUS0_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TUS0_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TUS0_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TUS0_PM_CONTROL Register TUS0_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TUS0_PM_CONTROL 0x80428
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_CONTROL 0x1BF80428u

//! Register Reset Value
#define TUS0_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TUS0_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TUS0_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TUS0_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TUS0_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TUS0_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TUS0_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TUS0_PM_ERROR_CLEAR_SINGLE Register TUS0_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TUS0_PM_ERROR_CLEAR_SINGLE 0x80430
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ERROR_CLEAR_SINGLE 0x1BF80430u

//! Register Reset Value
#define TUS0_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TUS0_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TUS0_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TUS0_PM_ERROR_CLEAR_MULTI Register TUS0_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TUS0_PM_ERROR_CLEAR_MULTI 0x80438
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ERROR_CLEAR_MULTI 0x1BF80438u

//! Register Reset Value
#define TUS0_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TUS0_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TUS0_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TUS0_PM_REQ_INFO_PERMISSION_0 Register TUS0_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TUS0_PM_REQ_INFO_PERMISSION_0 0x80448
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_REQ_INFO_PERMISSION_0 0x1BF80448u

//! Register Reset Value
#define TUS0_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_READ_PERMISSION_0 Register TUS0_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TUS0_PM_READ_PERMISSION_0 0x80450
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_READ_PERMISSION_0 0x1BF80450u

//! Register Reset Value
#define TUS0_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TUS0_PM_WRITE_PERMISSION_0 Register TUS0_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TUS0_PM_WRITE_PERMISSION_0 0x80458
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_WRITE_PERMISSION_0 0x1BF80458u

//! Register Reset Value
#define TUS0_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TUS0_PM_ADDR_MATCH_1 Register TUS0_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TUS0_PM_ADDR_MATCH_1 0x80460
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ADDR_MATCH_1 0x1BF80460u

//! Register Reset Value
#define TUS0_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0_PM_REQ_INFO_PERMISSION_1 Register TUS0_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TUS0_PM_REQ_INFO_PERMISSION_1 0x80468
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_REQ_INFO_PERMISSION_1 0x1BF80468u

//! Register Reset Value
#define TUS0_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_READ_PERMISSION_1 Register TUS0_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TUS0_PM_READ_PERMISSION_1 0x80470
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_READ_PERMISSION_1 0x1BF80470u

//! Register Reset Value
#define TUS0_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_WRITE_PERMISSION_1 Register TUS0_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TUS0_PM_WRITE_PERMISSION_1 0x80478
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_WRITE_PERMISSION_1 0x1BF80478u

//! Register Reset Value
#define TUS0_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_ADDR_MATCH_2 Register TUS0_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TUS0_PM_ADDR_MATCH_2 0x80480
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ADDR_MATCH_2 0x1BF80480u

//! Register Reset Value
#define TUS0_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0_PM_REQ_INFO_PERMISSION_2 Register TUS0_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TUS0_PM_REQ_INFO_PERMISSION_2 0x80488
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_REQ_INFO_PERMISSION_2 0x1BF80488u

//! Register Reset Value
#define TUS0_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_READ_PERMISSION_2 Register TUS0_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TUS0_PM_READ_PERMISSION_2 0x80490
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_READ_PERMISSION_2 0x1BF80490u

//! Register Reset Value
#define TUS0_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_WRITE_PERMISSION_2 Register TUS0_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TUS0_PM_WRITE_PERMISSION_2 0x80498
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_WRITE_PERMISSION_2 0x1BF80498u

//! Register Reset Value
#define TUS0_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_ADDR_MATCH_3 Register TUS0_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TUS0_PM_ADDR_MATCH_3 0x804A0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ADDR_MATCH_3 0x1BF804A0u

//! Register Reset Value
#define TUS0_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0_PM_REQ_INFO_PERMISSION_3 Register TUS0_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TUS0_PM_REQ_INFO_PERMISSION_3 0x804A8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_REQ_INFO_PERMISSION_3 0x1BF804A8u

//! Register Reset Value
#define TUS0_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_READ_PERMISSION_3 Register TUS0_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TUS0_PM_READ_PERMISSION_3 0x804B0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_READ_PERMISSION_3 0x1BF804B0u

//! Register Reset Value
#define TUS0_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_WRITE_PERMISSION_3 Register TUS0_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TUS0_PM_WRITE_PERMISSION_3 0x804B8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_WRITE_PERMISSION_3 0x1BF804B8u

//! Register Reset Value
#define TUS0_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_ADDR_MATCH_4 Register TUS0_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TUS0_PM_ADDR_MATCH_4 0x804C0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ADDR_MATCH_4 0x1BF804C0u

//! Register Reset Value
#define TUS0_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0_PM_REQ_INFO_PERMISSION_4 Register TUS0_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TUS0_PM_REQ_INFO_PERMISSION_4 0x804C8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_REQ_INFO_PERMISSION_4 0x1BF804C8u

//! Register Reset Value
#define TUS0_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_READ_PERMISSION_4 Register TUS0_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TUS0_PM_READ_PERMISSION_4 0x804D0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_READ_PERMISSION_4 0x1BF804D0u

//! Register Reset Value
#define TUS0_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_WRITE_PERMISSION_4 Register TUS0_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TUS0_PM_WRITE_PERMISSION_4 0x804D8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_WRITE_PERMISSION_4 0x1BF804D8u

//! Register Reset Value
#define TUS0_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_ADDR_MATCH_5 Register TUS0_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TUS0_PM_ADDR_MATCH_5 0x804E0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ADDR_MATCH_5 0x1BF804E0u

//! Register Reset Value
#define TUS0_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0_PM_REQ_INFO_PERMISSION_5 Register TUS0_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TUS0_PM_REQ_INFO_PERMISSION_5 0x804E8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_REQ_INFO_PERMISSION_5 0x1BF804E8u

//! Register Reset Value
#define TUS0_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_READ_PERMISSION_5 Register TUS0_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TUS0_PM_READ_PERMISSION_5 0x804F0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_READ_PERMISSION_5 0x1BF804F0u

//! Register Reset Value
#define TUS0_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_WRITE_PERMISSION_5 Register TUS0_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TUS0_PM_WRITE_PERMISSION_5 0x804F8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_WRITE_PERMISSION_5 0x1BF804F8u

//! Register Reset Value
#define TUS0_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_ADDR_MATCH_6 Register TUS0_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TUS0_PM_ADDR_MATCH_6 0x80500
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ADDR_MATCH_6 0x1BF80500u

//! Register Reset Value
#define TUS0_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0_PM_REQ_INFO_PERMISSION_6 Register TUS0_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TUS0_PM_REQ_INFO_PERMISSION_6 0x80508
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_REQ_INFO_PERMISSION_6 0x1BF80508u

//! Register Reset Value
#define TUS0_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_READ_PERMISSION_6 Register TUS0_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TUS0_PM_READ_PERMISSION_6 0x80510
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_READ_PERMISSION_6 0x1BF80510u

//! Register Reset Value
#define TUS0_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_WRITE_PERMISSION_6 Register TUS0_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TUS0_PM_WRITE_PERMISSION_6 0x80518
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_WRITE_PERMISSION_6 0x1BF80518u

//! Register Reset Value
#define TUS0_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_ADDR_MATCH_7 Register TUS0_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TUS0_PM_ADDR_MATCH_7 0x80520
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ADDR_MATCH_7 0x1BF80520u

//! Register Reset Value
#define TUS0_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0_PM_REQ_INFO_PERMISSION_7 Register TUS0_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TUS0_PM_REQ_INFO_PERMISSION_7 0x80528
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_REQ_INFO_PERMISSION_7 0x1BF80528u

//! Register Reset Value
#define TUS0_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_READ_PERMISSION_7 Register TUS0_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TUS0_PM_READ_PERMISSION_7 0x80530
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_READ_PERMISSION_7 0x1BF80530u

//! Register Reset Value
#define TUS0_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_WRITE_PERMISSION_7 Register TUS0_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TUS0_PM_WRITE_PERMISSION_7 0x80538
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_WRITE_PERMISSION_7 0x1BF80538u

//! Register Reset Value
#define TUS0_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_ADDR_MATCH_8 Register TUS0_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TUS0_PM_ADDR_MATCH_8 0x80540
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ADDR_MATCH_8 0x1BF80540u

//! Register Reset Value
#define TUS0_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0_PM_REQ_INFO_PERMISSION_8 Register TUS0_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TUS0_PM_REQ_INFO_PERMISSION_8 0x80548
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_REQ_INFO_PERMISSION_8 0x1BF80548u

//! Register Reset Value
#define TUS0_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_READ_PERMISSION_8 Register TUS0_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TUS0_PM_READ_PERMISSION_8 0x80550
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_READ_PERMISSION_8 0x1BF80550u

//! Register Reset Value
#define TUS0_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_WRITE_PERMISSION_8 Register TUS0_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TUS0_PM_WRITE_PERMISSION_8 0x80558
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_WRITE_PERMISSION_8 0x1BF80558u

//! Register Reset Value
#define TUS0_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_ADDR_MATCH_9 Register TUS0_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TUS0_PM_ADDR_MATCH_9 0x80560
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ADDR_MATCH_9 0x1BF80560u

//! Register Reset Value
#define TUS0_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0_PM_REQ_INFO_PERMISSION_9 Register TUS0_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TUS0_PM_REQ_INFO_PERMISSION_9 0x80568
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_REQ_INFO_PERMISSION_9 0x1BF80568u

//! Register Reset Value
#define TUS0_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_READ_PERMISSION_9 Register TUS0_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TUS0_PM_READ_PERMISSION_9 0x80570
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_READ_PERMISSION_9 0x1BF80570u

//! Register Reset Value
#define TUS0_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_WRITE_PERMISSION_9 Register TUS0_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TUS0_PM_WRITE_PERMISSION_9 0x80578
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_WRITE_PERMISSION_9 0x1BF80578u

//! Register Reset Value
#define TUS0_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_ADDR_MATCH_10 Register TUS0_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TUS0_PM_ADDR_MATCH_10 0x80580
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ADDR_MATCH_10 0x1BF80580u

//! Register Reset Value
#define TUS0_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0_PM_REQ_INFO_PERMISSION_10 Register TUS0_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TUS0_PM_REQ_INFO_PERMISSION_10 0x80588
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_REQ_INFO_PERMISSION_10 0x1BF80588u

//! Register Reset Value
#define TUS0_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_READ_PERMISSION_10 Register TUS0_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TUS0_PM_READ_PERMISSION_10 0x80590
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_READ_PERMISSION_10 0x1BF80590u

//! Register Reset Value
#define TUS0_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_WRITE_PERMISSION_10 Register TUS0_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TUS0_PM_WRITE_PERMISSION_10 0x80598
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_WRITE_PERMISSION_10 0x1BF80598u

//! Register Reset Value
#define TUS0_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_ADDR_MATCH_11 Register TUS0_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TUS0_PM_ADDR_MATCH_11 0x805A0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ADDR_MATCH_11 0x1BF805A0u

//! Register Reset Value
#define TUS0_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0_PM_REQ_INFO_PERMISSION_11 Register TUS0_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TUS0_PM_REQ_INFO_PERMISSION_11 0x805A8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_REQ_INFO_PERMISSION_11 0x1BF805A8u

//! Register Reset Value
#define TUS0_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_READ_PERMISSION_11 Register TUS0_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TUS0_PM_READ_PERMISSION_11 0x805B0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_READ_PERMISSION_11 0x1BF805B0u

//! Register Reset Value
#define TUS0_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_WRITE_PERMISSION_11 Register TUS0_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TUS0_PM_WRITE_PERMISSION_11 0x805B8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_WRITE_PERMISSION_11 0x1BF805B8u

//! Register Reset Value
#define TUS0_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_ADDR_MATCH_12 Register TUS0_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TUS0_PM_ADDR_MATCH_12 0x805C0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ADDR_MATCH_12 0x1BF805C0u

//! Register Reset Value
#define TUS0_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0_PM_REQ_INFO_PERMISSION_12 Register TUS0_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TUS0_PM_REQ_INFO_PERMISSION_12 0x805C8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_REQ_INFO_PERMISSION_12 0x1BF805C8u

//! Register Reset Value
#define TUS0_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_READ_PERMISSION_12 Register TUS0_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TUS0_PM_READ_PERMISSION_12 0x805D0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_READ_PERMISSION_12 0x1BF805D0u

//! Register Reset Value
#define TUS0_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_WRITE_PERMISSION_12 Register TUS0_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TUS0_PM_WRITE_PERMISSION_12 0x805D8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_WRITE_PERMISSION_12 0x1BF805D8u

//! Register Reset Value
#define TUS0_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_ADDR_MATCH_13 Register TUS0_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TUS0_PM_ADDR_MATCH_13 0x805E0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ADDR_MATCH_13 0x1BF805E0u

//! Register Reset Value
#define TUS0_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0_PM_REQ_INFO_PERMISSION_13 Register TUS0_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TUS0_PM_REQ_INFO_PERMISSION_13 0x805E8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_REQ_INFO_PERMISSION_13 0x1BF805E8u

//! Register Reset Value
#define TUS0_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_READ_PERMISSION_13 Register TUS0_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TUS0_PM_READ_PERMISSION_13 0x805F0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_READ_PERMISSION_13 0x1BF805F0u

//! Register Reset Value
#define TUS0_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_WRITE_PERMISSION_13 Register TUS0_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TUS0_PM_WRITE_PERMISSION_13 0x805F8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_WRITE_PERMISSION_13 0x1BF805F8u

//! Register Reset Value
#define TUS0_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_ADDR_MATCH_14 Register TUS0_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TUS0_PM_ADDR_MATCH_14 0x80600
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ADDR_MATCH_14 0x1BF80600u

//! Register Reset Value
#define TUS0_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0_PM_REQ_INFO_PERMISSION_14 Register TUS0_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TUS0_PM_REQ_INFO_PERMISSION_14 0x80608
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_REQ_INFO_PERMISSION_14 0x1BF80608u

//! Register Reset Value
#define TUS0_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_READ_PERMISSION_14 Register TUS0_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TUS0_PM_READ_PERMISSION_14 0x80610
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_READ_PERMISSION_14 0x1BF80610u

//! Register Reset Value
#define TUS0_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_WRITE_PERMISSION_14 Register TUS0_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TUS0_PM_WRITE_PERMISSION_14 0x80618
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_WRITE_PERMISSION_14 0x1BF80618u

//! Register Reset Value
#define TUS0_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_ADDR_MATCH_15 Register TUS0_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TUS0_PM_ADDR_MATCH_15 0x80620
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_ADDR_MATCH_15 0x1BF80620u

//! Register Reset Value
#define TUS0_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TUS0_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0_PM_REQ_INFO_PERMISSION_15 Register TUS0_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TUS0_PM_REQ_INFO_PERMISSION_15 0x80628
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_REQ_INFO_PERMISSION_15 0x1BF80628u

//! Register Reset Value
#define TUS0_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_READ_PERMISSION_15 Register TUS0_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TUS0_PM_READ_PERMISSION_15 0x80630
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_READ_PERMISSION_15 0x1BF80630u

//! Register Reset Value
#define TUS0_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0_PM_WRITE_PERMISSION_15 Register TUS0_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TUS0_PM_WRITE_PERMISSION_15 0x80638
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0_PM_WRITE_PERMISSION_15 0x1BF80638u

//! Register Reset Value
#define TUS0_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_ERROR_LOG Register TUS0C_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ERROR_LOG 0x80820
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ERROR_LOG 0x1BF80820u

//! Register Reset Value
#define TUS0C_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TUS0C_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TUS0C_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TUS0C_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TUS0C_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TUS0C_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TUS0C_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TUS0C_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TUS0C_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TUS0C_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TUS0C_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TUS0C_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TUS0C_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TUS0C_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TUS0C_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TUS0C_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TUS0C_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TUS0C_PM_CONTROL Register TUS0C_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TUS0C_PM_CONTROL 0x80828
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_CONTROL 0x1BF80828u

//! Register Reset Value
#define TUS0C_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TUS0C_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TUS0C_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TUS0C_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TUS0C_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TUS0C_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TUS0C_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TUS0C_PM_ERROR_CLEAR_SINGLE Register TUS0C_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ERROR_CLEAR_SINGLE 0x80830
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ERROR_CLEAR_SINGLE 0x1BF80830u

//! Register Reset Value
#define TUS0C_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TUS0C_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TUS0C_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TUS0C_PM_ERROR_CLEAR_MULTI Register TUS0C_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ERROR_CLEAR_MULTI 0x80838
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ERROR_CLEAR_MULTI 0x1BF80838u

//! Register Reset Value
#define TUS0C_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TUS0C_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TUS0C_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TUS0C_PM_REQ_INFO_PERMISSION_0 Register TUS0C_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TUS0C_PM_REQ_INFO_PERMISSION_0 0x80848
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_REQ_INFO_PERMISSION_0 0x1BF80848u

//! Register Reset Value
#define TUS0C_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_READ_PERMISSION_0 Register TUS0C_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TUS0C_PM_READ_PERMISSION_0 0x80850
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_READ_PERMISSION_0 0x1BF80850u

//! Register Reset Value
#define TUS0C_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TUS0C_PM_WRITE_PERMISSION_0 Register TUS0C_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TUS0C_PM_WRITE_PERMISSION_0 0x80858
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_WRITE_PERMISSION_0 0x1BF80858u

//! Register Reset Value
#define TUS0C_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TUS0C_PM_ADDR_MATCH_1 Register TUS0C_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ADDR_MATCH_1 0x80860
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ADDR_MATCH_1 0x1BF80860u

//! Register Reset Value
#define TUS0C_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0C_PM_REQ_INFO_PERMISSION_1 Register TUS0C_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TUS0C_PM_REQ_INFO_PERMISSION_1 0x80868
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_REQ_INFO_PERMISSION_1 0x1BF80868u

//! Register Reset Value
#define TUS0C_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_READ_PERMISSION_1 Register TUS0C_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TUS0C_PM_READ_PERMISSION_1 0x80870
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_READ_PERMISSION_1 0x1BF80870u

//! Register Reset Value
#define TUS0C_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_WRITE_PERMISSION_1 Register TUS0C_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TUS0C_PM_WRITE_PERMISSION_1 0x80878
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_WRITE_PERMISSION_1 0x1BF80878u

//! Register Reset Value
#define TUS0C_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_ADDR_MATCH_2 Register TUS0C_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ADDR_MATCH_2 0x80880
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ADDR_MATCH_2 0x1BF80880u

//! Register Reset Value
#define TUS0C_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0C_PM_REQ_INFO_PERMISSION_2 Register TUS0C_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TUS0C_PM_REQ_INFO_PERMISSION_2 0x80888
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_REQ_INFO_PERMISSION_2 0x1BF80888u

//! Register Reset Value
#define TUS0C_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_READ_PERMISSION_2 Register TUS0C_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TUS0C_PM_READ_PERMISSION_2 0x80890
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_READ_PERMISSION_2 0x1BF80890u

//! Register Reset Value
#define TUS0C_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_WRITE_PERMISSION_2 Register TUS0C_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TUS0C_PM_WRITE_PERMISSION_2 0x80898
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_WRITE_PERMISSION_2 0x1BF80898u

//! Register Reset Value
#define TUS0C_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_ADDR_MATCH_3 Register TUS0C_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ADDR_MATCH_3 0x808A0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ADDR_MATCH_3 0x1BF808A0u

//! Register Reset Value
#define TUS0C_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0C_PM_REQ_INFO_PERMISSION_3 Register TUS0C_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TUS0C_PM_REQ_INFO_PERMISSION_3 0x808A8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_REQ_INFO_PERMISSION_3 0x1BF808A8u

//! Register Reset Value
#define TUS0C_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_READ_PERMISSION_3 Register TUS0C_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TUS0C_PM_READ_PERMISSION_3 0x808B0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_READ_PERMISSION_3 0x1BF808B0u

//! Register Reset Value
#define TUS0C_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_WRITE_PERMISSION_3 Register TUS0C_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TUS0C_PM_WRITE_PERMISSION_3 0x808B8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_WRITE_PERMISSION_3 0x1BF808B8u

//! Register Reset Value
#define TUS0C_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_ADDR_MATCH_4 Register TUS0C_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ADDR_MATCH_4 0x808C0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ADDR_MATCH_4 0x1BF808C0u

//! Register Reset Value
#define TUS0C_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0C_PM_REQ_INFO_PERMISSION_4 Register TUS0C_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TUS0C_PM_REQ_INFO_PERMISSION_4 0x808C8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_REQ_INFO_PERMISSION_4 0x1BF808C8u

//! Register Reset Value
#define TUS0C_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_READ_PERMISSION_4 Register TUS0C_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TUS0C_PM_READ_PERMISSION_4 0x808D0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_READ_PERMISSION_4 0x1BF808D0u

//! Register Reset Value
#define TUS0C_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_WRITE_PERMISSION_4 Register TUS0C_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TUS0C_PM_WRITE_PERMISSION_4 0x808D8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_WRITE_PERMISSION_4 0x1BF808D8u

//! Register Reset Value
#define TUS0C_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_ADDR_MATCH_5 Register TUS0C_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ADDR_MATCH_5 0x808E0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ADDR_MATCH_5 0x1BF808E0u

//! Register Reset Value
#define TUS0C_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0C_PM_REQ_INFO_PERMISSION_5 Register TUS0C_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TUS0C_PM_REQ_INFO_PERMISSION_5 0x808E8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_REQ_INFO_PERMISSION_5 0x1BF808E8u

//! Register Reset Value
#define TUS0C_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_READ_PERMISSION_5 Register TUS0C_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TUS0C_PM_READ_PERMISSION_5 0x808F0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_READ_PERMISSION_5 0x1BF808F0u

//! Register Reset Value
#define TUS0C_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_WRITE_PERMISSION_5 Register TUS0C_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TUS0C_PM_WRITE_PERMISSION_5 0x808F8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_WRITE_PERMISSION_5 0x1BF808F8u

//! Register Reset Value
#define TUS0C_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_ADDR_MATCH_6 Register TUS0C_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ADDR_MATCH_6 0x80900
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ADDR_MATCH_6 0x1BF80900u

//! Register Reset Value
#define TUS0C_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0C_PM_REQ_INFO_PERMISSION_6 Register TUS0C_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TUS0C_PM_REQ_INFO_PERMISSION_6 0x80908
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_REQ_INFO_PERMISSION_6 0x1BF80908u

//! Register Reset Value
#define TUS0C_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_READ_PERMISSION_6 Register TUS0C_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TUS0C_PM_READ_PERMISSION_6 0x80910
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_READ_PERMISSION_6 0x1BF80910u

//! Register Reset Value
#define TUS0C_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_WRITE_PERMISSION_6 Register TUS0C_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TUS0C_PM_WRITE_PERMISSION_6 0x80918
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_WRITE_PERMISSION_6 0x1BF80918u

//! Register Reset Value
#define TUS0C_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_ADDR_MATCH_7 Register TUS0C_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ADDR_MATCH_7 0x80920
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ADDR_MATCH_7 0x1BF80920u

//! Register Reset Value
#define TUS0C_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0C_PM_REQ_INFO_PERMISSION_7 Register TUS0C_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TUS0C_PM_REQ_INFO_PERMISSION_7 0x80928
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_REQ_INFO_PERMISSION_7 0x1BF80928u

//! Register Reset Value
#define TUS0C_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_READ_PERMISSION_7 Register TUS0C_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TUS0C_PM_READ_PERMISSION_7 0x80930
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_READ_PERMISSION_7 0x1BF80930u

//! Register Reset Value
#define TUS0C_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_WRITE_PERMISSION_7 Register TUS0C_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TUS0C_PM_WRITE_PERMISSION_7 0x80938
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_WRITE_PERMISSION_7 0x1BF80938u

//! Register Reset Value
#define TUS0C_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_ADDR_MATCH_8 Register TUS0C_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ADDR_MATCH_8 0x80940
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ADDR_MATCH_8 0x1BF80940u

//! Register Reset Value
#define TUS0C_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0C_PM_REQ_INFO_PERMISSION_8 Register TUS0C_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TUS0C_PM_REQ_INFO_PERMISSION_8 0x80948
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_REQ_INFO_PERMISSION_8 0x1BF80948u

//! Register Reset Value
#define TUS0C_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_READ_PERMISSION_8 Register TUS0C_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TUS0C_PM_READ_PERMISSION_8 0x80950
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_READ_PERMISSION_8 0x1BF80950u

//! Register Reset Value
#define TUS0C_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_WRITE_PERMISSION_8 Register TUS0C_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TUS0C_PM_WRITE_PERMISSION_8 0x80958
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_WRITE_PERMISSION_8 0x1BF80958u

//! Register Reset Value
#define TUS0C_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_ADDR_MATCH_9 Register TUS0C_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ADDR_MATCH_9 0x80960
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ADDR_MATCH_9 0x1BF80960u

//! Register Reset Value
#define TUS0C_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0C_PM_REQ_INFO_PERMISSION_9 Register TUS0C_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TUS0C_PM_REQ_INFO_PERMISSION_9 0x80968
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_REQ_INFO_PERMISSION_9 0x1BF80968u

//! Register Reset Value
#define TUS0C_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_READ_PERMISSION_9 Register TUS0C_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TUS0C_PM_READ_PERMISSION_9 0x80970
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_READ_PERMISSION_9 0x1BF80970u

//! Register Reset Value
#define TUS0C_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_WRITE_PERMISSION_9 Register TUS0C_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TUS0C_PM_WRITE_PERMISSION_9 0x80978
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_WRITE_PERMISSION_9 0x1BF80978u

//! Register Reset Value
#define TUS0C_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_ADDR_MATCH_10 Register TUS0C_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ADDR_MATCH_10 0x80980
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ADDR_MATCH_10 0x1BF80980u

//! Register Reset Value
#define TUS0C_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0C_PM_REQ_INFO_PERMISSION_10 Register TUS0C_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TUS0C_PM_REQ_INFO_PERMISSION_10 0x80988
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_REQ_INFO_PERMISSION_10 0x1BF80988u

//! Register Reset Value
#define TUS0C_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_READ_PERMISSION_10 Register TUS0C_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TUS0C_PM_READ_PERMISSION_10 0x80990
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_READ_PERMISSION_10 0x1BF80990u

//! Register Reset Value
#define TUS0C_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_WRITE_PERMISSION_10 Register TUS0C_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TUS0C_PM_WRITE_PERMISSION_10 0x80998
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_WRITE_PERMISSION_10 0x1BF80998u

//! Register Reset Value
#define TUS0C_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_ADDR_MATCH_11 Register TUS0C_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ADDR_MATCH_11 0x809A0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ADDR_MATCH_11 0x1BF809A0u

//! Register Reset Value
#define TUS0C_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0C_PM_REQ_INFO_PERMISSION_11 Register TUS0C_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TUS0C_PM_REQ_INFO_PERMISSION_11 0x809A8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_REQ_INFO_PERMISSION_11 0x1BF809A8u

//! Register Reset Value
#define TUS0C_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_READ_PERMISSION_11 Register TUS0C_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TUS0C_PM_READ_PERMISSION_11 0x809B0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_READ_PERMISSION_11 0x1BF809B0u

//! Register Reset Value
#define TUS0C_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_WRITE_PERMISSION_11 Register TUS0C_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TUS0C_PM_WRITE_PERMISSION_11 0x809B8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_WRITE_PERMISSION_11 0x1BF809B8u

//! Register Reset Value
#define TUS0C_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_ADDR_MATCH_12 Register TUS0C_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ADDR_MATCH_12 0x809C0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ADDR_MATCH_12 0x1BF809C0u

//! Register Reset Value
#define TUS0C_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0C_PM_REQ_INFO_PERMISSION_12 Register TUS0C_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TUS0C_PM_REQ_INFO_PERMISSION_12 0x809C8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_REQ_INFO_PERMISSION_12 0x1BF809C8u

//! Register Reset Value
#define TUS0C_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_READ_PERMISSION_12 Register TUS0C_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TUS0C_PM_READ_PERMISSION_12 0x809D0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_READ_PERMISSION_12 0x1BF809D0u

//! Register Reset Value
#define TUS0C_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_WRITE_PERMISSION_12 Register TUS0C_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TUS0C_PM_WRITE_PERMISSION_12 0x809D8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_WRITE_PERMISSION_12 0x1BF809D8u

//! Register Reset Value
#define TUS0C_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_ADDR_MATCH_13 Register TUS0C_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ADDR_MATCH_13 0x809E0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ADDR_MATCH_13 0x1BF809E0u

//! Register Reset Value
#define TUS0C_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0C_PM_REQ_INFO_PERMISSION_13 Register TUS0C_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TUS0C_PM_REQ_INFO_PERMISSION_13 0x809E8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_REQ_INFO_PERMISSION_13 0x1BF809E8u

//! Register Reset Value
#define TUS0C_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_READ_PERMISSION_13 Register TUS0C_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TUS0C_PM_READ_PERMISSION_13 0x809F0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_READ_PERMISSION_13 0x1BF809F0u

//! Register Reset Value
#define TUS0C_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_WRITE_PERMISSION_13 Register TUS0C_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TUS0C_PM_WRITE_PERMISSION_13 0x809F8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_WRITE_PERMISSION_13 0x1BF809F8u

//! Register Reset Value
#define TUS0C_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_ADDR_MATCH_14 Register TUS0C_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ADDR_MATCH_14 0x80A00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ADDR_MATCH_14 0x1BF80A00u

//! Register Reset Value
#define TUS0C_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0C_PM_REQ_INFO_PERMISSION_14 Register TUS0C_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TUS0C_PM_REQ_INFO_PERMISSION_14 0x80A08
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_REQ_INFO_PERMISSION_14 0x1BF80A08u

//! Register Reset Value
#define TUS0C_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_READ_PERMISSION_14 Register TUS0C_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TUS0C_PM_READ_PERMISSION_14 0x80A10
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_READ_PERMISSION_14 0x1BF80A10u

//! Register Reset Value
#define TUS0C_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_WRITE_PERMISSION_14 Register TUS0C_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TUS0C_PM_WRITE_PERMISSION_14 0x80A18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_WRITE_PERMISSION_14 0x1BF80A18u

//! Register Reset Value
#define TUS0C_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_ADDR_MATCH_15 Register TUS0C_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TUS0C_PM_ADDR_MATCH_15 0x80A20
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_ADDR_MATCH_15 0x1BF80A20u

//! Register Reset Value
#define TUS0C_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS0C_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TUS0C_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TUS0C_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS0C_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS0C_PM_REQ_INFO_PERMISSION_15 Register TUS0C_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TUS0C_PM_REQ_INFO_PERMISSION_15 0x80A28
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_REQ_INFO_PERMISSION_15 0x1BF80A28u

//! Register Reset Value
#define TUS0C_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS0C_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_READ_PERMISSION_15 Register TUS0C_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TUS0C_PM_READ_PERMISSION_15 0x80A30
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_READ_PERMISSION_15 0x1BF80A30u

//! Register Reset Value
#define TUS0C_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS0C_PM_WRITE_PERMISSION_15 Register TUS0C_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TUS0C_PM_WRITE_PERMISSION_15 0x80A38
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS0C_PM_WRITE_PERMISSION_15 0x1BF80A38u

//! Register Reset Value
#define TUS0C_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS0C_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_ERROR_LOG Register TUS1_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TUS1_PM_ERROR_LOG 0x80C20
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ERROR_LOG 0x1BF80C20u

//! Register Reset Value
#define TUS1_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TUS1_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TUS1_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TUS1_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TUS1_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TUS1_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TUS1_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TUS1_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TUS1_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TUS1_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TUS1_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TUS1_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TUS1_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TUS1_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TUS1_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TUS1_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TUS1_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TUS1_PM_CONTROL Register TUS1_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TUS1_PM_CONTROL 0x80C28
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_CONTROL 0x1BF80C28u

//! Register Reset Value
#define TUS1_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TUS1_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TUS1_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TUS1_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TUS1_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TUS1_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TUS1_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TUS1_PM_ERROR_CLEAR_SINGLE Register TUS1_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TUS1_PM_ERROR_CLEAR_SINGLE 0x80C30
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ERROR_CLEAR_SINGLE 0x1BF80C30u

//! Register Reset Value
#define TUS1_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TUS1_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TUS1_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TUS1_PM_ERROR_CLEAR_MULTI Register TUS1_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TUS1_PM_ERROR_CLEAR_MULTI 0x80C38
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ERROR_CLEAR_MULTI 0x1BF80C38u

//! Register Reset Value
#define TUS1_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TUS1_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TUS1_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TUS1_PM_REQ_INFO_PERMISSION_0 Register TUS1_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TUS1_PM_REQ_INFO_PERMISSION_0 0x80C48
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_REQ_INFO_PERMISSION_0 0x1BF80C48u

//! Register Reset Value
#define TUS1_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_READ_PERMISSION_0 Register TUS1_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TUS1_PM_READ_PERMISSION_0 0x80C50
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_READ_PERMISSION_0 0x1BF80C50u

//! Register Reset Value
#define TUS1_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TUS1_PM_WRITE_PERMISSION_0 Register TUS1_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TUS1_PM_WRITE_PERMISSION_0 0x80C58
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_WRITE_PERMISSION_0 0x1BF80C58u

//! Register Reset Value
#define TUS1_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TUS1_PM_ADDR_MATCH_1 Register TUS1_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TUS1_PM_ADDR_MATCH_1 0x80C60
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ADDR_MATCH_1 0x1BF80C60u

//! Register Reset Value
#define TUS1_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1_PM_REQ_INFO_PERMISSION_1 Register TUS1_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TUS1_PM_REQ_INFO_PERMISSION_1 0x80C68
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_REQ_INFO_PERMISSION_1 0x1BF80C68u

//! Register Reset Value
#define TUS1_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_READ_PERMISSION_1 Register TUS1_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TUS1_PM_READ_PERMISSION_1 0x80C70
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_READ_PERMISSION_1 0x1BF80C70u

//! Register Reset Value
#define TUS1_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_WRITE_PERMISSION_1 Register TUS1_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TUS1_PM_WRITE_PERMISSION_1 0x80C78
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_WRITE_PERMISSION_1 0x1BF80C78u

//! Register Reset Value
#define TUS1_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_ADDR_MATCH_2 Register TUS1_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TUS1_PM_ADDR_MATCH_2 0x80C80
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ADDR_MATCH_2 0x1BF80C80u

//! Register Reset Value
#define TUS1_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1_PM_REQ_INFO_PERMISSION_2 Register TUS1_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TUS1_PM_REQ_INFO_PERMISSION_2 0x80C88
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_REQ_INFO_PERMISSION_2 0x1BF80C88u

//! Register Reset Value
#define TUS1_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_READ_PERMISSION_2 Register TUS1_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TUS1_PM_READ_PERMISSION_2 0x80C90
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_READ_PERMISSION_2 0x1BF80C90u

//! Register Reset Value
#define TUS1_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_WRITE_PERMISSION_2 Register TUS1_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TUS1_PM_WRITE_PERMISSION_2 0x80C98
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_WRITE_PERMISSION_2 0x1BF80C98u

//! Register Reset Value
#define TUS1_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_ADDR_MATCH_3 Register TUS1_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TUS1_PM_ADDR_MATCH_3 0x80CA0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ADDR_MATCH_3 0x1BF80CA0u

//! Register Reset Value
#define TUS1_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1_PM_REQ_INFO_PERMISSION_3 Register TUS1_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TUS1_PM_REQ_INFO_PERMISSION_3 0x80CA8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_REQ_INFO_PERMISSION_3 0x1BF80CA8u

//! Register Reset Value
#define TUS1_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_READ_PERMISSION_3 Register TUS1_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TUS1_PM_READ_PERMISSION_3 0x80CB0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_READ_PERMISSION_3 0x1BF80CB0u

//! Register Reset Value
#define TUS1_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_WRITE_PERMISSION_3 Register TUS1_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TUS1_PM_WRITE_PERMISSION_3 0x80CB8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_WRITE_PERMISSION_3 0x1BF80CB8u

//! Register Reset Value
#define TUS1_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_ADDR_MATCH_4 Register TUS1_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TUS1_PM_ADDR_MATCH_4 0x80CC0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ADDR_MATCH_4 0x1BF80CC0u

//! Register Reset Value
#define TUS1_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1_PM_REQ_INFO_PERMISSION_4 Register TUS1_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TUS1_PM_REQ_INFO_PERMISSION_4 0x80CC8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_REQ_INFO_PERMISSION_4 0x1BF80CC8u

//! Register Reset Value
#define TUS1_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_READ_PERMISSION_4 Register TUS1_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TUS1_PM_READ_PERMISSION_4 0x80CD0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_READ_PERMISSION_4 0x1BF80CD0u

//! Register Reset Value
#define TUS1_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_WRITE_PERMISSION_4 Register TUS1_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TUS1_PM_WRITE_PERMISSION_4 0x80CD8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_WRITE_PERMISSION_4 0x1BF80CD8u

//! Register Reset Value
#define TUS1_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_ADDR_MATCH_5 Register TUS1_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TUS1_PM_ADDR_MATCH_5 0x80CE0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ADDR_MATCH_5 0x1BF80CE0u

//! Register Reset Value
#define TUS1_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1_PM_REQ_INFO_PERMISSION_5 Register TUS1_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TUS1_PM_REQ_INFO_PERMISSION_5 0x80CE8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_REQ_INFO_PERMISSION_5 0x1BF80CE8u

//! Register Reset Value
#define TUS1_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_READ_PERMISSION_5 Register TUS1_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TUS1_PM_READ_PERMISSION_5 0x80CF0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_READ_PERMISSION_5 0x1BF80CF0u

//! Register Reset Value
#define TUS1_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_WRITE_PERMISSION_5 Register TUS1_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TUS1_PM_WRITE_PERMISSION_5 0x80CF8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_WRITE_PERMISSION_5 0x1BF80CF8u

//! Register Reset Value
#define TUS1_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_ADDR_MATCH_6 Register TUS1_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TUS1_PM_ADDR_MATCH_6 0x80D00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ADDR_MATCH_6 0x1BF80D00u

//! Register Reset Value
#define TUS1_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1_PM_REQ_INFO_PERMISSION_6 Register TUS1_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TUS1_PM_REQ_INFO_PERMISSION_6 0x80D08
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_REQ_INFO_PERMISSION_6 0x1BF80D08u

//! Register Reset Value
#define TUS1_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_READ_PERMISSION_6 Register TUS1_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TUS1_PM_READ_PERMISSION_6 0x80D10
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_READ_PERMISSION_6 0x1BF80D10u

//! Register Reset Value
#define TUS1_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_WRITE_PERMISSION_6 Register TUS1_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TUS1_PM_WRITE_PERMISSION_6 0x80D18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_WRITE_PERMISSION_6 0x1BF80D18u

//! Register Reset Value
#define TUS1_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_ADDR_MATCH_7 Register TUS1_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TUS1_PM_ADDR_MATCH_7 0x80D20
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ADDR_MATCH_7 0x1BF80D20u

//! Register Reset Value
#define TUS1_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1_PM_REQ_INFO_PERMISSION_7 Register TUS1_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TUS1_PM_REQ_INFO_PERMISSION_7 0x80D28
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_REQ_INFO_PERMISSION_7 0x1BF80D28u

//! Register Reset Value
#define TUS1_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_READ_PERMISSION_7 Register TUS1_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TUS1_PM_READ_PERMISSION_7 0x80D30
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_READ_PERMISSION_7 0x1BF80D30u

//! Register Reset Value
#define TUS1_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_WRITE_PERMISSION_7 Register TUS1_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TUS1_PM_WRITE_PERMISSION_7 0x80D38
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_WRITE_PERMISSION_7 0x1BF80D38u

//! Register Reset Value
#define TUS1_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_ADDR_MATCH_8 Register TUS1_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TUS1_PM_ADDR_MATCH_8 0x80D40
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ADDR_MATCH_8 0x1BF80D40u

//! Register Reset Value
#define TUS1_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1_PM_REQ_INFO_PERMISSION_8 Register TUS1_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TUS1_PM_REQ_INFO_PERMISSION_8 0x80D48
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_REQ_INFO_PERMISSION_8 0x1BF80D48u

//! Register Reset Value
#define TUS1_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_READ_PERMISSION_8 Register TUS1_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TUS1_PM_READ_PERMISSION_8 0x80D50
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_READ_PERMISSION_8 0x1BF80D50u

//! Register Reset Value
#define TUS1_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_WRITE_PERMISSION_8 Register TUS1_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TUS1_PM_WRITE_PERMISSION_8 0x80D58
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_WRITE_PERMISSION_8 0x1BF80D58u

//! Register Reset Value
#define TUS1_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_ADDR_MATCH_9 Register TUS1_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TUS1_PM_ADDR_MATCH_9 0x80D60
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ADDR_MATCH_9 0x1BF80D60u

//! Register Reset Value
#define TUS1_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1_PM_REQ_INFO_PERMISSION_9 Register TUS1_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TUS1_PM_REQ_INFO_PERMISSION_9 0x80D68
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_REQ_INFO_PERMISSION_9 0x1BF80D68u

//! Register Reset Value
#define TUS1_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_READ_PERMISSION_9 Register TUS1_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TUS1_PM_READ_PERMISSION_9 0x80D70
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_READ_PERMISSION_9 0x1BF80D70u

//! Register Reset Value
#define TUS1_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_WRITE_PERMISSION_9 Register TUS1_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TUS1_PM_WRITE_PERMISSION_9 0x80D78
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_WRITE_PERMISSION_9 0x1BF80D78u

//! Register Reset Value
#define TUS1_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_ADDR_MATCH_10 Register TUS1_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TUS1_PM_ADDR_MATCH_10 0x80D80
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ADDR_MATCH_10 0x1BF80D80u

//! Register Reset Value
#define TUS1_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1_PM_REQ_INFO_PERMISSION_10 Register TUS1_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TUS1_PM_REQ_INFO_PERMISSION_10 0x80D88
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_REQ_INFO_PERMISSION_10 0x1BF80D88u

//! Register Reset Value
#define TUS1_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_READ_PERMISSION_10 Register TUS1_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TUS1_PM_READ_PERMISSION_10 0x80D90
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_READ_PERMISSION_10 0x1BF80D90u

//! Register Reset Value
#define TUS1_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_WRITE_PERMISSION_10 Register TUS1_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TUS1_PM_WRITE_PERMISSION_10 0x80D98
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_WRITE_PERMISSION_10 0x1BF80D98u

//! Register Reset Value
#define TUS1_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_ADDR_MATCH_11 Register TUS1_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TUS1_PM_ADDR_MATCH_11 0x80DA0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ADDR_MATCH_11 0x1BF80DA0u

//! Register Reset Value
#define TUS1_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1_PM_REQ_INFO_PERMISSION_11 Register TUS1_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TUS1_PM_REQ_INFO_PERMISSION_11 0x80DA8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_REQ_INFO_PERMISSION_11 0x1BF80DA8u

//! Register Reset Value
#define TUS1_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_READ_PERMISSION_11 Register TUS1_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TUS1_PM_READ_PERMISSION_11 0x80DB0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_READ_PERMISSION_11 0x1BF80DB0u

//! Register Reset Value
#define TUS1_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_WRITE_PERMISSION_11 Register TUS1_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TUS1_PM_WRITE_PERMISSION_11 0x80DB8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_WRITE_PERMISSION_11 0x1BF80DB8u

//! Register Reset Value
#define TUS1_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_ADDR_MATCH_12 Register TUS1_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TUS1_PM_ADDR_MATCH_12 0x80DC0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ADDR_MATCH_12 0x1BF80DC0u

//! Register Reset Value
#define TUS1_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1_PM_REQ_INFO_PERMISSION_12 Register TUS1_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TUS1_PM_REQ_INFO_PERMISSION_12 0x80DC8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_REQ_INFO_PERMISSION_12 0x1BF80DC8u

//! Register Reset Value
#define TUS1_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_READ_PERMISSION_12 Register TUS1_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TUS1_PM_READ_PERMISSION_12 0x80DD0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_READ_PERMISSION_12 0x1BF80DD0u

//! Register Reset Value
#define TUS1_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_WRITE_PERMISSION_12 Register TUS1_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TUS1_PM_WRITE_PERMISSION_12 0x80DD8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_WRITE_PERMISSION_12 0x1BF80DD8u

//! Register Reset Value
#define TUS1_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_ADDR_MATCH_13 Register TUS1_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TUS1_PM_ADDR_MATCH_13 0x80DE0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ADDR_MATCH_13 0x1BF80DE0u

//! Register Reset Value
#define TUS1_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1_PM_REQ_INFO_PERMISSION_13 Register TUS1_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TUS1_PM_REQ_INFO_PERMISSION_13 0x80DE8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_REQ_INFO_PERMISSION_13 0x1BF80DE8u

//! Register Reset Value
#define TUS1_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_READ_PERMISSION_13 Register TUS1_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TUS1_PM_READ_PERMISSION_13 0x80DF0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_READ_PERMISSION_13 0x1BF80DF0u

//! Register Reset Value
#define TUS1_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_WRITE_PERMISSION_13 Register TUS1_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TUS1_PM_WRITE_PERMISSION_13 0x80DF8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_WRITE_PERMISSION_13 0x1BF80DF8u

//! Register Reset Value
#define TUS1_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_ADDR_MATCH_14 Register TUS1_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TUS1_PM_ADDR_MATCH_14 0x80E00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ADDR_MATCH_14 0x1BF80E00u

//! Register Reset Value
#define TUS1_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1_PM_REQ_INFO_PERMISSION_14 Register TUS1_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TUS1_PM_REQ_INFO_PERMISSION_14 0x80E08
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_REQ_INFO_PERMISSION_14 0x1BF80E08u

//! Register Reset Value
#define TUS1_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_READ_PERMISSION_14 Register TUS1_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TUS1_PM_READ_PERMISSION_14 0x80E10
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_READ_PERMISSION_14 0x1BF80E10u

//! Register Reset Value
#define TUS1_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_WRITE_PERMISSION_14 Register TUS1_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TUS1_PM_WRITE_PERMISSION_14 0x80E18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_WRITE_PERMISSION_14 0x1BF80E18u

//! Register Reset Value
#define TUS1_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_ADDR_MATCH_15 Register TUS1_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TUS1_PM_ADDR_MATCH_15 0x80E20
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_ADDR_MATCH_15 0x1BF80E20u

//! Register Reset Value
#define TUS1_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TUS1_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1_PM_REQ_INFO_PERMISSION_15 Register TUS1_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TUS1_PM_REQ_INFO_PERMISSION_15 0x80E28
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_REQ_INFO_PERMISSION_15 0x1BF80E28u

//! Register Reset Value
#define TUS1_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_READ_PERMISSION_15 Register TUS1_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TUS1_PM_READ_PERMISSION_15 0x80E30
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_READ_PERMISSION_15 0x1BF80E30u

//! Register Reset Value
#define TUS1_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1_PM_WRITE_PERMISSION_15 Register TUS1_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TUS1_PM_WRITE_PERMISSION_15 0x80E38
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1_PM_WRITE_PERMISSION_15 0x1BF80E38u

//! Register Reset Value
#define TUS1_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_ERROR_LOG Register TUS1C_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ERROR_LOG 0x81020
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ERROR_LOG 0x1BF81020u

//! Register Reset Value
#define TUS1C_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TUS1C_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TUS1C_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TUS1C_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TUS1C_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TUS1C_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TUS1C_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TUS1C_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TUS1C_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TUS1C_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TUS1C_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TUS1C_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TUS1C_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TUS1C_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TUS1C_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TUS1C_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TUS1C_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TUS1C_PM_CONTROL Register TUS1C_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TUS1C_PM_CONTROL 0x81028
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_CONTROL 0x1BF81028u

//! Register Reset Value
#define TUS1C_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TUS1C_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TUS1C_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TUS1C_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TUS1C_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TUS1C_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TUS1C_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TUS1C_PM_ERROR_CLEAR_SINGLE Register TUS1C_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ERROR_CLEAR_SINGLE 0x81030
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ERROR_CLEAR_SINGLE 0x1BF81030u

//! Register Reset Value
#define TUS1C_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TUS1C_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TUS1C_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TUS1C_PM_ERROR_CLEAR_MULTI Register TUS1C_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ERROR_CLEAR_MULTI 0x81038
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ERROR_CLEAR_MULTI 0x1BF81038u

//! Register Reset Value
#define TUS1C_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TUS1C_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TUS1C_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TUS1C_PM_REQ_INFO_PERMISSION_0 Register TUS1C_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TUS1C_PM_REQ_INFO_PERMISSION_0 0x81048
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_REQ_INFO_PERMISSION_0 0x1BF81048u

//! Register Reset Value
#define TUS1C_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_READ_PERMISSION_0 Register TUS1C_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TUS1C_PM_READ_PERMISSION_0 0x81050
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_READ_PERMISSION_0 0x1BF81050u

//! Register Reset Value
#define TUS1C_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TUS1C_PM_WRITE_PERMISSION_0 Register TUS1C_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TUS1C_PM_WRITE_PERMISSION_0 0x81058
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_WRITE_PERMISSION_0 0x1BF81058u

//! Register Reset Value
#define TUS1C_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TUS1C_PM_ADDR_MATCH_1 Register TUS1C_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ADDR_MATCH_1 0x81060
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ADDR_MATCH_1 0x1BF81060u

//! Register Reset Value
#define TUS1C_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1C_PM_REQ_INFO_PERMISSION_1 Register TUS1C_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TUS1C_PM_REQ_INFO_PERMISSION_1 0x81068
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_REQ_INFO_PERMISSION_1 0x1BF81068u

//! Register Reset Value
#define TUS1C_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_READ_PERMISSION_1 Register TUS1C_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TUS1C_PM_READ_PERMISSION_1 0x81070
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_READ_PERMISSION_1 0x1BF81070u

//! Register Reset Value
#define TUS1C_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_WRITE_PERMISSION_1 Register TUS1C_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TUS1C_PM_WRITE_PERMISSION_1 0x81078
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_WRITE_PERMISSION_1 0x1BF81078u

//! Register Reset Value
#define TUS1C_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_ADDR_MATCH_2 Register TUS1C_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ADDR_MATCH_2 0x81080
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ADDR_MATCH_2 0x1BF81080u

//! Register Reset Value
#define TUS1C_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1C_PM_REQ_INFO_PERMISSION_2 Register TUS1C_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TUS1C_PM_REQ_INFO_PERMISSION_2 0x81088
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_REQ_INFO_PERMISSION_2 0x1BF81088u

//! Register Reset Value
#define TUS1C_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_READ_PERMISSION_2 Register TUS1C_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TUS1C_PM_READ_PERMISSION_2 0x81090
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_READ_PERMISSION_2 0x1BF81090u

//! Register Reset Value
#define TUS1C_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_WRITE_PERMISSION_2 Register TUS1C_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TUS1C_PM_WRITE_PERMISSION_2 0x81098
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_WRITE_PERMISSION_2 0x1BF81098u

//! Register Reset Value
#define TUS1C_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_ADDR_MATCH_3 Register TUS1C_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ADDR_MATCH_3 0x810A0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ADDR_MATCH_3 0x1BF810A0u

//! Register Reset Value
#define TUS1C_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1C_PM_REQ_INFO_PERMISSION_3 Register TUS1C_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TUS1C_PM_REQ_INFO_PERMISSION_3 0x810A8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_REQ_INFO_PERMISSION_3 0x1BF810A8u

//! Register Reset Value
#define TUS1C_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_READ_PERMISSION_3 Register TUS1C_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TUS1C_PM_READ_PERMISSION_3 0x810B0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_READ_PERMISSION_3 0x1BF810B0u

//! Register Reset Value
#define TUS1C_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_WRITE_PERMISSION_3 Register TUS1C_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TUS1C_PM_WRITE_PERMISSION_3 0x810B8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_WRITE_PERMISSION_3 0x1BF810B8u

//! Register Reset Value
#define TUS1C_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_ADDR_MATCH_4 Register TUS1C_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ADDR_MATCH_4 0x810C0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ADDR_MATCH_4 0x1BF810C0u

//! Register Reset Value
#define TUS1C_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1C_PM_REQ_INFO_PERMISSION_4 Register TUS1C_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TUS1C_PM_REQ_INFO_PERMISSION_4 0x810C8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_REQ_INFO_PERMISSION_4 0x1BF810C8u

//! Register Reset Value
#define TUS1C_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_READ_PERMISSION_4 Register TUS1C_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TUS1C_PM_READ_PERMISSION_4 0x810D0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_READ_PERMISSION_4 0x1BF810D0u

//! Register Reset Value
#define TUS1C_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_WRITE_PERMISSION_4 Register TUS1C_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TUS1C_PM_WRITE_PERMISSION_4 0x810D8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_WRITE_PERMISSION_4 0x1BF810D8u

//! Register Reset Value
#define TUS1C_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_ADDR_MATCH_5 Register TUS1C_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ADDR_MATCH_5 0x810E0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ADDR_MATCH_5 0x1BF810E0u

//! Register Reset Value
#define TUS1C_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1C_PM_REQ_INFO_PERMISSION_5 Register TUS1C_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TUS1C_PM_REQ_INFO_PERMISSION_5 0x810E8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_REQ_INFO_PERMISSION_5 0x1BF810E8u

//! Register Reset Value
#define TUS1C_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_READ_PERMISSION_5 Register TUS1C_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TUS1C_PM_READ_PERMISSION_5 0x810F0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_READ_PERMISSION_5 0x1BF810F0u

//! Register Reset Value
#define TUS1C_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_WRITE_PERMISSION_5 Register TUS1C_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TUS1C_PM_WRITE_PERMISSION_5 0x810F8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_WRITE_PERMISSION_5 0x1BF810F8u

//! Register Reset Value
#define TUS1C_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_ADDR_MATCH_6 Register TUS1C_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ADDR_MATCH_6 0x81100
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ADDR_MATCH_6 0x1BF81100u

//! Register Reset Value
#define TUS1C_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1C_PM_REQ_INFO_PERMISSION_6 Register TUS1C_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TUS1C_PM_REQ_INFO_PERMISSION_6 0x81108
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_REQ_INFO_PERMISSION_6 0x1BF81108u

//! Register Reset Value
#define TUS1C_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_READ_PERMISSION_6 Register TUS1C_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TUS1C_PM_READ_PERMISSION_6 0x81110
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_READ_PERMISSION_6 0x1BF81110u

//! Register Reset Value
#define TUS1C_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_WRITE_PERMISSION_6 Register TUS1C_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TUS1C_PM_WRITE_PERMISSION_6 0x81118
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_WRITE_PERMISSION_6 0x1BF81118u

//! Register Reset Value
#define TUS1C_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_ADDR_MATCH_7 Register TUS1C_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ADDR_MATCH_7 0x81120
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ADDR_MATCH_7 0x1BF81120u

//! Register Reset Value
#define TUS1C_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1C_PM_REQ_INFO_PERMISSION_7 Register TUS1C_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TUS1C_PM_REQ_INFO_PERMISSION_7 0x81128
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_REQ_INFO_PERMISSION_7 0x1BF81128u

//! Register Reset Value
#define TUS1C_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_READ_PERMISSION_7 Register TUS1C_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TUS1C_PM_READ_PERMISSION_7 0x81130
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_READ_PERMISSION_7 0x1BF81130u

//! Register Reset Value
#define TUS1C_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_WRITE_PERMISSION_7 Register TUS1C_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TUS1C_PM_WRITE_PERMISSION_7 0x81138
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_WRITE_PERMISSION_7 0x1BF81138u

//! Register Reset Value
#define TUS1C_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_ADDR_MATCH_8 Register TUS1C_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ADDR_MATCH_8 0x81140
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ADDR_MATCH_8 0x1BF81140u

//! Register Reset Value
#define TUS1C_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1C_PM_REQ_INFO_PERMISSION_8 Register TUS1C_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TUS1C_PM_REQ_INFO_PERMISSION_8 0x81148
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_REQ_INFO_PERMISSION_8 0x1BF81148u

//! Register Reset Value
#define TUS1C_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_READ_PERMISSION_8 Register TUS1C_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TUS1C_PM_READ_PERMISSION_8 0x81150
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_READ_PERMISSION_8 0x1BF81150u

//! Register Reset Value
#define TUS1C_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_WRITE_PERMISSION_8 Register TUS1C_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TUS1C_PM_WRITE_PERMISSION_8 0x81158
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_WRITE_PERMISSION_8 0x1BF81158u

//! Register Reset Value
#define TUS1C_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_ADDR_MATCH_9 Register TUS1C_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ADDR_MATCH_9 0x81160
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ADDR_MATCH_9 0x1BF81160u

//! Register Reset Value
#define TUS1C_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1C_PM_REQ_INFO_PERMISSION_9 Register TUS1C_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TUS1C_PM_REQ_INFO_PERMISSION_9 0x81168
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_REQ_INFO_PERMISSION_9 0x1BF81168u

//! Register Reset Value
#define TUS1C_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_READ_PERMISSION_9 Register TUS1C_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TUS1C_PM_READ_PERMISSION_9 0x81170
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_READ_PERMISSION_9 0x1BF81170u

//! Register Reset Value
#define TUS1C_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_WRITE_PERMISSION_9 Register TUS1C_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TUS1C_PM_WRITE_PERMISSION_9 0x81178
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_WRITE_PERMISSION_9 0x1BF81178u

//! Register Reset Value
#define TUS1C_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_ADDR_MATCH_10 Register TUS1C_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ADDR_MATCH_10 0x81180
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ADDR_MATCH_10 0x1BF81180u

//! Register Reset Value
#define TUS1C_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1C_PM_REQ_INFO_PERMISSION_10 Register TUS1C_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TUS1C_PM_REQ_INFO_PERMISSION_10 0x81188
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_REQ_INFO_PERMISSION_10 0x1BF81188u

//! Register Reset Value
#define TUS1C_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_READ_PERMISSION_10 Register TUS1C_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TUS1C_PM_READ_PERMISSION_10 0x81190
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_READ_PERMISSION_10 0x1BF81190u

//! Register Reset Value
#define TUS1C_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_WRITE_PERMISSION_10 Register TUS1C_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TUS1C_PM_WRITE_PERMISSION_10 0x81198
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_WRITE_PERMISSION_10 0x1BF81198u

//! Register Reset Value
#define TUS1C_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_ADDR_MATCH_11 Register TUS1C_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ADDR_MATCH_11 0x811A0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ADDR_MATCH_11 0x1BF811A0u

//! Register Reset Value
#define TUS1C_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1C_PM_REQ_INFO_PERMISSION_11 Register TUS1C_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TUS1C_PM_REQ_INFO_PERMISSION_11 0x811A8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_REQ_INFO_PERMISSION_11 0x1BF811A8u

//! Register Reset Value
#define TUS1C_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_READ_PERMISSION_11 Register TUS1C_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TUS1C_PM_READ_PERMISSION_11 0x811B0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_READ_PERMISSION_11 0x1BF811B0u

//! Register Reset Value
#define TUS1C_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_WRITE_PERMISSION_11 Register TUS1C_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TUS1C_PM_WRITE_PERMISSION_11 0x811B8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_WRITE_PERMISSION_11 0x1BF811B8u

//! Register Reset Value
#define TUS1C_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_ADDR_MATCH_12 Register TUS1C_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ADDR_MATCH_12 0x811C0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ADDR_MATCH_12 0x1BF811C0u

//! Register Reset Value
#define TUS1C_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1C_PM_REQ_INFO_PERMISSION_12 Register TUS1C_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TUS1C_PM_REQ_INFO_PERMISSION_12 0x811C8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_REQ_INFO_PERMISSION_12 0x1BF811C8u

//! Register Reset Value
#define TUS1C_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_READ_PERMISSION_12 Register TUS1C_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TUS1C_PM_READ_PERMISSION_12 0x811D0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_READ_PERMISSION_12 0x1BF811D0u

//! Register Reset Value
#define TUS1C_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_WRITE_PERMISSION_12 Register TUS1C_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TUS1C_PM_WRITE_PERMISSION_12 0x811D8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_WRITE_PERMISSION_12 0x1BF811D8u

//! Register Reset Value
#define TUS1C_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_ADDR_MATCH_13 Register TUS1C_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ADDR_MATCH_13 0x811E0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ADDR_MATCH_13 0x1BF811E0u

//! Register Reset Value
#define TUS1C_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1C_PM_REQ_INFO_PERMISSION_13 Register TUS1C_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TUS1C_PM_REQ_INFO_PERMISSION_13 0x811E8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_REQ_INFO_PERMISSION_13 0x1BF811E8u

//! Register Reset Value
#define TUS1C_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_READ_PERMISSION_13 Register TUS1C_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TUS1C_PM_READ_PERMISSION_13 0x811F0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_READ_PERMISSION_13 0x1BF811F0u

//! Register Reset Value
#define TUS1C_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_WRITE_PERMISSION_13 Register TUS1C_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TUS1C_PM_WRITE_PERMISSION_13 0x811F8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_WRITE_PERMISSION_13 0x1BF811F8u

//! Register Reset Value
#define TUS1C_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_ADDR_MATCH_14 Register TUS1C_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ADDR_MATCH_14 0x81200
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ADDR_MATCH_14 0x1BF81200u

//! Register Reset Value
#define TUS1C_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1C_PM_REQ_INFO_PERMISSION_14 Register TUS1C_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TUS1C_PM_REQ_INFO_PERMISSION_14 0x81208
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_REQ_INFO_PERMISSION_14 0x1BF81208u

//! Register Reset Value
#define TUS1C_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_READ_PERMISSION_14 Register TUS1C_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TUS1C_PM_READ_PERMISSION_14 0x81210
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_READ_PERMISSION_14 0x1BF81210u

//! Register Reset Value
#define TUS1C_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_WRITE_PERMISSION_14 Register TUS1C_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TUS1C_PM_WRITE_PERMISSION_14 0x81218
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_WRITE_PERMISSION_14 0x1BF81218u

//! Register Reset Value
#define TUS1C_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_ADDR_MATCH_15 Register TUS1C_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TUS1C_PM_ADDR_MATCH_15 0x81220
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_ADDR_MATCH_15 0x1BF81220u

//! Register Reset Value
#define TUS1C_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TUS1C_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TUS1C_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TUS1C_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TUS1C_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TUS1C_PM_REQ_INFO_PERMISSION_15 Register TUS1C_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TUS1C_PM_REQ_INFO_PERMISSION_15 0x81228
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_REQ_INFO_PERMISSION_15 0x1BF81228u

//! Register Reset Value
#define TUS1C_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TUS1C_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_READ_PERMISSION_15 Register TUS1C_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TUS1C_PM_READ_PERMISSION_15 0x81230
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_READ_PERMISSION_15 0x1BF81230u

//! Register Reset Value
#define TUS1C_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TUS1C_PM_WRITE_PERMISSION_15 Register TUS1C_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TUS1C_PM_WRITE_PERMISSION_15 0x81238
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TUS1C_PM_WRITE_PERMISSION_15 0x1BF81238u

//! Register Reset Value
#define TUS1C_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TUS1C_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_ERROR_LOG Register TDM1R_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ERROR_LOG 0x81420
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ERROR_LOG 0x1BF81420u

//! Register Reset Value
#define TDM1R_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TDM1R_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TDM1R_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TDM1R_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TDM1R_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TDM1R_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TDM1R_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TDM1R_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TDM1R_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TDM1R_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TDM1R_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TDM1R_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TDM1R_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TDM1R_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TDM1R_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TDM1R_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TDM1R_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TDM1R_PM_CONTROL Register TDM1R_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TDM1R_PM_CONTROL 0x81428
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_CONTROL 0x1BF81428u

//! Register Reset Value
#define TDM1R_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TDM1R_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TDM1R_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TDM1R_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TDM1R_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TDM1R_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TDM1R_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TDM1R_PM_ERROR_CLEAR_SINGLE Register TDM1R_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ERROR_CLEAR_SINGLE 0x81430
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ERROR_CLEAR_SINGLE 0x1BF81430u

//! Register Reset Value
#define TDM1R_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TDM1R_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TDM1R_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TDM1R_PM_ERROR_CLEAR_MULTI Register TDM1R_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ERROR_CLEAR_MULTI 0x81438
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ERROR_CLEAR_MULTI 0x1BF81438u

//! Register Reset Value
#define TDM1R_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TDM1R_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TDM1R_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TDM1R_PM_REQ_INFO_PERMISSION_0 Register TDM1R_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TDM1R_PM_REQ_INFO_PERMISSION_0 0x81448
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_REQ_INFO_PERMISSION_0 0x1BF81448u

//! Register Reset Value
#define TDM1R_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_READ_PERMISSION_0 Register TDM1R_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TDM1R_PM_READ_PERMISSION_0 0x81450
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_READ_PERMISSION_0 0x1BF81450u

//! Register Reset Value
#define TDM1R_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TDM1R_PM_WRITE_PERMISSION_0 Register TDM1R_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TDM1R_PM_WRITE_PERMISSION_0 0x81458
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_WRITE_PERMISSION_0 0x1BF81458u

//! Register Reset Value
#define TDM1R_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TDM1R_PM_ADDR_MATCH_1 Register TDM1R_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ADDR_MATCH_1 0x81460
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ADDR_MATCH_1 0x1BF81460u

//! Register Reset Value
#define TDM1R_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1R_PM_REQ_INFO_PERMISSION_1 Register TDM1R_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TDM1R_PM_REQ_INFO_PERMISSION_1 0x81468
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_REQ_INFO_PERMISSION_1 0x1BF81468u

//! Register Reset Value
#define TDM1R_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_READ_PERMISSION_1 Register TDM1R_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TDM1R_PM_READ_PERMISSION_1 0x81470
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_READ_PERMISSION_1 0x1BF81470u

//! Register Reset Value
#define TDM1R_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_WRITE_PERMISSION_1 Register TDM1R_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TDM1R_PM_WRITE_PERMISSION_1 0x81478
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_WRITE_PERMISSION_1 0x1BF81478u

//! Register Reset Value
#define TDM1R_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_ADDR_MATCH_2 Register TDM1R_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ADDR_MATCH_2 0x81480
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ADDR_MATCH_2 0x1BF81480u

//! Register Reset Value
#define TDM1R_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1R_PM_REQ_INFO_PERMISSION_2 Register TDM1R_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TDM1R_PM_REQ_INFO_PERMISSION_2 0x81488
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_REQ_INFO_PERMISSION_2 0x1BF81488u

//! Register Reset Value
#define TDM1R_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_READ_PERMISSION_2 Register TDM1R_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TDM1R_PM_READ_PERMISSION_2 0x81490
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_READ_PERMISSION_2 0x1BF81490u

//! Register Reset Value
#define TDM1R_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_WRITE_PERMISSION_2 Register TDM1R_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TDM1R_PM_WRITE_PERMISSION_2 0x81498
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_WRITE_PERMISSION_2 0x1BF81498u

//! Register Reset Value
#define TDM1R_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_ADDR_MATCH_3 Register TDM1R_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ADDR_MATCH_3 0x814A0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ADDR_MATCH_3 0x1BF814A0u

//! Register Reset Value
#define TDM1R_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1R_PM_REQ_INFO_PERMISSION_3 Register TDM1R_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TDM1R_PM_REQ_INFO_PERMISSION_3 0x814A8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_REQ_INFO_PERMISSION_3 0x1BF814A8u

//! Register Reset Value
#define TDM1R_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_READ_PERMISSION_3 Register TDM1R_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TDM1R_PM_READ_PERMISSION_3 0x814B0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_READ_PERMISSION_3 0x1BF814B0u

//! Register Reset Value
#define TDM1R_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_WRITE_PERMISSION_3 Register TDM1R_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TDM1R_PM_WRITE_PERMISSION_3 0x814B8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_WRITE_PERMISSION_3 0x1BF814B8u

//! Register Reset Value
#define TDM1R_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_ADDR_MATCH_4 Register TDM1R_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ADDR_MATCH_4 0x814C0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ADDR_MATCH_4 0x1BF814C0u

//! Register Reset Value
#define TDM1R_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1R_PM_REQ_INFO_PERMISSION_4 Register TDM1R_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TDM1R_PM_REQ_INFO_PERMISSION_4 0x814C8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_REQ_INFO_PERMISSION_4 0x1BF814C8u

//! Register Reset Value
#define TDM1R_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_READ_PERMISSION_4 Register TDM1R_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TDM1R_PM_READ_PERMISSION_4 0x814D0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_READ_PERMISSION_4 0x1BF814D0u

//! Register Reset Value
#define TDM1R_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_WRITE_PERMISSION_4 Register TDM1R_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TDM1R_PM_WRITE_PERMISSION_4 0x814D8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_WRITE_PERMISSION_4 0x1BF814D8u

//! Register Reset Value
#define TDM1R_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_ADDR_MATCH_5 Register TDM1R_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ADDR_MATCH_5 0x814E0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ADDR_MATCH_5 0x1BF814E0u

//! Register Reset Value
#define TDM1R_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1R_PM_REQ_INFO_PERMISSION_5 Register TDM1R_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TDM1R_PM_REQ_INFO_PERMISSION_5 0x814E8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_REQ_INFO_PERMISSION_5 0x1BF814E8u

//! Register Reset Value
#define TDM1R_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_READ_PERMISSION_5 Register TDM1R_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TDM1R_PM_READ_PERMISSION_5 0x814F0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_READ_PERMISSION_5 0x1BF814F0u

//! Register Reset Value
#define TDM1R_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_WRITE_PERMISSION_5 Register TDM1R_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TDM1R_PM_WRITE_PERMISSION_5 0x814F8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_WRITE_PERMISSION_5 0x1BF814F8u

//! Register Reset Value
#define TDM1R_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_ADDR_MATCH_6 Register TDM1R_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ADDR_MATCH_6 0x81500
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ADDR_MATCH_6 0x1BF81500u

//! Register Reset Value
#define TDM1R_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1R_PM_REQ_INFO_PERMISSION_6 Register TDM1R_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TDM1R_PM_REQ_INFO_PERMISSION_6 0x81508
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_REQ_INFO_PERMISSION_6 0x1BF81508u

//! Register Reset Value
#define TDM1R_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_READ_PERMISSION_6 Register TDM1R_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TDM1R_PM_READ_PERMISSION_6 0x81510
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_READ_PERMISSION_6 0x1BF81510u

//! Register Reset Value
#define TDM1R_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_WRITE_PERMISSION_6 Register TDM1R_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TDM1R_PM_WRITE_PERMISSION_6 0x81518
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_WRITE_PERMISSION_6 0x1BF81518u

//! Register Reset Value
#define TDM1R_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_ADDR_MATCH_7 Register TDM1R_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ADDR_MATCH_7 0x81520
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ADDR_MATCH_7 0x1BF81520u

//! Register Reset Value
#define TDM1R_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1R_PM_REQ_INFO_PERMISSION_7 Register TDM1R_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TDM1R_PM_REQ_INFO_PERMISSION_7 0x81528
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_REQ_INFO_PERMISSION_7 0x1BF81528u

//! Register Reset Value
#define TDM1R_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_READ_PERMISSION_7 Register TDM1R_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TDM1R_PM_READ_PERMISSION_7 0x81530
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_READ_PERMISSION_7 0x1BF81530u

//! Register Reset Value
#define TDM1R_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_WRITE_PERMISSION_7 Register TDM1R_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TDM1R_PM_WRITE_PERMISSION_7 0x81538
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_WRITE_PERMISSION_7 0x1BF81538u

//! Register Reset Value
#define TDM1R_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_ADDR_MATCH_8 Register TDM1R_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ADDR_MATCH_8 0x81540
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ADDR_MATCH_8 0x1BF81540u

//! Register Reset Value
#define TDM1R_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1R_PM_REQ_INFO_PERMISSION_8 Register TDM1R_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TDM1R_PM_REQ_INFO_PERMISSION_8 0x81548
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_REQ_INFO_PERMISSION_8 0x1BF81548u

//! Register Reset Value
#define TDM1R_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_READ_PERMISSION_8 Register TDM1R_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TDM1R_PM_READ_PERMISSION_8 0x81550
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_READ_PERMISSION_8 0x1BF81550u

//! Register Reset Value
#define TDM1R_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_WRITE_PERMISSION_8 Register TDM1R_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TDM1R_PM_WRITE_PERMISSION_8 0x81558
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_WRITE_PERMISSION_8 0x1BF81558u

//! Register Reset Value
#define TDM1R_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_ADDR_MATCH_9 Register TDM1R_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ADDR_MATCH_9 0x81560
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ADDR_MATCH_9 0x1BF81560u

//! Register Reset Value
#define TDM1R_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1R_PM_REQ_INFO_PERMISSION_9 Register TDM1R_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TDM1R_PM_REQ_INFO_PERMISSION_9 0x81568
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_REQ_INFO_PERMISSION_9 0x1BF81568u

//! Register Reset Value
#define TDM1R_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_READ_PERMISSION_9 Register TDM1R_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TDM1R_PM_READ_PERMISSION_9 0x81570
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_READ_PERMISSION_9 0x1BF81570u

//! Register Reset Value
#define TDM1R_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_WRITE_PERMISSION_9 Register TDM1R_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TDM1R_PM_WRITE_PERMISSION_9 0x81578
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_WRITE_PERMISSION_9 0x1BF81578u

//! Register Reset Value
#define TDM1R_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_ADDR_MATCH_10 Register TDM1R_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ADDR_MATCH_10 0x81580
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ADDR_MATCH_10 0x1BF81580u

//! Register Reset Value
#define TDM1R_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1R_PM_REQ_INFO_PERMISSION_10 Register TDM1R_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TDM1R_PM_REQ_INFO_PERMISSION_10 0x81588
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_REQ_INFO_PERMISSION_10 0x1BF81588u

//! Register Reset Value
#define TDM1R_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_READ_PERMISSION_10 Register TDM1R_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TDM1R_PM_READ_PERMISSION_10 0x81590
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_READ_PERMISSION_10 0x1BF81590u

//! Register Reset Value
#define TDM1R_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_WRITE_PERMISSION_10 Register TDM1R_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TDM1R_PM_WRITE_PERMISSION_10 0x81598
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_WRITE_PERMISSION_10 0x1BF81598u

//! Register Reset Value
#define TDM1R_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_ADDR_MATCH_11 Register TDM1R_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ADDR_MATCH_11 0x815A0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ADDR_MATCH_11 0x1BF815A0u

//! Register Reset Value
#define TDM1R_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1R_PM_REQ_INFO_PERMISSION_11 Register TDM1R_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TDM1R_PM_REQ_INFO_PERMISSION_11 0x815A8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_REQ_INFO_PERMISSION_11 0x1BF815A8u

//! Register Reset Value
#define TDM1R_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_READ_PERMISSION_11 Register TDM1R_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TDM1R_PM_READ_PERMISSION_11 0x815B0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_READ_PERMISSION_11 0x1BF815B0u

//! Register Reset Value
#define TDM1R_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_WRITE_PERMISSION_11 Register TDM1R_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TDM1R_PM_WRITE_PERMISSION_11 0x815B8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_WRITE_PERMISSION_11 0x1BF815B8u

//! Register Reset Value
#define TDM1R_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_ADDR_MATCH_12 Register TDM1R_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ADDR_MATCH_12 0x815C0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ADDR_MATCH_12 0x1BF815C0u

//! Register Reset Value
#define TDM1R_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1R_PM_REQ_INFO_PERMISSION_12 Register TDM1R_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TDM1R_PM_REQ_INFO_PERMISSION_12 0x815C8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_REQ_INFO_PERMISSION_12 0x1BF815C8u

//! Register Reset Value
#define TDM1R_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_READ_PERMISSION_12 Register TDM1R_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TDM1R_PM_READ_PERMISSION_12 0x815D0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_READ_PERMISSION_12 0x1BF815D0u

//! Register Reset Value
#define TDM1R_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_WRITE_PERMISSION_12 Register TDM1R_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TDM1R_PM_WRITE_PERMISSION_12 0x815D8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_WRITE_PERMISSION_12 0x1BF815D8u

//! Register Reset Value
#define TDM1R_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_ADDR_MATCH_13 Register TDM1R_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ADDR_MATCH_13 0x815E0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ADDR_MATCH_13 0x1BF815E0u

//! Register Reset Value
#define TDM1R_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1R_PM_REQ_INFO_PERMISSION_13 Register TDM1R_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TDM1R_PM_REQ_INFO_PERMISSION_13 0x815E8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_REQ_INFO_PERMISSION_13 0x1BF815E8u

//! Register Reset Value
#define TDM1R_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_READ_PERMISSION_13 Register TDM1R_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TDM1R_PM_READ_PERMISSION_13 0x815F0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_READ_PERMISSION_13 0x1BF815F0u

//! Register Reset Value
#define TDM1R_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_WRITE_PERMISSION_13 Register TDM1R_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TDM1R_PM_WRITE_PERMISSION_13 0x815F8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_WRITE_PERMISSION_13 0x1BF815F8u

//! Register Reset Value
#define TDM1R_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_ADDR_MATCH_14 Register TDM1R_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ADDR_MATCH_14 0x81600
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ADDR_MATCH_14 0x1BF81600u

//! Register Reset Value
#define TDM1R_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1R_PM_REQ_INFO_PERMISSION_14 Register TDM1R_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TDM1R_PM_REQ_INFO_PERMISSION_14 0x81608
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_REQ_INFO_PERMISSION_14 0x1BF81608u

//! Register Reset Value
#define TDM1R_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_READ_PERMISSION_14 Register TDM1R_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TDM1R_PM_READ_PERMISSION_14 0x81610
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_READ_PERMISSION_14 0x1BF81610u

//! Register Reset Value
#define TDM1R_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_WRITE_PERMISSION_14 Register TDM1R_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TDM1R_PM_WRITE_PERMISSION_14 0x81618
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_WRITE_PERMISSION_14 0x1BF81618u

//! Register Reset Value
#define TDM1R_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_ADDR_MATCH_15 Register TDM1R_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TDM1R_PM_ADDR_MATCH_15 0x81620
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_ADDR_MATCH_15 0x1BF81620u

//! Register Reset Value
#define TDM1R_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1R_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TDM1R_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1R_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1R_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1R_PM_REQ_INFO_PERMISSION_15 Register TDM1R_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TDM1R_PM_REQ_INFO_PERMISSION_15 0x81628
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_REQ_INFO_PERMISSION_15 0x1BF81628u

//! Register Reset Value
#define TDM1R_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1R_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_READ_PERMISSION_15 Register TDM1R_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TDM1R_PM_READ_PERMISSION_15 0x81630
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_READ_PERMISSION_15 0x1BF81630u

//! Register Reset Value
#define TDM1R_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1R_PM_WRITE_PERMISSION_15 Register TDM1R_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TDM1R_PM_WRITE_PERMISSION_15 0x81638
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1R_PM_WRITE_PERMISSION_15 0x1BF81638u

//! Register Reset Value
#define TDM1R_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1R_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_ERROR_LOG Register TDM1T_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ERROR_LOG 0x81820
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ERROR_LOG 0x1BF81820u

//! Register Reset Value
#define TDM1T_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TDM1T_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TDM1T_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TDM1T_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TDM1T_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TDM1T_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TDM1T_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TDM1T_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TDM1T_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TDM1T_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TDM1T_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TDM1T_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TDM1T_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TDM1T_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TDM1T_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TDM1T_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TDM1T_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TDM1T_PM_CONTROL Register TDM1T_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TDM1T_PM_CONTROL 0x81828
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_CONTROL 0x1BF81828u

//! Register Reset Value
#define TDM1T_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TDM1T_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TDM1T_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TDM1T_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TDM1T_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TDM1T_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TDM1T_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TDM1T_PM_ERROR_CLEAR_SINGLE Register TDM1T_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ERROR_CLEAR_SINGLE 0x81830
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ERROR_CLEAR_SINGLE 0x1BF81830u

//! Register Reset Value
#define TDM1T_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TDM1T_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TDM1T_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TDM1T_PM_ERROR_CLEAR_MULTI Register TDM1T_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ERROR_CLEAR_MULTI 0x81838
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ERROR_CLEAR_MULTI 0x1BF81838u

//! Register Reset Value
#define TDM1T_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TDM1T_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TDM1T_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TDM1T_PM_REQ_INFO_PERMISSION_0 Register TDM1T_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TDM1T_PM_REQ_INFO_PERMISSION_0 0x81848
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_REQ_INFO_PERMISSION_0 0x1BF81848u

//! Register Reset Value
#define TDM1T_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_READ_PERMISSION_0 Register TDM1T_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TDM1T_PM_READ_PERMISSION_0 0x81850
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_READ_PERMISSION_0 0x1BF81850u

//! Register Reset Value
#define TDM1T_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TDM1T_PM_WRITE_PERMISSION_0 Register TDM1T_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TDM1T_PM_WRITE_PERMISSION_0 0x81858
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_WRITE_PERMISSION_0 0x1BF81858u

//! Register Reset Value
#define TDM1T_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TDM1T_PM_ADDR_MATCH_1 Register TDM1T_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ADDR_MATCH_1 0x81860
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ADDR_MATCH_1 0x1BF81860u

//! Register Reset Value
#define TDM1T_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1T_PM_REQ_INFO_PERMISSION_1 Register TDM1T_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TDM1T_PM_REQ_INFO_PERMISSION_1 0x81868
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_REQ_INFO_PERMISSION_1 0x1BF81868u

//! Register Reset Value
#define TDM1T_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_READ_PERMISSION_1 Register TDM1T_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TDM1T_PM_READ_PERMISSION_1 0x81870
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_READ_PERMISSION_1 0x1BF81870u

//! Register Reset Value
#define TDM1T_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_WRITE_PERMISSION_1 Register TDM1T_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TDM1T_PM_WRITE_PERMISSION_1 0x81878
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_WRITE_PERMISSION_1 0x1BF81878u

//! Register Reset Value
#define TDM1T_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_ADDR_MATCH_2 Register TDM1T_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ADDR_MATCH_2 0x81880
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ADDR_MATCH_2 0x1BF81880u

//! Register Reset Value
#define TDM1T_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1T_PM_REQ_INFO_PERMISSION_2 Register TDM1T_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TDM1T_PM_REQ_INFO_PERMISSION_2 0x81888
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_REQ_INFO_PERMISSION_2 0x1BF81888u

//! Register Reset Value
#define TDM1T_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_READ_PERMISSION_2 Register TDM1T_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TDM1T_PM_READ_PERMISSION_2 0x81890
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_READ_PERMISSION_2 0x1BF81890u

//! Register Reset Value
#define TDM1T_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_WRITE_PERMISSION_2 Register TDM1T_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TDM1T_PM_WRITE_PERMISSION_2 0x81898
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_WRITE_PERMISSION_2 0x1BF81898u

//! Register Reset Value
#define TDM1T_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_ADDR_MATCH_3 Register TDM1T_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ADDR_MATCH_3 0x818A0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ADDR_MATCH_3 0x1BF818A0u

//! Register Reset Value
#define TDM1T_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1T_PM_REQ_INFO_PERMISSION_3 Register TDM1T_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TDM1T_PM_REQ_INFO_PERMISSION_3 0x818A8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_REQ_INFO_PERMISSION_3 0x1BF818A8u

//! Register Reset Value
#define TDM1T_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_READ_PERMISSION_3 Register TDM1T_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TDM1T_PM_READ_PERMISSION_3 0x818B0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_READ_PERMISSION_3 0x1BF818B0u

//! Register Reset Value
#define TDM1T_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_WRITE_PERMISSION_3 Register TDM1T_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TDM1T_PM_WRITE_PERMISSION_3 0x818B8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_WRITE_PERMISSION_3 0x1BF818B8u

//! Register Reset Value
#define TDM1T_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_ADDR_MATCH_4 Register TDM1T_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ADDR_MATCH_4 0x818C0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ADDR_MATCH_4 0x1BF818C0u

//! Register Reset Value
#define TDM1T_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1T_PM_REQ_INFO_PERMISSION_4 Register TDM1T_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TDM1T_PM_REQ_INFO_PERMISSION_4 0x818C8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_REQ_INFO_PERMISSION_4 0x1BF818C8u

//! Register Reset Value
#define TDM1T_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_READ_PERMISSION_4 Register TDM1T_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TDM1T_PM_READ_PERMISSION_4 0x818D0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_READ_PERMISSION_4 0x1BF818D0u

//! Register Reset Value
#define TDM1T_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_WRITE_PERMISSION_4 Register TDM1T_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TDM1T_PM_WRITE_PERMISSION_4 0x818D8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_WRITE_PERMISSION_4 0x1BF818D8u

//! Register Reset Value
#define TDM1T_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_ADDR_MATCH_5 Register TDM1T_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ADDR_MATCH_5 0x818E0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ADDR_MATCH_5 0x1BF818E0u

//! Register Reset Value
#define TDM1T_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1T_PM_REQ_INFO_PERMISSION_5 Register TDM1T_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TDM1T_PM_REQ_INFO_PERMISSION_5 0x818E8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_REQ_INFO_PERMISSION_5 0x1BF818E8u

//! Register Reset Value
#define TDM1T_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_READ_PERMISSION_5 Register TDM1T_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TDM1T_PM_READ_PERMISSION_5 0x818F0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_READ_PERMISSION_5 0x1BF818F0u

//! Register Reset Value
#define TDM1T_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_WRITE_PERMISSION_5 Register TDM1T_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TDM1T_PM_WRITE_PERMISSION_5 0x818F8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_WRITE_PERMISSION_5 0x1BF818F8u

//! Register Reset Value
#define TDM1T_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_ADDR_MATCH_6 Register TDM1T_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ADDR_MATCH_6 0x81900
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ADDR_MATCH_6 0x1BF81900u

//! Register Reset Value
#define TDM1T_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1T_PM_REQ_INFO_PERMISSION_6 Register TDM1T_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TDM1T_PM_REQ_INFO_PERMISSION_6 0x81908
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_REQ_INFO_PERMISSION_6 0x1BF81908u

//! Register Reset Value
#define TDM1T_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_READ_PERMISSION_6 Register TDM1T_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TDM1T_PM_READ_PERMISSION_6 0x81910
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_READ_PERMISSION_6 0x1BF81910u

//! Register Reset Value
#define TDM1T_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_WRITE_PERMISSION_6 Register TDM1T_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TDM1T_PM_WRITE_PERMISSION_6 0x81918
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_WRITE_PERMISSION_6 0x1BF81918u

//! Register Reset Value
#define TDM1T_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_ADDR_MATCH_7 Register TDM1T_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ADDR_MATCH_7 0x81920
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ADDR_MATCH_7 0x1BF81920u

//! Register Reset Value
#define TDM1T_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1T_PM_REQ_INFO_PERMISSION_7 Register TDM1T_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TDM1T_PM_REQ_INFO_PERMISSION_7 0x81928
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_REQ_INFO_PERMISSION_7 0x1BF81928u

//! Register Reset Value
#define TDM1T_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_READ_PERMISSION_7 Register TDM1T_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TDM1T_PM_READ_PERMISSION_7 0x81930
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_READ_PERMISSION_7 0x1BF81930u

//! Register Reset Value
#define TDM1T_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_WRITE_PERMISSION_7 Register TDM1T_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TDM1T_PM_WRITE_PERMISSION_7 0x81938
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_WRITE_PERMISSION_7 0x1BF81938u

//! Register Reset Value
#define TDM1T_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_ADDR_MATCH_8 Register TDM1T_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ADDR_MATCH_8 0x81940
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ADDR_MATCH_8 0x1BF81940u

//! Register Reset Value
#define TDM1T_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1T_PM_REQ_INFO_PERMISSION_8 Register TDM1T_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TDM1T_PM_REQ_INFO_PERMISSION_8 0x81948
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_REQ_INFO_PERMISSION_8 0x1BF81948u

//! Register Reset Value
#define TDM1T_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_READ_PERMISSION_8 Register TDM1T_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TDM1T_PM_READ_PERMISSION_8 0x81950
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_READ_PERMISSION_8 0x1BF81950u

//! Register Reset Value
#define TDM1T_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_WRITE_PERMISSION_8 Register TDM1T_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TDM1T_PM_WRITE_PERMISSION_8 0x81958
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_WRITE_PERMISSION_8 0x1BF81958u

//! Register Reset Value
#define TDM1T_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_ADDR_MATCH_9 Register TDM1T_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ADDR_MATCH_9 0x81960
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ADDR_MATCH_9 0x1BF81960u

//! Register Reset Value
#define TDM1T_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1T_PM_REQ_INFO_PERMISSION_9 Register TDM1T_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TDM1T_PM_REQ_INFO_PERMISSION_9 0x81968
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_REQ_INFO_PERMISSION_9 0x1BF81968u

//! Register Reset Value
#define TDM1T_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_READ_PERMISSION_9 Register TDM1T_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TDM1T_PM_READ_PERMISSION_9 0x81970
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_READ_PERMISSION_9 0x1BF81970u

//! Register Reset Value
#define TDM1T_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_WRITE_PERMISSION_9 Register TDM1T_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TDM1T_PM_WRITE_PERMISSION_9 0x81978
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_WRITE_PERMISSION_9 0x1BF81978u

//! Register Reset Value
#define TDM1T_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_ADDR_MATCH_10 Register TDM1T_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ADDR_MATCH_10 0x81980
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ADDR_MATCH_10 0x1BF81980u

//! Register Reset Value
#define TDM1T_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1T_PM_REQ_INFO_PERMISSION_10 Register TDM1T_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TDM1T_PM_REQ_INFO_PERMISSION_10 0x81988
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_REQ_INFO_PERMISSION_10 0x1BF81988u

//! Register Reset Value
#define TDM1T_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_READ_PERMISSION_10 Register TDM1T_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TDM1T_PM_READ_PERMISSION_10 0x81990
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_READ_PERMISSION_10 0x1BF81990u

//! Register Reset Value
#define TDM1T_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_WRITE_PERMISSION_10 Register TDM1T_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TDM1T_PM_WRITE_PERMISSION_10 0x81998
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_WRITE_PERMISSION_10 0x1BF81998u

//! Register Reset Value
#define TDM1T_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_ADDR_MATCH_11 Register TDM1T_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ADDR_MATCH_11 0x819A0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ADDR_MATCH_11 0x1BF819A0u

//! Register Reset Value
#define TDM1T_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1T_PM_REQ_INFO_PERMISSION_11 Register TDM1T_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TDM1T_PM_REQ_INFO_PERMISSION_11 0x819A8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_REQ_INFO_PERMISSION_11 0x1BF819A8u

//! Register Reset Value
#define TDM1T_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_READ_PERMISSION_11 Register TDM1T_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TDM1T_PM_READ_PERMISSION_11 0x819B0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_READ_PERMISSION_11 0x1BF819B0u

//! Register Reset Value
#define TDM1T_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_WRITE_PERMISSION_11 Register TDM1T_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TDM1T_PM_WRITE_PERMISSION_11 0x819B8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_WRITE_PERMISSION_11 0x1BF819B8u

//! Register Reset Value
#define TDM1T_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_ADDR_MATCH_12 Register TDM1T_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ADDR_MATCH_12 0x819C0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ADDR_MATCH_12 0x1BF819C0u

//! Register Reset Value
#define TDM1T_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1T_PM_REQ_INFO_PERMISSION_12 Register TDM1T_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TDM1T_PM_REQ_INFO_PERMISSION_12 0x819C8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_REQ_INFO_PERMISSION_12 0x1BF819C8u

//! Register Reset Value
#define TDM1T_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_READ_PERMISSION_12 Register TDM1T_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TDM1T_PM_READ_PERMISSION_12 0x819D0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_READ_PERMISSION_12 0x1BF819D0u

//! Register Reset Value
#define TDM1T_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_WRITE_PERMISSION_12 Register TDM1T_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TDM1T_PM_WRITE_PERMISSION_12 0x819D8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_WRITE_PERMISSION_12 0x1BF819D8u

//! Register Reset Value
#define TDM1T_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_ADDR_MATCH_13 Register TDM1T_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ADDR_MATCH_13 0x819E0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ADDR_MATCH_13 0x1BF819E0u

//! Register Reset Value
#define TDM1T_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1T_PM_REQ_INFO_PERMISSION_13 Register TDM1T_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TDM1T_PM_REQ_INFO_PERMISSION_13 0x819E8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_REQ_INFO_PERMISSION_13 0x1BF819E8u

//! Register Reset Value
#define TDM1T_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_READ_PERMISSION_13 Register TDM1T_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TDM1T_PM_READ_PERMISSION_13 0x819F0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_READ_PERMISSION_13 0x1BF819F0u

//! Register Reset Value
#define TDM1T_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_WRITE_PERMISSION_13 Register TDM1T_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TDM1T_PM_WRITE_PERMISSION_13 0x819F8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_WRITE_PERMISSION_13 0x1BF819F8u

//! Register Reset Value
#define TDM1T_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_ADDR_MATCH_14 Register TDM1T_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ADDR_MATCH_14 0x81A00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ADDR_MATCH_14 0x1BF81A00u

//! Register Reset Value
#define TDM1T_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1T_PM_REQ_INFO_PERMISSION_14 Register TDM1T_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TDM1T_PM_REQ_INFO_PERMISSION_14 0x81A08
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_REQ_INFO_PERMISSION_14 0x1BF81A08u

//! Register Reset Value
#define TDM1T_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_READ_PERMISSION_14 Register TDM1T_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TDM1T_PM_READ_PERMISSION_14 0x81A10
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_READ_PERMISSION_14 0x1BF81A10u

//! Register Reset Value
#define TDM1T_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_WRITE_PERMISSION_14 Register TDM1T_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TDM1T_PM_WRITE_PERMISSION_14 0x81A18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_WRITE_PERMISSION_14 0x1BF81A18u

//! Register Reset Value
#define TDM1T_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_ADDR_MATCH_15 Register TDM1T_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TDM1T_PM_ADDR_MATCH_15 0x81A20
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_ADDR_MATCH_15 0x1BF81A20u

//! Register Reset Value
#define TDM1T_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TDM1T_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TDM1T_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TDM1T_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TDM1T_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TDM1T_PM_REQ_INFO_PERMISSION_15 Register TDM1T_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TDM1T_PM_REQ_INFO_PERMISSION_15 0x81A28
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_REQ_INFO_PERMISSION_15 0x1BF81A28u

//! Register Reset Value
#define TDM1T_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TDM1T_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_READ_PERMISSION_15 Register TDM1T_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TDM1T_PM_READ_PERMISSION_15 0x81A30
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_READ_PERMISSION_15 0x1BF81A30u

//! Register Reset Value
#define TDM1T_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TDM1T_PM_WRITE_PERMISSION_15 Register TDM1T_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TDM1T_PM_WRITE_PERMISSION_15 0x81A38
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TDM1T_PM_WRITE_PERMISSION_15 0x1BF81A38u

//! Register Reset Value
#define TDM1T_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TDM1T_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_ERROR_LOG Register TGWPR_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ERROR_LOG 0x81C20
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ERROR_LOG 0x1BF81C20u

//! Register Reset Value
#define TGWPR_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TGWPR_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TGWPR_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TGWPR_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TGWPR_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TGWPR_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TGWPR_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TGWPR_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TGWPR_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TGWPR_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TGWPR_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TGWPR_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TGWPR_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TGWPR_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TGWPR_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TGWPR_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TGWPR_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TGWPR_PM_CONTROL Register TGWPR_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TGWPR_PM_CONTROL 0x81C28
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_CONTROL 0x1BF81C28u

//! Register Reset Value
#define TGWPR_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TGWPR_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TGWPR_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TGWPR_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TGWPR_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TGWPR_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TGWPR_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TGWPR_PM_ERROR_CLEAR_SINGLE Register TGWPR_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ERROR_CLEAR_SINGLE 0x81C30
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ERROR_CLEAR_SINGLE 0x1BF81C30u

//! Register Reset Value
#define TGWPR_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TGWPR_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TGWPR_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TGWPR_PM_ERROR_CLEAR_MULTI Register TGWPR_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ERROR_CLEAR_MULTI 0x81C38
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ERROR_CLEAR_MULTI 0x1BF81C38u

//! Register Reset Value
#define TGWPR_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TGWPR_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TGWPR_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TGWPR_PM_REQ_INFO_PERMISSION_0 Register TGWPR_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TGWPR_PM_REQ_INFO_PERMISSION_0 0x81C48
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_REQ_INFO_PERMISSION_0 0x1BF81C48u

//! Register Reset Value
#define TGWPR_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_READ_PERMISSION_0 Register TGWPR_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TGWPR_PM_READ_PERMISSION_0 0x81C50
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_READ_PERMISSION_0 0x1BF81C50u

//! Register Reset Value
#define TGWPR_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TGWPR_PM_WRITE_PERMISSION_0 Register TGWPR_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TGWPR_PM_WRITE_PERMISSION_0 0x81C58
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_WRITE_PERMISSION_0 0x1BF81C58u

//! Register Reset Value
#define TGWPR_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1Fu

//! @}

//! \defgroup TGWPR_PM_ADDR_MATCH_1 Register TGWPR_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ADDR_MATCH_1 0x81C60
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ADDR_MATCH_1 0x1BF81C60u

//! Register Reset Value
#define TGWPR_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPR_PM_REQ_INFO_PERMISSION_1 Register TGWPR_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TGWPR_PM_REQ_INFO_PERMISSION_1 0x81C68
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_REQ_INFO_PERMISSION_1 0x1BF81C68u

//! Register Reset Value
#define TGWPR_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_READ_PERMISSION_1 Register TGWPR_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TGWPR_PM_READ_PERMISSION_1 0x81C70
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_READ_PERMISSION_1 0x1BF81C70u

//! Register Reset Value
#define TGWPR_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_WRITE_PERMISSION_1 Register TGWPR_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TGWPR_PM_WRITE_PERMISSION_1 0x81C78
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_WRITE_PERMISSION_1 0x1BF81C78u

//! Register Reset Value
#define TGWPR_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_ADDR_MATCH_2 Register TGWPR_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ADDR_MATCH_2 0x81C80
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ADDR_MATCH_2 0x1BF81C80u

//! Register Reset Value
#define TGWPR_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPR_PM_REQ_INFO_PERMISSION_2 Register TGWPR_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TGWPR_PM_REQ_INFO_PERMISSION_2 0x81C88
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_REQ_INFO_PERMISSION_2 0x1BF81C88u

//! Register Reset Value
#define TGWPR_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_READ_PERMISSION_2 Register TGWPR_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TGWPR_PM_READ_PERMISSION_2 0x81C90
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_READ_PERMISSION_2 0x1BF81C90u

//! Register Reset Value
#define TGWPR_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_WRITE_PERMISSION_2 Register TGWPR_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TGWPR_PM_WRITE_PERMISSION_2 0x81C98
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_WRITE_PERMISSION_2 0x1BF81C98u

//! Register Reset Value
#define TGWPR_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_ADDR_MATCH_3 Register TGWPR_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ADDR_MATCH_3 0x81CA0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ADDR_MATCH_3 0x1BF81CA0u

//! Register Reset Value
#define TGWPR_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPR_PM_REQ_INFO_PERMISSION_3 Register TGWPR_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TGWPR_PM_REQ_INFO_PERMISSION_3 0x81CA8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_REQ_INFO_PERMISSION_3 0x1BF81CA8u

//! Register Reset Value
#define TGWPR_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_READ_PERMISSION_3 Register TGWPR_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TGWPR_PM_READ_PERMISSION_3 0x81CB0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_READ_PERMISSION_3 0x1BF81CB0u

//! Register Reset Value
#define TGWPR_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_WRITE_PERMISSION_3 Register TGWPR_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TGWPR_PM_WRITE_PERMISSION_3 0x81CB8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_WRITE_PERMISSION_3 0x1BF81CB8u

//! Register Reset Value
#define TGWPR_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_ADDR_MATCH_4 Register TGWPR_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ADDR_MATCH_4 0x81CC0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ADDR_MATCH_4 0x1BF81CC0u

//! Register Reset Value
#define TGWPR_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPR_PM_REQ_INFO_PERMISSION_4 Register TGWPR_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TGWPR_PM_REQ_INFO_PERMISSION_4 0x81CC8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_REQ_INFO_PERMISSION_4 0x1BF81CC8u

//! Register Reset Value
#define TGWPR_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_READ_PERMISSION_4 Register TGWPR_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TGWPR_PM_READ_PERMISSION_4 0x81CD0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_READ_PERMISSION_4 0x1BF81CD0u

//! Register Reset Value
#define TGWPR_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_WRITE_PERMISSION_4 Register TGWPR_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TGWPR_PM_WRITE_PERMISSION_4 0x81CD8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_WRITE_PERMISSION_4 0x1BF81CD8u

//! Register Reset Value
#define TGWPR_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_ADDR_MATCH_5 Register TGWPR_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ADDR_MATCH_5 0x81CE0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ADDR_MATCH_5 0x1BF81CE0u

//! Register Reset Value
#define TGWPR_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPR_PM_REQ_INFO_PERMISSION_5 Register TGWPR_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TGWPR_PM_REQ_INFO_PERMISSION_5 0x81CE8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_REQ_INFO_PERMISSION_5 0x1BF81CE8u

//! Register Reset Value
#define TGWPR_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_READ_PERMISSION_5 Register TGWPR_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TGWPR_PM_READ_PERMISSION_5 0x81CF0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_READ_PERMISSION_5 0x1BF81CF0u

//! Register Reset Value
#define TGWPR_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_WRITE_PERMISSION_5 Register TGWPR_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TGWPR_PM_WRITE_PERMISSION_5 0x81CF8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_WRITE_PERMISSION_5 0x1BF81CF8u

//! Register Reset Value
#define TGWPR_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_ADDR_MATCH_6 Register TGWPR_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ADDR_MATCH_6 0x81D00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ADDR_MATCH_6 0x1BF81D00u

//! Register Reset Value
#define TGWPR_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPR_PM_REQ_INFO_PERMISSION_6 Register TGWPR_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TGWPR_PM_REQ_INFO_PERMISSION_6 0x81D08
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_REQ_INFO_PERMISSION_6 0x1BF81D08u

//! Register Reset Value
#define TGWPR_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_READ_PERMISSION_6 Register TGWPR_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TGWPR_PM_READ_PERMISSION_6 0x81D10
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_READ_PERMISSION_6 0x1BF81D10u

//! Register Reset Value
#define TGWPR_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_WRITE_PERMISSION_6 Register TGWPR_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TGWPR_PM_WRITE_PERMISSION_6 0x81D18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_WRITE_PERMISSION_6 0x1BF81D18u

//! Register Reset Value
#define TGWPR_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_ADDR_MATCH_7 Register TGWPR_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ADDR_MATCH_7 0x81D20
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ADDR_MATCH_7 0x1BF81D20u

//! Register Reset Value
#define TGWPR_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPR_PM_REQ_INFO_PERMISSION_7 Register TGWPR_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TGWPR_PM_REQ_INFO_PERMISSION_7 0x81D28
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_REQ_INFO_PERMISSION_7 0x1BF81D28u

//! Register Reset Value
#define TGWPR_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_READ_PERMISSION_7 Register TGWPR_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TGWPR_PM_READ_PERMISSION_7 0x81D30
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_READ_PERMISSION_7 0x1BF81D30u

//! Register Reset Value
#define TGWPR_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_WRITE_PERMISSION_7 Register TGWPR_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TGWPR_PM_WRITE_PERMISSION_7 0x81D38
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_WRITE_PERMISSION_7 0x1BF81D38u

//! Register Reset Value
#define TGWPR_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_ADDR_MATCH_8 Register TGWPR_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ADDR_MATCH_8 0x81D40
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ADDR_MATCH_8 0x1BF81D40u

//! Register Reset Value
#define TGWPR_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPR_PM_REQ_INFO_PERMISSION_8 Register TGWPR_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TGWPR_PM_REQ_INFO_PERMISSION_8 0x81D48
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_REQ_INFO_PERMISSION_8 0x1BF81D48u

//! Register Reset Value
#define TGWPR_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_READ_PERMISSION_8 Register TGWPR_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TGWPR_PM_READ_PERMISSION_8 0x81D50
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_READ_PERMISSION_8 0x1BF81D50u

//! Register Reset Value
#define TGWPR_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_WRITE_PERMISSION_8 Register TGWPR_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TGWPR_PM_WRITE_PERMISSION_8 0x81D58
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_WRITE_PERMISSION_8 0x1BF81D58u

//! Register Reset Value
#define TGWPR_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_ADDR_MATCH_9 Register TGWPR_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ADDR_MATCH_9 0x81D60
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ADDR_MATCH_9 0x1BF81D60u

//! Register Reset Value
#define TGWPR_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPR_PM_REQ_INFO_PERMISSION_9 Register TGWPR_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TGWPR_PM_REQ_INFO_PERMISSION_9 0x81D68
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_REQ_INFO_PERMISSION_9 0x1BF81D68u

//! Register Reset Value
#define TGWPR_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_READ_PERMISSION_9 Register TGWPR_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TGWPR_PM_READ_PERMISSION_9 0x81D70
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_READ_PERMISSION_9 0x1BF81D70u

//! Register Reset Value
#define TGWPR_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_WRITE_PERMISSION_9 Register TGWPR_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TGWPR_PM_WRITE_PERMISSION_9 0x81D78
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_WRITE_PERMISSION_9 0x1BF81D78u

//! Register Reset Value
#define TGWPR_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_ADDR_MATCH_10 Register TGWPR_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ADDR_MATCH_10 0x81D80
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ADDR_MATCH_10 0x1BF81D80u

//! Register Reset Value
#define TGWPR_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPR_PM_REQ_INFO_PERMISSION_10 Register TGWPR_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TGWPR_PM_REQ_INFO_PERMISSION_10 0x81D88
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_REQ_INFO_PERMISSION_10 0x1BF81D88u

//! Register Reset Value
#define TGWPR_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_READ_PERMISSION_10 Register TGWPR_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TGWPR_PM_READ_PERMISSION_10 0x81D90
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_READ_PERMISSION_10 0x1BF81D90u

//! Register Reset Value
#define TGWPR_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_WRITE_PERMISSION_10 Register TGWPR_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TGWPR_PM_WRITE_PERMISSION_10 0x81D98
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_WRITE_PERMISSION_10 0x1BF81D98u

//! Register Reset Value
#define TGWPR_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_ADDR_MATCH_11 Register TGWPR_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ADDR_MATCH_11 0x81DA0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ADDR_MATCH_11 0x1BF81DA0u

//! Register Reset Value
#define TGWPR_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPR_PM_REQ_INFO_PERMISSION_11 Register TGWPR_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TGWPR_PM_REQ_INFO_PERMISSION_11 0x81DA8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_REQ_INFO_PERMISSION_11 0x1BF81DA8u

//! Register Reset Value
#define TGWPR_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_READ_PERMISSION_11 Register TGWPR_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TGWPR_PM_READ_PERMISSION_11 0x81DB0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_READ_PERMISSION_11 0x1BF81DB0u

//! Register Reset Value
#define TGWPR_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_WRITE_PERMISSION_11 Register TGWPR_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TGWPR_PM_WRITE_PERMISSION_11 0x81DB8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_WRITE_PERMISSION_11 0x1BF81DB8u

//! Register Reset Value
#define TGWPR_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_ADDR_MATCH_12 Register TGWPR_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ADDR_MATCH_12 0x81DC0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ADDR_MATCH_12 0x1BF81DC0u

//! Register Reset Value
#define TGWPR_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPR_PM_REQ_INFO_PERMISSION_12 Register TGWPR_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TGWPR_PM_REQ_INFO_PERMISSION_12 0x81DC8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_REQ_INFO_PERMISSION_12 0x1BF81DC8u

//! Register Reset Value
#define TGWPR_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_READ_PERMISSION_12 Register TGWPR_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TGWPR_PM_READ_PERMISSION_12 0x81DD0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_READ_PERMISSION_12 0x1BF81DD0u

//! Register Reset Value
#define TGWPR_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_WRITE_PERMISSION_12 Register TGWPR_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TGWPR_PM_WRITE_PERMISSION_12 0x81DD8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_WRITE_PERMISSION_12 0x1BF81DD8u

//! Register Reset Value
#define TGWPR_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_ADDR_MATCH_13 Register TGWPR_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ADDR_MATCH_13 0x81DE0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ADDR_MATCH_13 0x1BF81DE0u

//! Register Reset Value
#define TGWPR_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPR_PM_REQ_INFO_PERMISSION_13 Register TGWPR_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TGWPR_PM_REQ_INFO_PERMISSION_13 0x81DE8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_REQ_INFO_PERMISSION_13 0x1BF81DE8u

//! Register Reset Value
#define TGWPR_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_READ_PERMISSION_13 Register TGWPR_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TGWPR_PM_READ_PERMISSION_13 0x81DF0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_READ_PERMISSION_13 0x1BF81DF0u

//! Register Reset Value
#define TGWPR_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_WRITE_PERMISSION_13 Register TGWPR_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TGWPR_PM_WRITE_PERMISSION_13 0x81DF8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_WRITE_PERMISSION_13 0x1BF81DF8u

//! Register Reset Value
#define TGWPR_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_ADDR_MATCH_14 Register TGWPR_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ADDR_MATCH_14 0x81E00
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ADDR_MATCH_14 0x1BF81E00u

//! Register Reset Value
#define TGWPR_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPR_PM_REQ_INFO_PERMISSION_14 Register TGWPR_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TGWPR_PM_REQ_INFO_PERMISSION_14 0x81E08
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_REQ_INFO_PERMISSION_14 0x1BF81E08u

//! Register Reset Value
#define TGWPR_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_READ_PERMISSION_14 Register TGWPR_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TGWPR_PM_READ_PERMISSION_14 0x81E10
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_READ_PERMISSION_14 0x1BF81E10u

//! Register Reset Value
#define TGWPR_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_WRITE_PERMISSION_14 Register TGWPR_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TGWPR_PM_WRITE_PERMISSION_14 0x81E18
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_WRITE_PERMISSION_14 0x1BF81E18u

//! Register Reset Value
#define TGWPR_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_ADDR_MATCH_15 Register TGWPR_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TGWPR_PM_ADDR_MATCH_15 0x81E20
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_ADDR_MATCH_15 0x1BF81E20u

//! Register Reset Value
#define TGWPR_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TGWPR_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TGWPR_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TGWPR_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TGWPR_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TGWPR_PM_REQ_INFO_PERMISSION_15 Register TGWPR_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TGWPR_PM_REQ_INFO_PERMISSION_15 0x81E28
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_REQ_INFO_PERMISSION_15 0x1BF81E28u

//! Register Reset Value
#define TGWPR_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TGWPR_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_READ_PERMISSION_15 Register TGWPR_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TGWPR_PM_READ_PERMISSION_15 0x81E30
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_READ_PERMISSION_15 0x1BF81E30u

//! Register Reset Value
#define TGWPR_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TGWPR_PM_WRITE_PERMISSION_15 Register TGWPR_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TGWPR_PM_WRITE_PERMISSION_15 0x81E38
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TGWPR_PM_WRITE_PERMISSION_15 0x1BF81E38u

//! Register Reset Value
#define TGWPR_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TGWPR_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_ERROR_LOG Register TLN20_PM_ERROR_LOG - error_log
//! @{

//! Register Offset (relative)
#define TLN20_PM_ERROR_LOG 0x82020
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ERROR_LOG 0x1BF82020u

//! Register Reset Value
#define TLN20_PM_ERROR_LOG_RST 0x0000000000000000u

//! Field CMD - cmd
#define TLN20_PM_ERROR_LOG_CMD_POS 0
//! Field CMD - cmd
#define TLN20_PM_ERROR_LOG_CMD_MASK 0x7u

//! Field REGION - region
#define TLN20_PM_ERROR_LOG_REGION_POS 4
//! Field REGION - region
#define TLN20_PM_ERROR_LOG_REGION_MASK 0xF0u

//! Field INITID - initid
#define TLN20_PM_ERROR_LOG_INITID_POS 8
//! Field INITID - initid
#define TLN20_PM_ERROR_LOG_INITID_MASK 0xFF00u

//! Field REQ_INFO - req_info
#define TLN20_PM_ERROR_LOG_REQ_INFO_POS 16
//! Field REQ_INFO - req_info
#define TLN20_PM_ERROR_LOG_REQ_INFO_MASK 0x1F0000u

//! Field CODE - code
#define TLN20_PM_ERROR_LOG_CODE_POS 24
//! Field CODE - code
#define TLN20_PM_ERROR_LOG_CODE_MASK 0xF000000u

//! Field SECONDARY - secondary
#define TLN20_PM_ERROR_LOG_SECONDARY_POS 30
//! Field SECONDARY - secondary
#define TLN20_PM_ERROR_LOG_SECONDARY_MASK 0x40000000u

//! Field MULTI - multi
#define TLN20_PM_ERROR_LOG_MULTI_POS 31
//! Field MULTI - multi
#define TLN20_PM_ERROR_LOG_MULTI_MASK 0x80000000u

//! Field GROUP - group
#define TLN20_PM_ERROR_LOG_GROUP_POS 32
//! Field GROUP - group
#define TLN20_PM_ERROR_LOG_GROUP_MASK 0x3F00000000u

//! @}

//! \defgroup TLN20_PM_CONTROL Register TLN20_PM_CONTROL - control
//! @{

//! Register Offset (relative)
#define TLN20_PM_CONTROL 0x82028
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_CONTROL 0x1BF82028u

//! Register Reset Value
#define TLN20_PM_CONTROL_RST 0x0000000000000000u

//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TLN20_PM_CONTROL_ERROR_PRIMARY_REP_POS 24
//! Field ERROR_PRIMARY_REP - error_primary_rep
#define TLN20_PM_CONTROL_ERROR_PRIMARY_REP_MASK 0x1000000u

//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TLN20_PM_CONTROL_ERROR_SECONDARY_REP_POS 25
//! Field ERROR_SECONDARY_REP - error_secondary_rep
#define TLN20_PM_CONTROL_ERROR_SECONDARY_REP_MASK 0x2000000u

//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TLN20_PM_CONTROL_SECONDARY_REP_SHADOW_POS 30
//! Field SECONDARY_REP_SHADOW - secondary_rep_shadow
#define TLN20_PM_CONTROL_SECONDARY_REP_SHADOW_MASK 0x40000000u

//! @}

//! \defgroup TLN20_PM_ERROR_CLEAR_SINGLE Register TLN20_PM_ERROR_CLEAR_SINGLE - error_clear_single
//! @{

//! Register Offset (relative)
#define TLN20_PM_ERROR_CLEAR_SINGLE 0x82030
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ERROR_CLEAR_SINGLE 0x1BF82030u

//! Register Reset Value
#define TLN20_PM_ERROR_CLEAR_SINGLE_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TLN20_PM_ERROR_CLEAR_SINGLE_CLEAR_POS 0
//! Field CLEAR - clear
#define TLN20_PM_ERROR_CLEAR_SINGLE_CLEAR_MASK 0x1u

//! @}

//! \defgroup TLN20_PM_ERROR_CLEAR_MULTI Register TLN20_PM_ERROR_CLEAR_MULTI - error_clear_multi
//! @{

//! Register Offset (relative)
#define TLN20_PM_ERROR_CLEAR_MULTI 0x82038
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ERROR_CLEAR_MULTI 0x1BF82038u

//! Register Reset Value
#define TLN20_PM_ERROR_CLEAR_MULTI_RST 0x0000000000000000u

//! Field CLEAR - clear
#define TLN20_PM_ERROR_CLEAR_MULTI_CLEAR_POS 0
//! Field CLEAR - clear
#define TLN20_PM_ERROR_CLEAR_MULTI_CLEAR_MASK 0x1u

//! @}

//! \defgroup TLN20_PM_REQ_INFO_PERMISSION_0 Register TLN20_PM_REQ_INFO_PERMISSION_0 - req_info_permission_0
//! @{

//! Register Offset (relative)
#define TLN20_PM_REQ_INFO_PERMISSION_0 0x82048
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_REQ_INFO_PERMISSION_0 0x1BF82048u

//! Register Reset Value
#define TLN20_PM_REQ_INFO_PERMISSION_0_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_0_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_0_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_READ_PERMISSION_0 Register TLN20_PM_READ_PERMISSION_0 - read_permission_0
//! @{

//! Register Offset (relative)
#define TLN20_PM_READ_PERMISSION_0 0x82050
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_READ_PERMISSION_0 0x1BF82050u

//! Register Reset Value
#define TLN20_PM_READ_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_0_BIT_VECTOR_MASK 0x1u

//! @}

//! \defgroup TLN20_PM_WRITE_PERMISSION_0 Register TLN20_PM_WRITE_PERMISSION_0 - write_permission_0
//! @{

//! Register Offset (relative)
#define TLN20_PM_WRITE_PERMISSION_0 0x82058
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_WRITE_PERMISSION_0 0x1BF82058u

//! Register Reset Value
#define TLN20_PM_WRITE_PERMISSION_0_RST 0x0000000000000001u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_0_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_0_BIT_VECTOR_MASK 0x1u

//! @}

//! \defgroup TLN20_PM_ADDR_MATCH_1 Register TLN20_PM_ADDR_MATCH_1 - addr_match_1
//! @{

//! Register Offset (relative)
#define TLN20_PM_ADDR_MATCH_1 0x82060
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ADDR_MATCH_1 0x1BF82060u

//! Register Reset Value
#define TLN20_PM_ADDR_MATCH_1_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_1_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_1_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_1_SIZE_POS 3
//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_1_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_1_LEVEL_POS 9
//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_1_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_1_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_1_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN20_PM_REQ_INFO_PERMISSION_1 Register TLN20_PM_REQ_INFO_PERMISSION_1 - req_info_permission_1
//! @{

//! Register Offset (relative)
#define TLN20_PM_REQ_INFO_PERMISSION_1 0x82068
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_REQ_INFO_PERMISSION_1 0x1BF82068u

//! Register Reset Value
#define TLN20_PM_REQ_INFO_PERMISSION_1_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_1_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_1_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_READ_PERMISSION_1 Register TLN20_PM_READ_PERMISSION_1 - read_permission_1
//! @{

//! Register Offset (relative)
#define TLN20_PM_READ_PERMISSION_1 0x82070
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_READ_PERMISSION_1 0x1BF82070u

//! Register Reset Value
#define TLN20_PM_READ_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_WRITE_PERMISSION_1 Register TLN20_PM_WRITE_PERMISSION_1 - write_permission_1
//! @{

//! Register Offset (relative)
#define TLN20_PM_WRITE_PERMISSION_1 0x82078
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_WRITE_PERMISSION_1 0x1BF82078u

//! Register Reset Value
#define TLN20_PM_WRITE_PERMISSION_1_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_1_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_1_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_ADDR_MATCH_2 Register TLN20_PM_ADDR_MATCH_2 - addr_match_2
//! @{

//! Register Offset (relative)
#define TLN20_PM_ADDR_MATCH_2 0x82080
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ADDR_MATCH_2 0x1BF82080u

//! Register Reset Value
#define TLN20_PM_ADDR_MATCH_2_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_2_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_2_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_2_SIZE_POS 3
//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_2_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_2_LEVEL_POS 9
//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_2_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_2_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_2_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN20_PM_REQ_INFO_PERMISSION_2 Register TLN20_PM_REQ_INFO_PERMISSION_2 - req_info_permission_2
//! @{

//! Register Offset (relative)
#define TLN20_PM_REQ_INFO_PERMISSION_2 0x82088
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_REQ_INFO_PERMISSION_2 0x1BF82088u

//! Register Reset Value
#define TLN20_PM_REQ_INFO_PERMISSION_2_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_2_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_2_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_READ_PERMISSION_2 Register TLN20_PM_READ_PERMISSION_2 - read_permission_2
//! @{

//! Register Offset (relative)
#define TLN20_PM_READ_PERMISSION_2 0x82090
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_READ_PERMISSION_2 0x1BF82090u

//! Register Reset Value
#define TLN20_PM_READ_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_WRITE_PERMISSION_2 Register TLN20_PM_WRITE_PERMISSION_2 - write_permission_2
//! @{

//! Register Offset (relative)
#define TLN20_PM_WRITE_PERMISSION_2 0x82098
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_WRITE_PERMISSION_2 0x1BF82098u

//! Register Reset Value
#define TLN20_PM_WRITE_PERMISSION_2_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_2_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_2_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_ADDR_MATCH_3 Register TLN20_PM_ADDR_MATCH_3 - addr_match_3
//! @{

//! Register Offset (relative)
#define TLN20_PM_ADDR_MATCH_3 0x820A0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ADDR_MATCH_3 0x1BF820A0u

//! Register Reset Value
#define TLN20_PM_ADDR_MATCH_3_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_3_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_3_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_3_SIZE_POS 3
//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_3_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_3_LEVEL_POS 9
//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_3_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_3_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_3_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN20_PM_REQ_INFO_PERMISSION_3 Register TLN20_PM_REQ_INFO_PERMISSION_3 - req_info_permission_3
//! @{

//! Register Offset (relative)
#define TLN20_PM_REQ_INFO_PERMISSION_3 0x820A8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_REQ_INFO_PERMISSION_3 0x1BF820A8u

//! Register Reset Value
#define TLN20_PM_REQ_INFO_PERMISSION_3_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_3_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_3_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_READ_PERMISSION_3 Register TLN20_PM_READ_PERMISSION_3 - read_permission_3
//! @{

//! Register Offset (relative)
#define TLN20_PM_READ_PERMISSION_3 0x820B0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_READ_PERMISSION_3 0x1BF820B0u

//! Register Reset Value
#define TLN20_PM_READ_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_WRITE_PERMISSION_3 Register TLN20_PM_WRITE_PERMISSION_3 - write_permission_3
//! @{

//! Register Offset (relative)
#define TLN20_PM_WRITE_PERMISSION_3 0x820B8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_WRITE_PERMISSION_3 0x1BF820B8u

//! Register Reset Value
#define TLN20_PM_WRITE_PERMISSION_3_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_3_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_3_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_ADDR_MATCH_4 Register TLN20_PM_ADDR_MATCH_4 - addr_match_4
//! @{

//! Register Offset (relative)
#define TLN20_PM_ADDR_MATCH_4 0x820C0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ADDR_MATCH_4 0x1BF820C0u

//! Register Reset Value
#define TLN20_PM_ADDR_MATCH_4_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_4_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_4_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_4_SIZE_POS 3
//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_4_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_4_LEVEL_POS 9
//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_4_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_4_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_4_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN20_PM_REQ_INFO_PERMISSION_4 Register TLN20_PM_REQ_INFO_PERMISSION_4 - req_info_permission_4
//! @{

//! Register Offset (relative)
#define TLN20_PM_REQ_INFO_PERMISSION_4 0x820C8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_REQ_INFO_PERMISSION_4 0x1BF820C8u

//! Register Reset Value
#define TLN20_PM_REQ_INFO_PERMISSION_4_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_4_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_4_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_READ_PERMISSION_4 Register TLN20_PM_READ_PERMISSION_4 - read_permission_4
//! @{

//! Register Offset (relative)
#define TLN20_PM_READ_PERMISSION_4 0x820D0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_READ_PERMISSION_4 0x1BF820D0u

//! Register Reset Value
#define TLN20_PM_READ_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_WRITE_PERMISSION_4 Register TLN20_PM_WRITE_PERMISSION_4 - write_permission_4
//! @{

//! Register Offset (relative)
#define TLN20_PM_WRITE_PERMISSION_4 0x820D8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_WRITE_PERMISSION_4 0x1BF820D8u

//! Register Reset Value
#define TLN20_PM_WRITE_PERMISSION_4_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_4_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_4_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_ADDR_MATCH_5 Register TLN20_PM_ADDR_MATCH_5 - addr_match_5
//! @{

//! Register Offset (relative)
#define TLN20_PM_ADDR_MATCH_5 0x820E0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ADDR_MATCH_5 0x1BF820E0u

//! Register Reset Value
#define TLN20_PM_ADDR_MATCH_5_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_5_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_5_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_5_SIZE_POS 3
//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_5_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_5_LEVEL_POS 9
//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_5_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_5_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_5_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN20_PM_REQ_INFO_PERMISSION_5 Register TLN20_PM_REQ_INFO_PERMISSION_5 - req_info_permission_5
//! @{

//! Register Offset (relative)
#define TLN20_PM_REQ_INFO_PERMISSION_5 0x820E8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_REQ_INFO_PERMISSION_5 0x1BF820E8u

//! Register Reset Value
#define TLN20_PM_REQ_INFO_PERMISSION_5_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_5_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_5_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_READ_PERMISSION_5 Register TLN20_PM_READ_PERMISSION_5 - read_permission_5
//! @{

//! Register Offset (relative)
#define TLN20_PM_READ_PERMISSION_5 0x820F0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_READ_PERMISSION_5 0x1BF820F0u

//! Register Reset Value
#define TLN20_PM_READ_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_WRITE_PERMISSION_5 Register TLN20_PM_WRITE_PERMISSION_5 - write_permission_5
//! @{

//! Register Offset (relative)
#define TLN20_PM_WRITE_PERMISSION_5 0x820F8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_WRITE_PERMISSION_5 0x1BF820F8u

//! Register Reset Value
#define TLN20_PM_WRITE_PERMISSION_5_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_5_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_5_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_ADDR_MATCH_6 Register TLN20_PM_ADDR_MATCH_6 - addr_match_6
//! @{

//! Register Offset (relative)
#define TLN20_PM_ADDR_MATCH_6 0x82100
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ADDR_MATCH_6 0x1BF82100u

//! Register Reset Value
#define TLN20_PM_ADDR_MATCH_6_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_6_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_6_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_6_SIZE_POS 3
//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_6_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_6_LEVEL_POS 9
//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_6_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_6_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_6_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN20_PM_REQ_INFO_PERMISSION_6 Register TLN20_PM_REQ_INFO_PERMISSION_6 - req_info_permission_6
//! @{

//! Register Offset (relative)
#define TLN20_PM_REQ_INFO_PERMISSION_6 0x82108
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_REQ_INFO_PERMISSION_6 0x1BF82108u

//! Register Reset Value
#define TLN20_PM_REQ_INFO_PERMISSION_6_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_6_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_6_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_READ_PERMISSION_6 Register TLN20_PM_READ_PERMISSION_6 - read_permission_6
//! @{

//! Register Offset (relative)
#define TLN20_PM_READ_PERMISSION_6 0x82110
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_READ_PERMISSION_6 0x1BF82110u

//! Register Reset Value
#define TLN20_PM_READ_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_WRITE_PERMISSION_6 Register TLN20_PM_WRITE_PERMISSION_6 - write_permission_6
//! @{

//! Register Offset (relative)
#define TLN20_PM_WRITE_PERMISSION_6 0x82118
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_WRITE_PERMISSION_6 0x1BF82118u

//! Register Reset Value
#define TLN20_PM_WRITE_PERMISSION_6_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_6_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_6_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_ADDR_MATCH_7 Register TLN20_PM_ADDR_MATCH_7 - addr_match_7
//! @{

//! Register Offset (relative)
#define TLN20_PM_ADDR_MATCH_7 0x82120
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ADDR_MATCH_7 0x1BF82120u

//! Register Reset Value
#define TLN20_PM_ADDR_MATCH_7_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_7_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_7_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_7_SIZE_POS 3
//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_7_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_7_LEVEL_POS 9
//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_7_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_7_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_7_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN20_PM_REQ_INFO_PERMISSION_7 Register TLN20_PM_REQ_INFO_PERMISSION_7 - req_info_permission_7
//! @{

//! Register Offset (relative)
#define TLN20_PM_REQ_INFO_PERMISSION_7 0x82128
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_REQ_INFO_PERMISSION_7 0x1BF82128u

//! Register Reset Value
#define TLN20_PM_REQ_INFO_PERMISSION_7_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_7_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_7_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_READ_PERMISSION_7 Register TLN20_PM_READ_PERMISSION_7 - read_permission_7
//! @{

//! Register Offset (relative)
#define TLN20_PM_READ_PERMISSION_7 0x82130
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_READ_PERMISSION_7 0x1BF82130u

//! Register Reset Value
#define TLN20_PM_READ_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_WRITE_PERMISSION_7 Register TLN20_PM_WRITE_PERMISSION_7 - write_permission_7
//! @{

//! Register Offset (relative)
#define TLN20_PM_WRITE_PERMISSION_7 0x82138
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_WRITE_PERMISSION_7 0x1BF82138u

//! Register Reset Value
#define TLN20_PM_WRITE_PERMISSION_7_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_7_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_7_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_ADDR_MATCH_8 Register TLN20_PM_ADDR_MATCH_8 - addr_match_8
//! @{

//! Register Offset (relative)
#define TLN20_PM_ADDR_MATCH_8 0x82140
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ADDR_MATCH_8 0x1BF82140u

//! Register Reset Value
#define TLN20_PM_ADDR_MATCH_8_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_8_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_8_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_8_SIZE_POS 3
//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_8_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_8_LEVEL_POS 9
//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_8_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_8_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_8_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN20_PM_REQ_INFO_PERMISSION_8 Register TLN20_PM_REQ_INFO_PERMISSION_8 - req_info_permission_8
//! @{

//! Register Offset (relative)
#define TLN20_PM_REQ_INFO_PERMISSION_8 0x82148
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_REQ_INFO_PERMISSION_8 0x1BF82148u

//! Register Reset Value
#define TLN20_PM_REQ_INFO_PERMISSION_8_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_8_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_8_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_READ_PERMISSION_8 Register TLN20_PM_READ_PERMISSION_8 - read_permission_8
//! @{

//! Register Offset (relative)
#define TLN20_PM_READ_PERMISSION_8 0x82150
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_READ_PERMISSION_8 0x1BF82150u

//! Register Reset Value
#define TLN20_PM_READ_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_WRITE_PERMISSION_8 Register TLN20_PM_WRITE_PERMISSION_8 - write_permission_8
//! @{

//! Register Offset (relative)
#define TLN20_PM_WRITE_PERMISSION_8 0x82158
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_WRITE_PERMISSION_8 0x1BF82158u

//! Register Reset Value
#define TLN20_PM_WRITE_PERMISSION_8_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_8_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_8_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_ADDR_MATCH_9 Register TLN20_PM_ADDR_MATCH_9 - addr_match_9
//! @{

//! Register Offset (relative)
#define TLN20_PM_ADDR_MATCH_9 0x82160
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ADDR_MATCH_9 0x1BF82160u

//! Register Reset Value
#define TLN20_PM_ADDR_MATCH_9_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_9_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_9_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_9_SIZE_POS 3
//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_9_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_9_LEVEL_POS 9
//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_9_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_9_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_9_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN20_PM_REQ_INFO_PERMISSION_9 Register TLN20_PM_REQ_INFO_PERMISSION_9 - req_info_permission_9
//! @{

//! Register Offset (relative)
#define TLN20_PM_REQ_INFO_PERMISSION_9 0x82168
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_REQ_INFO_PERMISSION_9 0x1BF82168u

//! Register Reset Value
#define TLN20_PM_REQ_INFO_PERMISSION_9_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_9_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_9_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_READ_PERMISSION_9 Register TLN20_PM_READ_PERMISSION_9 - read_permission_9
//! @{

//! Register Offset (relative)
#define TLN20_PM_READ_PERMISSION_9 0x82170
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_READ_PERMISSION_9 0x1BF82170u

//! Register Reset Value
#define TLN20_PM_READ_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_WRITE_PERMISSION_9 Register TLN20_PM_WRITE_PERMISSION_9 - write_permission_9
//! @{

//! Register Offset (relative)
#define TLN20_PM_WRITE_PERMISSION_9 0x82178
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_WRITE_PERMISSION_9 0x1BF82178u

//! Register Reset Value
#define TLN20_PM_WRITE_PERMISSION_9_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_9_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_9_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_ADDR_MATCH_10 Register TLN20_PM_ADDR_MATCH_10 - addr_match_10
//! @{

//! Register Offset (relative)
#define TLN20_PM_ADDR_MATCH_10 0x82180
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ADDR_MATCH_10 0x1BF82180u

//! Register Reset Value
#define TLN20_PM_ADDR_MATCH_10_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_10_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_10_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_10_SIZE_POS 3
//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_10_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_10_LEVEL_POS 9
//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_10_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_10_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_10_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN20_PM_REQ_INFO_PERMISSION_10 Register TLN20_PM_REQ_INFO_PERMISSION_10 - req_info_permission_10
//! @{

//! Register Offset (relative)
#define TLN20_PM_REQ_INFO_PERMISSION_10 0x82188
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_REQ_INFO_PERMISSION_10 0x1BF82188u

//! Register Reset Value
#define TLN20_PM_REQ_INFO_PERMISSION_10_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_10_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_10_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_READ_PERMISSION_10 Register TLN20_PM_READ_PERMISSION_10 - read_permission_10
//! @{

//! Register Offset (relative)
#define TLN20_PM_READ_PERMISSION_10 0x82190
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_READ_PERMISSION_10 0x1BF82190u

//! Register Reset Value
#define TLN20_PM_READ_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_WRITE_PERMISSION_10 Register TLN20_PM_WRITE_PERMISSION_10 - write_permission_10
//! @{

//! Register Offset (relative)
#define TLN20_PM_WRITE_PERMISSION_10 0x82198
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_WRITE_PERMISSION_10 0x1BF82198u

//! Register Reset Value
#define TLN20_PM_WRITE_PERMISSION_10_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_10_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_10_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_ADDR_MATCH_11 Register TLN20_PM_ADDR_MATCH_11 - addr_match_11
//! @{

//! Register Offset (relative)
#define TLN20_PM_ADDR_MATCH_11 0x821A0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ADDR_MATCH_11 0x1BF821A0u

//! Register Reset Value
#define TLN20_PM_ADDR_MATCH_11_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_11_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_11_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_11_SIZE_POS 3
//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_11_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_11_LEVEL_POS 9
//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_11_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_11_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_11_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN20_PM_REQ_INFO_PERMISSION_11 Register TLN20_PM_REQ_INFO_PERMISSION_11 - req_info_permission_11
//! @{

//! Register Offset (relative)
#define TLN20_PM_REQ_INFO_PERMISSION_11 0x821A8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_REQ_INFO_PERMISSION_11 0x1BF821A8u

//! Register Reset Value
#define TLN20_PM_REQ_INFO_PERMISSION_11_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_11_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_11_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_READ_PERMISSION_11 Register TLN20_PM_READ_PERMISSION_11 - read_permission_11
//! @{

//! Register Offset (relative)
#define TLN20_PM_READ_PERMISSION_11 0x821B0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_READ_PERMISSION_11 0x1BF821B0u

//! Register Reset Value
#define TLN20_PM_READ_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_WRITE_PERMISSION_11 Register TLN20_PM_WRITE_PERMISSION_11 - write_permission_11
//! @{

//! Register Offset (relative)
#define TLN20_PM_WRITE_PERMISSION_11 0x821B8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_WRITE_PERMISSION_11 0x1BF821B8u

//! Register Reset Value
#define TLN20_PM_WRITE_PERMISSION_11_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_11_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_11_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_ADDR_MATCH_12 Register TLN20_PM_ADDR_MATCH_12 - addr_match_12
//! @{

//! Register Offset (relative)
#define TLN20_PM_ADDR_MATCH_12 0x821C0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ADDR_MATCH_12 0x1BF821C0u

//! Register Reset Value
#define TLN20_PM_ADDR_MATCH_12_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_12_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_12_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_12_SIZE_POS 3
//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_12_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_12_LEVEL_POS 9
//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_12_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_12_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_12_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN20_PM_REQ_INFO_PERMISSION_12 Register TLN20_PM_REQ_INFO_PERMISSION_12 - req_info_permission_12
//! @{

//! Register Offset (relative)
#define TLN20_PM_REQ_INFO_PERMISSION_12 0x821C8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_REQ_INFO_PERMISSION_12 0x1BF821C8u

//! Register Reset Value
#define TLN20_PM_REQ_INFO_PERMISSION_12_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_12_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_12_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_READ_PERMISSION_12 Register TLN20_PM_READ_PERMISSION_12 - read_permission_12
//! @{

//! Register Offset (relative)
#define TLN20_PM_READ_PERMISSION_12 0x821D0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_READ_PERMISSION_12 0x1BF821D0u

//! Register Reset Value
#define TLN20_PM_READ_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_WRITE_PERMISSION_12 Register TLN20_PM_WRITE_PERMISSION_12 - write_permission_12
//! @{

//! Register Offset (relative)
#define TLN20_PM_WRITE_PERMISSION_12 0x821D8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_WRITE_PERMISSION_12 0x1BF821D8u

//! Register Reset Value
#define TLN20_PM_WRITE_PERMISSION_12_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_12_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_12_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_ADDR_MATCH_13 Register TLN20_PM_ADDR_MATCH_13 - addr_match_13
//! @{

//! Register Offset (relative)
#define TLN20_PM_ADDR_MATCH_13 0x821E0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ADDR_MATCH_13 0x1BF821E0u

//! Register Reset Value
#define TLN20_PM_ADDR_MATCH_13_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_13_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_13_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_13_SIZE_POS 3
//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_13_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_13_LEVEL_POS 9
//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_13_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_13_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_13_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN20_PM_REQ_INFO_PERMISSION_13 Register TLN20_PM_REQ_INFO_PERMISSION_13 - req_info_permission_13
//! @{

//! Register Offset (relative)
#define TLN20_PM_REQ_INFO_PERMISSION_13 0x821E8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_REQ_INFO_PERMISSION_13 0x1BF821E8u

//! Register Reset Value
#define TLN20_PM_REQ_INFO_PERMISSION_13_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_13_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_13_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_READ_PERMISSION_13 Register TLN20_PM_READ_PERMISSION_13 - read_permission_13
//! @{

//! Register Offset (relative)
#define TLN20_PM_READ_PERMISSION_13 0x821F0
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_READ_PERMISSION_13 0x1BF821F0u

//! Register Reset Value
#define TLN20_PM_READ_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_WRITE_PERMISSION_13 Register TLN20_PM_WRITE_PERMISSION_13 - write_permission_13
//! @{

//! Register Offset (relative)
#define TLN20_PM_WRITE_PERMISSION_13 0x821F8
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_WRITE_PERMISSION_13 0x1BF821F8u

//! Register Reset Value
#define TLN20_PM_WRITE_PERMISSION_13_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_13_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_13_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_ADDR_MATCH_14 Register TLN20_PM_ADDR_MATCH_14 - addr_match_14
//! @{

//! Register Offset (relative)
#define TLN20_PM_ADDR_MATCH_14 0x82200
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ADDR_MATCH_14 0x1BF82200u

//! Register Reset Value
#define TLN20_PM_ADDR_MATCH_14_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_14_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_14_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_14_SIZE_POS 3
//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_14_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_14_LEVEL_POS 9
//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_14_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_14_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_14_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN20_PM_REQ_INFO_PERMISSION_14 Register TLN20_PM_REQ_INFO_PERMISSION_14 - req_info_permission_14
//! @{

//! Register Offset (relative)
#define TLN20_PM_REQ_INFO_PERMISSION_14 0x82208
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_REQ_INFO_PERMISSION_14 0x1BF82208u

//! Register Reset Value
#define TLN20_PM_REQ_INFO_PERMISSION_14_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_14_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_14_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_READ_PERMISSION_14 Register TLN20_PM_READ_PERMISSION_14 - read_permission_14
//! @{

//! Register Offset (relative)
#define TLN20_PM_READ_PERMISSION_14 0x82210
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_READ_PERMISSION_14 0x1BF82210u

//! Register Reset Value
#define TLN20_PM_READ_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_WRITE_PERMISSION_14 Register TLN20_PM_WRITE_PERMISSION_14 - write_permission_14
//! @{

//! Register Offset (relative)
#define TLN20_PM_WRITE_PERMISSION_14 0x82218
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_WRITE_PERMISSION_14 0x1BF82218u

//! Register Reset Value
#define TLN20_PM_WRITE_PERMISSION_14_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_14_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_14_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_ADDR_MATCH_15 Register TLN20_PM_ADDR_MATCH_15 - addr_match_15
//! @{

//! Register Offset (relative)
#define TLN20_PM_ADDR_MATCH_15 0x82220
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_ADDR_MATCH_15 0x1BF82220u

//! Register Reset Value
#define TLN20_PM_ADDR_MATCH_15_RST 0x0000000000000000u

//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_15_ADDR_SPACE_POS 0
//! Field ADDR_SPACE - addr_space
#define TLN20_PM_ADDR_MATCH_15_ADDR_SPACE_MASK 0x7u

//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_15_SIZE_POS 3
//! Field SIZE - size
#define TLN20_PM_ADDR_MATCH_15_SIZE_MASK 0xF8u

//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_15_LEVEL_POS 9
//! Field LEVEL - level
#define TLN20_PM_ADDR_MATCH_15_LEVEL_MASK 0x200u

//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_15_BASE_ADDR_POS 10
//! Field BASE_ADDR - base_addr
#define TLN20_PM_ADDR_MATCH_15_BASE_ADDR_MASK 0xFFFFFFFFFFFFFC00u

//! @}

//! \defgroup TLN20_PM_REQ_INFO_PERMISSION_15 Register TLN20_PM_REQ_INFO_PERMISSION_15 - req_info_permission_15
//! @{

//! Register Offset (relative)
#define TLN20_PM_REQ_INFO_PERMISSION_15 0x82228
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_REQ_INFO_PERMISSION_15 0x1BF82228u

//! Register Reset Value
#define TLN20_PM_REQ_INFO_PERMISSION_15_RST 0x0000000000000000u

//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_15_REQ_INFO_POS 0
//! Field REQ_INFO - req_info
#define TLN20_PM_REQ_INFO_PERMISSION_15_REQ_INFO_MASK 0xFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_READ_PERMISSION_15 Register TLN20_PM_READ_PERMISSION_15 - read_permission_15
//! @{

//! Register Offset (relative)
#define TLN20_PM_READ_PERMISSION_15 0x82230
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_READ_PERMISSION_15 0x1BF82230u

//! Register Reset Value
#define TLN20_PM_READ_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_READ_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! \defgroup TLN20_PM_WRITE_PERMISSION_15 Register TLN20_PM_WRITE_PERMISSION_15 - write_permission_15
//! @{

//! Register Offset (relative)
#define TLN20_PM_WRITE_PERMISSION_15 0x82238
//! Register Offset (absolute) for 1st Instance SSX2_SSX
#define SSX2_SSX_TLN20_PM_WRITE_PERMISSION_15 0x1BF82238u

//! Register Reset Value
#define TLN20_PM_WRITE_PERMISSION_15_RST 0x0000000000000000u

//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_15_BIT_VECTOR_POS 0
//! Field BIT_VECTOR - bit_vector
#define TLN20_PM_WRITE_PERMISSION_15_BIT_VECTOR_MASK 0xFFFFFFFFFFFFFFFFu

//! @}

//! @}

#endif
