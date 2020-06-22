#!/bin/sh


SPACE_AVAL=$(df|grep jffs | awk '{print $2}')
MODEL=`nvram get productid`
if [ "$MODEL" == "GT-AC5300" ] || [ "$MODEL" == "GT-AX11000" ] || [ "$MODEL" == "GT-AC2900" ] || [ "$(nvram get merlinr_rog)" == "1" ];then
	ROG=1
elif [ "$MODEL" == "TUF-AX3000" ] || [ "$(nvram get merlinr_tuf)" == "1" ] ;then
	TUF=1
fi

if [ $SPACE_AVAL -gt 51200 -a "$(nvram get sc_mount)" == 0 ];then
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
	[ -n "$(mount |grep $usb_disk |grep tfat)" ] && logger "Unsupport TFAT!" && exit 1
	if [ ! -e "$usb_disk" ]; then
		nvram set sc_mount="0"
		nvram commit
		logger "USB flash drive not detected!/没有找到可用的USB磁盘!" 
		exit 1
	else
		if [ ! -f "/jffs/softcenter/webs/Main_Soft_center.asp" ] ;then
			mkdir -p /jffs/softcenter
			mkdir -p $usb_disk/bin
			mkdir -p $usb_disk/res
			mkdir -p $usb_disk/webs
			mkdir -p $usb_disk/scripts
			mkdir -p $usb_disk/lib
			mkdir -p /jffs/softcenter/etc
			mkdir -p /jffs/softcenter/init.d
			mkdir -p /jffs/softcenter/configs
			mkdir -p /jffs/softcenter/ss
			mkdir -p /jffs/softcenter/perp
			ln -sf $usb_disk/bin /jffs/softcenter/
			ln -sf $usb_disk/res /jffs/softcenter/
			ln -sf $usb_disk/webs /jffs/softcenter/
			ln -sf $usb_disk/scripts /jffs/softcenter/
			ln -sf $usb_disk/lib /jffs/softcenter/
			cd $usb_disk && touch .sc_installed
		fi
	fi
else
	logger "Not enough free space for JFFS!/当前jffs分区剩余空间不足!"
	logger "Exit!/退出安装!"
	exit 1
fi
fi
cp -rf /rom/etc/softcenter/scripts/* /jffs/softcenter/scripts/
cp -rf /rom/etc/softcenter/res/* /jffs/softcenter/res/
cp -rf /rom/etc/softcenter/webs/* /jffs/softcenter/webs/
cp -rf /rom/etc/softcenter/bin/* /jffs/softcenter/bin/
cp -rf /rom/etc/softcenter/perp /jffs/softcenter/
cp -rf /rom/etc/softcenter/automount.sh /jffs/softcenter/
if [ "$ROG" == "1" ]; then
	cp -rf /rom/etc/softcenter/ROG/res/* /jffs/softcenter/res/
elif [ "$TUF" == "1" ]; then
	cp -rf /rom/etc/softcenter/ROG/res/* /jffs/softcenter/res/
	sed -i 's/3e030d/3e2902/g;s/91071f/92650F/g;s/680516/D0982C/g;s/cf0a2c/c58813/g;s/700618/74500b/g;s/530412/92650F/g' /jffs/softcenter/res/*.css >/dev/null 2>&1
fi
[ ! -L "/jffs/softcenter/bin/base64_decode" ] && cd /jffs/softcenter/bin && ln -sf base64_encode base64_decode
[ ! -L "/jffs/softcenter/scripts/ks_app_remove.sh" ] && cd /jffs/softcenter/scripts && ln -sf ks_app_install.sh ks_app_remove.sh
chmod 755 /jffs/softcenter/scripts/*.sh
chmod 755 /jffs/softcenter/configs/*.sh
chmod 755 /jffs/softcenter/bin/*
chmod 755 /jffs/softcenter/init.d/*
chmod 755 /jffs/softcenter/perp/*
chmod 755 /jffs/softcenter/perp/.boot/*
chmod 755 /jffs/softcenter/perp/.control/*
chmod 755 /jffs/softcenter/automount.sh
echo 1.2.7 > /jffs/softcenter/.soft_ver
dbus set softcenter_firmware_version=`nvram get extendno|cut -d "_" -f2|cut -d "-" -f1|cut -c2-6`
ARCH=`uname -m`
KVER=`uname -r`
if [ "$ARCH" == "armv7l" ]; then
	if [ "$KVER" == "4.1.52" -o "$KVER" == "3.14.77" ];then
		dbus set softcenter_arch="armng"
	else
		dbus set softcenter_arch="$ARCH"
	fi
else
	if [ "$KVER" == "3.10.14" ];then
		dbus set softcenter_arch="mipsle"
	else
		dbus set softcenter_arch="$ARCH"
	fi
fi

dbus set softcenter_api=`cat /jffs/softcenter/.soft_ver`
dbus set softcenter_version=`cat /jffs/softcenter/.soft_ver`
nvram set sc_installed=1
nvram commit
# creat wan-start file
mkdir -p /jffs/scripts

	if [ -z "$(dbus get softcenter_server_tcode)" ]; then
		modelname=`nvram get modelname`
		if [ "$modelname" == "K3" -o "$modelname" == "XWR3100" ]; then
			dbus set softcenter_server_tcode=CN
		elif [ "$modelname" == "SBRAC1900P" -o "$modelname" == "SBR-AC1900P" -o "$modelname" == "SBRAC3200P" -o "$modelname" == "SBR-AC3200P" -o "$modelname" == "R7900P" -o "$modelname" == "R8000P" -o "$modelname" == "R7000P" ]; then
			dbus set softcenter_server_tcode=ALI
		else
			dbus set softcenter_server_tcode=`nvram get territory_code |cut -c 1-2`
			[ -z "$(dbus get softcenter_server_tcode)" ] && dbus set softcenter_server_tcode=GB
		fi
	fi

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

