#ifndef UTMS_ESTOP_H
#define UTMS_ESTOP_H

#include <stdint.h>

typedef void (*utms_estop_callback_t)(void);

void utms_estop_init(void);
void utms_estop_register_callback(utms_estop_callback_t cb);
void utms_estop_trigger(void);
uint8_t utms_estop_is_active(void);
void utms_estop_clear(void);

#endif
