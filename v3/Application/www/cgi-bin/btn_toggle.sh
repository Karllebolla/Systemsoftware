#!/bin/sh

BTN_GPIO=17
BTN_PATH="/sys/class/gpio/gpio$BTN_GPIO/value"
BTN_DIRECTION="/sys/class/gpio/gpio$BTN_GPIO/direction"

btn_status=$(cat $BTN_PATH)

echo
if [ "$btn_status" == "0" ]
then
	echo "1" > "$BTN_VALUE"
else
	echo "0" > "$BTN_VALUE"
fi
