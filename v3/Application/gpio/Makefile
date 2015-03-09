CC=/group/labworks/toolchain/buildroot.git/output/host/usr/bin/arm-buildroot-linux-uclibcgnueabihf-gcc
CFLAGS=-static -Wall
LDLIBS=-lrt -pthread

SRC=$(wildcard *.c)
OBJ= $(SRC:.c=.o)
BIN= $(SRC:.c=)




.PHONEY: clean all

all: $(BIN)

.c.o:
	@echo [CC] $<
	@$(CC) -c $(CFLAGS) $(LDLIBS) $^ -o $@
	
clean:
	rm -rf *.o $(BIN)
