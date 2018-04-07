/****************************************************************************
*                             Copyright (c) 2010
*                           Lantiq Deutschland GmbH
*                    Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 *****************************************************************************/
/*!
  \file ltqmips_usif_spi_ar10.h
  \ingroup IFX_USIF_SPI
  \brief USIF SPI driver AR10 specific file
*/
#ifndef LTQMIPS_USIF_SPI_AR10_H
#define LTQMIPS_USIF_SPI_AR10_H

#include <linux/err.h>
#include <linux/clk.h>

static inline void lantiq_usif_spi_pm_enable(void)
{
	struct clk *clk;
	
	/* Activate USIF SPI */
	clk = clk_get_sys("1da00000.usif", "NULL");
	if (IS_ERR(clk)) {
	        printk(KERN_ERR "USIF: failed to get clock\n");
			        return;
	}
	clk_enable(clk);
}


static inline void lantiq_usif_spi_pm_disable(void)
{
	struct clk *clk;
	
	/* Activate USIF SPI */
	clk = clk_get_sys("1da00000.usif", "usif");
	if (IS_ERR(clk)) {
	        printk(KERN_ERR "USIF: failed to get clock\n");
			        return;
	}
	clk_disable(clk);
}
#endif /* LTQMIPS_USIF_SPI_AR10_H */

