/*
 * util.c
 *
 *  Created on: May 8, 2025
 *      Author: acweiss
 */


//#include "main.h"
#include "util.h"
#include <math.h>
//#include <stdio.h>
//#include "usart.h"
//#include "usb_device.h"



//int _write(int file, char *ptr, int len) {// Custom implementation of _write function
//	HAL_UART_Transmit(&huart2, (uint8_t*) ptr, len, HAL_MAX_DELAY);
//	return len;
//}



float round_to(float value, int n) {
    float factor = powf(10.0f, (float)n);
    return roundf(value * factor) / factor;
}

float lookup_sin_interp(float *table, float angle) {
    float idx_f = angle * (SINE_TABLE_SIZE / 360.0f);
    int idx = (int)idx_f;
    float frac = idx_f - idx;
    int idx1 = idx % SINE_TABLE_SIZE;
    int idx2 = (idx + 1) % SINE_TABLE_SIZE;
    if (idx1 < 0) idx1 += SINE_TABLE_SIZE;
    if (idx2 < 0) idx2 += SINE_TABLE_SIZE;
    float v1 = table[idx1];
    float v2 = table[idx2];
    return v1 + (v2 - v1) * frac;
}

float lookup_cos_interp(float *table, float angle) {
    // Cosine is sine with a +90 degree phase shift
    return lookup_sin_interp(table, angle + 90.0f);
}

void generate_sine_table( float *table) {
    for (int i = 0; i < SINE_TABLE_SIZE; i++) {
        float angle = (float)i * 360.0f / (float)SINE_TABLE_SIZE;
        table[i] = sinf(angle * (3.14159265358979323846f / 180.0f));
    }
}
