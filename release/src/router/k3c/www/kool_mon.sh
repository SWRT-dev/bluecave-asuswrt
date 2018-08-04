#!/bin/sh
check_time=120

while [ "1" = "1" ]  
do 
  sleep $check_time

#check iptables

   icount=`netstat -naut|grep ":8118"|grep "LISTEN" |grep -v grep |wc -l`

   icount2=`iptables -t nat -S|grep kool|wc -l`
   adblock_mode=`nvram get adblock_mode 2>/dev/null`
   if [ $icount = 0 -o $icount2 -lt 5 ] ;then
    if [ "$adblock_mode" == "1" ] ;then
        logger -t "koolproxy" "守护发现异常重启进程!"
        echo "【koolproxy】守护发现异常重启进程!" >> /tmp/adbyby.log
    else
        logger -t "adbyby" "守护发现异常重启进程!"
        echo "【adbyby】守护发现异常重启进程!" >> /tmp/adbyby.log
    fi
   /usr/sbin/k3c_adbyby.sh 
   exit 0
   fi

done

