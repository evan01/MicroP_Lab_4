#include <stdio.h>
#include "stm32f4xx_hal.h"
#include <stm32f407xx.h>

//SAMPLE NUMBER = {A,B,C,D,E,F,G}
int NUMBER_0[7] = {1,1,1,1,1,1,0};
int NUMBER_1[7] = {0,1,1,0,0,0,0};
int NUMBER_2[7] = {1,1,0,1,1,0,1};
int NUMBER_3[7] = {1,1,1,1,0,0,1};
int NUMBER_4[7] = {0,1,1,0,0,1,1};
int NUMBER_5[7] = {1,0,1,1,0,1,1};
int NUMBER_6[7] = {1,0,1,1,1,1,1};
int NUMBER_7[7] = {1,1,1,0,0,0,0};
int NUMBER_8[7] = {1,1,1,1,1,1,1};
int NUMBER_9[7] = {1,1,1,1,0,1,1};

//Array of pointers to pointers.
int *NUMBERS[10] = {
		NUMBER_0,
		NUMBER_1,
		NUMBER_2,
		NUMBER_3,
		NUMBER_4,
		NUMBER_5,
		NUMBER_6,
		NUMBER_7,
		NUMBER_8,
		NUMBER_9
};

/*
	Gets all the digits and dots to be displayed from the number array
*/


/*
	Activates the common cathode of the corresponding panel
*/
int activatePanel(int panelNumber){

/*
	TODO: CHANGE PIN ACCORDING TO NEW CONFIGURATION
	*/	
	
    if(panelNumber == 0){
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
    }else if(panelNumber == 1){
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
    }else if(panelNumber == 2){
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
    }else if(panelNumber == 3){
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
    }
		return 0;
}
void resetDisplay(void){
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
}
/*
	Activates the corresponding anode segments depending on the given pins and decimal boolean
*/
int setDisplay(const int *pins){
	
	//Activates pins first
	if (pins[0] == 1) {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
	}

	if (pins[1] == 1) {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
	}

	if (pins[2] == 1) {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
	}

	if (pins[3] == 1) {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
	}

	if (pins[4] == 1) {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
	}

	if (pins[5] == 1) {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
	}

	if (pins[6] == 1) {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
	}

	return 0;
}

int displayDigits(int value){
	static int counter_display = 0;
	static int limit = 4;
	if(value >= 1000 && limit != 4){
		limit = 4;
		counter_display = 0;
	}else if(value > 100 && value < 1000 && limit != 3){
		limit = 3;
		counter_display = 0;
	}else if(value > 10 && value < 100 && limit != 2){	
		limit = 2;
		counter_display = 0;
	}else if(value >= 0 && value < 10 && limit != 1){
		limit = 1;
		counter_display = 0;
	}
	char output[4];
	int int_output[4];
	//Convert integer input to 4 element int array
	snprintf(output, 50, "%d", value);
	for (int i=0;i<4;i++){
		int_output[i] = output[i] - '0';
	}
	
	//Set the Number pins to point to the correct value
	setDisplay(NUMBERS[int_output[counter_display]]);
	//The the panel pins to point to the correct pannel
	activatePanel(counter_display);
	counter_display++;
	
	if(counter_display == limit){
		counter_display = 0;
	}
	return 0;
}
