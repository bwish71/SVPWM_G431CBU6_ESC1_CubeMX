/*
 * adc.h
 *
 *  Created on: May 20, 2025
 *      Author: acweiss
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <stdint.h>

/**
 * @brief  Initialize ADC1 on PB12 (IN11) for continuous, 12-bit, right-aligned conversions.
 *         After this, conversions are running continuously in the background.
 */
void adc_init(void);

/**
 * @brief  Poll for one end-of-conversion and return the raw 12-bit result [0..4095].
 * @retval uint16_t ADC sample on channel IN11 (PB12)
 */
uint16_t adc_read(void);


#endif /* INC_ADC_H_ */
