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
	importFiles* files = new importFiles(argc,argv); 	//import config,algorithms,houses
	if (files->getErr()){delete files; return -1;}		//if there was an error in config/all houses/ all algorithms --> exit
	Simulator* simulator= new Simulator(files->getGoodHouses(),files->getParameters(),files->getAlgorithms());		//init simulator
	simulator->run();		//run sumulator
	files->printErrors();	//print errors if exsits

	delete simulator;
	delete files;
	return 0;
}
