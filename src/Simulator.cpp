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
	
	

	algorithms = _algorithms;
	
	//todo fix matrixes for each algo .
	matrixes = (char***) malloc(sizeof(char**)*algorithmsAmount);
	for(int j=0 ; j < algorithmsAmount ; j++){
	  matrixes[j] = (char**) malloc(sizeof(char*) * (*house)->getRowCount());
	  for (int i = 0; i < (*house)->getRowCount(); ++i) {
	    matrixes[j][i] = (char*) malloc(sizeof(char) * (*house)->getColCount());
	    memcpy(matrixes[j][i], (*house)->getMatrix()[i], (*house)->getColCount());
	  }
	}


}

Simulator::~Simulator(){
	//delete algorithm;
	//delete house;
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
	  int dustAmount = (*houseIt)->initDustAmount();//todo get amount.
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
	      memcpy(matrixes[i][j], (*houseIt)->getMatrix()[i], (*houseIt)->getColCount());
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
			stepResult = this->moveRight(c);
			break;
		case Direction::West:
			stepResult = this->moveLeft(c);
			break;
		case Direction::North:
			stepResult = this->moveUp(c);
			break;
		case Direction::South:
			stepResult = this->moveDown(c);
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
	  
	  //todo run on each algo and calculate its score . then put the score in the scores table .
	  int y=0;
	  if(!winnerSteps){
	    winnerSteps = simulationSteps;
	    
	  }
//	  cout<<this->stepsMade[0]<<endl;
//	  cout<<this->stepsMade[1]<<endl;

	  for(map<string,pair<AbstractAlgorithm*,string>>::iterator algoIt = algorithms.begin(); algoIt!=algorithms.end() ; ++algoIt){
	    int position = 1;
	    for(int j = 0 ; j<this->algorithmsAmount ; j++){
	      if(this->stepsMade[y] > this->stepsMade[j]) { 
		position++;
		
	      }
	    }
	    if(position > 4) position = 4;
	    scores[houseIndex][y] = calcScore(wonAlready[y],position,y,(*houseIt));
//	      cout<<"--"<<scores[houseIndex][y]<<"--"<<endl;

	    y++;
	  }
	  houseIndex++; 
	}
//	cout<<this->stepsMade[1]<<endl;
	
	//todo print scores
	
	this->printScores(houseIndex);
	
	for (int i = 0; i < house->getRowCount(); ++i);
		//free(matrix[i]);
	//free(matrix);
}

void Simulator::printScores(int houseIndex){
  	int tableWidth = 15+11*(houseIndex+1);
	int houseSize;
	char houseName[11];
	char* line = (char*) malloc(sizeof(char)*tableWidth);
	for(int i = 0 ; i < tableWidth ; i++){
	  line[i] = '-';
	  line[i+1] = '\0';
	}
	//header
	cout<< line <<endl;
	
	cout << "|            |";
	for(vector<House*>::iterator houseIt = houses.begin(); houseIt!=houses.end() ; ++houseIt){
		houseSize=min(10,(int)((*houseIt)->getName().size()));
		strncpy(houseName,((*houseIt)->getName()).c_str(),houseSize);
		houseName[houseSize]='\0';
		cout<<houseName;
		for(int k=0;k<10-houseSize;k++){
		  cout<<" ";
		}
		cout<<"|";
	}
	cout<<"AVG        |"<<endl;
	
	//header end.
	cout<< line <<endl;

	//printAlgosStart
	int i=0;
	int totalAlgoScore;
	for(map<string,pair<AbstractAlgorithm*,string>>::iterator algoIt = algorithms.begin(); algoIt!=algorithms.end() ; ++algoIt){
	    totalAlgoScore=0;
	    if(algoIt->second.second.empty()){
	      cout<<"|";
	      cout<<algoIt->first;
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
	      	cout<< line <<endl;
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

