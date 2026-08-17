#include <stdint.h>
#include <utms/hal/stm32f4xx.h>
#include <utms/hal/uart.h>
#include <utms/hal/adc.h>
#include <utms/hal/step_timer.h>
#include <utms/hal/estop.h>
#include <utms/kernel/memory_pool.h>
#include <utms/sdk/machine_config.h>
#include <utms/sdk/thermistor.h>
#include <utms/sdk/heater.h>
#include <utms/sdk/spindle.h>
#include <utms/sdk/laser.h>
#include <utms/sdk/coolant.h>

UTMS_MEM_POOL_DEFINE(test_pool_small, 64, 32);
UTMS_MEM_POOL_DEFINE(test_pool_large, 256, 16);

static volatile uint16_t adc_samples[2];

static utms_adc_t test_adc = {
    .channel_map = { 0, 1, 0, 0, 0, 0, 0, 0 },
    .channel_count = 2,
    .buffer = adc_samples
};

static utms_thermistor_t bed_thermistor = {
    .adc = &test_adc,
    .channel_index = 0,
    .series_resistor = 4700.0f,
    .nominal_resistance = 100000.0f,
    .nominal_temp = 25.0f,
    .beta = 3950.0f,
    .vref = 3.3f
};

static utms_heater_t bed_heater = {
    .pwm_tim = TIM1,
    .pwm_channel = 1,
    .thermistor = &bed_thermistor,
    .pid = {
        .kp = 2.0f,
        .ki = 0.5f,
        .kd = 0.1f,
        .integral = 0.0f,
        .prev_error = 0.0f,
        .output_min = 0.0f,
        .output_max = 1.0f
    },
    .target_temp = 60.0f,
    .enabled = 0
};

static utms_spindle_t test_spindle = {
    .pwm_tim = TIM8,
    .pwm_channel = 1,
    .max_rpm = 24000,
    .current_rpm = 0,
    .enabled = 0
};

static utms_laser_t test_laser = {
    .pwm_tim = TIM8,
    .pwm_channel = 2,
    .max_power = 100.0f,
    .current_power = 0.0f,
    .enabled = 0
};

static utms_coolant_t test_coolant = {
    .port = GPIOD,
    .pin = 12,
    .active_level = 1,
    .enabled = 0
};

static step_timer_t sync_x = { .tim = TIM5, .dma_stream = DMA1_Stream2, .dma_channel = 6, .psc = 84 - 1, .arr = 1000, .ccr = 500, .role = STEP_TIMER_MASTER };
static step_timer_t sync_y = { .tim = TIM2, .dma_stream = DMA1_Stream1, .dma_channel = 3, .psc = 84 - 1, .arr = 1000, .ccr = 500, .role = STEP_TIMER_SLAVE };
static step_timer_t sync_z = { .tim = TIM3, .dma_stream = DMA1_Stream6, .dma_channel = 2, .psc = 84 - 1, .arr = 1000, .ccr = 500, .role = STEP_TIMER_SLAVE };
static step_timer_t sync_e = { .tim = TIM4, .dma_stream = DMA1_Stream2, .dma_channel = 5, .psc = 84 - 1, .arr = 1000, .ccr = 500, .role = STEP_TIMER_SLAVE };

static void report(const char *name, uint32_t value) {
    uart_puts(name);
    uart_puts(": ");
    uart_print_hex(value);
    uart_puts("\r\n");
}

static uint32_t float_bits(float v) {
    union {
        float f;
        uint32_t u;
    } x;
    x.f = v;
    return x.u;
}

static void stop_heater(void) {
    utms_heater_emergency_stop(&bed_heater);
}

static void stop_spindle(void) {
    utms_spindle_emergency_stop(&test_spindle);
}

static void stop_laser(void) {
    utms_laser_emergency_stop(&test_laser);
}

static void stop_coolant(void) {
    utms_coolant_emergency_stop(&test_coolant);
}

static void busy_delay(void) {
    for (volatile uint32_t d = 0; d < 200000; d++) {}
}

int main(void) {
    uart_init();
    uart_puts("UTMS9 TEST FIRMWARE START\r\n");

    report("MPU_CTRL", *(volatile uint32_t *)0xE000ED94UL);

    utms_machine_config_t cfg = {
        .type = UTMS_MACHINE_PRINTER,
        .axis_count = 4,
        .heater_count = 1,
        .spindle_enabled = 1,
        .laser_enabled = 1,
        .coolant_enabled = 1
    };
    utms_machine_set_config(&cfg);
    report("MACHINE_TYPE", (uint32_t)utms_machine_get_type());
    report("MACHINE_AXES", utms_machine_get_config()->axis_count);

    utms_mem_pool_init(&test_pool_small);
    utms_mem_pool_init(&test_pool_large);
    void *blk_a = utms_mem_alloc(&test_pool_small);
    void *blk_b = utms_mem_alloc(&test_pool_small);
    if (blk_a && blk_b) {
        *(uint32_t *)blk_a = 0xDEADBEEF;
        *(uint32_t *)blk_b = 0xCAFEBABE;
    }
    report("POOL_FREE_ALLOC", utms_mem_free_count(&test_pool_small));
    report("POOL_PATTERN_A", blk_a ? *(uint32_t *)blk_a : 0);
    report("POOL_PATTERN_B", blk_b ? *(uint32_t *)blk_b : 0);
    utms_mem_free(&test_pool_small, blk_a);
    report("POOL_FREE_AFTER", utms_mem_free_count(&test_pool_small));

    utms_adc_init(&test_adc);
    utms_adc_start(&test_adc);
    report("ADC_RAW_CH0", utms_adc_read(&test_adc, 0));
    report("TEMP_FLOAT_BITS", float_bits(utms_thermistor_read_celsius(&bed_thermistor)));

    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN | RCC_APB2ENR_TIM8EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM4EN | RCC_APB1ENR_TIM5EN;

    utms_heater_init(&bed_heater);
    utms_heater_set_target(&bed_heater, 60.0f);
    utms_heater_enable(&bed_heater, 1);
    for (uint32_t i = 0; i < 10; i++) {
        utms_heater_update(&bed_heater, 0.1f);
    }
    report("HEATER_CCR1", TIM1->CCR1);

    utms_spindle_init(&test_spindle);
    utms_spindle_enable(&test_spindle, 1);
    utms_spindle_set_rpm(&test_spindle, 12000);
    report("SPINDLE_CCR1", TIM8->CCR1);

    utms_laser_init(&test_laser);
    utms_laser_enable(&test_laser, 1);
    utms_laser_set_power(&test_laser, 50.0f);
    report("LASER_CCR2", TIM8->CCR2);

    utms_coolant_init(&test_coolant);
    utms_coolant_enable(&test_coolant, 1);
    report("COOLANT_ODR", GPIOD->ODR);

    utms_estop_register_callback(stop_heater);
    utms_estop_register_callback(stop_spindle);
    utms_estop_register_callback(stop_laser);
    utms_estop_register_callback(stop_coolant);
    utms_estop_trigger();
    report("ESTOP_ACTIVE", utms_estop_is_active());
    report("ESTOP_HEATER_CCR1", TIM1->CCR1);
    report("ESTOP_SPINDLE_CCR1", TIM8->CCR1);
    report("ESTOP_COOLANT_ODR", GPIOD->ODR);
    utms_estop_clear();

    step_timer_init(&sync_x);
    step_timer_init(&sync_y);
    step_timer_init(&sync_z);
    step_timer_init(&sync_e);
    step_timer_t *sync_timers[4] = { &sync_x, &sync_y, &sync_z, &sync_e };
    step_timer_sync_start(sync_timers, 4);
    report("SYNC_MASTER_CR1", TIM5->CR1);
    report("SYNC_SLAVE2_CR1", TIM2->CR1);
    report("SYNC_SLAVE3_CR1", TIM3->CR1);
    report("SYNC_SLAVE4_CR1", TIM4->CR1);

    uart_puts("ALL TESTS COMPLETE\r\n");

    while (1) {
        GPIOD->ODR ^= (1UL << 13);
        busy_delay();
    }

    return 0;
}
