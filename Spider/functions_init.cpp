/*
The functions executed once when start up.
*/

#include "functions_init.h"
#include "configuration.h"
#include "pins.h"
#include "stepper.h"

extern Stepper STPR[STPR_QTY];

const uint8_t STPR_STEP_PINS[STPR_QTY] = {
	S0_STEP_PIN, S1_STEP_PIN, S2_STEP_PIN,
	S3_STEP_PIN, S4_STEP_PIN
};
const int STPR_DIR_PINS[STPR_QTY] = {
	S0_DIR_PIN, S1_DIR_PIN, S2_DIR_PIN,
	S3_DIR_PIN, S4_DIR_PIN
};
const int STPR_ENABLE_PINS[STPR_QTY] = {
	S0_ENABLE_PIN, S1_ENABLE_PIN, S2_ENABLE_PIN,
	S3_ENABLE_PIN, S4_ENABLE_PIN
};
const int STPR_MIN_PINS[STPR_QTY] = {
	S0_MIN_PIN, S1_MIN_PIN, S2_MIN_PIN,
	S3_MIN_PIN, S4_MIN_PIN
};
const int STPR_MAX_PINS[STPR_QTY] = {
	S0_MAX_PIN, S1_MAX_PIN, S2_MAX_PIN,
	S3_MAX_PIN, S4_MAX_PIN
};
const int SERVO_PINS[STPR_QTY] = {
	SERVO_0_PIN, SERVO_1_PIN, SERVO_2_PIN,
	SERVO_3_PIN, SERVO_4_PIN
};

void init_pins() {
	/*
	Initiate PIN mode
	*/

	//Set input pins to INPUT mode

	int input_pins[] = {
		S0_MIN_PIN, S0_MAX_PIN,
		S1_MIN_PIN, S1_MAX_PIN,
		S2_MIN_PIN, S2_MAX_PIN,
		S3_MIN_PIN, S3_MAX_PIN,
		S4_MIN_PIN, S4_MAX_PIN,
		S5_MIN_PIN, S5_MAX_PIN
	};

	int input_len = sizeof(input_pins) / sizeof(input_pins[0]);
	for (int i = 0; i < input_len; i++) {
		pinMode(input_pins[i], INPUT);
	}
	Serial.println("Set " + String(input_len) + " input pins.");

	/* Set output pins to OUTPUT mode. */
	int output_pins[] = {
	  S0_STEP_PIN, S0_DIR_PIN, S0_ENABLE_PIN, SERVO_0_PIN,
	  S1_STEP_PIN, S1_DIR_PIN, S1_ENABLE_PIN, SERVO_1_PIN,
	  S2_STEP_PIN, S2_DIR_PIN, S2_ENABLE_PIN, SERVO_2_PIN,
	  S3_STEP_PIN, S3_DIR_PIN, S3_ENABLE_PIN, SERVO_3_PIN,
	  S4_STEP_PIN, S4_DIR_PIN, S4_ENABLE_PIN, SERVO_4_PIN

	};

	int output_len = sizeof(output_pins) / sizeof(output_pins[0]);
	for (int i = 0; i < output_len; i++) {
		pinMode(output_pins[i], OUTPUT);
		digitalWrite(output_pins[i], HIGH);
	}
	Serial.println("Set " + String(output_len) + " output pins.");
}

float STPR_COORD[STPR_QTY][2] = {         //Set steppers' coordinate here.
	{	      0.0,         800 } ,
	{  760.845213,  247.213595 } ,
	{  470.228202, -647.213595 } ,
	{ -470.228202, -647.213595 } ,
	{ -760.845213,  247.213595 }
};

float MOUNT_COORD[STPR_QTY][2] = {
	{          0,         56 },
	{  53.259165,  17.304952 },
	{  32.915974, -45.304952 },
	{ -32.915974, -45.304952 },
	{ -53.259165,  17.304952 },
};

void create_stepper_instances() {
	FOR_LOOP{
		(STPR + i)->initCoord(STPR_COORD[i], MOUNT_COORD[i]);
		(STPR + i)->initPin(STPR_STEP_PINS[i], STPR_DIR_PINS[i], STPR_ENABLE_PINS[i], STPR_MIN_PINS[i], STPR_MAX_PINS[i], SERVO_PINS[i]);
		(STPR + i)->initStepPerUnit(1 / SYS_RES);
	}
}