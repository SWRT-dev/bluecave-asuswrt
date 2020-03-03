#!/bin/sh

#From dbus to local variable
eval `dbus export softcenter_installing_`
source /jffs/softcenter/scripts/base.sh
export PERP_BASE=/jffs/softcenter/perp

#softcenter_installing_module 	#正在安装的模块
#softcenter_installing_todo 	#希望安装的模块
#softcenter_installing_tick 	#上次安装开始的时间
#softcenter_installing_version 	#正在安装的版本
#softcenter_installing_md5 	#正在安装的版本的md5值
#softcenter_installing_tar_url 	#模块对应的下载地址

#softcenter_installing_status=		#尚未安装
#softcenter_installing_status=0		#尚未安装
#softcenter_installing_status=1		#已安装
#softcenter_installing_status=2		#将被安装到jffs分区...
#softcenter_installing_status=3		#正在下载中...请耐心等待...
#softcenter_installing_status=4		#正在安装中...
#softcenter_installing_status=5		#安装成功！请5秒后刷新本页面！...
#softcenter_installing_status=6		#卸载中......
#softcenter_installing_status=7		#卸载成功！
#softcenter_installing_status=8		#没有检测到在线版本号！
#softcenter_installing_status=9		#正在下载更新......
#softcenter_installing_status=10	#正在安装更新...
#softcenter_installing_status=11	#安装更新成功，5秒后刷新本页！
#softcenter_installing_status=12	#下载文件校验不一致！
#softcenter_installing_status=13	#然而并没有更新！
#softcenter_installing_status=14	#正在检查是否有更新~
#softcenter_installing_status=15	#检测更新错误！

softcenter_home_url=`dbus get softcenter_home_url`
CURR_TICK=`date +%s`
BIN_NAME=$(basename "$0")
BIN_NAME="${BIN_NAME%.*}"
if [ "$ACTION" != "" ]; then
	BIN_NAME=$ACTION
fi
ARCH=`uname -m`
if [ "$ARCH" == "armv7l" ]; then
	ARCH_SUFFIX="arm"
elif [ "$ARCH" == "aarch64" ]; then
	ARCH_SUFFIX="arm64"
elif [ "$ARCH" == "mips" ]; then
	ARCH_SUFFIX="mips"
elif [ "$ARCH" == "mipsle" ]; then
	ARCH_SUFFIX="mipsle"
else
	ARCH_SUFFIX="arm"
fi
KVER=`uname -r`
if [ "$KVER" == "4.1.52" -o "$KVER" == "3.14.77" ];then
	ARCH_SUFFIX="armng"
fi
if [ "$KVER" == "3.10.14" ];then
	ARCH_SUFFIX="mipsle"
fi
VER_SUFFIX=_version
MD5_SUFFIX=_md5
INSTALL_SUFFIX=_install
UNINSTALL_SUFFIX=_uninstall

LOGGER() {
#	echo $1
	logger $1
}

install_module() {
	if [ "$softcenter_home_url" = "" -o "$softcenter_installing_md5" = "" -o "$softcenter_installing_version" = "" ]; then
		LOGGER "input error, something not found"
		exit 1
	fi

	if [ "$softcenter_installing_tick" = "" ]; then
		export softcenter_installing_tick=0
	fi
	LAST_TICK=`expr $softcenter_installing_tick + 20`
	if [ "$LAST_TICK" -ge "$CURR_TICK" -a "$softcenter_installing_module" != "" ]; then
		LOGGER "module $softcenter_installing_module is installing"
		exit 2
	fi

	if [ "$softcenter_installing_todo" = "" ]; then
		#curr module name not found
		LOGGER "module name not found"
		exit 3
	fi

	# Just ignore the old installing_module
	export softcenter_installing_module=$softcenter_installing_todo
	export softcenter_installing_tick=`date +%s`
	dbus set softcenter_installing_status="2"
	sleep 1
	dbus save softcenter_installing_

	URL_SPLIT="/"
	#OLD_MD5=`dbus get softcenter_module_$softcenter_installing_module$MD5_SUFFIX`
	OLD_VERSION=`dbus get softcenter_module_$softcenter_installing_module$VER_SUFFIX`
	if [ -z "$(dbus get softcenter_server_tcode)" ]; then
		modelname=`nvram get modelname`
		if [ "$modelname" == "K3" ]; then
			dbus set softcenter_server_tcode=CN
		elif [ "$modelname" == "SBRAC1900P" -o "$modelname" == "SBR-AC1900P" -o "$modelname" == "SBRAC3200P" -o "$modelname" == "SBR-AC3200P" -o "$modelname" == "R7900P" -o "$modelname" == "R8000P" ]; then
			dbus set softcenter_server_tcode=ALI
		elif [ "$modelname" == "GTAC2900" -o "$modelname" == "GTAC5300" -o "$modelname" == "RTAC86U" -o "$modelname" == "RTAX86U" -o "$modelname" == "RTAX68U" -o "$modelname" == "RTAX58U" -o "$modelname" == "RTAX82U" -o "$modelname" == "TUFAX3000" -o "$modelname" == "RTACRH17" ]; then
			dbus set softcenter_server_tcode=CN1
		else
			dbus set softcenter_server_tcode=`nvram get territory_code |cut -c 1-2`
			[ -z "$(dbus get softcenter_server_tcode)" ] && dbus set softcenter_server_tcode=GB
		fi
	fi
	eval `dbus export softcenter_server_tcode`
	if [ "$softcenter_server_tcode" == "CN" ]; then
		HOME_URL="http://update.wifi.com.cn/$ARCH_SUFFIX"
	elif [ "$softcenter_server_tcode" == "CN1" ]; then
		HOME_URL="https://sc.softcenter.site/$ARCH_SUFFIX"
	elif [ "$softcenter_server_tcode" == "ALI" ]; then
		HOME_URL="https://wufan.softcenter.site/$ARCH_SUFFIX"
	else
		HOME_URL="https://sc.paldier.com/$ARCH_SUFFIX"
	fi

	#HOME_URL=`dbus get softcenter_home_url`
	TAR_URL=$HOME_URL$URL_SPLIT$softcenter_installing_tar_url
	FNAME=`basename $softcenter_installing_tar_url`

	if [ "$OLD_VERSION" = "" ]; then
		OLD_VERSION=0
	fi

	CMP=`versioncmp $softcenter_installing_version $OLD_VERSION`
	if [ -f /jffs/softcenter/webs/Module_$softcenter_installing_module.sh -o "$softcenter_installing_todo" = "softcenter" ]; then
		CMP="-1"
	fi
	if [ "$CMP" = "-1" ]; then

	cd /tmp
	rm -f $FNAME
	rm -rf "/tmp/$softcenter_installing_module"
	dbus set softcenter_installing_status="3"
	sleep 1
	wget --no-check-certificate --tries=1 --timeout=15 $TAR_URL
	RETURN_CODE=$?

	if [ "$RETURN_CODE" != "0" ]; then
		dbus set softcenter_installing_status="12"
		sleep 2
		dbus set softcenter_installing_status="0"
		dbus set softcenter_installing_module=""
		dbus set softcenter_installing_todo=""
		LOGGER "wget $TAR_URL error, $RETURN_CODE"
	exit 4
	fi

	md5sum_gz=$(md5sum /tmp/$FNAME | sed 's/ /\n/g'| sed -n 1p)
	if [ "$md5sum_gz"x != "$softcenter_installing_md5"x ]; then
		LOGGER "md5 not equal $md5sum_gz"
		dbus set softcenter_installing_status="12"
		rm -f $FNAME
		sleep 2

		dbus set softcenter_installing_status="0"
		dbus set softcenter_installing_module=""
		dbus set softcenter_installing_todo=""

		rm -f $FNAME
		rm -rf "/tmp/$softcenter_installing_module"
		exit
	else
		tar -zxf $FNAME
		dbus set softcenter_installing_status="4"

		if [ ! -f /tmp/$softcenter_installing_module/install.sh ]; then
			dbus set softcenter_installing_status="0"
			dbus set softcenter_installing_module=""
			dbus set softcenter_installing_todo=""

			#rm -f $FNAME
			#rm -rf "/tmp/$softcenter_installing_module"

			LOGGER "package hasn't install.sh"
			exit 5
		fi

		if [ -f /tmp/$softcenter_installing_module/uninstall.sh ]; then
			chmod 755 /tmp/$softcenter_installing_module/uninstall.sh
			mv /tmp/$softcenter_installing_module/uninstall.sh /jffs/softcenter/scripts/uninstall_$softcenter_installing_todo.sh
		fi

		chmod a+x /tmp/$softcenter_installing_module/install.sh
		sh /tmp/$softcenter_installing_module/install.sh
		sleep 2

		rm -f $FNAME
		rm -rf "/tmp/$softcenter_installing_module"

		if [ "$softcenter_installing_module" != "softcenter" ]; then
			dbus set "softcenter_module_$softcenter_installing_module$MD5_SUFFIX=$softcenter_installing_md5"
			dbus set "softcenter_module_$softcenter_installing_module$VER_SUFFIX=$softcenter_installing_version"
			dbus set "softcenter_module_$softcenter_installing_module$INSTALL_SUFFIX=1"
			dbus set "$softcenter_installing_module$VER_SUFFIX=$softcenter_installing_version"
		else
			dbus set softcenter_version=$softcenter_installing_version;
			dbus set softcenter_md5=$softcenter_installing_md5
		fi
		dbus set softcenter_installing_module=""
		dbus set softcenter_installing_todo=""
		dbus set softcenter_installing_status="1"
	fi

	else
		LOGGER "current version is newest version"
		dbus set softcenter_installing_status="13"
		sleep 3

		dbus set softcenter_installing_status="0"
		dbus set softcenter_installing_module=""
		dbus set softcenter_installing_todo=""
	fi
}

uninstall_module() {
	if [ "$softcenter_installing_tick" = "" ]; then
		export softcenter_installing_tick=0
	fi
	LAST_TICK=`expr $softcenter_installing_tick + 20`
	if [ "$LAST_TICK" -ge "$CURR_TICK" -a "$softcenter_installing_module" != "" ]; then
		LOGGER "module $softcenter_installing_module is installing"
		exit 2
	fi

	if [ "$softcenter_installing_todo" = "" -o "$softcenter_installing_todo" = "softcenter" ]; then
		#curr module name not found
		LOGGER "module name not found"
		exit 3
	fi

	ENABLED=`dbus get "$softcenter_installing_todo""_enable"`
	if [ "$ENABLED" = "1" ]; then
		LOGGER "please disable this module than try again"
		exit 4
	fi

	# Just ignore the old installing_module
	export softcenter_installing_module=$softcenter_installing_todo
	export softcenter_installing_tick=`date +%s`
	export softcenter_installing_status="6"
	dbus save softcenter_installing_

	dbus remove "softcenter_module_$softcenter_installing_module$MD5_SUFFIX"
	dbus remove "softcenter_module_$softcenter_installing_module$VER_SUFFIX"
	dbus remove "softcenter_module_$softcenter_installing_module$INSTALL_SUFFIX"

	txt=`dbus list $softcenter_installing_todo`
	printf "%s\n" "$txt" |
	while IFS= read -r line; do
		line2="${line%=*}"
		if [ "$line2" != "" ]; then
			dbus remove $line2
		fi
	done

	sleep 3
	dbus set softcenter_installing_module=""
	dbus set softcenter_installing_status="7"
	dbus set softcenter_installing_todo=""

	#try to call uninstall script
	if [ -f "/jffs/softcenter/scripts/$softcenter_installing_todo$UNINSTALL_SUFFIX.sh"]; then
 		sh /jffs/softcenter/scripts/$softcenter_installing_todo$UNINSTALL_SUFFIX.sh
	elif [ -f "/jffs/softcenter/scripts/uninstall_$softcenter_installing_todo.sh" ]; then
		sh /jffs/softcenter/scripts/uninstall_$softcenter_installing_todo.sh
	else
		rm -f /jffs/softcenter/webs/Module_$softcenter_installing_todo.asp
        rm -f /jffs/softcenter/init.d/S*$softcenter_installing_todo.sh
	fi
}

#LOGGER $BIN_NAME
case $BIN_NAME in
start)
	sh /jffs/softcenter/perp/perp.sh stop
	sh /jffs/softcenter/perp/perp.sh start
	dbus set softcenter_installing_status=1
	;;
update)
	install_module
	;;
install)
	install_module
	;;
ks_app_install)
	install_module
	;;
ks_app_remove)
	uninstall_module
	;;
*)
	install_module
	;;
esac

