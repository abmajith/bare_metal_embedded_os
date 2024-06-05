#include "uart0.h"

void default_handler(void) { uart0_puts("Synchronous EL1t\n"); }
void irq_handler(void)     { uart0_puts("IRQ EL1t\n"); }
void fiq_handler(void)     { uart0_puts("FIQ EL1t\n"); }

void kernel_main(void)
{
	uart0_init();
	uart0_puts("Hello, Kernel World!\n");

	while (1)
	{
		uart0_putc(uart0_getc());
	}
}



