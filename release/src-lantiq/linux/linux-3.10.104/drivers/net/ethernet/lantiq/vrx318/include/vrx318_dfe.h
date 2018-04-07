/******************************************************************************
 *
 *                        Copyright (c) 2012, 2014, 2015
 *                           Lantiq Deutschland GmbH
 *
 *  For licensing information, see the file 'LICENSE' in the root folder of
 *  this software module.
 *
 ******************************************************************************/
#ifndef __VRX218_DFE_H
#define __VRX218_DFE_H

#include "vrx318_dsl_address_define.h"

struct me_cfg_def {
	u32 addr;
	u32 data;
	u32 type;
};

/* Header file to set the Loopback rate to programmable! */
#include "prograte.h"

#endif
