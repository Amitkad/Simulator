 

 
#include "201191897_A_.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

_201191897_A::_201191897_A(){
	stepsLeft=9999999;
	stepsMade; // empty vector of ints
	amountOfStepsMade = 0;	 
}

_201191897_A::~_201191897_A() {
	//delete sensor;
}

void _201191897_A::setSensor(const AbstractSensor& sensor) {
	this->sensor = &sensor;
}

void _201191897_A::setConfiguration(map<string, int> config) {
	this->config = config;
}

Direction _201191897_A::step() {
    int dir=-1;
    struct SensorInformation sensed = sensor->sense();
    if(stepsMade.size() >= stepsLeft-1 && stepsMade.size()>0){
      int back[5] = {1,0,3,2,4};
      dir = back[stepsMade.back()];//permute
      stepsMade.resize(stepsMade.size()-1);
      return Direction(dir);
    }else{
      if(!(sensed.isWall[0]) && dir==-1){
	dir = 0;
      }else{
	if(!(sensed.isWall[2]) && dir==-1){
	dir = 2;
	}else{
	  if(!(sensed.isWall[1]) && dir==-1){
	    dir = 1;
	  }else{
	    if(!(sensed.isWall[3]) && dir==-1){
	      dir = 3;
	    }else{
	      dir=4;
	    }
	  }
	}
      }
    }
    stepsMade.push_back(dir);
    return (Direction)dir;
}

void _201191897_A::aboutToFinish(int stepsTillFinishing) {
  stepsLeft = stepsTillFinishing;
} 

//return instance for .so matters
extern "C" AbstractAlgorithm* getAbstractAlgorithmPointer()
{
    return new _201191897_A();
}
