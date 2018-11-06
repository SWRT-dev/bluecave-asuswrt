include ../common.mak

all:

install:
	install -d $(INSTALLDIR)/sbin
	install -d $(INSTALLDIR)/usr/sbin
	install -d $(INSTALLDIR)/lib
	install -d $(INSTALLDIR)/usr/bin
	install -d $(INSTALLDIR)/usr/sbin/ssr
	install -d $(INSTALLDIR)/usr/lib
	install -d $(INSTALLDIR)/usr/sbin/webshell
	install -d $(INSTALLDIR)/usr/kool
	install -d $(INSTALLDIR)/www
	install -d $(INSTALLDIR)/usr/kms

# softcenter update uboot&ubootconfig tool
	install -m 0755 mtd $(INSTALLDIR)/sbin
# softcenter softcenter
	install -m 0755 www/Softcenter.asp $(INSTALLDIR)/www
# softcenter SSR and adbyby plus
	install -m 0755 base64 $(INSTALLDIR)/usr/bin
	install -m 0755 ssr-redir $(INSTALLDIR)/usr/sbin
	install -m 0755 ss-redir $(INSTALLDIR)/usr/sbin
	install -m 0755 ssr-rules $(INSTALLDIR)/usr/sbin
	install -m 0755 ssr-state $(INSTALLDIR)/usr/sbin
	install -m 0755 usr/ssr/gfw_addr.conf $(INSTALLDIR)/usr/sbin/ssr
	install -m 0755 usr/ssr/gfw_list.conf $(INSTALLDIR)/usr/sbin/ssr
	install -m 0755 usr/ssr/ignore.list $(INSTALLDIR)/usr/sbin/ssr
	install -m 0755 usr/ssr/ignore.user $(INSTALLDIR)/usr/sbin/ssr
	install -m 0755 usr/ssr/ssr_mon.sh $(INSTALLDIR)/usr/sbin/ssr
	install -m 0755 www/softcenter-init.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/softcenter_ssr.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/Tools_Adbyby.asp $(INSTALLDIR)/www
	install -m 0755 www/softcenter_adbyby.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/user.txt $(INSTALLDIR)/usr/kool
	install -m 0755 www/openssl.cnf $(INSTALLDIR)/usr/kool
	install -m 0755 www/kool_mon.sh $(INSTALLDIR)/usr/kool
	install -m 0755 www/kool.sh $(INSTALLDIR)/usr/kool
	install -m 0755 www/gen_ca.sh $(INSTALLDIR)/usr/kool
	install -m 0755 www/adblock.conf $(INSTALLDIR)/usr/kool
	install -m 0755 nohup $(INSTALLDIR)/usr/bin

# softcenter dns
	install -m 0755 usr/pdnsd $(INSTALLDIR)/usr/sbin

# softcenter ipset
	install -m 0755 ipset $(INSTALLDIR)/usr/sbin
	install -m 0755 libipset.so.3.6.0 $(INSTALLDIR)/usr/lib
	install -m 0755 libcares.so.2.2.0 $(INSTALLDIR)/usr/lib
	install -m 0755 libmbedcrypto.so.2.6.0 $(INSTALLDIR)/usr/lib
	install -m 0755 libmnl.so.0.1.0 $(INSTALLDIR)/usr/lib
	install -m 0755 libsodium.so.18.2.0 $(INSTALLDIR)/usr/lib
	cd $(INSTALLDIR)/usr/lib && ln -sf libipset.so.3.6.0 libipset.so.3 && ln -sf libipset.so.3 libipset.so && ln -sf libmnl.so.0.1.0 libmnl.so.0 && ln -sf libcares.so.2.2.0 libcares.so.2 && ln -sf libmbedcrypto.so.2.6.0 libmbedcrypto.so.0 && ln -sf libmbedcrypto.so.0 libmbedcrypto.so && ln -sf libsodium.so.18.2.0 libsodium.so.18

# softcenter Entware setup script
	#install -m 0755 entware-setup.sh $(INSTALLDIR)/usr/sbin

# softcenter webshell
	install -m 0755 www/softcenter_webshell.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/shellinaboxd $(INSTALLDIR)/usr/sbin/webshell
	install -m 0755 www/white-on-black.css $(INSTALLDIR)/usr/sbin/webshell
	install -m 0755 www/Tools_webshell.asp $(INSTALLDIR)/www
	install -m 0755 www/certificate.pem $(INSTALLDIR)/usr/sbin/webshell
# softcenter ngrok
	install -m 0755 www/softcenter_ngrok.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/ngrokc $(INSTALLDIR)/usr/sbin
	install -m 0755 www/Tools_ngrok.asp $(INSTALLDIR)/www
# softcenter xunlei
	install -m 0755 www/softcenter_xunlei.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/Tools_xunlei.asp $(INSTALLDIR)/www
# softcenter kms
	install -m 0755 www/vlmcsd.ini $(INSTALLDIR)/usr/kms
	install -m 0755 www/kms_dns.conf $(INSTALLDIR)/usr/kms
	install -m 0755 www/softcenter_kms.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/vlmcsd $(INSTALLDIR)/usr/sbin
	install -m 0755 www/Tools_Kms.asp $(INSTALLDIR)/www
# softcenter aria2
	install -m 0755 www/Tools_aria2.asp $(INSTALLDIR)/www
	#install -m 0755 www/aria2c $(INSTALLDIR)/usr/sbin
	#install -m 0755 www/softcenter_aria2.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 libssh2.so.1.0.1 $(INSTALLDIR)/usr/lib
	cd $(INSTALLDIR)/usr/lib && ln -sf libssh2.so.1.0.1 libssh2.so.1 && ln -sf libssh2.so.1 libssh2.so
# softcenter frp
	install -m 0755 www/Tools_frpc.asp $(INSTALLDIR)/www
	#install -m 0755 www/frpc $(INSTALLDIR)/usr/sbin/frp
	install -m 0755 www/frp_mon.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/softcenter_frpc.sh $(INSTALLDIR)/usr/sbin
# softcenter v2ray
	install -m 0755 www/Tools_v2ray.asp $(INSTALLDIR)/www
# softcenter swap
	install -m 0755 www/Tools_swap.asp $(INSTALLDIR)/www
	install -m 0755 www/softcenter_swap.sh $(INSTALLDIR)/usr/sbin
# softcenter swap
	install -m 0755 www/Tools_softcenter.asp $(INSTALLDIR)/www
	install -m 0755 www/softcenter_tools.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/softcenterex_start.sh $(INSTALLDIR)/usr/sbin
# softcenter script
	install -m 0755 www/softcenter_script.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/softcenter_script_ch.sh $(INSTALLDIR)/usr/sbin
	install -m 0755 www/Tools_Script.asp $(INSTALLDIR)/www

clean:

