#ifndef UTMS_SCHEDULER_H
#define UTMS_SCHEDULER_H

#include <stdint.h>

typedef enum {
    TASK_STATE_READY = 1,
    TASK_STATE_BLOCKED = 2
} utms_task_state_t;

typedef struct utms_task {
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t sp;
    uint32_t lr;
    uint32_t exc_return;
    struct utms_task *next;
    uint8_t priority;
    uint8_t state;
    uint32_t wake_up_tick;
} utms_task_t;

extern utms_task_t *current_task;
extern utms_task_t *next_task;

void scheduler_init(void);
utms_task_t *scheduler_create_task(void (*entry)(void), uint32_t *stack_top, uint8_t priority);
void scheduler_start(void);
void scheduler_tick(void);
void scheduler_pick_next(void);
void delay_ms(uint32_t ms);

#endif
