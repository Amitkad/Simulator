#include <iostream>
#include <algorithm>
#include <cstring>

#include "Simulator.h"
#include <vector>

using namespace std;
//CleanAlgorithm algorithm;
Simulator::Simulator(vector<House*> _houses, const map<string, int> _config,
		vector<AbstractAlgorithm*> _algorithms , int _housesAmount , int _algorithmsAmount) :
		config(_config) {
	algorithmsAmount = _algorithmsAmount;
	housesAmount = _housesAmount;
	maxSteps = _config.find("MaxSteps")->second;
	maxStepsAfterWinner = _config.find("MaxStepsAfterWinner")->second;
	batteryCapacity = _config.find("BatteryCapacity")->second;
	batteryConsumptionRate = _config.find("BatteryConsumptionRate")->second;
	batteryRechargeRate = _config.find("BatteryRechargeRate")->second;
	simulationSteps = 0;
	winnerSteps = 0;
	currentHouse = 0;
	
	scores = (int**) malloc(sizeof(int*) * _algorithmsAmount);
	for(int i = 0 ; i < _algorithmsAmount ; i++){
	  scores[i] = (int*) malloc(sizeof(int) * _housesAmount);
	  for(int j = 0 ; j < _housesAmount ; j++){
	    scores[i][j] = 0;
	  }
	}
	
	houses = _houses;
	stepsMade = (int*) malloc(sizeof(int) * _algorithmsAmount);
	batteryUsed = (int*) malloc(sizeof(int) * _algorithmsAmount);
	dustAmountInHome = (int*) malloc(sizeof(int) * _algorithmsAmount);
	currentYPos = (int*) malloc(sizeof(int) * _algorithmsAmount);
	currentXPos = (int*) malloc(sizeof(int) * _algorithmsAmount);
	
	
	int amountOfDustInFirstHouse = (this->houses.begin())->initDustAmount(); // should be first house 
	int dockX = (_houses.begin())->getDockStationX();
	int dockY = (_houses.begin())->getDockStationY();
	
	
	for(int i = 0 ; i < _algorithmsAmount ; i++){
	  stepsMade[i] = 0;
	  batteryUsed[i] = 0;
	  dustAmountInHome[i] = amountOfDustInFirstHouse;
	  currentYPos[i] = dockY;
	  currentXPos[i] = dockX;
	}
	
	

	algorithms = _algorithms;
	
	//todo fix matrixes for each algo .
	matrixes = (char***) malloc(sizeof(char**)*_algorithmsAmount);
	for(int j=0 ; j < _algorithmsAmount ; j++){
	  matrixes[j] = (char**) malloc(sizeof(char*) * _house->getRowCount());
	  for (int i = 0; i < _house->getRowCount(); ++i) {
	    matrixes[j][i] = (char*) malloc(sizeof(char) * _house->getColCount());
	    memcpy(matrixes[j][i], _house->getMatrix()[i], _house->getColCount());
	  }
	}


}

Simulator::~Simulator(){
	delete algorithm;
	delete house;
}

void Simulator::run() {

  int stepsMadeAfterWinner = (int*)malloc(sizeof(int)*this.algorithmsAmount);
  bool algoSentToAWall = (bool*)malloc(sizeof(int)*this.algorithmsAmount);
  bool wonAlready = (bool*)malloc(sizeof(int)*this.algorithmsAmount);
  AbstractSensor* sensors = (AbstractSensor*)malloc(sizeof(AbstractSensor)*this.algorithmsAmount); 
  int houseIndex = 0;
	for(vector<House*>::iterator houseIt = houses.begin(); houseIt!=houses.end() ; ++houseIt){
	  winnerFound = false;
	  int dustAmount = houseIt->initDustAmount();//todo get amount.
	  int xDoc = houseIt->getDockStationX();
	  int yDoc = houseIt->getDockStationY();
	  int stepsMadeAfterWinnerFound;
	  house = houseIt;
	  
	  for(int i = 0 ; i < this.algorithmsAmount ; i++){
	    this.stepsMade[i] = 0;
	    this.batteryUsed[i] = 0;
	    this.dustAmountInHome[i] = dustAmount;
	    this.currentXPos[i] = xDoc;
	    this.currentYPos[i] = yDoc;
	    stepsMadeAfterWinner[i] = 0;
	    wonAlready[i]=false;
	    algoSentToAWall[i] = false;
	    
	    matrixes[i] = (char**) malloc(sizeof(char*) * houseIt->getRowCount());
	    for (int j = 0; j < houseIt->getRowCount(); ++j) {
	      matrixes[i][j] = (char*) malloc(sizeof(char) * houseIt->getColCount());
	      memcpy(matrixes[j][i], houseIt->getMatrix()[i], houseIt->getColCount());
	    }
	    
	    sensors[i] = new SimpleSensor(matrixes[i], &(currentXPos[i]), &(currentYPos[i]));

	    
	    if (dustAmount == 0) {
		wonAlready[i] = true;
		winnerFound = true;
	    }
	  }
	  int i=0;
	
	  for(vector<AbstractAlgorithm*>::iterator algoIt = algorithms.begin(); algoIt!=algorithms.end() ; ++algoIt){
	    algoIt->setSensor(*(sensors[i]));
	    algoIt->setConfiguration(this->config);
	    i++;	  
	  }
	  
	  for(int stepNumber = 0 ; stepNumber < maxSteps && stepsMadeAfterWinnerFound < MaxStepsAfterWinner ; stepNumber++){
	    
		int i = 0;
	    	for(vector<AbstractAlgorithm*>::iterator algoIt = algorithms.begin(); algoIt!=algorithms.end() ; ++algoIt){
		  
		  if(algoSentToAWall[i] || wonAlready[i] || this->batteryUsed[i] > this->batteryCapacity || this->stepsMade[i] > this->maxSteps || (winnerFound & stepsMadeAfterWinner[i] > this.maxStepsAfterWinner)){
		  
		  }else{
		    
		    	if (matrixes[i][currentYPos[i]][currentXPos[i]] != 'D' && matrixes[i][currentYPos[i]][currentXPos[i]] != ' ') { // if we are on a dusty block we clean it
			  matrixes[i][currentYPos[i]][currentXPos[i]]--;
			  dustAmountInHome[i]--;
			}
			
					//if in dockStation recharge battery
		if (matrixes[i][currentYPos[i]][currentXPos[i]] == 'D') {
			batteryUsed[i] =
					batteryUsed[i] - batteryRechargeRate > 0 ?
							batteryUsed[i] - batteryRechargeRate : 0;
		} else { //else consume
			batteryUsed[i] += batteryConsumptionRate;
		}
		if (winnerFound) { //if there is already a winner we count the steps made after winner also.
			stepsMadeAfterWinner[i]++;
		}
		if (wonAlready[i]) {
			winnerFound = true;
		}
		
		Direction algoRecomendation = algorithm->step();
//		cout<<endl;
		int stepResult = 1;

		switch (algoRecomendation) { //simulator check for algorithm return value
		case Direction::East:
			stepResult = this->moveRight(i);
			break;
		case Direction::West:
			stepResult = this->moveLeft(i);
			break;
		case Direction::North:
			stepResult = this->moveUp(i);
			break;
		case Direction::South:
			stepResult = this->moveDown(i);
			break;
		case Direction::Stay:
			break;
		}

		this->stepsMade++;
		if (stepResult == 0) {
//			cout<<"wall" <<endl;
			algoSentToAWall[i] = true;
		}
		if (dustAmountInHome[i] == 0 && matrixes[i][currentYPos[i]][currentXPos[i]] == 'D') {
		  
		  if(!winnerFound) winnerSteps = stepNumber;
					wonAlready[i] = true;
				}
		  int lowest = maxSteps;
		  lowest = lowest < maxSteps - stepNumber ? lowest : maxSteps - stepNumber ; 
		  lowest = lowest < MaxStepsAfterWinner - stepsMadeAfterWinner[i] ? lowest : MaxStepsAfterWinner - stepsMadeAfterWinner[i] ; 
		  lowest = lowest < this->batteryCapacity - batteryUsed[i] ? lowest : this->batteryCapacity - batteryUsed[i] ; 
		  *algoIt.aboutToFinish(lowest);
			this.stepsMade[i]++;
		  
		  }
		  i++;
		}
		simulationSteps++;
		
	    
	  }
	  
	  //todo run on each algo and calculate its score . then put the score in the scores table .
	  int i=0;
	  for(vector<AbstractAlgorithm*>::iterator algoIt = algorithms.begin(); algoIt!=algorithms.end() ; ++algoIt){
	    scores[houseIndex][i] = calcScore(wonAlready[i]);
	    i++;
	  }
	  houseIndex++;
	  
	}
  
  
	
	//delete sensor;
	this->winnerSteps = this->stepsMade; // only for ex1 the single algorithm is also the winner
	//freeing matrix
	for (int i = 0; i < house->getRowCount(); ++i)
		free(matrix[i]);
	free(matrix);
}

int Simulator::calcScore(bool wonalready) {
	int score = 2000;
	score -= ((1 - 1) * 50); /*we assume the algorithm position is 1 because there is one algorithm in ex1 */
	score += (this->winnerSteps - this->stepsMade) * (10);
	score -= this->dustAmountInHome * 3;
	score +=
			(currentXPos == house->getDockStationX())
					&& (currentYPos == house->getDockStationY()) ? 50 : -200;
	return max(0, score);
}

//try to move up .. on success go up and return 1 otherwise do nothing and return 0
int Simulator::moveUp(int i ) {
	if (currentYPos[i] > 0 && matrixes[i][currentYPos[i] - 1][currentXPos[i]] != 'W') {
		(currentYPos[i])--;
		return 1;
	}
	return 0;
}
//try to move down .. on success go down and return 1 otherwise do nothing and return 0
int Simulator::moveDown(int i) {
	if (currentYPos[i] < house->getRowCount()
			&& house->getMatrix()[currentYPos[i] + 1][currentXPos[i]] != 'W') {
		(currentYPos[i])++;
		return 1;
	}
	return 0;
}
//try to move left .. on success go left and return 1 otherwise do nothing and return 0
int Simulator::moveLeft(int i) {
	if (currentXPos[i] > 0
			&& house->getMatrix()[currentYPos[i]][currentXPos[i] - 1] != 'W') {
		(currentXPos[i])--;
		return 1;
	}
	return 0;
}
//try to move right .. on success go right and return 1 otherwise do nothing and return 0
int Simulator::moveRight(int i) {
	if (currentXPos[i] < house->getColCount()
			&& house->getMatrix()[currentYPos[i]][currentXPos[i] + 1] != 'W') {
		(currentXPos[i])++;
		return 1;
	}
	return 0;
}

