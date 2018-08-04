#
# Makefile for DWC_common library
#

EXTRA_CFLAGS	+= -DDWC_LINUX
EXTRA_CFLAGS	+= -DDEBUG
#EXTRA_CFLAGS	+= -DDWC_DEBUG_REGS
#EXTRA_CFLAGS	+= -DDWC_DEBUG_MEMORY

EXTRA_CFLAGS	+= -DDWC_LIBMODULE
EXTRA_CFLAGS	+= -DDWC_CCLIB
EXTRA_CFLAGS	+= -DDWC_CRYPTOLIB
EXTRA_CFLAGS	+= -DDWC_NOTIFYLIB
EXTRA_CFLAGS	+= -DDWC_UTFLIB

obj-$(CONFIG_USB_DWC2_COMMON_PORT)	:= dwc_common_port_lib.o
dwc_common_port_lib-objs := dwc_cc.o dwc_modpow.o dwc_dh.o \
			    dwc_crypto.o dwc_notifier.o \
			    dwc_common_linux.o dwc_mem.o
