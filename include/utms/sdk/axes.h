#ifndef UTMS_AXES_H
#define UTMS_AXES_H

#include <stdint.h>
#include <utms/hal/stm32f4xx.h>

typedef struct {
    GPIO_TypeDef *port;
    uint8_t pin;
} gpio_pin_t;

typedef enum {
    AXIS_TYPE_LINEAR = 0,
    AXIS_TYPE_ROTARY = 1,
    AXIS_TYPE_EXTRUDER = 2
} axis_type_t;

typedef struct {
    gpio_pin_t step;
    gpio_pin_t dir;
    gpio_pin_t en;
    axis_type_t type;
    uint32_t steps_per_mm;
    uint32_t max_speed;
    uint32_t acceleration;
} axis_t;

void axes_init(void);
void axis_set_dir(uint32_t axis_id, uint8_t dir);
void axis_enable(uint32_t axis_id, uint8_t enable);

#endif
