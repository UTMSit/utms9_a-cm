#ifndef UTMS_UART_H
#define UTMS_UART_H

#include <stdint.h>

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
void uart_print_hex(uint32_t val);
char uart_getc(void);
uint8_t uart_has_data(void);

#endif
