#!/bin/sh 

stop() {
killall -9 kool_mon.sh 2>/dev/null
#清除防火墙规则
iptables -t nat -F kool_chain 2>/dev/null
iptables -t nat -D PREROUTING -p tcp -j kool_chain 2>/dev/null
iptables -t nat -X kool_chain 2>/dev/null
ipset -F adblock 2>/dev/null
ipset -X adblock 2>/dev/null
ipset -F noadblock 2>/dev/null
ipset -X noadblock 2>/dev/null


if [ -f /tmp/koolproxy/koolproxy ] ;then
killall -9 koolproxy
rm -rf /tmp/koolproxy
fi

if [ -f /root/adbyby/adbyby ] ;then
killall -9 adbyby
rm -rf /root/adbyby
fi



icount=`ps -w|grep kool.sh|grep -v grep|wc -l`
if [ $icount -gt 0 ] ;then
mid=`ps -w|grep kool.sh|grep -v grep|awk '{print $1}'`
kill -9 $mid 2>/dev/null 
fi
  echo "广告屏蔽已关闭" > /tmp/adbyby.log
}

start() {
#不重复启动
icount=`ps -w|grep kool.sh|grep -v grep|wc -l`
icount2=`netstat -naut|grep ":8118"|grep "LISTEN"|grep -v grep |wc -l`
if [ $icount != 0 -o $icount2 != 0 ] ;then
stop
sleep 2
fi

/usr/kool/kool.sh &
  echo "广告屏蔽已开启" > /tmp/adbyby.log

}

restart() {
  stop
  sleep 1
  menable=`nvram get koolproxy_enable`
  if [ "$menable" == "1" ] ;then
  start
  fi
}

restart
