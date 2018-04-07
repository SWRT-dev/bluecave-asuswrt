#!/bin/sh
PATH=/usr/bin:/bin:/usr/sbin:/sbin

prn(){
	echo "BT: $*" | logger -c
}

sleep 30

hciconfig hci0 | grep 'UP RUNNING' >& /dev/null
if [ $? != "0" ] ; then
	prn "No hci0 up running..."
	prn "Restart BT function!"
	killall bluetoothd
	rmmod ath3k
	rmmod btusb
	rmmod xhci_hcd
	modprobe btusb
	modprobe ath3k
	modprobe xhci_hcd
fi
