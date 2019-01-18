#!/bin/sh
check_time=120
dns_mode=`nvram get ssr_dnsmode`
while [ "1" = "1" ]  
do 
  sleep $check_time

#check iptables

   icount=`ps -w|grep ssr-retcp |grep -v grep |wc -l`

   icount2=`iptables -t nat -S|grep SHADOW|wc -l`
   if [ $icount = 0 -o $icount2 -lt 5 ] ;then
   logger -t "SSR" "iptables error,restart ssr!"
   /usr/sbin/k3c_ssr.sh 
   exit 0
   fi

#check pdnsd
if [ "$dns_mode" = "1" ] ;then
   icount=`ps -w|grep pdnsd |grep -v grep |wc -l`
   if [ $icount = 0 ] ;then
   logger -t "SSR" "pdnsd error,restart ssr!"
   /usr/sbin/k3c_ssr.sh 
   exit 0
   fi
fi

done

