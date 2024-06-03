


#include "interrupt_controllers.h"
#include "uart0.h"


volatile __attribute__((aligned(2048)))  
	void (*vectors[])(void) = {
		[0] = default_handler,  // Synchronous EL1t
		[1] = irq_handler,      // IRQ EL1t
		[2] = fiq_handler,      // FIQ EL1t
		[3] = default_handler,  // SError EL1t
		[4] = default_handler,  // Synchronous EL1h
		[5] = irq_handler,      // IRQ EL1h
		[6] = fiq_handler,      // FIQ EL1h
		[7] = default_handler,  // SError EL1h
		[8] = default_handler,  // Synchronous EL0_64
		[9] = irq_handler,      // IRQ EL0_64
		[10] = fiq_handler,     // FIQ EL0_64
		[11] = default_handler, // SError EL0_64
		[12] = default_handler, // Synchronous EL0_32
		[13] = irq_handler,     // IRQ EL0_32
		[14] = fiq_handler,     // FIQ EL0_32
		[15] = default_handler, // SError EL0_32
	};


void default_handler(void)
{
	uart0_puts("Unhandled exception\r\n");
}

void irq_handler(void)
{
	uart0_puts("IRQ received\n");
}
void fiq_handler(void)
{
	uart0_puts("FIQ received\n");
}

void set_vbar_el1(uint64_t addr)
{
	asm volatile ("msr vbar_el1, %0" : : "r" (addr) : "memory");
}

