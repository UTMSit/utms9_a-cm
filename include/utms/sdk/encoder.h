#ifndef UTMS_ENCODER_H
#define UTMS_ENCODER_H

#include <stdint.h>
#include <utms/hal/stm32f4xx.h>

typedef struct {
    TIM_TypeDef *tim;
} encoder_t;

void encoder_init(const encoder_t *enc);
int32_t encoder_get_position(const encoder_t *enc);
void encoder_reset(const encoder_t *enc);

#endif
