#include "importFiles.h"

using namespace std;

//TODO - destructors
// -------------------------------------------------- Base Class: import files --------------------------------------------------------//

//c'tor
importFiles::importFiles(int argc, char* argv[]) :
		algorithmPath("/"), housePath("/"), configPath("/config.ini") {

	if (!checkArgValidity(argc, argv))
		return;
	this->fillInputFromFiles(argc, argv);
}

//check flags validity and number of parameters, updates to new paths and put error accordingly
bool importFiles::checkArgValidity(int argc, char* argv[]) {
	return true; //TODO
}

void importFiles::fillInputFromFiles(int argc, char* argv[]) {
	config = new importFiles::importConfig(configPath,*this);
	if (err)
		return;
	algorithms = new importFiles::importAlgs(algorithmPath,*this);
	if (err)
		return;
	houses = new importFiles::importHouses(housePath,*this);
	if (err)
		return;

}

//class member getters
const vector<map<AbstractAlgorithm*, string>>& importFiles::getAlgorithms() {
	return algorithms->getAlgorithms();

}
const vector<map<House, string>>& importFiles::getHouses() {
	return houses->getHouses();

}
const map<string, int>& importFiles::getParameters() {
	return config->getParameters();

}

//-------------------------------------------------------- Nested: importConfig --------------------------------------------------------//

static vector<string> split(const string &s, char delim) {
	vector<string> elems;
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

static string trim(string& str) {
	str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
	str.erase(str.find_last_not_of(' ') + 1);         //surfixing spaces
	return str;
}

void importFiles::importConfig::processLine(const string& line) {

	vector<string> tokens = split(line, '=');
	if (tokens.size() != 2) {
		return;
	}

	this->parameters[trim(tokens[0])] = atoi(tokens[1].c_str()); //put parsed strings in "parametrs"
}

void importFiles::importConfig::loadFromFile(const string& iniPath) {
	this->parameters.clear();
	ifstream fin(iniPath.c_str());
	if (!fin.good()) { // check iniPath existence
		parent.err = false;
		cout<< "Usage: simulator [­config <config path>] [­house_path <house path>][­algorithm_path <algorithm path>]\n"<< endl;
		return;
	}
	string line;
	while (getline(fin, line)) //fill "parameters" with the the parameters and values
	{
		this->processLine(line);
	}
}

importFiles::importConfig::importConfig(const string& iniPath,importFiles& _parent):parent(_parent) {
	this->loadFromFile(iniPath);
	if (!parent.err)
		return;
	checkParameters();
}
void importFiles::importConfig::checkParameters(){
	//TODO write the function that checks all parameters exists(if not update error and return)
}

const map<string, int>& importFiles::importConfig::getParameters() {
	return parameters;
}

//-------------------------------------------------------- Nested: import algorithms --------------------------------------------------------//

//TODO
importFiles::importAlgs::importAlgs(const string& iniPath,importFiles& _parent):parent(_parent) {}

const vector<map<AbstractAlgorithm*, string>>& importFiles::importAlgs::getAlgorithms() {}

//-------------------------------------------------------- Nested: import houses --------------------------------------------------------//

//TODO

importFiles::importHouses::importHouses(const string& iniPath,importFiles& _parent):parent(_parent) {}

const vector<map<House, string>>& importFiles::importHouses::getHouses() {}



































//int errCode = 0;
//const char* algorithmPath, housePath, configPath;
//vector<map<AbstractAlgorithm*, char*>> algorithms; //list of algorithms: <algorithm,Error>
//vector<map<House, char*>> houses;//list of houses: <house,Error> (if no errors then error="")
//map<string, int> config;//cnfig.ini <parameter,value>

//
//
//	//config parser nested privet class
//		class configParser {
//			map<string, int> parameters; //holds the parameters and its values
//			/*splits, trims and put result in "parameters"
//			 * @param
//			 * line - a text input
//			 */
//			void processLine(const string& line);
//			/*load the file iniPath or default config.ini if iniPath doesn't exists
//			 * @param
//			 * iniPath - path for config file
//			 */
//			void loadFromFile(const string& iniPath);
//		public:
//			//c'tor
//			configParser(const string& iniPath);
//			//"parametrs" getter function
//			const map<string, int>& getParameters();
//		};

