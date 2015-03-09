#!/bin/sh

SYSINFO=$(cat /proc/cpuinfo)

echo
echo "<h1>System Inforamtion</h1>"
echo "<div id=\"sysinfo\">"
echo "<pre>$SYSINFO</pre>"
echo "</div>"

