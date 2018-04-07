#
# Makefile for the platform specific kernel interface routines under Linux.
#

obj-$(CONFIG_LTQ_PTM) = ltqmips_ptm.o

ifeq ($(CONFIG_LTQ_PTM_XRX200),y)
  ltqmips_ptm-objs := ltqmips_ptm_vdsl.o ltqmips_ptm_vr9.o
endif

ifeq ($(CONFIG_LTQ_PTM_TEST_PROC),y)
  ifeq ($(CONFIG_LTQ_PTM),y)
    ltqmips_ptm-objs += ltqmips_ptm_test.o
  else
    obj-$(CONFIG_LTQ_PTM) += ltqmips_ptm_test.o
  endif
endif
