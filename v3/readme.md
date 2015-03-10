# Lernziele

Ziel dieser Aufgabe ist die Erstellung eines Embedded ARM Systems mit Hilfe von buildroot.
Dazu soll ausgehend von einer ARM Standard Konfiguration ein Embedded Linux System erstellt werden, welches die Funktionalität von V1 und V2 besitzt.
Das System soll sowohl unter QEMU also auch auf dem Raspberry Pi funktionieren. Die Aufgaben im einzelnen:

- Gemeinsamer Kernel für Raspberry Pi und Qemu erstellen.
- Gesamtes System in die BuildRoot Generierung übernehmen.

Eine eigene Anwendung soll dann auf dem Raspberry Pi eine einfache Schaltung, bestehen aus LED und Schalter steuern können.
Die GPIO Schnittstelle des Raspberry Pi stellt dafür einen einfachen Anschluss für Sensoren und Aktoren dar.
Der Kernel selbst bietet eine Schnittstelle z.B. über das SYS-Filesystem an.

## Folgende Themen werden erarbeitet:


- Gemeinsamer Kernel für Raspberry Pi und qemu.
    - Patchen.
    - konfigurieren.
    - Kompilierung.
    - Systeme starten und einrichten.
- BuildRoot System.
    - Portierung der Konfiguration in das BuildRoot System.
