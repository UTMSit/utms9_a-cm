#include <utms/sdk/spindle.h>

#define TIM_CCMR1_OC1M_PWM1 (6UL << 4)
#define TIM_CCMR1_OC1PE     (1UL << 3)
#define TIM_CCER_CC1E       (1UL << 0)
#define TIM_CR1_CEN         (1UL << 0)

void utms_spindle_init(utms_spindle_t *sp) {
    sp->pwm_tim->PSC = 84 - 1;
    sp->pwm_tim->ARR = 1000;
    sp->pwm_tim->CCR1 = 0;
    sp->pwm_tim->CCMR1 = TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE;
    sp->pwm_tim->CCER = TIM_CCER_CC1E;
    sp->pwm_tim->CR1 = TIM_CR1_CEN;
    sp->current_rpm = 0;
    sp->enabled = 0;
}

void utms_spindle_set_rpm(utms_spindle_t *sp, uint32_t rpm) {
    if (!sp->enabled) return;
    if (rpm > sp->max_rpm) rpm = sp->max_rpm;
    sp->current_rpm = rpm;

    float duty = (float)rpm / (float)sp->max_rpm;
    uint32_t arr = sp->pwm_tim->ARR;
    sp->pwm_tim->CCR1 = (uint32_t)(duty * (float)arr);
}

void utms_spindle_enable(utms_spindle_t *sp, uint8_t enable) {
    sp->enabled = enable;
    if (!enable) {
        sp->pwm_tim->CCR1 = 0;
        sp->current_rpm = 0;
    }
}

void utms_spindle_emergency_stop(utms_spindle_t *sp) {
    sp->enabled = 0;
    sp->pwm_tim->CCR1 = 0;
    sp->current_rpm = 0;
}
