ARCH="arm"
CROSS_COMPILE="/group/labworks/toolchain/buildroot.git/output/host/usr/bin/arm-buildroot-linux-uclibcgnueabihf-"
BB="busybox-1.22.1"
DEFAULT_COMMAND=all

download() {
	
	wget http://www.busybox.net/downloads/$BB.tar.bz2
	bunzip2 $BB.tar.bz2
	tar xfv $BB.tar
	
	
}

config() {
	cd $BB
	ARCH=$ARCH \
	CROSS_COMPILE=$CROSS_COMPILE make allnoconfig > /dev/null
	cp ../.config .
	cd ..
	
}

compile() {

	cd $BB
	ARCH=$ARCH \
	CROSS_COMPILE=$CROSS_COMPILE make -j 4
	cd ..
	
}

install() {
	cd $BB
	ARCH=$ARCH \
	CROSS_COMPILE=$CROSS_COMPILE make install
	cd ..
	
}

createRootFs() {
	
	install
	cd $BB
	cd _install
	mkdir etc etc/init.d tmp proc sys dev etc/udhcp www www/cgi-bin
	cp ../../../Application/shellScript/rcS.sh etc/init.d/rcS
	chmod +x etc/init.d/rcS
	cp ../../../Application/shellScript/inittab etc/inittab
	cd ../../../Application/src
	make
	cd ../gpio
	make
	cd ../../busybox/$BB/_install
	cp ../../../Application/src/helloWorld usr/bin/helloWorld
	cp ../../../Application/gpio/led5 usr/bin/led5
	cp ../../../Application/gpio/switch usr/bin/switch
	cp ../../../Application/gpio/led.sh usr/bin/led.sh
	cp ../examples/udhcp/* etc/udhcp
	
	#dateien fuer http
	cp -r ../../../Application/www/cgi-bin www
	cp -r ../../../Application/www/jquery-ui www
	cp ../../../Application/www/index.html www
	cd ..
	createCpio
	
}

createCpio() {
	
	cd $BB
	#auf gleicher ebene mit dem busybox folder
	ROOTFSCPIO=../../rootfs
	
	cd _install
	rm -rf $ROOTFSCPIO	
	find . | cpio -o --format=newc > $ROOTFSCPIO
	cd ../..
	
}

clean() {

	rm -rf $BB
	rm -rf $BB.tar

}

all() {

	download
	config
	compile
	createRootFs

}

usage() 
{
	echo "Usage $0 [-c <command>]"
	echo -e "-c:\t\tCommand to execute:"
	echo -e "\t\tdownload, config, compile, createRootFs, all"
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
shift $(($OPTIND -1))

if [ -z $command ]
then
		command=$DEFAULT_COMMAND
fi

echo "Running $command"

$command

	
	
	
	
