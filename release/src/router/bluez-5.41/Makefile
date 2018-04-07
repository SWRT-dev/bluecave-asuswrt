include ../common.mak
CFLAGS = -Os -Wall $(EXTRACFLAGS)

all:
ifeq ($(wildcard $(SRCBASE)/include/bluetooth),)
ifeq ($(wildcard bluetooth),)
	mkdir bluetooth
	cp -rf lib/* bluetooth
endif
	cp -rf bluetooth $(SRCBASE)/include
endif

install: all
	cp -f prebuild/bccmd				tools/
	cp -f prebuild/bluemoon				tools/
	cp -f prebuild/ciptool				tools/
	cp -f prebuild/hcitool				tools/
	cp -f prebuild/hcidump				tools/
	cp -f prebuild/hciattach			tools/
	cp -f prebuild/hciconfig			tools/
	cp -f prebuild/hid2hci				tools/
	cp -f prebuild/l2ping				tools/
	cp -f prebuild/l2test				tools/
	cp -f prebuild/mpris-proxy			tools/
	cp -f prebuild/rctest				tools/
	cp -f prebuild/rfcomm				tools/
	cp -f prebuild/sdptool				tools/
	cp -f prebuild/bluetoothctl			client/
	cp -f prebuild/bluetoothd			src/
	cp -f prebuild/gatttool				attrib/

clean:


.PHONY: all clean
