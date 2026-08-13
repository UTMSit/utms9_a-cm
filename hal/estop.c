#include <utms/hal/estop.h>

#define UTMS_ESTOP_MAX_CALLBACKS 16

static utms_estop_callback_t estop_callbacks[UTMS_ESTOP_MAX_CALLBACKS];
static uint32_t estop_callback_count = 0;
static volatile uint8_t estop_active = 0;

void utms_estop_init(void) {
    estop_callback_count = 0;
    estop_active = 0;
}

void utms_estop_register_callback(utms_estop_callback_t cb) {
    if (estop_callback_count >= UTMS_ESTOP_MAX_CALLBACKS) return;
    estop_callbacks[estop_callback_count++] = cb;
}

void utms_estop_trigger(void) {
    estop_active = 1;
    for (uint32_t i = 0; i < estop_callback_count; i++) {
        if (estop_callbacks[i]) {
            estop_callbacks[i]();
        }
    }
}

uint8_t utms_estop_is_active(void) {
    return estop_active;
}

void utms_estop_clear(void) {
    estop_active = 0;
}
