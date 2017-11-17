#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"

osSemaphoreId state_sem;
osSemaphoreDef(state_sem);