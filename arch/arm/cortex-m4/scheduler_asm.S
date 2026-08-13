.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

.global SysTick_Handler
.global PendSV_Handler
.global start_first_task
.global yield

.section .text.start_first_task
.type start_first_task, %function
start_first_task:
    ldr r0, =current_task
    ldr r0, [r0]
    ldr r1, [r0, #36]
    mov lr, #0xFFFFFFFD
    ldmia r1!, {r4-r11}
    msr psp, r1
    bx lr

.section .text.SysTick_Handler
.type SysTick_Handler, %function
SysTick_Handler:
    push {lr}
    bl scheduler_tick
    pop {lr}
    bx lr

.section .text.PendSV_Handler
.type PendSV_Handler, %function
PendSV_Handler:
    mrs r0, psp
    stmdb r0!, {r4-r11}

    ldr r1, =current_task
    ldr r1, [r1]
    str r0, [r1, #36]

    ldr r2, =next_task
    ldr r2, [r2]
    str r2, [r1]

    ldr r0, [r2, #36]
    ldmia r0!, {r4-r11}
    msr psp, r0
    bx lr

.section .text.yield
.type yield, %function
yield:
    mrs r0, psp
    stmdb r0!, {r4-r11}

    ldr r1, =current_task
    ldr r1, [r1]
    str r0, [r1, #36]

    ldr r0, =next_task
    ldr r0, [r0]
    ldr r0, [r0, #36]

    ldmia r0!, {r4-r11}
    msr psp, r0
    bx lr
