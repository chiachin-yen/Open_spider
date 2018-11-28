// main.h
#ifndef _SPIDER_MAIN_h
#define _SPIDER_MAIN_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "configuration.h"

#include <inttypes.h>

extern const float CURSOR_MOUNT_VECTOR[STPR_QTY][2];

void setup();
void loop();
void serialEvent();

bool code_seen(char x);
int code_value();
float code_value_float();
void run_command();

void cmd_M0();
void cmd_M1();
void cmd_M2();
void cmd_M9();
void cmd_M101();
void cmd_M102();
void cmd_M220();
void cmd_M221();
void cmd_M222();
void cmd_M997();
void cmd_M998();
void cmd_M999();

void cmd_G0();
void cmd_G1();
void cmd_G5();
void cmd_G9();
void cmd_G28();
void cmd_G92();
void cmd_G997();
void cmd_G998();
void cmd_G999();

void CSM(float t_movement[STPR_QTY]);

#endif
