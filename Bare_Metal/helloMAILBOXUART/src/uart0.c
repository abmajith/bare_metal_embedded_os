
/*
* Defining various uart0 functions here
*/


#include "mmio.h"
#include "mailbox.h"



void uart0_init()
{
	// Disable UART0
	*UART0_CR = 0x00000000;
	
	// set up our mail box message buffer
	mbox_buf[0] = 9*4;                  // size of our message buffer
	mbox_buf[1] = MAILBOX_M_REQUEST;    // request message
	mbox_buf[2] = MAILBOX_M_SETCLKRATE; // request to change clock rate
	mbox_buf[3] = 12;                   // Value buffer size (bytes)
	mbox_buf[4] = 8;                    // Request size (bytes)
	mbox_buf[5] = 2;                    // Clock id for UART
	mbox_buf[6] = 12000000;              // I choose 12MHz
	mbox_buf[7] = 0;										// End tag
	mbox_buf[8] = MAILBOX_M_LAST;       // pad the message with proper boundary close

	mailbox_call(MAILBOX_M_CH_PROP); // hope it get response from firmware videocore gpu for changing the clock freq


	// Configure GPIO pins for UART0
	unsigned int ra = *GPFSEL1;
	ra &= ~((7 << 12) | (7 << 15));  // Clear GPIO14 and GPIO15
	ra |= (4 << 12) | (4 << 15);     // Set ALT0 for GPIO14 and GPIO15
	*GPFSEL1 = ra;

	// Disable pull-up/down for GPIO14 and GPIO15
	*GPPUD = 0;
	for (ra = 0; ra < 150; ra++) {asm volatile("nop");}
	*GPPUDCLK0 = (1 << 14) | (1 << 15);
	for (ra = 0; ra < 150; ra++) {asm volatile("nop");}
	*GPPUDCLK0 = 0;

	// Clearn pending interrupts
	*UART0_ICR = 0x7FF;

	// Set integer & fractional part of baud rate
	
	/*
		12 X 10**6 / (16 * 115200) ~= 6
		frac(_) = .5104
		(.5104 * 64 + 0.5) = 33.16666
	*/
	*UART0_IBRD = 6;    // Integer part of baud rate divisor
	*UART0_FBRD = 33;   // Fractional part of baud rate divisor
	
	// Enable FIFO & 8-bit data transmission (1 stop bit, no parity)
	*UART0_LCRH = (3 << 5) | (1 << 4);

	// Mask all interrupts
	*UART0_IMSC = (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) |
								(1 << 8) | (1 << 9) | (1 << 10);
	// Enable UART0, receive & transmit part of UART
	*UART0_CR = (1 << 9) | (1 << 8) | (1 << 0);
}


void uart0_putc(unsigned int c)
{
	do{asm volatile("nop");}while(*UART0_FR &  (1 << 5) );
	*UART0_DR = c;
}


unsigned char uart0_getc()
{
	do{asm volatile("nop");}while(*UART0_FR & (1 << 4) );
	return (unsigned char)(*UART0_DR);
}


void uart0_puts(char *str)
{	
	while (*str)
	{
		if (*str == '\n')
		{ 
			uart0_putc('\r');
		}
		uart0_putc(*str++);
	}
}
