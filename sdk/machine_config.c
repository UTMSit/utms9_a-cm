#include <utms/sdk/machine_config.h>

static utms_machine_config_t current_config = {
    .type = UTMS_MACHINE_CUSTOM,
    .axis_count = 3,
    .heater_count = 0,
    .spindle_enabled = 0,
    .laser_enabled = 0,
    .coolant_enabled = 0
};

void utms_machine_set_config(const utms_machine_config_t *cfg) {
    current_config = *cfg;
}

const utms_machine_config_t *utms_machine_get_config(void) {
    return &current_config;
}

utms_machine_type_t utms_machine_get_type(void) {
    return current_config.type;
}
