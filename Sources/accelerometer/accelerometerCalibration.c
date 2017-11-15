//
// Created by Evan on 2017-11-06.
// Assumes an already initialized Accelerometer.
//
//
#include "LIS3DSH.h"
#include <stm32f407xx.h>

//CALIBRATION SETTINGS
//GLOBAL Variables
int sampleCount=0;
#define MAX_NUMBER_OF_SAMPLES 10000

// ***** CALLBACK FUNCTION *** gets called from stm32f4xx_it.c > stm32f4xx_hal_gpio.c, on INTERRUPT
/**
 * It's CRUCIAL that you comment out the callback function inside of accelerometer.c,
 * which uses a filter. We are overriding it here, because we don't want to filter data/calculate pitch and roll
 * @param GPIO_Pin
 */
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//    if(sampleCount<MAX_NUMBER_OF_SAMPLES){
//        float Buffer[3];
//        //Get the filtered x,y,z readings from the accelerometer
//        LIS3DSH_ReadACC(&Buffer[0]);
//        printf("%3f,%3f,%3f\n", (float)Buffer[0], (float)Buffer[1], (float)Buffer[2]);
//        sampleCount++;
//    }
//
//}

