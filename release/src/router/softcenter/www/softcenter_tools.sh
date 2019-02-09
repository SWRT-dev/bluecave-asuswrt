#! /bin/sh

mdisk=`nvram get k3c_disk`
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

	usb_uuid=`dbus get jffs_ext`
	if [ -n "$usb_uuid" ]; then
		mdisk=`blkid |grep "${usb_uuid}" |cut -c6-9`
	else
		mdisk=`nvram get k3c_disk`
	fi
	usb_disk="/tmp/mnt/$mdisk"
	nmount=`ls /tmp/mnt/ |grep $mdisk`
	if [ -n "$nmount" -a "$nmount" != "$mdisk" ]; then
		uuid=`blkid |grep $ndisk |cut -d '"' -f 2`
		dbus set jffs_ext="${uuid}"
	elif [ -n "$nmount" -a -z "$usb_uuid" ]; then
		uuid=`blkid |grep $ndisk |cut -d '"' -f 2`
		dbus set jffs_ext="${uuid}"
	fi

	if [ ! -e "$usb_disk" ]; then
		k3c_warnning="1"
		nvram set k3c_enable="0"
		nvram commit
		echo " $(date "+%F %T"):""没有找到可用的USB磁盘！" >> /tmp/k3c.log
	fi
	if [ "$k3c_warnning" != "1" ];then
		if [ ! -d "/jffs/softcenter" ] ;then
			mkdir -p /jffs/softcenter
			mkdir -p $usb_disk/bin
			mkdir -p $usb_disk/res
			mkdir -p $usb_disk/webs
			mkdir -p $usb_disk/scripts
			mkdir -p /jffs/softcenter/etc
			mkdir -p /jffs/softcenter/init.d
			mkdir -p /jffs/softcenter/configs
			mkdir -p /jffs/softcenter/ss
			mkdir -p /jffs/softcenter/perp
			if [ ! -e "/jffs/softcenter/init.d/S10softcenter.sh" ]; then
				ln -s $usb_disk/bin /jffs/softcenter/bin
				ln -s $usb_disk/res /jffs/softcenter/res
				ln -s $usb_disk/webs /jffs/softcenter/webs
				ln -s $usb_disk/scripts /jffs/softcenter/scripts
				wget --no-check-certificate --timeout=10 --tries=3 -qO /tmp/softcenter.tar.gz http://sc.paldier.com/softcenter/softcenter.tar.gz
				if [ "$?" == "0" ]; then
					tar -zxvf /tmp/softcenter.tar.gz -C /tmp
					cp -rf /tmp/softcenter/scripts/* /jffs/softcenter/scripts/
					cp -rf /tmp/softcenter/res/* /jffs/softcenter/res/
					cp -rf /tmp/softcenter/webs/* /jffs/softcenter/webs/
					cp -rf /tmp/softcenter/bin/* /jffs/softcenter/bin/
					cp -rf /tmp/softcenter/perp/* /jffs/softcenter/perp/
					chmod 755 /jffs/softcenter/scripts/*.sh
					chmod 755 /jffs/softcenter/bin/*
					chmod 755 /jffs/softcenter/perp/*
					ln -sf /jffs/softcenter/scripts/ks_app_install.sh /jffs/softcenter/init.d/S10softcenter.sh
					cp -rf /jffs/softcenter/scripts/ks_app_install.sh /jffs/softcenter/scripts/ks_app_remove.sh
					echo " $(date "+%F %T"):""安装完成，重启后生效！" >> /tmp/k3c.log
				else
					echo " $(date "+%F %T"):""下载失败！" >> /tmp/k3c.log
				fi
			fi
		fi
		ln -s $usb_disk/bin /jffs/softcenter/bin
		ln -s $usb_disk/res /jffs/softcenter/res
		ln -s $usb_disk/webs /jffs/softcenter/webs
		ln -s $usb_disk/scripts /jffs/softcenter/scripts
		echo " $(date "+%F %T"):""挂载完成！" >> /tmp/k3c.log
	fi
}

stop(){
	#rm -rf /jffs/softcenter
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
