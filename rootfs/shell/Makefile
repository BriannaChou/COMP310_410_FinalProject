
CC := aarch64-none-elf-gcc
LD := aarch64-none-elf-ld
SIZE := aarch64-none-elf-size


OBJFILENAME = shell

CONFIGS := -DCONFIG_HEAP_SIZE=4096

CFLAGS := -O0 -ffreestanding -fno-pie -fno-stack-protector -g3 -Wall $(CONFIGS)
CFLAGS += -I../libc/src

ODIR = obj
SDIR = src

OBJS = \
	shell.o \
	start.o \
    nalloc.o \
    rprintf.o \
    clibfuncs.o \
    uart.o \




OBJ = $(patsubst %,$(ODIR)/%,$(OBJS))

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) -c -g -o $@ $^

$(ODIR)/%.o: $(SDIR)/%.s
	$(CC) -c -g -o $@ $^


all: bin

bin: $(OBJ)
	$(LD)  obj/* -L../libc/ -o $(OBJFILENAME)
	$(SIZE) $(OBJFILENAME)

clean:
	rm -f obj/*
	rm -f $(OBJFILENAME)

