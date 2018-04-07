#!/bin/sh

stop()
{
killall -q -9 frpc >/dev/null 2>&1
killall -q -9 frp_mon.sh >/dev/null 2>&1
if [ -n "`cru l|grep restart_frpc`" ];then
		cru d restart_frpc
fi
[ -f /tmp/frpc.log ] && rm -f /tmp/frpc.log

}

start()
{
	killall -q -9 frp_mon.sh >/dev/null 2>&1
		
# creating frpc.ini
if [ ! -f /jffs/frp/frpc.ini ]; then
cat > /jffs/frp/frpc.ini <<-EOF
# http://cc.nokc.cc:82
[common]
server_addr = nokc.cc
server_port = 5443
privilege_token = cc.nokc.cc

#[sshxx]
## sshxx Must be modified
#type = tcp
#local_port = 22
#remote_port = 5000

#[webxx]
## webxxh Must be modified
#type = http
#local_ip = ${lan_ipaddr}
#local_port = 80
#subdomain = xx
## xx Must be modified
EOF
fi

[ -f /jffs/frp/frpc.ini ] && chmod 644 /jffs/frp/frpc.ini

/usr/sbin/frp/frpc -c /jffs/frp/frpc.ini -L /tmp/frpc.log &
/usr/sbin/frp/frp_mon.sh & 
sleep 1
mauto=`nvram get frpc_auto`
if [ "$mauto" = "0"  ] ;then
	cru a restart_frpc "28 4 * * * /usr/sbin/k3c_frpc.sh 2>/dev/null"
	echo "$(date "+%F %T"):" "Enable automatic restart!" >> /tmp/frpc.log
	logger -t "frpc:" "Enable automatic restart every day!"
else
	if [ -n "`cru l|grep restart_frpc`" ];then
		cru d restart_frpc
	fi
	echo "$(date "+%F %T"):" "Do not restart!" >> /tmp/frpc.log
fi
logger -t "frpc" "started!"
exit 0
}

restart() {
	stop
	sleep 1
	menable=`nvram get frpc_enable`
	if [ "$menable" == "1" ] ;then
		start
	fi
}

restart

