#include <utms/hal/adc.h>
#include <utms/hal/stm32f4xx.h>

void utms_adc_init(utms_adc_t *adc) {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    for (uint32_t i = 0; i < adc->channel_count; i++) {
        uint32_t ch = adc->channel_map[i];
        if (ch < 8) {
            GPIOA->MODER |= (3UL << (ch * 2));
        }
    }

    DMA1_Stream0->CR = 0;
    while (DMA1_Stream0->CR & DMA_SxCR_EN);

    DMA1_Stream0->NDTR = adc->channel_count;
    DMA1_Stream0->PAR = (uint32_t)&ADC1->DR;
    DMA1_Stream0->M0AR = (uint32_t)adc->buffer;
    DMA1_Stream0->CR = DMA_SxCR_CHSEL_0 |
                       DMA_SxCR_DIR_P2M |
                       DMA_SxCR_MINC |
                       DMA_SxCR_PSIZE_16 |
                       DMA_SxCR_MSIZE_16 |
                       DMA_SxCR_PL_HIGH |
                       DMA_SxCR_CIRC;
    DMA1_Stream0->FCR = 0;
    DMA1_Stream0->CR |= DMA_SxCR_EN;

    ADC_COMMON->CCR = 0;

    ADC1->CR1 = 0;
    ADC1->SMPR1 = 0x00FFFFFF;
    ADC1->SMPR2 = 0x3FFFFFFF;

    uint32_t sq = 0;
    for (uint32_t i = 0; i < adc->channel_count; i++) {
        sq |= (adc->channel_map[i] << (i * 5));
    }
    ADC1->SQR3 = sq;
    ADC1->SQR1 = ((adc->channel_count - 1) << ADC_SQR1_L_SHIFT);

    ADC1->CR2 = ADC_CR2_DMA | ADC_CR2_DDS | ADC_CR2_CONT | ADC_CR2_ADON;
}

uint16_t utms_adc_read(utms_adc_t *adc, uint32_t channel_index) {
    if (channel_index >= adc->channel_count) return 0;
    return adc->buffer[channel_index];
}

void utms_adc_start(utms_adc_t *adc) {
    (void)adc;
    ADC1->CR2 |= ADC_CR2_SWSTART;
}
