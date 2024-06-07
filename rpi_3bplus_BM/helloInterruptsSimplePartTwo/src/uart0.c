
#include "gpio.h"
#include "uart0.h"
#include "mailbox.h"

#include <stdarg.h>
#include <stdint.h>



// for printf functions
static const char* hex_digits = "0123456789ABCDEF";
static char str_buffer[10];

void uart0_init()
{
  // Disable UART0
  *UART0_CR = 0x00000000;
  // set up our mail box message buffer
  mbox_buf[0] = 9*4;                  // size of our message buffer
  mbox_buf[1] = MAILBOX_M_REQUEST;    // request message
  mbox_buf[2] = MAILBOX_M_SETCLKRATE; // request to change clock rate
  mbox_buf[3] = 12;                   // Value buffer size (bytes)
  mbox_buf[4] = 8;                    // Request size (bytes)
  mbox_buf[5] = 2;                    // Clock id for UART
  mbox_buf[6] = 12000000;              // I choose 12MHz
  mbox_buf[7] = 0;                    // End tag
  mbox_buf[8] = MAILBOX_M_LAST;       // pad the message with proper boundary close

  mailbox_call(MAILBOX_M_CH_PROP); // hope it get response from firmware videocore gpu for changing the clock freq

  // Configure GPIO pins for UART0
  unsigned int ra = *GPFSEL1;
  ra &= ~((7 << 12) | (7 << 15));  // Clear GPIO14 and GPIO15
  ra |= (4 << 12) | (4 << 15);     // Set ALT0 for GPIO14 and GPIO15
  *GPFSEL1 = ra;

  // Disable pull-up/down for GPIO14 and GPIO15
  *GPPUD = 0;
  for (ra = 0; ra < 150; ra++) {asm volatile("nop");}
  *GPPUDCLK0 = (1 << 14) | (1 << 15);
  for (ra = 0; ra < 150; ra++) {asm volatile("nop");}
  *GPPUDCLK0 = 0;

	// Clear pending interrupts
  *UART0_ICR = 0x7FF;

  // Set integer & fractional part of baud rate

  // this is a hard coding to set up the required clock signal for uart0, now we will keep like this
  // but should be avoided in the future development

  /*
    will assing clock signal at 12MHz
    12 X 10**6 / (16 * 115200) ~= 6
    frac(_) = .5104
    (.5104 * 64 + 0.5) = 33.16666
  */
  *UART0_IBRD = 6;    // Integer part of baud rate divisor
  *UART0_FBRD = 33;   // Fractional part of baud rate divisor

  // Enable FIFO & 8-bit data transmission (1 stop bit, no parity)
  *UART0_LCRH = (3 << 5) | (1 << 4);

  // Mask all interrupts
  *UART0_IMSC = (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) |
                (1 << 8) | (1 << 9) | (1 << 10);
  // Enable UART0, receive & transmit part of UART
  *UART0_CR = (1 << 9) | (1 << 8) | (1 << 0);

}


void uart0_putc(unsigned int c)
{
  do{asm volatile("nop");}while(*UART0_FR &  (1 << 5) );
  *UART0_DR = c;
}


unsigned char uart0_getc()
{
  do{asm volatile("nop");}while(*UART0_FR & (1 << 4) );
  return (unsigned char)(*UART0_DR);
}

void uart0_puts(char *str)
{
  while (*str)
  {
    if (*str == '\n')
    {
      uart0_putc('\r');
    }
    uart0_putc(*str++);
  }
}

void uart0_put_hex(uintptr_t num)
{
	uart0_puts("0x");
	for (int i = 7; i >= 0; i--)
	{
		uart0_putc(hex_digits[(num >> (i * 4)) & 0xF]);
	}
}


void uart0_put_uint(unsigned int num)
{
	char *p = str_buffer + 9;
	*p = '\0';
	for (int i = 0; i < 9; i++)	
	{
		str_buffer[i] = '0';
	}
	do
	{
		*--p = '0' + (num % 10);
		num /= 10;
	} while (num);
	uart0_puts(str_buffer);
}

void uart0_put_int(int num)
{
	if (num < 0)
	{
		uart0_putc('-');
		num = -num;
	}
	uart0_put_uint((unsigned int) num);
}

void uart0_printf(const char* fmt, ...)
{
	va_list args;
	va_start(args,fmt);

	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			switch(*fmt)
			{
				case 'd':
				{
					int num = va_arg(args, int);
					uart0_put_int(num);
					break;
				}
				case 'u':
				{
					unsigned int num = va_arg(args, unsigned int);
					uart0_put_uint(num);
					break;
				}
				case 's':
				{
					char* str = va_arg(args, char*);
					uart0_puts(str);
					break;
				}
				case 'x':
				{
					unsigned int num = va_arg(args, unsigned int);
					uart0_put_hex(num);
					break;
				}
				case 'p':
				{
					void* ptr = va_arg(args, void*);
					uart0_put_hex((uintptr_t)ptr);
					break;
				}
				case 'c':
				{
					char c = (char)va_arg(args, int);
					uart0_putc(c);
					break;
				}
				default:
				{
					uart0_putc('%');
					uart0_putc(*fmt);
					break;
				}
			}
		}
		else 
		{
			uart0_putc(*fmt);
		}
		fmt++;
	}
	va_end(args);
}

