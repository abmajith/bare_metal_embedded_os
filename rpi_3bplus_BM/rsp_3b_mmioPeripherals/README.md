## Raspberry Pi 3 B+ MMIO and Peripherals


A picture is worth 1000 words!


<img src="https://github.com/abmajith/bare_metal_embedded_os/blob/main/rpi_3bplus_BM/rsp_3b_mmioPeripherals/raspi3b_arch.jpg" alt="J" width="1200"/>

- CPU Core: The central processing unit that executes instructions.
- System Timer: Provides timing and delay functionalities.
- GPIO (General Purpose Input/Output): Used for controlling pins for input and output operations.
- UART0: The primary UART for serial communication.
- UART1: The secondary UART for serial communication.
- Mailbox: Used for communication between ARM CPU and VideoCore GPU
  (Yes our Ras Pi 3B has a GPU! that shares the same RAM for computation!),
  including framebuffer setup.
- Random Number Generator: Provides hardware-generated random numbers.
  (Yes, PRG (Pseudo Random Generator) works with the help of hardware support!)
- Interrupt Controller: Manages interrupt requests.
- Power Management: Handles power-related functions, including watchdog timers.
- FrameBuffer: Manages the display output via the GPU. Interfaced through the mailbox.
- I2C (Inter-Integrated Circuit): For communicating with other I2C-compatible devices.
- SPI (Serial Peripheral Interface): Used for SPI communication with peripherals.


So far we have listed the basic peripherals, 
not mentioned yet USB ports, HDMI ports, audio ports, 
LAN cables, Display modules, and Camera modules.

As you see, this Rasberry Pi is a personal computer, 
when we install a Debian-based Linux and connect it with other computer accessories. 

What are we doing here? we are using a widely available 
and cheap processor board to get hands-on experience with embedded OS 
and the development process.
