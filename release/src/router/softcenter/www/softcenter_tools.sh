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
	usb_disk="/tmp/mnt/$mdisk"

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
			mkdir -p /jffs/softcenter/lib
			mkdir -p /jffs/softcenter/perp
			ln -s $usb_disk/bin /jffs/softcenter/bin
			ln -s $usb_disk/res /jffs/softcenter/res
			ln -s $usb_disk/webs /jffs/softcenter/webs
			ln -s $usb_disk/scripts /jffs/softcenter/scripts
			cp -rf /rom/etc/softcenter/scripts/* /jffs/softcenter/scripts/
			cp -rf /rom/etc/softcenter/res/* /jffs/softcenter/res/
			cp -rf /rom/etc/softcenter/webs/* /jffs/softcenter/webs/
			cp -rf /rom/etc/softcenter/bin/* /jffs/softcenter/bin/
			cp -rf /rom/etc/softcenter/perp /jffs/softcenter/
			chmod 755 /jffs/softcenter/scripts/*.sh
			chmod 755 /jffs/softcenter/bin/*
			chmod 755 /jffs/softcenter/perp/*
			chmod 755 /jffs/softcenter/perp/.boot/*
			chmod 755 /jffs/softcenter/perp/.control/*
			chmod 755 /jffs/softcenter/init.d/*
			cp -rf /jffs/softcenter/scripts/ks_app_install.sh /jffs/softcenter/scripts/ks_app_remove.sh
			echo 1.2.0 > /jffs/softcenter/.soft_ver
			dbus set softcenter_version=`cat /jffs/softcenter/.soft_ver`
			echo " $(date "+%F %T"):""安装完成，重启后生效！" >> /tmp/k3c.log
		fi
		echo " $(date "+%F %T"):""挂载完成！" >> /tmp/k3c.log
		if [ ! -f "/jffs/scripts/wan-start" ];then
			cat > /jffs/scripts/wan-start <<-EOF
			#!/bin/sh
			/jffs/softcenter/bin/softcenter-wan.sh start
			EOF
			chmod +x /jffs/scripts/wan-start
		else
			STARTCOMAND1=`cat /jffs/scripts/wan-start | grep -c "/jffs/softcenter/bin/softcenter-wan.sh start"`
			[ "$STARTCOMAND1" -gt "1" ] && sed -i '/softcenter-wan.sh/d' /jffs/scripts/wan-start && sed -i '1a /jffs/softcenter/bin/softcenter-wan.sh start' /jffs/scripts/wan-start
			[ "$STARTCOMAND1" == "0" ] && sed -i '1a /jffs/softcenter/bin/softcenter-wan.sh start' /jffs/scripts/wan-start
		fi
		
		if [ ! -f "/jffs/scripts/nat-start" ];then
			cat > /jffs/scripts/nat-start <<-EOF
			#!/bin/sh
			/jffs/softcenter/bin/softcenter-net.sh start_nat
			EOF
			chmod +x /jffs/scripts/nat-start
		else
			STARTCOMAND2=`cat /jffs/scripts/nat-start | grep -c "/jffs/softcenter/bin/softcenter-net.sh start"`
			[ "$STARTCOMAND2" -gt "1" ] && sed -i '/softcenter-net.sh/d' /jffs/scripts/nat-start && sed -i '1a /jffs/softcenter/bin/softcenter-net.sh start' /jffs/scripts/nat-start
			[ "$STARTCOMAND2" == "0" ] && sed -i '1a /jffs/softcenter/bin/softcenter-net.sh start' /jffs/scripts/nat-start
		fi
		
		if [ ! -f "/jffs/scripts/post-mount" ];then
			cat > /jffs/scripts/post-mount <<-EOF
			#!/bin/sh
			/jffs/softcenter/bin/softcenter-mount.sh start
			EOF
			chmod +x /jffs/scripts/post-mount
		else
			STARTCOMAND2=`cat /jffs/scripts/post-mount | grep "/jffs/softcenter/bin/softcenter-mount.sh start"`
			[ "$STARTCOMAND2" -gt "1" ] && sed -i '/softcenter-mount.sh/d' /jffs/scripts/post-mount && sed -i '1a /jffs/softcenter/bin/softcenter-mount.sh start' /jffs/scripts/post-mount
			[ "$STARTCOMAND2" == "0" ] && sed -i '1a /jffs/softcenter/bin/softcenter-mount.sh start' /jffs/scripts/post-mount
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
