 
/*
 * randAlgorithm.h
 *
 *  Created on: 19 Mar 2016
 *      Author: Amit
 */
#ifndef RANDALGORITHM_H_
#define RANDALGORITHM_H_

#include <map>
#include <string>
#include <cstdlib>
#include "AbstractAlgorithm.h"
#include "SimpleSensor.h"

using namespace std;

class topDownAlgo: public AbstractAlgorithm {
	const AbstractSensor* sensor;
	map<string, int> config;
	int lastMove;
	int stepsLeft;
public:
	topDownAlgo();
	~topDownAlgo();

	// setSensor is called once when the Algorithm is initialized
	void setSensor(const AbstractSensor& sensor);

	// setConfiguration is called once when the Algorithm is initialized - see below
	void setConfiguration(map<string, int> config);

	// step is called by the simulation for each time unit
	Direction step();

	// this method is called by the simulation either when there is a winner or
	// when steps == MaxSteps - MaxStepsAfterWinner
	// parameter stepsTillFinishing == MaxStepsAfterWinner
	void aboutToFinish(int stepsTillFinishing);
};

#endif /* RANDALGORITHM_H_ */
