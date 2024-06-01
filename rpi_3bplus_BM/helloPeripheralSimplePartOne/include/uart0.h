/*
* document all the functions defined in uart.c file
*/

#ifndef UART0_H
#define UART0_H


#include "mmio.h"


#define UART0_DR        ((volatile unsigned int *)(UART0_BASE + 0x00))  // data register
#define UART0_FR        ((volatile unsigned int *)(UART0_BASE + 0x18))  // flag register
#define UART0_IBRD      ((volatile unsigned int *)(UART0_BASE + 0x24))  // integer baud rate div
#define UART0_FBRD      ((volatile unsigned int *)(UART0_BASE + 0x28))  // fractional baud rate div
#define UART0_LCRH      ((volatile unsigned int *)(UART0_BASE + 0x2C))  // line control register
#define UART0_CR        ((volatile unsigned int *)(UART0_BASE + 0x30))  // control register
#define UART0_IMSC      ((volatile unsigned int *)(UART0_BASE + 0x38))  // interrupt mask set/clear register
#define UART0_ICR       ((volatile unsigned int *)(UART0_BASE + 0x44))  // interrupt control register



void uart0_init();
void uart0_putc(unsigned int c);
unsigned char uart0_getc();
void uart0_puts(char *s);


#endif // UART0_H

