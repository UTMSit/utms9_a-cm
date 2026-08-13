#ifndef UTMS_MOTION_PROFILE_H
#define UTMS_MOTION_PROFILE_H

#include <stdint.h>
#include <utms/sdk/planner.h>
#include <utms/hal/step_timer.h>

typedef struct {
    step_timer_t *timers[4];
    uint32_t *buffers[4];
    uint32_t buffer_len;
} motion_profile_t;

void motion_profile_generate(const planner_block_t *block, motion_profile_t *profile);

#endif
