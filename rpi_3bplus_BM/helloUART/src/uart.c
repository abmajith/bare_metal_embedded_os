
/*
* Defining various uart functions here
*/


#include "mmio.h"


void uart_init()
{

  mmio_write(AUX_ENABLES, 0x1);          // Enable mini UART
  mmio_write(AUX_MU_IER_REG, 0x0);       // Disable interrupts
  mmio_write(AUX_MU_IIR_REG, 0xc6);      // Disable interrupts
  mmio_write(AUX_MU_CNTL_REG, 0x0);      // Disable transmitter and receiver
  mmio_write(AUX_MU_LCR_REG, 0x3);       // 8-bit mode
  mmio_write(AUX_MU_MCR_REG, 0x0);       // RTS line
  mmio_write(AUX_MU_BAUD_REG, 0x10E);    // 115200 baud rate, 270

  uint32_t ra = mmio_read(GPFSEL1);
  ra &= ~((7 << 12) | (7 << 15));      // Clear GPIO 14 and 15
  ra |= (2 << 12) | (2 << 15);         // Set GPIO 14 and 15 to ALT5
  mmio_write(GPFSEL1, ra);

  mmio_write(GPPUD, 0);                // Disable pull-up/down
  for (int i = 0; i < 150; i++) { asm volatile("nop"); } // delay for 150 clocks
  mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
  for (int i = 0; i < 150; i++) { asm volatile("nop"); } // delay for 150 clocks
  mmio_write(GPPUDCLK0, 0);
  mmio_write(AUX_MU_CNTL_REG, 0x3);      // Enable transmitter and receiver

}


void uart_putc(unsigned char c)
{
  /*
  * wait for the LSR (previous action) to ready for the current action
  * send a character to the IO register,
  */
  while (!(mmio_read(AUX_MU_LSR_REG) & 0x20));
  mmio_write(AUX_MU_IO_REG, c);
}

void uart_puts(const char *str)
{
  while (*str)
  {
    uart_putc(*str++);
  }
}
