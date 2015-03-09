#!/bin/sh

cd ../../root/template
BUFFERBLOCKING=$(./access --buffer)

echo
echo "<pre>$BUFFERBLOCKING</pre>"
