## Bare Metal RASPBERRY PI Toolchain Preparation


As we know, we need a cross-toolchain to build a binary that can run on the SBC board,
in our case *Ras pi 3b plus*.

Building a cross-toolchain for our purpose from scratch is not that complicated.
There are many tutorials available. Here we use the toolchain provided by *ARM software support*.
We chose a bare metal compiler instead of full-fledged support as we saw in the folder *rpi_3bplu_EL*.



```bash
mkdir toolchain
cat download_toolchain.sh
wget https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-x86_64-aarch64-none-elf.tar.xz
tar -xf arm-gnu-toolchain-13.2.rel1-x86_64-aarch64-none-elf.tar.xz
./download_toolchain.sh
// this is after doing chmod
```

We have a bare metal toolchain in the folder *toolchain*.

Next, we collect the bare minimum *ras pi* firmware files

```bash
mkdir rsp_3b_firmware
cat dw_bootable.sh
wget https://github.com/raspberrypi/firmware/raw/master/boot/bootcode.bin
wget https://github.com/raspberrypi/firmware/raw/master/boot/fixup.dat
wget https://github.com/raspberrypi/firmware/raw/master/boot/start.elf
./dw_bootable.sh
```

By doing this way, we consistently make the constant toolchain and firmware throughout our project development.


We are ready to create subfolders and develop typical embedded applications related to protocols and OS modules.
Here we have a few test cases, that will progress day by day!

To start our embedded OS development process, we will first look into two different 
(*ras pi 3b+, Broadcom 2837 hardware* provides this mechanism using *UART0,UART1*)
ways to establish a primitive serial console communication between the host (our personal computer) 
and target (our Raspberry Pi board).

We have two demos one for *UART1*: <a href="https://github.com/abmajith/bare_metal_embedded_os/tree/main/rpi_3bplus_BM/helloUART" class="custom-link">Hello UART1</a>
and another for *UART0*: <a href="https://github.com/abmajith/bare_metal_embedded_os/tree/main/rpi_3bplus_BM/helloMAILBOXUART" class="custom-link">Hello UART0 Mailbox</a>
in the current folder. 

Once we have successfully seen a serial console message in the host computer,
we will head to list the memory-mapped I/O (MMIO) addresses for the 
<a href="https://github.com/abmajith/bare_metal_embedded_os/tree/main/rpi_3bplus_BM/rsp_3b_mmioPeripherals" class="custom-link">Raspberry Pi 3 B+ and its peripherals</a>

