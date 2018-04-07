/****************************************************************************
*                             Copyright (c) 2010
*                           Lantiq Deutschland GmbH
&                    Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 *****************************************************************************/
/*!
  \file ltqmips_usif_sflash_ar10.h
  \ingroup LANTIQ_USIF_SFLASH
  \brief ifx usif serial flash driver AR10 specific file
*/

#ifndef LTQMIPS_USIF_SFLASH_AR10_H
#define LTQMIPS_USIF_SFLASH_AR10_H

#define LANTIQ_USIF_SFLASH_MODE                LANTIQ_USIF_SPI_MODE_0
#define LANTIQ_USIF_SFLASH_PRIORITY            LANTIQ_USIF_SPI_PRIO_LOW
#define LANTIQ_USIF_SFLASH_FRAGSIZE            264

#define LANTIQ_USIF_SFLASH_MAXFIFOSIZE         32

#ifdef CONFIG_USE_EMULATOR
#define LANTIQ_USIF_SFLASH_BAUDRATE            10000 /*  10K Hz */
/* MXIC */
#define LANTIQ_USIF_SFLASH_CS                  LANTIQ_USIF_SPI_CS0
#else
#define LANTIQ_USIF_SFLASH_BAUDRATE            20000000 /*  20 MHz */
#define LANTIQ_USIF_SFLASH_CS                  LANTIQ_USIF_SPI_CS0
#endif /* CONFIG_USE_EMULATOR */

#endif /* LTQMIPS_USIF_SFLASH_AR10_H */

