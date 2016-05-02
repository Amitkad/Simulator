
/*
 * randAlgorithm.h
 *
 *  Created on: 19 Mar 2016
 *      Author: Amit
 */
#ifndef _201191897_A_H_
#define _201191897_A_H_

#include <map>
#include <string>
#include <cstdlib>
#include <vector>
#include "../AbstractAlgorithm.h"
#include "../SimpleSensor.h"

using namespace std;

class _201191897_A: public AbstractAlgorithm {
	const AbstractSensor* sensor;
	map<string, int> config;
	int stepsLeft;
	
	vector<int> stepsMade; // empty vector of ints
	int amountOfStepsMade;
public:
	_201191897_A();
	~_201191897_A();
	_201191897_A& operator=(const _201191897_A&) = delete;
	_201191897_A(const _201191897_A&) = delete;

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
