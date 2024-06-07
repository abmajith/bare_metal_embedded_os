


#include "interrupt_controllers.h"
#include "uart0.h"
#include "timer.h"


void setup_vector_table(void* vector_base) 
{
	asm volatile (
		"msr VBAR_EL1, %0\n" // Set VBAR_EL1 to the base address of the vector table
		"isb\n"               // Instruction Synchronization Barrier to ensure the change takes effect immediately
		:                     // No output operands
		: "r" (vector_base)   // Input operand: the address of the vector table
		: "memory"            // Clobber list
	);
}

void default_handler(void)
{
	uart0_puts("Unhandled exception\r\n");
}

void irq_handler(void)
{
	uart0_puts("IRQ received\n");
	unsigned int irq	= *IRQ_PENDING_1;

	switch (irq)
	{
		case (SYSTEM_TIMER_IRQ_1):
			handle_timer_interrupt();
			break;
		default:
			uart0_puts("Unknown pending irq"); // need better print statment to include integer, hexadecimal etc...
	}
}


void enable_interrupt ( void )
{
	asm volatile (
		"msr daifclr, #2\n"   // Clear the I flag (IRQ enable)
		:
		:
		: "memory"   // clobber list
	);
}

void disable_interrupt( void )
{
	asm volatile (
		"msr daifset, #2\n"   // Set the I flag (IRQ disable)
		:
		:
		: "memory"  // clobber list
	);
}


// kind of global interrupt controller
void enable_interrupt_controller()
{
	*ENABLE_IRQS_1 = SYSTEM_TIMER_IRQ_1;
}
