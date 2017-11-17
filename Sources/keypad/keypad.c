#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "../Gpio/gpio.h"
#include "../StateMachine/state_machine.h"
#include "keypad.h"
#include "../stm32f4xx_it.h"

int columns[3] = {0,0,0};
int rows[4] = {0,0,0,0};
int column = 0;
int row = 0;
int keypressed = 0;
int key = 0;
int keypad_counter = 0;

void switchIO(){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//Output to Input
	GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//Input to Output
	/*Configure GPIO pins : PC4 PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	/*Configure GPIO pins : PB0 PBPin */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

	
	
}

void resetIO(){
	rows[0] = 0;
	rows[1] = 0;
	rows[2] = 0;
	rows[3] = 0;
	
	columns[0] = 0;
	columns[1] = 0;
	columns[2] = 0;
	GPIO_InitTypeDef GPIO_InitStruct;

	/*Configure GPIO pins : PB12 PB13 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);


/*Configure GPIO pins : PC4 PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	/*Configure GPIO pins : PB0 PBPin */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

int getPressedKey(int column, int row){
	switch(column){
		case 1:
			switch(row){
				case 1:
					return 1;
				case 2:
					return 4;
				case 3:
					return 7;
				case 4:
					return 10;
			}
		case 2:
			switch(row){
				case 1:
					return 2;
				case 2:
					return 5;
				case 3:
					return 8;
				case 4:
					return 0;
			}
		case 3:
			switch(row){
				case 1:
					return 3;
				case 2:
					return 6;
				case 3:
					return 9;
				case 4:
					return 11;
			}
	}
	return -1;
}

int readInput(void){
	static int timer_start = 0;
	static int timer_end = 0;

	int a = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4);
	int b = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5);
	int c = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
	if(a == 0 || b == 0 || c == 0){
		if(a == 0){
			column = 1;
		}else if(b == 0){
			column = 2;
		}else if(c == 0){
			column = 3;
		}

		
		switchIO();
		
		int d = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
		int e = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
		int f = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
		int g = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);

		if(d == 0){
			row = 1;
		}else if(e == 0){
			row = 2;
		}else if(f == 0){
			row = 3;
		}else if(g == 0){
			row = 4;
		}
		key = getPressedKey(column, row);
		if(keypressed == 0){
				timer_start = HAL_GetTick();
		}
		keypressed = 1;
		resetIO();
	}else{
		if(keypressed == 1){
			timer_end = HAL_GetTick();
			int duration;
			int keypad_counter = (timer_end - timer_start)/1000;
			if(keypad_counter == 0){
				duration = 0;
			}else if(keypad_counter == 1){
				duration = 1;
			}else if(keypad_counter == 2){
				duration = 2;
			}else if(keypad_counter >= 3){
				duration = 3;
			}
						timer_end = HAL_GetTick();

			printf("key: %d, duration: %d, timer start: %d, timer end: %d, diff: %d\n", key, duration, timer_start, timer_end, (timer_end-timer_start));
			updateState(key, duration);
			keypressed = 0;
			keypad_counter = 0;
		}
		keypad_counter = 0;
		
	}
	return 0;
}

