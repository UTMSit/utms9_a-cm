#include <utms/sdk/planner.h>
#include <utms/kernel/scheduler.h>

static planner_block_t buffer[PLANNER_BUFFER_SIZE];
static uint32_t head = 0;
static uint32_t tail = 0;
static uint32_t count = 0;

static float fast_sqrtf(float x) {
    float result;
    __asm__ volatile (
        "vsqrt.f32 %0, %1"
        : "=t" (result)
        : "t" (x)
    );
    return result;
}

void planner_init(void) {
    head = 0;
    tail = 0;
    count = 0;
}

int planner_add_block(const planner_block_t *block) {
    if (count >= PLANNER_BUFFER_SIZE) {
        return -1;
    }

    buffer[head] = *block;
    head = (head + 1) % PLANNER_BUFFER_SIZE;
    count++;

    return 0;
}

planner_block_t *planner_get_next_block(void) {
    if (count == 0) {
        return 0;
    }

    planner_block_t *block = &buffer[tail];
    tail = (tail + 1) % PLANNER_BUFFER_SIZE;
    count--;

    return block;
}

void planner_recalculate(void) {
    if (count == 0) return;

    uint32_t idx = (head + PLANNER_BUFFER_SIZE - 1) % PLANNER_BUFFER_SIZE;

    buffer[idx].exit_speed = 0.0f;

    for (uint32_t i = 0; i < count; i++) {
        planner_block_t *current = &buffer[idx];

        float v_entry_sq = current->exit_speed * current->exit_speed +
                           2.0f * current->acceleration * current->distance;
        float v_entry = fast_sqrtf(v_entry_sq);

        if (v_entry > current->max_speed) {
            v_entry = current->max_speed;
        }

        current->entry_speed = v_entry;

        idx = (idx + PLANNER_BUFFER_SIZE - 1) % PLANNER_BUFFER_SIZE;

        if (count > 1) {
            buffer[idx].exit_speed = v_entry;
        }
    }
}
