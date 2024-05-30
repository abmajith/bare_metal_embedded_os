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
LAN cables, Display modules, Camera modules, etc.

As you see, this Rasberry Pi is a personal computer, 
when we install a Debian-based Linux and connect it with other computer accessories. 


For the sake of completeness, let's list all the accessories ports, and peripherals
<img src="https://github.com/abmajith/bare_metal_embedded_os/blob/main/rpi_3bplus_BM/rsp_3b_mmioPeripherals/raspi3bplus_detailed.jpg" alt="J" width="1200"/>


## Address Space Layout

Ras Pi 3B+ uses a 32-bit address space. The total addressable space is 4 GB (0x00000000 to 0xFFFFFFFF).

- 1GB  RAM of Ras Pi 3B+ resides in a contiguous space in the boundary of 0x00000000 - 0x3EFFFFFF
- Peripheral Registers are within the space of 0x3F000000 - 0x40000000
	-	0x3F000000 - 0x3F003FFF: Interrupt Controller
	-	0x3F200000 - 0x3F2000B3: GPIO
	-	0x3F201000 - 0x3F201FFF: UART0
	-	0x3F202000 - 0x3F202FFF: UART1
	-	0x3F00B880 - 0x3F00B8FF: Mailbox Interface
	-	0x3F003000 - 0x3F00300F: System Timer
	-	0x3F101000 - 0x3F101FFF: I2C and SPI Controllers


Let's define the base address of important peripherals in a header file as 
```c
#ifndef MMIO_H
#define MMIO_H
// Base address for MMIO
#define MMIO_BASE       0x3F000000

// Peripheral base addresses
#define TIMER_BASE      (MMIO_BASE + 0x00003000)  // System Timer
#define INTERRUPT_BASE  (MMIO_BASE + 0x0000B000)  // Interrupt Controller
#define MAILBOX_BASE    (MMIO_BASE + 0x0000B880)  // Mailbox Interface
#define POWER_MNG_BASE  (MMIO_BASE + 0x00100000)  // Power Management base
#define RNG_BASE        (MMIO_BASE + 0x00104000)  // Random Number Generator Base
#define GPIO_BASE       (MMIO_BASE + 0x00200000)  // GPIO Base
#define UART0_BASE      (MMIO_BASE + 0x00201000)  // UART0 (serial port, PL011)
#define UART1_BASE      (MMIO_BASE + 0x00215000)  // UART1 (serial port, AUX mini UART)


#define I2C0_BASE       (MMIO_BASE + 0x00205000)  // I2C0 Controller
#define I2C1_BASE       (MMIO_BASE + 0x00804000)  // I2C1 Controller
#define I2C2_BASE       (MMIO_BASE + 0x00805000)  // I2C2 Controller

#define SPI0_BASE       (MMIO_BASE + 0x00204000)  // SPI0 Controller
#define SPI1_BASE       (MMIO_BASE + 0x00215080)  // SPI1 Controller
#define SPI2_BASE       (MMIO_BASE + 0x002150C0)  // SPI2 Controller

#endif // MMIO_H
```
