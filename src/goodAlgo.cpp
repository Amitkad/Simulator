 

 
#include "topDownAlgo.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

goodAlgo::goodAlgo(){
	 srand(time(NULL));//for randomization matters
	 
}

goodAlgo::~goodAlgo() {
	delete sensor;
}

void goodAlgo::setSensor(const AbstractSensor& sensor) {
	this->sensor = &sensor;
}

void goodAlgo::setConfiguration(map<string, int> config) {
	this->config = config;
}

Direction goodAlgo::step() {
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

void goodAlgo::aboutToFinish(int stepsTillFinishing) {
  stepsLeft = stepsTillFinishing;
} 

//return instance for .so matters
extern "C" AbstractAlgorithm* getAbstractAlgorithmPointer()
{
    return new goodAlgo();
}
