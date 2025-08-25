/*
 * debug.h
 *
 *  Created on: May 6, 2025
 *      Author: acweiss
 */

#ifndef INC_DEBUG_H_
#define INC_DEBUG_H_

#include <stdbool.h>
#include "SVPWM.h" // for SvpwmData and error codes

// function prototypes to print debug information and check pin states
void print_debug_info(SvpwmData* data);

void debug_toggle_pwm_outputs(void); // Toggle all PWM outputs for debugging indefinitely

#endif /* INC_DEBUG_H_ */
