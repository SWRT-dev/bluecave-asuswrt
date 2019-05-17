#!/bin/sh 

modecheck=`nvram get sw_mode`
[ "$modecheck" != "1" ] && logger -t "软件中心" "非路由模式不支持插件！" && exit 0
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

/usr/sbin/softcenter_webshell.sh &
/usr/sbin/softcenter_xunlei.sh &
/usr/sbin/softcenter_kms.sh &
/usr/sbin/softcenter_tools.sh &
kenable=`nvram get k3c_enable`
if [ "$kenable" == "1" ];then
/usr/sbin/softcenter_swap.sh &
/usr/sbin/softcenter_ngrok.sh &
/usr/sbin/softcenter_frpc.sh &
/usr/sbin/softcenter_adbyby.sh &
/usr/sbin/softcenter_start.sh &
else
  logger -t "软件中心" "jffs扩展挂载未开启！"
fi

