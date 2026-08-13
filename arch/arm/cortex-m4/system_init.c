#include <stdint.h>
#include <utms/hal/stm32f4xx.h>
#include <utms/hal/mpu.h>
#include <utms/hal/estop.h>

#define SCB_VTOR (*(volatile uint32_t *)0xE000ED08UL)
#define SCB_SHPR3 (*(volatile uint32_t *)0xE000ED20UL)
#define SCB_SHPR2 (*(volatile uint32_t *)0xE000ED1CUL)

static void enable_fpu(void) {
    __asm__ volatile (
        "movw r0, #0x0000 \n\t"
        "movt r0, #0xC000 \n\t"
        "mcr p15, 0, r0, c1, c0, 2 \n\t"
        "dsb \n\t"
        "isb \n\t"
        ::: "r0", "memory", "cc"
    );
}

static void configure_interrupts(void) {
    SCB_SHPR3 = (0xFF << 24) | (0x00 << 16);
    SCB_SHPR2 = (0x00 << 0);
}

void SystemInit(void) {
    SCB_VTOR = 0x08000000UL;

    enable_fpu();
    configure_interrupts();

    FLASH_REG->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY(5);

    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));

    RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC_HSE |
                   RCC_PLLCFGR_PLLM(8) |
                   RCC_PLLCFGR_PLLN(336) |
                   RCC_PLLCFGR_PLLP(0) |
                   RCC_PLLCFGR_PLLQ(7);

    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    RCC->CFGR = RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    while ((RCC->CFGR & (3UL << 2)) != RCC_CFGR_SWS_PLL);

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    utms_estop_init();
    utms_mpu_init();
}
