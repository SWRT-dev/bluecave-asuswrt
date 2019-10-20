#!/bin/sh

# this scripts used for .asusrouer to start softcenter
source /jffs/softcenter/scripts/base.sh
sh /jffs/softcenter/perp/perp.sh start
nvram set jffs2_scripts=1
nvram commit

#============================================
# check start up scripts 
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

if [ -f "/jffs/scripts/service-start" ];then
	STARTCOMAND=`cat /jffs/scripts/service-start | grep -c "watchdog"`
	[ "$STARTCOMAND" -gt "1" ] && sed -i '/watchdog/d' /jffs/scripts/service-start && reboot
fi

if [ ! -f "/jffs/scripts/nat-start" ];then
	cat > /jffs/scripts/nat-start <<-EOF
	#!/bin/sh
	/jffs/softcenter/bin/softcenter-net.sh start_nat
	EOF
	chmod +x /jffs/scripts/nat-start
else
	STARTCOMAND2=`cat /jffs/scripts/nat-start | grep -c "/jffs/softcenter/bin/softcenter-net.sh start"`
	[ "$STARTCOMAND2" -gt "1" ] && sed -i '/softcenter-net.sh/d' /jffs/scripts/nat-start && sed -i '1a /jffs/softcenter/bin/softcenter-net.sh start_nat' /jffs/scripts/nat-start
	[ "$STARTCOMAND2" == "0" ] && sed -i '1a /jffs/softcenter/bin/softcenter-net.sh start_nat' /jffs/scripts/nat-start
fi

if [ ! -f "/jffs/scripts/post-mount" ];then
	cat > /jffs/scripts/post-mount <<-EOF
	#!/bin/sh
	/jffs/softcenter/bin/softcenter-mount.sh start
	EOF
	chmod +x /jffs/scripts/post-mount
else
	STARTCOMAND3=`cat /jffs/scripts/post-mount | grep -c "/jffs/softcenter/bin/softcenter-mount.sh start"`
	[ "$STARTCOMAND3" -gt "1" ] && sed -i '/softcenter-mount.sh/d' /jffs/scripts/post-mount && sed -i '1a /jffs/softcenter/bin/softcenter-mount.sh start' /jffs/scripts/post-mount
	[ "$STARTCOMAND3" == "0" ] && sed -i '1a /jffs/softcenter/bin/softcenter-mount.sh start' /jffs/scripts/post-mount
fi
#============================================

