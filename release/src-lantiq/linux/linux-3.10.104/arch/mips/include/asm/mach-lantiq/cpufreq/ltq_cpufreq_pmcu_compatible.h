/******************************************************************************

		Copyright (c) 2010
		Lantiq Deutschland GmbH
		Am Campeon 3; 85579 Neubiberg, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/
#ifndef _LTQ_CPUFREQ_PMCU_COMPATIBLE_H_
#define _LTQ_CPUFREQ_PMCU_COMPATIBLE_H_

/*********************************************************************
 * Compatibility with PMCU                                           *
 *********************************************************************/
#define IFX_PMCU_MODULE_WLAN		LTQ_CPUFREQ_MODULE_WLAN
#define IFX_PMCU_MODULE_PPE		LTQ_CPUFREQ_MODULE_PPE
#define IFX_PMCU_MODULE_VE		LTQ_CPUFREQ_MODULE_VE
#define IFX_PMCU_MODULE_PCIE		LTQ_CPUFREQ_MODULE_PCIE
#define IFX_PMCU_MODULE_USB		LTQ_CPUFREQ_MODULE_USB
#define IFX_PMCU_MODULE_DEU		LTQ_CPUFREQ_MODULE_DEU

#define IFX_PMCU_STATE_INVALID		LTQ_CPUFREQ_PS_UNDEF
#define IFX_PMCU_STATE_D0		LTQ_CPUFREQ_PS_D0
#define IFX_PMCU_STATE_D1		LTQ_CPUFREQ_PS_D1
#define IFX_PMCU_STATE_D2		LTQ_CPUFREQ_PS_D2
#define IFX_PMCU_STATE_D3		LTQ_CPUFREQ_PS_D3
#define IFX_PMCU_STATE_D0D3		LTQ_CPUFREQ_PS_D0D3

#define IFX_PMCU_PWR_STATE_ON		1
#define IFX_PMCU_PWR_STATE_OFF		0

#define IFX_PMCU_RETURN_SUCCESS		LTQ_CPUFREQ_RETURN_SUCCESS
#define IFX_PMCU_RETURN_DENIED		LTQ_CPUFREQ_RETURN_DENIED
#define IFX_PMCU_RETURN_ERROR		-1
#define IFX_PMCU_RETURN_NOACTIVITY	LTQ_CPUFREQ_RETURN_NOACTIVITY
#define IFX_PMCU_RETURN_NOTDEFINED	LTQ_CPUFREQ_RETURN_NOTDEFINED

typedef enum	ltq_cpufreq_state	IFX_PMCU_STATE_t;
typedef enum	ltq_cpufreq_module	IFX_PMCU_MODULE_t;
typedef int	IFX_PMCU_PWR_STATE_ENA_t;
typedef int	IFX_PMCU_RETURN_t;

#endif		/* _LTQ_CPUFREQ_PMCU_COMPATIBLE_H_ */
