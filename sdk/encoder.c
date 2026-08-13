#include <utms/sdk/encoder.h>

void encoder_init(const encoder_t *enc) {
    enc->tim->SMCR = 0;
    enc->tim->CCMR1 = (1UL << 0) | (1UL << 8);
    enc->tim->CCER = 0;
    enc->tim->PSC = 0;
    enc->tim->ARR = 0xFFFFFFFF;
    enc->tim->CNT = 0;
    enc->tim->CR1 = TIM_CR1_CEN;
}

int32_t encoder_get_position(const encoder_t *enc) {
    return (int32_t)enc->tim->CNT;
}

void encoder_reset(const encoder_t *enc) {
    enc->tim->CNT = 0;
}
