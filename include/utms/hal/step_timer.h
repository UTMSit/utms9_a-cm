#ifndef UTMS_STEP_TIMER_H
#define UTMS_STEP_TIMER_H

#include <stdint.h>
#include <utms/hal/stm32f4xx.h>

typedef struct {
    TIM_TypeDef *tim;
    DMA_Stream_TypeDef *dma_stream;
    uint32_t dma_channel;
    uint32_t psc;
    uint32_t arr;
    uint32_t ccr;
} step_timer_t;

void step_timer_init(step_timer_t *st);
void step_timer_set_freq(step_timer_t *st, uint32_t freq_hz);
void step_timer_dma_init(step_timer_t *st);
void step_timer_set_buffer(step_timer_t *st, uint32_t *buffer, uint32_t len);

#endif
