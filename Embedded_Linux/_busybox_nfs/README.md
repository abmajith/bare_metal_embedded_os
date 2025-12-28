# Setting Up NFS FS using BusyBox

```bash
pwd 
/home/<username>/dev
git clone https://git.busybox.net/busybox busybox_github
cd ~/dev/busybox_github

# include path to the cross compiled tool chain bin folder in the environment
export PATH=$PATH:/home/<user>/dev/<sbc>/x-tools/ARCH-VENDOR-KERNEL-SRC/bin
make ARCH=<arch> CROSS_COMPILE=ARCH-VENDOR-KERNEL-SRC- defconfig
# example
export PATH=$PATH:/home/jvneumann/dev/raz_3b_plus/x-tools/aarch64-rpi3-linux-gnu/bin
make ARCH=arm64 CROSS_COMPILE=aarch64-rpi3-linux-gnu- defconfig
make ARCH=arm64 CROSS_COMPILE=aarch64-rpi3-linux-gnu- menuconfig
```

Navigate to: Settings

- In Settings -> Build Options, check "Build static binary"
- Press Y to select it.
- Optional: Under Networking Utilities, ensure _udhcpc_ is enabled (for your NFS/Network boot).
- Save and Exit.

```bash
make -j$(nproc)
chmod -R 755 /srv/nfs/<sbc>-root
make CONFIG_PREFIX=/srv/nfs/<sbc>-root install

# Manually create the rest of the hierarchy
cd /srv/nfs/<sbc>-root
sudo mkdir -p dev proc sys etc/init.d lib lib/modules lib/modules/<kernel-version>

# Example 
sudo mkdir -p dev proc sys etc/init.d lib lib/modules lib/modules/6.6.10


vim /srv/nfs/<sbc>-root/etc/init.d/rcS
mount -t proc none /proc
mount -t sysfs none /sys
/sbin/mdev -s  # Populates /dev automatically
echo "System Initialized. Ready for Driver Testing."

sudo chmod a+x etc/init.d/rcS
```

## Install kernel Modules in the nfs
Linux kernel module install

```bash
# include path to the cross compiled tool chain bin folder in the environment
export PATH=$PATH:/home/<user>/dev/<sbc>/x-tools/ARCH-VENDOR-KERNEL-SRC/bin
make ARCH=arm64 CROSS_COMPILE=ARCH-VENDOR-KERNEL-SRC- modules -j$(nproc)
make ARCH=arm64 CROSS_COMPILE=ARCH-VENDOR-KERNEL-SRC-  INSTALL_MOD_PATH=/srv/nfs/<sbc>-root modules_install
```


## NFS host file checks
Also make sure  or write like this in the inittab
```bash
sudo vim  /srv/nfs/<sbc>-root/etc/inittab
::sysinit:/etc/init.d/rcS
::askfirst:-/bin/sh
::restart:/sbin/init
::ctrlaltdel:/sbin/reboot
```

and ensure 

```bash
pwd 
ray@ray:/srv/nfs/<sbc>-root$ ls -lh dev/console
crwxr-xr-x 1 root root 5, 1 Dec 27 15:45 dev/console
ray@ray:/srv/nfs/<sbc>-root$ ls -lh dev/null 
crwxr-xr-x 1 root root 1, 3 Dec 27 15:45 dev/null


# if its not present then create them
sudo mknod -m 600 /srv/nfs/rpi-root/dev/console c 5 1
sudo mknod -m 666 /srv/nfs/rpi-root/dev/null c 1 3
```

```bash
sudo chmod -R 755 /srv/nfs/<sbc>-root
```