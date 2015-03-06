# Kernel Source konfigurieren

-  Wie müssen nun die ARCH und CROSS_COMPILE Variablen gesetzt werden?

        - ARCH=arm

        - CROSS_COMPILE=/group/labworks/toolchain/buildroot.git/output/host/usr/bin/arm-buildroot-linux-uclibcgnueabihf-

-  Wie können Sie diese Variablen in Ihrer Shell automatisch setzen, sodass Sie die Variablen nicht ständig angeben müssen?
        - export VAR=Wert

- Konfiguration erstellen
        - make vexpress_defconfig

        - Manuelle Einstellung
            -  .....................

-  Welche Gerätetreiber sind aktiviert?

    - Memory Technology Device support
    - Block devices
    - SCSI device support -> SCSI disk support
    - Serial ATA and ...
    - Network device support -> Ethernet driver support
    - HID devices
    - USB support
    - MMC/SD/SDIO card support
    - Realtime Clock
    - IOMMU Hardware support


- Welche Konsolen sind konfiguriert?
    - 

-  Deaktivieren Sie den 'Loadable Module Support' im Kernel und achten Sie auf die feste Einbindung der Treiber in den Kernel.
    - Ändert die Einbindung von "Modul" zu "integriert"
 
- Wie muss QEMU für unser gewähltes ARM Board gestartet werden?
    - qemu-system-arm -M vexpress-a9 -kernel <Kernel_Path>
 
- Welche ARM Architekturen unterstützt QEMU?
    - 
 
- Damit Sie die Bootmeldungen des Kernels sehen können, muss der Kernel mit einem geeigneten console= Parameter gestartet werden:

    Warum benötigt der Kernel diesen Parameter, wie lautet dieser Parameter?
    - -serial stdio -append "console=ttyAMA0"
 
- Benutzen Sie nun eine serielle Schnittstelle für die Konsole
    - Welche Konsole ist im Linux Kernel konfiguriert?
        - console=ttyAMA0

    - Wie aktivieren Sie die Konsole unter Linux beim Booten?
        - -append "console=tty1"

    - Wie können Sie sich die Konsole unter QEMU ausgeben lassen?
        - .....................

    - Experimentieren Sie auch mit der Konsolenausgabe von QEMU auf eine pty Schnittstelle von homer. Mittels minicom oder screen können Sie auf pty Schnittstellen zugreifen.
        - minicom 

        screen:

        - -serial pty -append "console=ttyAMA0" -nographic -> char device redericted to /dev/pts/31
        - extra Konsole folgender Aufruf starten: screen /dev/pts/31


    - Zum Rebooten des Kernels in Qemu benutzen Sie dessen Monitorfunktionialität.
        - qemu system_reset

# BusyBox konfigurieren

- Betrachten Sie das erzeugte Binary mit dem Kommando file. Welche Informationen erhalten Sie? Wie gross ist das Binary? Überlegen Sie, welche Möglichkeiten Ihnen zu Verfügung stehen, ein kleineres Binary zu Verfügung zu stellen.
    - dynamisch Linken
    - entfernen von Funktionen
    - kleiner lib als cLibc benutzen   z.B. uLibc
    - busybox: ELF 32-bit LSB excutable, ARM, version 1 (SYSV), statically linked, stripped


- Führen Sie das Binary aus. Was passiert?
    - Es wird angezeit wie busybox aufgerufen werden kann
    - Es wird angezeit was für FUnktionen definiert wurden. Z.B. cat, chmod, echo,.....

# Erstellen des rootFS mit busybox
    - ARCH=arm CROSS_COMPILE=.... busybox -install
        - make menuconfig -> BusyBox Settings -> Install Options kann der name des rootFs festgelegt werden

- Betrachten Sie das Layout des Dateisystems von dem RootFS Abbild.
    - 

- Welche Prozesse laufen? Wieviel Speicher (RAM) belegt das derzeitige System?
    -  init, /bin/ash
    - 6,5 MB
# Netzwerk
 
- Welche Netzwerkkarten sind in der Linux Kernel Konfiguration aktiviert?
    - Device Drivers -> Ethernet driver support
        - ARC, Cadence, Brodcom, Cirrus, Faraday, Hisilicon, Intel, Marvell, Micrel, National Semi-conductor, Samsung Ethernet, SEEQ, SMC, STMicroelectronics, VIA, WIZnet

    - Welche Netzwerkkarten unterstützt QEMU? Welche brauchen Sie?
        - qemu-system-arm -M vexpress-a9 -net nic,model=? -> lan9118

    - qemu aufruf
        - "-net nic,model=lan9118,macaddr=00:00:00:00:09,vlan=0
           -net vde,sock= /tmp/vde2-tap0.ctl,vlan=0"

- Warum funktioniert ein ping auf die eigene zugewiesene IP-Adresse evtl. nicht? (Hinweis: Welches Interface gibt es neben eth0 noch?) 
    - loopback interface 127.0.0.1

Aktivieren Sie den telnetd auf dem Target und loggen Sie sich vom Host ein.

    - auf target: telnetd -l/bin/sh
    - auf host: telnet 192.168.29.X


