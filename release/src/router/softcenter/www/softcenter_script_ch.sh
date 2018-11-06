#!/bin/sh 
script_name=`nvram get script_name`
if [ -f "/jffs/scripts/$script_name" ] ;then
cp -f /jffs/scripts/$script_name /tmp/script_temp
else
echo "" > /tmp/script_temp
fi


