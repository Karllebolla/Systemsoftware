#!/bin/sh

LED_GPIO=18
LED_PATH="/sys/class/gpio/gpio$LED_GPIO/value"


led_status=$(cat $LED_PATH)

echo
if [ "$led_status" == "0" ]
then
	echo "1" > "$LED_PATH"
else
	echo "0" > "$LED_PATH"
fi
