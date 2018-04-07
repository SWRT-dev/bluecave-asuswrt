/****************************************************************************
*                             Copyright (c) 2010
*                           Lantiq Deutschland GmbH
*                    Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 *****************************************************************************/
/*!
  \file ltqmips_usif_spi_vr9.h
  \ingroup IFX_USIF_SPI
  \brief USIF SPI driver VR9 specific file
*/
#ifndef LTQMIPS_USIF_SPI_VR9_H
#define LTQMIPS_USIF_SPI_VR9_H

#include <linux/clk.h>

static inline void lantiq_usif_spi_pm_enable(void)
{
	/* Activate USIF SPI and AHB Master */
//	USIF_PMU_SETUP(IFX_PMU_ENABLE);
//	AHBM_PMU_SETUP(IFX_PMU_ENABLE);
	struct clk *clk = NULL;

	clk = clk_get_sys("1da00000.usif", "NULL");
	if (clk < 0 || !clk) {
		printk(KERN_ERR "%s: getting the clock for USIF failed\n",
						__func__);
	} else {
		clk_enable(clk);
		printk ("%s: successfully enabled the USIF clock\n", __FUNCTION__);
	}
}

static inline void lantiq_usif_spi_pm_disable(void)
{
//	USIF_PMU_SETUP(IFX_PMU_DISABLE);
//	AHBM_PMU_SETUP(IFX_PMU_DISABLE);
	struct clk *clk;

	clk = clk_get_sys("1da00000.usif", NULL);
	clk_disable (clk);
}
#endif /* LTQMIPS_USIF_SPI_VR9_H */

