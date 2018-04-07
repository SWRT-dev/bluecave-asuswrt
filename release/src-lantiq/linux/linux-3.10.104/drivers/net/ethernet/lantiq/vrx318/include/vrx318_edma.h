/******************************************************************************
 *
 *                        Copyright (c) 2012, 2014, 2015
 *                           Lantiq Deutschland GmbH
 *
 *  For licensing information, see the file 'LICENSE' in the root folder of
 *  this software module.
 *
 ******************************************************************************/
#ifndef __VRX218_EDMA_H
#define __VRX218_EDMA_H

#define EDMA_WRITE_CH           0
#define EDMA_READ_CH            1

/* EDMA reg */
#define PCIE_AP_BASE		0x1E102000
#define PCIE_AP_OFFSET		0x00102000
#define PCIE_DM_BASE		0x1E700000
#define PCIE_DM_OFFSET		0x00700000
#define EDMA_OFFSET		(PCIE_DM_OFFSET + 0x700)

#define EDMA_WCH_DATA_LLE_NUM   32
#define EDMA_RCH_DATA_LLE_NUM   32

/* EDMA Global Register */
#define EDMA_WCH_EN			(0x27C + EDMA_OFFSET)
#define EDMA_WCH_DOORBELL		(0x280 + EDMA_OFFSET)
#define EDMA_RCH_EN			(0x29C + EDMA_OFFSET)
#define EDMA_RCH_DOORBELL		(0x2A0 + EDMA_OFFSET)

/* EDMA Interrupt Registers */
#define EDMA_WCH_INT_STATUS		(0x2BC + EDMA_OFFSET)
#define EDMA_WCH_INT_MASK		(0x2C4 + EDMA_OFFSET)
#define EDMA_WCH_INT_CLEAR		(0x2C8 + EDMA_OFFSET)
#define EDMA_WCH_ERR_STATUS		(0x2CC + EDMA_OFFSET)
#define EDMA_WCH_DONE_IMWR_ADDR_LOW	(0x2D0 + EDMA_OFFSET)
#define EDMA_WCH_DONE_IMWR_ADDR_HIGH	(0x2D4 + EDMA_OFFSET)
#define EDMA_WCH_ABORT_IMWR_ADDR_LOW	(0x2D8 + EDMA_OFFSET)
#define EDMA_WCH_ABORT_IMWR_ADDR_HIGH	(0x2DC + EDMA_OFFSET)
#define EDMA_WCH_0_IMWR_DATA		(0x2E0 + EDMA_OFFSET)
#define EDMA_WCH_LL_ERR_EN		(0x300 + EDMA_OFFSET)

#define EDMA_RCH_INT_STATUS		(0x310 + EDMA_OFFSET)
#define EDMA_RCH_INT_MASK		(0x318 + EDMA_OFFSET)
#define EDMA_RCH_INT_CLEAR		(0x31C + EDMA_OFFSET)
#define EDMA_RCH_ERR_STATUS_LOW		(0x324 + EDMA_OFFSET)
#define EDMA_RCH_ERR_STATUS_HIGH	(0x328 + EDMA_OFFSET)
#define EDMA_RCH_LL_ERR_EN		(0x334 + EDMA_OFFSET)
#define EDMA_RCH_DONE_IMWR_ADDR_LOW	(0x33C + EDMA_OFFSET)
#define EDMA_RCH_DONE_IMWR_ADDR_HIGH	(0x340 + EDMA_OFFSET)
#define EDMA_RCH_ABORT_IMWR_ADDR_LOW	(0x344 + EDMA_OFFSET)
#define EDMA_RCH_ABORT_IMWR_ADDR_HIGH	(0x348 + EDMA_OFFSET)
#define EDMA_RCH_0_IMWR_DATA		(0x34C + EDMA_OFFSET)

/* EDMA Channel Context Index Registers */
#define EDMA_CH_IDX			(0x36C + EDMA_OFFSET)
#define EDMA_CH_CTRL			(0x370 + EDMA_OFFSET)
#define EDMA_TRANSFER_SIZE		(0x378 + EDMA_OFFSET)
#define EDMA_SAR_LOW			(0x37C + EDMA_OFFSET)
#define EDMA_SAR_HIGH			(0x380 + EDMA_OFFSET)
#define EDMA_DAR_LOW			(0x384 + EDMA_OFFSET)
#define EDMA_DAR_HIGH			(0x388 + EDMA_OFFSET)
#define EDMA_LL_PTR_LOW			(0x38C + EDMA_OFFSET)
#define EDMA_LL_PTR_HIGH		(0x390 + EDMA_OFFSET)


typedef struct {
    // The following TLP header fields are used by the eDMA when generating
    // MRd/Mwr (not IMWr) TLPs
    unsigned int at : 2;    // Address Translation (AT)

    unsigned int tc : 3;    // Traffic Class (TC)

    unsigned int td : 1;    // Traffic Digest, the PCIe core adds the ECRC
                            // field and sets the TD bit in TLP header
    unsigned int ro : 1;    // Releaxed Ording

    unsigned int ns : 1;    // No Snoop

    // -----------------------
    unsigned int res0 : 7;

    unsigned int fn   : 5;  // Function Number (FN) for generated MRd/MWr DMA TLPs
                            // The core uses this when generating the RID for the
                            // MRd/MWr DMA TLP
    unsigned int res1 : 2;

    unsigned int ll_en: 1;  // link list enable

    unsigned int ccs  : 1;  // Consumer Cycles State (CCS).
                            // Used in Link List mode only. It is used to synchronize
                            // the Producer (Software) and the Consumer (DMA).
                            // *You must initialize this bit.* The DMA updates
                            // this bit during linked list operation
    unsigned int res2 : 1;

    unsigned int cs   : 2;  // Channel Status (CS)
                            // The Channel Status bits identify the current operational
                            // state of the DMA write or read channel.
                            //  00: Reserved
                            //  01: Running, this channel is active and transferring data
                            //  10: Halted. An error condition has been detected.
                            //      and the DMA has stopped this channel
                            //  11: Stopped. The DMA has transferred all data for
                            //      this channel or you have prematurely stopped
                            //      this channel by writing to the Stop field of
                            //      of the DMA R/W doorbell register

    unsigned int rie  : 1;  // remote interrupt enable

    unsigned int lie  : 1;  // local interrupt enable

    unsigned int llp  : 1;  // load link pointer (LLP)
                            // Used in link list mode only. Indicates that
                            // this linked list element is a link element, and
                            // it's LL element pointer DWORDs are pointint to the next
                            // (non-contiguous) element
                            // the DMA loads this field with the LLP of the
                            // linked list element

    unsigned int tcb  : 1;  // Toggle Cycle Bit (TCB)
                            // Indicates to the DMA to toglle its intepreation of
                            // the CB. Used in linked list mode only. It is used
                            // to synchorize the Producer (Software) and the
                            // Consumer (DMA).
                            // The DMA loads this field with the TCB of the linked
                            // list element.
                            // Note: this field is not defined in a ata LL element

    unsigned int cb  : 1;   // Cycle Bit (CB)
                            // Unsed in Linked list mode only.  It is used
                            // to synchorize the Producer (Software) and the
                            // Consumer (DMA).
                            // The DMA loads this field with the CB of the linked
                            // list elment

} edma_ch_ctrl_t;

typedef struct {
    //-----------------------------------------------
    // {{PPE_FW fields
    // for transfer_type DES_xxx, ctxt_ptr --> DES_SYNC_CFG_CTXT
    // for transfer_type PKT_xxx, ctxt_ptr --> EDMA_LLE_EXT
    unsigned int ctxt_ptr       : 16;

    // PKT_WRITE       : 0
    // PKT_READ        : 1
    //
    // DES_WRITE_DW0    : 3
    // DES_WRITE_DW1    : 4
    // DES_READ         : 5
    unsigned int transfer_type  : 4;

    // PPE_FW fields }}
    //-----------------------------------------------

    unsigned int res0 : 7;

    unsigned int rie  : 1;  // = 0, load link pointer (LLP)
    unsigned int lie  : 1;  // = 0, load link pointer (LLP)
    unsigned int llp  : 1;  // = 0, load link pointer (LLP)
    unsigned int res1 : 1;  //
    unsigned int cb   : 1;  // Cycle Bit (CB)

    unsigned int transfer_size;
    unsigned int sar_low;
    unsigned int sar_high;
    unsigned int dar_low;
    unsigned int dar_high;

} edma_lle_data_t;

typedef struct {
    unsigned int res0 : 29;
    unsigned int llp  : 1;  // = 1, load link pointer (LLP)
    unsigned int tcb  : 1;  // Toggle Cycle Bit (TCB)
    unsigned int cb   : 1;  // Cycle Bit (CB)

    unsigned int res1;

    unsigned int lle_ptr_low;
    unsigned int lle_ptr_high;

    unsigned int res2;
    unsigned int res3;
} edma_lle_link_t;



#endif
