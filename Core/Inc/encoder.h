#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include "stm32g4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ENCODER_LINES 1024              // Electrical cycles (A channel periods) per mechanical revolution
// Backward compatibility macro (old name CPR -> lines). Remove later if not needed.
#define ENCODER_CPR ENCODER_LINES
// Hardware encoder mode (TIM_ENCODERMODE_TI12) counts one edge per channel (rising)
// => x2 decoding (2 * LINES counts / mechanical rev). Both-edge (CCxNP=1 & CCxP=1) forbidden in encoder mode.
#define ENCODER_COUNTS_PER_REV (2 * ENCODER_LINES)

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
