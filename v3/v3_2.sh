#!/bin/sh

DEFAULT_COMMAND=all

#Variablen
LINUXVERSION="linux-3.17.2"
patchName="linux-smsc95xx_allow_mac_setting.patch"
ARCH="arm"
CROSS_COMPILE="/group/labworks/toolchain/buildroot.git/output/host/usr/bin/arm-buildroot-linux-uclibcgnueabihf-"

download() 
{
	echo "\033[32mDownload buildroot\033[0m"
	#download buildroot
	git clone git://git.buildroot.net/buildroot
	cd buildroot
	git checkout 80e406090893ab5a7b5d78e4d243d12fca2b22fb
	rm -rf .git
	cd ..
}


copy()
{
	echo "\033[32mConfig buildroot\033[0m"
	cd buildroot
	cp -r ../package/* package/
	cp ../config/Config.in package/
	make defconfig BR2_DEFCONFIG=../config/buildroot.config
	cd ..
}

compile() 
{
	echo "\033[32mCompile buildroot\033[0m"
	cd buildroot
	make source
	make
	cd ..
	copyTftp
}

copyTftp() {
	echo "\033[32mCopy Files to TFTP Folder\033[0m"
	cd buildroot/output/images
	fakeroot cp rootfs.cpio.uboot /srv/tftp/rpi/7/
	fakeroot cp zImage /srv/tftp/rpi/7/
	fakeroot cp bcm2835-rpi-b.dtb /srv/tftp/rpi/7/
}

all()
{
	download
	copy
	compile
}

usage() 
{
	echo "Usage $0"
	echo "\t\t--run for download sources and patching"
	echo "\t\t--cp for copy of GitLab sources"
	echo "\t\t--co for compile sources"
	echo "\t\tDefault is all"
	exit 1
}

if  [ $# -eq 0 ]
then
		command=$DEFAULT_COMMAND
fi

PARSED_OPTIONS=$(getopt -o hc -l "run,cp,co" -- "$@")

eval set -- "$PARSED_OPTIONS"

while true; do
	case $1 in
			--run)
				shift
				download
				;;
			--cp)
				shift
				copy
				;;
			--co)
				shift
				compile
				;;
			-c)
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

#if [ -z $command ]
#then
#		command=$DEFAULT_COMMAND
#fi

#echo "Running $command"

#$command

