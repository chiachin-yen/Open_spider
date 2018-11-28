// planner.h

#ifndef _PLANNER_h
#define _PLANNER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "configuration.h"

extern uint8_t topSpeed;
extern uint8_t baseSpeed;
extern uint8_t accelSpeed;

typedef struct {
	uint32_t steps[STPR_QTY];
	uint32_t blockTime;
}Path_Block;

void operation_planner(Coord &targetCoor);
void block_execute(const Path_Block &);

bool planner_homing();
inline uint16_t speed_calc(uint16_t blockCountTemp, const uint16_t &accelBlockTemp, const uint16_t &decelBlockTemp) {
	if(blockCountTemp <= (accelBlockTemp)){
		return baseSpeed + blockCountTemp * accelSpeed;
	}else if (blockCountTemp < decelBlockTemp){
		return topSpeed;
	}
	else {
		return topSpeed - (blockCountTemp - decelBlockTemp)*accelSpeed;
	}
}

inline uint8_t blockspeed(uint16_t , uint16_t); // Simplify anatomy of planner.

#endif

