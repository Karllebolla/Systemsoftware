#!/bin/sh
# Benjamin Frank      Dominik Riemelmoser
trap "echo \"18\" > /sys/class/gpio/unexport" EXIT

echo "18" > /sys/class/gpio/export

while [ true ]
do
	echo "out" > /sys/class/gpio/gpio18/direction
	echo "0" > /sys/class/gpio/gpio18/value
	sleep 1s
	echo "in" > /sys/class/gpio/gpio18/direction
	sleep 1s
done
