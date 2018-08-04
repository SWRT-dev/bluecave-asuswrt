#
# Makefile for DWC_otg Highspeed USB controller driver
#

ccflags-$(CONFIG_USB_LTQ_DWC2_DEBUG)	:= -DDEBUG

EXTRA_CFLAGS	+= -Dlinux -DDWC_HS_ELECT_TST
EXTRA_CFLAGS   	+= -Idrivers/usb/ltq_dwc2/dwc_common_port
EXTRA_CFLAGS   	+= -DDWC_LINUX
EXTRA_CFLAGS   	+= $(CFI)
EXTRA_CFLAGS	+= $(BUS_INTERFACE)

obj-$(CONFIG_USB_LTQ_DWC2)	:= dwc_otg.o

dwc_otg-objs	:= dwc_otg_driver.o dwc_otg_attr.o 
dwc_otg-objs	+= dwc_otg_cil.o dwc_otg_cil_intr.o 
dwc_otg-objs	+= dwc_otg_hcd.o dwc_otg_hcd_linux.o dwc_otg_hcd_intr.o dwc_otg_hcd_queue.o dwc_otg_hcd_ddma.o
dwc_otg-objs	+= dwc_otg_adp.o
dwc_otg-objs	+= ltq-dwc2.o
ifneq ($(CFI),)
dwc_otg-objs	+= dwc_otg_cfi.o
endif

obj-$(CONFIG_USB_DWC2_COMMON_PORT)	+= dwc_common_port/
