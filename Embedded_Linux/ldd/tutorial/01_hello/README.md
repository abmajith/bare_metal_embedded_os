# X-tool in Environment 

```bash
export PATH="$PATH:/home/<user>/dev/<sbc>/x-tools/ARCH-vendor-KERNEL-SOURCE/bin"
# example
export PATH="$PATH:/home/ray/dev/razione/x-tools/aarch64-rpi3-linux-gnu/bin"


cd ~/dev/<sbc>/ldd/01_hello

make

make clean
```


## Check SBC board
```bash
cd home
insmod hello.ko
# will see some greeting words, check the code to see what exactly it will print
rmmod hello.ko
```