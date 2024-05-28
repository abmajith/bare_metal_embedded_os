#our downloaded tool chain binary directory

TOOLCHAIN_DIR := /home/abdul/Desktop/bare_metal_embedded_os/rpi_3bplus_BM/toolchain/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-elf/bin
CROSS_COMPILE := $(TOOLCHAIN_DIR)/aarch64-none-elf-
CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy


INCLUDE_HEADERS := -I ../toolchain/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-elf/lib/gcc/aarch64-none-elf/13.2.1/include

CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles

# Source files
SRC = src/main.c src/uart0.c src/mailbox.c
OBJ = $(SRC:.c=.o) start.o



all: clean kernel8.img


# linker job
kernel8.img: $(OBJ)
	$(LD) -nostdlib  $(OBJ) -T linker.ld  -o kernel8.elf
	$(OBJCOPY) -O binary kernel8.elf kernel8.img

# assembler job
start.o: start.S
	$(CC) $(CFLAGS) -c start.S -o start.o

# compiler job
src/main.o: src/main.c src/uart0.h src/mmio.h src/mailbox.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/uart0.o: src/uart0.c src/uart0.h src/mmio.h src/mailbox.h
	$(CC) $(CFLAGS) -c src/uart0.c -o src/uart0.o

src/mailbox.o: src/mailbox.c src/mailbox.h src/mmio.h
	$(CC) $(CFLAGS) -c src/mailbox.c -o src/mailbox.o

clean:
	rm -f *.o src/*.o  *.img *.elf >/dev/null 2>/dev/null || true

.PHONY: all clean
