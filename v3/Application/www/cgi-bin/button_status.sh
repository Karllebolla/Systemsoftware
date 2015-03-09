#!/bin/sh

BTN_GPIO=17
BTN_PATH="/sys/class/gpio/gpio$BTN_GPIO/value"


btn_status=$(cat $LED_PATH)

echo
if [ "$btn_status" == "0" ]
then
	echo "pressed"
else
	echo "unpressed"
fi
