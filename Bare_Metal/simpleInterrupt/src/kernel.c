#include "uart0.h"
#include "timer.h"
#include "interrupt_controller.h"



void kernel_main(void)
{
	uart0_init();
	

	uart0_printf("Hello, Folks!\n This is Bare Metal Embedded DEV (using ARM64 GCC ToolChain)\n");
	int serial_number_part1 = 0xE8DBA4EB;
	int serial_number_part2 = 0x00000000;
	uart0_printf("Serial number is %d %d\n", serial_number_part1, serial_number_part2);
	
	setup_vector_table(&vector_table);
	timer_init();
	enable_interrupt_controller();
	enable_interrupt();
	while (1)
	{
		asm volatile("wfi"); // wait for interrupt
	}
}

