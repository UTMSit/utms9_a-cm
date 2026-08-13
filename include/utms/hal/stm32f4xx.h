#ifndef UTMS_HAL_STM32F4XX_H
#define UTMS_HAL_STM32F4XX_H

#include <stdint.h>

#define __IO volatile

#define RCC_BASE      (0x40023800UL)
#define FLASH_BASE    (0x40023C00UL)
#define SCB_BASE      (0xE000ED90UL)
#define SYSTICK_BASE  (0xE000E010UL)

#define GPIOA_BASE    (0x40020000UL)
#define GPIOB_BASE    (0x40020400UL)
#define GPIOC_BASE    (0x40020800UL)
#define GPIOD_BASE    (0x40020C00UL)

#define USART2_BASE   (0x40004400UL)
#define TIM5_BASE     (0x40000C00UL)
#define TIM2_BASE     (0x40000000UL)
#define TIM3_BASE     (0x40000400UL)
#define TIM4_BASE     (0x40000800UL)
#define DMA1_BASE     (0x40026000UL)

#define DMA1_Stream0_BASE (DMA1_BASE + 0x10)
#define DMA1_Stream1_BASE (DMA1_BASE + 0x10 + 0x18 * 1)
#define DMA1_Stream2_BASE (DMA1_BASE + 0x10 + 0x18 * 2)
#define DMA1_Stream6_BASE (DMA1_BASE + 0x10 + 0x18 * 6)

typedef struct {
    __IO uint32_t CR;
    __IO uint32_t PLLCFGR;
    __IO uint32_t CFGR;
    __IO uint32_t CIR;
    __IO uint32_t AHB1RSTR;
    __IO uint32_t AHB2RSTR;
    __IO uint32_t AHB3RSTR;
    uint32_t      RESERVED0;
    __IO uint32_t APB1RSTR;
    __IO uint32_t APB2RSTR;
    uint32_t      RESERVED1[2];
    __IO uint32_t AHB1ENR;
    __IO uint32_t AHB2ENR;
    __IO uint32_t AHB3ENR;
    uint32_t      RESERVED2;
    __IO uint32_t APB1ENR;
    __IO uint32_t APB2ENR;
} RCC_TypeDef;

typedef struct {
    __IO uint32_t ACR;
    __IO uint32_t KEYR;
    __IO uint32_t OPTKEYR;
    __IO uint32_t SR;
    __IO uint32_t CR;
    __IO uint32_t OPTCR;
} FLASH_TypeDef;

typedef struct {
    uint32_t RESERVED0[856];
    __IO uint32_t ICSR;
    __IO uint32_t VTOR;
    __IO uint32_t AIRCR;
    __IO uint32_t SCR;
    __IO uint32_t CCR;
    __IO uint8_t  SHP[12];
    __IO uint32_t SHCSR;
} SCB_TypeDef;

typedef struct {
    __IO uint32_t CTRL;
    __IO uint32_t LOAD;
    __IO uint32_t VAL;
    __IO uint32_t CALIB;
} SYSTICK_TypeDef;

typedef struct {
    __IO uint32_t MODER;
    __IO uint32_t OTYPER;
    __IO uint32_t OSPEEDR;
    __IO uint32_t PUPDR;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t LCKR;
    __IO uint32_t AFR[2];
} GPIO_TypeDef;

typedef struct {
    __IO uint32_t SR;
    __IO uint32_t DR;
    __IO uint32_t BRR;
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t CR3;
    __IO uint32_t GTPR;
} USART_TypeDef;

typedef struct {
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t SMCR;
    __IO uint32_t DIER;
    __IO uint32_t SR;
    __IO uint32_t EGR;
    __IO uint32_t CCMR1;
    __IO uint32_t CCMR2;
    __IO uint32_t CCER;
    __IO uint32_t CNT;
    __IO uint32_t PSC;
    __IO uint32_t ARR;
    uint32_t RESERVED0;
    __IO uint32_t CCR1;
    __IO uint32_t CCR2;
    __IO uint32_t CCR3;
    __IO uint32_t CCR4;
    uint32_t RESERVED1;
    __IO uint32_t DCR;
    __IO uint32_t DMAR;
} TIM_TypeDef;

typedef struct {
    __IO uint32_t LISR;
    __IO uint32_t HISR;
    __IO uint32_t LIFCR;
    __IO uint32_t HIFCR;
} DMA_TypeDef;

typedef struct {
    __IO uint32_t CR;
    __IO uint32_t NDTR;
    __IO uint32_t PAR;
    __IO uint32_t M0AR;
    __IO uint32_t M1AR;
    __IO uint32_t FCR;
} DMA_Stream_TypeDef;

#define RCC       ((RCC_TypeDef *)RCC_BASE)
#define FLASH_REG ((FLASH_TypeDef *)FLASH_BASE)
#define SCB       ((SCB_TypeDef *)SCB_BASE)
#define SYSTICK   ((SYSTICK_TypeDef *)SYSTICK_BASE)
#define GPIOA     ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB     ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC     ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD     ((GPIO_TypeDef *)GPIOD_BASE)
#define USART2    ((USART_TypeDef *)USART2_BASE)
#define TIM5      ((TIM_TypeDef *)TIM5_BASE)
#define TIM2      ((TIM_TypeDef *)TIM2_BASE)
#define TIM3      ((TIM_TypeDef *)TIM3_BASE)
#define TIM4      ((TIM_TypeDef *)TIM4_BASE)
#define DMA1      ((DMA_TypeDef *)DMA1_BASE)
#define DMA1_Stream0 ((DMA_Stream_TypeDef *)DMA1_Stream0_BASE)
#define DMA1_Stream1 ((DMA_Stream_TypeDef *)DMA1_Stream1_BASE)
#define DMA1_Stream2 ((DMA_Stream_TypeDef *)DMA1_Stream2_BASE)
#define DMA1_Stream6 ((DMA_Stream_TypeDef *)DMA1_Stream6_BASE)

#define RCC_CR_HSEON        (1UL << 16)
#define RCC_CR_HSERDY       (1UL << 17)
#define RCC_CR_PLLON        (1UL << 24)
#define RCC_CR_PLLRDY       (1UL << 25)

#define RCC_CFGR_SW_PLL     (2UL << 0)
#define RCC_CFGR_SWS_PLL    (2UL << 2)
#define RCC_CFGR_HPRE_DIV1  (0UL << 4)
#define RCC_CFGR_PPRE1_DIV4 (5UL << 10)
#define RCC_CFGR_PPRE2_DIV2 (4UL << 13)

#define RCC_PLLCFGR_PLLSRC_HSE (1UL << 22)
#define RCC_PLLCFGR_PLLM(x)    ((uint32_t)(x) << 0)
#define RCC_PLLCFGR_PLLN(x)    ((uint32_t)(x) << 6)
#define RCC_PLLCFGR_PLLP(x)    ((uint32_t)(x) << 16)
#define RCC_PLLCFGR_PLLQ(x)    ((uint32_t)(x) << 24)

#define FLASH_ACR_LATENCY(x) ((uint32_t)(x) << 0)
#define FLASH_ACR_PRFTEN     (1UL << 8)
#define FLASH_ACR_ICEN       (1UL << 9)
#define FLASH_ACR_DCEN       (1UL << 10)

#define SCB_ICSR_PENDSVSET_Msk (1UL << 28)

#define SYSTICK_CTRL_ENABLE    (1UL << 0)
#define SYSTICK_CTRL_TICKINT   (1UL << 1)
#define SYSTICK_CTRL_CLKSOURCE (1UL << 2)

#define RCC_AHB1ENR_GPIOAEN  (1UL << 0)
#define RCC_AHB1ENR_GPIODEN  (1UL << 3)
#define RCC_APB1ENR_USART2EN (1UL << 17)
#define RCC_APB1ENR_TIM5EN   (1UL << 3)
#define RCC_APB1ENR_TIM2EN   (1UL << 0)
#define RCC_APB1ENR_TIM3EN   (1UL << 1)
#define RCC_APB1ENR_TIM4EN   (1UL << 2)
#define RCC_AHB1ENR_DMA1EN   (1UL << 21)

#define USART_SR_TXE   (1UL << 7)
#define USART_SR_RXNE  (1UL << 5)
#define USART_CR1_UE   (1UL << 13)
#define USART_CR1_TE   (1UL << 3)
#define USART_CR1_RE   (1UL << 2)

#define TIM_CR1_CEN       (1UL << 0)
#define TIM_CCER_CC1E     (1UL << 0)
#define TIM_CCMR1_OC1M_PWM1 (6UL << 4)
#define TIM_CCMR1_OC1PE   (1UL << 3)
#define TIM_DIER_UDE      (1UL << 8)

#define DMA_SxCR_CHSEL_3  (3UL << 25)
#define DMA_SxCR_CHSEL_5  (5UL << 25)
#define DMA_SxCR_CHSEL_6  (6UL << 25)
#define DMA_SxCR_CHSEL_2  (2UL << 25)
#define DMA_SxCR_DIR_M2P  (1UL << 6)
#define DMA_SxCR_MINC     (1UL << 10)
#define DMA_SxCR_PSIZE_32 (2UL << 11)
#define DMA_SxCR_MSIZE_32 (2UL << 13)
#define DMA_SxCR_PL_HIGH  (2UL << 16)
#define DMA_SxCR_EN       (1UL << 0)

void SystemInit(void);

#endif
