#include "randAlgorithm.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

randAlgorithm::randAlgorithm(){
	 srand(time(NULL));//for randomization matters
}

randAlgorithm::~randAlgorithm() {
	delete sensor;
}

void randAlgorithm::setSensor(const AbstractSensor& sensor) {
	this->sensor = &sensor;
}

void randAlgorithm::setConfiguration(map<string, int> config) {
	this->config = config;
}

Direction randAlgorithm::step() {
	int dir;
	do {
		dir = (rand() % 5);
//		printf("%s<--move to<-- ",d[dir]);
		if (dir ==4) //if stay--> return stay
			return Direction(dir);
	} while (sensor->sense().isWall[dir]); //generate random if thre's a wall

	return Direction(dir);
}

void randAlgorithm::aboutToFinish(int stepsTillFinishing) {
} //TODO

