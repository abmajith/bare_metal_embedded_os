#include "interrupt_controller.h"
#include "uart0.h"
#include "timer.h"

void setup_vector_table(void* vector_base) 
{
    asm volatile (
        "msr VBAR_EL1, %0\n" // Set VBAR_EL1 to the base address of the vector table
        "isb\n"              // Instruction Synchronization Barrier to ensure the change takes effect immediately
        :                    // No output operands
        : "r" (vector_base)  // Input operand: the address of the vector table
        : "memory"           // Clobber list
    );
}


void default_handler(void) 
{
    uart0_printf("Unhandled exception\r\n");
}

void irq_handler(void) 
{
    uart0_puts("IRQ received\n");
    unsigned int irq = *IRQ_PENDING_1;

    if (irq & SYSTEM_TIMER_IRQ_1) {
        handle_timer_interrupt();
    } else {
        uart0_puts("Unknown pending irq\n");
    }
}

void enable_interrupt(void) 
{
    asm volatile (
        "msr daifclr, #2\n"   // Clear the I flag (IRQ enable)
        :
        :
        : "memory"   // Clobber list
    );
}

void disable_interrupt(void) 
{
    asm volatile (
        "msr daifset, #2\n"   // Set the I flag (IRQ disable)
        :
        :
        : "memory"  // Clobber list
    );
}


void enable_interrupt_controller() 
{
    *ENABLE_IRQS_1 = SYSTEM_TIMER_IRQ_1;
}
