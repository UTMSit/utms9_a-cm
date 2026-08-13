#include <utms/sdk/coolant.h>

void utms_coolant_init(utms_coolant_t *c) {
    c->port->MODER &= ~(3UL << (c->pin * 2));
    c->port->MODER |= (1UL << (c->pin * 2));
    c->enabled = 0;

    if (c->active_level) {
        c->port->BSRR = (1UL << (c->pin + 16));
    } else {
        c->port->BSRR = (1UL << c->pin);
    }
}

void utms_coolant_enable(utms_coolant_t *c, uint8_t enable) {
    c->enabled = enable;
    if (enable) {
        if (c->active_level) {
            c->port->BSRR = (1UL << c->pin);
        } else {
            c->port->BSRR = (1UL << (c->pin + 16));
        }
    } else {
        if (c->active_level) {
            c->port->BSRR = (1UL << (c->pin + 16));
        } else {
            c->port->BSRR = (1UL << c->pin);
        }
    }
}

void utms_coolant_emergency_stop(utms_coolant_t *c) {
    c->enabled = 0;
    if (c->active_level) {
        c->port->BSRR = (1UL << (c->pin + 16));
    } else {
        c->port->BSRR = (1UL << c->pin);
    }
}
