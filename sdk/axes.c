#include <utms/sdk/axes.h>

static axis_t axes[3];

void axes_init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    axes[0].step.port = GPIOA;
    axes[0].step.pin = 0;
    axes[0].dir.port = GPIOA;
    axes[0].dir.pin = 1;
    axes[0].en.port = GPIOA;
    axes[0].en.pin = 2;
    axes[0].type = AXIS_TYPE_LINEAR;
    axes[0].steps_per_mm = 80;
    axes[0].max_speed = 1000;
    axes[0].acceleration = 100;

    axes[0].dir.port->MODER &= ~(3UL << (axes[0].dir.pin * 2));
    axes[0].dir.port->MODER |= (1UL << (axes[0].dir.pin * 2));

    axes[0].en.port->MODER &= ~(3UL << (axes[0].en.pin * 2));
    axes[0].en.port->MODER |= (1UL << (axes[0].en.pin * 2));
}

void axis_set_dir(uint32_t axis_id, uint8_t dir) {
    if (axis_id >= 3) return;
    if (dir) {
        axes[axis_id].dir.port->BSRR = (1UL << axes[axis_id].dir.pin);
    } else {
        axes[axis_id].dir.port->BSRR = (1UL << (axes[axis_id].dir.pin + 16));
    }
}

void axis_enable(uint32_t axis_id, uint8_t enable) {
    if (axis_id >= 3) return;
    if (enable) {
        axes[axis_id].en.port->BSRR = (1UL << (axes[axis_id].en.pin + 16));
    } else {
        axes[axis_id].en.port->BSRR = (1UL << axes[axis_id].en.pin);
    }
}
