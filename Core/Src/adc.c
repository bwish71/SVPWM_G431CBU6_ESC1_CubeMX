/* adc.c */
#include "adc.h"
#include "stm32g431xx.h"

/* ~20 µs delay */
static void delay_20us(void) {
    for (volatile int i = 0; i < 2000; i++);
}

void adc_init(void)
{
    // 1) Clocks for GPIOB & ADC12
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN   // PB12
                  | RCC_AHB2ENR_ADC12EN; // ADC1 & ADC2 common

    // 2) PB12 → analog, no pull
    GPIOB->MODER |=  (3U << (12*2));
    GPIOB->PUPDR &= ~(3U << (12*2));

    // 3) Select SYSCLK for ADC12
    RCC->CCIPR &= ~RCC_CCIPR_ADC12SEL;

    // 4) Exit deep-power-down, enable VREG
    ADC1->CR &= ~ADC_CR_DEEPPWD;
    ADC1->CR |=  ADC_CR_ADVREGEN;
    delay_20us();

    // 5) Power on ADC & wait ready
    ADC1->ISR |= ADC_ISR_ADRDY; // clear ADRDY
    ADC1->CR  |= ADC_CR_ADEN;
    while (!(ADC1->ISR & ADC_ISR_ADRDY));

    // 6) Self‐calibration (must follow ADEN)
    ADC1->CR |= ADC_CR_ADCAL;
    while (ADC1->CR & ADC_CR_ADCAL);

    // 7) Continuous mode + preserve on overrun
    ADC1->CFGR = ADC_CFGR_CONT    // continuous
                | ADC_CFGR_OVRMOD; // keep old data on overrun

    // 8) ADC clock prescaler = PCLK/4 via common CCR
    ADC12_COMMON->CCR = (ADC12_COMMON->CCR & ~ADC_CCR_CKMODE)
                       | ADC_CCR_CKMODE_1; // 10b → PCLK/4

    // 9) Sample time for IN11 (PB12) = 81.5 cycles
    ADC1->SMPR2 = (ADC1->SMPR2 & ~ADC_SMPR2_SMP11)
                | ADC_SMPR2_SMP11_2;

    // 10) Regular sequence: one conversion, first rank = channel 11
    ADC1->SQR1 &= ~ADC_SQR1_L;            // L = 0 → 1 conversion
    ADC1->SQR3 &= ~ADC_SQR3_SQ10_Msk   ;          // clear old rank1 bits
    ADC1->SQR3 |=  (11U << ADC_SQR3_SQ10_Pos); // SQ1 = IN11 :contentReference[oaicite:0]{index=0}

    // 11) Start continuous conversions
    ADC1->CR |= ADC_CR_ADSTART;
}

uint16_t adc_read(void)
{
    // wait end-of-conversion
    while (!(ADC1->ISR & ADC_ISR_EOC));
    uint16_t v = (uint16_t)(ADC1->DR & 0x0FFF);
    // clear EOC
    ADC1->ISR |= ADC_ISR_EOC;
    return v;
}

