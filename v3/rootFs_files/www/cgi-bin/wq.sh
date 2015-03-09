#!/bin/sh

dmesg -c | /dev/null
LOAD=$(modprobe wq)
sleep 5
INFO=$(dmesg)

echo
echo "<pre>$INFO</pre>"
