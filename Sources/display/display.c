#include <stdio.h>
#include "stm32f4xx_hal.h"
//#include "gpio.h"
//#include "stm32f4xx_hal_gpio.h"
#include <stm32f407xx.h>


static int DisplayValues[4] = {0,0,0,0};
int DecimalLocation[4] = {0,0,0,0};
int counter_display = 0;

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
//int getDisplayValues(const char *output){
//	//Assume that there will always be one decimal, and that it's not the first element... 'no .0002'
//	int location = 0;
//	int index = 0;
//	for (int i = 0; i < 5; ++i) {
//		//Find the location of the decimal values
//		if(output[i] == '.'){
//			location = i-1;
//			continue;
//		}
//		DisplayValues[index]  = output[i] - '0';
//		index++;
//	}
//	DecimalLocation[location] = 1;
//	for(int i = 0; i < 4; ++i) {
//		if(i != location){
//			DecimalLocation[i] = 0;
//		}
//	}
//	return 0;
//}

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
int setDisplay(const int *pins, int decimal, int panelNumber){
	
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

	//Then set decimal Point
	if (decimal) {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
	}

	//Then set the correct transitor
	activatePanel(panelNumber);
	return 0;
}

int sendValuesToDisplay(int panelToDisplay){
	//Given an integer to display, we want to enable the correct pins.
	
    int DISPLAY_NUMBER_VALUE = DisplayValues[panelToDisplay]; //Get integer value to display
		setDisplay(NUMBERS[DISPLAY_NUMBER_VALUE], DecimalLocation[panelToDisplay], panelToDisplay); //knowing pings, whether to show a decimal, and the display
		return 0;
}

int displayDigits(float value){
	char output[4];
    static int counter = 0;
    if (counter < 10){
        counter = 0;
    } else{
        counter++;
        return 0;
    }
	//First thing is to load the values for RMS VOLTAGE into the display values
	snprintf(output, 50, "%f", value);
	int location = 0;
	int index = 0;
	int i = 0;
	for (i = 0; i < 5; ++i) {
		//Find the location of the decimal values
		if(output[i] == '.'){
			location = i-1;
			continue;
		}
		DisplayValues[index]  = output[i] - '0';
		index++;
	}
	DecimalLocation[location] = 1;
	for(i = 0; i < 4; ++i) {
		if(i != location){
			DecimalLocation[i] = 0;
		}
	}
//	getDisplayValues(output); //Sets global DisplayValues and Decimal location arrays
	sendValuesToDisplay(counter_display);
	counter_display++;
	/*
		Counter to determine which digit to display
	*/
	if(counter_display == 4){
		counter_display = 0;
	}
	return 0;
}
