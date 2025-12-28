
#include "gpio.h"

void gpio_set_function(unsigned int pin, unsigned int function)
{
	unsigned int reg = pin / 10;
	unsigned int shift = (pin % 10) * 3;

	volatile unsigned int* fsel_reg = (volatile unsigned int*)((char*)GPFSEL0 + (reg * 4));

	unsigned int value = *fsel_reg;
	value &= ~(7 << shift); // clear the bits for this pin
	value |= (function << shift); // set the new function
	*fsel_reg = value;
}


#ifdef USE_LED_COUNTER16
void initialize_counter16_led_pins()
{
	gpio_set_function(LED_PIN_0, GPIO_FUNC_OUTPUT);
	gpio_set_function(LED_PIN_1, GPIO_FUNC_OUTPUT);
	gpio_set_function(LED_PIN_2, GPIO_FUNC_OUTPUT);
	gpio_set_function(LED_PIN_3, GPIO_FUNC_OUTPUT);
}
#endif

void gpio_write(unsigned int pin, unsigned int value)
{
	if (pin < 32)
	{
		if (value)
		{
			*GPSET0 = (1 << pin);
		} else
		{
			*GPCLR0 = (1 << pin);
		}
	}
	else if (pin < 54) 
	{
		if (value)
		{
			*GPSET1 = (1 << (pin - 32));
		} else
		{
			*GPCLR1 = (1 << (pin - 32));
		}
	}
}

#ifdef USE_LED_COUNTER16
void set_counter16_led_pins(unsigned int value)
{
	gpio_write(LED_PIN_0, (value >> 0) & 1);
	gpio_write(LED_PIN_1, (value >> 1) & 1);
	gpio_write(LED_PIN_2, (value >> 2) & 1);
	gpio_write(LED_PIN_3, (value >> 3) & 1);

}
#endif
