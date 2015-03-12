#!/bin/sh

#variable
DRIVERNAME=

info() {
	modinfo $DRIVERNAME
}

load() {
	dmesg -C
	modprobe $DRIVERNAME
}

loadInfo() {
	dmesg
	#cat /var/log/kern.log | grep $DRIVERNAME
}

infoProc() {
	cat /proc/devices | grep $DRIVERNAME
}

unload() {
	dmesg -C
	rmmod $DRIVERNAME
}

unloadInfo() {
	dmesg
	#cat /var/log/kern.log | grep $DRIVERNAME
}


usage() 
{
	echo "Usage $0"
	echo "\t\t--info <DRIVER_NAME> show information via modinfo"
	echo "\t\t--load <DRIVER_NAME> load driver modul"
	echo "\t\t--loadinfo <DRIVER_NAME> show load info from kernel log"
	echo "\t\t--infoproc <DRIVER_NAME> show info from proc/devices"
	echo "\t\t--unload <DRIVER_NAME> unload drivermodul"
	echo "\t\t--unloadinfo <DRIVER_NAME> show unload info from kernel log"
	exit 1
}

if  [ $# -eq 0 ]
then
		command=$DEFAULT_COMMAND
fi

PARSED_OPTIONS=$(getopt -o hil -l "info:,load:,loadinfo:,infoproc:,unload:,unloadinfo:" -- "$@")

eval set -- "$PARSED_OPTIONS"

while true; do
	case $1 in
			--info|i)
				 
				DRIVERNAME=$2
				info
				shift
				;;
			--load|l)
				DRIVERNAME=$2
				load
				shift
				;;
			--loadinfo)
				DRIVERNAME=$2
				loadInfo
				shift
				;;
			--infoproc)
				DRIVERNAME=$2
				infoProc
				shift
				;;
			--unload)
				DRIVERNAME=$2
				unload
				shift
				;;
			--unloadinfo)
				DRIVERNAME=$2
				unloadInfo
				shift
				;;
			-h)
				usage
				;;
			?)
				echo "Invalid option!"
				usage
				;;
			--)
				shift
				break
				;;
	esac
done

