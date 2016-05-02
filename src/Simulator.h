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
	char*** matrixes;
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
	House* house; 
	std::vector<House*> houses;
	std::map<string,pair<AbstractAlgorithm*,string>> algorithms;
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
	Simulator(vector<House*> _houses, const map<string, int> _config,map<string,pair<AbstractAlgorithm*,string>> _algorithms);
	
	/*
	 destructor deleting algorithm and house
	 */
	~Simulator();
	
	Simulator& operator=(const Simulator&)=delete;
	Simulator(const Simulator&) = delete;
	/*
	 runs the simulator using the algorithm and the config 
	 then prints the scores
	 */
	void run();
	
	/*
	 * prints the scores with the beautifull table 
	 */
	void printScores(int houseIndex);
	/*
	 * calculates scores
	    @param
	    wonalready - indices if the simulation was already won .
	    @return 
	    returns the score from the run
	 */
	int calcScore(bool wonalready,int position,int i,House* currentHouse);
	/*
	 try to move up
	 @return
	 tries to move up . return 1 on success 0 on error
	 */
	int moveUp(int i,House* house);
	
	
	/*
	 try to move down
	 @return
	 tries to move down . return 1 on success 0 on error
	 */	
	int moveDown(int i,House* house);

	/*
	 try to move left
	 @return
	 tries to move left . return 1 on success 0 on error
	 */
	int moveLeft(int i,House* house);

	/*
	 try to move right
	 @return
	 tries to move right . return 1 on success 0 on error
	 */
	int moveRight(int i,House* house);

};

#endif
