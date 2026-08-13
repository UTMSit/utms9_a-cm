#ifndef UTMS_HEATER_H
#define UTMS_HEATER_H

#include <stdint.h>
#include <utms/hal/stm32f4xx.h>
#include <utms/sdk/thermistor.h>

typedef struct {
    float kp;
    float ki;
    float kd;
    float integral;
    float prev_error;
    float output_min;
    float output_max;
} utms_pid_t;

typedef struct {
    TIM_TypeDef *pwm_tim;
    uint8_t pwm_channel;
    utms_thermistor_t *thermistor;
    utms_pid_t pid;
    float target_temp;
    uint8_t enabled;
} utms_heater_t;

void utms_heater_init(utms_heater_t *h);
void utms_heater_set_target(utms_heater_t *h, float temp);
void utms_heater_update(utms_heater_t *h, float dt);
void utms_heater_enable(utms_heater_t *h, uint8_t enable);
void utms_heater_emergency_stop(utms_heater_t *h);

#endif
