#include <utms/sdk/spindle.h>

static void spindle_set_duty(utms_spindle_t *sp, float duty) {
    if (duty < 0.0f) duty = 0.0f;
    if (duty > 1.0f) duty = 1.0f;

    uint32_t arr = sp->pwm_tim->ARR;
    uint32_t ccr = (uint32_t)(duty * (float)arr);

    switch (sp->pwm_channel) {
        case 1: sp->pwm_tim->CCR1 = ccr; break;
        case 2: sp->pwm_tim->CCR2 = ccr; break;
        case 3: sp->pwm_tim->CCR3 = ccr; break;
        case 4: sp->pwm_tim->CCR4 = ccr; break;
    }
}

static void spindle_configure_channel(utms_spindle_t *sp) {
    switch (sp->pwm_channel) {
        case 1:
            sp->pwm_tim->CCMR1 |= TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE;
            sp->pwm_tim->CCER |= TIM_CCER_CC1E;
            break;
        case 2:
            sp->pwm_tim->CCMR1 |= TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_OC2PE;
            sp->pwm_tim->CCER |= TIM_CCER_CC2E;
            break;
        case 3:
            sp->pwm_tim->CCMR2 |= TIM_CCMR2_OC3M_PWM1 | TIM_CCMR2_OC3PE;
            sp->pwm_tim->CCER |= TIM_CCER_CC3E;
            break;
        case 4:
            sp->pwm_tim->CCMR2 |= TIM_CCMR2_OC4M_PWM1 | TIM_CCMR2_OC4PE;
            sp->pwm_tim->CCER |= TIM_CCER_CC4E;
            break;
    }
}

void utms_spindle_init(utms_spindle_t *sp) {
    sp->pwm_tim->PSC = 84 - 1;
    sp->pwm_tim->ARR = 1000;
    spindle_configure_channel(sp);
    sp->pwm_tim->CR1 = TIM_CR1_CEN;
    sp->current_rpm = 0;
    sp->enabled = 0;
    spindle_set_duty(sp, 0.0f);
}

void utms_spindle_set_rpm(utms_spindle_t *sp, uint32_t rpm) {
    if (!sp->enabled) return;
    if (rpm > sp->max_rpm) rpm = sp->max_rpm;
    sp->current_rpm = rpm;
    spindle_set_duty(sp, (float)rpm / (float)sp->max_rpm);
}

void utms_spindle_enable(utms_spindle_t *sp, uint8_t enable) {
    sp->enabled = enable;
    if (!enable) {
        sp->current_rpm = 0;
        spindle_set_duty(sp, 0.0f);
    }
}

void utms_spindle_emergency_stop(utms_spindle_t *sp) {
    sp->enabled = 0;
    sp->current_rpm = 0;
    spindle_set_duty(sp, 0.0f);
}
