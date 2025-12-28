KDIR := /srv/nfs/rpi-root/lib/modules/6.6.10/build

TARGET_DIR := /srv/nfs/rpi-root/home

obj-m += razione_char.o

ARCH := arm64
CROSS_COMPILE := aarch64-rpi3-linux-gnu-

NFS_PATH="/srv/nfs/rpi-root"
KERNEL_VERSION=6.6.10

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules
	cp razione_char.ko $(TARGET_DIR)/

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean
