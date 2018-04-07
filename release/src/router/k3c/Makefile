include ../common.mak

all:

install:
	install -d $(INSTALLDIR)/sbin
	install -d $(INSTALLDIR)/usr/sbin
	install -d $(INSTALLDIR)/lib
	install -d $(INSTALLDIR)/usr/bin
	install -d $(INSTALLDIR)/usr/sbin/ssr
	install -d $(INSTALLDIR)/usr/sbin/pcap-dnsproxy
	install -d $(INSTALLDIR)/usr/lib
	install -d $(INSTALLDIR)/usr/sbin/webshell
	install -d $(INSTALLDIR)/usr/sbin/frp
	install -d $(INSTALLDIR)/usr/adb
	install -d $(INSTALLDIR)/www
	install -d $(INSTALLDIR)/usr/adbyby
	install -d $(INSTALLDIR)/usr/adbyby/data
	install -d $(INSTALLDIR)/usr/adbyby/doc
	install -d $(INSTALLDIR)/usr/kms

# K3C update uboot&ubootconfig tool
	install -m 0755 mtd $(INSTALLDIR)/sbin
# K3C SSR and adbyby plus
	install -m 0755 base64 $(INSTALLDIR)/usr/bin
	install -m 0755 ssr-redir $(INSTALLDIR)/usr/sbin
	install -m 0755 ssr-rules $(INSTALLDIR)/usr/sbin
	install -m 0755 ssr-state $(INSTALLDIR)/usr/sbin
	install -m 0755 usr/ssr/gfw_addr.conf $(INSTALLDIR)/usr/sbin/ssr
	install -m 0755 usr/ssr/gfw_list.conf $(INSTALLDIR)/usr/sbin/ssr
	install -m 0755 usr/ssr/ignore.list $(INSTALLDIR)/usr/sbin/ssr
	install -m 0755 usr/ssr/ignore.user $(INSTALLDIR)/usr/sbin/ssr
	install -m 0755 usr/ssr/ssr_mon.sh $(INSTALLDIR)/usr/sbin/ssr
	install -m 0755 www/k3c-init.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/k3c_ssr.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/Tools_Adbyby.asp $(INSTALLDIR)/www
	install -m 0755 www/k3c_adbyby.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/user.txt $(INSTALLDIR)/usr/adb
	install -m 0755 www/openssl.cnf $(INSTALLDIR)/usr/adb
	install -m 0755 www/adbyby_mon.sh $(INSTALLDIR)/usr/adb
	install -m 0755 www/adbyby.sh $(INSTALLDIR)/usr/adb
	install -m 0755 www/gen_ca.sh $(INSTALLDIR)/usr/adb
	install -m 0755 www/adblock.conf $(INSTALLDIR)/usr/adb
	install -m 0755 usr/adbyby/adbyby $(INSTALLDIR)/usr/adbyby
	install -m 0755 usr/adbyby/adhook.ini $(INSTALLDIR)/usr/adbyby
	install -m 0755 usr/adbyby/delstart.sh1 $(INSTALLDIR)/usr/adbyby
	install -m 0755 usr/adbyby/start.sh $(INSTALLDIR)/usr/adbyby
	install -m 0755 usr/adbyby/update.info $(INSTALLDIR)/usr/adbyby
	install -m 0755 usr/adbyby/user.action $(INSTALLDIR)/usr/adbyby
	install -m 0755 usr/adbyby/data/clean.ini $(INSTALLDIR)/usr/adbyby/data
	install -m 0755 usr/adbyby/data/lazy.bin $(INSTALLDIR)/usr/adbyby/data
	install -m 0755 usr/adbyby/data/lazy.txt $(INSTALLDIR)/usr/adbyby/data
	install -m 0755 usr/adbyby/data/user.txt $(INSTALLDIR)/usr/adbyby/data
	install -m 0755 usr/adbyby/data/video.txt $(INSTALLDIR)/usr/adbyby/data
	install -m 0755 usr/adbyby/doc/hidecss.js $(INSTALLDIR)/usr/adbyby/doc

# K3C dns
	install -m 0755 usr/Pcap_DNSProxy $(INSTALLDIR)/usr/sbin
	install -m 0755 usr/pcap-dnsproxy/dnsmasq.conf $(INSTALLDIR)/usr/sbin/pcap-dnsproxy
	install -m 0755 usr/pcap-dnsproxy/Config.conf $(INSTALLDIR)/usr/sbin/pcap-dnsproxy
	install -m 0755 usr/pcap-dnsproxy/Hosts.conf $(INSTALLDIR)/usr/sbin/pcap-dnsproxy
	install -m 0755 usr/pcap-dnsproxy/IPFilter.conf $(INSTALLDIR)/usr/sbin/pcap-dnsproxy
	install -m 0755 usr/pcap-dnsproxy/Routing.txt $(INSTALLDIR)/usr/sbin/pcap-dnsproxy
	install -m 0755 usr/pcap-dnsproxy/WhiteList.txt $(INSTALLDIR)/usr/sbin/pcap-dnsproxy
# K3C ipset
	install -m 0755 ipset $(INSTALLDIR)/usr/sbin
	install -m 0755 libipset.so.3.6.0 $(INSTALLDIR)/usr/lib
	install -m 0755 libmnl.so.0.1.0 $(INSTALLDIR)/usr/lib
	cd $(INSTALLDIR)/usr/lib && ln -sf libipset.so.3.6.0 libipset.so.3 && ln -sf libipset.so.3 libipset.so && ln -sf libmnl.so.0.1.0 libmnl.so.0

# K3C Entware setup script
	install -m 0755 entware-setup.sh $(INSTALLDIR)/usr/sbin

# K3C webshell
	install -m 0755 www/k3c_webshell.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/shellinaboxd $(INSTALLDIR)/usr/sbin/webshell
	install -m 0755 www/white-on-black.css $(INSTALLDIR)/usr/sbin/webshell
	install -m 0755 www/Tools_webshell.asp $(INSTALLDIR)/www
	install -m 0755 www/certificate.pem $(INSTALLDIR)/usr/sbin/webshell
# K3C ngrok
	install -m 0755 www/k3c_ngrok.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/ngrokc $(INSTALLDIR)/usr/sbin
	install -m 0755 www/Tools_ngrok.asp $(INSTALLDIR)/www
# K3C xunlei
	install -m 0755 www/k3c_xunlei.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/Tools_xunlei.asp $(INSTALLDIR)/www
# K3C kms
	install -m 0755 www/vlmcsd.ini $(INSTALLDIR)/usr/kms
	install -m 0755 www/kms_dns.conf $(INSTALLDIR)/usr/kms
	install -m 0755 www/k3c_kms.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/vlmcsd $(INSTALLDIR)/usr/sbin
	install -m 0755 www/Tools_Kms.asp $(INSTALLDIR)/www
# K3C aria2
	install -m 0755 www/Tools_aria2.asp $(INSTALLDIR)/www
	install -m 0755 www/aria2c $(INSTALLDIR)/usr/sbin
	install -m 0755 www/k3c_aria2.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 libssh2.so.1.0.1 $(INSTALLDIR)/usr/lib
	cd $(INSTALLDIR)/usr/lib && ln -sf libssh2.so.1.0.1 libssh2.so.1 && ln -sf libssh2.so.1 libssh2.so
# K3C frp
	install -m 0755 www/Tools_frpc.asp $(INSTALLDIR)/www
	install -m 0755 www/frpc $(INSTALLDIR)/usr/sbin/frp
	install -m 0755 www/frp_mon.sh $(INSTALLDIR)/usr/sbin/frp
	install -m 0755 www/k3c_frpc.sh $(INSTALLDIR)/usr/sbin

clean:
