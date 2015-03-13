CC=/group/labworks/toolchain/buildroot.git/output/host/usr/bin/arm-buildroot-linux-uclibcgnueabihf-gcc
CFLAGS=-static -Wall
LDFLAGS=
LDLIBS=
SOURCES=ledmaxSys.c differenz_zeitmessung.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ledmaxSys

.PHONEY: clean all

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDLIBS) $^ -o $@
	
clean:
	rm -rf *.o $(EXECUTABLE)

differenz_zeitmessung.o: differenz_zeitmessung.c differenz_zeitmessung.h

