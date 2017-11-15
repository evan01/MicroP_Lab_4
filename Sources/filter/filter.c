//
// Created by Evan on 2017-11-07.
//
#include "filter.h"
#include <math.h>
#include "arm_math.h"
#include <stdlib.h>
#include "stdio.h"
#include <stm32f407xx.h>

/** FILTER PARAMETERS */
static float filterX_coef[5] = {0.9995,0.01,0.0429,-0.0238,-0.0097};
static float filterY_coef[5] = {0.9526,0.01,0.0460,-0.01233,-0.0055};
static float filterZ_coef[5] = {0.9526,0.01,0.0460,-0.01233,-0.0055};

/**
 * This is a filter function, with which we feed in the accelerometer values.
 * @param InputArray
 * @param OutputArray
 * @param coef
 * @param Length
 * @return
 */
float* IIR_CMSIS(float* InputArray, float* OutputArray, uint32_t Length, float* filter_coef){
    float pState[8] = {0.0,0.0,0.0,0.0};
    uint8_t numStages = 1;
    arm_biquad_casd_df1_inst_f32 S1 = {numStages, pState, filter_coef}; //Init filter
    arm_biquad_cascade_df1_f32(&S1, InputArray, OutputArray, Length); //Calculate input
    return OutputArray;
}

/**
 * Take in a unfilted pitch and roll, and depending on history, filters to get a new pitch and roll
 * @param pitch
 * @param roll
 */
struct SAMPLE filter(float x, float y, float z){
	static struct SAMPLE in_oldestSample;
    static struct SAMPLE in_lastSample;
	static struct SAMPLE out_oldestSample;
	static struct SAMPLE out_lastSample;
	
    //Get params to pass to filter from history
    float x_in[3] = {in_oldestSample.x, in_lastSample.x, x};
    float y_in[3] = {in_oldestSample.y, in_lastSample.y, y};
    float z_in[3] = {in_oldestSample.z, in_lastSample.z, z};
	
    float x_out[3] = {out_oldestSample.x, out_lastSample.x, 0};
    float y_out[3] = {out_oldestSample.y, out_lastSample.y, 0};
    float z_out[3] = {out_oldestSample.z, out_lastSample.z, 0};

    //Filter
    IIR_CMSIS(x_in,x_out,3, filterX_coef);
    IIR_CMSIS(y_in,y_out,3, filterY_coef);
    IIR_CMSIS(z_in,z_out,3, filterZ_coef);

    in_oldestSample = in_lastSample;
    out_oldestSample= out_lastSample;
    in_lastSample.x = x;
    in_lastSample.y = y;
    in_lastSample.z = z;
    out_lastSample.x = x_out[2];
    out_lastSample.y = y_out[2];
    out_lastSample.z = z_out[2];

    return out_lastSample;
    //    printf("FILTERED----->\t\tPitch: %3f   Roll: %3f\n", x_out[2], y_out[2]);
}
