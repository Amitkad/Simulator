#include <iostream>
#include <algorithm>
#include <cstring>

#include "Simulator.h"
#include <vector>

using namespace std;
//CleanAlgorithm algorithm;
Simulator::Simulator(vector<House*> _houses, const map<string, int> _config,map<string,pair<AbstractAlgorithm*,string>> _algorithms) :
		config(_config) {
	algorithmsAmount = _algorithms.size();
	housesAmount = _houses.size();
	maxSteps = _config.find("MaxSteps")->second;
	maxStepsAfterWinner = _config.find("MaxStepsAfterWinner")->second;
	batteryCapacity = _config.find("BatteryCapacity")->second;
	batteryConsumptionRate = _config.find("BatteryConsumptionRate")->second;
	batteryRechargeRate = _config.find("BatteryRechargeRate")->second;
	simulationSteps = 0;
	winnerSteps = 0;
	currentHouse = 0;
	
	scores = (int**) malloc(sizeof(int*) * algorithmsAmount);
	for(int i = 0 ; i < algorithmsAmount ; i++){
	  scores[i] = (int*) malloc(sizeof(int) * housesAmount);
	  for(int j = 0 ; j < housesAmount ; j++){
	    scores[i][j] = 0;
	  }
	}
	
	houses = _houses;
	stepsMade = (int*) malloc(sizeof(int) * algorithmsAmount);
	batteryUsed = (int*) malloc(sizeof(int) * algorithmsAmount);
	dustAmountInHome = (int*) malloc(sizeof(int) * algorithmsAmount);
	currentYPos = (int*) malloc(sizeof(int) * algorithmsAmount);
	currentXPos = (int*) malloc(sizeof(int) * algorithmsAmount);
		algorithms = _algorithms;

		matrixes = (char***) malloc(sizeof(char**)*algorithmsAmount);
/*
	vector<House*>::iterator house = houses.begin();
	int amountOfDustInFirstHouse = (*house)->initDustAmount(); // should be first house 
	int dockX = (*house)->getDockStationX();
	int dockY = (*house)->getDockStationY();
	
	
	for(int i = 0 ; i < algorithmsAmount ; i++){
	  stepsMade[i] = 0;
	  batteryUsed[i] = 0;
	  dustAmountInHome[i] = amountOfDustInFirstHouse;
	  currentYPos[i] = dockY;
	  currentXPos[i] = dockX;
	}
	
	

	
	// fix matrixes for each algo .
	
	for(int j=0 ; j < algorithmsAmount ; j++){
	  matrixes[j] = (char**) malloc(sizeof(char*) * (*house)->getRowCount());
	  for (int i = 0; i < (*house)->getRowCount(); ++i) {
	    matrixes[j][i] = (char*) malloc(sizeof(char) * (*house)->getColCount());
	    memcpy(matrixes[j][i], (*house)->getMatrix()[i], (*house)->getColCount());
	  }
	}
*/	
      

}

Simulator::~Simulator(){
	
  
	free(stepsMade);
	free(batteryUsed);
	free(dustAmountInHome); 
	free(currentYPos); 
	free(currentXPos);
	free(matrixes);
	
	for(int i = 0 ; i < algorithmsAmount ; i++){
	  free(scores[i]);
	}
	free(scores);
	
	for(map<string,pair<AbstractAlgorithm*,string>>::iterator algoIt = algorithms.begin(); algoIt!=algorithms.end() ; ++algoIt){
	  //delete (algoIt->second).first;
	}

	for(vector<House*>::iterator houseIt = houses.begin(); houseIt!=houses.end() ; ++houseIt){
	  //delete (*houseIt);
	}
}

void Simulator::run() {
  int* stepsMadeAfterWinner = (int*)malloc(sizeof(int)*this->algorithmsAmount);
  bool* algoSentToAWall = (bool*)malloc(sizeof(int)*this->algorithmsAmount);
  bool* wonAlready = (bool*)malloc(sizeof(int)*this->algorithmsAmount);
  bool winnerFound;
  AbstractSensor** sensors = (AbstractSensor**)malloc(sizeof(AbstractSensor*)*this->algorithmsAmount); 
//  cout<<"run"<<endl;
  int houseIndex = 0;
  int stepsMadeAfterWinnerFound=0;
	for(vector<House*>::iterator houseIt = houses.begin(); houseIt!=houses.end() ; ++houseIt){
	  winnerFound = false;
	  int dustAmount = (*houseIt)->initDustAmount();// get amount.
	  int xDoc = (*houseIt)->getDockStationX();
	  int yDoc = (*houseIt)->getDockStationY();
	  house = (*houseIt);

	  for(int i = 0 ; i < this->algorithmsAmount ; i++){
	    this->stepsMade[i] = 0;
	    this->batteryUsed[i] = 0;
	    this->dustAmountInHome[i] = dustAmount;
	    this->currentXPos[i] = xDoc;
	    this->currentYPos[i] = yDoc;
	    stepsMadeAfterWinner[i] = 0;
	    wonAlready[i]=false;
	    algoSentToAWall[i] = false;
	    matrixes[i] = (char**) malloc(sizeof(char*) * (*houseIt)->getRowCount());
	    for (int j = 0; j < (*houseIt)->getRowCount(); ++j) {
	      matrixes[i][j] = (char*) malloc(sizeof(char) * (*houseIt)->getColCount());
	      memcpy(matrixes[i][j], (*houseIt)->getMatrix()[j], (*houseIt)->getColCount());
	    }
	    
	    sensors[i] = new SimpleSensor(matrixes[i], &(currentXPos[i]), &(currentYPos[i]));

	    if (dustAmount == 0) {
		wonAlready[i] = true;
		winnerFound = true;
	    }
	  }
	  int q=0;
	
	  for(map<string,pair<AbstractAlgorithm*,string>>::iterator algoIt = algorithms.begin(); algoIt!=algorithms.end() ; ++algoIt){
	    ((algoIt->second).first)->setSensor(*(sensors[q]));
	    ((algoIt->second).first)->setConfiguration(this->config);
	    q++;	  
	  }
	  
	  for(int stepNumber = 0 ; stepNumber < maxSteps && stepsMadeAfterWinnerFound < this->maxStepsAfterWinner ; stepNumber++){
		int c = 0;
	    	for(map<string,pair<AbstractAlgorithm*,string>>::iterator algoIt = algorithms.begin(); algoIt!=algorithms.end() ; ++algoIt){
//		  cout<<this->stepsMade[c]<<endl;
		  if(algoSentToAWall[c] ||
		    wonAlready[c] || 
		    this->batteryUsed[c] > this->batteryCapacity || 
		    this->stepsMade[c] > this->maxSteps ||
		    ( (winnerFound & ((stepsMadeAfterWinner[c]) > (this->maxStepsAfterWinner)))) || 
		    !(((algoIt->second).second).empty())){
		    
		    if(this->batteryUsed[c] > this->batteryCapacity){
		      this->stepsMade[c]++;
		    }
		    if(algoSentToAWall[c] && ((algoIt->second).second).empty()){
		      algoIt->second.second = "sent into a wall";
		    }
		  
		  }else{
		    
		    	if (matrixes[c][currentYPos[c]][currentXPos[c]] != 'D' && matrixes[c][currentYPos[c]][currentXPos[c]] != ' ') { // if we are on a dusty block we clean it
			  matrixes[c][currentYPos[c]][currentXPos[c]]--;
			  dustAmountInHome[c]--;
			}
			
					//if in dockStation recharge battery
		if (matrixes[c][currentYPos[c]][currentXPos[c]] == 'D') {
			batteryUsed[c] =
					batteryUsed[c] - batteryRechargeRate > 0 ?
							batteryUsed[c] - batteryRechargeRate : 0;
		} else { //else consume
			batteryUsed[c] += batteryConsumptionRate;
		}
		if (winnerFound) { //if there is already a winner we count the steps made after winner also.
			stepsMadeAfterWinner[c]++;
		}
		if (wonAlready[c]) {
			winnerFound = true;
		}
		
//		cout<<"before direction"<<endl;

		Direction algoRecomendation = ((algoIt->second).first)->step();
//		cout<<endl;
		int stepResult = 1;
//		cout<<"after algoRecomendation direction"<<endl;

		switch (algoRecomendation) { //simulator check for algorithm return value
		case Direction::East:
			stepResult = this->moveRight(c,house);
			break;
		case Direction::West:
			stepResult = this->moveLeft(c,house);
			break;
		case Direction::North:
			stepResult = this->moveUp(c,house);
			break;
		case Direction::South:
			stepResult = this->moveDown(c,house);
			break;
		case Direction::Stay:
			break;
		}
//		cout<<"after direction"<<endl;
		if (stepResult == 0) {
			algoSentToAWall[c] = true;
		}
		if (dustAmountInHome[c] == 0 && matrixes[c][currentYPos[c]][currentXPos[c]] == 'D') {
		  
		  if(!winnerFound) winnerSteps = stepNumber;
					wonAlready[c] = true;
		}
		  int lowest = maxSteps;
		  lowest = lowest < maxSteps - stepNumber ? lowest : maxSteps - stepNumber ; 
		  lowest = lowest < maxStepsAfterWinner - stepsMadeAfterWinner[c] ? lowest : maxStepsAfterWinner - stepsMadeAfterWinner[c] ; 
		  lowest = lowest < this->batteryCapacity - batteryUsed[c] ? lowest : this->batteryCapacity - batteryUsed[c] ; 
		  algoIt->second.first->aboutToFinish(lowest);
			this->stepsMade[c]++;
		  }
		  c++;
		}
		simulationSteps++;
		
	    
	  }
	  
	  // run on each algo and calculate its score . then put the score in the scores table .
	  int y=0;
	  if(!winnerSteps){
	    winnerSteps = simulationSteps;
	    
	  }

	  for(map<string,pair<AbstractAlgorithm*,string>>::iterator algoIt = algorithms.begin(); algoIt!=algorithms.end() ; ++algoIt){
	    int position = 1;
	    for(int j = 0 ; j<this->algorithmsAmount ; j++){
	      if(this->stepsMade[y] > this->stepsMade[j]) { 
		position++;
		
	      }
	    }
	    if(position > 4) position = 4;
	    scores[houseIndex][y] = calcScore(wonAlready[y],position,y,(*houseIt));

	    y++;
	  }
	  
	  //clear sensors
	  	  for(int t = 0 ; t < this->algorithmsAmount ; t++){
		    delete sensors[t];
		  }
		  
	//free matrixes;
	for(int t = 0 ; t < this->algorithmsAmount ; t++){
	  for (int r = 0; r < house->getRowCount(); ++r){
	    	    free(matrixes[t][r]);
	  }
	  free(matrixes[t]);
	}
	//end free matrixes
	  
	  houseIndex++; 
	}

	free(sensors);

	
	// print scores
	

	this->printScores(houseIndex);
	

	
	  free(stepsMadeAfterWinner);
  free(algoSentToAWall);
  free(wonAlready);


}
void Simulator::printLine(int width){
  for(int i=0;i<width;i++){
    cout<< '-';
  }
  cout<<endl;
}

void Simulator::printScores(int houseIndex){
  	int tableWidth = 15+11*(houseIndex+1);
	int houseSize;
	char houseName[11];

	//header
	printLine(tableWidth);
	
	cout << "|             |";
	for(vector<House*>::iterator houseIt = houses.begin(); houseIt!=houses.end() ; ++houseIt){
		houseSize=min(10,(int)((*houseIt)->getName().substr(0, (*houseIt)->getName().find_last_of(".")).size()));
		strncpy(houseName,((*houseIt)->getName()).substr(0, (*houseIt)->getName().find_last_of(".")).c_str(),houseSize);
		houseName[houseSize]='\0';
		cout<<houseName;
		for(int k=0;k<10-houseSize;k++){
		  cout<<" ";
		}
		cout<<"|";
	}
	cout<<"AVG        |"<<endl;
	
	//header end.
	printLine(tableWidth);

	//printAlgosStart
	int i=0;
	int totalAlgoScore;
	for(map<string,pair<AbstractAlgorithm*,string>>::iterator algoIt = algorithms.begin(); algoIt!=algorithms.end() ; ++algoIt){
	    totalAlgoScore=0;
	    if(algoIt->second.second.empty()){
	      cout<<"|";
	      cout<<algoIt->first.substr(0, algoIt->first.find_last_of("."));
	      cout<<" |";
	      for(int j=0;j<this->housesAmount;j++){
		for(int k=0 ; k < 10-(int)(to_string(this->scores[j][i]).size());k++){
		  cout<<" ";
		}
		cout<<this->scores[j][i]<<"|";
		totalAlgoScore+=this->scores[j][i];
		}
		for(int k=0 ; k < 10-(int)((to_string(totalAlgoScore/this->housesAmount).size())+2);k++){
		  cout<<" ";	  
		}
		printf("%.2f", totalAlgoScore/(double)this->housesAmount);
		cout<<"|"<<endl;
	      	printLine(tableWidth);
	    }
	    i++;
	    //else continue without prints
	}
	//printAlgosEnd
	
}

int Simulator::calcScore(bool wonalready,int position,int i,House* currentHouse) {
	int score = 2000;
	score -= ((position - 1) * 50); /*we assume the algorithm position is 1 because there is one algorithm in ex1 */
	score += (this->winnerSteps - this->stepsMade[i]) * (10);
//	cout<<"--"<<score<<"--"<<endl;
	score -= this->dustAmountInHome[i] * 3;
//	cout<<"--"<<score<<"--"<<endl;
	score +=(currentXPos[i] == currentHouse->getDockStationX())&& (currentYPos[i] == currentHouse->getDockStationY()) ? 50 : -200;
//	cout<<"--"<<score<<"--"<<endl;
	return max(0, score);
}

//try to move up .. on success go up and return 1 otherwise do nothing and return 0
int Simulator::moveUp(int i ,House* house) {
	if (currentYPos[i] > 0 && house->getMatrix()[currentYPos[i] - 1][currentXPos[i]] != 'W' && house->getMatrix()[currentYPos[i] - 1][currentXPos[i]] != 'w') {
		(currentYPos[i])--;
		return 1;
	}
	return 0;
}
//try to move down .. on success go down and return 1 otherwise do nothing and return 0
int Simulator::moveDown(int i,House* house) {


    if (currentYPos[i] < house->getRowCount()
			&& house->getMatrix()[currentYPos[i] + 1][currentXPos[i]] != 'W' && house->getMatrix()[currentYPos[i] + 1][currentXPos[i]] != 'w') {
		(currentYPos[i])++;
		return 1;
	}
	return 0;
}
//try to move left .. on success go left and return 1 otherwise do nothing and return 0
int Simulator::moveLeft(int i,House* house) {
	if (currentXPos[i] > 0
			&& house->getMatrix()[currentYPos[i]][currentXPos[i] - 1] != 'W' && house->getMatrix()[currentYPos[i]][currentXPos[i] - 1] != 'w') {
		(currentXPos[i])--;
		return 1;
	}
	return 0;
}
//try to move right .. on success go right and return 1 otherwise do nothing and return 0
int Simulator::moveRight(int i, House* house) {

	if (currentXPos[i] < house->getColCount()
			&& house->getMatrix()[currentYPos[i]][currentXPos[i] + 1] != 'W' &&  house->getMatrix()[currentYPos[i]][currentXPos[i] + 1] != 'w') {
		(currentXPos[i])++;
		return 1;
	}
	return 0;
}

