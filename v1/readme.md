# Lernziele

Ziel dieser Aufgabe ist die Konfiguration eines Minimalen Linux 3.x Kernels für die x86 (i386) 32Bit Architektur,
der mittels dem InitRamFS (RootFS) bootet und das Ausführen von Shell Parameters erlaubt. Das System wird im QEMU Emulator getestet.

Wichtig ist, dass Sie wirklich ein Minimales System erstellen! Unter anderem werden Dateigröße sowie konfigurierte Parameter ausgewertet.

## Folgende Themen werden erarbeitet:

- GCC - GNU Compiler Collection
- Linux Kernel Struktur
- Linux Kernel Configuration
- Root-FS Erstellung
- Qemu Simulator

# Fragen

Wie können mit der x86_64 Variante von GCC Binärdateien für i386 erzeugt? 

- ARCH=x86 als Flag für make

Welche Schritte sind auszuführen um einen Linux Kernel zu erstellen?

    - Download Linux Kernel von http://www.kernel.org + evt. patchen
    - Kernel konfigurieren: ARCH=x86 make allnoconfig, ARCH=x86 make menuconfig
    - Menuconfig: General Setup -> Initial RAM filesystem and RAM disk support
                  Executable File Formats -> Kernelsupport for Elf-binaries
                  General Setup -> Configure standard kernel features -> enable support for printk
                  Device drivers -> Character devices -> tty
                  Device drivers -> Generic driver options -> devtmpfs
    - Kernel kompilieren: ARCH=x86 make -j 4

Welches sind die wichtigsten Optionen bei der Kernel Konfiguration?

- Initial RAM filesystem and RAM disk support
 - Executable File Formats -> Kernelsupport for Elf-binaries
 - General Setup -> Configure standard kernel features -> enable support for printk

Welche Einstellungen können Sie speziell für eingebettete Systeme vornehmen?

- General setup -> Embedded system


Was ist qemu, wie benutzt man qemu?
 
- Ein Emulator für einen Prozessor und Peripherie.
- Qemu emuliert User-level Prozesse für die Target CPU auf der Host CPU.
- Qemu emuliert komplette Hardwaresysteme.
- Qemu kann als ein Virtual Machine Monitor gesehen werden.


Wie lautet der qemu Aufruf?

- qemu-system-i386 -kernel <link to kernel>/arch/x86/root/bzImage



Wie greift man auf die Monitor COnsole von Qemu zu?

- qemu-system-i386 -kernel <link to kernel>/arch/x86/root/bzImage -qmp tcp:127.0.0.1:4444,server
- Verbinden: telnet localhost 4444

Was ist der Unterschied zwischen tmpfs, ramfs und einer RAM Disk?

 - Tmpfs ist der Nachfolger von ramfs. Unterschied: bei einem tmpfs kann die maximale Größe im Arbeitsspeicher begrenzt werden. Bei ramfs nicht.
 - Ram Disk ist ein virtueller temporärer Datenträger im Arbeitsspeicher.


Was bedeutet initramfs ?

- initial ram filesystem. Nachfolger von initrd.
- Komprimiertes Archiv, das für den Systemstart benötigte Dateien enthält.


Wie gross ist der Startup-Code, wie gross ist das Kernel-Image? Welchen CRC Code erstellt der Kernel-Build Prozess?

- Setup is 15576 bytes (padded to 15872 bytes)
- System is 1916 kB
- CRC 401040ea

Von welchem Typ sind die einzelnen Dateien?

-

Welchen inhalt haben die einzelnen Dateien (Tipp: Programm file)?

-

Welche Images stehen nach einem erfolgreichen Durchlauf von make zur Verfügung? 

- bzImage (nur von bei make übergebenen Architektur) Linux kernel x86 boote executable bzImage


Wie weit bootet der Kernel zum jetzigen Zeitpunkt?

- Bis zum laden des root FS.


Welche Funktionen sind über den Monitor möglich (qemu) ?

- Bietet eine Reihe von Möglichkeiten zur Verwaltung und Steuerung der virtuellen Maschine.
- info GERÄT -> Gibt inofs über das virtuelle Gerät aus z.B. block (Festplatte) snapshot, usb, network.
- change GERÄT GERÄTEDATEI -> tauscht ein Wechselmedium aus.
- commit Schreibt einen snapshot, wenn mit - snapshot gestartet wurde.
- screebdump DATEI erstellt ein Screenshot
- sendkey ctrl-alt-f1 sendet die Tastenkombination.
help  (BEFEHL) Hilfe.


Welches Device (/dev) benötigen sie, damit das Programm ausgaben machen kann?

- /dev/console

Warum hängt sich der Kernel nach der Ausgabe auf?

- Unser init Prgramm gibt die Kontrolle nicht an... ab und beendet sich. Dadurch....


Was macht der Befehl dmesg? Welche Ausgaben liefert dmesg?

- Gibt den Kernel log auf der Konsole aus.

Wieviel Prozesse laufen beim aufruf von top?

- 14 Prozesse

Wieviel Speicher steht Ihnen im laufenden System zu Verfügung?

- 130 MB

Wieviel Speicher muss der Emulator dem Linux System mindestens zu Verfügung stellen, so dass der Linux Kernel korrekt bootet?

- 21 MB mit -m 21 beim Aufruf von qemu
 
Experimentieren Sie mit mdev. Wie können Sie beim Booten das /dev Verzeichnis durch mdev füllen?

-  mdev -s läuft den sysfs-Baum ab und macht alle Aktionen, aber generiert keine Hotplug-events.
Mdev has two primary uses: initial population and dynamic updates.  Both
require sysfs support in the kernel and have it mounted at /sys.  For dynamic
updates, you also need to have hotplugging enabled in your kernel.

http://git.busybox.net/busybox/plain/docs/mdev.txt

