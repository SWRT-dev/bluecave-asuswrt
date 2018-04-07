obj-$(CONFIG_LTQ_DATAPATH) = datapath_api.o datapath_proc_api.o datapath_proc.o datapath_pmac.o datapath_mib.o

ifneq ($(CONFIG_LTQ_DATAPATH_LOOPETH),)
obj-$(CONFIG_LTQ_DATAPATH) += datapath_loopeth_dev.o
endif

ifneq ($(CONFIG_LTQ_DATAPATH_CPUFREQ),)
obj-$(CONFIG_LTQ_DATAPATH) += datapath_coc.o
endif

ifneq ($(LTQ_DATAPATH_FILTER),)
obj-$(CONFIG_LTQ_DATAPATH_FILTER) += datapath_filter.o
endif


ifneq ($(CONFIG_LTQ_DP_MPE_FASTHOOK_TEST_COMPILE),)
obj-$(CONFIG_LTQ_DP_MPE_FASTHOOK_TEST_COMPILE) += mpe/
endif



