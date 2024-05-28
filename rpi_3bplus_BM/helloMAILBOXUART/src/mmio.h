/********************************************
* GPIO MAP, This definition is based on hardware in the Broadcom SoC used by Ras Pi
* For Configuring GPIO pins, allowing to set them as input, output or assign them to a peripheral function
* like UART, SPI, etc,.
*********************************************/

#ifndef MMIO_H
#define MMIO_H

#define MMIO_BASE       0x3F000000


#define GPFSEL0         ((volatile unsigned int*)(MMIO_BASE+0x00200000))
#define GPFSEL1         ((volatile unsigned int*)(MMIO_BASE+0x00200004))
#define GPFSEL2         ((volatile unsigned int*)(MMIO_BASE+0x00200008))
#define GPFSEL3         ((volatile unsigned int*)(MMIO_BASE+0x0020000C))
#define GPFSEL4         ((volatile unsigned int*)(MMIO_BASE+0x00200010))
#define GPFSEL5         ((volatile unsigned int*)(MMIO_BASE+0x00200014))

#define GPSET0          ((volatile unsigned int*)(MMIO_BASE+0x0020001C))
#define GPSET1          ((volatile unsigned int*)(MMIO_BASE+0x00200020))
#define GPCLR0          ((volatile unsigned int*)(MMIO_BASE+0x00200028))

#define GPPUD           ((volatile unsigned int*)(MMIO_BASE+0x00200094))
#define GPPUDCLK0       ((volatile unsigned int*)(MMIO_BASE+0x00200098))
#define GPPUDCLK1       ((volatile unsigned int*)(MMIO_BASE+0x0020009C))



//uart0 serial communication (PLO11) full-featured uart
#define UART0_BASE      (MMIO_BASE + 0x201000) // uart0 base register
#define UART0_DR        ((volatile unsigned int *)(UART0_BASE + 0x00))  // data register
#define UART0_FR        ((volatile unsigned int *)(UART0_BASE + 0x18))  // flag register
#define UART0_IBRD      ((volatile unsigned int *)(UART0_BASE + 0x24))	// integer baud rate div
#define UART0_FBRD      ((volatile unsigned int *)(UART0_BASE + 0x28))	// fractional baud rate div
#define UART0_LCRH      ((volatile unsigned int *)(UART0_BASE + 0x2C))	// line control register
#define UART0_CR        ((volatile unsigned int *)(UART0_BASE + 0x30))	// control register
#define UART0_IMSC      ((volatile unsigned int *)(UART0_BASE + 0x38))	// interrupt mask set/clear register
#define UART0_ICR				((volatile unsigned int *)(UART0_BASE + 0x44))  // interrupt control register


// mail box set up
/*
* The mailbox interface on the Raspberry Pi is used for communication 
* between the ARM CPU and the VideoCore GPU. It allows the ARM to request 
* various operations from the GPU, such as configuring the framebuffer, 
* setting clock rates, and other system settings.
*/

#define VIDEO_CORE_GPU_BASE   (MMIO_BASE + 0x0000B880)
#define MAILBOX_READ 					((volatile unsigned int *)(VIDEO_CORE_GPU_BASE + 0x0))
#define MAILBOX_POLL					((volatile unsigned int *)(VIDEO_CORE_GPU_BASE + 0x10))
#define MAILBOX_SENDER				((volatile unsigned int *)(VIDEO_CORE_GPU_BASE + 0x14))
#define MAILBOX_STATUS				((volatile unsigned int *)(VIDEO_CORE_GPU_BASE + 0x18))
#define MAILBOX_CONFIG				((volatile unsigned int *)(VIDEO_CORE_GPU_BASE + 0x1C))
#define MAILBOX_WRITE					((volatile unsigned int *)(VIDEO_CORE_GPU_BASE + 0x20))

// some mail box message type (return message)
#define MAILBOX_RESPONSE   0x80000000
#define MAILBOX_FULL       0x80000000
#define MAILBOX_EMPTY      0x40000000

// some mail box message request type (sending message)

#define MAILBOX_M_CH_PROP			8
#define MAILBOX_M_GETSERIAL		0x10004
#define MAILBOX_M_SETCLKRATE	0x38002
#define MAILBOX_M_LAST				0
#define MAILBOX_M_REQUEST			0

#endif // MMIO_H

