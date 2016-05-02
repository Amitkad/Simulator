
 
#include "201191897_B_.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

_201191897_B::_201191897_B(){
	 lastMove = 0;
	 srand(time(NULL));//for randomization matters
}

_201191897_B::~_201191897_B() {
	delete sensor;
}

void _201191897_B::setSensor(const AbstractSensor& sensor) {
	this->sensor = &sensor;
}

void _201191897_B::setConfiguration(map<string, int> config) {
	this->config = config;
}

Direction _201191897_B::step() {
	int dir;
	
	if(stepsLeft <= 2 && char(sensor->sense().dirtLevel) + '0' == 'D'){
	  return Direction(4);//stay
	}
	if(lastMove == 0){
	  if(!sensor->sense().isWall[3]){ lastMove = 3; return Direction(3); }
	  if(!sensor->sense().isWall[2]){ lastMove = 2; return Direction(2); }
	}
	if(lastMove==2){
	  //wentDown
	  if(!sensor->sense().isWall[3]){
	    	  lastMove=3;
		  return Direction(3);
	  }else{
	    lastMove=4;
	    return Direction(4);
	    
	  }
	}else{
	  if(lastMove==3){
	    //wentUp
	    if(!sensor->sense().isWall[2]){
	    	  lastMove=2;
		  return Direction(2);
	  }else{
	    lastMove=4;
	    return Direction(4);
	    
	  }
	  }else{
	    //stayed
	    return Direction(4);
	  }
	}
	
}

void _201191897_B::aboutToFinish(int stepsTillFinishing) {
  stepsLeft = stepsTillFinishing;
} //TODO

//return instance for .so matters
extern "C" AbstractAlgorithm* getAbstractAlgorithmPointer()
{
    return new _201191897_B();
}
