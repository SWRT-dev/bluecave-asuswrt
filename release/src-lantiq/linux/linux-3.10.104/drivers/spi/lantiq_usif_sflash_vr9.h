/******************************************************************************
**
** FILE NAME    : ltqmips_usif_sflash_vr9.h
** PROJECT      : IFX UEIP
** MODULES      : Serial Flash
**
** DATE         : 16 Oct 2009
** AUTHOR       : Lei Chuanhua
** DESCRIPTION  : SPI Flash vr9 specific configuration
** COPYRIGHT    :       Copyright (c) 2009
**                      Infineon Technologies AG
**                      Am Campeon 1-12, 85579 Neubiberg, Germany
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
**
** HISTORY
** $Date        $Author         $Comment
**  03,July,2009 Lei Chuanhua   Initial UEIP release
*******************************************************************************/
/*!
  \file ltqmips_usif_sflash_vr9.h
  \ingroup LANTIQ_USIF_SFLASH
  \brief ifx usif serial flash driver VRX specific file
*/

#ifndef LTQMIPS_USIF_SFLASH_VR9_H
#define LTQMIPS_USIF_SFLASH_VR9_H

#define LANTIQ_USIF_SFLASH_MODE                LANTIQ_USIF_SPI_MODE_0
#define LANTIQ_USIF_SFLASH_PRIORITY            LANTIQ_USIF_SPI_PRIO_LOW
#define LANTIQ_USIF_SFLASH_FRAGSIZE            264
#define LANTIQ_USIF_SFLASH_MAXFIFOSIZE         32

#ifdef CONFIG_USE_EMULATOR
#define LANTIQ_USIF_SFLASH_BAUDRATE            10000/*  10K Hz */
/* MXIC */
#define LANTIQ_USIF_SFLASH_CS                  LANTIQ_USIF_SPI_CS0

#else
#define LANTIQ_USIF_SFLASH_BAUDRATE            20000000 /*  20 MHz */

#define LANTIQ_USIF_SFLASH_CS                  LANTIQ_USIF_SPI_CS2
#endif /* CONFIG_USE_EMULATOR */

#endif /* LTQMIPS_USIF_SFLASH_VR9_H */

