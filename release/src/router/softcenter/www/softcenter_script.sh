#!/bin/sh 
script_name=`nvram get script_name`
script_content=`nvram get script_content`
echo "$script_content">/tmp/script_temp
sed -i "s/,yushi,/\n/g" /tmp/script_temp
cp -f /tmp/script_temp /jffs/scripts/$script_name 
chmod 755 /jffs/scripts/$script_name

