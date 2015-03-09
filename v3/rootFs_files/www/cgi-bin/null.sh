#!/bin/sh

cd ../../root/template
NULL=$(./access -n)

echo
echo "<pre>$NULL</pre>"
