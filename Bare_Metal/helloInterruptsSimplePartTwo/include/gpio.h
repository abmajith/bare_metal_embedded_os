#ifndef GPIO_H
#define GPIO_H

#include "debug_config.h"
#include "mmio.h"

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

#define GPREN0          ((volatile unsigned int*)(GPIO_BASE + 0x4C))
#define GPFEN0          ((volatile unsigned int*)(GPIO_BASE + 0x58))


#define GPHEN0          ((volatile unsigned int*)(GPIO_BASE + 0x64))
#define GPHEN1          ((volatile unsigned int*)(GPIO_BASE + 0x68))

#define GPPUD           ((volatile unsigned int*)(GPIO_BASE + 0x94))
#define GPPUDCLK0       ((volatile unsigned int*)(GPIO_BASE + 0x98))
#define GPPUDCLK1       ((volatile unsigned int*)(GPIO_BASE + 0x9C))


// some led pins
#define LED_PIN_0 6
#define LED_PIN_1 13
#define LED_PIN_2 19
#define LED_PIN_3 26


#define GPIO_FUNC_INPUT  0b000
#define GPIO_FUNC_OUTPUT 0b001




void gpio_set_function(unsigned int pin, unsigned int function);

#ifdef USE_LED_COUNTER16
void initialize_counter16_led_pins(void);
void set_counter16_led_pins(unsigned int value);
#endif

void gpio_write(unsigned int pin, unsigned int value);

#endif // GPIO_H

