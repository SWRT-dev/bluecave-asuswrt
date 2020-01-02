#!/bin/sh


SPACE_AVAL=$(df|grep jffs | awk '{print $2}')

if [ $SPACE_AVAL -gt 51200 -a "$(nvram get sc_mount)" == 0 ];then
if [ ! -d /jffs/softcenter ]; then
	mkdir -p /jffs/softcenter
	cp -rf /rom/etc/softcenter/* /jffs/softcenter/
fi
mkdir -p /jffs/softcenter/init.d
mkdir -p /jffs/softcenter/bin
mkdir -p /jffs/softcenter/etc
mkdir -p /jffs/softcenter/scripts
mkdir -p /jffs/softcenter/webs
mkdir -p /jffs/softcenter/res
mkdir -p /jffs/softcenter/ss
mkdir -p /jffs/softcenter/lib
mkdir -p /jffs/configs/dnsmasq.d
mkdir -p /jffs/softcenter/configs
else
if [ "$(nvram get sc_mount)" == 1 ];then
	mdisk=`nvram get sc_disk`
	usb_disk="/tmp/mnt/$mdisk"
	if [ "$(nvram get productid)" == "BLUECAVE" ];then
		[ -n "$(mount |grep sda1 |grep tfat)" ] && logger "Unsupport TFAT!" && exit 1
	fi
	if [ ! -e "$usb_disk" ]; then
		nvram set sc_mount="0"
		nvram commit
		logger "没有找到可用的USB磁盘！" 
		exit 1
	else
		if [ ! -f "/jffs/softcenter/webs/Main_Soft_center.asp" ] ;then
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
			ln -sf $usb_disk/bin /jffs/softcenter/bin
			ln -sf $usb_disk/res /jffs/softcenter/res
			ln -sf $usb_disk/webs /jffs/softcenter/webs
			ln -sf $usb_disk/scripts /jffs/softcenter/scripts
		fi
	fi
else
	logger "当前jffs分区剩余空间不足！"
	logger "退出安装！"
	exit 1
fi
fi
cp -rf /rom/etc/softcenter/scripts/* /jffs/softcenter/scripts/
cp -rf /rom/etc/softcenter/res/* /jffs/softcenter/res/
cp -rf /rom/etc/softcenter/webs/* /jffs/softcenter/webs/
cp -rf /rom/etc/softcenter/bin/* /jffs/softcenter/bin/
cp -rf /rom/etc/softcenter/perp /jffs/softcenter/
if [ "`nvram get model`" == "GT-AC5300" ] || [ "`nvram get model`" == "GT-AC2900" ];then
rm -rf /jffs/softcenter/ROG
fi
ln -sf /jffs/softcenter/bin/base64_encode /jffs/softcenter/bin/base64_decode
ln -sf /jffs/softcenter/scripts/ks_app_install.sh /jffs/softcenter/scripts/ks_app_remove.sh
chmod 755 /jffs/softcenter/scripts/*.sh
chmod 755 /jffs/softcenter/configs/*.sh
chmod 755 /jffs/softcenter/bin/*
chmod 755 /jffs/softcenter/init.d/*
chmod 755 /jffs/softcenter/perp/*
chmod 755 /jffs/softcenter/perp/.boot/*
chmod 755 /jffs/softcenter/perp/.control/*
echo 1.2.0 > /jffs/softcenter/.soft_ver
dbus set softcenter_firmware_version=`nvram get extendno|cut -d "_" -f2|cut -d "-" -f1|cut -c2-6`
ARCH=`uname -m`
KVER=`uname -r`
if [ "$ARCH" == "armv7l" ]; then
	if [ "$KVER" == "4.1.52" -o "$KVER" == "4.1.49" ];then
		dbus set softcenter_arch="armng"
	elif [ "$KVER" == "3.14.77" ];then
		dbus set softcenter_arch="armqca"
	else
		dbus set softcenter_arch="$ARCH"
	fi
else
	dbus set softcenter_arch="$ARCH"
fi

dbus set softcenter_api=`cat /jffs/softcenter/.soft_ver`
if [ "`nvram get model`" == "GT-AC5300" ] || [ "`nvram get model`" == "GT-AC2900" ];then
	cp -rf /rom/etc/softcenter/ROG/webs/* /jffs/softcenter/webs/
	cp -rf /rom/etc/softcenter/ROG/res/* /jffs/softcenter/res/
fi
nvram set sc_installed=1
# creat wan-start file
mkdir -p /jffs/scripts

# creat profile file
if [ ! -f /jffs/configs/profile.add ]; then
cat > /jffs/configs/profile.add <<EOF
alias ls='ls -Fp --color=auto'
alias ll='ls -lFp --color=auto'
alias l='ls -AlFp --color=auto'
source /jffs/softcenter/scripts/base.sh
export PERP_BASE=/softcenter/perp

EOF
fi


