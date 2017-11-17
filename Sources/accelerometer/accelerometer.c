/*
 *  ACCELEROMETER CODE
	This is the code for the accelerometer on the board. Here's how to set up the accelerometer.

	gpio.c
		-GPIO_InitStruct.Pin = GPIO_PIN_0;
		-GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		-GPIO_InitStruct.Pull = GPIO_NOPULL;
		-HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    main.c
        -initializeAccelerometer()
*/


#include "LIS3DSH.h"
#include <stm32f407xx.h>
#include <arm_math.h>
#include "arm_math.h"
#define M_PI 3.14159265358979323846
#include "../filter/filter.h"
#include "stdlib.h"
#include "../Threads.h"

//Global Constants
float roll = 0.00;
float pitch = 0.00;

float32_t calibrationData[4][3] = {
    {0.0009251,-0.0000121,0.0000021},
    {-0.0000411,0.0009581,-0.0000221},
    {0.0000091,0.0000081,0.0009661},
    {-0.0079481,-0.0018801,-0.0259401}
};
float32_t output[3];

arm_matrix_instance_f32 IN;
arm_matrix_instance_f32 CAL;
arm_matrix_instance_f32 OUT;

LIS3DSH_InitTypeDef Acc_instance;
LIS3DSH_DRYInterruptConfigTypeDef Acc_interruptConfig;
int counter = 0;

// ***** CALLBACK FUNCTION *** gets called from stm32f4xx_it.c > stm32f4xx_hal_gpio.c, on INTERRUPT
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

    float Buffer[3];
    float accX, accY, accZ;

    //First get the x,y,z readings from the accelerometer
    LIS3DSH_ReadACC(&Buffer[0]);

    accX = Buffer[0];
    accY = Buffer[1];
    accZ = Buffer[2];

    //Then CALIBRATE the readings through matrix multiplication
//	if(counter < 1100)
//		//printf("%3f,%3f,%3f\n", accX, accY, accZ);
//		counter = 0;
//	printf("\n\n\nRAW----->\t\tX: %3f   Y: %3f   Z: %3f\n", accX, accY, accZ);
    float32_t readings[4] = {accX,accY,accZ,1};
    arm_mat_init_f32(&IN, 1,4, (float32_t *)readings);
    arm_mat_init_f32(&CAL, 4,3, (float32_t *)calibrationData);
    arm_mat_init_f32(&OUT, 1,3, (float32_t *)output);
    arm_mat_mult_f32(&IN,&CAL,&OUT);
    accX = OUT.pData[0];
    accY = OUT.pData[1];
    accZ = OUT.pData[2];

    //Then FILTER the readings.
    //printf("CLBRT----->\t\tX: %3f   Y: %3f   Z: %3f\n", accX, accY, accZ);
    struct SAMPLE f = filter(accX,accY,accZ);

    //Set global pitch and roll variables.
//	printf("FILTER---->\t\tX: %3f   Y: %3f   Z: %3f\n\n",f.x,f.y,f.z);
	osSemaphoreWait(roll_sem, osWaitForever);
	roll = (double)atan2f(f.x, f.z) * 180.0 / M_PI;
	roll = (float)round(fabsf(roll));
	osSemaphoreRelease(roll_sem);
	
	osSemaphoreWait(pitch_sem, osWaitForever);
    pitch = (double)atan2f((-f.y), sqrtf(f.x * f.x + f.z * f.z)) * 180.0 / M_PI;
	pitch = (float)round(fabsf(pitch));
	osSemaphoreRelease(pitch_sem);

    if (counter == 100){
		HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_13);
        printf("\t\t\t=======PITCH: %3f,ROLL:%3f\n\n",pitch,roll);
        counter = 0;
    }
	counter++;
}



/**
 * Initilizes the accelerometer with the configuration struct
 */
void initializeAccelerometer(void){

    Acc_instance.Axes_Enable				= LIS3DSH_XYZ_ENABLE;
    Acc_instance.AA_Filter_BW				= LIS3DSH_AA_BW_50;
    Acc_instance.Full_Scale					= LIS3DSH_FULLSCALE_2;
    Acc_instance.Power_Mode_Output_DataRate		= LIS3DSH_DATARATE_50; //todo why is this 25?
    Acc_instance.Self_Test					= LIS3DSH_SELFTEST_NORMAL;
    Acc_instance.Continous_Update   = LIS3DSH_ContinousUpdate_Enabled;

    LIS3DSH_Init(&Acc_instance); //there are other parameters configured in this function if needed...

    /* Enabling interrupt conflicts with push button. Be careful when you plan to
    use the interrupt of the accelerometer sensor connceted to PIN A.0
    Acc_interruptConfig.
    */
    Acc_interruptConfig.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;
    Acc_interruptConfig.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
    Acc_interruptConfig.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;
    LIS3DSH_DataReadyInterruptConfig(&Acc_interruptConfig);

    //ENABLE the INTERRUPTS
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    HAL_NVIC_SetPriority(EXTI0_IRQn, 4,4);

}

