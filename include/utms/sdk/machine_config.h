#ifndef UTMS_MACHINE_CONFIG_H
#define UTMS_MACHINE_CONFIG_H

#include <stdint.h>

typedef enum {
    UTMS_MACHINE_CNC = 0,
    UTMS_MACHINE_PRINTER = 1,
    UTMS_MACHINE_LASER = 2,
    UTMS_MACHINE_CUSTOM = 3
} utms_machine_type_t;

typedef struct {
    utms_machine_type_t type;
    uint32_t axis_count;
    uint32_t heater_count;
    uint32_t spindle_enabled;
    uint32_t laser_enabled;
    uint32_t coolant_enabled;
} utms_machine_config_t;

void utms_machine_set_config(const utms_machine_config_t *cfg);
const utms_machine_config_t *utms_machine_get_config(void);
utms_machine_type_t utms_machine_get_type(void);

#endif
