#!/bin/sh
check_time=120

while [ "1" = "1" ]  #死循环
do 
  sleep $check_time

#check 
 

   icount=`ps -w|grep frpc|grep -v grep|wc -l`

   if [ $icount = 0 -o $icount -gt 1 ] ;then
   logger -t "frp" "restart frpc!"
   /usr/toolscript/frpc.sh 2>/dev/null
   exit 0
   fi

done

