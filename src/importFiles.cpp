#include "importFiles.h"

using namespace std;

//TODO - destructors
// --------import files --------//

//c'tor
importFiles::importFiles(int argc, char* argv[]) :
		algorithmPath("/"), housePath("/"), configPath("/config.ini") {

	if (!checkArgValidity(argc, argv))
		return;
	this->fillInputFromFiles(argc, argv);
}

//check flags validity and number of parameters, updates paths and error accordingly
bool importFiles::checkArgValidity(int argc, char* argv[]) {
	return true; //TODO
}

void importFiles::fillInputFromFiles(int argc, char* argv[]) {
	config = new importFiles::importConfig(configPath, *this);
	if (err)
		return;
	algorithms = new importFiles::importAlgs(algorithmPath, *this);
	if (err)
		return;
	houses = new importFiles::importHouses(housePath, *this);
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
//err getter
bool importFiles::getErr() {
	return this->err;
}
//err setter
void importFiles::setErr(bool err) {
	this->err = err;
}

//--------------importConfig----------------//

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
		parent.setErr(true);
		cout
				<< "Usage: simulator [­config <config path>] [­house_path <house path>][­algorithm_path <algorithm path>]\n"
				<< endl;
		return;
	}
	string line;

	while (getline(fin, line)) //fill "parameters" with the the parameters and values
	{
		this->processLine(line);
	}
}

importFiles::importConfig::importConfig(const string& iniPath,
		importFiles& _parent) :
		parent(_parent) {
	this->loadFromFile(iniPath);
	if (parent.getErr())
		return;
	checkParameters(); //TODO write the function that checks all parameters exists(if not update error and return)
}
<<<<<<< HEAD
void importFiles::importConfig::checkParameters(){
<<<<<<< HEAD

=======
	//TODO write the function that checks all parameters exists(if not update error and return)
>>>>>>> parent of bde7637... config check finish
=======

void importFiles::importConfig::checkParameters() {
	if ((parameters.find("MaxStepsAfterWinner") == parameters.end())
			|| (parameters.find("BatteryCapacity") == parameters.end())
			|| (parameters.find("BatteryConsumptionRate") == parameters.end())
			|| (parameters.find("BatteryRechargeRate") == parameters.end())) {
		parent.setErr(true);
	}
>>>>>>> refs/remotes/origin/master
}

const map<string, int>& importFiles::importConfig::getParameters() {
	return parameters;
}

//--------------import algorithms -------------//

//TODO
importFiles::importAlgs::importAlgs(const string& iniPath, importFiles& _parent) :
		parent(_parent) {
}

const vector<map<AbstractAlgorithm*, string>>& importFiles::importAlgs::getAlgorithms() {
}

//--------------import houses--------------------//

//TODO

importFiles::importHouses::importHouses(const string& iniPath,
		importFiles& _parent) :
		parent(_parent) {
}

const vector<map<House, string>>& importFiles::importHouses::getHouses() {
}


