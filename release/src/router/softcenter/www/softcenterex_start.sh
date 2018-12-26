#! /bin/sh

#我们需要延时等待wan口上线和usb挂载
usbmount=`ls /tmp/mnt/`
while [ "$usbmount" == "" ]
do
	sleep 5s
	usbmount=`ls /tmp/mnt/`
done
for _file in /jffs/softcenter/etc/init.d/*; do
	./$_file
done
logger -t "软件中心" "外置插件启动完毕！"

