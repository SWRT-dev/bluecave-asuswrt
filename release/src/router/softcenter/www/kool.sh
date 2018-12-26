#!/bin/sh

kool_mod=`nvram get koolproxy_mode 2>/dev/null`
https_en=`nvram get koolproxy_https_mode 2>/dev/null`
kool_sip=`nvram get koolproxy_srcip 2>/dev/null`
kool_httpsip=`nvram get koolproxy_httpsip 2>/dev/null`
kool_mon=`nvram get koolproxy_monitor 2>/dev/null`
adblock_mode=`nvram get adblock_mode 2>/dev/null`
usb_uuid=`dbus get jffs_ext`
if [ -n "$usb_uuid" ]; then
mdisk=`blkid |grep "${usb_uuid}" |cut -c6-9`
else
mdisk=`nvram get k3c_disk`
fi
usb_disk="/tmp/mnt/$mdisk"

cd /tmp
#等待网络连接成功
wget_ok="0"
get_source="0"

if [ "$adblock_mode" == "1" ]  ;then
#kp处理
while [ "$wget_ok" = "0" ] 
do 

 wget --no-check-certificate https://koolproxy.com/downloads/mips -O /tmp/mips -t 1 -T 5 2>/dev/null
 if [ "$?" == "0" ]; then
 wget_ok="1"
 else
  wget http://iytc.net/tools/mips_new -O /tmp/mips -t 1 -T 5 2>/dev/null
  if [ "$?" == "0" ]; then
  wget_ok="1"
  else
   sleep 30
  fi
 fi
  echo " $(date "+%F %T"):""KP已成功下载" >> /tmp/adbyby.log
done

#解压、启动

mkdir -p koolproxy
mv mips ./koolproxy/koolproxy
chmod 755 /tmp/koolproxy/koolproxy

else
#adbyby处理
#cp -rf /usr/adbyby /root
while [ "$wget_ok" = "0" ] 
do 
  wget --no-check-certificate https://k3c.paldier.com/tools/adbyby-K3C.zip -O /tmp/adbyby-K3C.zip -t 1 -T 5
  if [ "$?" = "0" ] ;then
     wget_ok="1"
  else
    sleep 30
  fi
done
  echo " $(date "+%F %T"):""adbyby已成功下载" >> /tmp/adbyby.log
mkdir $usb_disk/adbyby
unzip -o /tmp/adbyby-K3C.zip -d $usb_disk/adbyby
rm -f /tmp/adbyby-K3C.zip
chmod 755 $usb_disk/adbyby/adbyby
fi


if [ "$adblock_mode" == "1" ]  ;then
#kp处理
#更新一次规则
cd ./koolproxy
mkdir -p data/rules
  echo " $(date "+%F %T"):""更新规则..." >> /tmp/adbyby.log
wget  --no-check-certificate https://kprule.com/kp.dat -O /tmp/koolproxy/data/rules/kp.dat -t 1 -T 5 2>/dev/null
wget  --no-check-certificate https://kprule.com/daily.txt -O /tmp/koolproxy/data/rules/daily.txt -t 1 -T 5 2>/dev/null
wget  --no-check-certificate https://kprule.com/koolproxy.txt -O /tmp/koolproxy/data/rules/koolproxy.txt -t 1 -T 5 2>/dev/null
if [ "$?" != "0" ]; then
wget http://iytc.net/tools/koolproxy_new.txt -O /tmp/koolproxy/data/rules/koolproxy.txt -t 1 -T 5 2>/dev/null
wget http://iytc.net/tools/1_new.dat -O /tmp/koolproxy/data/rules/kp.dat -t 1 -T 5 2>/dev/null
fi

if [ -f "/jffs/configs/kool_user.txt" ] ;then
cp -f /jffs/configs/kool_user.txt  /tmp/koolproxy/data/rules/user.txt 2>/dev/null
else
cp -f /usr/kool/user.txt  /tmp/koolproxy/data/rules/user.txt 2>/dev/null
fi

if [ "$https_en" = "1" ]  ;then
 if [ ! -f "/jffs/koolproxy/data/certs/ca.crt" ] ;then
  mkdir -p /jffs/koolproxy/data/certs
  cp -f /usr/kool/gen_ca.sh /jffs/koolproxy/data/gen_ca.sh
  cp -f /usr/kool/openssl.cnf /jffs/koolproxy/data/openssl.cnf
  cd /jffs/koolproxy/data
 ./gen_ca.sh >/dev/null 2>&1   
 fi
cp -r /jffs/koolproxy/data/certs  /tmp/koolproxy/data 2>/dev/null
cp -r /jffs/koolproxy/data/private  /tmp/koolproxy/data 2>/dev/null
  echo " $(date "+%F %T"):""证书已生成，路径/jffs/koolproxy/data/certs/ca.crt" >> /tmp/adbyby.log
fi


if [ -f "/tmp/koolproxy/koolproxy"  ] ;then
 cd /tmp/koolproxy
  ./koolproxy -p 8118 >/dev/null 2>&1 &  
  echo " $(date "+%F %T"):""koolproxy已成功启动" >> /tmp/adbyby.log
else
return 1
fi
else
#adbyby处理
#更新一次规则
  echo " $(date "+%F %T"):""更新规则..." >> /tmp/adbyby.log
wget --no-check-certificate https://raw.githubusercontent.com/adbyby/xwhyc-rules/master/video.txt -O /tmp/video.txt -t 1 -T 5 2>/dev/null
wget --no-check-certificate https://raw.githubusercontent.com/adbyby/xwhyc-rules/master/lazy.txt  -O /tmp/lazy.txt -t 1 -T 5 2>/dev/null
if [ "$?" == "0" ]; then
cp -f /tmp/video.txt $usb_disk/adbyby/data/video.txt 2>/dev/null
cp -f /tmp/lazy.txt $usb_disk/adbyby/data/lazy.txt 2>/dev/null
rm -f /tmp/video.txt
rm -f /tmp/lazy.txt
else
sleep 60
  wget --no-check-certificate https://raw.githubusercontent.com/adbyby/xwhyc-rules/master/video.txt -O /tmp/video.txt -t 1 -T 5 2>/dev/null
  wget --no-check-certificate https://raw.githubusercontent.com/adbyby/xwhyc-rules/master/lazy.txt  -O /tmp/lazy.txt -t 1 -T 5 2>/dev/null
if [ "$?" == "0" ]; then
cp -f /tmp/video.txt $usb_disk/adbyby/data/video.txt 2>/dev/null
cp -f /tmp/lazy.txt $usb_disk/adbyby/data/lazy.txt 2>/dev/null
rm -f /tmp/video.txt
rm -f /tmp/lazy.txt
else
  wget  http://down.iytc.net/tools/video.txt -O /tmp/video.txt -t 1 -T 5 2>/dev/null
  wget http://down.iytc.net/tools/lazy.txt  -O /tmp/lazy.txt -t 1 -T 5 2>/dev/null
if [ "$?" == "0" ]; then
cp -f /tmp/video.txt $usb_disk/adbyby/data/video.txt 2>/dev/null
cp -f /tmp/lazy.txt $usb_disk/adbyby/data/lazy.txt 2>/dev/null
rm -f /tmp/video.txt
rm -f /tmp/lazy.txt
fi
fi
fi
if [ -f "/jffs/configs/kool_user.txt" ] ;then
 cp -f /jffs/configs/kool_user.txt  $usb_disk/adbyby/data/user.txt 2>/dev/null
fi
  cd $usb_disk/adbyby
  ./adbyby >/dev/null 2>&1 &
  echo " $(date "+%F %T"):""adbyby已成功启动" >> /tmp/adbyby.log
fi


iptables -t nat  -N kool_chain 2>/dev/null
iptables -t nat  -F kool_chain 2>/dev/null
iptables -t nat  -C PREROUTING -p tcp -j kool_chain 2>/dev/null
if [ "$?" != "0" ]  ;then
iptables -t nat  -A PREROUTING -p tcp -j kool_chain 2>/dev/null
fi

iptables -t nat  -A kool_chain -d 192.168.0.0/16 -j RETURN
iptables -t nat  -A kool_chain -d 10.0.0.0/8 -j RETURN
iptables -t nat  -A kool_chain -d 0.0.0.0/8 -j RETURN
iptables -t nat  -A kool_chain -d 127.0.0.0/8 -j RETURN
iptables -t nat  -A kool_chain -d 224.0.0.0/4 -j RETURN

if [ "$kool_mod" == "1" ]; then
ipset create adblock nethash 2>/dev/null
ipset add adblock 110.110.110.110 2>/dev/null
cp -f /usr/kool/adblock.conf /tmp/etc/dnsmasq.user/adblock.conf
/sbin/restart_dns 2>/dev/null
 iptables -t nat  -C kool_chain -p tcp --dport 80 -m set --match-set adblock dst -j REDIRECT --to-ports 8118  2>/dev/null
 if [ "$?" != "0" ]  ;then
  iptables -t nat  -A kool_chain -p tcp --dport 80 -m set --match-set adblock dst -j REDIRECT --to-ports 8118  2>/dev/null
  if [ -f "/tmp/koolproxy/data/certs/ca.crt" -a "$https_en" = "1"  ] ;then
   if [ -z "$kool_httpsip" ] ;then
   iptables -t nat -A kool_chain -p tcp --dport 443 -m set --match-set adblock dst -j REDIRECT --to-ports 8118  2>/dev/null
   else
	echo $kool_httpsip>/tmp/kool_httpsip.txt
	sed -i "s/,/\n/g" /tmp/kool_httpsip.txt
	sed '/.*/s/.*/iptables -t nat -A kool_chain -s & -p tcp --dport 443 -m set --match-set adblock dst -j REDIRECT --to-ports 8118/' /tmp/kool_httpsip.txt | sh
	rm -f /tmp/kool_httpsip.txt
   fi
  fi
 fi
else
 iptables -t nat  -C kool_chain -p tcp --dport 80 -j REDIRECT --to-ports 8118  2>/dev/null
 if [ "$?" != "0" ]  ;then
  iptables -t nat  -A kool_chain -p tcp --dport 80 -j REDIRECT --to-ports 8118  2>/dev/null
  if [ -f "/tmp/koolproxy/data/certs/ca.crt" -a "$https_en" = "1"  ] ;then
   if [ -z "$kool_httpsip" ] ;then
   iptables -t nat -A kool_chain -p tcp --dport 443 -j REDIRECT --to-ports 8118  2>/dev/null
   else
	echo $kool_httpsip>/tmp/kool_httpsip.txt
	sed -i "s/,/\n/g" /tmp/kool_httpsip.txt
	sed '/.*/s/.*/iptables -t nat -A kool_chain -s & -p tcp --dport 443 -j REDIRECT --to-ports 8118/' /tmp/kool_httpsip.txt | sh
	rm -f /tmp/kool_httpsip.txt
   fi
  fi
 fi
fi

if [ ! -z "$kool_sip" ] ;then
echo $kool_sip>/tmp/kool_sip.txt
sed -i "s/,/\n/g" /tmp/kool_sip.txt
sed '/.*/s/.*/iptables -t nat -I kool_chain -s & -j RETURN/' /tmp/kool_sip.txt | sh
rm -f /tmp/kool_sip.txt
fi

if [ -f "/jffs/configs/kool_noblock.txt" ] ;then
icount=`cat /jffs/configs/kool_noblock.txt|grep .|wc -l`
if [ $icount -gt 0 ] ;then
ipset create noadblock nethash 2>/dev/null
sed '/.*/s/.*/ipset=\/&\/noadblock/' /jffs/configs/kool_noblock.txt >/tmp/etc/dnsmasq.user/noadblock.conf
/sbin/restart_dns 2>/dev/null
iptables -t nat  -I kool_chain -p tcp  -m set --match-set noadblock dst -j RETURN
else
rm -f /tmp/etc/dnsmasq.user/noadblock.conf
/sbin/restart_dns 2>/dev/null
fi
fi

if [ "$kool_mon" == "1" ]  ;then
/usr/kool/kool_mon.sh &
  echo " $(date "+%F %T"):""守护已成功启动" >> /tmp/adbyby.log
fi

