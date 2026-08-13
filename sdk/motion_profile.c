#include <utms/sdk/motion_profile.h>
#include <utms/sdk/planner.h>
#include <utms/hal/step_timer.h>

void motion_profile_generate(const planner_block_t *block, uint32_t *arr_buffer, uint32_t buffer_len, step_timer_t *timer) {
    float entry_speed = block->entry_speed;
    float exit_speed = block->exit_speed;
    float max_speed = block->max_speed;
    float acceleration = block->acceleration;
    float distance = block->distance;

    uint32_t steps_accel = (uint32_t)((max_speed * max_speed - entry_speed * entry_speed) / (2.0f * acceleration));
    uint32_t steps_decel = (uint32_t)((max_speed * max_speed - exit_speed * exit_speed) / (2.0f * acceleration));
    uint32_t total_steps = (uint32_t)(distance * 80.0f);

    if (steps_accel + steps_decel > total_steps) {
        steps_accel = total_steps / 2;
        steps_decel = total_steps / 2;
    }

    uint32_t steps_cruise = total_steps - steps_accel - steps_decel;

    uint32_t timer_freq = 1000000;
    uint32_t idx = 0;

    for (uint32_t i = 0; i < steps_accel && idx < buffer_len; i++) {
        float v = entry_speed + acceleration * (float)i / 80.0f;
        arr_buffer[idx++] = timer_freq / (uint32_t)(v * 80.0f);
    }

    for (uint32_t i = 0; i < steps_cruise && idx < buffer_len; i++) {
        arr_buffer[idx++] = timer_freq / (uint32_t)(max_speed * 80.0f);
    }

    for (uint32_t i = 0; i < steps_decel && idx < buffer_len; i++) {
        float v = max_speed - acceleration * (float)i / 80.0f;
        arr_buffer[idx++] = timer_freq / (uint32_t)(v * 80.0f);
    }

    step_timer_set_buffer(timer, arr_buffer, idx);
}
