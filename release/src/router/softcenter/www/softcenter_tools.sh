#! /bin/sh

usb_uuid=`dbus get jffs_ext`
if [ -n "$usb_uuid" ]; then
mdisk=`blkid |grep "${usb_uuid}" |cut -c6-9`
else
mdisk=`nvram get k3c_disk`
fi
usb_disk="/tmp/mnt/$mdisk"
usbmount=`ls /tmp/mnt/`

check(){
ndisk=`nvram get k3c_disk`
nmount=`ls /tmp/mnt/ |grep $mdisk`
if [ -n "$nmount" -a "$nmount" != "$mdisk" ]; then
	uuid=`blkid |grep $ndisk |cut -d '"' -f 2`
	dbus set jffs_ext="${uuid}"
else if [ -n "$nmount" -a -z "$usb_uuid" ]; then
	uuid=`blkid |grep $ndisk |cut -d '"' -f 2`
	dbus set jffs_ext="${uuid}"
fi
}

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
	check
	if [ ! -e "$usb_disk" ]; then
		k3c_warnning="1"
		nvram set k3c_enable="0"
		nvram commit
		echo " $(date "+%F %T"):""没有找到可用的USB磁盘！" >> /tmp/k3c.log
	fi
	if [ "$k3c_warnning" != "1" ];then
		if [ ! -d "/jffs/softcenter" -a ! -h "/jffs/softcenter" ] ;then

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
		else
			rm -rf /jffs/softcenter
			ln -s $usb_disk /jffs/softcenter
		fi
	fi
}

stop(){
	rm -rf /jffs/softcenter
	dbus remove jffs_ext
}

restart() {
senable=`nvram get k3c_enable`
sleep 1
if [ "$senable" == "1" ];then
  echo " $(date "+%F %T"):""jffs扩展挂载启动！" >> /tmp/k3c.log
  start
else
  stop
  echo " $(date "+%F %T"):""jffs扩展挂载关闭！" >> /tmp/k3c.log
fi
}

restart
