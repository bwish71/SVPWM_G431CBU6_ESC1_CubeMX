/*
 * util.h
 *
 *  Created on: May 8, 2025
 *      Author: acweiss
 */

#ifndef INC_UTIL_H_
#define INC_UTIL_H_

#define SINE_TABLE_SIZE 1440

int _write(int, char*, int);

float round_to(float , int);

/**
 * @brief Interpolated lookup for sine from a table
 * @param table: pointer to the lookup table
 * @param tableSize: number of entries in the table
 * @param angle: angle in degrees (0-360)
 * @return interpolated value
 */
void generate_sine_table( float *table);
float lookup_sin_interp( float *table, float angle);

/**
 * @brief Interpolated lookup for cosine from a sine table
 * @param table: pointer to the sine lookup table
 * @param tableSize: number of entries in the table
 * @param angle: angle in degrees (0-360)
 * @return interpolated value (cosine)
 */
float lookup_cos_interp( float *table, float angle);


#endif /* INC_UTIL_H_ */
