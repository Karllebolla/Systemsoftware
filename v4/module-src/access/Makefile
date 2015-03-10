CC=/group/labworks/toolchain/buildroot.git/output/host/usr/bin/arm-buildroot-linux-uclibcgnueabihf-gcc
CFLAGS=-static -Wall
LDFLAGS=
LDLIBS=-pthread
SOURCES=access.c test_openclose.c test_zero.c test_write_null.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=access

.PHONEY: clean all

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDLIBS) $^ -o $@
	
clean:
	rm -rf *.o $(EXECUTABLE)

test_openclose.o: test_openclose.c test_openclose.h
test_zero.o: test_zero.c test_zero.h
test_write_null.o: test_write_null.c test_write_null.h
