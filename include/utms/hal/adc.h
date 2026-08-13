#ifndef UTMS_ADC_H
#define UTMS_ADC_H

#include <stdint.h>

#define UTMS_ADC_MAX_CHANNELS 8

typedef struct {
    uint32_t channel_map[UTMS_ADC_MAX_CHANNELS];
    uint32_t channel_count;
    volatile uint16_t *buffer;
} utms_adc_t;

void utms_adc_init(utms_adc_t *adc);
uint16_t utms_adc_read(utms_adc_t *adc, uint32_t channel_index);
void utms_adc_start(utms_adc_t *adc);

#endif
