# Embedded Linux using Linaro Toolchain

If either an Embedded Linux or a bare metal OS, we need a toolchain, that
compile and produce the binary code of the application, OS.

To not complicate it much but still have some details, we will use the Linaro GCC toolchain
for arm architecture.

Before obtaining the toolchain, if you feel to update, and upgrade your system (I will refer hereafter to HOST),
update it and then follow the commands.

It is recommended to perform all the following actions under */home/your_project_path*

## Folder Structure

- `bootloader/`: Contains U-Boot sources and build files.
- `kernel/`: Contains Linux kernel sources and build files.
- `rootfs/`: Contains Buildroot sources and configuration.
- `toolchain/`: Contains the cross-compilation toolchain.
- `sdcard/`: Temporary directory for preparing SD card contents.
- `build/`: Intermediate build files.
- `Makefile`: Top-level Makefile for automated building.


## Downloading a pre-build toolchain from Linaro and extract toolchain
```bash
mkdir -p toolchain
cd toolchain
wget https://releases.linaro.org/components/toolchain/binaries/latest-7/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz
tar -xf gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz
export PATH=/home/your_project_path/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin:$PATH
cd ../
```

## Clone and Build U-Boot
```bash
mkdir -p bootloader
cd bootloader
git clone https://source.denx.de/u-boot/u-boot.git
cd u-boot/
make ARCH=arm CROSS_COMPILE=aarch64-linux-gnu- rpi_3_b_plus_defconfig
make ARCH=arm CROSS_COMPILE=aarch64-linux-gnu- -j$(nproc)
cd ../../
```
I recommend replacing *-j$(nproc)* by *-j x/2* if you're using a laptop with x cores, If you're using a desktop it is okay to use
the full cores i.e. *-j$(nproc)*. I tested it, and for a laptop, it's better to use a lesser number of processor cores.

## Clone and Build Linux kernel
```bash
mkdir -p kernel
cd kernel
git clone git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git
cd linux
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- defconfig
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -j$(nproc)
cd ../../
```

## Download and Extract buildroot
```bash
mkdir -p rootfs
cd rootfs
wget https://buildroot.org/downloads/buildroot-2024.02.2.tar.gz
tar -xf buildroot-2024.02.2.tar.gz
cd buildroot-2024.02.2
make raspberrypi3_64_defconfig
```
Note: *Buildroot* itself can produce a toolchain, bootloader, Linux kernel, 
and root file system, the reason why we perform all the details ourselves is to get familiarized with embedded Linux.  Because once we move from here to
work on custom boards and software, we will work to fine-tune (in the sense that choosing the requirements for the project/application) the toolchain and kernel.

It's our time to set up the recipe to build our root file system.

```bash
make menuconfig
```
Appears a graphic menu bar to choose various recipes for the root file systemMake sure that the Target option is as  <br />
*Target Architecture* (AArch64 (little endian)) ---> <br />
*Target Architecture* Variant (cortex-A53) --->  <br />
Our *Rasberry pi-3b+* is an arm 64-bit little endian format broadcom processor.  <br />
In the *Toolchain* option, choose Toolchain type as External toolchain,  <br />
In the *Toolchain external options*: Toolchain as Custom toolchain,  <br />
Toolchain origin as Pre-installed toolchain, because we already downloaded Linaro toolchain  <br />
and set Toolchain path as (*/home/your_project_path//toolchain/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu*)  <br />
The Toolchain prefix is set as *aarch64-linux-gnu* <br />

Our downloaded Linaro toolchain has some specific properties, <br />
choose External toolchain gcc version as *7.x*,  <br />
External toolchain kernel headers series as *4.10.x*,  <br />
External toolchain C library as *glibc*  <br />
Make sure you enable *Toolchain SSP, RPC, Fortron, OpemMb, and C++*. <br />
These all are present in the Linaro toolchain. And also disable the rest. 
There is nothing to change in *Build Options*,

In *system configuration* options, make sure the Custom scripts to run after creating filesystem images are empty, 
i.e. remove the string if it's there. 


In Kernel disable the Linux Kernel, since we already build a Linux kernel.  <br />
And finally ensure that in File System images *ext2/3/4* root filesystem, *ext4* is enabled.  <br />
Once the chosen parameters are, the selected configuration is stored in the same folder with a hidden file name *.config*, now run

```bash
make
```
I have created a Makefile, changing only the *toolchain_dir*, if you had a similar folder structure, it should work fine!

If you opt to use make file, before using make file, do the following
```bash
mkdir -p toolchain
mkdir -p kernel
mkdir -p rootfs
mkdir -p bootloader
cd toolchain
wget https://releases.linaro.org/components/toolchain/binaries/latest-7/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz
tar -xf gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz
cd ../bootloader
git clone https://source.denx.de/u-boot/u-boot.git
cd ../kernel
git clone git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git
cd ../rootfs
wget https://buildroot.org/downloads/buildroot-2024.02.2.tar.gz
tar -xf buildroot-2024.02.2.tar.gz
cd ../
// now run 
make uboot
make kernel
make rootfs_init
make rootfs // here you should choose the buildroot configuration as dicussed earlier
make sdcard // finally collect Image, rootfs.ext4, kernel8.img, rasperrypi device tree blob (dtb) the required files and put in one folder
```


## Boot Process of Raspberry Pi:
- Initial Bootloader (in ROM)
  - It initialize the *Ras Pi* GPU and SDRAM, it is hardcoded in *Ras Pi*'s GPU.
- Secondary Bootloader (in sdcard)
  - It looks for FAT32 partiion on the sd card, and loads *bootcode.bin* file from this FAT32 Partition
- Loading Firmware (in sdcard)
  - *bootcode.bin* loads *start.elf* file, which is the main firmware for the GPU, it further initialize the system and 
    loads *config.txt* file
- Loading Kernel or binary executable file as Image
  - The firmware then loads the kernel image, typically named as *kernel8.img* for *ARM64* from the same FAT32 partition.


We build our own *kernel8.img*, here, but we use all the rest of the boot loading process from the *Ras Pi* firmware files.

These files are collected from the appropriate firmware. If you use the terminal command instead of make file,
then follow these steps

```bash 
mkdir - p sdcard/boot
wget https://github.com/raspberrypi/firmware/archive/master.zip
unzip -j master.zip firmware-master/boot/{bootcode.bin,fixup.dat,start.elf} -d sdcard/boot/
cp bootloader/u-boot/u-boot.bin sdcard/boot/kernel8.img
cp kernel/linux/arch/arm64/boot/Image  sdcard/boot/
cp kernel/linux/arch/arm64/boot/dts/broadcom/bcm2837-rpi-3-b-plus.dtb  sdcard/boot/

cp rootfs/buildroot/output/images/rootfs.ext4 sdcard/
// needed additional config file to inform uart enable 
cat config.txt 
enable_uart=1
kernel=kernel8.img
arm_64bit=1
core_freq=250

cp config.txt sdcard/boot/
```

Now, collect all the compiled file and place them on the real sdcard

```bash
// create a sd card layout like this
cat sdlayout
label: dos
label-id: 0x85baa6c1
device: /dev/sdb
unit: sectors

/dev/sdb1 : start=        2048, size=      524288, type=c, bootable
/dev/sdb2 : start=      526336, size=    29726687, type=83

//The SD card might appear differently in your system, 
//it could be /dev/sda or /dev/mmcblk0, change accordingly in the file and also in the following command

sudo sfdisk /dev/sdb < sdlayout
sudo mkfs.vfat /dev/sdb1
sudo mkfs.ext4 /dev/sdb2
sudo dd if=sdcard/rootfs.ext4 of=/dev/sdb2 bs=4M

// say your /dev/sdb1 is mounted as /media/username/abcd-1234/, check this by lsblk, 
// if its not mounted then use sudo mount /dev/sdb1 /media/../..

cp sdcard/boot/*  /media/username/abcd-1234/ 
sudo sync
sudo eject /dev/sdb
```


## Booting Raspberry board via RS232 Cable

Now inser the sdcard on the rasberry pi board, connect RS232 USB cable, USB on the Laptop side, 
and uart wires on the GPIO pins (6 - Common Gnd Pin, 8- *UART0_RXT* for the board, *TXT* for the computer, 10- *UART0_TXT* for the board, *RXD* for the computer).

Dont yet the board power up, first connect the computer to this USB cable.

```bash
sudo apt-get install screen
// insert your rs232 usb cable converter
ls -l /dev/ttyUSB0 // keep simple, have only one rs232 USB connected

id

// if it shows dialout then move on o.w, follow this procedure
sudo usermod -a -G dialout username
newgrp dialout

// now check
id
// for me 
uid=1000(abdul) gid=1000(abdul) groups=1000(abdul),4(adm),20(dialout),27(sudo),121(lpadmin)

ls -l /dev/ttyUSB0
crw-rw---- 1 root dialout 188, 0 May 23 03:03 /dev/ttyUSB0

sudo screen /dev/ttyUSB0 115200


// after some time, the boot command appears, do the following
U-Boot> setenv kernel_addr_r 0x80000
U-Boot> setenv fdt_addr_r 0x3000000 

//Make sure the above fdt_addr_r is large enough to hold the kernel image file within the address range 0x80000 to 0x3000000-1
// follow the message from U-Boot carefully,
// I accidentally put it smaller and spent some hours to understand it

U-Boot> setenv bootcmd 'fatload mmc 0:1 ${kernel_addr_r} Image; fatload mmc 0:1 ${fdt_addr_r} bcm2837-rpi-3-b-plus.dtb; booti ${kernel_addr_r} - ${fdt_addr_r}'
U-Boot> saveenv

// it should print something back to you OK after saving the provided parameters

U-Boot> boot
// after some time
// you can use the rasberrypi on terminal 
# ls -a
...

```
