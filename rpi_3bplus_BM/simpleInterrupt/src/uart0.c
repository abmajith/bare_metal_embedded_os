#include "mmio.h"
#include "uart0.h"
#include <stdarg.h>
#include <stdint.h>



// base address for memory-mapped I/O's
#define MAILBOX_BASE    (MMIO_BASE + 0x0000B880)  // Mailbox Interface
#define UART0_BASE      (MMIO_BASE + 0x00201000)  // UART0 (serial port, PL011)

#define GPIO_BASE       (MMIO_BASE + 0x00200000)  // GPIO Base



#define UART0_DR        ((volatile unsigned int *)(UART0_BASE + 0x00))  // data register
#define UART0_FR        ((volatile unsigned int *)(UART0_BASE + 0x18))  // flag register
#define UART0_IBRD      ((volatile unsigned int *)(UART0_BASE + 0x24))  // integer baud rate div
#define UART0_FBRD      ((volatile unsigned int *)(UART0_BASE + 0x28))  // fractional baud rate div
#define UART0_LCRH      ((volatile unsigned int *)(UART0_BASE + 0x2C))  // line control register
#define UART0_CR        ((volatile unsigned int *)(UART0_BASE + 0x30))  // control register
#define UART0_IMSC      ((volatile unsigned int *)(UART0_BASE + 0x38))  // interrupt mask set/clear register
#define UART0_ICR       ((volatile unsigned int *)(UART0_BASE + 0x44))  // interrupt control register



#define GPFSEL0         ((volatile unsigned int*)(GPIO_BASE + 0x00))
#define GPFSEL1         ((volatile unsigned int*)(GPIO_BASE + 0x04))
#define GPFSEL2         ((volatile unsigned int*)(GPIO_BASE + 0x08))
#define GPFSEL3         ((volatile unsigned int*)(GPIO_BASE + 0x0C))
#define GPFSEL4         ((volatile unsigned int*)(GPIO_BASE + 0x10))
#define GPFSEL5         ((volatile unsigned int*)(GPIO_BASE + 0x14))

#define GPSET0          ((volatile unsigned int*)(GPIO_BASE + 0x1C))
#define GPSET1          ((volatile unsigned int*)(GPIO_BASE + 0x20))
#define GPCLR0          ((volatile unsigned int*)(GPIO_BASE + 0x28))
#define GPCLR1          ((volatile unsigned int*)(GPIO_BASE + 0x2C))

#define GPLEV0          ((volatile unsigned int*)(GPIO_BASE + 0x34))
#define GPLEV1          ((volatile unsigned int*)(GPIO_BASE + 0x38))

#define GPEDS0          ((volatile unsigned int*)(GPIO_BASE + 0x40))
#define GPEDS1          ((volatile unsigned int*)(GPIO_BASE + 0x44))

#define GPHEN0          ((volatile unsigned int*)(GPIO_BASE + 0x64))
#define GPHEN1          ((volatile unsigned int*)(GPIO_BASE + 0x68))

#define GPPUD           ((volatile unsigned int*)(GPIO_BASE + 0x94))
#define GPPUDCLK0       ((volatile unsigned int*)(GPIO_BASE + 0x98))
#define GPPUDCLK1       ((volatile unsigned int*)(GPIO_BASE + 0x9C))




#define MAILBOX_READ    ((volatile unsigned int *)(MAILBOX_BASE + 0x0))
#define MAILBOX_POLL    ((volatile unsigned int *)(MAILBOX_BASE + 0x10))
#define MAILBOX_SENDER  ((volatile unsigned int *)(MAILBOX_BASE + 0x14))
#define MAILBOX_STATUS  ((volatile unsigned int *)(MAILBOX_BASE + 0x18))
#define MAILBOX_CONFIG  ((volatile unsigned int *)(MAILBOX_BASE + 0x1C))
#define MAILBOX_WRITE   ((volatile unsigned int *)(MAILBOX_BASE + 0x20))



// some mail box message type (return message)
#define MAILBOX_RESPONSE   0x80000000
#define MAILBOX_FULL       0x80000000
#define MAILBOX_EMPTY      0x40000000


// some mail box channels type

#define MAILBOX_M_CH_POWER    0 // for power management
#define MAILBOX_M_CH_FB       1 // for frame buffer
#define MAILBOX_M_CH_VUART    2 // 
#define MAILBOX_M_CH_VCHIQ    3 // for interrupt controller
#define MAILBOX_M_CH_LEDS     4
#define MAILBOX_M_CH_BTNS     5
#define MAILBOX_M_CH_TOUCH    6
#define MAILBOX_M_CH_COUNT    7 // for counter
#define MAILBOX_M_CH_PROP     8 // for property


// some mail box message request type (sending message)
#define MAILBOX_M_GETSERIAL   0x10004
#define MAILBOX_M_SETCLKRATE  0x38002
#define MAILBOX_M_GETCLKRATE  0x30002
#define MAILBOX_M_SETPOWER    0x28001
#define MAILBOX_M_LAST        0
#define MAILBOX_M_REQUEST     0





volatile unsigned int __attribute__((aligned(16))) mbox_buf[36]; // provide some space for mailbox communications



// for printf functions
static const char* hex_digits = "0123456789ABCDEF";
static char str_buffer[10];

int mailbox_call(unsigned char channel)
{
  unsigned int r = (((unsigned int)((unsigned long)&mbox_buf) & ~0xF) | (channel & 0xF));
  // wait for the mail box
  do{asm volatile("nop");}while (*MAILBOX_STATUS & MAILBOX_FULL);

  /* sending aligned mailbox memory address, so that gpu firmware will read
  * and perform actions accordingly
  */

  *MAILBOX_WRITE = r;
  while (1)
  {
    // wait until mailbox is not empty
    do{asm volatile("nop");}while (*MAILBOX_STATUS & MAILBOX_EMPTY);

    if (r == *MAILBOX_READ)
    {
      return mbox_buf[1] == MAILBOX_RESPONSE; // got the acknowledgement, gpu firmware read our mailbox message
    }
  }
  return 0;
}



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

