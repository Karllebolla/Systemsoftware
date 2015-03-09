#!/bin/sh

dmesg -c | /dev/null
LOAD=$(modprobe tasklet)
sleep 5
INFO=$(dmesg)

echo
echo "<pre>$INFO</pre>"
