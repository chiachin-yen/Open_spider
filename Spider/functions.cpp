/*
Regularly used functions.
*/

#include "functions.h"

void planned_movement() {

	//Serial.println("CODE G5 RECEIVED");
	//Serial.println("////////////////////INITIALIZATION START////////////////////");

	float currentCursorCoor_P[2] = { currentCursorCoor[0], currentCursorCoor[1] };
	float targetCursorCoor_P[2] = { targetCursorCoor[0], targetCursorCoor[1] };
	float currentBeltLen_P[STPR_QTY];
	float targetBeltLen_P[STPR_QTY];
	float deltaBeltLen_P[STPR_QTY];
	for (int i = 0; i < STPR_QTY; i++) {
		currentBeltLen_P[i] = currentBeltLen[i];
		targetBeltLen_P[i] = 0;
		deltaBeltLen_P[i] = 0;
	}

	float movement_vector[2] = { (targetCursorCoor_P[0] - currentCursorCoor_P[0]), (targetCursorCoor_P[1] - currentCursorCoor_P[1]) };
	int division = ((sqrt(sq(movement_vector[0]) + sq(movement_vector[1])) + (PATH_SEGMENT_LENGTH / 2)) / PATH_SEGMENT_LENGTH);
	if (division > MAXIMUM_DIVISION) {
		division = MAXIMUM_DIVISION;
	}
	else if (division == 0) {
		division = 1;
	}
	float movement_vector_d[2] = { (movement_vector[0] / division), (movement_vector[1] / division) };
	/*
	Serial.println("currentCursorCoor_P=" + String(currentCursorCoor_P[0]) + "," + String(currentCursorCoor_P[1]));
	Serial.println("targetCursorCoor_P=" + String(targetCursorCoor_P[0]) + "," + String(targetCursorCoor_P[1]));
	Serial.print("currentBeltLen_P=");
	for (int i = 0; i < STPR_AMOUNT; i++) {
	Serial.print(String(currentBeltLen_P[i]) + ",");
	}
	Serial.print("\n");
	Serial.println("movement_vector=" + String(movement_vector[0]) + "," + String(movement_vector[1]));
	Serial.println("Division=" + String(division));
	Serial.println("movement_vector_d=" + String(movement_vector_d[0]) + "," + String(movement_vector_d[1]));
	Serial.println("////////////////////INITIALIZATION   END////////////////////");
	*/
	for (int i = 0; i < division; i++) {
		//Serial.println("--------------------Loop=" + String(i) + "--------------------");
		//Serial.println("currentCursorCoor_P=" + String(currentCursorCoor_P[0]) + "," + String(currentCursorCoor_P[1]));
		targetCursorCoor_P[0] = currentCursorCoor_P[0] + movement_vector_d[0];
		targetCursorCoor_P[1] = currentCursorCoor_P[1] + movement_vector_d[1];
		//Serial.println("TargetCursorCoor=" + String(targetCursorCoor_P[0]) + "," + String(targetCursorCoor_P[1]));
		for (int j = 0; j < STPR_QTY; j++) {
			float targetcursorMountCoor_P[2] = { (targetCursorCoor_P[0] + CURSOR_MOUNT_VECTOR[j][0]), (targetCursorCoor_P[1] + CURSOR_MOUNT_VECTOR[j][1]) };
			//Serial.println("Motor Mount "+String(j)+" Target Coor="+String(targetcursorMountCoor_P[0]) + "," + String(targetcursorMountCoor_P[1]));
			targetBeltLen_P[j] = distance_2pt(targetcursorMountCoor_P, STPR_COOR[j]);
			//Serial.println("Motor "+String(j)+" Target Belt Len=" + String(targetBeltLen_P[j]));
			deltaBeltLen_P[j] = targetBeltLen_P[j] - currentBeltLen_P[j];
			//Serial.println("Motor "+String(j)+" Delta Belt Len=" + String(deltaBeltLen_P[j]));
		}
		CSM(deltaBeltLen_P);
		//Serial.println("Movement Finished");
		for (int j = 0; j < STPR_QTY; j++) {
			currentBeltLen_P[j] = targetBeltLen_P[j];
			//Serial.println("Motor "+String(j)+" Current Belt Len=" + String(currentBeltLen_P[j]));
		}
		currentCursorCoor_P[0] = targetCursorCoor_P[0];
		currentCursorCoor_P[1] = targetCursorCoor_P[1];
	}
}

float distance_2pt(float pt_1[2], float pt_2[2]) {
	/* Caculate the distance between two coordinates. */
	return (sqrt(pow((pt_2[0] - pt_1[0]), 2) + pow((pt_2[1] - pt_1[1]), 2)));
}

float distance_2pt(Coord pt_1, Coord pt_2) {
	return (sqrt(pow((pt_2.x - pt_1.x), 2) + pow((pt_2.y - pt_1.y), 2)));
}

void update_current_cursor_coor(float curCoor[2]) {
	/*
	Update current cursor's position (stored in a array) to given coordinate.
	*/

	currentCursorCoor[0] = curCoor[0];
	currentCursorCoor[1] = curCoor[1];

	for (int i = 0; i < STPR_QTY; i++) {
		currentCursorMountCoor[i][0] = currentCursorCoor[0] + CURSOR_MOUNT_VECTOR[i][0];
		currentCursorMountCoor[i][1] = currentCursorCoor[1] + CURSOR_MOUNT_VECTOR[i][1];
	}
	
	for (int i = 0; i < STPR_QTY; i++) {
		currentBeltLen[i] = distance_2pt(currentCursorMountCoor[i], STPR_COOR[i]);
	}
	//Serial.println("Cursor's current coordinate is { " + String(curCoor[0]) + ", " + String(curCoor[1]) + " }.");
}

void update_target_cursor_coor(float curCoor[2]) {
	/*
	Update target cursor's position (stored in a array) to given coordinate.
	*/

	/* check if target coordinate exceed maxium travel range. */
	/*if ((pow(targetCoor[0], 2) + pow(targetCoor[1], 2)) > pow(MAXIUM_PRINT_RADIUS, 2)) {
	//Activate Coor mapping()
	}*/

	targetCursorCoor[0] = curCoor[0];
	targetCursorCoor[1] = curCoor[1];

	for (int i = 0; i < STPR_QTY; i++) {
		targetCursorMountCoor[i][0] = targetCursorCoor[0] + CURSOR_MOUNT_VECTOR[i][0];
		targetCursorMountCoor[i][1] = targetCursorCoor[1] + CURSOR_MOUNT_VECTOR[i][1];
	}

	for (int i = 0; i < STPR_QTY; i++) {
		targetBeltLen[i] = distance_2pt(targetCursorMountCoor[i], STPR_COOR[i]);
	}
}


void calc_deltabeltLen() {
	for (int i = 0; i < STPR_QTY; i++) {
		deltaBeltLen[i] = targetBeltLen[i] - currentBeltLen[i];
	}
}

int speed_cal(int movement_cap, int iteration) {
	//partial acc/dec mode
	if (movement_cap < (ACC_STEP * 2)) {
		//even mode
		if (movement_cap % 2 == 0) {
			//acc part
			if (iteration <= (movement_cap / 2)) {
				return acc_cal(iteration);
			}
			//dec part
			else {
				return dec_cal((movement_cap - iteration));
			}
		}
		//odd mode
		else {
			if (iteration <= ((movement_cap - 1) / 2)) {
				return acc_cal(iteration);
			}
			else if (iteration > ((movement_cap - 1) / 2 + 1)) {
				return dec_cal((movement_cap - iteration));
			}
			else {
				return acc_cal((iteration - 1));
			}
		}
	}
	//complete acc/dec mode
	else {
		if (iteration <= ACC_STEP) {
			return acc_cal(iteration);
		}
		else if (iteration > (movement_cap - ACC_STEP)) {
			return dec_cal((movement_cap - iteration));
		}
		else {
			return F_S_INTERVAL;
		}
	}
}

int acc_cal(int it_a) {
	//Serial.println(String(it_a));
	//int aaa=(BASE_SPEED + ((ACC_DIST * it_a) / ACC_STEP));
	//Serial.println(String(aaa));
	return ((SYS_RES_INT / (DEFAULT_BASE_SPEED + ((ACC_DIST * it_a) / ACC_STEP))) - DELAY_OFFSET);
}

int dec_cal(int it_d) {
	//Serial.println(String(it_d));
	//int bbb=(BASE_SPEED + ((ACC_DIST * it_d) / ACC_STEP));
	//Serial.println(String(bbb));
	return ((SYS_RES_INT / (DEFAULT_BASE_SPEED + ((ACC_DIST * it_d) / ACC_STEP))) - DELAY_OFFSET);
}
