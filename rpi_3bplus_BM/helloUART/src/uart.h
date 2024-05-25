/*
* document all the functions defined in uart.c file
*/

#ifndef UART_H
#define UART_H

void uart_init(void);
void uart_putc(unsigned char c);
void uart_puts(const char *str);

#endif // UART_H

