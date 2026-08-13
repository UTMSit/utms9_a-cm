#include <utms/sdk/motion_profile.h>
#include <utms/sdk/planner.h>
#include <utms/hal/step_timer.h>

void motion_profile_generate(const planner_block_t *block, motion_profile_t *profile) {
    float entry_speed = block->entry_speed;
    float exit_speed = block->exit_speed;
    float max_speed = block->max_speed;
    float acceleration = block->acceleration;

    uint32_t steps[4] = {
        block->steps_x,
        block->steps_y,
        block->steps_z,
        block->steps_e
    };

    uint32_t total_steps = 0;
    for (uint32_t i = 0; i < 4; i++) {
        if (steps[i] > total_steps) {
            total_steps = steps[i];
        }
    }

    if (total_steps == 0) return;

    uint32_t steps_accel = (uint32_t)((max_speed * max_speed - entry_speed * entry_speed) / (2.0f * acceleration));
    uint32_t steps_decel = (uint32_t)((max_speed * max_speed - exit_speed * exit_speed) / (2.0f * acceleration));

    if (steps_accel + steps_decel > total_steps) {
        steps_accel = total_steps / 2;
        steps_decel = total_steps / 2;
    }

    uint32_t steps_cruise = total_steps - steps_accel - steps_decel;

    uint32_t timer_freq = 1000000;

    for (uint32_t axis = 0; axis < 4; axis++) {
        if (!profile->timers[axis] || !profile->buffers[axis]) continue;

        uint32_t idx = 0;
        float axis_ratio = (float)steps[axis] / (float)total_steps;

        for (uint32_t i = 0; i < steps_accel && idx < profile->buffer_len; i++) {
            float v = entry_speed + acceleration * (float)i / (float)total_steps;
            uint32_t freq = (uint32_t)(v * axis_ratio * 80.0f);
            if (freq > 0) {
                profile->buffers[axis][idx++] = timer_freq / freq;
            }
        }

        for (uint32_t i = 0; i < steps_cruise && idx < profile->buffer_len; i++) {
            uint32_t freq = (uint32_t)(max_speed * axis_ratio * 80.0f);
            if (freq > 0) {
                profile->buffers[axis][idx++] = timer_freq / freq;
            }
        }

        for (uint32_t i = 0; i < steps_decel && idx < profile->buffer_len; i++) {
            float v = max_speed - acceleration * (float)i / (float)total_steps;
            uint32_t freq = (uint32_t)(v * axis_ratio * 80.0f);
            if (freq > 0) {
                profile->buffers[axis][idx++] = timer_freq / freq;
            }
        }

        step_timer_set_buffer(profile->timers[axis], profile->buffers[axis], idx);
    }
}
