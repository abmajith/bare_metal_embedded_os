## Hello UART1 Bare Metal RASPBERRY PI

Here, we have a simple *OS* kernel that prints a hello message on the host terminal via RS232 cable.

The files in this folder are self-explanatory, This folder is a primitive concept that I mostly referred to and used the code from
<a href="https://wiki.osdev.org/Raspberry_Pi_Bare_Bones" class="custom-link">OS DEV Tutorial</a>.

```bash
make
// will create kernel8.img, copy this and firmware files to the bootable sdcard

cp ../rsp_3b_firmware/{bootcode.bin  dw_bootable.sh  fixup.dat  start.elf} /mnt/boot/
cp kernel8.img  /mnt/boot/
```

To make bootable sdcard using terminal fdisk utility
```bash
sudo umount /media/username/FFFFFFFFF
// for me 
sudo umount /media/abdul/B7C6-9BA6
sudo fdisk /dev/mmcblk0 // check how your sd card read in your system, its very important
```



- Type *d* to delete the partition
- Repeat as many times as the number of partitions in your sd card
- Type *o* to change the partition table type to dos
- Type *n* to create new partition  
- Accept the defaults when asking for the partition type and size (here we need only one partition unlike in embedded Linux)
- Type *p* to see existing parition
- Type *t* to change the file system type
	- Type *c* to create a W95 FAT32 (LBA) partition
	- Type *a* to activate
	- Type *w* to write the changes to disk

- In the terminal type
	```bash
	sudo mkfs.vfat /dev/mmcblk0p1 // for you it might be /dev/sdb1

	cross check the created parition as 
	sudo fdisk -l
	```
	
Note: For me, It only prints part of the string, comment about your solution here.
