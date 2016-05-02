 
#include "201191897_C_.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

_201191897_C::_201191897_C(){
	 srand(time(NULL));//for randomization matters
}

_201191897_C::~_201191897_C() {
	delete sensor;
}

void _201191897_C::setSensor(const AbstractSensor& sensor) {
	this->sensor = &sensor;
}

void _201191897_C::setConfiguration(map<string, int> config) {
	this->config = config;
}

Direction _201191897_C::step() {
  //stay in place
	    return Direction(4);
	}
	


void _201191897_C::aboutToFinish(int stepsTillFinishing) {
} //TODO


//return instance for .so matters
extern "C" AbstractAlgorithm* getAbstractAlgorithmPointer()
{
    return new _201191897_C();
}

 
