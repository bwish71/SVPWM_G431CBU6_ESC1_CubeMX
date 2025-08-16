/*
 * util.c
 *
 *  Created on: May 8, 2025
 *      Author: acweiss
 */


#include "main.h"
#include "util.h"
#include <math.h>
#include <stdio.h>
#include "usart.h"
#include "usb_device.h"



//int _write(int file, char *ptr, int len) {// Custom implementation of _write function
//	HAL_UART_Transmit(&huart2, (uint8_t*) ptr, len, HAL_MAX_DELAY);
//	return len;
//}



float round_to(float value, int n) {
    float factor = powf(10.0f, (float)n);
    return roundf(value * factor) / factor;
}