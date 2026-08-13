#ifndef UTMS_LASER_H
#define UTMS_LASER_H

#include <stdint.h>
#include <utms/hal/stm32f4xx.h>

typedef struct {
    TIM_TypeDef *pwm_tim;
    uint8_t pwm_channel;
    float max_power;
    float current_power;
    uint8_t enabled;
} utms_laser_t;

void utms_laser_init(utms_laser_t *l);
void utms_laser_set_power(utms_laser_t *l, float power);
void utms_laser_enable(utms_laser_t *l, uint8_t enable);
void utms_laser_emergency_stop(utms_laser_t *l);

#endif
