
obj-$(CONFIG_LTQ_ATM) = ltqmips_atm.o

ltqmips_atm-objs   := ltqmips_atm_core.o
ifeq ($(CONFIG_LTQ_ATM_XRX200),y)
  ltqmips_atm-objs += ltqmips_atm_vr9.o
endif
ifeq ($(CONFIG_LTQ_ATM_XRX300),y)
  ltqmips_atm-objs += ltqmips_atm_ar9.o
endif
