//============================================================================
// Name        : Simulator.cpp
// Author      : Amit K & Elinor S
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================
//#include "randAlgorithm.h"
#include "SimpleSensor.h"
#include "House.h"
#include "importFiles.h"
#include "Simulator.h"

using namespace std;

int main(int argc, char *argv[]) {




	importFiles* files = new importFiles(argc,argv);
	cout<<files->getGoodHouses().back()->getColCount()<<endl;
	cout << "success1" << endl;
	Simulator* simulator= new Simulator(files->getGoodHouses(),files->getParameters(),files->getAlgorithms());
	simulator->run();







}


	/*
	configParser* config;//parser for the command line input

	// hard-coding a house...
	string houseName = "First House";
	string houseDesc = "A hard coded house";
	int const rows = 19;
	int const cols = 81;
	char house[rows][cols] =
			{
					//             1         2         3         4         5         6         7
				//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
					"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",// 0
					"W     33D33 W                                                                  W", // 1
					"W     33333WWWWWWW                123213           W                           W", // 2
					"W     3333     W                                   W                           W", // 3
					"W              W                                   W                           W", // 4
					"W      2       W           WWWWWWWWWWWWWWWWWWWWWWWWW                13123      W", // 5
					"W              W                                                               W", // 6
					"W   213213     W                                                               W", // 7
					"W                          WWWWWWWWWWWWWWWWWWWWWW  WWWWWWW                     W", // 8
					"W                                                  W     W                     W", // 9
					"W              W                                   W     W                     W", // 10
					"W      23      W                         2         W     W                     W", // 11
					"W              W       123213                      W     W                     W", // 12
					"W              W                                   WW   WW                     W", // 13
					"W              W       1111                         W   W                      W", // 14
					"W              W                  2                                            W", // 15
					"W              W                                                               W", // 16
					"W              W                         2                                     W", // 17
					"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW" // 18
			};//end of hard-coding

	    //"copying" the house into the code dynamically
	    char** matrix = (char**)malloc(sizeof(char*)*rows);
	    for(int i=0 ; i<rows ; i++){
	    	matrix[i] = (char*)malloc(sizeof(char)*cols);
	    }
	    for(int k=0 ; k < rows ; k++){
	    	for(int l=0 ; l < cols ; l++){
	    		matrix[k][l] = house[k][l];
	    	}
	    }

	//check flags validity and fill the paths accordingly
	switch (isValid_ComandLine(argc, argv)) {
	case -1: //invalid arguments
		for (int i = 0; i < rows; ++i)
				free(matrix[i]);
			free(matrix);
		return -1;
		break;
	case 0: //no arguments
		config = new configParser("config.ini");
		break;
	case 1: //only config
		config = new configParser(string(argv[2]));
		break;
	default: //no need to get here
		config = new configParser("config.ini");
		break;
	}
	House* houseP = new House(matrix, static_cast<int>(cols),static_cast<int>(rows), houseName,houseDesc);//house saves the house matrix and othe configurations
	AbstractAlgorithm* algorithm = new randAlgorithm();//creating algorithm run object
	Simulator* simulator = new Simulator(houseP, config->getParameters(),algorithm);//creating simulator
	simulator->run();//running algorithms on the houses

	//release and return
	delete simulator;
	delete config;
	return 0;
}
*/
