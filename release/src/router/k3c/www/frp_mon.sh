#!/bin/sh
check_time=120

while [ "1" = "1" ]  #死循环
do 
  sleep $check_time

#check 
 

   icount=`ps -w|grep frpc|grep -v grep|wc -l`

   if [ $icount = 0 -o $icount -gt 1 ] ;then
   echo "$(date "+%F %T"):""守护发现异常重启进程!" >> /tmp/frpc.log
   /usr/sbin/k3c_frpc.sh 2>/dev/null
   exit 0
   fi

done

