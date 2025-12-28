/*
* Base mmio address and relative offset address for the 
* UART0
* UART1
* MailBOX
* GPIO
* TIMER
* Random Number Generator RNG
* Interrupt controller
* Power Management
*/


#ifndef MMIO_H
#define MMIO_H

// base address for memory-mapped I/O's
#define MMIO_BASE       0x3F000000



#define SYS_TIMER_BASE  (MMIO_BASE + 0x00003000)  // System Timer
#define GIC_BASE        (MMIO_BASE + 0x0000B000)  // Generic Interrupt Controller
#define MAILBOX_BASE    (MMIO_BASE + 0x0000B880)  // Mailbox Interface


#define PM_BASE         (MMIO_BASE + 0x00100000)  // Power Management base
#define RNG_BASE        (MMIO_BASE + 0x00104000)  // Random Number Generator Base

#define GPIO_BASE       (MMIO_BASE + 0x00200000)  // GPIO Base


#define UART0_BASE      (MMIO_BASE + 0x00201000)  // UART0 (serial port, PL011)

#endif // MMIO_H

