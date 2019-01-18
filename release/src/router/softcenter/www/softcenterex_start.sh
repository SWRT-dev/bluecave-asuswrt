#! /bin/sh

#我们需要延时等待wan口上线和usb挂载
mdisk=`nvram get k3c_disk`
usbmount=`ls /tmp/mnt/`
while [ "$usbmount" == "" ]
do
	sleep 5s
	usbmount=`ls /tmp/mnt/ |grep $mdisk`
done
sleep 3s
for i in $(find /jffs/softcenter/init.d/ -name 'S*'); do
	case "i" in
		S* | *.sh )
			# Source shell script for speed.
			trap "" INT QUIT TSTP EXIT
            if [ -r "$i" ]; then
            . $i start
            fi
            ;;
        *)
            # No sh extension, so fork subprocess.
            $i start
            ;;
    esac
done
logger -t "软件中心" "外置插件启动完毕！"
