#!/bin/sh

cd ../../root/template
MUTEX=$(./access --mutex)

echo
echo "<pre>$MUTEX</pre>"
