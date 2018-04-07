#!/bin/sh 
mreact=`nvram get xunlei_react`
mdisk=`nvram get xunlei_disk`
usb_disk="/tmp/mnt/$mdisk"

stop() {
if [ -f "/tmp/k3copt/xunlei/portal" ] ;then
/tmp/k3copt/xunlei/portal -s 
if [ "$mreact" = "1" ] ;then
nvram set xunlei_react=0
rm -rf /tmp/k3copt/xunlei
fi
fi
}

start() {
if [ "$mdisk" != "0" ] ;then
rm -f /tmp/k3copt
ln -s $usb_disk /tmp/k3copt
fi
if [ ! -f "/tmp/k3copt/xunlei/portal" ] ;then
 mkdir -p /tmp/k3copt/xunlei
 if [ ! -d "/tmp/k3copt/xunlei" ] ;then
 echo "迅雷远程启动失败，请确认U盘是否已经插好。"> /tmp/xunlei.log
 exit 0
 fi

  wget --no-check-certificate https://k3c.paldier.tk/tools/xunlei-K3C.zip -O /tmp/xunlei-K3C.zip -t 1 -T 5
  if [ "$?" = "0" ] ;then
  unzip -o /tmp/xunlei-K3C.zip -d /tmp/k3copt/xunlei
  rm -f /tmp/xunlei-K3C.zip
  chmod 755 /tmp/k3copt/xunlei/portal
  else
   echo "迅雷安装包下载失败，请确认网络连接是否正常。"> /tmp/xunlei.log
  fi
fi
cd /tmp/k3copt/xunlei
./portal > /tmp/xunlei.log 
mnum=`cat /tmp/xunlei.log|grep ACTIVE|awk -F':' '{printf $2}' `
if [ ! -z "$mnum" ] ;then
oldnum=`nvram get xunlei_num`
if [ "$oldnum" != "$mnum" ] ;then
nvram set xunlei_num="$mnum"
fi
fi
}

restart() {
  stop
  sleep 1
  menable=`nvram get xunlei_enable`
  if [ "$menable" == "1" ] ;then
  start
  fi
}

restart
