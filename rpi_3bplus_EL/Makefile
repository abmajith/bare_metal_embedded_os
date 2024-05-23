# Top-level Makefile for building the entire embedded Linux system

TOOLCHAIN_DIR := /home/username/rpi_3bplusEL/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu
CROSS_COMPILE := $(TOOLCHAIN_DIR)/bin/aarch64-linux-gnu-
BUILD_DIR := build
UBOOT_DIR := bootloader/u-boot
KERNEL_DIR := kernel/linux
BUILDROOT_DIR := rootfs/buildroot
SDCARD_DIR := sdcard

all: toolchain uboot kernel rootfs extract


toolchain:
	@echo "Toolchain already extracted"

uboot:
	$(MAKE) -C $(UBOOT_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) rpi_3_b_plus_defconfig
	$(MAKE) -C $(UBOOT_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) -j$(nproc)

kernel:
	$(MAKE) -C $(KERNEL_DIR) ARCH=arm64 CROSS_COMPILE=$(CROSS_COMPILE)  defconfig
	$(MAKE) -C $(KERNEL_DIR) ARCH=arm64 CROSS_COMPILE=$(CROSS_COMPILE) -j 4


rootfs_init: 
	$(MAKE) -C $(BUILDROOT_DIR) raspberrypi3_64_defconfig

rootfs: rootfs_init
	$(MAKE) -C $(BUILDROOT_DIR) menuconfig
	$(MAKE) -C $(BUILDROOT_DIR)

sdcard: 
	mkdir -p $(SDCARD_DIR)/boot
	cp $(UBOOT_DIR)/u-boot.bin $(SDCARD_DIR)/boot/kernel8.img
	cp $(KERNEL_DIR)/arch/arm64/boot/Image $(SDCARD_DIR)/boot/
	cp $(KERNEL_DIR)/arch/arm64/boot/dts/broadcom/bcm2837-rpi-3-b-plus.dtb $(SDCARD_DIR)/boot/
	wget https://github.com/raspberrypi/firmware/archive/master.zip -O firmware.zip
	unzip -j firmware.zip firmware-master/boot/{bootcode.bin,fixup.dat,start.elf} -d $(SDCARD_DIR)/boot/
	cp config.txt $(SDCARD_DIR)/boot/
	cp $(BUILDROOT_DIR)/output/images/rootfs.ext4 $(SDCARD_DIR)

clean:
	$(MAKE) -C $(UBOOT_DIR) clean
	$(MAKE) -C $(KERNEL_DIR) clean
	$(MAKE) -C $(BUILDROOT_DIR) clean

.PHONY: all toolchain uboot kernel rootfs sdcard clean

