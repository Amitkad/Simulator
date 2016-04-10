#ifndef SIMULATION_H
#define SIMULATION_H
#include "House.h"
#include "AbstractAlgorithm.h"
#include "SimpleSensor.h"
#include <vector>




class Simulator {
	int housesAmount;
	int algorithmsAmount;
	int* currentYPos;
	int* currentXPos;
	std::vector<char**> matrix;
	int* batteryUsed;
	int batteryCapacity;
	int batteryConsumptionRate;
	int batteryRechargeRate;
	int* dustAmountInHome;
	int winnerSteps;
	int* stepsMade;
	int maxSteps;
	int maxStepsAfterWinner;
	int simulationSteps;
	std::vector<House*> houses;
	std::vector<AbstractAlgorithm*> algorithms;
	const map<string, int> config;
	int** scores;
	int currentHouse;
public:
  /*
   constructor 
   @param
   house - an House instance to run the simulator on
   config - configuration of the simulator
   algorithm - an algorithm to use while running
   */
	Simulator(House* house, const map<string, int> config,
			AbstractAlgorithm* algorithm);
	
	/*
	 destructor deleting algorithm and house
	 */
	~Simulator();
	
	/*
	 runs the simulator using the algorithm and the config 
	 then prints the scores
	 */
	void run();
	/*
	 * calculates scores
	    @param
	    wonalready - indices if the simulation was already won .
	    @return 
	    returns the score from the run
	 */
	int calcScore(bool wonalready);
	/*
	 try to move up
	 @return
	 tries to move up . return 1 on success 0 on error
	 */
	int moveUp();
	
	
	/*
	 try to move down
	 @return
	 tries to move down . return 1 on success 0 on error
	 */	
	int moveDown();

	/*
	 try to move left
	 @return
	 tries to move left . return 1 on success 0 on error
	 */
	int moveLeft();

	/*
	 try to move right
	 @return
	 tries to move right . return 1 on success 0 on error
	 */
	int moveRight();

};

#endif
