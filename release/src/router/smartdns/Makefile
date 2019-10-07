include ../common.mak

all:

install:
	#install -d $(INSTALLDIR)/rom/etc
	install -d $(INSTALLDIR)/usr/sbin

	#install -m 0644 smartdns.conf $(INSTALLDIR)/rom/etc
	#install -m 0644 smartdns-opt.conf $(INSTALLDIR)/rom/etc
ifeq ($(RTCONFIG_LANTIQ),y)
	install -D smartdns_mips $(INSTALLDIR)/usr/sbin/smartdns
else ifeq ($(RTCONFIG_RALINK),y)
	install -D smartdns_mipsle $(INSTALLDIR)/usr/sbin/smartdns
else ifeq ($(HND_ROUTER),y)
	install -D smartdns_arm64 $(INSTALLDIR)/usr/sbin/smartdns
#REALTEK? unsupport
else
	install -D smartdns_arm $(INSTALLDIR)/usr/sbin/smartdns
endif

clean:
