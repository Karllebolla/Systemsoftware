#!/bin/bash

DEFAULT_COMMAND=all



#Variablen
linuxVersion="linux-3.17"
newLinuxVersion="linux-3.17.1"
linuxPatch="patch-3.17.1"
ARCH="arm"
CROSS_COMPILE="/group/labworks/toolchain/buildroot.git/output/host/usr/bin/arm-buildroot-linux-uclibcgnueabihf-"



#Kernel 3.14 herunterladen patchen und signatur pruefen
download() 
{
	gpg --keyserver hkp://keys.gnupg.net --recv-keys 00411886 6092693E
	echo -e "\033[32m1. Download Linuxkernel $linuxVersion\033[0m "
	wget https://www.kernel.org/pub/linux/kernel/v3.x/$linuxVersion.tar.xz
	wget https://www.kernel.org/pub/linux/kernel/v3.x/$linuxVersion.tar.sign


	#signatur verifizieren
	echo -e "\033[32m2. Verify kernel\033[0m"
	xz -d $linuxVersion.tar.xz
	gpg --verify $linuxVersion.tar.sign

	#entpacken des kernels
	echo -e "\033[32m3. Unzip Linuxkernel $linuxVersion\033[0m"
	tar xvf $linuxVersion.tar

	#patch herunterladen
	echo -e "\033[32m4. Download patch for Linuxkernel $linuxPatch\033[0m"
	wget https://www.kernel.org/pub/linux/kernel/v3.x/$linuxPatch.gz
	wget https://www.kernel.org/pub/linux/kernel/v3.x/$linuxPatch.sign

	#entpacken des patches
	echo -e "\033[32m5. Unzip Linuxkernel patch $linuxPatch\033[0m"
	gzip -d $linuxPatch.gz

	#patch verifizieren
	echo -e "\033[32m6. Verify patch\033[0m"
	gpg --verify $linuxPatch.sign

	cd $linuxVersion

	#kernel patchen
	echo -e "\033[32m7. Patch Kernel $linuxVersion to $linuxPatch\033[0m"
	patch -p1 < ../$linuxPatch

	cd ..
	#unbenoetigte Dateien loeschen
	rm $linuxPatch.sign $linuxPatch $linuxVersion.tar $linuxVersion.tar.sign

	#linux Ordner umbenenen auf neue Version
	mv $linuxVersion $newLinuxVersion
}

#Kernel konfigurieren
config() 
{

	echo -e "\033[32m8. Config the Kernel\033[0m"
	cd $newLinuxVersion
	ARCH=$ARCH make vexpress_defconfig
	cp ../config/.config .

	cd ../busybox
	./busyb.sh
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
	#ccache -s
	cd ..
}

emulate() {

	#QEMU_AUDIO_DRV=none qemu-system-arm -kernel arch/$ARCH/boot/zImage -M vexpress-a9  -initrd ../busybox/rootfs -	append "rdinit=/sbin/init" -serial stdio
	QEMU_AUDIO_DRV=none qemu-system-$ARCH -kernel $newLinuxVersion/arch/$ARCH/boot/zImage -M vexpress-a9  -initrd busybox/rootfs -append "rdinit=/sbin/init console=ttyAMA0" -net nic,model=lan9118,macaddr=00:00:00:00:00:09,vlan=0 -net vde,sock=/tmp/vde2-tap0.ctl,vlan=0 -nographic

}

all() 
{
	download
	config
	compile
	emulate
}

usage() 
{
	echo "Usage $0 [-c <command>]"
	echo -e "-c:\t\tCommand to execute:"
	echo -e "\t\tdownload, config, compile, emulate, all"
	echo -e "\t\tDefault is -c all"
	exit 1
}


if  [ $# -eq 0 ]
then
		command=$DEFAULT_COMMAND
fi

while getopts ":c:h" opt; do
	case $opt in
			c)
				command=$OPTARG
				;;
			h)
				usage
				;;
			:)
				echo "Option rquires an argument."
				usage
				;;
			?)
				echo "Invalid option!"
				usage
				;;
			*)
				echo "Unimplemented option!"
				usage
				;;
	esac
done
#shift $(($OPTIND -1))

if [ -z $command ]
then
		command=$DEFAULT_COMMAND
fi

echo "Running $command"

$command

