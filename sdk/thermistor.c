#include <utms/sdk/thermistor.h>

static float fast_logf(float x) {
    if (x <= 0.0f) return -1e38f;

    union { float f; uint32_t i; } vx;
    vx.f = x;
    uint32_t exp = (vx.i >> 23) & 0xFF;
    vx.i = (vx.i & 0x007FFFFF) | 0x3F800000;
    float y = vx.f - 1.0f;

    float log2_m = y * (1.0f + y * (-0.5f + y * (0.3333333f + y * (-0.25f + y * 0.2f))));
    return 0.6931471805599453f * ((float)(exp - 127) + log2_m);
}

void utms_thermistor_init(utms_thermistor_t *th) {
    (void)th;
}

float utms_thermistor_read_celsius(utms_thermistor_t *th) {
    uint16_t raw = utms_adc_read(th->adc, th->channel_index);
    if (raw == 0 || raw >= 4095) return -273.15f;

    float voltage = ((float)raw / 4095.0f) * th->vref;
    float resistance = th->series_resistor * voltage / (th->vref - voltage);

    float steinhart = resistance / th->nominal_resistance;
    steinhart = fast_logf(steinhart);
    steinhart /= th->beta;
    steinhart += 1.0f / (th->nominal_temp + 273.15f);
    steinhart = 1.0f / steinhart;
    steinhart -= 273.15f;

    return steinhart;
}
