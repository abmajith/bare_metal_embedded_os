#include "mmio.h"
#include "gpio.h"
#include "rng.h"
#include "uart0.h"
#include "mailbox.h"
#include "power.h"
#include "timer.h"
#include "privilege.h"
#include "interrupt_controllers.h"
#include "framebuffer.h"


void kernel_main(void)
{
  uart0_init(); // init debugging serial port
	setup_vector_table(&vector_table);
	timer_init();
	enable_interrupt_controller();
	enable_interrupt();
	rng_init();   // init the random number generation
	

  uart0_printf("Hello, Folks!\r\n This is Bare Metal Embedded DEV (using ARM64 GCC ToolChain)\r\n");

  // set up our mail box message buffer
  mbox_buf[0] = 8*4;                  // size of our message buffer
  mbox_buf[1] = MAILBOX_M_REQUEST;    // request message
  mbox_buf[2] = MAILBOX_M_GETSERIAL;  // request to get serial number
  mbox_buf[3] = 8;                    // Value buffer size (bytes)
  mbox_buf[4] = 8;                    // Request size (bytes)
  mbox_buf[5] = 0;                    //
  mbox_buf[6] = 0;                    // End tag
  mbox_buf[7] = MAILBOX_M_LAST;       // pad the message with proper boundary close


  if (mailbox_call(MAILBOX_M_CH_PROP))
  {
    uart0_printf("Serial number is %u  %u \r\n", mbox_buf[5], mbox_buf[6]);
  }
  else
  {
    uart0_printf("Unable to query Serial number!\r\n");
  }
	

	// testing my random numbers
	unsigned int random_number = rng_get_random();
	uart0_printf("Generated random number is : %u \r\n\r", random_number);
	
	random_number = rng_get_random();
	uart0_printf("another generated random number : %u \r\n\r", random_number);
	
	unsigned int el = get_current_el();

	uart0_printf("Current Exception Level: %c \n\r", '0' + el); // Assuming EL is between 0 and 3

	

	// testing my timer delay pooling type
	uart0_printf("Polling sleep for 0x100000...\r\n");
	delay_milliseconds(0x100000);
	disable_interrupt();
	delay_milliseconds(0x100000);
	enable_interrupt();
	delay_milliseconds(0x100000);
	disable_interrupt();
	


	// testing my power managment functions
	/*
  // both were tested it works
  uart0_puts("watch dog reset...\n");
  watchdog_reset(0x10000);
  uart0_puts("reset...\n");
  reset();
  */
	// power_off();
	
	// checking frame buffer initialization
	if (framebuffer_init() == 0)
	{
		uart0_printf("Framebuffer initialized successfully \r\n");
	} 
	else 
	{
		uart0_printf("Failed Framebuffer Initializtion\r\n");
	}

  while(1) { asm volatile("wfi"); } // infinite loop
}

