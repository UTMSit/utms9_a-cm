#ifndef UTMS_STEP_TIMER_H
#define UTMS_STEP_TIMER_H

#include <stdint.h>
#include <utms/hal/stm32f4xx.h>

typedef enum {
    STEP_TIMER_MASTER = 0,
    STEP_TIMER_SLAVE = 1
} step_timer_role_t;

typedef struct {
    TIM_TypeDef *tim;
    DMA_Stream_TypeDef *dma_stream;
    uint32_t dma_channel;
    uint32_t psc;
    uint32_t arr;
    uint32_t ccr;
    step_timer_role_t role;
} step_timer_t;

void step_timer_init(step_timer_t *st);
void step_timer_set_freq(step_timer_t *st, uint32_t freq_hz);
void step_timer_dma_init(step_timer_t *st);
void step_timer_set_buffer(step_timer_t *st, uint32_t *buffer, uint32_t len);
void step_timer_sync_start(step_timer_t **masters, uint32_t count);
void step_timer_sync_stop(step_timer_t **masters, uint32_t count);

#endif
