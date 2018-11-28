/*
Store all settings which shouldn't be changed
after upload to Arduino.
*/

#ifndef _CONFIGURATION_h
#define _CONFIGURATION_h
#include <inttypes.h>

//General Settings
#define SYS_NAME			"Spider"	
#define MOTHERBOARD			2	//Infill 1 = RAMPS 1.4 or 2 = RAMPS-FD
/*
1 = Arduino MEGA + RAMPS 1.4
2 = Arduino DUE + RAMPS-FD
*/

//Communication Settings
#define BAUD				250000
#define BUFSIZE				64
#define READY_SIGNAL		"@\n"
#define REQUEST_SIGNAL		"?\n"

//Stepper Settings
#define STPR_RESOLUTION		200     // Steps in a full rotation
#define STPR_PULLEY_TOOTH	20		// Tooth Per Round
#define BELT_PITCH_LENGTH	2       // Pitch length (mm)
#define STPR_MICRO_STEP		16      // Micro Steps division 1,2,4,8,16

//#define SYS_UNIT_MULTIPLIER 1000000 //Scale system unit to avoid float point caculation
#define SYS_RES				((float)STPR_PULLEY_TOOTH * BELT_PITCH_LENGTH / (STPR_RESOLUTION * STPR_MICRO_STEP))
#define SYS_RES_INT			((int)(1000000*SYS_RES)) //1,000,000*SYS_RES
//#define STEP_FACTOR			1


#define STPR_QTY         5		// Quantity of steppers

//Speed Settings

#define DEFAULT_SPEED		50		// Default travel speed at mm/s
#define DEFAULT_TOP_SPEED	200		//in mm/sec
#define DEFAULT_BASE_SPEED	5	  	//in mm/sec
#define ACC_DURATION		0.5		//time for acceleration or deceleration in seconds
#define ACC_STEP			(int)((float)((DEFAULT_TOP_SPEED+DEFAULT_BASE_SPEED)*ACC_DURATION)/(2*SYS_RES)) //steps used for acceleration/deceleration
#define ACC_DIST			(int)((float)(DEFAULT_TOP_SPEED-DEFAULT_BASE_SPEED))// speed difference between top and base speed
#define OFFSET_DIST			1000

//Delay Settings
#define DELAY_OFFSET		53 //Offset for Acc/Dec
			
#define F_S_INTERVAL		((int)((SYS_RES_INT/DEFAULT_TOP_SPEED)-37))//Full Speed Interval w/ offset
#define CYCLE_INTERVAL		7.6 //Motor_Move Cycle time


//Coordinate Settings
/* Coordinates Variables located in spider_main.cpp. */
#define MAXIMUM_PRINT_RADIUS 600 //Max raius setting for round shape

//Planner Settings
#define PATH_SEGMENT_LENGTH 2.0
#define CALIBRATION_FREQUENCY 10
#define MAXIMUM_DIVISION     (MAXIMUM_PRINT_RADIUS*2)/PATH_SEGMENT_LENGTH

//IDE Macros

#define FOR_LOOP	for(size_t i = 0; i != STPR_QTY; i++)

//note
/*
  int8_t	     -127 ~ 127
 unit8_t	        0 ~ 255
 int16_t       -32765 ~ 32765
unit16_t	        0 ~ 65535
 int32_t  -2147483647 ~ 2147483647
unit32_t            0 ~ 4294967295
*/

typedef struct {
	float x;
	float y;
}Coord;

inline Coord setCoord(const float (&coord)[2]) {
	Coord temp;
	temp.x = coord[0];
	temp.y = coord[1];
	return temp;
}

inline Coord setCoord(const float &coord_x, const float &coord_y) {
	Coord temp;
	temp.x = coord_x;
	temp.y = coord_y;
	return temp;
}

inline Coord coordAdd(const Coord &coord_1, const Coord &coord_2) {
	Coord temp;
	temp.x = coord_1.x + coord_2.x;
	temp.y = coord_1.y + coord_2.y;
	return temp;
}

inline Coord coordSub(const Coord &coord_1, const Coord &coord_2) {
	Coord temp;
	temp.x = coord_1.x - coord_2.x;
	temp.y = coord_1.y - coord_2.y;
	return temp;
}

//

#endif
