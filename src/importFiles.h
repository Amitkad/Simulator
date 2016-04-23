#ifndef INPUTPARSER_H_
#define INPUTPARSER_H_
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <string.h>
#include <utility>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "AbstractAlgorithm.h"
#include "house.h"

using namespace std;

//TODO put comments
class importFiles {


	// ---------- config parser nested private class -------//
	class importConfig {
		importFiles& parent;
		map<string, int> parameters; //cnfig.ini <parameter,value>
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
		//TODO - תיעוד
		void checkParameters();
	public:
		//c'tor
		importConfig(const string& iniPath,importFiles& _parent);
		//"parametrs" getter function
		const map<string, int>& getParameters();
	};

	// ---------- algorithms parser nested private class -------//
	class importAlgs {
		importFiles& parent;
		vector<map<AbstractAlgorithm*, string>> algorithms; //list of algorithms: <algorithm,Error>

		/*load the file iniPath or default config.ini if iniPath doesn't exists
		 * @param
		 * iniPath - path for config file
		 */
		void loadFromFile(const string& iniPath);
	public:
		//c'tor
		importAlgs(const string& iniPath,importFiles& _parent);
		//"algorithms" getter function
		const vector<map<AbstractAlgorithm*, string>>& getAlgorithms();
	};

	// ---------- houses parser nested private class -------//
	class importHouses {
		importFiles& parent;
		vector<map<House, string>> houses; //list of houses: <house,Error> (if no errors then error="")

		/*load the file iniPath or default config.ini if iniPath doesn't exists
		 * @param
		 * iniPath - path for config file
		 */
		void loadFromFile(const string& iniPath);
	public:
		//c'tor
		importHouses(const string& iniPath,importFiles& _parent);
		//"houses" getter function
		const vector<map<House, string>>& getHouses();
	};

	bool err = false;//true iff there was an error
	const string algorithmPath, housePath, configPath;
	importAlgs* algorithms; //list of algorithms: <algorithm,Error>
	importHouses* houses; //list of houses: <house,Error> (if no errors then error="")
	importConfig* config; //cnfig.ini <parameter,value>

	//check flags validity and number of parameters
	bool checkArgValidity(int argc, char* argv[]);

	//fills config, algorithms and houses data from files
	void fillInputFromFiles(int argc, char* argv[]);

public:
	//c'tor
	importFiles(int argc, char* argv[]);

	//class member getters
	const vector<map<AbstractAlgorithm*, string>>& getAlgorithms();
	const vector<map<House, string>>& getHouses();
	const map<string, int>& getParameters();
	bool getErr();
	void setErr(bool err);
};

#endif /* INPUTPARSER_H_ */

//vector<map<AbstractAlgorithm*, char*>> algorithms; //list of algorithms: <algorithm,Error>
//	vector<map<House, char*>> houses; //list of houses: <house,Error> (if no errors then error="")
//	map<string, int> parameters; //cnfig.ini <parameter,value>
