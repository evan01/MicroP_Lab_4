/*******************************************************************************
  * @file    Thread_LED.c
  * @author  Amirhossein Shahshahani
	* @version V1.2.0
  * @date    10-Nov-2017
  * @brief   This file initializes one LED as an output, implements the LED thread 
  *					 which toggles and LED, and function which creates and starts the thread	
  ******************************************************************************
  */
	
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "Tim/tim.h"
#include "Display/display.h"
#include "StateMachine/state_machine.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "accelerometer/accelerometer.h"



void Thread_Display (void const *argument);                 // thread function
void Thread_Keypad (void const *argument);                 // thread function
osThreadId tid_Thread_Display;                              // thread id
osThreadId tid_Thread_Keypad;                              // thread id

/*		**** depending on the version of the CMSIS library, you may need to make your threads in different way:   *******

	osThreadDef(Thread_1,Thread_LED, osPriorityNormal, osPriorityNormal, 128);
	osThreadDef(Thread_2, Thread_LED_2, osPriorityNormal, osPriorityNormal, 128);
	osThreadDef(Thread_3, Thread_LED_3, osPriorityNormal, osPriorityNormal, 128);
 
	tid_Thread_LED_1 = osThreadCreate(osThread(Thread_1), NULL);
	tid_Thread_LED_2 = osThreadCreate(osThread(Thread_2), NULL);
	tid_Thread_LED_3 = osThreadCreate(osThread(Thread_3), NULL);

	This is a style when you create a project with CubMx as it uses the newest version

*/

// Following is different format of creating your threads. This project is based on the older CMSIS version.
osThreadDef(Thread_Display, osPriorityNormal, 1, 0);
osThreadDef( Thread_Keypad, osPriorityNormal, 1, 0);
//GPIO_InitTypeDef 				LED_configuration;

/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Threads (void) {

	tid_Thread_Display = osThreadCreate(osThread(Thread_Display), NULL);
	tid_Thread_Keypad = osThreadCreate(osThread(Thread_Keypad), NULL);
  //if (!tid_Thread_LED_1) return(-1); 
	//else if (!tid_Thread_LED_2) return(-1); 
	//else if (!tid_Thread_LED_3) return(-1); 
 // return(0);
}

 /*----------------------------------------------------------------------------
*      Thread  'LED_Thread': Toggles LED
 *---------------------------------------------------------------------------*/
uint32_t judgeDuty(uint32_t target, float current) {
    float rval;
    rval = 200.0 * ((fabsf((float) target - (float) current)) / 180.0);
    return (uint32_t) rval;

}

int display(int value) {
    //display the digits
    if (value == 1) {
        return displayDigits(pitch);
    } else if (value == 0) {
        return displayDigits(roll);
    } else {
        //In sleep state
        return displayDigits(value);
    }
}

int infiniteLoop() {
    //Main program execution ins here.
		printf("%d\n", state);
    if (state == PITCH_MONITOR_STATE) {
        display(1);
    } else if (state == ROLL_MONITOR_STATE) {
        display(0);
    } else if (state == START_STATE) {
        display(8888);
    } else if (state == SLEEP_STATE) {
        resetDisplay();
    } else if (state == ENTER_ROLL_STATE) {
        float f=0.0;
        sscanf(roll_buf, "%f", &f);
        display((int) f);
    } else if (state == ENTER_PITCH_STATE) {
        float f=0.0;
        sscanf(pitch_buf, "%f", &f);
        display((int) f);
    }
		uint32_t intensityPitch = judgeDuty(target_pitch, pitch);
		uint32_t intensityRoll = judgeDuty(target_roll, roll);
		setLedIntensityPitch(intensityPitch);
		setLedIntensityRoll(intensityRoll);
}
	void Thread_Display (void const *argument) {
		int counter =0;
		while(1){
//				osDelay(1000);
//				if(counter == 1){
//					setLedIntensityPitch(0);
//					setLedIntensityRoll(200);
//					counter = 0;
//					printf("toggling to 0");
//				}else{
//					setLedIntensityPitch(200);
//					setLedIntensityRoll(0);	
//					counter= 1;
//					printf("toggling to 1");

//				}
				infiniteLoop();
				printf("%d\n", HAL_GetTick());
			
			
			}
	}
	
	
	void Thread_Keypad (void const *argument) {
		while(1){
				osDelay(500);
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
			}
	}	

