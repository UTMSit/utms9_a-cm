#include <utms/kernel/scheduler.h>
#include <utms/hal/stm32f4xx.h>

#define ENTER_CRITICAL() __asm__ volatile ("cpsid i" ::: "memory")
#define EXIT_CRITICAL()  __asm__ volatile ("cpsie i" ::: "memory")

volatile uint32_t system_ticks = 0;

utms_task_t *current_task = 0;
utms_task_t *next_task = 0;
static utms_task_t *task_list_head = 0;

static void task_exit(void) {
    while (1) {}
}

void scheduler_init(void) {
    SYSTICK->LOAD = 168000 - 1;
    SYSTICK->VAL = 0;
    SYSTICK->CTRL = SYSTICK_CTRL_ENABLE | SYSTICK_CTRL_TICKINT | SYSTICK_CTRL_CLKSOURCE;
}

utms_task_t *scheduler_create_task(void (*entry)(void), uint32_t *stack_top, uint8_t priority) {
    uint32_t *sp = stack_top;
    sp -= 16;

    sp[0] = 0x01000000;
    sp[1] = (uint32_t)entry;
    sp[2] = (uint32_t)task_exit;
    sp[3] = 0;
    sp[4] = 0;
    sp[5] = 0;
    sp[6] = 0;
    sp[7] = 0;
    sp[8] = 0;
    sp[9] = 0;
    sp[10] = 0;
    sp[11] = 0;
    sp[12] = 0;
    sp[13] = 0;
    sp[14] = 0;
    sp[15] = 0;

    uint32_t *task_mem = (uint32_t *)((uint32_t)sp - sizeof(utms_task_t));
    utms_task_t *t = (utms_task_t *)task_mem;

    t->r4 = 0;
    t->r5 = 0;
    t->r6 = 0;
    t->r7 = 0;
    t->r8 = 0;
    t->r9 = 0;
    t->r10 = 0;
    t->r11 = 0;
    t->sp = (uint32_t)sp;
    t->lr = 0xFFFFFFFD;
    t->exc_return = 0xFFFFFFFD;
    t->priority = priority;
    t->state = TASK_STATE_READY;
    t->wake_up_tick = 0;
    t->next = 0;

    ENTER_CRITICAL();
    if (!task_list_head) {
        task_list_head = t;
        task_list_head->next = t;
    } else {
        utms_task_t *p = task_list_head;
        while (p->next != task_list_head) {
            p = p->next;
        }
        p->next = t;
        t->next = task_list_head;
    }
    EXIT_CRITICAL();

    return t;
}

void scheduler_start(void) {
    if (task_list_head) {
        current_task = task_list_head;
        next_task = task_list_head;
    }
}

void scheduler_pick_next(void) {
    ENTER_CRITICAL();

    utms_task_t *best_task = 0;
    utms_task_t *p = task_list_head;

    do {
        if (p->state == TASK_STATE_READY) {
            if (!best_task || p->priority > best_task->priority) {
                best_task = p;
            }
        }
        p = p->next;
    } while (p != task_list_head);

    if (!best_task) {
        best_task = current_task;
    }

    if (best_task->priority == current_task->priority && best_task == current_task) {
        best_task = current_task->next;
        while (best_task != current_task) {
            if (best_task->state == TASK_STATE_READY && best_task->priority == current_task->priority) {
                break;
            }
            best_task = best_task->next;
        }
    }

    next_task = best_task;
    EXIT_CRITICAL();
}

void scheduler_tick(void) {
    system_ticks++;
    if (!current_task) return;

    ENTER_CRITICAL();

    utms_task_t *p = task_list_head;
    do {
        if (p->state == TASK_STATE_BLOCKED && system_ticks >= p->wake_up_tick) {
            p->state = TASK_STATE_READY;
        }
        p = p->next;
    } while (p != task_list_head);

    EXIT_CRITICAL();

    scheduler_pick_next();

    if (next_task != current_task) {
        SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
    }
}

void delay_ms(uint32_t ms) {
    ENTER_CRITICAL();
    current_task->wake_up_tick = system_ticks + ms;
    current_task->state = TASK_STATE_BLOCKED;
    EXIT_CRITICAL();

    scheduler_pick_next();

    if (next_task != current_task) {
        __asm__ volatile ("bl yield");
    }
}
