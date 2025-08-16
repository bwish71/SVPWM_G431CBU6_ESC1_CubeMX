/*
 * SVPWM.h
 *
 *  Created on: May 3, 2025
 *      Author: acweiss
 */

#ifndef INC_SVPWM_H_
#define INC_SVPWM_H_

#define SVPWM_NO_ERROR 0
#define SVPWM_SECTOR_ERROR 1
#define SVPWM_OVERMODULATION_ERROR 2
#define SVPWM_PIN_STATE_ERROR 3

typedef struct SvpwmData{
	float VBus; // DC bus voltage
    int sector;     // Sector number (1..6)
    int sector_soll; // Expected sector number (1..6)
    float grad;	  // Angle in degrees
    float Va;
    float Vb;
    float T0;      // Time for zero vector
    float T1;      // Time for first active vector
    float T2;      // Time for second active vector
    int ChA;     // Duty cycle for channel A
    int ChB;     // Duty cycle for channel B
    int ChC;     // Duty cycle for channel C
    int error;     // Error code (0 = no error, 1 = sector error, 2 = overmodulation)
} SvpwmData;

int svpwm_calculate(SvpwmData*);
//int SVPWM2(float, float, float, SVPWM*);

int run_svpwm_range(SvpwmData*, float, float, float);


#endif /* INC_SVPWM_H_ */
