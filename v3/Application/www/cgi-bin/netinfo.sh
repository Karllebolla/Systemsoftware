#!/bin/sh

IFCONFIG=$(ifconfig)

echo
echo "<h1>Network Devoces</h1>"
echo "<div id=\"ifconfig\">"
echo "<pre>$IFCONFIG</pre>"
echo "</div>"
