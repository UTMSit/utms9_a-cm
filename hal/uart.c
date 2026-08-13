#include <utms/hal/stm32f4xx.h>
#include <utms/hal/uart.h>

void uart_init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    GPIOA->MODER &= ~(3UL << (2 * 2));
    GPIOA->MODER |= (2UL << (2 * 2));

    GPIOA->AFR[0] &= ~(15UL << (2 * 4));
    GPIOA->AFR[0] |= (7UL << (2 * 4));

    GPIOA->MODER &= ~(3UL << (3 * 2));
    GPIOA->MODER |= (2UL << (3 * 2));

    GPIOA->AFR[0] &= ~(15UL << (3 * 4));
    GPIOA->AFR[0] |= (7UL << (3 * 4));

    USART2->BRR = 42000000 / 115200;
    USART2->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void uart_putc(char c) {
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = c;
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

void uart_print_hex(uint32_t val) {
    char buf[8];
    for (int i = 7; i >= 0; i--) {
        uint8_t nibble = (val >> (i * 4)) & 0xF;
        buf[7 - i] = nibble < 10 ? '0' + nibble : 'A' + nibble - 10;
    }
    for (int i = 0; i < 8; i++) {
        uart_putc(buf[i]);
    }
}

char uart_getc(void) {
    while (!(USART2->SR & USART_SR_RXNE));
    return (char)USART2->DR;
}

uint8_t uart_has_data(void) {
    return (USART2->SR & USART_SR_RXNE) ? 1 : 0;
}
