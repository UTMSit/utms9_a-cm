#include <utms/hal/step_timer.h>

#define TIM_CR2_MMS_UPDATE (2UL << 4)
#define TIM_SMCR_TS_ITR0   (0UL << 4)
#define TIM_SMCR_SMS_TRIG  (6UL << 0)

void step_timer_init(step_timer_t *st) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~(3UL << (0 * 2));
    GPIOA->MODER |= (2UL << (0 * 2));

    GPIOA->AFR[0] &= ~(15UL << (0 * 4));
    GPIOA->AFR[0] |= (2UL << (0 * 4));

    st->tim->PSC = st->psc;
    st->tim->ARR = st->arr;
    st->tim->CCR1 = st->ccr;

    st->tim->CCMR1 = TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE;
    st->tim->CCER = TIM_CCER_CC1E;

    if (st->role == STEP_TIMER_MASTER) {
        st->tim->CR2 = TIM_CR2_MMS_UPDATE;
        st->tim->SMCR = 0;
    } else {
        st->tim->SMCR = TIM_SMCR_TS_ITR0 | TIM_SMCR_SMS_TRIG;
    }
}

void step_timer_set_freq(step_timer_t *st, uint32_t freq_hz) {
    if (freq_hz == 0) {
        st->tim->CR1 &= ~TIM_CR1_CEN;
        return;
    }

    st->tim->CR1 |= TIM_CR1_CEN;

    uint32_t arr = 1000000 / freq_hz;
    st->tim->ARR = arr;
    st->tim->CCR1 = arr / 2;
}

void step_timer_dma_init(step_timer_t *st) {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

    st->dma_stream->CR = 0;
    while (st->dma_stream->CR & DMA_SxCR_EN);

    st->dma_stream->NDTR = 16;
    st->dma_stream->PAR = (uint32_t)&st->tim->ARR;
    st->dma_stream->M0AR = 0;

    st->dma_stream->CR = (st->dma_channel << 25) |
                       DMA_SxCR_DIR_M2P |
                       DMA_SxCR_MINC |
                       DMA_SxCR_PSIZE_32 |
                       DMA_SxCR_MSIZE_32 |
                       DMA_SxCR_PL_HIGH;

    st->dma_stream->FCR = 0;

    st->tim->DIER |= TIM_DIER_UDE;

    st->dma_stream->CR |= DMA_SxCR_EN;
}

void step_timer_set_buffer(step_timer_t *st, uint32_t *buffer, uint32_t len) {
    st->dma_stream->CR &= ~DMA_SxCR_EN;
    while (st->dma_stream->CR & DMA_SxCR_EN);

    st->dma_stream->NDTR = len;
    st->dma_stream->M0AR = (uint32_t)buffer;

    st->dma_stream->CR |= DMA_SxCR_EN;
}

void step_timer_sync_start(step_timer_t **masters, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        if (masters[i] && masters[i]->role == STEP_TIMER_MASTER) {
            masters[i]->tim->EGR = 1;
            masters[i]->tim->CR1 |= TIM_CR1_CEN;
        }
    }
}

void step_timer_sync_stop(step_timer_t **masters, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        if (masters[i]) {
            masters[i]->tim->CR1 &= ~TIM_CR1_CEN;
            masters[i]->dma_stream->CR &= ~DMA_SxCR_EN;
        }
    }
}
