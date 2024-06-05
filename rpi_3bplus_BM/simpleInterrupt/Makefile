TOOLCHAIN_DIR := ../toolchain/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-elf/bin
CROSS_COMPILE := $(TOOLCHAIN_DIR)/aarch64-none-elf-
CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy

INCLUDE_HEADERS := -I include

CFLAGS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding $(INCLUDE_HEADERS)
LDFLAGS = -T link.ld

all: kernel8.img

kernel8.img: boot.o src/kernel.o src/uart0.o src/vector_table.o
	$(LD) $(LDFLAGS) boot.o src/kernel.o src/uart0.o src/vector_table.o -o kernel8.elf
	$(OBJCOPY) -O binary kernel8.elf kernel8.img

boot.o: boot.S
	$(CC) $(CFLAGS) -c boot.S -o boot.o

src/kernel.o: src/kernel.c include/uart0.h
	$(CC) $(CFLAGS) -c src/kernel.c -o src/kernel.o

src/uart.o: src/uart0.c include/uart0.h
	$(CC) $(CFLAGS) -c src/uart0.c -o src/uart0.o

src/vector_table.o: src/vector_table.S
	$(CC) $(CFLAGS) -c src/vector_table.S -o src/vector_table.o

clean:
	rm -f src/*.o kernel8.elf kernel8.img boot.o

