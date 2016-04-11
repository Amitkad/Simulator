#ifndef INPUTPARSER_H_
#define INPUTPARSER_H_
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include <utility>
#include <vector>

using namespace std;

//code from class with a bit of edits.
class inputParser {
	char* algorithmPath,housePath,configPath;

	vector<map<AbstractAlgorithm*,char*>> algorithms;//list of algorithms: <algorithm,Error>
	vector<map<House,char*>> houses;//list of houses: <house,Error> (if no errors then error="")
	map<string,int> config; //cnfig.ini <parameter,value>


public:
	//c'tor
	inputParser(int argc, char* argv[]);

};

#endif /* INPUTPARSER_H_ */
