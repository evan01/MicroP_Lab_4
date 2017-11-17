#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"

extern osSemaphoreId state_sem;
extern osSemaphoreId pitch_sem;
extern osSemaphoreId roll_sem;
extern osThreadId tid_Thread_Display;                              // thread id
extern osThreadId tid_Thread_Keypad;                              // thread id
extern osThreadId tid_Thread_LED;                              // thread id
