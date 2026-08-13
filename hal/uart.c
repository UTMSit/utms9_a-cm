#include <utms/hal/stm32f4xx.h>
#include <utms/hal/uart.h>

static char uart_rx_buffer[128];
static volatile uint32_t uart_rx_head = 0;
static volatile uint32_t uart_rx_tail = 0;

void uart_init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_DMA1EN;
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
    USART2->CR3 |= USART_CR3_DMAR;

    DMA1_Stream5->CR = 0;
    while (DMA1_Stream5->CR & DMA_SxCR_EN);

    DMA1_Stream5->NDTR = 1;
    DMA1_Stream5->PAR = (uint32_t)&USART2->DR;
    DMA1_Stream5->M0AR = (uint32_t)uart_rx_buffer;

    DMA1_Stream5->CR = DMA_SxCR_CHSEL_4 |
                       DMA_SxCR_DIR_P2M |
                       DMA_SxCR_MINC |
                       DMA_SxCR_PSIZE_8 |
                       DMA_SxCR_MSIZE_8 |
                       DMA_SxCR_PL_HIGH |
                       DMA_SxCR_TCIE;

    DMA1_Stream5->FCR = 0;

    NVIC->ISER[16 / 32] = (1UL << (16 % 32));
    NVIC->IPR[16] = 0x40;

    DMA1_Stream5->CR |= DMA_SxCR_EN;
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

uint8_t uart_has_data(void) {
    return (uart_rx_head != uart_rx_tail) ? 1 : 0;
}

char uart_getc(void) {
    while (uart_rx_head == uart_rx_tail);
    char c = uart_rx_buffer[uart_rx_tail];
    uart_rx_tail = (uart_rx_tail + 1) % 128;
    return c;
}

void DMA1_Stream5_IRQHandler(void) {
    if (DMA1->HISR & (1UL << 5)) {
        DMA1->HIFCR = (1UL << 5);

        uart_rx_head = (uart_rx_head + 1) % 128;

        DMA1_Stream5->CR &= ~DMA_SxCR_EN;
        DMA1_Stream5->NDTR = 1;
        DMA1_Stream5->CR |= DMA_SxCR_EN;
    }
}
