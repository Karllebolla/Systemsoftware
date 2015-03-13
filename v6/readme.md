# Lernziele

Bisher wurde die Schnittstelle des SYS-Filesystems für den GPIO benutzt.
In diesem Laborversuch entwickeln Sie einen eigenen GPIO Treiber um auch die hardwarenahe Programmierung innerhalb des Linux-Kernels kennen zu lernen.

Die GPIO Schnittstelle des Raspberry Pi ermöglicht einen einfachen Anschluss von Sensoren und Aktoren.
Diese müssen jedoch vom Kernel über einen Gerätetreiber den Anwendungen zur Verfügung gestellt werden.
Der Kernel selbst bietet eine Schnittstelle über das SYS-Filesystem, welche jedoch sehr langsam ist.
Über einen eigenen GPIO Treiber arbeitet die Anwendung etwa 250 Mal schneller als über das SYS-Filesystem.

Die Schaltung zur Ansteuerung haben Sie im Prinzip bereits in V3 aufgebaut.
Als Eingabe für einen GPIO-Pin benutzen wir einen Schalter, die Ausgabe eines GPIO Pins steuert eine LED an.

## Folgende Themen werden erarbeitet:

- RAW Hardwarezugriffe auf GPIO über einen eigenen Treiber.
- Performance Messung der Treiberschnittstelle.
- Analyse kritischer Bereich bei Hardwarezugriffen.
