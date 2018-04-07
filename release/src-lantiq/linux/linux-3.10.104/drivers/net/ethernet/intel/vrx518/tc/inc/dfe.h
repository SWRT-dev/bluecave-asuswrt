/*******************************************************************************

  Intel SmartPHY DSL PCIe TC driver
  Copyright(c) 2016 Intel Corporation.

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

*******************************************************************************/

#ifndef __DFE_H__
#define __DFE_H__

#define MEI_OFFSET		0x2C0000
#define DSL_OFFSET		0x2D0000
#define MEIAD(a)		((a)+MEI_OFFSET)

/* Dsp Mgmt Address Constants */
#define DSP_MGMT_BASE		(0x0000)
#define DSP_MGMT_REG(r)		(DSP_MGMT_BASE + (r))
#define ME_ARC2ME_STAT		DSP_MGMT_REG(0x0004)
#define ME_DBG_RD_AD		DSP_MGMT_REG(0x002C)
#define ME_DBG_WR_AD		DSP_MGMT_REG(0x0030)
#define ME_DBG_DATA		DSP_MGMT_REG(0x0034)


struct me_cfg_def {
	u32 addr;
	u32 data;
	u32 type;
};

/* Header file to set the Loopback rate to programmable! */
#include "prograte.h"

#endif /* __DFE_H__ */
