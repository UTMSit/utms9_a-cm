.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

.global Reset_Handler
.global Default_Handler
.global HardFault_Handler
.global MemManage_Handler
.global SysTick_Handler
.global PendSV_Handler
.global DMA1_Stream5_IRQHandler

.section .isr_vector, "a", %progbits
.type vector_table, %object
.size vector_table, .-vector_table

vector_table:
    .word _estack
    .word Reset_Handler
    .word Default_Handler
    .word HardFault_Handler
    .word MemManage_Handler
    .word Default_Handler
    .word Default_Handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word Default_Handler
    .word Default_Handler
    .word 0
    .word SysTick_Handler
    .word PendSV_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word DMA1_Stream5_IRQHandler

.section .text.Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
copy_data:
    cmp r0, r1
    beq bss_loop
    ldr.w r3, [r2], #4
    str.w r3, [r0], #4
    b copy_data

bss_loop:
    ldr r0, =_sbss
    ldr r1, =_ebss
    mov r2, #0
zero_bss:
    cmp r0, r1
    beq main_call
    str.w r2, [r0], #4
    b zero_bss

main_call:
    bl SystemInit
    bl main
    b .

.section .text.Default_Handler
.type Default_Handler, %function
Default_Handler:
    b .

.section .text.HardFault_Handler
.type HardFault_Handler, %function
HardFault_Handler:
    b .

.section .text.MemManage_Handler
.type MemManage_Handler, %function
MemManage_Handler:
    cpsid i
    bl utms_estop_trigger
    b .
