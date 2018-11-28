/*
Plan the steps queue according to given coordinate.
*/

#include "planner.h"
#include "spider_main.h"
#include "functions.h"
#include "stepper.h"
#include "messages.h"


extern Stepper STPR[STPR_QTY];

Coord currentCursor = setCoord(0, 0);
Coord targrtCursor = setCoord(0, 0);

// MAX BLOCK STEPS = BLOCK_LENGTH / SYS_RES
// MIN BLOCK TIME =  Block_LENGTH / TOP_SPEED
// MIN intervals = 
// JERK_SPEED = 10 mm/s = BASE_SPEED = ACC_SPEED


void operation_planner(Coord &targetCoor) {
	if (targetCoor.x == currentCursor.x && targetCoor.y == currentCursor.y) {
		return;
	}

	float path_length = distance_2pt(currentCursor, targetCoor);
	uint16_t blocks_qty = ceil(path_length / PATH_SEGMENT_LENGTH);
	float block_length = (path_length / (float)blocks_qty);

	Serial.println("path length : " + String(path_length));
	Serial.println("block Qty :" + String(blocks_qty));
	Serial.println("block length :" + String(block_length));

	Coord deltaCursor = coordSub(targetCoor, currentCursor);
	Coord deltaBlock = setCoord(((float)deltaCursor.x / blocks_qty), ((float)deltaCursor.y / blocks_qty));

	uint16_t blockCount = 0;
	uint16_t accelBlock = 0;
	uint16_t decelBlock = 0;
	
	uint16_t fullBlock = ceil((float)topSpeed / baseSpeed) - 1;
	uint16_t block_speed = 0;

	if (blocks_qty >= (fullBlock * 2 + 1)) {
		accelBlock = fullBlock;
		decelBlock = blocks_qty - fullBlock;
	}
	else if (blocks_qty % 2 == 0) {
		decelBlock = blocks_qty / 2;
		accelBlock = decelBlock - 1;
	}
	else {
		decelBlock = (blocks_qty + 1) / 2;
		accelBlock = decelBlock - 1;
	}

	Serial.println("===block index===");
	Serial.println("Total :" + String(blocks_qty));
	Serial.println("ideal acc :" + String(fullBlock));
	Serial.println("Acc until:" + String(accelBlock));
	Serial.println("Decel when :" + String(decelBlock));

	Serial.println("cursor coord : " + String(currentCursor.x) + ", " + String(currentCursor.y));
	Serial.println("===block index===");
	Serial.println();

	Path_Block pathBlock;

	while (blockCount != blocks_qty) {
		if (blockCount <= accelBlock) {
			currentCursor = coordAdd(currentCursor, deltaBlock);
			FOR_LOOP{
				pathBlock.steps[i] = (STPR + i)->calc_length(currentCursor);
			}
			block_speed += baseSpeed;
			pathBlock.blockTime = ceil(1000000 * block_length / block_speed);

			//<>
			Serial.println("No. " + String(blockCount));
			Serial.println("Target Coord : " + String(currentCursor.x) + ", " + String(currentCursor.y));
			Serial.println("AccelBlock");
			Serial.println("Block Speed : " + String(block_speed));
			Serial.println("Block Time : " + String(pathBlock.blockTime));
			Serial.print("Steps Target :");
			FOR_LOOP{
				Serial.print(String(pathBlock.steps[i]) + ",");
			}
			Serial.println();
			//</>

			block_execute(pathBlock);
			blockCount++;
		}
		else if (blockCount < decelBlock) {
			currentCursor = coordAdd(currentCursor, deltaBlock);
			FOR_LOOP{
				pathBlock.steps[i] = (STPR + i)->calc_length(currentCursor);
			}
			block_speed = topSpeed;
			pathBlock.blockTime = ceil(1000000 * block_length / block_speed);

			//<>
			Serial.println("No. " + String(blockCount));
			Serial.println("NominalBlock");
			Serial.println("Block Speed : " + String(block_speed));
			Serial.println("Block Time : " + String(pathBlock.blockTime));
			Serial.print("Steps Target :");
			FOR_LOOP{
				Serial.print(String(pathBlock.steps[i]) + ",");
			}
			Serial.println();
			//</>

			block_execute(pathBlock);
			blockCount++;
		}
		else if (blockCount < blocks_qty - 1) {
			currentCursor = coordAdd(currentCursor, deltaBlock);
			FOR_LOOP{
				pathBlock.steps[i] = (STPR + i)->calc_length(currentCursor);
			}
			block_speed -= baseSpeed;
			pathBlock.blockTime = ceil(1000000 * block_length / block_speed);

			//<monitor>
			Serial.println("No. " + String(blockCount));
			Serial.println("decelBlock");
			Serial.println("Block Speed : " + String(block_speed));
			Serial.println("Block Time : " + String(pathBlock.blockTime));
			Serial.print("Steps Target :");
			FOR_LOOP{
				Serial.print(String(pathBlock.steps[i]) + ",");
			}
			Serial.println();
			//</monitor>

			block_execute(pathBlock);
			blockCount++;
		}
		else {
			currentCursor = targetCoor;
			FOR_LOOP{
				pathBlock.steps[i] = (STPR + i)->calc_length(currentCursor);
			}
			block_speed = baseSpeed;
			pathBlock.blockTime = ceil(1000000 * block_length / block_speed);

			//<>
			Serial.println("No. " + String(blockCount));
			Serial.println("FinalBlock");
			Serial.println("Block Speed : " + String(block_speed));
			Serial.println("Block Time (us): " + String(pathBlock.blockTime));
			Serial.print("Steps Target :");
			FOR_LOOP{
				Serial.print(String(pathBlock.steps[i]) + ",");
			}
			Serial.println();
			//</>

			block_execute(pathBlock);
			blockCount++;
		}
	}
	Serial.println(MSG_ECHO_0);
}

void block_execute(const Path_Block &block) {
	Serial.println("<block section>");
	static int32_t delta_steps[STPR_QTY] = { 0 };
	static uint32_t abs_steps[STPR_QTY] = { 0 };

	/*
	Set direction then convert negative steps into positive.
	*/

	FOR_LOOP{
		*(delta_steps + i) = block.steps[i] - (STPR + i)->get_position();
	}
		FOR_LOOP{
			if (*(delta_steps + i) < 0) {
				*(abs_steps + i) = -*(delta_steps + i);
				(STPR + i)->set_direction(-1);
			}
			else {
				*(abs_steps + i) = *(delta_steps + i);
				(STPR + i)->set_direction(1);
			}
	}
		/*
	Serial.print("Target Steps : ");
	FOR_LOOP{
		Serial.print(String(block.steps[i]) + ", ");
	}Serial.println();

	Serial.print("Now Steps : ");
	FOR_LOOP{
		Serial.print(String((STPR + i)->beltLength) + ", ");
	}Serial.println();

	Serial.print("Delta Steps : ");
	FOR_LOOP{
	Serial.print(String(abs_steps[i]) + ", ");
	}Serial.println();

	Serial.print("Direction ");
	FOR_LOOP{
		Serial.print(String((STPR+i)->direction) + ", ");
	}Serial.println();
	*/

	/*
	Find the largest steps quantity.
	*/
	static uint16_t domain = 0;
	static uint8_t domainID = 0;
	domain = 0;
	FOR_LOOP{
		if (abs_steps[i] > domain) {
			domain = abs_steps[i];
			domainID = i;
		}
	}

	Serial.println("Domain :" + String(domain) + "[" + String(domainID) + "]");

	uint16_t blockInterval = floor(block.blockTime / domain);
	static uint16_t stepper_intervals[STPR_QTY] = { 0 };

	FOR_LOOP{
		*(stepper_intervals + i) = ceil((float)block.blockTime / *(abs_steps + i));
		(STPR + i)->set_interval(*(stepper_intervals + i));
	}

	Serial.print("Delta Steps : ");
	FOR_LOOP{
		Serial.print(String(delta_steps[i]) + ", ");
	}Serial.println();

	Serial.print("Step Interval: ");
	FOR_LOOP{
		Serial.print(String(*(stepper_intervals + i)) + ", ");
	}
	Serial.println();

	static uint32_t lastMoved[STPR_QTY] = { 0 };
	static uint32_t timeS = micros();

	while ((STPR + domainID)->get_position() != block.steps[domainID]) {
		FOR_LOOP{
			(STPR + i)->run_temp();
		// CAUTION : Mysterious Bug Occur
		}
	}

	Serial.println("Block Summary");
	FOR_LOOP{
		Serial.print(String(block.steps[i]) + "(T: " + String((STPR + i)->get_position()) + ")" + ", ");
	}
	Serial.println("Block Error");
	//Serial.println();
	FOR_LOOP{
		int16_t err = (long)(STPR + i)->get_position() - block.steps[i];
		Serial.print(String(err) + ", ");
	}

	Serial.println();
	Serial.println("</block section>");
	Serial.println();
}

bool planner_homing() {
	FOR_LOOP{
		;
	}
	return true;
}