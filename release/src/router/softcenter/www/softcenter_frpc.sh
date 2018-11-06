#!/bin/sh

frpc_sip=`nvram get frpc_srcip 2>/dev/null`
frpc_user=`nvram get frpc_user 2>/dev/null`
frpc_vernum=`nvram get frpc_vernum 2>/dev/null`
mdisk=`nvram get k3c_disk 2>/dev/null`
usb_disk="/tmp/mnt/$mdisk"
usbmount=`ls /tmp/mnt/`

stop() {
killall -q -9 frp_mon.sh >/dev/null 2>&1
killall -q -9 frpc >/dev/null 2>&1
if [ -n "`cru l|grep restart_frpc`" ];then
	cru d restart_frpc
fi
}

start() {
#
#开机启动太早会卡？
if [ "$usbmount" == "" ];then
    echo " $(date "+%F %T"):""系统正在启动，等待USB设备挂载中！" >> /tmp/frpc.log
fi
while [ "$usbmount" == "" ]
do
	sleep 5s
	usbmount=`ls /tmp/mnt/ |grep $mdisk`
done
lan_ipaddr=$(nvram get lan_ipaddr)
mserver=`nvram get frpc_server`
mport=`nvram get frpc_port`
mtoken=`nvram get frpc_token`
mrulelist=`nvram get frpc_rulelist`

download_frp

if [ "$frpc_user" = "1" ] ;then
	echo $frpc_sip > /tmp/frpc_sip.ini
	sed -i "s/,yushi,/\n/g" /tmp/frpc_sip.ini
	$usb_disk/frp/frpc -c /tmp/frpc_sip.ini 2>&1 &
	echo "$(date "+%F %T"):" "frpc使用自定义配置启动!" >> /tmp/frpc.log
else

let mloc="2"

mcmd="echo \"$mrulelist\"|awk -F '<' '{printf \$$mloc}'"

mstr=`echo $mcmd |sh`

mtype=`echo "$mstr"|awk -F'>' '{printf $1}'`
mhost=`echo "$mstr"|awk -F'>' '{printf $3}'`
mlport=`echo "$mstr"|awk -F'>' '{printf $4}'`
mname=`echo "$mstr"|awk -F'>' '{printf $2}'`
mrport=`echo "$mstr"|awk -F'>' '{printf $5}'`

if [ -z "$mstr"  ] ;then
exit 0
fi

if [ "$mtype" = "http" ] ;then
webname=web${mname}
# creating frpc.ini
cat > /tmp/.frpc.ini <<-EOF
# http://www.frps.top/
[common]
server_addr = ${mserver}
server_port = ${mport}
privilege_token = ${mtoken}
log_file = /tmp/frpc.log
log_level = info
log_max_days = 3
#tcp_mux = ${frpc_common_tcp_mux}
#protocol = ${frpc_common_protocol}
#login_fail_exit = ${frpc_common_login_fail_exit}
#user = ${frpc_common_user}

[${webname}]
type = ${mtype}
local_ip = ${mhost}
local_port = ${mlport}
subdomain = ${mname}
EOF
fi

if [ "$mtype" = "tcp" ] ;then
sshname=ssh${mname}
#
cat >> /tmp/.frpc.ini <<-EOF

[${sshname}]
type = ${mtype}
local_port = ${mlport}
remote_port = ${mrport}
EOF
fi

$usb_disk/frp/frpc -c /tmp/.frpc.ini 2>&1 &
echo "$(date "+%F %T"):" "frpc使用列表配置启动!" >> /tmp/frpc.log
fi

cru a restart_frpc "18 5 * * * /usr/sbin/softcenter_frpc.sh"
echo "$(date "+%F %T"):" "每天5:18自动重启!" >> /tmp/frpc.log
/usr/sbin/frp_mon.sh &
echo "$(date "+%F %T"):" "守护进程启动!" >> /tmp/frpc.log

echo "$(date "+%F %T"):" "frpc 当前版本: v`$usb_disk/frp/frpc --version 2>/dev/null | head -n1 | awk '{print $1}'`" >> /tmp/frpc.log
echo "$(date "+%F %T"):" "frpc 启动完毕!" >> /tmp/frpc.log

exit 0
}

download_frp(){
#
	[ "$frpc_vernum" == "0" ] && FRPCver="0.18.0"
	[ "$frpc_vernum" == "1" ] && FRPCver="0.17.0"
	[ "$frpc_vernum" == "2" ] && FRPCver="0.16.1"
	[ "$frpc_vernum" == "3" ] && FRPCver="0.9.3"
	[ "$frpc_vernum" == "4" ] && FRPCver="0.20.0"

	[ -n "$FRPCver" ] && VER="$FRPCver" || VER="0.20.0"
	Tmpfrp=/tmp/frpc
	tarfile=frp_"$VER"_linux_mips.tar.gz
	Frp_bin1=https://github.com/fatedier/frp/releases/download/v"$VER"/$tarfile
	Frp_bin2=https://k3c.paldier.com/tools/$tarfile
	FRP_VER=`$usb_disk/frp/frpc --version 2>/dev/null | head -n 1 | cut -d " " -f1` || 0
	d_rule() {
		wget --no-check-certificate --timeout=10 --tries=3 -qO $1 $2
	}
	[ ! -d $usb_disk/frp ] && mkdir -p $usb_disk/frp && chmod 755 $usb_disk/frp
	if [ "$FRP_VER" != "$VER" ]; then
		echo "$(date "+%F %T"):" "本地版本 v$FRP_VER 与选择版本 v$VER 不同，下载 v$VER ......" >> /tmp/frpc.log
		icount=`ps -w|grep frpc |grep -v grep|wc -l`
		[ $icount != 0 ] && stop && sleep 1
		[ -f "$usb_disk/frp/frpc" ] && rm -f $usb_disk/frp/frpc
		cd /tmp		
		if [ ! -s /tmp/$tarfile ]; then
			d_rule $tarfile $Frp_bin1
			[ "$?" != "0" ] && sleep 2 && d_rule $tarfile $Frp_bin2 && \
			[ "$?" != "0" ] && logger -t "【frpc】" "v$VER 下载失败" && echo "$(date "+%F %T"):" "frpc v$VER 下载失败" >> /tmp/frpc.log && exit 1
			tar zxvf /tmp/$tarfile -C /tmp && mv -f /tmp/frp_"$VER"_linux_mips/frpc $Tmpfrp
		fi
		if [ -f "$Tmpfrp" ]; then
			chmod 755 $Tmpfrp
			mv -f $Tmpfrp $usb_disk/frp/frpc
			echo "$(date "+%F %T"):" "frpc v$VER 下载成功" >> /tmp/frpc.log
		fi
		rm -rf /tmp/$tarfile /tmp/frp_"$VER"_linux_mips /tmp/frp >/dev/null 2>&1
	else
		echo "$(date "+%F %T"):" "本地版本 v$FRP_VER 与选择版本 v$VER 相同，无需下载!" >> /tmp/frpc.log
	fi
	
}

restart() {
	menable=`nvram get frpc_enable`
	kenable=`nvram get k3c_enable`
	if [ "$menable" == "1" ] ;then
		stop
		if [ "$kenable" == "1" ] ;then
			start
		else
			logger -t "K3C""K3C扩展设置挂载未开启！"
			echo "$(date "+%F %T"):"  "K3C扩展设置挂载未开启！" >> /tmp/frpc.log
			exit 0
		fi
	else
		stop
		echo "$(date "+%F %T"):"  "frpc已关闭！" >> /tmp/frpc.log
	fi
}

restart

