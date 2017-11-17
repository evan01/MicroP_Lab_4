#include <stdio.h>
#include <string.h>
#include "state_machine.h"
#include "../Threads.h"


int target_roll;
char roll_buf[10];
int roll_pointer = 0;
int target_pitch;
char pitch_buf[10];
int pitch_pointer = 0;
float value = 0.000;
float placeholder_value = 0.000;

state_e state = START_STATE;
state_e next_state;
press_type_e press_type;
event_e event;


/*
0 -> roll
1 -> pitch
*/

int printState(state_e state){
	switch(state){
		case START_STATE:
			printf("START_STATE\n");
			break;
		case SLEEP_STATE:
			printf("SLEEP_STATE\n");
			break;
		case ENTER_ROLL_STATE:
			printf("ENTER_ROLL_STATE\n");
			break;
		case ENTER_PITCH_STATE:
			printf("ENTER_PITCH_STATE\n");
			break;
		case PITCH_MONITOR_STATE:
			printf("PITCH_MONITOR_STATE\n");
			break;
		case ROLL_MONITOR_STATE:
			printf("ROLL_MONITOR_STATE\n");
			break;
		case TARGET_PITCH_STATE:
			printf("TARGET_PITCH_STATE\n");
			break;
		case TARGET_ROLL_STATE:
			printf("TARGET_ROLL_STATE\n");
			break;
	}
	return 0;
}

void updateAngle(event_e event, int pitch_roll){
	if(pitch_roll == 0){
		sprintf(roll_buf + roll_pointer, "%d", event);
		printf("%s\n", roll_buf);
		roll_pointer++;
	}else{
		sprintf(pitch_buf + pitch_pointer, "%d", event);
		printf("%s\n", pitch_buf);
		pitch_pointer++;
	}
}

void clearingLastDigit(int pitch_roll){
	if(pitch_roll == 0){
		roll_pointer--;
		sprintf(roll_buf + roll_pointer, "\0");
		printf("%s\n", roll_buf);
	}else{
		pitch_pointer--;
		sprintf(pitch_buf + pitch_pointer, "\0");
		printf("%s\n", pitch_buf);
	}
}


int setPressType(int duration){
	printf("Press Type: ");
	switch(duration){
		case 0: 
			press_type = REGULAR;  
			printf("REGULAR, ");
			break;
		case 1:
			press_type = MID_PRESS;
			printf("MID_PRESS, ");
			break;
		case 2:
			press_type = MID_PRESS;
			printf("MID_PRESS, ");
			break;
		case 3:
			press_type = LONG_PRESS;
			printf("LONG_PRESS, ");
			break;
		default:
			press_type = REGULAR;
			printf("REGULAR, ");
			break;
	}
	return 0;
}

int set_state(){
	printf("Initial State: ");
	printState(state);

	if(state == START_STATE){
		memset(roll_buf, 0, 10);
		memset(pitch_buf, 0, 10);
		roll_pointer = 0;
		pitch_pointer = 0;
		state = ENTER_ROLL_STATE;
//		sscanf(roll_buf, "%f", &placeholder_value);
	}
	if(press_type == LONG_PRESS){
		if(event == STAR){
			printf("going to SLEEP_STATE\n");
			next_state = SLEEP_STATE;
		}else if(event == HASHTAG && state == SLEEP_STATE){
			printf("going to START_STATE\n");
			next_state = START_STATE;
		}else{
			next_state = state;
		}
	}else if(press_type == MID_PRESS){
		printf("Going back to START_STATE  %d \n", event);
		if(event == STAR && state != SLEEP_STATE){
			next_state = START_STATE;
			memset(roll_buf, 0, 10);
			memset(pitch_buf, 0, 10);
			roll_pointer = 0;
			pitch_pointer = 0;
		}else{
			next_state = state;
		}
	}else{
		switch(state){
			case START_STATE:
				next_state = ENTER_ROLL_STATE;
			case SLEEP_STATE:
				next_state = SLEEP_STATE;
				
				break;
			case ENTER_ROLL_STATE:
				if(event == HASHTAG){
					next_state = ENTER_PITCH_STATE;
					// sprintf(roll_buf + roll_pointer, "\0");
					sscanf(roll_buf, "%d", &target_roll);
					printf("Final roll = %d\n", target_roll);
				}else if(event == STAR){
					//clear last digit
					printf("Clearing digit from roll\n");
					next_state = ENTER_ROLL_STATE;
					clearingLastDigit(0);
				}else{
					//add number to roll variable
					int digit;
					if(event == NUMBER_0){
						digit = 0;
					}else{
						digit = event-1;
					}
					printf("Adding digit %d\n", digit);
					next_state = ENTER_ROLL_STATE;
					updateAngle(digit, 0);
				}
				break;
			case ENTER_PITCH_STATE:
				if(event == HASHTAG){
					next_state = PITCH_MONITOR_STATE;
					// sprintf(pitch_buf + pitch_pointer, "\0");
					sscanf(pitch_buf, "%d", &target_pitch);
					printf("Final pitch = %d\n", target_pitch);
				}else if(event == STAR){
					//clear last digit
					printf("Clearing digit from pitch\n");
					next_state = ENTER_PITCH_STATE;
					clearingLastDigit(1);
				}else{
					//add number to pitch variable
					int digit;
					if(event == NUMBER_0){
						digit = 0;
					}else{
						digit = event-1;
					}
					printf("Adding digit %d\n", digit);
					next_state = ENTER_PITCH_STATE;
					updateAngle(digit, 1);
				}
				break;
			case PITCH_MONITOR_STATE:
//				if(event == NUMBER_1){
//					next_state = PITCH_MONITOR_STATE;
//				}else if(event == NUMBER_2){
//					next_state = ROLL_MONITOR_STATE;
//				}else if(event == HASHTAG){
//					next_state = TARGET_PITCH_STATE;
//				}else{
//					next_state = PITCH_MONITOR_STATE;
//				}
			
				if(event == HASHTAG){
					next_state = TARGET_PITCH_STATE;
				}	else{
					next_state = PITCH_MONITOR_STATE;
				}
				break;
			case ROLL_MONITOR_STATE:
//				if(event == NUMBER_1){
//					next_state = PITCH_MONITOR_STATE;
//				}else if(event == NUMBER_2){
//					next_state = ROLL_MONITOR_STATE;
//				}else if(event == HASHTAG){
//					next_state = TARGET_ROLL_STATE;
//				}else{
//					next_state = ROLL_MONITOR_STATE;
//				}
				if(event == HASHTAG){
					next_state = TARGET_ROLL_STATE;
				}	else{
					next_state = ROLL_MONITOR_STATE;
				}
				break;
			case TARGET_PITCH_STATE:
//				if(event == NUMBER_1){
//					next_state = TARGET_PITCH_STATE;
//				}else if(event == NUMBER_2){
//					next_state = ROLL_MONITOR_STATE;
//				}else if(event == HASHTAG){
//					next_state = PITCH_MONITOR_STATE;
//				}else{
//					next_state = TARGET_PITCH_STATE;
//				}
			
				if(event == HASHTAG){
					next_state = ROLL_MONITOR_STATE;
				}	else{
					next_state = TARGET_PITCH_STATE;
				}
				break;
			case TARGET_ROLL_STATE:
//				if(event == NUMBER_1){
//					next_state = PITCH_MONITOR_STATE;
//				}else if(event == NUMBER_2){
//					next_state = TARGET_ROLL_STATE;
//				}else if(event == HASHTAG){
//					next_state = ROLL_MONITOR_STATE;
//				}else{
//					next_state = TARGET_ROLL_STATE;
//				}
			
				if(event == HASHTAG){
					next_state = PITCH_MONITOR_STATE;
				}	else{
					next_state = TARGET_ROLL_STATE;
				}
				break;
		}
	}
	osSemaphoreWait(state_sem, osWaitForever);
	state = next_state;
	osSemaphoreRelease(state_sem);
	
	printf("End State: ");
	printState(state);
	return 0;
}

int setEvent(int digit){
	printf("Event: ");
	switch(digit){
		case 0:
			event = NUMBER_0;
			printf("NUMBER_0\n");
			break;
		case 1:
			event = NUMBER_1;
			printf("NUMBER_1\n");
			break;
		case 2:
			event = NUMBER_2;
			printf("NUMBER_2\n");
			break;
		case 3:
			event = NUMBER_3;
			printf("NUMBER_3\n");
			break;
		case 4:
			event = NUMBER_4;
			printf("NUMBER_4\n");
			break;
		case 5:
			event = NUMBER_5;
			printf("NUMBER_5\n");
			break;
		case 6:
			event = NUMBER_6;
			printf("NUMBER_6\n");
			break;
		case 7:
			event = NUMBER_7;
			printf("NUMBER_7\n");
			break;
		case 8:
			event = NUMBER_8;
			printf("NUMBER_8\n");
			break;
		case 9:
			event = NUMBER_9;
			printf("NUMBER_9\n");
			break;
		case 10:
			event = STAR;
			printf("STAR\n");
			break;
		case 11:
			event = HASHTAG;
			printf("HASHTAG\n");
			break;
	}
	return 0;
}

int updateState(int digit, int duration){
	printf("digit: %d, duration: %d\n", digit, duration);
	setPressType(duration);
	setEvent(digit);
	set_state();
	return 0;
}
