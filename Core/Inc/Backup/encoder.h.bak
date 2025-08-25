#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include "stm32g4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif



void encoder_init(TIM_HandleTypeDef *htim);
int32_t encoder_abs_count(void);
int32_t encoder_revs(void);
int32_t encoder_inrev(void);

// To be called from IRQ handler when update (overflow/underflow) flag is set
// NOTE: Update (overflow/underflow) IRQ fires only when 16-bit counter wraps (ARR+1 counts).
// No periodic interrupt in between; counting works without IRQ. IRQ only extends to 32 bits.
void encoder_irq_on_update(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif

#endif // ENCODER_H
