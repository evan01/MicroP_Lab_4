#include "tim.h"

// Obtained from https://github.com/owen12123/MicroProcessorSystem/blob/master/LAB3_Accelerometer/lab3base/lab3_stm32f4cube_base_project/timer.c
TIM_HandleTypeDef tim4handle;
TIM_OC_InitTypeDef pwmConf;
TIM_Base_InitTypeDef tim4Config;
uint32_t period = 42000;
	
void initTimer(void)
{
	tim4Config.Prescaler= (uint32_t)2;
	tim4Config.CounterMode=TIM_COUNTERMODE_UP;
	tim4Config.Period =period;
	

	tim4handle.Instance = TIM4;
	tim4handle.Init = tim4Config;
	tim4handle.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	

	pwmConf.OCMode = TIM_OCMODE_PWM1;
	pwmConf.Pulse = 50; //start off
	
	/*ENABLE CLOCKS */
	HAL_TIM_PWM_MspInit(&tim4handle);
	HAL_TIM_PWM_Init(&tim4handle);
	HAL_TIM_PWM_ConfigChannel(&tim4handle, &pwmConf, TIM_CHANNEL_1 );
	HAL_TIM_PWM_ConfigChannel(&tim4handle, &pwmConf, TIM_CHANNEL_2 );
	HAL_TIM_PWM_ConfigChannel(&tim4handle, &pwmConf, TIM_CHANNEL_3 );
	HAL_TIM_PWM_ConfigChannel(&tim4handle, &pwmConf, TIM_CHANNEL_4 );
	HAL_TIM_PWM_Start(&tim4handle, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&tim4handle, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&tim4handle, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&tim4handle, TIM_CHANNEL_4);
	
	
}

void setLedIntensity(uint32_t duty, uint32_t channel)
{
	pwmConf.Pulse=duty*tim4Config.Period/100;
	HAL_TIM_PWM_ConfigChannel(&tim4handle, &pwmConf, channel );
	HAL_TIM_PWM_Start(&tim4handle, TIM_CHANNEL_1);
}

void setLedIntensityPitch(uint32_t duty)
{
	pwmConf.Pulse=duty*tim4Config.Period/100;
	HAL_TIM_PWM_ConfigChannel(&tim4handle, &pwmConf, TIM_CHANNEL_4 );
	HAL_TIM_PWM_ConfigChannel(&tim4handle, &pwmConf, TIM_CHANNEL_2 );
	HAL_TIM_PWM_Start(&tim4handle, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&tim4handle, TIM_CHANNEL_2);
}

void setLedIntensityRoll(uint32_t duty)
{
	pwmConf.Pulse=duty*tim4Config.Period/100;
	HAL_TIM_PWM_ConfigChannel(&tim4handle, &pwmConf, TIM_CHANNEL_3 );
	HAL_TIM_PWM_ConfigChannel(&tim4handle, &pwmConf, TIM_CHANNEL_1 );
	HAL_TIM_PWM_Start(&tim4handle, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&tim4handle, TIM_CHANNEL_1);
}


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{
	__TIM4_CLK_ENABLE();
	
	//GPIO CONFIG
	__GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef dpins_Init;
	dpins_Init.Pin = GPIO_PIN_12 | GPIO_PIN_13 |GPIO_PIN_14 |GPIO_PIN_15;	//D12 is green LED
	dpins_Init.Mode= GPIO_MODE_AF_PP; //hopefully doesnt burn LEDs xD
	//dpins_Init.Pull= GPIO_NOPULL;
	dpins_Init.Speed= GPIO_SPEED_FREQ_MEDIUM;
	dpins_Init.Alternate = GPIO_AF2_TIM4;
	HAL_GPIO_Init(GPIOD,&dpins_Init);
	
}
