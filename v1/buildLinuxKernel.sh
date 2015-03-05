#!/bin/bash

gpg --keyserver hkp://keys.gnupg.net --recv-keys 00411886 6092693E

#Kernel 3.14 herunterladen
echo -e "\033[32m1. Download Linuxkernel 3.14\033[0m "
wget https://www.kernel.org/pub/linux/kernel/v3.x/linux-3.17.tar.xz
wget https://www.kernel.org/pub/linux/kernel/v3.x/linux-3.17.tar.sign


#signatur verifizieren
echo -e "\033[32m2. Verify kernel\033[0m"
xz -d linux-3.17.tar.xz
gpg --verify linux-3.17.tar.sign

#entpacken des kernels
echo -e "\033[32m3. Unzip Linuxkernel 3.14\033[0m"
tar xvf linux-3.17.tar

#patch herunterladen
echo -e "\033[32m4. Download patch for Linuxkernel 3.14.22\033[0m"
wget https://www.kernel.org/pub/linux/kernel/v3.x/patch-3.17.1.gz
wget https://www.kernel.org/pub/linux/kernel/v3.x/patch-3.17.1.sign

#entpacken des patches
echo -e "\033[32m5. Unzip Linuxkernel patch 3.14.22\033[0m"
gzip -d patch-3.17.1.gz

#patch verifizieren
echo -e "\033[32m6. Verify patch\033[0m"
gpg --verify patch-3.17.1.sign

cd linux-3.17

#kernel patchen
echo -e "\033[32m7. Patch Kernel 3.14 to 3.14.22\033[0m"
patch -p1 < ../patch-3.17.1

cd ..
#unbenoetigte Dateien loeschen
rm patch-3.17.1.sign patch-3.17.1 linux-3.17.tar linux-3.17.tar.sign

#linux Ordner umbenenen auf neue Version
mv linux-3.17 linux-3.17.1

#Kernel konfigurieren
echo -e "\033[32m8. Config the Kernel\033[0m"
cd linux-3.17.1
ARCH=x86 make allnoconfig
cp ../config/.config .
cp ../config/busybox .
echo -e "\033[32m8. Busybox\033[0m"
fakeroot ./busybox --install -s
#erstellung von initramfs
#nod /dev/console 777 0 0 c 5 1 wird mdev -s ersetzt
#slink /bin/kill busybox 777 0 0 wird durch busybox -install ersetzt 
touch rootFsConfigFile
echo "dir /dev 777 0 0
dir /sbin 777 0 0
dir /bin 777 0 0
dir /usr 777 0 0
dir /usr/sbin 777 0 0
dir /usr/bin 777 0 0
dir /etc 777 0 0
dir /var 777 0 0
dir /tmp 777 0 0
dir /proc 777 0 0
dir /sys 777 0 0
dir /etc/init.d 777 0 0
nod /dev/console 777 0 0 c 5 1
file /bin/busybox busybox 777 0 0
file /usr/bin/helloWorld Application/src/helloWorld 755 0 0
file /etc/inittab Application/shellScript/inittab 755 0 0
file /etc/init.d/rcS Application/shellScript/mountDevice.sh 755 0 0
slink /init /bin/busybox 777 0 0
slink /bin/sh busybox 777 0 0" >> rootFsConfigFile

cp -r ../Application .
cd Application/src
make
cd ../..


mv Makefile Maketmp
sed  's/CC		= $(CROSS_COMPILE)gcc/CC		= ccache $(CROSS_COMPILE)gcc/' Maketmp > tmp
sed  's/HOSTCC       = gcc/HOSTCC=ccache gcc/' tmp > Makefile
rm tmp Maketmp

ARCH=x86 make -j 4
ccache -s
cd ..
qemu-system-i386 -kernel linux-3.17.1/arch/x86/boot/bzImage -append "mem=4MB"

