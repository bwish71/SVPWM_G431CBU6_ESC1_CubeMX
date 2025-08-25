/*
 * SVPWM.c
 *
 *  Created on: May 3, 2025
 *      Author: acweiss
 */

// Function prototype for SVPWM is now in main.h
/**
 * SVPWM algorithm for 3-phase motors.
 *
 * @param Va     Alpha-axis component of reference vector
 * @param Vb     Beta-axis component of reference vector
 * @param VBus   DC bus voltage
 * @param sector_out Pointer to int to receive sector (1..6)
 * @param ChA_out Pointer to double to receive channel A duty
 * @param ChB_out Pointer to double to receive channel B duty
 * @param ChC_out Pointer to double to receive channel C duty
 */

#include "main.h" // Include main header for T_VAL and SQRT3
#include "SVPWM.h"
#include <stdio.h>
#include "util.h" // Include ASW util header file
#include <math.h> // Include math library for cos() and sin()
#include "debug.h" // Include ASW debug header file
#include <assert.h>



int svpwm_calculate(SvpwmData* data) {
    float T0, T1, T2;
    float ChA, ChB, ChC;

//    float sqrt3Va = round_to(SQRT3 * data->Va, 5); // Precompute for efficiency
//    float Sqrt3Vb = round_to(SQRT3 * data->Vb, 5);
    float sqrt3Va = (SQRT3 * data->Va); // Precompute for efficiency
    float Sqrt3Vb = (SQRT3 * data->Vb);

    data->error = 0; // Initialize error to 0

//    float Va = round_to(data->Va, 5); // Round to 5 decimal places
//    float Vb = round_to(data->Vb, 5); // Round to 5 decimal places
    float Va = data->Va; // Round to 5 decimal places
    float Vb = data->Vb; // Round to 5 decimal places


    if (Va < 0.0f) {
        if (Vb > 0.0f) {
            data->sector = ((Vb + sqrt3Va) > 0.0f) ? 2 : 3; // ToDo: Same as sector = (Vb > -sqrt3Va) ? 2 : 3;
        } else { // Vb <= 0
            data->sector = ((Vb - sqrt3Va) > 0.0f) ? 4 : 5;
        }
    } else { // Va >= 0
        if (Vb < 0.0f) {
            data->sector = ((Vb + sqrt3Va) < 0.0f) ? 5 : 6;
        } else { // Vb >= 0
            data->sector = ((Vb - sqrt3Va) < 0.0f) ? 1 : 2;
        }
    }


    data->sector_soll = ((int)data->grad / 60) + 1; // Calculate the expected sector based on the angle
//    if (data->sector != data->sector_soll) {
//    	printf("Error: Sector %d does not match expected sector %d for grad %f\r\n",
//			   data->sector, data->sector_soll, data->grad);
//    	while(1); // Stop execution if sector does not match expected sector
//    }

    // Compute switching times based on sector
    // Todo: precalculate T_Val / VBus

    float VBus = data->VBus; // Use VBus from data structure

    switch (data->sector) {
    case 1:
        T1 = (T_VAL / (2.0f * VBus)) * (3.0f * Va - Sqrt3Vb);
        T2 = (T_VAL / VBus) * Sqrt3Vb;
        break;
    case 2:
        T1 = (T_VAL / (2.0f * VBus)) * (3.0f * Va + Sqrt3Vb);
        T2 = (T_VAL / (2.0f * VBus)) * (-3.0f * Va + Sqrt3Vb);
        break;
    case 3:
        T1 = (T_VAL / VBus) * (Sqrt3Vb);
        T2 = (T_VAL / (2.0f * VBus)) * (-3.0f * Va - Sqrt3Vb);
        break;
    case 4:
        T1 = (T_VAL / (2.0f * VBus)) * (-3.0f * Va + Sqrt3Vb);
        T2 = (T_VAL / VBus) * (-Sqrt3Vb);
        break;
    case 5:
        T1 = (T_VAL / (2.0f * VBus)) * (-3.0f * Va - Sqrt3Vb);
        T2 = (T_VAL / (2.0f * VBus)) * (3.0f * Va - Sqrt3Vb);
        break;
    case 6:
        T1 = (T_VAL / VBus) * (-Sqrt3Vb);
        T2 = (T_VAL / (2.0f * VBus)) * (3.0f * Va + Sqrt3Vb);
        break;
    default:
        assert(false);
        break;
    }


    // Calculate T0 and check for overmodulation. Overmudulation can also happen if VBus is lower than expected.
    // ToDo: Better Scale than or return error that VBus is too low?
    T0 = T_VAL - T1 - T2;
    if (T0 < 0.0f || T1 < 0.0f || T2 < 0.0f) { // Check if any time is negative (indicates overmodulation or calculation issue)
        // Optionally clamp/handle overmodulation instead of just failing
        data->error = SVPWM_OVERMODULATION_ERROR;
        return SVPWM_OVERMODULATION_ERROR;
    }
    // ToDo: Also check for T1 & T2 > T_VAL ...? Is this possible?


    data->T0 = T0;
    data->T1 = T1;
    data->T2 = T2;

    // Calculate duty cycles (compare values) for each channel based on sector
    // These represent the timer counts for the compare registers
    switch (data->sector) {
    case 1:
        ChA = T0 * 0.5f;
        ChB = ChA + T1;
        ChC = ChB + T2;
        break;
    case 2:
        ChB = T0 * 0.5f;
        ChA = ChB + T2;
        ChC = ChA + T1;
        break;
    case 3:
        ChB = T0 * 0.5f;
        ChC = ChB + T1;
        ChA = ChC + T2;
        break;
    case 4:
        ChC = T0 * 0.5f;
        ChB = ChC + T2;
        ChA = ChB + T1;
        break;
    case 5:
        ChC = T0 * 0.5f;
        ChA = ChC + T1;
        ChB = ChA + T2;
        break;
    case 6:
        ChA = T0 * 0.5f;
        ChC = ChA + T2;
        ChB = ChC + T1;
        break;
    default:
        assert(false);
        break;
    }

    // Output results
    data->ChA = (int)ChA;
    data->ChB = (int)ChB;
    data->ChC = (int)ChC;

    // Apply the calculated duty cycles to the timer channels
    TIM1->CCR1 = (uint32_t) ChA;
    TIM1->CCR2 = (uint32_t) ChB;
    TIM1->CCR3 = (uint32_t) ChC;

    // Debug:
//    TIM1->CCR1 = (uint32_t) 2000;
//    TIM1->CCR2 = (uint32_t) 2000;
//    TIM1->CCR3 = (uint32_t) 2000;


    return 0; // No error
}


int run_svpwm_range(SvpwmData* data, float start, float end, float step) {
    // Function to run SVPWM for a range of angles
	printf("Grad, Va, Vb, Sector, SectorSoll, T0, T1, T2, ChA, ChB, ChC\r\n");

    for (float grad = start; grad <= end; grad += step) {

        data->grad = grad;
        data->sector_soll = ((int)grad / 60) + 1; // Calculate the expected sector based on the angle

        // Convert degrees to radians
        const float PI_F = 3.14159265358979323846f;
        float rad = grad * (PI_F / 180.0f);
        // Calculate alpha and beta components
        data->Va = (cosf(rad)) * 0.57735f * data->VBus;
        data->Vb = (sinf(rad)) * 0.57735f * data->VBus;

        svpwm_calculate(data);
        if (data->error) {
            print_debug_info(data);
        }

        printf("%f,%f,%f,%d,%d,%f,%f,%f,%d,%d,%d\r\n",
               data->grad, data->Va, data->Vb, data->sector, data->sector_soll,
               data->T0, data->T1, data->T2, data->ChA, data->ChB, data->ChC);
    }
    return 0;
}
