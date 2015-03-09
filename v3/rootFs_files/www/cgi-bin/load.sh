#!/bin/sh

dmesg -c | /dev/null
LOAD=$(modprobe lock)
LOAD+=$(modprobe open_once)
LOAD+=$(modprobe myTSBuf)
INFO=$(dmesg)

echo
echo "<pre>$INFO</pre>"
