#! /bin/sh

#我们需要延时等待wan口上线和usb挂载
sleep 80s
if [ -e "/jffs/k3cex/www" ] ;then
	/jffs/k3cex/www/script/k3cex-init.sh
	mount --bind /jffs/k3cex/www/web/Softcenter.asp /www/Softcenter.asp 2>/dev/null
else
    sleep 10s
    [ ! -e "/jffs/k3cex/www" ] && sleep 10s
    mount --bind /jffs/k3cex/www/web/Softcenter.asp /www/Softcenter.asp 2>/dev/null
fi

