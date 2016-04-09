/*
 * configParser.h
 *
 *  Created on: 21 Mar 2016
 *      Author: Amit
 */

#ifndef CONFIGPARSER_H_
#define CONFIGPARSER_H_
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include <utility>
#include <vector>

using namespace std;

//code from class with a bit of edits.
class configParser {

	map<string, int> parameters; //holds the parameters and its values

	/*splits, trims and put result in "parameters"
	 * @param
	 * line - a text input
	 */
	void processLine(const string& line);


	/*load the file iniPath or default config.ini if iniPath doesn't exists
	 * @param
	 * iniPath - path for config file
	 */
	void loadFromFile(const string& iniPath);

public:
	//c'tor
	configParser(const string& iniPath);

	//"parametrs" getter function
	const map<string, int>& getParameters();

};

#endif /* CONFIGPARSER_H_ */
