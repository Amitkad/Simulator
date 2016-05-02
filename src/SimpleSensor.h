#ifndef SIMPLESENSOR_H
#define SIMPLESENSOR_H

#include "AbstractSensor.h"
#include "Direction.h"

using namespace std;


class SimpleSensor: public AbstractSensor {
	char** matrix;
	const int* xLocation;
	const int* yLocation;
public:
	/*
	 constructor
	 @param
	 _matrix - the matrix for the sensor to know the environment
	 _x - index for the sensor to know where to "sense" 
	 _y - index for the sensor to know where to "sense" 
	 */
	SimpleSensor(char** _matrix, const int* _x, const int* _y);

	/*
	 a function that returns the status around the "cleaning robot" 
	 @return 
	 returns the sensor information .. an int for the value of dust under the robot and the existance of adjacent walls  
	 */
	SensorInformation sense() const;

	/*
	 destructor
	 */
	~SimpleSensor();
	//no need for operator= and copy c'tor
	SimpleSensor& operator=(const SimpleSensor&)=delete;
	SimpleSensor(const SimpleSensor&) = delete;
};

#endif
