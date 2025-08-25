#include <TIM1_Init.h>
#include "main.h"


// TIM1 initialization (SVPWM @20kHz, center-aligned): dsadsa
void TIM1_Init(void) {

    // Enable clock for TIM1
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    // Prescaler = 0 (no prescaling)
    TIM1->PSC = 0;


    // Auto-reload value (ARR) = 4250 (170MHz / 20KHz / 2)
    TIM1->ARR = T_VAL;

    // Center-aligned (up-down counting)
    TIM1->CR1 |= TIM_CR1_CMS_0;		// Center-aligned mode 1 (up-down counting)

    // Preloads enable
    TIM1->CR1 |= TIM_CR1_ARPE;							// Enable auto-reload preload for counter
    TIM1->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE; 	// Enable preload for channels 1 and 2
    TIM1->CCMR2 |= TIM_CCMR2_OC3PE;					 	// Enable preload for channel 3

    // Enable PWM Mode 2 for channels 1-3. PWM Mode 2 = 0b111
    TIM1->CCMR1 |= (TIM_CCMR1_OC1M_2 | // OC1M[2]: PWM Mode 2
                    TIM_CCMR1_OC1M_1 | // OC1M[1]: PWM Mode 2
                    TIM_CCMR1_OC1M_0 | // OC1M[0]: PWM Mode 2
                    TIM_CCMR1_OC2M_2 | // OC2M[2]: PWM Mode 2
                    TIM_CCMR1_OC2M_1 | // OC2M[1]: PWM Mode 2
                    TIM_CCMR1_OC2M_0); // OC2M[0]: PWM Mode 2
    TIM1->CCMR2 |= (TIM_CCMR2_OC3M_2 | // OC3M[2]: PWM Mode 2
                    TIM_CCMR2_OC3M_1 | // OC3M[1]: PWM Mode 2
                    TIM_CCMR2_OC3M_0); // OC3M[0]: PWM Mode 2

    // alternative setting of mode2
   // TIM1->CCMR1 |= (0x7 << TIM_CCMR1_OC1M_Pos);  // PWM mode 2 (use 0x7 for PWM2)

    // Configure a very small dead time
   // TIM1->BDTR |= (0x01 << TIM_BDTR_DTG_Pos); // Set a small dead time (e.g., 1 clock cycle)
    // Dead-time configuration (~100ns @ 170MHz = ~17 )
    // TIM1->BDTR |= (17 & 0x7F);  // No scaling needed, bits 7:5 = 000

    TIM1->BDTR |= TIM_BDTR_MOE;   	// Main Output Enable of the timer output

    // Disable dead time
    TIM1->BDTR &= ~TIM_BDTR_DTG; 	// Clear the DTG field (no dead time)

    // Activate PWM outputs (channels 1,2,3 and complementary channels)
    TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E |TIM_CCER_CC3E;
    TIM1->CCER |= TIM_CCER_CC1NE | TIM_CCER_CC2NE | TIM_CCER_CC3NE; // Enable complementary outputs

    // Start the timer
    TIM1->CR1 |= TIM_CR1_CEN;

}
