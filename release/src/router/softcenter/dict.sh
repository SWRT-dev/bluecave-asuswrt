#!/bin/sh
cd ../www
for i in $(find *.dict)
do
	sed -i '/Softcenter/d' $i
	sed -i '/Tencent Acceleration/d' $i
	sed -i '/TencentAcceleration/d' $i
	if [ "$i" = "CN.dict" ];then
		echo "Softcenter=软件中心" >> $i
		echo "Softcenter_tool=工具箱" >> $i
		echo "TencentAcceleration=腾讯网游加速器" >> $i
	elif [ "$i" = "TW.dict" ];then
		echo "Softcenter=軟體中心" >> $i
		echo "Softcenter_tool=工具箱" >> $i
		echo "TencentAcceleration=腾讯网游加速器" >> $i
	else
		echo "Softcenter=Softcenter" >> $i
		echo "Softcenter_tool=Tools" >> $i
		echo "TencentAcceleration=Tencent Acceleration" >> $i
	fi
done

