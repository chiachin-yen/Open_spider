/*
Stepper class designed for the spider.
It may contains general functions for a 4 wires stepper with driver
*/

#ifndef _STEPPER_h
#define _STEPPER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif


class Stepper
{
	uint8_t stepPin;
	uint8_t dirPin;
	uint8_t enablePin;

	uint8_t minPin;
	uint8_t maxPin;
	uint8_t servoPin;

	uint16_t stepPerUnit;		//Steps per unit

	int32_t position;			//current position in steps
	int32_t positionMin;		//
	int32_t positionMax;

	int32_t targetPosition;		//steps, target position in steps
	uint32_t lastStepTime;		//microsecond, Last system time when stepper step
								//uint32_t nextStepTime;		//microsecond, next system time when stepper step
	uint16_t interval;			//microsecond, steps intervals
	uint16_t speed;				//steps/second must < 25000
	
	
protected:
	

public:
	//static const uint16_t STEP_PER_UNIT;
	int8_t direction;

	Stepper();
	//Step Pin, Direction Pin, Enable Pin, Endstop Pin Min, EndstopPin Max, Servo Pin 
	void initPin(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

	void initBound(const int32_t &, const int32_t &);
	void initStepPerUnit(const uint16_t &);

	void run(int16_t steps, uint8_t intervals);
	void run(uint16_t steps, uint8_t intervals);
	void tick();

	bool run_temp();			//time based tick (non-blocking);
	bool move_to(int32_t);		//move to absolute target position with acceleration
	bool move(int32_t);			//move to relative target position with acceleration

	bool const_speed_move_to();
	bool const_speed_move_to(int32_t);
	bool const_speed_move(int32_t);
	
	void homing(int8_t);		//-1. home to min, 1 home to max

	bool arrived();				//return if stepper arrived target position

	void set_direction(const int8_t &);
	void set_interval(const uint16_t &);
	void set_speed(const uint16_t &);   // mm/sec
	void set_position(const int32_t &); //Set position in steps
	void set_position_u(const int16_t &); //Set position in unit
	void set_targetPosition(const int32_t &);
	void set_targetPosition_u(const int16_t &);

	uint16_t get_stepPerUnit();		//get Steps per unit
	int32_t get_position() const;
	int32_t get_targetPosition() const;

	void enable();
	void disable();
};



#endif

