#include "SimpleSensor.h"
#include <iostream>
#include <stdio.h>

using namespace std;
//should get pointer to the matrix , pointer to x location and pointer to the y location all from the simulator
SimpleSensor::SimpleSensor(char** _matrix, const int* _x, const int* _y) :
		matrix(_matrix), xLocation(_x), yLocation(_y) {}

SimpleSensor::~SimpleSensor(){

}

// returns the sensor's information of the current location of the robot
SensorInformation SimpleSensor::sense() const {

	int dirtLevel = matrix[*yLocation][*xLocation]-'0';
	bool isWall[4];
	isWall[static_cast<int>(Direction::East)] =
			(matrix[*yLocation ][*xLocation+ 1] == 'W'); //should work when compiling with 11
	isWall[static_cast<int>(Direction::West)] =
			(matrix[*yLocation][*xLocation-1] == 'W');
	isWall[static_cast<int>(Direction::South)] =
			(matrix[*yLocation+1][*xLocation] == 'W');
	isWall[static_cast<int>(Direction::North)] =
			(matrix[*yLocation-1][*xLocation] == 'W');
	SensorInformation currentSensorInfo;
	currentSensorInfo.dirtLevel = dirtLevel;
	for (int i = 0; i < 4; i++) {
		currentSensorInfo.isWall[i] = isWall[i];
	}
//	printf("(%d,%d) : (%d,%d,%d,%d)",*xLocation,*yLocation,currentSensorInfo.isWall[1],currentSensorInfo.isWall[3],currentSensorInfo.isWall[0],currentSensorInfo.isWall[2]);
	return currentSensorInfo;
}

//a d'tor is implemented in abstract class already
//SimpleSensor::~SimpleSensor() {
//}
