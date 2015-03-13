# Lernziele

Bisher wurde die Schnittstelle des SYS-Filesystems f�r den GPIO benutzt.
In diesem Laborversuch entwickeln Sie einen eigenen GPIO Treiber um auch die hardwarenahe Programmierung innerhalb des Linux-Kernels kennen zu lernen.

Die GPIO Schnittstelle des Raspberry Pi erm�glicht einen einfachen Anschluss von Sensoren und Aktoren.
Diese m�ssen jedoch vom Kernel �ber einen Ger�tetreiber den Anwendungen zur Verf�gung gestellt werden.
Der Kernel selbst bietet eine Schnittstelle �ber das SYS-Filesystem, welche jedoch sehr langsam ist.
�ber einen eigenen GPIO Treiber arbeitet die Anwendung etwa 250 Mal schneller als �ber das SYS-Filesystem.

Die Schaltung zur Ansteuerung haben Sie im Prinzip bereits in V3 aufgebaut.
Als Eingabe f�r einen GPIO-Pin benutzen wir einen Schalter, die Ausgabe eines GPIO Pins steuert eine LED an.

## Folgende Themen werden erarbeitet:

- RAW Hardwarezugriffe auf GPIO �ber einen eigenen Treiber.
- Performance Messung der Treiberschnittstelle.
- Analyse kritischer Bereich bei Hardwarezugriffen.
