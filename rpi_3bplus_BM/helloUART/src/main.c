#include "uart.h"

void kernel_main(void)
{
  uart_init();
  uart_puts("Hello, Folks!\n This is Bare Metal Embedded DEV (using ARM64 GCC ToolChain)\n");
  while(1) 
	{
		uart_putc(uart_getc());
	}
}
