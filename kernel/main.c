#include <stdint.h>
#include <utms/hal/stm32f4xx.h>
#include <utms/kernel/scheduler.h>
#include <utms/hal/uart.h>
#include <utms/hal/step_timer.h>
#include <utms/sdk/axes.h>
#include <utms/sdk/gcode.h>
#include <utms/sdk/planner.h>
#include <utms/sdk/kinematics.h>
#include <utms/sdk/motion_profile.h>

static uint8_t stack1[1024] __attribute__((aligned(8)));
static uint8_t stack2[1024] __attribute__((aligned(8)));
static uint8_t stack3[1024] __attribute__((aligned(8)));
static uint8_t stack4[1024] __attribute__((aligned(8)));
static uint32_t step_profile_x[256];
static uint32_t step_profile_y[256];
static uint32_t step_profile_z[256];
static uint32_t step_profile_e[256];
static char gcode_line[128];

static step_timer_t timer_x = {
    .tim = TIM5,
    .dma_stream = DMA1_Stream2,
    .dma_channel = 6,
    .psc = 84 - 1,
    .arr = 1000,
    .ccr = 500
};

static step_timer_t timer_y = {
    .tim = TIM2,
    .dma_stream = DMA1_Stream1,
    .dma_channel = 3,
    .psc = 84 - 1,
    .arr = 1000,
    .ccr = 500
};

static step_timer_t timer_z = {
    .tim = TIM3,
    .dma_stream = DMA1_Stream2,
    .dma_channel = 5,
    .psc = 84 - 1,
    .arr = 1000,
    .ccr = 500
};

static step_timer_t timer_e = {
    .tim = TIM4,
    .dma_stream = DMA1_Stream6,
    .dma_channel = 2,
    .psc = 84 - 1,
    .arr = 1000,
    .ccr = 500
};

static void task1(void) {
    while (1) {
        GPIOD->ODR ^= (1UL << 12);
        delay_ms(500);
    }
}

static void task2(void) {
    while (1) {
        GPIOD->ODR ^= (1UL << 13);
        delay_ms(250);
    }
}

static void task3(void) {
    planner_init();

    while (1) {
        planner_block_t *next = planner_get_next_block();
        if (next) {
            uart_puts("Executing block\r\n");
            motion_profile_generate(next, step_profile_x, 256, &timer_x);
        }
        delay_ms(10);
    }
}

static void task4(void) {
    uint32_t idx = 0;

    while (1) {
        if (uart_has_data()) {
            char c = uart_getc();

            if (c == '\n' || c == '\r') {
                if (idx > 0) {
                    gcode_line[idx] = '\0';
                    idx = 0;

                    gcode_cmd_t cmd;
                    gcode_parse(gcode_line, &cmd);

                    if (cmd.cmd == 0 || cmd.cmd == 1) {
                        planner_block_t block;
                        block.x = cmd.x;
                        block.y = cmd.y;
                        block.z = cmd.z;
                        block.e = cmd.e;
                        block.f = cmd.f;
                        block.steps_x = (uint32_t)(cmd.x * 80.0f);
                        block.steps_y = (uint32_t)(cmd.y * 80.0f);
                        block.steps_z = (uint32_t)(cmd.z * 80.0f);
                        block.steps_e = (uint32_t)(cmd.e * 80.0f);
                        block.distance = 10.0f;
                        block.entry_speed = 0.0f;
                        block.exit_speed = 0.0f;
                        block.max_speed = cmd.f / 60.0f;
                        block.acceleration = 100.0f;
                        block.direction_x = 1;
                        block.direction_y = 1;
                        block.direction_z = 1;
                        block.direction_e = 1;

                        planner_add_block(&block);
                        planner_recalculate();

                        uart_puts("G-code block added\r\n");
                    }
                }
            } else {
                if (idx < sizeof(gcode_line) - 1) {
                    gcode_line[idx++] = c;
                }
            }
        }
        delay_ms(1);
    }
}

void SystemInit(void);
void start_first_task(void);

int main(void) {
    GPIOD->MODER &= ~(3UL << (12 * 2));
    GPIOD->MODER |= (1UL << (12 * 2));

    GPIOD->MODER &= ~(3UL << (13 * 2));
    GPIOD->MODER |= (1UL << (13 * 2));

    GPIOD->MODER &= ~(3UL << (14 * 2));
    GPIOD->MODER |= (1UL << (14 * 2));

    GPIOD->MODER &= ~(3UL << (15 * 2));
    GPIOD->MODER |= (1UL << (15 * 2));

    uart_init();
    uart_puts("UTMS9_a+cm: Kernel started\r\n");

    axes_init();
    axis_enable(0, 1);
    axis_set_dir(0, 1);

    uart_puts("UTMS9_a+cm: Axes initialized\r\n");

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM4EN;

    step_timer_init(&timer_x);
    step_timer_dma_init(&timer_x);

    step_timer_init(&timer_y);
    step_timer_dma_init(&timer_y);

    step_timer_init(&timer_z);
    step_timer_dma_init(&timer_z);

    step_timer_init(&timer_e);
    step_timer_dma_init(&timer_e);

    uart_puts("UTMS9_a+cm: All STEP timers with DMA started\r\n");

    scheduler_init();

    scheduler_create_task(task1, (uint32_t *)(stack1 + sizeof(stack1)), 1);
    scheduler_create_task(task2, (uint32_t *)(stack2 + sizeof(stack2)), 1);
    scheduler_create_task(task3, (uint32_t *)(stack3 + sizeof(stack3)), 2);
    scheduler_create_task(task4, (uint32_t *)(stack4 + sizeof(stack4)), 3);

    scheduler_start();

    uart_puts("UTMS9_a+cm: Scheduler started\r\n");

    start_first_task();

    return 0;
}
