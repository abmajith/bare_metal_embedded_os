# Embedded Linux using Linaro Toolchain

If either a Embedded Linux or a bare metal OS, we need tool chain, that 
compile and produce the binary code of application, OS.

In order to not complicate much but still some details, we will use the linaro gcc toolchain 
for arm architecture.

Before optaining the toolchain, if you feel to update, and upgrade your system (I will refer here after HOST), 
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


Downloading a pre build tool chain from linaro and extract toolchain:
```bash
mkdir -p toolchain
cd toolchain
wget https://releases.linaro.org/components/toolchain/binaries/latest-7/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz
tar -xf gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz
export PATH=/home/your_project_path/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin:$PATH
cd ../
```

Clone and Build U-Boot for generating bootloader
```bash
mkdir -p bootloader
cd bootloader
git clone https://source.denx.de/u-boot/u-boot.git
cd u-boot/
make ARCH=arm CROSS_COMPILE=aarch64-linux-gnu- rpi_3_b_plus_defconfig
make ARCH=arm CROSS_COMPILE=aarch64-linux-gnu- -j$(nproc)
cd ../../
```
I recommend to replace *-j$(nproc)* by *-j x/2* if you using a laptop with x cores, If you using a desktop it is okay to use 
the full cores i.e *-j$(nproc)*. I personally tested it, for laptop its better to use lesser number of processor cores.


Clone and Build Linux kernel
```bash
mkdir -p kernel
cd kernel
git clone git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git
cd linux
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- defconfig
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -j$(nproc)
cd ../../
```

Download and Extract buildroot
```bash
mkdir -p rootfs
cd rootfs
wget https://buildroot.org/downloads/buildroot-2024.02.2.tar.gz
tar -xf buildroot-2024.02.2.tar.gz
cd buildroot-2024.02.2
make raspberrypi3_64_defconfig
```
Note: buildroot itself able to produce toolchain, bootloader, linux kernel and rootfile system, the reason why we doing this way
is first introducing ourself what are the components involved in building embedded linux. Because, once we move from here to 
work on custom board and software, we will work to fine tune the toolchain, and kernel.


After executing the last steps, now its our time to set up the receipe on how to build our rootfs!.

```bash
make menuconfig
```
It appreas a graphica menu bar to choose various receipe for the root file system

Make sure that the *Target option* as  <br />
Target Architecture (AArch64 (little endian))  --->  <br />
Target Architecture Variant (cortex-A53)  --->       <br />

Our *rasberry pi-3b+* is arm 64 bit little endian format processor.


In *Toolchain* option, choose Toolchain type as External toolchain, <br />
toolchain external options: Toolchain as Custom toolchain, <br />
Toolchain origin as Pre-installed toolchain, because we already downloaded linaro toolchain <br />
and set Toolchain path as *(/home/your_project_path//toolchain/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu)* <br />
The Toolchain prefix set as *aarch64-linux-gnu* <br />
Our downloaded linaro tool chain has some specific property,  <br />
choose  External toolchain gcc version as *7.x*,   <br />
External toolchain kernel headers series as  *4.10.x*,  <br /> 
External toolchain C library as *glibc* <br />
And make sure you enable Toolchain *SSP,RPC, Fortron, OpemMb, and C++* These all are present in the linaro toolchain. And also enable all the rest.


There is nothing to change in *Build Options*, 

In system configuration options, 
make sure *Custom scripts to run after creating filesystem images* is empty, i.e remove string if its there. <br />

In Kernel disable the Linux Kernel, since we already build a Linux kernel.

And finally ensure that in *File System images* ext2/3/4 root filesystem, ext4 is enabled.

Once saved the choosen prameter, the selected configuration stored in the same folder hidden file name *.config*

now run 
```bash
make
```
I have created a Makefile, there change only the *toolchain_dir*, if you had similar folder structure, it should work fine!

If you opt to use make file, before using make file, do the followings
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

The Raspberry Pi starts execution in the GPU core with a first stage boot loader in the boot ROM. 
It then passes control over to a file named *bootcode.bin* located in a FAT partition.
This represents a second stage boot loader. 
It’s main purpose is setting up the *SDRAM* and jumping to a third-stage bootloader named *start.elf*
(along with its counterpart ‘fixup.dat’) that is also located on the FAT partition. 
This *start.elf* has the ability to start the CPU core and boot our kernel or bootloader image.

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


// create a sd card layout like this
cat sdlayout
label: dos
label-id: 0x85baa6c1
device: /dev/sdb
unit: sectors

/dev/sdb1 : start=        2048, size=      524288, type=c, bootable
/dev/sdb2 : start=      526336, size=    29726687, type=83
// the sd card might appear differently in your system, it could be /dev/sda or /dev/mmcblk0, change accordingly in file also in the command
sudo sfdisk /dev/sdb < sdlayout
sudo mkfs.vfat /dev/sdb1
sudo mkfs.ext4 /dev/sdb2
sudo dd if=sdcard/rootfs.ext4 of=/dev/sdb2 bs=4M
// say your /dev/sdb1 is mounted as /media/username/abcd-1234/, check this by lsblk, if its not mounted use sudo mount /dev/sdb1 /media/../..

cp sdcard/boot/*  /media/username/abcd-1234/ 
sudo sync
sudo eject /dev/sdb
```


Note, what we did so far is we prepared the linux kernel with root file system partition on the sdcard, insert the sdcard, connect first RS232 connected,
 and then open your screen
```bash
sudo apt-get install screen
// insert your rs232 usb cable converted
type ls -l /dev/ttyUSB0 // keep simple, have only one rs232 UBS connected
id
// if it shows dialout then dont worry, otherwise
sudo usermod -a -G dialout username
newgrp dialout
// now check, if it shows 
id
// for me 
uid=1000(abdul) gid=1000(abdul) groups=1000(abdul),4(adm),20(dialout),27(sudo),121(lpadmin)
ls -l /dev/ttyUSB0
crw-rw---- 1 root dialout 188, 0 May 23 03:03 /dev/ttyUSB0

sudo screen /dev/ttyUSB0 115200



// after some time, the boot command appears, do the following
U-Boot> setenv kernel_addr_r 0x80000
U-Boot> setenv fdt_addr_r 0x3000000 // make sure this one is larger so that kernel can hold on inbetween 0x80000 to 0x3000000-1
U-Boot> setenv bootcmd 'fatload mmc 0:1 ${kernel_addr_r} Image; fatload mmc 0:1 ${fdt_addr_r} bcm2837-rpi-3-b-plus.dtb; booti ${kernel_addr_r} - ${fdt_addr_r}'
U-Boot> saveenv
// it should print something back to you OK, ow there is some problem
U-Boot> boot
// after some time
// you can use the rasberrypi purely on the terminal 
# ls -a
...

```
