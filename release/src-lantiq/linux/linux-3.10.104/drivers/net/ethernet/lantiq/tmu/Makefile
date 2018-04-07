EXTRA_CFLAGS += -DTMU_LINUX_KERNEL

obj-$(CONFIG_LTQ_TMU)	+= drv_tmu_ll_proc.o drv_tmu_ll.o drv_tmu_ll_lookup_proc.o

ifdef CONFIG_LTQ_TMU_CHIPTEST

obj-$(CONFIG_LTQ_TMU) += drv_tmu_testcase.o

endif
