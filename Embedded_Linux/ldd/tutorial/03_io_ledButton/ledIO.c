#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

#define BCM_LED 21
#define BCM_BUZZER 20
#define BCM_BTN 16

static int irqNumber;
static int gpio_base = 0;
static struct timer_list razione_timer;
static bool is_active = false;

static void razione_timer_callback(struct timer_list *t)
{
	gpio_set_value(gpio_base + BCM_LED, 0);
	gpio_set_value(gpio_base + BCM_BUZZER, 0);
	is_active = false;
	pr_info("RazioneIO: Timer expired - LED and Buzzer turned OFF\n");
}

static irqreturn_t button_irq_handler(int irq, void *dev_id) 
{
	if (is_active)
	{
		return IRQ_HANDLED;
	}
	is_active = true;
	pr_info("RazioneIO: Button pressed! Turning ON for 1 second...\n");
	gpio_set_value(gpio_base + BCM_LED, 1);
	gpio_set_value(gpio_base + BCM_BUZZER, 1);
	mod_timer(&razione_timer, jiffies + msecs_to_jiffies(1000));
	return IRQ_HANDLED;
}

static int __init led_io_init(void) 
{
	gpio_base = 512; // to find this number follow the readme text
	if(gpio_request(gpio_base + BCM_LED, "LED") || gpio_request(gpio_base + BCM_BUZZER, "BUZZER") || gpio_request(gpio_base + BCM_BTN, "BTN")) 
	{
		return -EBUSY;
    	}
	gpio_direction_output(gpio_base + BCM_LED, 0);
	gpio_direction_output(gpio_base + BCM_BUZZER, 0);
	gpio_direction_input(gpio_base + BCM_BTN);

	irqNumber = gpio_to_irq(gpio_base + BCM_BTN);
	if (irqNumber < 0) goto fail;

	timer_setup(&razione_timer, razione_timer_callback, 0);

	if (request_irq(irqNumber, (irq_handler_t) button_irq_handler, IRQF_TRIGGER_FALLING, "razione_handler", NULL))
	{
		goto fail;
	}
	pr_info("RazioneIO: Driver loaded on RPi 3B+. IRQ: %d\n", irqNumber);
	return 0;

fail:
	gpio_free(gpio_base + BCM_LED);
	gpio_free(gpio_base + BCM_BUZZER);
	gpio_free(gpio_base + BCM_BTN);
	return -EINVAL;
}

static void __exit led_io_exit(void) 
{
	del_timer(&razione_timer);
    	free_irq(irqNumber, NULL);
    	gpio_free(gpio_base + BCM_LED);
    	gpio_free(gpio_base + BCM_BUZZER);
    	gpio_free(gpio_base + BCM_BTN);
	pr_info("RazioneIO: Driver unloaded.\n");
}

module_init(led_io_init);
module_exit(led_io_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("RAZIONE");
MODULE_DESCRIPTION("Interrupt driven IO with auto-off timer");
