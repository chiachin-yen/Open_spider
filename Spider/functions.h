// functions.h

#ifndef _FUNCTIONS_h
#define _FUNCTIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include "spider_main.h"
#include "pins.h"

extern float STPR_COOR[STPR_QTY][2];

extern float currentCursorCoor[2];
extern float currentCursorMountCoor[STPR_QTY][2];
extern float currentBeltLen[STPR_QTY];

extern float deltaCursorCoor[2];
extern float deltaBeltLen[STPR_QTY];

extern float targetCursorCoor[2];
extern float targetBeltLen[STPR_QTY];
extern float targetCursorMountCoor[STPR_QTY][2];

extern unsigned int mainSpeed;
extern uint8_t topSpeed;
extern uint8_t baseSpeed;
extern uint8_t accelSpeed;

inline long step_of(float input) {
	/*Convert mm to steps*/
	return (long)((float)input / SYS_RES);
}
inline int interval_of(int speed) {
	/*
	Convert travel speed(mm/s) into stepper intervals(ms)
	Travel distance per step(TDPS) / intervals = Speed
	-> intervals = 1000'000 * TDPS / Speed (s)
	*/
	return (1000000 * SYS_RES / speed);
}

float distance_2pt(float pt_1[2], float pt_2[2]);
float distance_2pt(Coord pt_1, Coord pt_2);
void update_current_cursor_coor(float curCoor[2]);
void update_target_cursor_coor(float curCoor[2]);
void calc_deltabeltLen();

int speed_cal(int movement_cap, int iteration);
int acc_cal(int it_a);
int dec_cal(int it_d);

#endif

