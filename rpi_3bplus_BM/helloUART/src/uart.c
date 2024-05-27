
/*
* Defining various uart functions here
*/


#include "mmio.h"

void uart_init()
{
	register unsigned int r;

	/* initialize UART */
	*AUX_ENABLE |=1;       // Enable mini UART (1)
	*AUX_MU_CNTL = 0;			 // Disable transmitter and receiver
	*AUX_MU_LCR = 3;       // 8 bit mode
	*AUX_MU_MCR = 0;       // RTS line
	*AUX_MU_IER = 0;       // Disable interrupts
	*AUX_MU_IIR = 0xc6;    // Disable interrupts
	*AUX_MU_BAUD = 270;    // 115200 baud rate, 
	/* map UART1 to GPIO pins */
	r=*GPFSEL1;
	r&=~((7<<12)|(7<<15)); // Clear GPIO 14, 15
	r|=(2<<12)|(2<<15);    // Alt5
	*GPFSEL1 = r;
	*GPPUD = 0;            // Enable pins 14 and 15
	
	r=150; while(r--) { asm volatile("nop"); }
	*GPPUDCLK0 = (1<<14)|(1<<15);
	r=150; while(r--) { asm volatile("nop"); }
	*GPPUDCLK0 = 0;        // flush GPIO setup
	*AUX_MU_CNTL = 3;      // enable Tx, Rx
}


unsigned char uart_getc()
{
    /* wait until something is in the buffer */
    do { asm volatile("nop"); }while( !(*AUX_MU_LSR & 0x01) );
    return (unsigned char)(*AUX_MU_IO);
}



void uart_putc(unsigned char c)
{
  /*
  * wait for the LSR (previous action) to ready for the current action
  * send a character to the IO register,
  */
  do { asm volatile("nop"); }while ( !(*AUX_MU_LSR & 0x01) );
  *AUX_MU_IO=c;
}


void uart_puts(const char *str)
{
  while (*str)
  {	
		if (*str == '\n') 
		{ uart_putc('\r');
			str++;
		} else 
		{
    	uart_putc(*str++);
		}
  }
}

