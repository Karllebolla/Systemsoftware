#!/bin/sh

echo "Mounting proc Filesystem..."
mount -t proc /proc /proc 
echo "Mounting sys Filesystem..."
mount -t sysfs /sys /sys
echo "Mounting tmp Filesystem..."
mount -t tmpfs /tmp /tmp

/sbin/mdev -s

echo "Network"

udhcpc -s /etc/udhcp/simple.script
echo ""
echo "loopback interface"
ifconfig lo 127.0.0.1
route add -host 127.0.0.1 lo


httpd -h /www/ &

exec /usr/bin/helloWorld

