#
# Makefile for USB Host Controller Drivers
#

ccflags-$(CONFIG_USB_DEBUG) := -DDEBUG

isp1760-y := isp1760-hcd.o isp1760-if.o

fhci-y := fhci-hcd.o fhci-hub.o fhci-q.o
fhci-y += fhci-mem.o fhci-tds.o fhci-sched.o

fhci-$(CONFIG_FHCI_DEBUG) += fhci-dbg.o

xhci-hcd-y := xhci.o xhci-mem.o
xhci-hcd-y += xhci-ring.o xhci-hub.o xhci-dbg.o
xhci-hcd-$(CONFIG_PCI)	+= xhci-pci.o

ifneq ($(CONFIG_USB_XHCI_PLATFORM), )
	xhci-hcd-y		+= xhci-plat.o
endif

obj-$(CONFIG_USB_WHCI_HCD)	+= whci/

obj-$(CONFIG_PCI)		+= pci-quirks.o

obj-$(CONFIG_USB_EHCI_HCD)	+= ehci-hcd.o
obj-$(CONFIG_USB_EHCI_PCI)	+= ehci-pci.o
obj-$(CONFIG_USB_EHCI_HCD_PLATFORM)	+= ehci-platform.o
obj-$(CONFIG_USB_EHCI_MXC)	+= ehci-mxc.o
obj-$(CONFIG_USB_EHCI_HCD_OMAP)	+= ehci-omap.o
obj-$(CONFIG_USB_EHCI_HCD_ORION)	+= ehci-orion.o
obj-$(CONFIG_USB_EHCI_HCD_SPEAR)	+= ehci-spear.o
obj-$(CONFIG_USB_EHCI_S5P)	+= ehci-s5p.o
obj-$(CONFIG_USB_EHCI_HCD_AT91) += ehci-atmel.o
obj-$(CONFIG_USB_EHCI_MSM)	+= ehci-msm.o

obj-$(CONFIG_USB_OXU210HP_HCD)	+= oxu210hp-hcd.o
obj-$(CONFIG_USB_ISP116X_HCD)	+= isp116x-hcd.o
obj-$(CONFIG_USB_ISP1362_HCD)	+= isp1362-hcd.o
obj-$(CONFIG_USB_OHCI_HCD)	+= ohci-hcd.o
obj-$(CONFIG_USB_UHCI_HCD)	+= uhci-hcd.o
obj-$(CONFIG_USB_FHCI_HCD)	+= fhci.o
obj-$(CONFIG_USB_XHCI_HCD)	+= xhci-hcd.o
obj-$(CONFIG_USB_SL811_HCD)	+= sl811-hcd.o
obj-$(CONFIG_USB_SL811_CS)	+= sl811_cs.o
obj-$(CONFIG_USB_U132_HCD)	+= u132-hcd.o
obj-$(CONFIG_USB_R8A66597_HCD)	+= r8a66597-hcd.o
obj-$(CONFIG_USB_ISP1760_HCD)	+= isp1760.o
obj-$(CONFIG_USB_HWA_HCD)	+= hwa-hc.o
obj-$(CONFIG_USB_IMX21_HCD)	+= imx21-hcd.o
obj-$(CONFIG_USB_FSL_MPH_DR_OF)	+= fsl-mph-dr-of.o
obj-$(CONFIG_USB_OCTEON2_COMMON) += octeon2-common.o
obj-$(CONFIG_USB_HCD_BCMA)	+= bcma-hcd.o
obj-$(CONFIG_USB_HCD_SSB)	+= ssb-hcd.o


	ltqusb_host-objs    := ltqusb_driver.o
	ltqusb_host-objs    += ltqusb_ctl.o
	ltqusb_host-objs    += ltqusb_cif_h.o
	ltqusb_host-objs    += ltqhcd.o
	ltqusb_host-objs    += ltqhcd_es.o
	ltqusb_host-objs    += ltqhcd_intr.o
	ltqusb_host-objs    += ltqhcd_queue.o


	EXTRA_CFLAGS  += -Dlinux -D__LINUX__
	EXTRA_CFLAGS  += -D__KERNEL__
	EXTRA_CFLAGS  += -D__DEBUG__
	EXTRA_CFLAGS  += -D__ENABLE_DUMP__

ifeq ($(CONFIG_USB_HOST_LTQ_FORCE_USB11),y)
	EXTRA_CFLAGS  += -D__FORCE_USB11__
endif
ifeq ($(CONFIG_USB_HOST_LTQ_WITH_HS_ELECT_TST),y)
	EXTRA_CFLAGS  += -D__WITH_HS_ELECT_TST__
endif
ifeq ($(CONFIG_USB_HOST_LTQ_WITH_ISO),y)
	EXTRA_CFLAGS  += -D__EN_ISOC__
endif
ifeq ($(CONFIG_USB_HOST_LTQ_UNALIGNED_ADJ),y)
	EXTRA_CFLAGS  += -D__UNALIGNED_BUF_ADJ__
#	EXTRA_CFLAGS  += -D__UNALIGNED_BUF_BURST__
endif
ifeq ($(CONFIG_USB_HOST_LTQ_UNALIGNED_CHK),y)
	EXTRA_CFLAGS  += -D__UNALIGNED_BUF_CHK__
#	EXTRA_CFLAGS  += -D__UNALIGNED_BUF_BURST__
endif
ifeq ($(CONFIG_USB_HOST_LTQ_COC),y)
#	EXTRA_CFLAGS  += -D__HOST_COC__
endif

	EXTRA_CFLAGS  += -D__NEW_COC__

	EXTRA_CFLAGS  += -D__DYN_SOF_INTR__
#	EXTRA_CFLAGS  += -D__EN_ISOC__
#	EXTRA_CFLAGS  += -D__EN_ISOC_SPLIT__
#	EXTRA_CFLAGS  += -D__EPQD_DESTROY_TIMEOUT__

	EXTRA_CFLAGS  += -D__PHY_LONG_PREEMP__

	EXTRA_CFLAGS  += -D__INTRNAKRETRY__
	EXTRA_CFLAGS  += -D__INTRINCRETRY__

	EXTRA_CFLAGS  += -D__BURST_XFER__
#	EXTRA_CFLAGS  += -D__SINGLE_XACT__

obj-$(CONFIG_USB_HOST_LTQ)	+= ltqusb_host.o


