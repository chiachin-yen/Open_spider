/*
Under Construction
*/

#include "stepper.h"

Stepper::Stepper() {
	stepPin = -1;
	dirPin = -1;
	enablePin = -1;
	minPin = -1;
	maxPin = -1;
	servoPin = -1;

	stepPerUnit = 200;
	position = 0;
	targetPosition = position;
	positionMin = 0;
	positionMax = 2147483647;
	lastStepTime = 0;
	//nextStepTime = 0;
	interval = 100;
}

void Stepper::initPin(uint8_t stepPin_i, uint8_t dirPin_i,
	uint8_t enablePin_i, uint8_t minPin_i,
	uint8_t maxPin_i, uint8_t servoPin_i) {
	stepPin = stepPin_i;
	dirPin = dirPin_i;
	enablePin = enablePin_i;
	minPin = minPin_i;
	maxPin = maxPin_i;
	servoPin = servoPin_i;
}

void Stepper::initBound(const int32_t &min_i, const int32_t &max_i) {
	if (min_i > max_i) {
		positionMin = max_i;
		positionMax = min_i;
	}
	else {
		positionMin = min_i;
		positionMax = max_i;
	}
}

void Stepper::initStepPerUnit(const uint16_t &spu_i) {
	stepPerUnit = spu_i;
}

void Stepper::run(int16_t steps, uint8_t interval) {
	if (steps < 0) { //	Check if the target travel distance is positive or negative.
		steps = -steps;
		digitalWrite(dirPin, LOW);
		for (int16_t i = 0; i <= steps; i++) {
			digitalWrite(stepPin, LOW);
			digitalWrite(stepPin, HIGH);
			delayMicroseconds(interval);
		}
		position -= steps; //Caution : 
	}
	else {
		digitalWrite(dirPin, HIGH);
		for (int16_t i = 0; i <= steps; i++) {
			digitalWrite(stepPin, LOW);
			digitalWrite(stepPin, HIGH);
			delayMicroseconds(interval);
		}
		position += steps;
	}
}

void Stepper::run(uint16_t steps, uint8_t interval) {
	for (int16_t i = 0; i <= steps; i++) {
		digitalWrite(stepPin, LOW);
		digitalWrite(stepPin, HIGH);
		delayMicroseconds(interval);
	}
	position += steps;

}

void Stepper::tick() {
	digitalWrite(stepPin, LOW);
	digitalWrite(stepPin, HIGH);
	position += direction;
}

bool Stepper::run_temp() {
	uint32_t time = micros();
	uint32_t nextStepTime = lastStepTime + interval;

	// Gymnastics to detect wrapping of either the nextStepTime and/or the current time
	if (((nextStepTime >= lastStepTime) && ((time >= nextStepTime) || (time < lastStepTime)))
		|| ((nextStepTime < lastStepTime) && ((time >= nextStepTime) && (time < lastStepTime))))
	{
		digitalWrite(stepPin, LOW);
		digitalWrite(stepPin, HIGH);
		position += direction;
		lastStepTime = time;
		//Serial.println("S");
		return true;
	}
	else {
		return false;
	}
}

bool Stepper::move_to(int32_t abs_position) {
	targetPosition = abs_position;

	if (position == targetPosition) {
		return false;
	}
	else if (position < targetPosition) {
		set_direction(1);
	}
	else {
		set_direction(-1);
	}

	while (arrived() == false) {
		run_temp();
	}
	return true;
}

bool Stepper::move(int32_t relative_position) {
	if (relative_position == 0) {
		return false;
	}
	else if (relative_position > 0) {
		set_direction(1);
	}
	else {
		set_direction(-1);
	}

	targetPosition = position + relative_position;
	while (arrived() == false) {
		run_temp();
	}
	return true;
}

bool Stepper::const_speed_move_to() {
	while (!arrived()) {
		run_temp();
	}
	return true;
}

bool Stepper::const_speed_move_to(int32_t abs_position) {
	//set_targetPosition
}

bool Stepper::const_speed_move(int32_t abs_position) {

}

void Stepper::homing(int8_t home_direction) {
	const uint16_t current_interval = interval;
	interval = 100;

	if (home_direction <= 0) {
		set_direction(-1);
		while (digitalRead(minPin) == HIGH) {
			tick();
			delayMicroseconds(interval);
			//position -= direction;  // hold position parameter.
		}
		position = positionMin;
	}
	else {
		set_direction(1);
		while (digitalRead(minPin) == HIGH) {
			tick();
			delayMicroseconds(interval);
			//position -= direction;  // hold position parameter.
		}
		position = positionMax;
	}

	interval = current_interval;
}

bool Stepper::arrived() {
	if (position == targetPosition) {
		return true;
	}
	else {
		return false;
	}
}

void Stepper::set_direction(const int8_t &dir) {
	if (dir < 0) {
		digitalWrite(dirPin, LOW);
		direction = -1;
	}
	else {
		digitalWrite(dirPin, HIGH);
		direction = 1;
	}
}

void Stepper::set_interval(const uint16_t &i_interval) {
	if (i_interval < 40) {
		interval = 40;  //minimum interval
	}
	else {
		interval = i_interval;
	}
}

void Stepper::set_speed(const uint16_t &i_speed) {
	uint16_t i_interval = 1000000 / (i_speed * stepPerUnit);
	set_interval(i_interval);
}

void Stepper::set_position(const int32_t &position_temp) {
	targetPosition = position = position_temp;
}

void Stepper::set_position_u(const int16_t &position_temp) {
	targetPosition = position = position_temp * stepPerUnit;
}

void Stepper::set_targetPosition(const int32_t &position_temp) {
	targetPosition = position_temp;
}

void Stepper::set_targetPosition_u(const int16_t &position_temp) {
	targetPosition = position_temp * stepPerUnit;
}

uint16_t Stepper::get_stepPerUnit() {
	return stepPerUnit;
}

int32_t Stepper::get_position() const {
	return position;
}

int32_t Stepper::get_targetPosition() const {
	return targetPosition;
}

void Stepper::enable() {
	digitalWrite(enablePin, LOW);
}

void Stepper::disable() {
	digitalWrite(enablePin, HIGH);
}
