#!/bin/sh 

stop() {
killall -9 shellinaboxd
iptables -D INPUT -p tcp --dport 4200 -j ACCEPT
 echo "webshell已关闭。">> /tmp/webshell.log
}

start() {
#不重复启动
icount=`ps -w|grep shellinaboxd|grep -v grep|wc -l`
if [ $icount != 0  ] ;then
stop
sleep 2
fi
/usr/sbin/webshell/shellinaboxd --css=/usr/sbin/webshell/white-on-black.css --service=/:LOGIN -b --disable-ssl
iptables -I INPUT -p tcp --dport 4200 -j ACCEPT
#这里有个bug，系统启动时wan还没有上线，所以wan的端口是没有打开的
 echo "webshell已启动。">> /tmp/webshell.log
}

restart() {
  stop
  sleep 1
  menable=`nvram get webshell_enable`
  if [ "$menable" == "1" ] ;then
  start
  fi
}

restart
