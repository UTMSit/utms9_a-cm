#ifndef UTMS_THERMISTOR_H
#define UTMS_THERMISTOR_H

#include <stdint.h>
#include <utms/hal/adc.h>

typedef struct {
    utms_adc_t *adc;
    uint32_t channel_index;
    float series_resistor;
    float nominal_resistance;
    float nominal_temp;
    float beta;
    float vref;
} utms_thermistor_t;

void utms_thermistor_init(utms_thermistor_t *th);
float utms_thermistor_read_celsius(utms_thermistor_t *th);

#endif
