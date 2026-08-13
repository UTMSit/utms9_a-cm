#ifndef UTMS_SPINDLE_H
#define UTMS_SPINDLE_H

#include <stdint.h>
#include <utms/hal/stm32f4xx.h>

typedef struct {
    TIM_TypeDef *pwm_tim;
    uint8_t pwm_channel;
    uint32_t max_rpm;
    uint32_t current_rpm;
    uint8_t enabled;
} utms_spindle_t;

void utms_spindle_init(utms_spindle_t *sp);
void utms_spindle_set_rpm(utms_spindle_t *sp, uint32_t rpm);
void utms_spindle_enable(utms_spindle_t *sp, uint8_t enable);
void utms_spindle_emergency_stop(utms_spindle_t *sp);

#endif
