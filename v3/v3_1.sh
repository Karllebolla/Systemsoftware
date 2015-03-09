#!/bin/sh

DEFAULT_COMMAND=all

#Variablen
linuxVersion="linux-3.17.2"
patchNameOne="linux-smsc95xx_allow_mac_setting.patch"
patchNameTwo="linux-fix-gpio-enumeration.patch"
newLinuxVersion="linux-3.17.2_patched"
ARCH="arm"
CROSS_COMPILE="/group/labworks/toolchain/buildroot.git/output/host/usr/bin/arm-buildroot-linux-uclibcgnueabihf-"

download() 
{
	#gpg --keyserver hkp://keys.gnupg.net --recv-keys 00411886 6092693E
	echo -e "\033[32mDownload Linuxkernel $linuxVersion\033[0m "
	wget https://www.kernel.org/pub/linux/kernel/v3.x/$linuxVersion.tar.xz
	#wget https://www.kernel.org/pub/linux/kernel/v3.x/$linuxVersion.tar.sign

	#signatur verifizieren
	echo -e "\033[32mVerify kernel\033[0m"
	xz -d $linuxVersion.tar.xz
	#gpg --verify $linuxVersion.tar.sign

	#entpacken des kernels
	echo -e "\033[32mUnzip Linuxkernel $linuxVersion\033[0m"
	tar xvf $linuxVersion.tar

	#unbenoetigte Dateien loeschen
	rm $linuxVersion.tar $linuxVersion.tar.sign
}

patchKernel() 
{
	#kernel patchen
	echo -e "\033[32mPatch Kernel $linuxVersion\033[0m"
	cd $linuxVersion
	patch -p1 < ../rpiPatch/$patchNameOne
	patch -p1 < ../rpiPatch/$patchNameTwo
	cd ..
	#rename folder
	mv $linuxVersion $newLinuxVersion
}


config()
{
	echo -e "\033[32m8. Config the Kernel\033[0m"
	cd $newLinuxVersion
	ARCH=$ARCH make vexpress_defconfig
	cp ../config/kernel.config .config

	cd ../busybox
	./busybtmp.sh
	cd ..
}

compile() 
{
	cd $newLinuxVersion
	mv Makefile Maketmp
	sed  's/CC		= $(CROSS_COMPILE)gcc/CC		= ccache $(CROSS_COMPILE)gcc/' Maketmp > tmp
	sed  's/HOSTCC       = gcc/HOSTCC=ccache gcc/' tmp > Makefile
	rm tmp Maketmp
	ARCH=$ARCH CROSS_COMPILE=$CROSS_COMPILE make -j 4
	ARCH=$ARCH CROSS_COMPILE=$CROSS_COMPILE make vexpress-v2p-ca9.dtb
	#ccache -s
	cd ..
}

emulate() {

	#QEMU_AUDIO_DRV=none qemu-system-arm -kernel arch/$ARCH/boot/zImage -M vexpress-a9  -initrd ../busybox/rootfs -	append "rdinit=/sbin/init" -serial stdio              
	QEMU_AUDIO_DRV=none qemu-system-$ARCH -kernel $newLinuxVersion/arch/$ARCH/boot/zImage -M vexpress-a9 -initrd busybox/rootfs.cpio -append "init=/sbin/init console=ttyAMA0" -net nic,model=lan9118,macaddr=00:00:00:00:00:09,vlan=0 -net vde,sock=/tmp/vde2-tap0.ctl,vlan=0 -dtb ./$newLinuxVersion/arch/$ARCH/boot/dts/vexpress-v2p-ca9.dtb -m 1024 -nographic
 
}

all()
{
	download
	patchKernel
	config
	compile
}

usage() 
{
	echo "Usage $0"
	echo "\t\t--dn for download sources"
	echo "\t\t--pa for patching kernel"
	echo "\t\t--cp for copy of GitLab sources"
	echo "\t\t--co for compile sources"
	echo "\t\t--qe for starting qemu + window with terminal to serial interface"
	echo "\t\tDefault is all"
	exit 1
}

if  [ $# -eq 0 ]
then
		command=$DEFAULT_COMMAND
fi

PARSED_OPTIONS=$(getopt -o hc -l "dn,pa,cp,co,qe" -- "$@")

eval set -- "$PARSED_OPTIONS"

while true; do
	case $1 in
			--dn)
				shift
				download
				;;
			--pa)
				shift
				patchKernel
				;;
			--cp)
				shift
				config
				;;
			--co)
				shift
				compile
				;;
			--qe)
				shift
				emulate
				;;
			-c)
				shift
				config
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

if [ -z $command ]
then
		command=$DEFAULT_COMMAND
fi

#echo "Running $command"

#$command

