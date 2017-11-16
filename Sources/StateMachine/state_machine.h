#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

typedef enum {
	SLEEP_STATE,
	START_STATE,
	ENTER_ROLL_STATE,
	ENTER_PITCH_STATE,
	PITCH_MONITOR_STATE,
	ROLL_MONITOR_STATE,
	TARGET_PITCH_STATE,
	TARGET_ROLL_STATE
}state_e;

typedef enum {
	HASHTAG,
	STAR,
	NUMBER_1,
	NUMBER_2,
	NUMBER_3,
	NUMBER_4,
	NUMBER_5,
	NUMBER_6,
	NUMBER_7,
	NUMBER_8,
	NUMBER_9,
	NUMBER_0
}event_e;

typedef enum {
	REGULAR,
	MID_PRESS,
	LONG_PRESS
} press_type_e;

extern float value;

extern press_type_e press_type;
extern event_e event;
extern state_e state;

extern char pitch_buf[10];
extern char roll_buf[10];

extern int target_roll;
extern int target_pitch;

int updateState(int digit, int duration);


#endif
