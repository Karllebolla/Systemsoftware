#!/bin/sh

dmesg -c | /dev/null
LOAD=$(modprobe timer)
sleep 5
INFO=$(dmesg)
UNLOAD=$(rmmod timer)

echo
echo "<pre>$INFO</pre>"
