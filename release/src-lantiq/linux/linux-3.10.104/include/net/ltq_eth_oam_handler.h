#ifndef __LTQ_ETH_OAM_HANDLER_H
#define __LTQ_ETH_OAM_HANDLER_H

/******************************************************************************
**
** FILE NAME    : ltq_eth_oam_handler.h
** PROJECT      : UGW
** MODULES      : Ethernet OAM
**
** DATE         : 5 APRIL 2013
** AUTHOR       : Purnendu Ghosh
** DESCRIPTION  : Ethernet OAM Functions Header
** COPYRIGHT    :   Copyright (c) 2013
**                LANTIQ DEUTSCHLAND GMBH,
**                Lilienthalstrasse 15, 85579 Neubiberg, Germany
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
**
** HISTORY
** $Date        $Author                $Comment
** 5 APRIL 2013 PURNENDU GHOSH         Initiate Version
** 21 Feb 2014  Mahipati Deshpande     Ported to Kernel-3.10
*******************************************************************************/


/*! \file ltq_eth_oam_handler.h
    \brief This file contains: Ethernet OAM api.
*/
extern struct net_device * (*fp_ltq_eth_oam_dev)(void);

#endif 
