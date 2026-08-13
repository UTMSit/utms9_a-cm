#ifndef UTMS_ENDSTOP_H
#define UTMS_ENDSTOP_H

#include <stdint.h>
#include <utms/hal/stm32f4xx.h>

typedef struct {
    GPIO_TypeDef *port;
    uint8_t pin;
    uint8_t triggered_level;
} endstop_t;

void endstop_init(const endstop_t *es);
uint8_t endstop_is_triggered(const endstop_t *es);

#endif
