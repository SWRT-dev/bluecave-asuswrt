#! /bin/sh

#我们需要延时等待wan口上线和usb挂载
kenable=`nvram get k3c_enable`
mdisk=`nvram get k3c_disk`
usbmount=`ls /tmp/mnt/`
[ "$kenable" == "0" ] && exit 1

ACTION=$1

if [ $# -lt 1 ]; then
    printf "Usage: $0 {start|stop|restart|reconfigure|check|kill}\n" >&2
    exit 1
fi

while [ -z "$usbmount" ]
do
	sleep 5s
	usbmount=`ls /tmp/mnt/ |grep $mdisk`
done

[ $ACTION = stop -o $ACTION = restart -o $ACTION = kill ] && ORDER="-r"

for i in $(find /jffs/softcenter/init.d/ -name 'S*' | sort $ORDER ) ;
do
    case "$i" in
        S* | *.sh )
            # Source shell script for speed.
            trap "" INT QUIT TSTP EXIT
            #set $1
            #echo "trying $i" >> /tmp/rc.log
            if [ -r "$i" ]; then
            . $i $ACTION
            fi
            ;;
        *)
            # No sh extension, so fork subprocess.
            $i $ACTION
            ;;
    esac
done

