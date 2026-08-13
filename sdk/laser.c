#include <utms/sdk/laser.h>

#define TIM_CCMR1_OC1M_PWM1 (6UL << 4)
#define TIM_CCMR1_OC1PE     (1UL << 3)
#define TIM_CCER_CC1E       (1UL << 0)
#define TIM_CR1_CEN         (1UL << 0)

void utms_laser_init(utms_laser_t *l) {
    l->pwm_tim->PSC = 84 - 1;
    l->pwm_tim->ARR = 1000;
    l->pwm_tim->CCR1 = 0;
    l->pwm_tim->CCMR1 = TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE;
    l->pwm_tim->CCER = TIM_CCER_CC1E;
    l->pwm_tim->CR1 = TIM_CR1_CEN;
    l->current_power = 0.0f;
    l->enabled = 0;
}

void utms_laser_set_power(utms_laser_t *l, float power) {
    if (!l->enabled) return;
    if (power < 0.0f) power = 0.0f;
    if (power > l->max_power) power = l->max_power;
    l->current_power = power;

    float duty = power / l->max_power;
    uint32_t arr = l->pwm_tim->ARR;
    l->pwm_tim->CCR1 = (uint32_t)(duty * (float)arr);
}

void utms_laser_enable(utms_laser_t *l, uint8_t enable) {
    l->enabled = enable;
    if (!enable) {
        l->pwm_tim->CCR1 = 0;
        l->current_power = 0.0f;
    }
}

void utms_laser_emergency_stop(utms_laser_t *l) {
    l->enabled = 0;
    l->pwm_tim->CCR1 = 0;
    l->current_power = 0.0f;
}
