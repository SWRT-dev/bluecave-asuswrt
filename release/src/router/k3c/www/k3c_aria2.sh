#!/bin/sh


mdisk=`nvram get k3c_disk 2>/dev/null`
usb_disk="/tmp/mnt/$mdisk"
usbmount=`ls /tmp/mnt/`
aria2ver=`nvram get k3c_aria2ver 2>/dev/null`
aria2en=`nvram get k3c_aria2en 2>/dev/null`


start()
{
#开机启动太早会卡？
if [ "$usbmount" == "" ];then
    echo " $(date "+%F %T"):""系统正在启动，等待USB设备挂载中！" >> /tmp/aria2.log
fi
while [ "$usbmount" == "" ]
do
	sleep 5s
done
aria2verold=`$usb_disk/aria2c -v | grep "aria2 version" | cut -c14-20`

if [ "$aria2verold" != "$aria2ver" ];then
        echo "$(date "+%F %T"):" "版本不同，下载中......" >> /tmp/aria2.log
        wget --no-check-certificate --timeout=10 --tries=3 https://k3c.paldier.tk/tools/aria2c -O /tmp/aria2c
        [ "$?" != "0" ] && echo "$(date "+%F %T"):" "下载失败" >> /tmp/aria2.log
        mv -f /tmp/aria2c $usb_disk/aria2c
        chmod 755 $usb_disk/aria2c
        echo "$(date "+%F %T"):" "下载成功" >> /tmp/aria2.log
        aria2verold=`$usb_disk/aria2c -v | grep "aria2 version" | cut -c14-20`
        nvram set k3c_aria2ver="$aria2verold"
else
	echo "$(date "+%F %T"):" "版本相同，无需下载!" >> /tmp/aria2.log
fi

open_port
$usb_disk/aria2c --conf-path=/tmp/etc/aria2.conf -D >/dev/null 2>&1 &
}

stop()
{
killall -q -9 aria2c >/dev/null 2>&1
close_port
}

# start aria2c
creat_conf(){
cat > /tmp/etc/aria2.conf <<EOF
`dbus list aria2 | grep -vw aria2_enable | grep -vw aria2_binary| grep -vw aria2_binary_custom | grep -vw aria2_check | grep -vw aria2_check_time | grep -vw aria2_sleep | grep -vw aria2_update_enable| grep -vw aria2_update_sel | grep -vw aria2_version | grep -vw aria2_version_web | grep -vw aria2_warning | grep -vw aria2_custom | grep -vw aria2_install_status|grep -vw aria2_restart |grep -vw aria2_dir| sed 's/aria2_//g' | sed 's/_/-/g'`
`dbus list aria2|grep -w aria2_dir|sed 's/aria2_//g'`
EOF

cat >> /tmp/etc/aria2.conf <<EOF
`dbus list aria2|grep -w aria2_custom|sed 's/aria2_custom=//g'|sed 's/,/\n/g'`
EOF

# if [ "$aria2_enable_rpc" = "false" ];then
# sed -i '/rpc/d' /koolshare/aria2/aria2.conf
# fi
}


# generate token
generate_token(){
	if [ -z $aria2_rpc_secret ];then
		sed -i "s/rpc-secret=/rpc-secret=$token/g" "/tmp/etc/aria2.conf"
		dbus set aria2_rpc_secret="$token"
	fi
}

# open firewall port
open_port(){
	echo open firewall port $aria2_rpc_listen_port and 8088
	iptables -I INPUT -p tcp --dport $aria2_rpc_listen_port -j ACCEPT >/dev/null 2>&1
	iptables -I INPUT -p tcp --dport 8088 -j ACCEPT >/dev/null 2>&1
	iptables -I INPUT -p tcp --dport 6881:6889 -j ACCEPT >/dev/null 2>&1
	iptables -I INPUT -p tcp --dport 51413 -j ACCEPT >/dev/null 2>&1
	iptables -I INPUT -p tcp --dport 52413 -j ACCEPT >/dev/null 2>&1
	iptables -I INPUT -p udp --dport 52413 -j ACCEPT >/dev/null 2>&1
	echo done
}

# close firewall port
close_port(){
	echo close firewall port $aria2_rpc_listen_port and 8088
	iptables -D INPUT -p tcp --dport $aria2_rpc_listen_port -j ACCEPT >/dev/null 2>&1
	iptables -D INPUT -p tcp --dport 8088 -j ACCEPT >/dev/null 2>&1
	iptables -D INPUT -p tcp --dport 6881:6889 -j ACCEPT >/dev/null 2>&1
	iptables -D INPUT -p tcp --dport 51413 -j ACCEPT >/dev/null 2>&1
	iptables -D INPUT -p tcp --dport 52413 -j ACCEPT >/dev/null 2>&1
	iptables -D INPUT -p udp --dport 52413 -j ACCEPT >/dev/null 2>&1
	echo done
}


load_default(){
	del_version_check
	kill_aria2
	close_port
	dbus set tmp_aria2_version=`dbus get aria2_version`
	dbus set tmp_aria2_version_web=`dbus get aria2_version_web`
	for r in `dbus list aria2|cut -d"=" -f 1`
	do
	dbus remove $r
	done
	dbus set aria2_enable=0
	dbus set aria2_install_status=1
	dbus set aria2_version=`dbus get tmp_aria2_version`
	dbus set aria2_version_web=`dbus get tmp_aria2_version_web`
	dbus remove tmp_aria2_version
}
# ============================================

restart()
{
	kenable=`nvram get k3c_enable`
	if [ "$aria2en" == "1" -a "$kenable" == "1" ] ;then
		stop
        sleep 2s
        echo "$(date "+%F %T"):"  "aria2已开启！" >> /tmp/aria2.log
		start
	elif [ "$aria2en" == "1" ] ;then
                logger -t "K3C""K3C扩展设置挂载未开启！"
                echo "$(date "+%F %T"):"  "K3C扩展设置挂载未开启！" >> /tmp/aria2.log
                stop
		exit 0
	else
                stop
                echo "$(date "+%F %T"):"  "aria2已关闭！" >> /tmp/aria2.log
	fi
}

restart
