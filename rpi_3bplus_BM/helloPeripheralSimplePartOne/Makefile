#our downloaded tool chain binary directory

TOOLCHAIN_DIR := ../toolchain/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-elf/bin
CROSS_COMPILE := $(TOOLCHAIN_DIR)/aarch64-none-elf-
CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy


INCLUDE_HEADERS := -I include -I ../toolchain/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-elf/lib/gcc/aarch64-none-elf/13.2.1/include

CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles $(INCLUDE_HEADERS)

# Source files
SRC = src/main.c src/uart0.c src/mailbox.c src/rng.c src/power.c src/timer.c src/privilege.c
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
src/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/uart0.o: src/uart0.c
	$(CC) $(CFLAGS) -c src/uart0.c -o src/uart0.o

src/mailbox.o: src/mailbox.c
	$(CC) $(CFLAGS) -c src/mailbox.c -o src/mailbox.o

src/rng.o: src/rng.c
	$(CC) $(CFLAGS) -c src/rng.c -o src/rng.o

src/power.o: src/power.c
	$(CC) $(CFLAGS) -c src/power.c -o src/power.o

src/timer.o: src/timer.c
	$(CC) $(CFLAGS) -c src/timer.c -o src/timer.o

src/privilege.o: src/privilege.c
	$(CC) $(CFLAGS) -c src/privilege.c -o src/privilege.o


clean:
	rm -f *.o src/*.o  *.img *.elf >/dev/null 2>/dev/null || true

.PHONY: all clean
