/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __timer_H
#define __timer_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
	 
/* Prototypes ----------------------------------------------------------------*/
void initTimer(void);
void setLedIntensity(uint32_t duty, uint32_t channel);
void setLedIntensityPitch(uint32_t duty);
void setLedIntensityRoll(uint32_t duty);

#endif
