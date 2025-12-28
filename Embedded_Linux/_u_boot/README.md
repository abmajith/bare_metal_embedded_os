# Booting SBC with u-boot Bootloader

## Clone U-boot

```bash
pwd 
~/dev
git clone https://source.denx.de/u-boot/u-boot.git u-boot
cd u-boot/
export PATH=$PATH:/home/<user>/dev/<sbc>/x-tools/ARCH-VENDOR-KERNEL-SRC/bin
make ARCH=arch CROSS_COMPILE=ARCH-VENDOR-KERNEL-SOURCE- <sbc>_defconfig
make ARCH=arch CROSS_COMPILE=ARCH-VENDOR-KERNEL-SOURCE-  -j$(nproc)

# For raspberry pi 3b Plus board
export PATH=$PATH:/home/jvneumann/dev/raz_3b_plus/x-tools/aarch64-rpi3-linux-gnu/bin
make CROSS_COMPILE=aarch64-rpi3-linux-gnu- rpi_3_b_plus_defconfig
make CROSS_COMPILE=aarch64-rpi3-linux-gnu- -j$(nproc)
```

U-boot way of compiling and producing the boot Image is stable and good documentation available online.
So, experiment with U-boot _menuconfig_ and documentation simpler than any other mechanism involves in Embedded Linux

```bash
make CROSS_COMPILE=aarch64-rpi3-linux-gnu- rpi_3_b_plus_defconfig menuconfig
```
Play with options like Device Drivers Command line interface File systems,

For NFS FS
- File systems  --->
- [*] Network File Systems
- [*] NFS


at the end of make u-boot

```bash
make CROSS_COMPILE=aarch64-rpi3-linux-gnu- -j$(nproc)
```

it produces u-boot.bin, u-boot,
check
```bash
file u-boot
# if compiled for aarch64, closely follow if your desire architecture was not noticed in this output
ELF 64-bit LSB executable, ARM aarch64
```

finally will use u-boot.bin for booting the board, usually placed along with board dtb - device tree blob, an architecture independent compiled code used at boot process to initialize various board components live. 
These files should be placed at the _sdcard_ or usb fat32 partition.


Although it initialize board components, it not necessarily will stay alive in Kernel, its depends on how kernel initialize system and information gathers from u-boot initialization.