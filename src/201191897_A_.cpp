 

 
#include "201191897_A_.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

_201191897_A::_201191897_A(){
	 srand(time(NULL));//for randomization matters
	 
}

_201191897_A::~_201191897_A() {
	delete sensor;
}

void _201191897_A::setSensor(const AbstractSensor& sensor) {
	this->sensor = &sensor;
}

void _201191897_A::setConfiguration(map<string, int> config) {
	this->config = config;
}

Direction _201191897_A::step() {
    int dir=-1;
    if(stepsMade.size() >= stepsLeft-1){
      dir = stepsMade.end();
      stepsMade.resize(stepsMade.size()-1);
      return Direction(dir);
    }else{
      if(!(sensor->sense().isWall[0]) && dir==-1){
	dir = 0;
      }else{
	if(!(sensor->sense().isWall[2]) && dir==-1){
	dir = 2;
	}else{
	  if(!(sensor->sense().isWall[1]) && dir==-1){
	    dir = 1;
	  }else{
	    if(!(sensor->sense().isWall[3]) && dir==-1){
	      dir = 3;
	    }else{
	      dir=4;
	    }
	  }
	}
      }
    }
    stepsMade.push_back(dir);
    return dir;
}

void _201191897_A::aboutToFinish(int stepsTillFinishing) {
  stepsLeft = stepsTillFinishing;
} 

//return instance for .so matters
extern "C" AbstractAlgorithm* getAbstractAlgorithmPointer()
{
    return new _201191897_A();
}
