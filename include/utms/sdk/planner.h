#ifndef UTMS_PLANNER_H
#define UTMS_PLANNER_H

#include <stdint.h>

#define PLANNER_BUFFER_SIZE 16

typedef struct {
    float x, y, z, e;
    float f;
    uint32_t steps_x, steps_y, steps_z, steps_e;
    float distance;
    float entry_speed;
    float exit_speed;
    float max_speed;
    float acceleration;
    uint8_t direction_x, direction_y, direction_z, direction_e;
} planner_block_t;

void planner_init(void);
int planner_add_block(const planner_block_t *block);
planner_block_t *planner_get_next_block(void);
void planner_recalculate(void);

#endif
