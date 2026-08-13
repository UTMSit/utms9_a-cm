#ifndef UTMS_COOLANT_H
#define UTMS_COOLANT_H

#include <stdint.h>
#include <utms/hal/stm32f4xx.h>

typedef struct {
    GPIO_TypeDef *port;
    uint8_t pin;
    uint8_t active_level;
    uint8_t enabled;
} utms_coolant_t;

void utms_coolant_init(utms_coolant_t *c);
void utms_coolant_enable(utms_coolant_t *c, uint8_t enable);
void utms_coolant_emergency_stop(utms_coolant_t *c);

#endif
