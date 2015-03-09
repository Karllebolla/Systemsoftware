#!/bin/sh

dmesg -c | /dev/null
UNLOAD=$(rmmod tasklet)
UNLOAD+=$(rmmod timer)
UNLOAD+=$(rmmod kernelThread)
UNLOAD+=$(rmmod wq)
UNLOAD+=$(rmmod lock)
UNLOAD+=$(rmmod open_once)
UNLOAD+=$(rmmod myTSBuf)
INFO=$(dmesg)

echo
echo "<pre>$INFO</pre>"
