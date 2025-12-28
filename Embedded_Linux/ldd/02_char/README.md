# X-tool in Environment 
```bash
cd ~/dev/<sbc>/ldd/02_char
make
make clean
```

## Check SBC board
```bash
cd home
insmod razione_char.ko
ls -l /dev/razione_char
echo "Simple Conceptual Character Device Driver" > /dev/razione_char

cat /dev/razione_char
rmmod razione_char
```

Now you see the benefit of NFS over the sdcard or usb booted Kernel image (type of linux not matter), but NFS help all compilation and addition the module to the file system, a perfect way to do the Embedded Application development/test on the board. 