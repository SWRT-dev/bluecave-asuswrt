#!/bin/sh 

stop() {

#清除防火墙规则
killall -q -9 ssr_mon.sh

/usr/sbin/ssr-rules clean all

killall -q -9 ssr-redir
killall -q -9 ss-redir
kill $(cat /var/run/pdnsd.pid) >/dev/null 2>&1 || killall -9 pdnsd >/dev/null 2>&1

[ -f /var/run/ssr-retcp.pid ] && kill -9 `cat /var/run/ssr-retcp.pid` 2>/dev/null && rm -f /var/run/ssr-retcp.pid
rm -f /tmp/etc/dnsmasq.user/gfw_list.conf 2>/dev/null 
rm -f /tmp/etc/dnsmasq.user/gfw_addr.conf 2>/dev/null
rm -f /tmp/etc/dnsmasq.user/gfw_user.conf 2>/dev/null

service restart_dnsmasq 2>/dev/null
ienable=`nvram get ipv6_service`
if [ "$ienable" != "disabled" ] ;then
service stop_ipv6_tunnel
sleep 2
service start_ipv6_tunnel
fi
}


start() {
#不重复启动
icount=`ps -w|grep ssr-retcp|grep -v grep|wc -l`

if [ $icount != 0 ] ;then
stop
sleep 2
fi

sindex=`nvram get ssr_index`

mcmd="echo \"`nvram get ssr_server_ip`\"|awk -F '>' '{printf \$$sindex}'"
mip=`echo $mcmd |sh`

mcmd="echo \"`nvram get ssr_server_port`\"|awk -F '>' '{printf \$$sindex}'"
mport=`echo $mcmd |sh`

mcmd="echo \"`nvram get ssr_server_timeout`\"|awk -F '>' '{printf \$$sindex}'"
mtimeout=`echo $mcmd |sh`

mcmd="echo \"`nvram get ssr_server_passwd`\"|awk -F '>' '{printf \$$sindex}'"
mpasswd=`echo $mcmd |sh`

mcmd="echo \"`nvram get ssr_server_encrypt`\"|awk -F '>' '{printf \$$sindex}'"
mencrypt=`echo $mcmd |sh`

mcmd="echo \"`nvram get ssr_server_protocol`\"|awk -F '>' '{printf \$$sindex}'"
mprotocol=`echo $mcmd |sh`

mcmd="echo \"`nvram get ssr_server_obfs`\"|awk -F '>' '{printf \$$sindex}'"
mobfs=`echo $mcmd |sh`

mcmd="echo \"`nvram get ssr_server_obfspara`\"|awk -F '>' '{printf \$$sindex}'"
mobfspara=`echo $mcmd |sh`

mcmd="echo \"`nvram get ssr_server_protocolpara`\"|awk -F '>' '{printf \$$sindex}'"
mpropara=`echo $mcmd |sh`

mcmd="echo \"`nvram get ssr_server_type`\"|awk -F '>' '{printf \$$sindex}'"
mtype=`echo $mcmd |sh`

mcmd="echo \"`nvram get ssr_server_ssencrypt`\"|awk -F '>' '{printf \$$sindex}'"
mssencrypt=`echo $mcmd |sh`

udp_enable=`nvram get ssr_udp_enable`

	serverip=`echo $mip|grep -E "([0-9]{1,3}[\.]){3}[0-9]{1,3}|:"`
	if [ -z "$serverip" ];then
			server_ip=`nslookup "$serverip" 8.8.8.8 | sed '1,4d' | awk '{print $3}' | grep -v :|awk 'NR==1{print}'`
			if [ "$?" == "0" ]; then
				logger -t "SSR" "SSR服务器的ip地址解析成功!"
			else
				logger -t "SSR" "SSR服务器域名解析失败！"
				server_ip=`nslookup "$serverip" 114.114.114.114 | sed '1,4d' | awk '{print $3}' | grep -v :|awk 'NR==1{print}'`
				if [ "$?" == "0" ]; then
					logger -t "SSR" "SSR服务器的ip地址解析成功!"
				else
					logger -t "SSR" "SSR服务器域名解析失败！"
				fi
			fi

		if [ ! -z "$server_ip" ];then
			mip="$server_ip"
		fi
	fi

if [ "$mtype" == "SSR" ] ;then
#产生配置文件
		cat <<-EOF >/tmp/shadowsocksr.json
		{
		    
		    "server": "$mip",
		    "server_port": $mport,
		    "local_address": "0.0.0.0",
		    "local_port": 1234,
		    "password": "$mpasswd",
		    "timeout": $mtimeout,
		    "method": "$mencrypt",
		    "protocol": "$mprotocol",
		    "protocol_param": "$mpropara",
		    "obfs": "$mobfs",
		    "obfs_param": "$mobfspara",
		    "reuse_port": true,
		    "fast_open": false
		}
EOF

if [ "$udp_enable" == "1" ] ;then
	/usr/sbin/ssr-redir  -u -c /tmp/shadowsocksr.json -f /var/run/ssr-reudp.pid >/dev/null 2>&1
fi
	for i in 1 2 3
	do 
		/usr/sbin/ssr-redir  -c /tmp/shadowsocksr.json -f /var/run/ssr-retcp_$i.pid >/dev/null 2>&1
	done
else
		cat <<-EOF >/tmp/shadowsocks.json
		{
		    
		    "server": "$mip",
		    "server_port": $mport,
		    "local_address": "0.0.0.0",
		    "local_port": 1234,
		    "password": "$mpasswd",
		    "timeout": $mtimeout,
		    "method": "$mssencrypt",
		    "reuse_port": true,
		    "fast_open": false
		}
EOF

if [ "$udp_enable" == "1" ] ;then
		/usr/sbin/ss-redir  -u -c /tmp/shadowsocks.json -f /var/run/ssr-reudp.pid >/dev/null 2>&1
fi
	for i in 1 2 3
	do 
		/usr/sbin/ss-redir  -c /tmp/shadowsocks.json -f /var/run/ssr-retcp_$i.pid >/dev/null 2>&1
	done
fi

logger -t "SSR" "Starting!"

/usr/sbin/ssr-rules  $mip  1234 &
/usr/sbin/ssr-state 2>/dev/null || exit 0
}

restart() {
  stop
  sleep 2
  menable=`nvram get ssr_enable`
  if [ "$menable" == "1" ] ;then
  start
  fi
}

restart

