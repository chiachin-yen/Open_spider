// function_init.h

#ifndef _FUNCTION_INIT_h
#define _FUNCTION_INIT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "spider_main.h"

void init_pins(void);
void create_stepper_instances(void);

#endif

