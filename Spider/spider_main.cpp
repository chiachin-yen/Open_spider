/*
The main part of the firmawre.
*/

//#include <string.h>


#include "functions_init.h"
#include "functions.h"
#include "messages.h"
#include "pins.h"
#include "stepper.h"
#include "planner.h"

String inputString = "";			//A String stores charecters received from serial.
bool inputStrComplete = false;		//A boolean to mark whether the input is finished or not. 

char commandToDo[BUFSIZE];			//The completed command waiting for being processed.
static char *cmdPtr;				//A pointer mark on one certain character in the string.

Stepper STPR[STPR_QTY];				//Stepper instances
//Stepper *STPptr = STPR;			  //STPR is a pointer already
extern Coord currentCursor;

/*
Coordinate Constants
*/
float STPR_COOR[STPR_QTY][2] = {         //Set steppers' coordinate here.
	{         0.0,         800 } ,
	{  760.845213,  247.213595 } ,
	{  470.228202, -647.213595 } ,
	{ -470.228202, -647.213595 } ,
	{ -760.845213,  247.213595 }
};
extern const float CURSOR_MOUNT_VECTOR[STPR_QTY][2] = {
	{		   0,         56},
	{  53.259165,  17.304952},
	{  32.915974, -45.304952},
	{ -32.915974, -45.304952},
	{ -53.259165,  17.304952},
};

/*
Coordinate Variables
*/
float currentCursorCoor[2] = { 0, 0 };
float currentBeltLen[STPR_QTY] = { 0,0,0,0,0 };
float currentCursorMountCoor[STPR_QTY][2] = {
	{          0,         56 },
	{  53.259165,  17.304952 },
	{  32.915974, -45.304952 },
	{ -32.915974, -45.304952 },
	{ -53.259165,  17.304952 },
};

float deltaCursorCoor[2] = { 0,0 };
float deltaBeltLen[STPR_QTY] = { 0,0,0,0,0 };

float targetCursorCoor[2] = { 0,0 };
float targetBeltLen[STPR_QTY] = { 0,0,0,0,0 };
float targetCursorMountCoor[STPR_QTY][2] = {
	{          0,         56 },
	{  53.259165,  17.304952 },
	{  32.915974, -45.304952 },
	{ -32.915974, -45.304952 },
	{ -53.259165,  17.304952 },
};
long DistDiff[STPR_QTY] = { 0 };
long mvCrr[STPR_QTY] = { 0 };
/*
Speed Variables
*/
unsigned int mainSpeed = DEFAULT_SPEED;
uint8_t topSpeed = DEFAULT_TOP_SPEED;
uint8_t baseSpeed = DEFAULT_BASE_SPEED;
uint8_t accelSpeed = DEFAULT_BASE_SPEED;

//int textX = 100;


/*
Status Toggle
*/

bool queue_mode = false;

/*
Core Function
*/

void setup() {
	Serial.begin(BAUD);
	inputString.reserve(BUFSIZE);
	Serial.println(SYS_NAME);
	init_pins();
	update_current_cursor_coor(currentCursorCoor); //Calculate belt length
	create_stepper_instances();
	delay(100);
	Serial.println("System Activated");


	/*
	Following lines are for test
	*/
}

void loop() {
	if (inputStrComplete) {
		inputString.trim();			//Remove any leading and trailing whitespace
		strcpy(commandToDo, inputString.c_str()); //Convert string into a character array
		run_command();

		inputString = "";			//clear string stored in inputString
		inputStrComplete = false;   //Reset 
	}
}

void serialEvent() {
	/*
	Store the incoming transmission character by character
	until received a new line code.
	*/
	while (Serial.available()) {
		char inChar = (char)Serial.read();
		if (inChar == '\n' || inChar == '\r') {
			inputStrComplete = true;
		}
		else {
			inputString += inChar;
		}
	}
}

/*
Common funtions for interpreting received command string.
*/
bool code_seen(char x) {
	/* Find certain character in a character array. */

	cmdPtr = strchr(commandToDo, x);
	return (cmdPtr != NULL);
}
int code_value() {
	/* Convert the numbers in string into an integer.*/

	cmdPtr++;  //Shift pointer a character to skip the prefix
	int value = strtol(cmdPtr, NULL, 10);
	return value;
}
float code_value_float() {
	/* Convert the numbers in string into a float. */

	cmdPtr++;    //Shift pointer a character to skip the prefix
	float value = strtod(cmdPtr, NULL);
	return value;
}

void run_command() {
	/*
	Interprete command string then decide which function should be call.
	*/

	cmdPtr = &commandToDo[0];  //Create a pointer point to the address of received command.

	switch (commandToDo[0]) {
	case 'M':
		switch (code_value()) {
		case 0:
			cmd_M0();
			break;
		case 1:
			cmd_M1();
			break;
		case 2:
			cmd_M2();
			break;
		case 9:
			cmd_M9();
			break;
		case 101:
			cmd_M101();
			break;
		case 102:
			cmd_M102();
			break;
		case 220:
			cmd_M220();
			break;
		case 221:
			cmd_M221();
			break;
		case 997:
			cmd_M997();
			break;
		case 998:
			cmd_M998();
			break;
		case 999:
			cmd_M999();
			break;
		default:
			Serial.println(MSG_ERR_BadCmd);
			break;
		}
		break;

	case 'G':
		switch (code_value()) {
		case 0:
			cmd_G0();
			break;
		case 1:
			cmd_G1();
			break;
		case 5:
			cmd_G5();
			break;
		case 9:
			cmd_G9();
			break;
		case 28:
			cmd_G28();
			break;
		case 92:
			cmd_G92();
			break;
		case 997:
			cmd_G997();
			break;
		case 998:
			cmd_G998();
			break;
		case 999:
			cmd_G999();
			break;
		default:
			Serial.println(MSG_ERR_BadCmd);
			break;
		}
		break;

	default:
		Serial.println(MSG_ERR_BadCmd);
		break;
	}

	if (queue_mode) {
		Serial.print(REQUEST_SIGNAL);
	}
}

/*
Gcode Index

  M0   - Check if Arduino is available, respond a READY_SIGNAL
  M1   - turn on Queue mode
  M2   - turn off queue mode
  M9   - Drive one certain stepper with given speed and distance.(M9 T0 H000.0 S000)
  M220 - Set Main Speed
  M221 - Set Top Speed
  M222 - Set Base Speed
  M101 - unlock all Stepper
  M102 - lock all stepper
  M997 - Testing Command
  M998 - Testing Command
  M999 - Testing Command

  G0   - Fast linear Move
  G1   - Linear Move
  G92  - Force Set Coordinate


  G997 - CSM move
  G998 - Display Coordinate and belt length
  G999 -

*/

/*
M prefixed commands
*/
void cmd_M0() {
	Serial.write('@');
}

void cmd_M1() {
	queue_mode = true;
	Serial.write(READY_SIGNAL);
	delay(10);
}

void cmd_M2() {
	queue_mode = false;
}

void cmd_M9() {
	/*Drive one certain stepper with given speed and distance.*/

	int stepperID;
	float travelDis;
	int localSpeed = mainSpeed;

	if (code_seen('T')) { //Find Tool argument
		stepperID = code_value();
		if (stepperID >= STPR_QTY) {
			Serial.println(MSG_ERR_BadArg);
			return;
		}
	}
	else {
		Serial.println(MSG_ERR_NoArg);
		return;
	}

	if (code_seen('H')) { //Find offset argument
		travelDis = code_value();
	}
	else {
		Serial.println(MSG_ECHO_E);
		return;
	}

	if (code_seen('S')) { //Find speed argument
		localSpeed = code_value();
	}

	(STPR + stepperID)->set_speed(localSpeed);
	(STPR + stepperID)->set_targetPosition((STPR + stepperID)->get_position() + (1 / SYS_RES) * travelDis);

	while (!(STPR + stepperID)->arrived()) {
		(STPR + stepperID)->run_temp();
	}

	Serial.println(MSG_ECHO_0);
}

void cmd_M101() {
	//unlock_all_steppers();
}

void cmd_M102() {
	//lock_all_steppers();
}

void cmd_M220()
/*
Set main speed
*/
{
	if (code_seen('S')) {
		mainSpeed = code_value();
	}
	else {
		mainSpeed = DEFAULT_SPEED; //if leave S value empty, then restore default
	}
	Serial.println("Speed Set : " + String(mainSpeed) + "mm/s");
}

void cmd_M221()
/*
Set top speed
*/
{
	if (code_seen('S')) {
		topSpeed = code_value();
	}
	else {
		mainSpeed = DEFAULT_TOP_SPEED; //if leave S value empty, then restore default
	}
	Serial.println("Top Speed Set : " + String(topSpeed) + "mm/s");
}

void cmd_M222() {
	/*
	Set base speed
	*/
	if (code_seen('S')) {
		baseSpeed = code_value();
	}
	else {
		mainSpeed = DEFAULT_BASE_SPEED; //if leave S value empty, then restore default
	}
	Serial.println("Base Speed Set : " + String(baseSpeed) + "mm/s");
}

void cmd_M997() {
	Serial.println(String(STPR->get_position()));
}

void cmd_M998() {
	if (code_seen('X')) {
		STPR->enable();
		STPR->move_to(code_value_float() * STPR->get_stepPerUnit());
		STPR->disable();
	}
}

void cmd_M999() {
	//STPR->set_targetPosition(STPR->get_position + 600);
	STPR->enable();
	STPR->homing(-1);
	STPR->disable();
	Serial.println(MSG_ECHO_0);
}


/*
G prefixed commands
*/

void cmd_G0() {
	float tarCoor[2] = { 0,0 };
	if (code_seen('X')) {
		tarCoor[0] = code_value_float();
	}
	else {
		tarCoor[0] = currentCursorCoor[0];
	}

	if (code_seen('Y')) {
		tarCoor[1] = code_value_float();
	}
	else {
		tarCoor[1] = currentCursorCoor[1];
	}

	//operation_planner(tarCoor);
}
void cmd_G1() {
	/*
	Drive cursor to given destination coordinate.
	*/

	if (code_seen('X')) {
		targetCursorCoor[0] = code_value_float();
	}
	else {
		targetCursorCoor[0] = currentCursorCoor[0];
	}

	if (code_seen('Y')) {
		targetCursorCoor[1] = code_value_float();
	}
	else {
		targetCursorCoor[1] = currentCursorCoor[1];
	}

	update_target_cursor_coor(targetCursorCoor); //need to refine
	calc_deltabeltLen();

	for (int i = 0; i < STPR_QTY; i++) {
		Serial.print(String(deltaBeltLen[i]) + ',');
	}
	Serial.println();
	//CSM(deltaBeltLen);
	update_current_cursor_coor(targetCursorCoor);
}

void cmd_G5() {
	/*
	Drive cirsor to given destination coordinate.
	*/

	if (code_seen('X')) {
		targetCursorCoor[0] = code_value_float();
	}
	else {
		targetCursorCoor[0] = currentCursorCoor[0];
	}

	if (code_seen('Y')) {
		targetCursorCoor[1] = code_value_float();
	}
	else {
		targetCursorCoor[1] = currentCursorCoor[1];
	}

	update_target_cursor_coor(targetCursorCoor); //need to refine

	calc_deltabeltLen();

	/*
	for (int i = 0; i < STPR_AMOUNT; i++) {
		//Serial.print(String(deltaBeltLen[i]) + ',');
	}
	*/
	Serial.println();
	//planned_movement();
	update_current_cursor_coor(targetCursorCoor);
}

void cmd_G9() {
	float tarCoor[2] = { 0,0 };
	if (code_seen('X')) {
		tarCoor[0] = code_value_float();
	}
	else {
		tarCoor[0] = currentCursorCoor[0];
	}

	if (code_seen('Y')) {
		tarCoor[1] = code_value_float();
	}
	else {
		tarCoor[1] = currentCursorCoor[1];
	}

	//operation_planner(tarCoor);
}

void cmd_G28() {
	if (planner_homing()) {
		Serial.println("MSG_ECHO_HM");
	}
	else {

	}
}

void cmd_G92() {
	/*
	update the cursor's coordinate to {0,0} or given number.
	*/

	float setCurCoor[2] = { 0, 0 };

	if (code_seen('X')) { //Find X coordinate to apply
		setCurCoor[0] = code_value();
	}

	if (code_seen('Y')) {
		setCurCoor[1] = code_value();
	}
	update_current_cursor_coor(setCurCoor);
	currentCursor = setCoord(setCurCoor[0], setCurCoor[1]);
	FOR_LOOP{
		(STPR + i)->set_position(currentCursor);
	}

	Serial.println(MSG_ECHO_0);
}

void cmd_G997() {
	float temp_movement[STPR_QTY] = { 0, 0 ,0 ,0 ,0 };

	if (code_seen('A')) { //Find X coordinate to apply
		temp_movement[0] = code_value_float();
	}

	if (code_seen('B')) { //Find X coordinate to apply
		temp_movement[1] = code_value_float();
	}

	if (code_seen('C')) { //Find X coordinate to apply
		temp_movement[2] = code_value_float();
	}

	if (code_seen('D')) { //Find X coordinate to apply
		temp_movement[3] = code_value_float();
	}

	if (code_seen('E')) { //Find X coordinate to apply
		temp_movement[4] = code_value_float();
	}

	//CSM(temp_movement);
}

void cmd_G998() {
	Serial.println(String(currentCursor.x) + "," + String(currentCursor.y));
	FOR_LOOP{
		Serial.print(String((STPR + i)->get_position()) + ',');
	}
	Serial.println("");
}

void cmd_G999() {

	Coord temp_targetCoord = setCoord(0, 0);

	if (code_seen('X')) { //Find X coordinate to apply
		temp_targetCoord.x = code_value_float();
	}

	if (code_seen('Y')) {
		temp_targetCoord.y = code_value_float();
	}
	FOR_LOOP{
		(STPR + i)->enable();
	}
	operation_planner(temp_targetCoord);

	/*
	STPR->set_direction(-1);
	uint16_t XX = 3200;
	STPR->run(XX, 100);
	Serial.println(MSG_ECHO_0);
	*/
}

/*
void CSM(float t_movement[STPR_QTY]) {

	//following codes for testing

	long millisstart;
	long millisfin;
	Serial.println();

	//codes above for testing


	millisstart = millis();
	long movement[STPR_QTY];
	for (int i = 0; i < STPR_QTY; i++) {
		movement[i] = step_of(t_movement[i]);
		//Serial.print("STPR "+String(i)+" ="+"movement: "+String(int(movement[i]))+"Crr: "+String(int(mvCrr[i]))+",");
		movement[i] +=mvCrr[i]; //calculate the steps to move
		//Serial.print("Corrected: "+String(movement[i]));
		mvCrr[i] = 0;
		DistDiff[i] += (long(t_movement[i]*1000000) - (movement[i])*SYS_RES_INT);
		//Serial.print("DistDiff: " + String(DistDiff[i]));
		if (int(DistDiff[i]) >= SYS_RES_INT || int(DistDiff[i]) <= SYS_RES_INT) {
			//Serial.print("Dist Crr: 1");
			movement[i] += (DistDiff[i] / long(SYS_RES_INT));
			//Serial.print("Corrected: " + String(movement[i]));
			DistDiff[i] = DistDiff[i] % SYS_RES_INT;
			//Serial.print("DistDiff: " + String(DistDiff[i]));
		}
		else {
			//Serial.print("Dist Crr: 0");
			//Serial.print("Corrected: " + String(movement[i]));
			//Serial.print("DistDiff: " + String(DistDiff[i]));
		}
		//Serial.print("\n");
	}
	//Serial.println();
	long movement_abs[STPR_QTY];
	long movement_dir[STPR_QTY];

	//find direction
	for (int i = 0; i < STPR_QTY; i++) {
		if (movement[i] > 0) {
			movement_dir[i] = 1;
			movement_abs[i] = movement[i];
		}
		else if (movement[i] < 0) {
			movement_dir[i] = -1;
			movement_abs[i] = -movement[i];
		}
		else {
			movement_dir[i] = 0;
			movement_abs[i] = 0;
		}
	}

	for (int i = 0; i < STPR_QTY; i++) { //Set Stepper Direction
		set_stepper_dir(i, movement_dir[i]);
	}

	long cap = 0;
	for (int i = 0; i < STPR_QTY; i++) {
		if (movement_abs[i] > cap) { //find the maximum movement in the array
			cap = movement_abs[i];
		}
	}

	//initialize "moved steps" array
	long moved_steps[STPR_QTY];
	for (int i = 0; i < STPR_QTY; i++) {
		moved_steps[i] = 0;
	}

	// find the modder array
	long mod[STPR_QTY];
	for (int i = 0; i < STPR_QTY; i++) {
		if (movement_abs[i] == 0) {
			mod[i] = 999999 * OFFSET_DIST;
		}
		else {
			mod[i] = ((OFFSET_DIST * cap / movement_abs[i]));
		}
	}

	for (int i = 0; i < STPR_QTY; i++) {
		digitalWrite(STPR_ENABLE_PINS[i], LOW);
	}

	//movement
	for (long i = 1; i <= cap; i++) {
		for (int j = 0; j < STPR_QTY; j++) {
			if (movement_abs[j] != 0) {
				if (i * OFFSET_DIST >= moved_steps[j] * mod[j]) {
					stepper_tick(j);
					moved_steps[j]++;
				}
				else {
					delayMicroseconds(CYCLE_INTERVAL);
				}
			}
			else {
				delayMicroseconds(CYCLE_INTERVAL);
			}
		}
		delayMicroseconds(speed_cal(cap, i));
	}

	//check on movement
	Serial.print("Movement check:");
	for (int i = 0; i < STPR_QTY; i++) {
		mvCrr[i] = movement_dir[i]*(movement_abs[i] - moved_steps[i]);
	}
	millisfin = millis();
	Serial.println("Movement Duration=" + String(millisfin-millisstart));
	//
	for (int i = 0; i < STPR_AMOUNT; i++) {
		digitalWrite(STPR_ENABLE_PINS[i], HIGH);
	}
	Serial.println(MSG_ECHO_0);
}

*/