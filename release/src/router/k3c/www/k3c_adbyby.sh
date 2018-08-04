#!/bin/sh 

mdisk=`nvram get k3c_disk`
usb_disk="/tmp/mnt/$mdisk"
usbmount=`ls /tmp/mnt/`

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

killall -9 koolproxy 2>/dev/null
killall -9 adbyby 2>/dev/null
rm -rf /tmp/etc/dnsmasq.user/adblock.conf
icount=`ps -w|grep kool.sh|grep -v grep|wc -l`
if [ $icount -gt 0 ] ;then
mid=`ps -w|grep kool.sh|grep -v grep|awk '{print $1}'`
kill -9 $mid 2>/dev/null 
fi
/sbin/restart_dns 2>/dev/null
  echo " $(date "+%F %T"):""广告屏蔽已关闭" >> /tmp/adbyby.log
}

start() {
#不重复启动
icount=`ps -w|grep kool.sh|grep -v grep|wc -l`
icount2=`netstat -naut|grep ":8118"|grep "LISTEN"|grep -v grep |wc -l`
if [ $icount != 0 -o $icount2 != 0 ] ;then
stop
sleep 2
fi
if [ "$usbmount" == "" ];then
    echo " $(date "+%F %T"):""系统正在启动，等待USB设备挂载中！" >> /tmp/k3c.log
fi
while [ "$usbmount" == "" ]
do
	sleep 5s
done
/usr/kool/kool.sh &
  echo " $(date "+%F %T"):""广告屏蔽已开启" >> /tmp/adbyby.log

}

restart() {
  stop
  sleep 1
  menable=`nvram get koolproxy_enable`
  kenable=`nvram get k3c_enable`
  if [ "$menable" == "1" ];then
    if [ "$kenable" == "1" ] ;then
      start
    else 
      logger -t "K3C" "K3C扩展设置挂载未开启！"
      echo " $(date "+%F %T"):""K3C扩展设置挂载未开启！" >> /tmp/adbyby.log
    fi
  fi
}

restart
