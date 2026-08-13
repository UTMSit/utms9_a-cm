#include <utms/sdk/endstop.h>

void endstop_init(const endstop_t *es) {
    es->port->MODER &= ~(3UL << (es->pin * 2));
    es->port->PUPDR &= ~(3UL << (es->pin * 2));

    if (es->triggered_level) {
        es->port->PUPDR |= (2UL << (es->pin * 2));
    } else {
        es->port->PUPDR |= (1UL << (es->pin * 2));
    }
}

uint8_t endstop_is_triggered(const endstop_t *es) {
    uint8_t pin_state = (es->port->IDR >> es->pin) & 1UL;
    return (pin_state == es->triggered_level) ? 1 : 0;
}
