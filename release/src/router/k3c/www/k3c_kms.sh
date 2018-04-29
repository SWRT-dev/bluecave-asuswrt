#!/bin/sh 

stop() {
killall -9 vlmcsd

}

start() {
#不重复启动
icount=`ps -w|grep vlmcsd|grep -v grep|wc -l`

if [ $icount = 0  ] ;then

/usr/sbin/vlmcsd -i /usr/kms/vlmcsd.ini -p /tmp/vlmcsd.pid -l /tmp/kms.log

mauto=`nvram get kms_auto`
if [ "$mauto" = "1"  ] ;then
cp -f /usr/kms/kms_dns.conf /tmp/etc/dnsmasq.user/kms_dns.conf
else
rm -f /tmp/etc/dnsmasq.user/kms_dns.conf
fi
/sbin/restart_dnsmasq 2>/dev/null

fi

}

restart() {
  stop
  sleep 1
  menable=`nvram get kms_enable`
  if [ "$menable" == "1" ] ;then
  start
  fi
}

restart

