CC=/group/labworks/toolchain/buildroot.git/output/host/usr/bin/arm-buildroot-linux-uclibcgnueabihf-gcc
CFLAGS=-static -Wall
LDFLAGS=
SOURCES=helloWorld.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=helloWorld

.PHONEY: clean all

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@
	
clean:
	rm -rf *.o helloWorld
