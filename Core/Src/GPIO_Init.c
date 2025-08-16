#include <GPIO_Init.h>
#include "main.h"

void GPIO_Init(void) {
	// Enable clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN        // Enable GPIOA
	| RCC_AHB2ENR_GPIOBEN        // Enable GPIOB
			| RCC_AHB2ENR_GPIOCEN;       // Enable GPIOC

#ifdef ESC_BOARD
	// Configure PC6 as output for the LED
	// Clear mode bits for PC6
	GPIOC->MODER &= ~(GPIO_MODER_MODE6);
	// Set PC6 to General purpose output mode (01)
	GPIOC->MODER |= GPIO_MODER_MODE6_0;

	// GPIO Setting for Compare Outputs of TIM1
	// Function | Pin | Port | Notes
	// PWM_U_H | PA8 | A | TIM1_CH1	| AF6
	// PWM_U_L | PC13 | C | TIM1_CH1N | AF4
	// PWM_V_H | PA9 | A | TIM1_CH2 	| AF6
	// PWM_V_L | PA12 | A | TIM1_CH2N | AF6
	// PWM_W_H | PA10 | A | TIM1_CH3 	| AF6
	// PWM_W_L | PB15 | B | TIM1_CH3N | AF4

	// Set all used GPIOs

	// PA8, PA9, PA10, PA12
	GPIOA->MODER &= ~(GPIO_MODER_MODE8 | GPIO_MODER_MODE9 |
			GPIO_MODER_MODE10 | GPIO_MODER_MODE12);// Clear mode bits
	GPIOA->MODER |= (GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1 |
			GPIO_MODER_MODE10_1 | GPIO_MODER_MODE12_1);// AF mode

	GPIOA->AFR[1] |= (6 << GPIO_AFRH_AFSEL8_Pos)// PA8  = AF6 (CH1)
	| (6 << GPIO_AFRH_AFSEL9_Pos)// PA9  = AF6 (CH2)
	| (6 << GPIO_AFRH_AFSEL10_Pos)// PA10 = AF6 (CH3)
	| (6 << GPIO_AFRH_AFSEL12_Pos);// PA12 = AF6 (CH2N)

	// PB15
	GPIOB->MODER &= ~(GPIO_MODER_MODE15);// Clear mode bits
	GPIOB->MODER |= (GPIO_MODER_MODE15_1);// AF mode

	GPIOB->AFR[1] |= (4<< GPIO_AFRH_AFSEL15_Pos);// PB15 = AF4 (CH3N)

	// PC13
	GPIOC->MODER &= ~(GPIO_MODER_MODE13);// Clear mode bits
	GPIOC->MODER |= (GPIO_MODER_MODE13_1);// AF mode

	GPIOC->AFR[1] |= (4 << GPIO_AFRH_AFSEL13_Pos);// PC13 = AF4 (CH1N)

	// Set speed to high (to reduce rise/fall time)
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED8 | GPIO_OSPEEDR_OSPEED9 |
	GPIO_OSPEEDR_OSPEED10 | GPIO_OSPEEDR_OSPEED12;
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED15;
	GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED13;

	// Optional: push-pull, no pull-up/down
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9 |
			GPIO_OTYPER_OT10 | GPIO_OTYPER_OT12);// Push-pull
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD8 | GPIO_PUPDR_PUPD9 |
			GPIO_PUPDR_PUPD10 | GPIO_PUPDR_PUPD12);// No pull-up/down

	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT15);// Push-pull
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD15);// No pull-up/down

	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT13);// Push-pull
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD13);// No pull-up/down

	// configure switch as input on PC10 for ESC Board or PC13 for WeAct Board
	GPIOC->MODER &= ~(GPIO_MODER_MODE13);// Clear mode bits
	// Enable pull-down resistor
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD13);// No pull-up/down
	GPIOC->PUPDR |= GPIO_PUPDR_PUPD13_1;// Pull-down

	// Configure the potentiometer input on PB12 (0 to 3.3V)
	GPIOB->MODER &= ~(GPIO_MODER_MODE12); // Clear mode bits
	GPIOB->MODER |= GPIO_MODER_MODE12_0; // Set PB12 to Analog mode (00)
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD12); // No pull-up/down
	GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED12); // Set speed to low (to reduce power consumption)


#elif defined(WEACT_BOARD)
	// Configure PC6 as output for the LED
	GPIOC->MODER &= ~(GPIO_MODER_MODE6);
	GPIOC->MODER |= GPIO_MODER_MODE6_0; // Set PC6 to General purpose output mode (01)

	/**TIM1 GPIO Configuration
	 PC13     ------> TIM1_CH1N -> AF4
	 PB14     ------> TIM1_CH2N -> AF6
	 PB15     ------> TIM1_CH3N -> AF4
	 PA8     ------> TIM1_CH1 -> AF6
	 PA9     ------> TIM1_CH2 -> AF6
	 PA10     ------> TIM1_CH3 -> AF6
	 */

	// Set all used GPIOs
	GPIOA->MODER &= ~(GPIO_MODER_MODE8 | GPIO_MODER_MODE9 |
	GPIO_MODER_MODE10); 	// Clear mode bits
	GPIOA->MODER |= (GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1 |
	GPIO_MODER_MODE10_1); 	// AF mode
	GPIOA->AFR[1] 	|= (6 << GPIO_AFRH_AFSEL8_Pos) 	// PA8  = AF6 (CH1)
					| (6 << GPIO_AFRH_AFSEL9_Pos) 	// PA9  = AF6 (CH2)
					| (6 << GPIO_AFRH_AFSEL10_Pos); 	// PA10 = AF6 (CH3)
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED8 | GPIO_OSPEEDR_OSPEED9 |
	GPIO_OSPEEDR_OSPEED10; // Set speed to high (to reduce rise/fall time)
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9 |
	GPIO_OTYPER_OT10); // Push-pull
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD8 | GPIO_PUPDR_PUPD9 |
	GPIO_PUPDR_PUPD10); // No pull-up/down

	GPIOB->MODER &= ~(GPIO_MODER_MODE14 | GPIO_MODER_MODE15); // Clear mode bits
	GPIOB->MODER |= (GPIO_MODER_MODE14_1 | GPIO_MODER_MODE15_1); // AF mode
	GPIOB->AFR[1] 	|= (6 << GPIO_AFRH_AFSEL14_Pos) // PB14 = AF6 (CH2N)
					| (4 << GPIO_AFRH_AFSEL15_Pos); // PB15 = AF4 (CH3N)
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED14 | GPIO_OSPEEDR_OSPEED15; // Set speed to high (to reduce rise/fall time)
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT14 | GPIO_OTYPER_OT15); // Push-pull
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD14 | GPIO_PUPDR_PUPD15); // No pull-up/down

	GPIOC->MODER &= ~(GPIO_MODER_MODE13); // Clear mode bits
	GPIOC->MODER |= (GPIO_MODER_MODE13_1); // AF mode
	GPIOC->AFR[1] |= (4 << GPIO_AFRH_AFSEL13_Pos); // PC13 = AF4 (CH1N)
	GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED13; // Set speed to high (to reduce rise/fall time)
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT13); // Push-pull
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD13); // No pull-up/down


	// Configure PA0 and PA1 for encoder inputs
	GPIOA->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1); // Set PA0 and PA1 as input
	GPIOA->MODER |= (GPIO_MODER_MODE0_1 | GPIO_MODER_MODE1_1); // Set PA0 and PA1 to Alternate Function mode (MODER = 0b10)
	GPIOA->AFR[0] |= (1 << GPIO_AFRL_AFSEL0_Pos) | (1 << GPIO_AFRL_AFSEL1_Pos); // Select AF1 for TIM2 on PA0 and PA1
	GPIOA->PUPDR |= (GPIO_PUPDR_PUPD0_1 | GPIO_PUPDR_PUPD1_1); // Pull-down PA0 and PA1


#endif

}

