# X-tool in Environment 
```bash
cd ~/dev/<sbc>/ldd/03_io_ledButton
make
make clean
```

## Check SBC board
```bash
cd home
mount -t debugfs none /sys/kernel/debug
# i used gpio 16 for the buzzer so 
grep "16" /sys/kernel/debug/gpio 
# example
gpio-516 (GPIO_GCLK           )
gpio-528 (GPIO16              ) 
# we need to add this (base address 512) + 16 = 528

insmod ledIO.ko

cat /proc/interrupts | grep razione
113:          0          0          0          0  pinctrl-bcm2835  16 Edge      razione_handler
/home # [ 1288.520645] RazioneIO: Button pressed! Turning ON for 1 second...
[ 1289.532150] RazioneIO: Timer expired - LED and Buzzer turned OFF
[ 1307.661216] RazioneIO: Button pressed! Turning ON for 1 second...
[ 1308.668151] RazioneIO: Timer expired - LED and Buzzer turned OFF

/home # 
/home # 
/home # cat /proc/interrupts | grep razione
113:          8          0          0          0  pinctrl-bcm2835  16 
rmmod ledIO.ko
```

Here we added an explicit interrupt via device driver