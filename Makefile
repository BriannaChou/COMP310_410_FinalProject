
#CC := aarch64-none-elf-gcc
#LD := aarch64-none-elf-ld
#OBJDUMP := aarch64-none-elf-objdump
#OBJCOPY := aarch64-none-elf-objcopy


CC := gcc
LD := ld
OBJDUMP := objdump
OBJCOPY := objcopy

CONFIGS := -DCONFIG_HEAP_SIZE=4096

CFLAGS := -O0 -ffreestanding -fno-pie -fno-stack-protector -g3 -Wall $(CONFIGS) -mgeneral-regs-only



ODIR = obj
SDIR = src

OBJS = \
	boot.o \
	kernel_main.o \
	serial.o \
	rprintf.o \
	page.o \
	list.o \
	clibfuncs.o \
    	uart.o \
	nalloc.o \
	mmu.o \
	process.o \
	sd.o \
	nfat.o \

OBJ = $(patsubst %,$(ODIR)/%,$(OBJS))

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) -c -g -o $@ $^

$(ODIR)/%.o: $(SDIR)/%.s
	$(CC) $(CFLAGS) -c -g -o $@ $^


all: bin rootfs.img

bin: $(OBJ)
	$(LD) obj/* -Tkernel.ld -o kernel8.img
	cp kernel8.img kernel8.elf
	$(OBJCOPY) -O binary kernel8.img
	size kernel8.elf

clean:
	rm -f obj/*
	rm -f rootfs.img
	rm -f rootfs.img
	rm -f kernel8.img
	rm -f kernel8.elf
	killall screen

debug:
	screen -S qemu -d -m qemu-system-aarch64 -machine raspi3 -kernel kernel8.img -hda rootfs.img -S -s -serial null -serial stdio -monitor none -nographic -k en-us 
	TERM=xterm gdb -x gdb_init_prot_mode.txt

run:
	qemu-system-aarch64 -machine raspi3 -kernel kernel8.img -hda rootfs.img -serial null -serial stdio -monitor none -nographic -k en-us

disassemble:
	$(OBJDUMP) -D kernel8.elf

rootfs.img:
	dd if=/dev/zero of=rootfs.img bs=1M count=16
	mkfs.fat -F16 rootfs.img
	sudo mount rootfs.img /mnt/disk
	sudo mkdir -p /mnt/disk/boot/firmware
	sudo mkdir /mnt/disk/bin
	sudo mkdir /mnt/disk/etc
	sudo cp rootfs/shell/obj/shell /mnt/disk/bin
	sudo umount /mnt/disk

