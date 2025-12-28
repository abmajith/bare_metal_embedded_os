TOOLCHAIN_DIR := ../toolchain/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-elf/bin
CROSS_COMPILE := $(TOOLCHAIN_DIR)/aarch64-none-elf-
CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy

INCLUDE_HEADERS := -I include \
	-I ../toolchain/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-elf/lib/gcc/aarch64-none-elf/13.2.1/include \
	-I ../toolchain/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-elf/aarch64-none-elf/include 

CFLAGS = -Wall -O2 -nostdlib -nostdinc -nostartfiles -ffreestanding $(INCLUDE_HEADERS)
LDFLAGS = -T link.ld

all: kernel8.img

SRC := src/kernel.c src/uart0.c src/timer.c src/interrupt_controller.c src/vector_table.c
OBJ = $(SRC:.c=.o) boot.o

kernel8.img: $(OBJ)
	$(LD) -nostdlib  $(OBJ) -T link.ld  -o kernel8.elf
	$(OBJCOPY) -O binary kernel8.elf kernel8.img

boot.o: boot.S
	$(CC) $(CFLAGS) -c boot.S -o boot.o

src/kernel.o: src/kernel.c 
	$(CC) $(CFLAGS) -c src/kernel.c -o src/kernel.o

src/uart.o: src/uart0.c
	$(CC) $(CFLAGS) -c src/uart0.c -o src/uart0.o

src/vector_table.o: src/vector_table.S
	$(CC) $(CFLAGS) -c src/vector_table.S -o src/vector_table.o

src/interrupt_controller.o: src/interrupt_controller.c
	$(CC) $(CFLAGS) -c src/interrupt_controller.c -o  src/interrupt_controller.o

src/timer.o: src/timer.c
	$(CC) $(CFLAGS) -c src/timer.c -o src/timer.o

clean:
	rm -f src/*.o kernel8.elf kernel8.img boot.o

