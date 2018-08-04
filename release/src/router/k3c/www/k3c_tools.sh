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
done
	if [ ! -e "$usb_disk" ];then
		k3c_warnning="1"
		nvram set k3c_enable="0"
		echo " $(date "+%F %T"):""没有找到可用的USB磁盘！" >> /tmp/k3c.log
	fi

	if [ "$k3c_warnning" != "1" ];then
		if [ ! -e "/jffs/k3cex" ] ;then
			ln -s $usb_disk /jffs/k3cex
			if [ ! -e "/jffs/k3cex/www" ] ;then
				mkdir -p /jffs/k3cex/www
				mkdir -p /jffs/k3cex/www/img
				mkdir -p /jffs/k3cex/www/web
				mkdir -p /jffs/k3cex/www/script
				touch /jffs/k3cex/www/script/k3cex-init.sh
				echo "#! /bin/sh" > /jffs/k3cex/www/script/k3cex-init.sh
				cp /www/Softcenter.asp /jffs/k3cex/www/web/Softcenter.asp
				mount --bind /jffs/k3cex/www/web/Softcenter.asp /www/Softcenter.asp
			fi
		fi
	fi
}

stop(){
  rm -rf /jffs/k3cex
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
