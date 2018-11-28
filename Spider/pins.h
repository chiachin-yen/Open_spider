#include "configuration.h"
#if MOTHERBOARD == 1

/*
Arduino Mega pin assignment for RAMPS1.4
*/

#define S0_STEP_PIN         54
#define S0_DIR_PIN          55
#define S0_ENABLE_PIN       38

#define S1_STEP_PIN         60
#define S1_DIR_PIN          61
#define S1_ENABLE_PIN       56

#define S2_STEP_PIN         46
#define S2_DIR_PIN          48
#define S2_ENABLE_PIN       62

#define S3_STEP_PIN         26
#define S3_DIR_PIN          28
#define S3_ENABLE_PIN       24

#define S4_STEP_PIN         36
#define S4_DIR_PIN          34
#define S4_ENABLE_PIN       30

//Servo Motors
#define SERVO_0_PIN			-1
#define SERVO_1_PIN			-1
#define SERVO_2_PIN			-1
#define SERVO_3_PIN			-1
#define SERVO_4_PIN			-1
#define SERVO_5_PIN			-1

#define S0_MIN_PIN          -1
#define S0_MAX_PIN          -1
#define S1_MIN_PIN          -1
#define S1_MAX_PIN          -1
#define S2_MIN_PIN          -1
#define S2_MAX_PIN          -1
#define S3_MIN_PIN          -1
#define S3_MAX_PIN          -1
#define S4_MIN_PIN          -1
#define S4_MAX_PIN          -1
#define S5_MIN_PIN          -1
#define S5_MAX_PIN          -1

#endif

#if MOTHERBOARD == 2
/*
Arduino DUE pin assignment for RAMPS FD
*/

#define S0_STEP_PIN         63
#define S0_DIR_PIN          62
#define S0_ENABLE_PIN       48
#define S0_MIN_PIN          22
#define S0_MAX_PIN          30   //2 //Max endstops default to disabled "-1", set to commented value to enable.

#define S1_STEP_PIN         65
#define S1_DIR_PIN          64
#define S1_ENABLE_PIN       46
#define S1_MIN_PIN          24
#define S1_MAX_PIN          38   //15

#define S2_STEP_PIN         67
#define S2_DIR_PIN          66
#define S2_ENABLE_PIN       44
#define S2_MIN_PIN          26
#define S2_MAX_PIN          34

#define S3_STEP_PIN        36
#define S3_DIR_PIN         28
#define S3_ENABLE_PIN      42
#define S3_MIN_PIN         26
#define S3_MAX_PIN         26

#define S4_STEP_PIN        43
#define S4_DIR_PIN         41
#define S4_ENABLE_PIN      39
#define S4_MIN_PIN         26
#define S4_MAX_PIN         26

#define S5_STEP_PIN        32
#define S5_DIR_PIN         47
#define S5_ENABLE_PIN      45
#define S5_MIN_PIN         26
#define S5_MAX_PIN         26


//Servo Motors
#define SERVO_0_PIN			3
#define SERVO_1_PIN			3
#define SERVO_2_PIN			3
#define SERVO_3_PIN			3
#define SERVO_4_PIN			3
#define SERVO_5_PIN			3

#endif