#include "uart.h"

void kernel_main(void)
{
  uart_init();
  uart_puts("Hello, Folks, This is Bare Metal Embedded DEV (using ARM64 GCC ToolChain)");
  while(1);
}
