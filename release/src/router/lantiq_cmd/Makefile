TARGET=lantiq_cmd

include ../common.mak
include $(TOP)/.config

NVRAM_DIR=$(TOP)/nvram$(BCMEX)$(EX7)
JSON_C_DIR=$(TOP)/json-c
SHARED_DIR=$(TOP)/shared

SRC=$(wildcard *.c)
#OBJS=$(patsubst %.c, %.o, $(SRC))
OBJS = ../hostapd-2.6/src/common/wpa_ctrl.o
OBJS += ../hostapd-2.6/src/utils/os_unix.o
ifeq ($(wildcard $(TOP)/lantiq_cmd/*.c),)
all:
	-cp -f $(TOP)/lantiq_cmd/prebuild/lantiq_cmd lantiq_cmd
else
all: $(TARGET)
endif

ROUTER_LIBS=-lnvram -lshared
ROUTER_LDFLAGS=-L$(NVRAM_DIR) -L$(SHARED_DIR)

CFLAGS += -g
CFLAGS += -I.
CFLAGS += -I$(JSON_C_DIR)
CFLAGS += -I$(NVRAM_DIR)
CFLAGS += -I$(SRCBASE)/include
CFLAGS +=  $(ROUTER_CFLAGS)
CFLAGS += -I$(TOP)/hostapd-2.6/src/utils/
CFLAGS += -I$(TOP)/hostapd-2.6/src/common/
LIBS = -ldl -pthread -lrt $(ROUTER_LIBS)
LDFLAGS += -L$(JSON_C_DIR)/.libs -L. $(ROUTER_LDFLAGS)
LDFLAGS += -L$(TOP)/fapi_wlan_common-05.04.00.131 -lfapiwlancommon -ldl -L$(TOP)/libhelper-1.4.0.2
LDFLAGS += -lhelper -L$(TOP)/libfapi-0.1 -lfapi
LDFLAGS += -L$(TOP)/wlc_nt -lwlc_nt_client
CFLAGS  += -I$(TOP)/shared/sysdeps/lantiq -I$(TOP)/wireless_tools -I$(TOP)/fapi_wlan_common-05.04.00.131/include
CFLAGS  += -I$(TOP)/libhelper-1.4.0.2/include -DPACKAGE_ID=\"fapi_wlan_common\" -DLOGGING_ID="fapi_wlan_common"
CFLAGS  += -I$(TOP)/wlc_nt
ifeq ($(wildcard $(TOP)/aaews/*.c),)
router:
	-cp -f $(TOP)/lantiq_cmd/prebuild/lantiq_cmd lantiq_cmd
else
router: $(TARGET)
endif

$(TARGET):  main.o 
	$(CXX) -fPIC $(CFLAGS) $(LDFLAGS) $(LIBS) $(OBJS) -o $@ $^	

%.o: %.c
	$(CC) -c  -fPIC $(CFLAGS) $< -o $@ 

#install: all
#	$(STRIP) $(WSLIB)
#	install -D $(WSLIB) $(INSTALLDIR)/lib/$(WSLIB)
	
clean:
	echo $(WB_DIR)
	echo $(TOP)
	rm -f *.o  *.so *.dylib *.a lantiq_cmd -r

