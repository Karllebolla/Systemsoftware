#!/bin/sh

dmesg -c | /dev/null
LOAD=$(modprobe kernelThread)
sleep 10
INFO=$(dmesg)

echo
echo "<pre>$INFO</pre>"
