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
	isWall[static_cast<int>(Direction::East)] = (matrix[*yLocation ][*xLocation+ 1] == 'W' || matrix[*yLocation ][*xLocation+ 1] == 'w');
	isWall[static_cast<int>(Direction::West)] =
			(matrix[*yLocation][*xLocation-1] == 'W'||matrix[*yLocation][*xLocation-1] == 'w');
	isWall[static_cast<int>(Direction::South)] =
			(matrix[*yLocation+1][*xLocation] == 'W'||matrix[*yLocation+1][*xLocation] == 'w');
	isWall[static_cast<int>(Direction::North)] =
			(matrix[*yLocation-1][*xLocation] == 'W'||matrix[*yLocation-1][*xLocation] == 'w');
	SensorInformation currentSensorInfo;
	currentSensorInfo.dirtLevel = dirtLevel;
	for (int i = 0; i < 4; i++) {
		currentSensorInfo.isWall[i] = isWall[i];
	}
	return currentSensorInfo;
}
