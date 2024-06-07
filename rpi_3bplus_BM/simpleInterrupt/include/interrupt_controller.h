#ifndef INTERRUPT_CONTROLLERS_H
#define INTERRUPT_CONTROLLERS_H

#include "mmio.h"

#define GIC_BASE        (MMIO_BASE + 0x0000B000)  // Generic Interrupt Controller
#define IRQ_BASIC_PENDING  ((volatile unsigned int*)(GIC_BASE + 0x200))
#define IRQ_PENDING_1      ((volatile unsigned int*)(GIC_BASE + 0x204))
#define IRQ_PENDING_2      ((volatile unsigned int*)(GIC_BASE + 0x208))
#define FIQ_CONTROL        ((volatile unsigned int*)(GIC_BASE + 0x20C))
#define ENABLE_IRQS_1      ((volatile unsigned int*)(GIC_BASE + 0x210))
#define ENABLE_IRQS_2      ((volatile unsigned int*)(GIC_BASE + 0x214))
#define ENABLE_BASIC_IRQS  ((volatile unsigned int*)(GIC_BASE + 0x218))
#define DISABLE_IRQS_1     ((volatile unsigned int*)(GIC_BASE + 0x21C))
#define DISABLE_IRQS_2     ((volatile unsigned int*)(GIC_BASE + 0x220))
#define DISABLE_BASIC_IRQS ((volatile unsigned int*)(GIC_BASE + 0x224))

#define SYSTEM_TIMER_IRQ_1  (1 << 1) // I am handling only one timer interrupt here

extern void vector_table(void);

// Set up the interrupt vector table address
void setup_vector_table(void* vector_base);

void default_handler(void);
void irq_handler(void);

void enable_interrupt_controller(void);
void enable_interrupt(void); // Clear the I flag in DAIF
void disable_interrupt(void); // Set the I flag in DAIF

#endif  // INTERRUPT_CONTROLLERS_H
