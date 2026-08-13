#ifndef UTMS_MOTION_PROFILE_H
#define UTMS_MOTION_PROFILE_H

#include <stdint.h>
#include <utms/sdk/planner.h>
#include <utms/hal/step_timer.h>

void motion_profile_generate(const planner_block_t *block, uint32_t *arr_buffer, uint32_t buffer_len, step_timer_t *timer);

#endif
