#! /bin/sh

mdisk=`nvram get swap_disk`
usb_disk="/tmp/mnt/$mdisk"
ext_type=`/bin/mount | grep -E "$usb_disk" | sed -n 1p | cut -d" " -f5`
swap_size=`nvram get swap_size`
usbmount=`ls /tmp/mnt/`
start(){
	
	# 1	没有找到可用的USB磁盘
	# 2	USB磁盘格式不符合要求
	# 3	成功检测到ext？格式磁盘,可以创建swap
	# 4	swap分区已经加载
#开机启动太早会卡？
if [ "$usbmount" == "" ];then
    echo " $(date "+%F %T"):""系统正在启动，等待USB设备挂载中！" >> /tmp/swap.log
    
fi
while [ "$usbmount" == "" ]
do
	sleep 5s
	usbmount=`ls /tmp/mnt/ |grep $mdisk`
done
	swapon=`free | grep Swap | awk '{print $2}'`
	
if [ "$swapon" == "0" ];then
	if [ -z "$usb_disk" ];then
		swap_warnning="1"
		nvram set swap_enable="0"
		echo " $(date "+%F %T"):""没有找到可用的USB磁盘！" >> /tmp/swap.log
	else
		if [ -f "$usb_disk"/swapfile ];then
			swapon "$usb_disk"/swapfile
			swap_warnning="4"
			echo " $(date "+%F %T"):""已经挂载虚拟内存！" >> /tmp/swap.log
		else
			if [ "$ext_type" == "ext2" ] || [ "$ext_type" == "ext3" ] || [ "$ext_type" == "ext4" ] || [ "$ext_type" == "tfat" ] || [ "$ext_type" == "tntfs" ];then
				swap_warnning="3"
				echo " $(date "+%F %T"):""成功检测到磁盘,可以创建swap！" >> /tmp/swap.log
			else
				swap_warnning="2"
				nvram set swap_enable="0"
				echo " $(date "+%F %T"):""USB磁盘格式不符合要求！磁盘格式:""$ext_type" >> /tmp/swap.log
			fi
		fi
	fi
else
	swap_warnning="4"
	echo " $(date "+%F %T"):""已经挂载虚拟内存！" >> /tmp/swap.log
fi

if [ "$swap_warnning" == "3" ];then
		[ "$swap_size" == "1" ] && size=256144
		[ "$swap_size" == "2" ] && size=524288
		[ "$swap_size" == "3" ] && size=1048576
		if [ ! -f  $usb_disk/swap ];then
			dd if=/dev/zero of=$usb_disk/swapfile bs=1024 count="$size"
			/sbin/mkswap $usb_disk/swapfile
			chmod 0600 $usb_disk/swapfile
			swapon $usb_disk/swapfile
		fi
	fi
}


stop(){
	swapoff $usb_disk/swapfile
	rm -rf $usb_disk/swapfile
}

restart() {
senable=`nvram get swap_enable`
sleep 1
if [ "$senable" == "1" ]
then
  echo " $(date "+%F %T"):""虚拟内存启动！" >> /tmp/swap.log
  start
else
  stop
  echo " $(date "+%F %T"):""虚拟内存已关闭！" >> /tmp/swap.log
fi
}

restart
