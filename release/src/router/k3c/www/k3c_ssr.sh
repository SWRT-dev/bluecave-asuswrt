#!/bin/sh 

stop() {

#清除防火墙规则
killall -q -9 ssr_mon.sh

/usr/sbin/ssr-rules clean all

killall -q -9 ssr-redir
killall -q -9 ss-redir
 
rm -f /tmp/etc/dnsmasq.user/gfw_list.conf 2>/dev/null 
rm -f /tmp/etc/dnsmasq.user/gfw_addr.conf 2>/dev/null
rm -f /tmp/etc/dnsmasq.user/gfw_user.conf 2>/dev/null
service restart_dns 2>/dev/null

}

start() {
#不重复启动
icount=`ps -w|grep ssr-retcp|grep -v grep|wc -l`

if [ $icount != 0 ] ;then
stop
sleep 2
fi

sindex=`nvram get ssr_index 2>/dev/null`

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
		    "fast_open": false
		}
EOF

if [ "$udp_enable" == "1" ] ;then
/usr/sbin/ssr-redir  -u -c /tmp/shadowsocksr.json -f /tmp/ssr-retcp.pid
else
/usr/sbin/ssr-redir  -c /tmp/shadowsocksr.json -f /tmp/ssr-retcp.pid
fi

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
		    "fast_open": false
		}
EOF

if [ "$udp_enable" == "1" ] ;then
/usr/sbin/ss-redir  -u -c /tmp/shadowsocks.json -f /tmp/ssr-retcp.pid
else
/usr/sbin/ss-redir  -c /tmp/shadowsocks.json -f /tmp/ssr-retcp.pid
fi

fi
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
