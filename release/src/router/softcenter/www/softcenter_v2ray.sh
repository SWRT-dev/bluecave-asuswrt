#!/bin/sh

udp_enable=`nvram get  v2ray_udp_enable`
v2ray_user=`nvram get v2ray_user`
v2ray_sip=`nvram get v2ray_srcip`
dns_mode=`nvram get v2ray_dnsmode`
ss_dns_china=`nvram get v2ray_dns`
mdisk=`nvram get k3c_disk`
usb_disk="/tmp/mnt/$mdisk"
usbmount=`ls /tmp/mnt/`
V2RAY_CONFIG_FILE="/tmp/etc/v2rayconfig.json"
TEMP_CONFIG_FILE="/tmp/v2rayconfig.pb"

gen_conf() {
json_data=`/jffs/softcenter/bin/jq . $V2RAY_CONFIG_FILE`
/jffs/softcenter/bin/jq -e . $V2RAY_CONFIG_FILE >/dev/null 2>&1 || return 2
local json_key="socks"
local json_value="\"dokodemo-door\""
local line_data=$(echo "$json_data" | grep -w "$json_key" )
[ "${line_data:$((${#line_data}-1))}" = "," ] && json_value="${json_value},"
local json_new_data=$(echo "$json_data" | sed "s/$line_data/    \"protocol\": $json_value/")
echo "$json_new_data" | /jffs/softcenter/bin/jq -e . >/dev/null 2>&1 || return 3
json_data="$json_new_data" && echo "$json_data" > $V2RAY_CONFIG_FILE
}
gen_conf1() {
json_data=`/jffs/softcenter/bin/jq . $V2RAY_CONFIG_FILE`
/jffs/softcenter/bin/jq -e . $V2RAY_CONFIG_FILE >/dev/null 2>&1 || return 2
local json_key="1080,"
local json_value="1234"
local line_data=$(echo "$json_data" | grep -w "$json_key" )
[ "${line_data:$((${#line_data}-1))}" = "," ] && json_value="${json_value},"
local json_new_data=$(echo "$json_data" | sed "s/$line_data/    \"port\": $json_value/")
echo "$json_new_data" | /jffs/softcenter/bin/jq -e . >/dev/null 2>&1 || return 3
json_data="$json_new_data" && echo "$json_data" > $V2RAY_CONFIG_FILE
}
gen_conf2() {
json_data=`/jffs/softcenter/bin/jq . $V2RAY_CONFIG_FILE`
/jffs/softcenter/bin/jq -e . $V2RAY_CONFIG_FILE >/dev/null 2>&1 || return 2
local json_key="127.0.0.1"
local json_value="      \"followRedirect\": true"
local line_data=$(echo "$json_data" | grep -w "$json_key" )
[ "${line_data:$((${#line_data}-1))}" = "," ] && json_value="${json_value}," && json_key="\"${json_key}\","
local json_new_data=$(echo "$json_data" | sed "s/$line_data/      \"ip\": $json_key\n$json_value/")
json_data="$json_new_data" && echo "$json_data" > $V2RAY_CONFIG_FILE
}

download_v2ray(){
#
	PKG_VERSION=$(wget --no-check-certificate https://api.github.com/repos/v2ray/v2ray-core/releases/latest -q -O -|grep tag_name|awk {'print $2'}|cut -d '"' -f 2)
	V2RA_VER=`/jffs/softcenter/bin/v2ray --version 2>/dev/null | head -n1 | awk '{print $2}'` || 0
	Tmpv2ray=v2ray
	Tmpv2ctl=v2ctl
	tarfile=v2ray-linux-mips.zip
	v2ray_bin=https://github.com/v2ray/v2ray-core/releases/download/"$PKG_VERSION"/$tarfile
	v2ray_bin2=https://k3c.paldier.com/tools/$tarfile
	d_rule() {
		wget --no-check-certificate --timeout=10 --tries=3 -qO $1 $2
	}
	echo "$(date "+%F %T") 在线版本: $PKG_VERSION" >> /tmp/v2ray.log
	echo "$(date "+%F %T") 本地版本: $V2RA_VER" >> /tmp/v2ray.log
	logger -t "【v2ray】" "在线版本 $PKG_VERSION"
	logger -t "【v2ray】" "本地版本 $V2RA_VER"
	if [ "v$V2RA_VER" != "$PKG_VERSION" ]; then
		logger -t "【v2ray】" "本地版本与在线版本不同，下载 $PKG_VERSION ......"
		echo "$(date "+%F %T"): 本地版本与在线版本不同，下载 $PKG_VERSION ......" >> /tmp/v2ray.log
		cd /tmp
		d_rule $tarfile $v2ray_bin
		[ "$?" != "0" ] && sleep 2 && d_rule $tarfile $v2ray_bin2 && \
			[ "$?" != "0" ] && logger -t "【v2ray】" "$PKG_VERSION 下载失败" && echo "$(date "+%F %T"): $PKG_VERSION 下载失败" >> /tmp/v2ray.log && exit 1
		mkdir /tmp/v2ray-"$PKG_VERSION"-linux-mips
		unzip -o /tmp/$tarfile -d /tmp/v2ray-"$PKG_VERSION"-linux-mips
		mv /tmp/v2ray-"$PKG_VERSION"-linux-mips/v2ray_softfloat /jffs/softcenter/bin/v2ray && chmod 755 /jffs/softcenter/bin/v2ray
		mv /tmp/v2ray-"$PKG_VERSION"-linux-mips/v2ctl_softfloat /jffs/softcenter/bin/v2ctl && chmod 755 /jffs/softcenter/bin/v2ctl
		mv /tmp/v2ray-"$PKG_VERSION"-linux-mips/geosite.dat /jffs/softcenter/bin/geosite.dat && chmod 755 /jffs/softcenter/bin/geosite.dat
		mv /tmp/v2ray-"$PKG_VERSION"-linux-mips/geoip.dat /jffs/softcenter/bin/geoip.dat && chmod 755 /jffs/softcenter/bin/geoip.dat
		if [ ! -e "/jffs/softcenter/bin/jq" ] ;then
		wget --no-check-certificate --timeout=10 --tries=3 -qO /jffs/softcenter/bin/jq http://k3c.paldier.com/tools/jq
		wget --no-check-certificate --timeout=10 --tries=3 -qO /jffs/softcenter/bin/dns2socks http://k3c.paldier.com/tools/dns2socks
		chmod 755 /jffs/softcenter/bin/jq >/dev/null 2>&1
		chmod 755 /jffs/softcenter/bin/dns2socks >/dev/null 2>&1
		fi
		jqmd5=`md5sum /jffs/softcenter/bin/jq |awk  '{print $1}'`
		if [ "$jqmd5" != "91d61fbe4378a0d077109f9c9047dffa" ] ;then
			wget --no-check-certificate --timeout=10 --tries=3 -qO /jffs/softcenter/bin/jq http://k3c.paldier.com/tools/jq
			[ "$?" != "0" ] && sleep 2 && wget --no-check-certificate --timeout=10 --tries=3 -qO /jffs/softcenter/bin/jq http://k3c.paldier.com/tools/jq
			[ "$?" != "0" ] && echo "$(date "+%F %T"): jq 下载失败" >> /tmp/v2ray.log && exit 1
		fi
		logger -t "【v2ray】" "$PKG_VERSION 下载成功!"
		echo "$(date "+%F %T"): $PKG_VERSION 下载成功" >> /tmp/v2ray.log
		rm -rf /tmp/$tarfile /tmp/v2ray-"$PKG_VERSION"-linux-mips >/dev/null 2>&1
	fi
}

v2ray_test(){
echo "$(date "+%F %T"): 测试V2Ray配置文件....." >> /tmp/v2ray.log
result=$(/jffs/softcenter/bin/v2ray -test -config="$V2RAY_CONFIG_FILE" | grep "Configuration OK.")
if [ -n "$result" ];then
	echo $result
	echo "$(date "+%F %T"): V2Ray配置文件通过测试!!!" >> /tmp/v2ray.log
	logger -t "【v2ray】" "配置文件通过测试!"
	[ "$v2ray_user" = "1" ] && {
		cp -f "$V2RAY_CONFIG_FILE" /jffs/softcenter/etc/v2rayconfig.json.bak
		echo "$(date "+%F %T"): V2Ray配置文件已备份!!!" >> /tmp/v2ray.log
		echo "$(date "+%F %T"): 运行正常后请关闭启用配置选项，不然每重启均保存一次!" >> /tmp/v2ray.log
		logger -t "【v2ray】" "配置文件已备份!"
		logger -t "【v2ray】" "运行正常后请关闭启用配置选项，不然每重启均保存一次!"
	}
	/jffs/softcenter/bin/v2ctl config < "$V2RAY_CONFIG_FILE" > "$TEMP_CONFIG_FILE"
else
	echo "$(date "+%F %T"): V2Ray配置文件没有通过测试，请检查设置!!!" >> /tmp/v2ray.log
	logger -t "【v2ray】" "配置文件没有通过测试，请检查设置!"
	exit 1
fi

}

v2ray_serverip(){
# 检测用户json的服务器ip地址
v2ray_protocal=`cat "$V2RAY_CONFIG_FILE" | /jffs/softcenter/bin/jq -r .outbound.protocol`
case $v2ray_protocal in
	vmess)
		v2ray_server=`cat "$V2RAY_CONFIG_FILE" | /jffs/softcenter/bin/jq -r .outbound.settings.vnext[0].address`
		;;
	socks)
		v2ray_server=`cat "$V2RAY_CONFIG_FILE" | /jffs/softcenter/bin/jq -r .outbound.settings.servers[0].address`
		;;
	shadowsocks)
		v2ray_server=`cat "$V2RAY_CONFIG_FILE" | /jffs/softcenter/bin/jq -r .outbound.settings.servers[0].address`
		;;
	*)
		v2ray_server=""
		;;
esac

if [ -n "$v2ray_server" -a "$v2ray_server" != "null" ];then
	IFIP_VS=`echo $v2ray_server|grep -E "([0-9]{1,3}[\.]){3}[0-9]{1,3}|:"`
	if [ -n "$IFIP_VS" ];then
		v2ray_server_ip="$v2ray_server"
		echo "$(date "+%F %T"): 检测到你的json配置的v2ray服务器是：$v2ray_server"  >> /tmp/v2ray.log
	else
		echo "$(date "+%F %T"): 检测到你的json配置的v2ray服务器：$v2ray_server不是ip格式！"  >> /tmp/v2ray.log
		echo "$(date "+%F %T"): 为了确保v2ray的正常工作，建议配置ip格式的v2ray服务器地址！"  >> /tmp/v2ray.log
		echo "$(date "+%F %T"): 尝试解析v2ray服务器的ip地址..."  >> /tmp/v2ray.log
		v2ray_server_ip=`nslookup "$v2ray_server" 114.114.114.114 | sed '1,4d' | awk '{print $3}' | grep -v :|awk 'NR==1{print}'`
		if [ "$?" == "0" ]; then
			v2ray_server_ip=`echo $v2ray_server_ip|grep -E "([0-9]{1,3}[\.]){3}[0-9]{1,3}|:"`
		else
			echo "$(date "+%F %T"): v2ray服务器域名解析失败！"  >> /tmp/v2ray.log
			echo "$(date "+%F %T"): 尝试用resolveip方式解析..."  >> /tmp/v2ray.log
			v2ray_server_ip=`resolveip -4 -t 2 $ss_basic_server|awk 'NR==1{print}'`
			if [ "$?" == "0" ];then
				v2ray_server_ip=`echo $v2ray_server_ip|grep -E "([0-9]{1,3}[\.]){3}[0-9]{1,3}|:"`
			fi
		fi
		if [ -n "$v2ray_server_ip" ];then
			echo "$(date "+%F %T"): v2ray服务器的ip地址解析成功：$v2ray_server_ip"  >> /tmp/v2ray.log
			echo "address=/$v2ray_server/$v2ray_server_ip" > /etc/dnsmasq.user/ss_host.conf
			v2ray_server_ip="$v2ray_server_ip"
		else
			echo "$(date "+%F %T"): v2ray服务器的ip地址解析失败!插件将继续运行，域名解析将由v2ray自己进行！"  >> /tmp/v2ray.log
			echo "$(date "+%F %T"): 请自行将v2ray服务器的ip地址填入IP/CIDR白名单中!"  >> /tmp/v2ray.log
			echo "$(date "+%F %T"): 为了确保v2ray的正常工作，建议配置ip格式的v2ray服务器地址！" >> /tmp/v2ray.log
		fi
	fi
else
	echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> /tmp/v2ray.log
	echo "+       没有检测到你的v2ray服务器地址，如果你确定你的配置是正确的        +" >> /tmp/v2ray.log
	echo "+   请自行将v2ray服务器的ip地址填入黑名单中，以确保正常使用   +" >> /tmp/v2ray.log
	echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" >> /tmp/v2ray.log
fi
mip=$v2ray_server_ip
}

stop() {
killall -q -9 v2ray_mon.sh >/dev/null 2>&1 && killall v2ray_mon.sh >/dev/null 2>&1
killall -q -9 dns2socks 2>/dev/null && killall dns2socks 2>/dev/null
killall -q -9 v2ray 2>/dev/null && killall v2ray 2>/dev/null
killall -q pdnsd 2>/dev/null
service restart_dnsmasq >/dev/null 2>&1

}

start() {
killall -q -9 v2ray_mon.sh >/dev/null 2>&1
icount=`ps -w|grep v2rayconfig |grep -v grep|wc -l`

if [ $icount != 0 ] ;then
	stop
	sleep 2s
fi

if [ "$usbmount" == "" ];then
    echo " $(date "+%F %T"):""系统正在启动，等待USB设备挂载中！" >> /tmp/v2ray.log
fi
while [ "$usbmount" == "" ]
do
	sleep 5s
	usbmount=`ls /tmp/mnt/ |grep $mdisk`
done
[ "$ss_dns_china" == "0" ] && GFWCDN="208.67.222.222"
[ "$ss_dns_china" == "1" ] && GFWCDN="8.8.8.8"

download_v2ray

if [ "$v2ray_user" = "1" ] ;then
	echo $v2ray_sip > $V2RAY_CONFIG_FILE
	sed -i "s/,yushi,/\n/g" $V2RAY_CONFIG_FILE
	
gen_conf
gen_conf1
gen_conf2
else
	if [ -f /jffs/softcenter/etc/v2rayconfig.json.bak ];then
		rm -rf "$V2RAY_CONFIG_FILE"
		cp -f /jffs/softcenter/etc/v2rayconfig.json.bak "$V2RAY_CONFIG_FILE"
		echo "$(date "+%F %T"): V2Ray使用备份配置启动!!!" >> /tmp/v2ray.log
		logger -t "【v2ray】" "使用备份配置启动!"
	else
		echo "$(date "+%F %T"): V2Ray没选择启用配置，地球上也找不到备份配置，暂时休息!!!" >> /tmp/v2ray.log
		logger -t "【v2ray】" "没选择启用配置，地球上也找不到备份配置，暂时休息!"
		exit 1
	fi
fi

#
v2ray_test
v2ray_serverip

#if [ -f "$TEMP_CONFIG_FILE" ];then
	/jffs/softcenter/bin/v2ray -format pb -config "$TEMP_CONFIG_FILE" >/dev/null 2>&1 &
#else
#	/jffs/toolscript/ssr/v2ray -config "$V2RAY_CONFIG_FILE" >/dev/null 2>&1 &
#fi
if [ "$udp_enable" == "1" ];then
	echo "$(date "+%F %T"): V2Ray暂不支持前端UDP转发这个选项，不影响程序继续运行!!!" >> /tmp/v2ray.log
	logger -t "【v2ray】" "V2Ray暂不支持前端UDP转发这个选项，不影响程序继续运行!"
fi
if [ "$dns_mode" == "2" ];then
	/jffs/softcenter/bin/dns2socks 127.0.0.1:23456 $GFWCDN:53 127.0.0.1:7913 >/dev/null 2>&1 &
fi
if [ "$dns_mode" == "0" ];then
	echo "$(date "+%F %T"): V2Ray暂不支持远程解析模式，请选择其它解析模式再试!!!" >> /tmp/v2ray.log
	logger -t "【v2ray】" "暂不支持远程解析模式，请选择其它解析模式再试!"
	exit 1
fi
/usr/sbin/v2ray-rules  $mip  1234 &

/usr/sbin/ssr-state 2>/dev/null &
rm -rf $V2RAY_CONFIG_FILE
exit 0
}

restart() {
  stop
  sleep 2
  menable=`nvram get v2ray_enable`
  kenable=`nvram get k3c_enable`
  if [ "$menable" == "1" ];then
    if [ "$kenable" == "1" ] ;then
      start
    else 
      logger -t "K3C" "K3C扩展设置挂载未开启！"
      echo " $(date "+%F %T"):""K3C扩展设置挂载未开启！" >> /tmp/v2ray.log
    fi
  fi
}

restart

