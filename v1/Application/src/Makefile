CC=gcc
CFLAGS=-static -Wall -m32
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
