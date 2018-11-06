#! /bin/sh

mdisk=`nvram get k3c_disk`
usb_disk="/tmp/mnt/$mdisk"
usbmount=`ls /tmp/mnt/`
start(){

#开机启动太早会卡？
if [ "$usbmount" == "" ];then
    echo " $(date "+%F %T"):""系统正在启动，等待USB设备挂载中！" >> /tmp/k3c.log
fi
while [ "$usbmount" == "" ]
do
	sleep 5s
	usbmount=`ls /tmp/mnt/ |grep $mdisk`
done
	if [ ! -e "$usb_disk" ];then
		k3c_warnning="1"
		nvram set k3c_enable="0"
		echo " $(date "+%F %T"):""没有找到可用的USB磁盘！" >> /tmp/k3c.log
	fi
	if [ "$k3c_warnning" != "1" ];then
		if [ ! -e "/jffs/softcenter" ] ;then
			ln -s $usb_disk /jffs/softcenter
			if [ ! -e "/jffs/softcenter/webs" ] ;then
				mkdir -p /jffs/softcenter/bin
				mkdir -p /jffs/softcenter/res
				mkdir -p /jffs/softcenter/webs
				mkdir -p /jffs/softcenter/scripts
				mkdir -p /jffs/softcenter/etc
				mkdir -p /jffs/softcenter/etc/init.d
				if [ ! -e "/jffs/softcenter/webs/softcenter.xml" ] ;then
				cp -rf /usr/sbin/softcenter.xml /jffs/softcenter/webs
				fi
			fi
		fi
	fi
}

stop(){
	rm -rf /jffs/softcenter
}

restart() {
senable=`nvram get k3c_enable`
sleep 1
if [ "$senable" == "1" ];then
  echo " $(date "+%F %T"):""K3C扩展设置挂载启动！" >> /tmp/k3c.log
  start
else
  stop
  echo " $(date "+%F %T"):""K3C扩展设置挂载关闭！" >> /tmp/k3c.log
fi
}

restart

