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
	
	matrix = (char**) malloc(sizeof(char*) * _house->getRowCount());
	for (int i = 0; i < _house->getRowCount(); ++i) {
		matrix[i] = (char*) malloc(sizeof(char) * _house->getColCount());
		memcpy(matrix[i], _house->getMatrix()[i], _house->getColCount());
	}

}

Simulator::~Simulator(){
	delete algorithm;
	delete house;
}

void Simulator::run() {
	bool algoSentToAWall = false;
	bool winnerFound = false;
	bool wonAlready = false;
	int stepsMadeAfterWinner = 0;
	AbstractSensor* sensor = new SimpleSensor(matrix, &currentXPos, &currentYPos);
	algorithm->setSensor(*sensor);
	algorithm->setConfiguration(this->config);
	if (dustAmountInHome == 0) {
		wonAlready = true;
		winnerFound = true;
	}
	while (this->stepsMade <= this->maxSteps
			&& // there are steps left to do
			this->batteryUsed <= this->batteryCapacity
			&& // battery is not depleted
			(!winnerFound || stepsMadeAfterWinner <= this->maxStepsAfterWinner)
			&& // there was no winner yet or the amount of steps made after winner were small
			!wonAlready && !algoSentToAWall) { // algo failed us and sent the robot to collide with a wall
//		cout<<simulationSteps<<": ";
		simulationSteps++; //increment simulationSteps;
		if (matrix[currentYPos][currentXPos] != 'D'
				&& matrix[currentYPos][currentXPos] != ' ') { // if we are on a dusty block we clean it
			matrix[currentYPos][currentXPos]--;
			dustAmountInHome--;
		}
		//if in dockStation recharge battery
		if (matrix[currentYPos][currentXPos] == 'D') {
			batteryUsed =
					batteryUsed - batteryRechargeRate > 0 ?
							batteryUsed - batteryRechargeRate : 0;
		} else { //else consume
			batteryUsed += batteryConsumptionRate;
		}
		if (winnerFound) { //if there is already a winner we count the steps made after winner also.
			stepsMadeAfterWinner++;
		}
		if (wonAlready) {
			winnerFound = true;
		}

		Direction algoRecomendation = algorithm->step();
//		cout<<endl;
		int stepResult = 1;

		switch (algoRecomendation) { //simulator check for algorithm return value
		case Direction::East:
			stepResult = this->moveRight();
			break;
		case Direction::West:
			stepResult = this->moveLeft();
			break;
		case Direction::North:
			stepResult = this->moveUp();
			break;
		case Direction::South:
			stepResult = this->moveDown();
			break;
		case Direction::Stay:
			break;
		}

		this->stepsMade++;
		if (stepResult == 0) {
//			cout<<"wall" <<endl;
			algoSentToAWall = true;
		}
		if (dustAmountInHome == 0 && matrix[currentYPos][currentXPos] == 'D') {
					wonAlready = true;
				}

	}
	
	//delete sensor;
	this->winnerSteps = this->stepsMade; // only for ex1 the single algorithm is also the winner
	int score = calcScore(wonAlready);
	cout << score <<endl;
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
int Simulator::moveUp() {
	if (currentYPos > 0 && matrix[currentYPos - 1][currentXPos] != 'W') {
		(currentYPos)--;
		return 1;
	}
	return 0;
}
//try to move down .. on success go down and return 1 otherwise do nothing and return 0
int Simulator::moveDown() {
	if (currentYPos < house->getRowCount()
			&& house->getMatrix()[currentYPos + 1][currentXPos] != 'W') {
		(currentYPos)++;
		return 1;
	}
	return 0;
}
//try to move left .. on success go left and return 1 otherwise do nothing and return 0
int Simulator::moveLeft() {
	if (currentXPos > 0
			&& house->getMatrix()[currentYPos][currentXPos - 1] != 'W') {
		(currentXPos)--;
		return 1;
	}
	return 0;
}
//try to move right .. on success go right and return 1 otherwise do nothing and return 0
int Simulator::moveRight() {
	if (currentXPos < house->getColCount()
			&& house->getMatrix()[currentYPos][currentXPos + 1] != 'W') {
		(currentXPos)++;
		return 1;
	}
	return 0;
}

