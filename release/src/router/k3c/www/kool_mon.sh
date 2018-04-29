#!/bin/sh
check_time=120

while [ "1" = "1" ]  
do 
  sleep $check_time

#check iptables

   icount=`netstat -naut|grep ":8118"|grep "LISTEN" |grep -v grep |wc -l`

   icount2=`iptables -t nat -S|grep kool|wc -l`
   if [ $icount = 0 -o $icount2 -lt 5 ] ;then
   logger -t "koolproxy" "iptables error,restart koolproxy!"
   echo "iptables error,restart koolproxy!" > /tmp/adbyby.log
   /usr/sbin/k3c_adbyby.sh 
   exit 0
   fi



done

