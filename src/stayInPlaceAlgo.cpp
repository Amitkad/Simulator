 
#include "stayInPlaceAlgo.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

stayInPlaceAlgo::stayInPlaceAlgo(){
	 lastMove = 0;
	 srand(time(NULL));//for randomization matters
}

stayInPlaceAlgo::~stayInPlaceAlgo() {
	delete sensor;
}

void stayInPlaceAlgo::setSensor(const AbstractSensor& sensor) {
	this->sensor = &sensor;
}

void stayInPlaceAlgo::setConfiguration(map<string, int> config) {
	this->config = config;
}

Direction stayInPlaceAlgo::step() {
  //stay in place
	    return Direction(4);
	}
	
}

void stayInPlaceAlgo::aboutToFinish(int stepsTillFinishing) {
} //TODO



 