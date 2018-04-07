#!/bin/sh 

stop() {
killall -9 adb_mon.sh 2>/dev/null
#清除防火墙规则
iptables -t nat -F adbyby_chain 2>/dev/null
iptables -t nat -D PREROUTING -p tcp -j adbyby_chain 2>/dev/null
iptables -t nat -X adbyby_chain 2>/dev/null
#ipset -F adblock 2>/dev/null
#ipset -X adblock 2>/dev/null
#ipset -F noadblock 2>/dev/null
#ipset -X noadblock 2>/dev/null


if [ -f /tmp/koolproxy/koolproxy ] ;then
killall -9 koolproxy
rm -rf /tmp/koolproxy
fi

if [ -f /root/adbyby/adbyby ] ;then
killall -9 adbyby
rm -rf /root/adbyby
fi



icount=`ps -w|grep adbyby.sh|grep -v grep|wc -l`
if [ $icount -gt 0 ] ;then
mid=`ps -w|grep adbyby.sh|grep -v grep|awk '{print $1}'`
kill -9 $mid 2>/dev/null 
fi

}

start() {
#不重复启动
icount=`ps -w|grep adbyby.sh|grep -v grep|wc -l`
icount2=`netstat -naut|grep ":8118"|grep "LISTEN"|grep -v grep |wc -l`
if [ $icount != 0 -o $icount2 != 0 ] ;then
stop
sleep 2
fi

/usr/adb/adbyby.sh &



}

restart() {
  stop
  sleep 1
  menable=`nvram get adbyby_enable`
  if [ "$menable" == "1" ] ;then
  start
  fi
}

restart
