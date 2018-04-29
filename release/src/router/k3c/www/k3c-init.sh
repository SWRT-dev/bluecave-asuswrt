#!/bin/sh 

#载入iptables模块
insmod /lib/modules/3.10.104/kernel/net/netfilter/nfnetlink.ko
insmod /lib/modules/3.10.104/kernel/net/netfilter/ipset/ip_set.ko
insmod /lib/modules/3.10.104/kernel/net/netfilter/ipset/ip_set_bitmap_ip.ko
insmod /lib/modules/3.10.104/kernel/net/netfilter/ipset/ip_set_bitmap_ipmac.ko
insmod /lib/modules/3.10.104/kernel/net/netfilter/ipset/ip_set_bitmap_port.ko
insmod /lib/modules/3.10.104/kernel/net/netfilter/ipset/ip_set_hash_ip.ko
insmod /lib/modules/3.10.104/kernel/net/netfilter/ipset/ip_set_hash_ipport.ko
insmod /lib/modules/3.10.104/kernel/net/netfilter/ipset/ip_set_hash_ipportip.ko
insmod /lib/modules/3.10.104/kernel/net/netfilter/ipset/ip_set_hash_ipportnet.ko
insmod /lib/modules/3.10.104/kernel/net/netfilter/ipset/ip_set_hash_net.ko
insmod /lib/modules/3.10.104/kernel/net/netfilter/ipset/ip_set_hash_netiface.ko
insmod /lib/modules/3.10.104/kernel/net/netfilter/ipset/ip_set_hash_netport.ko
insmod /lib/modules/3.10.104/kernel/net/netfilter/ipset/ip_set_list_set.ko
insmod /lib/modules/3.10.104/kernel/net/netfilter/xt_set.ko
insmod /lib/modules/3.10.104/kernel/net/netfilter/xt_TPROXY.ko

/usr/sbin/k3c_webshell.sh &
/usr/sbin/k3c_xunlei.sh &
/usr/sbin/k3c_ngrok.sh
/usr/sbin/k3c_frpc.sh
/usr/sbin/k3c_ssr.sh
/usr/sbin/k3c_kms.sh
/usr/sbin/k3c_adbyby.sh

