Entwurf

===========

### ModInit ###

Da nicht direkt auf die HW-Adressen der GPIO-Pins zugegriffen werden 
kann, müssen virtuelle Adressen verwendet werden.
Diese werden mit ioremap() in den Treiber Adresseraum eingeblendet.
Die erste Adresse des GPIO-Pins ist 0x20200000 (GPIO Basisadresse).

### driver_open ###

Makro: #define ADR_GPFSEL(pin) (u32*)(gpio_basis_adr+(pin/10))

In der driver_open Funktion werden die Pins auf in- und output gesetzt.
Dazu müssen die Register GPFSEL verwendet werden. Um dieses Register 
auszuwählen wird das Makro ADR_GPGSEL verwendet. Das Makro gibt einen
Pointer vom Typ u32 zurück.
In einem Register können 10 Pins angesprochen werden. In unserem Makro
wird auf die Basisadresse GPIO Basisadresse der benötigte Pin / 10
addiert. Wird z.B. der Pin 18 angesprochen ist es 18 / 10 = 1. Durch
die Pointerarithmetik wird durch die Addition + 1 ein u32 (32 bit) auf
den Pointer addiert. Dieses entspricht dann dem ersten Register von
GPFSEL.
Möchte man den Pin auf Input setzen, wird die Hexzahl 0x7 um
Pin modulo 10 ( 10 Pins pro Register ) mal 3 ( 3 bits pro Pin ) nach
links geschiftet und in einer Bitmaske gespeichert. Diese Bitmaske muss
anschließend invertiert werden und mit dem Register das auf das 
entsprechende GFPSEL Register zeigt verunded werden.
Wenn man den Pin auf Output setzen möchte, muss andstatt wie oben
beschrieben nicht 0x7 sonder 0x1 geschiftet werden und anstelle der
verundung, verodert werden. Die Invertierung fällt hierbei auch weg.


### driver_write ###

Makro: #define ADR_GPSET(pin) (u32*)(gpio_basis_adr+7+(pin/32))
Makro: #define ADR_GPCLR(pin) (u32*)(gpio_basis_adr+10+(pin/32))

In driver_write wird auf dem Pin entweder 3,3V oder 0V angelegt. Um die 
entprechenden Register auszuwählen werden die Makros ADR_GPSET und
ADR_GPCLR verwendet. Für das setzen für 3,3V wird das Makro ADR_GPSET 
verwendet. Diese gibt ein Point vom Typ u32 zurück, 
dass auf das entsprechende GPSET Register zeigt. 
Dazu addieren wird auf die Basisadresse 7 ( 7 mal ein u32 Register ) und
addieren den benötigten Pin / 32 ( pro GPSET Register werden 32 Pins
angesprochen). Für das GPCLR Register wird identisch vorgegangen, außer
der addition mit 7 auf die Basisadresse wird 10 auf die Basisadresse
addiert.
Um das entsprechende Bit, dass den jeweiligen Pin repräsentiert, muss
an die entpsrechende Stelle eine 1 geschiftet werden. Diese Bitmaske
wird dann an die durch das jeweilige Makro ermittelte Adresse (Register)
geschrieben.


### driver_read ###

Makro: #define ADR_GPLEVpin) (u32*)(gpio_basis_adr+13+(pin/32))

Um zu lesen was an einem Pin anliegt, muss das entsprechende GPLEV
Register ausgewählt werden. Dazu wird auf die Basisadresse 13 addiert
und der benötigte Pin / 32 ebenfalls darauf addiert. Mit dieser Adresse
wird das jeweilige Register ausgelesen, das hier ebenfalls 32 bit breit
ist, jedes bit entspricht dabei einem Pin. Dieses Register muss nun mit
einer Bitmaske verunded werden, an dessen Stelle eine 1 steht, dessen
Pin man auslesen möchte.



Auswertung Zugriff über eigenen GPIO Treiber "mygpio"

===========

### Zeitmessung ###

Über den eigenen Treiber lässt sich eine LED mit einer Frequenz von
200kHz zum Blinken bringen.

Mit dem Zugriff über das SYS-Filesystem kann eine LED mit maximal
3,4kHz zum Blinken gebracht werden.

***Gründe für höhere Frequenz:***

Ein Grund warum unser Treiber schneller ist, kann sein, da der Zugriff
auf ihn nicht Synchronisiert wird. 
Der Zugriff über das SYS - Filesystem ist Synchronisiert, 
die Zeit für die Synchronisation kann ein Grund sein für die langsamere
Frequenz.
Ein weiterer Grund ist, dass bei unserem Treiber nur ein "1" und eine
"0" geschrieben werden. Bei dem Zugriff über das SYS - Filesystem wird
"in" und "out" geschrieben, hierbei müssen mehr Zeichen zwischen User-
und Kernel- Level geschrieben werden.


### Race Conditions ###

***1. Race Condition***
Das device file kann nicht von einem Programm exklusiv geöffnet werden.
Bsp.: "Instanz A" reserviert "Pin 1" als "out", "Instanz B" reserviert
ebenfalls "Pin 1", aber als "in".
"Instanz A" möchte dann auf "Pin 1" schreiben, dies ist aber in der
Zwischenzeit nicht mehr möglich, da "Pin 1" auf "in" gesetzt ist.
Lösung: Device File muss erst reserviert werden. Dann kann es benutzt
werden. Nach dem es fertig benutzt wurde muss es dann wieder
freigegeben werden.
