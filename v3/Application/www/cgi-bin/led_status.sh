#!/bin/sh

LED_GPIO=18
LED_PATH="/sys/class/gpio/gpio$LED_GPIO/value"


led_status=$(cat $LED_PATH)

echo
if [ "$led_status" == "0" ]
then
	echo "on"
else
	echo "off"
fi
