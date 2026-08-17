#include <utms/sdk/laser.h>

static void laser_set_duty(utms_laser_t *l, float duty) {
    if (duty < 0.0f) duty = 0.0f;
    if (duty > 1.0f) duty = 1.0f;

    uint32_t arr = l->pwm_tim->ARR;
    uint32_t ccr = (uint32_t)(duty * (float)arr);

    switch (l->pwm_channel) {
        case 1: l->pwm_tim->CCR1 = ccr; break;
        case 2: l->pwm_tim->CCR2 = ccr; break;
        case 3: l->pwm_tim->CCR3 = ccr; break;
        case 4: l->pwm_tim->CCR4 = ccr; break;
    }
}

static void laser_configure_channel(utms_laser_t *l) {
    switch (l->pwm_channel) {
        case 1:
            l->pwm_tim->CCMR1 |= TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE;
            l->pwm_tim->CCER |= TIM_CCER_CC1E;
            break;
        case 2:
            l->pwm_tim->CCMR1 |= TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_OC2PE;
            l->pwm_tim->CCER |= TIM_CCER_CC2E;
            break;
        case 3:
            l->pwm_tim->CCMR2 |= TIM_CCMR2_OC3M_PWM1 | TIM_CCMR2_OC3PE;
            l->pwm_tim->CCER |= TIM_CCER_CC3E;
            break;
        case 4:
            l->pwm_tim->CCMR2 |= TIM_CCMR2_OC4M_PWM1 | TIM_CCMR2_OC4PE;
            l->pwm_tim->CCER |= TIM_CCER_CC4E;
            break;
    }
}

void utms_laser_init(utms_laser_t *l) {
    l->pwm_tim->PSC = 84 - 1;
    l->pwm_tim->ARR = 1000;
    laser_configure_channel(l);
    l->pwm_tim->CR1 = TIM_CR1_CEN;
    l->current_power = 0.0f;
    l->enabled = 0;
    laser_set_duty(l, 0.0f);
}

void utms_laser_set_power(utms_laser_t *l, float power) {
    if (!l->enabled) return;
    if (power < 0.0f) power = 0.0f;
    if (power > l->max_power) power = l->max_power;
    l->current_power = power;
    laser_set_duty(l, power / l->max_power);
}

void utms_laser_enable(utms_laser_t *l, uint8_t enable) {
    l->enabled = enable;
    if (!enable) {
        l->current_power = 0.0f;
        laser_set_duty(l, 0.0f);
    }
}

void utms_laser_emergency_stop(utms_laser_t *l) {
    l->enabled = 0;
    l->current_power = 0.0f;
    laser_set_duty(l, 0.0f);
}
