#include <utms/sdk/heater.h>

#define TIM_CCMR1_OC1M_PWM1 (6UL << 4)
#define TIM_CCMR1_OC1PE     (1UL << 3)
#define TIM_CCMR1_OC2M_PWM1 (6UL << 12)
#define TIM_CCMR1_OC2PE     (1UL << 11)
#define TIM_CCER_CC1E       (1UL << 0)
#define TIM_CCER_CC2E       (1UL << 4)
#define TIM_CR1_CEN         (1UL << 0)

static void heater_set_duty(utms_heater_t *h, float duty) {
    if (duty < 0.0f) duty = 0.0f;
    if (duty > 1.0f) duty = 1.0f;

    uint32_t arr = h->pwm_tim->ARR;
    uint32_t ccr = (uint32_t)(duty * (float)arr);

    switch (h->pwm_channel) {
        case 1: h->pwm_tim->CCR1 = ccr; break;
        case 2: h->pwm_tim->CCR2 = ccr; break;
        case 3: h->pwm_tim->CCR3 = ccr; break;
        case 4: h->pwm_tim->CCR4 = ccr; break;
    }
}

void utms_heater_init(utms_heater_t *h) {
    h->pwm_tim->PSC = 84 - 1;
    h->pwm_tim->ARR = 1000;

    switch (h->pwm_channel) {
        case 1:
            h->pwm_tim->CCMR1 |= TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE;
            h->pwm_tim->CCER |= TIM_CCER_CC1E;
            break;
        case 2:
            h->pwm_tim->CCMR1 |= TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_OC2PE;
            h->pwm_tim->CCER |= TIM_CCER_CC2E;
            break;
        case 3:
            h->pwm_tim->CCMR2 |= TIM_CCMR2_OC3M_PWM1 | TIM_CCMR2_OC3PE;
            h->pwm_tim->CCER |= TIM_CCER_CC3E;
            break;
        case 4:
            h->pwm_tim->CCMR2 |= TIM_CCMR2_OC4M_PWM1 | TIM_CCMR2_OC4PE;
            h->pwm_tim->CCER |= TIM_CCER_CC4E;
            break;
    }

    h->pwm_tim->CR1 = TIM_CR1_CEN;

    h->pid.integral = 0.0f;
    h->pid.prev_error = 0.0f;
    h->target_temp = 0.0f;
    h->enabled = 0;

    heater_set_duty(h, 0.0f);
}

void utms_heater_set_target(utms_heater_t *h, float temp) {
    h->target_temp = temp;
}

void utms_heater_update(utms_heater_t *h, float dt) {
    if (!h->enabled) {
        heater_set_duty(h, 0.0f);
        return;
    }

    float measured = utms_thermistor_read_celsius(h->thermistor);
    float error = h->target_temp - measured;

    h->pid.integral += error * dt;
    if (h->pid.integral > 1000.0f) h->pid.integral = 1000.0f;
    if (h->pid.integral < -1000.0f) h->pid.integral = -1000.0f;

    float derivative = (error - h->pid.prev_error) / dt;
    h->pid.prev_error = error;

    float output = h->pid.kp * error + h->pid.ki * h->pid.integral + h->pid.kd * derivative;

    if (output > h->pid.output_max) output = h->pid.output_max;
    if (output < h->pid.output_min) output = h->pid.output_min;

    heater_set_duty(h, output);
}

void utms_heater_enable(utms_heater_t *h, uint8_t enable) {
    h->enabled = enable;
    if (!enable) {
        heater_set_duty(h, 0.0f);
    }
}

void utms_heater_emergency_stop(utms_heater_t *h) {
    h->enabled = 0;
    heater_set_duty(h, 0.0f);
    h->pid.integral = 0.0f;
}
