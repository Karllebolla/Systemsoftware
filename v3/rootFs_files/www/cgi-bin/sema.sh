#!/bin/sh

cd ../../root/template
SEMA=$(./access --sema)

echo
echo "<pre>$SEMA</pre>"
