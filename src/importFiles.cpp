#include "importFiles.h"
#define SPACE 32
#define MIN(a,b) ((a) < (b) ? (a) : (b))

using namespace std;

//TODO - destructors
// -------------------------------------------------- Base Class: import files --------------------------------------------------------//

//c'tor
importFiles::importFiles(int argc, char* argv[]) :
		algorithmPath("./"), housePath("./"), configPath("./config.ini") {

	if (!checkArgValidity(argc, argv))
		return;
	this->fillInputFromFiles(argc, argv);
}

//check flags validity and number of parameters, updates to new paths and put error accordingly
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
const map<AbstractAlgorithm*, string>& importFiles::getAlgorithms() {
	return algorithms->getAlgorithms();
}
const map<House, string>& importFiles::getHouses() {
	return houses->getHouses();
}
const map<string, int>& importFiles::getParameters() {
	return config->getParameters();
}

//setters
void importFiles::setErr(bool err) {
	this->err = err;
}
void importFiles::setHousePath(const string path){
	this->housePath=path;
}
void importFiles::setAlgPath(const string path){
	this->algorithmPath=path;
}
// getters
bool importFiles::getErr() {
	return this->err;
}
string importFiles::getHousePath() const {
	return housePath;
}
string importFiles::getAlgPath() const {
	return algorithmPath;
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
		parent.setErr(true);
		cout << "Usage: simulator [­config <config path>] [­house_path <house path>][­algorithm_path <algorithm path>]\n" << endl;
		return;
	}
	string line;
	while (getline(fin, line)) //fill "parameters" with the the parameters and values
	{
		this->processLine(line);
	}
}

importFiles::importConfig::importConfig(const string& iniPath, importFiles& _parent) :
		parent(_parent) {
	this->loadFromFile(iniPath);
	if (parent.getErr())
		return;
	checkParameters(); //checks all config parameteres exsits
}

void importFiles::importConfig::checkParameters() {
	int cnt = 0;
	string missingFiles = string("");

	(parameters.find("MaxStepsAfterWinner") == parameters.end()) ? (missingFiles += "MaxStepsAfterWinner, ", cnt++) : missingFiles += "";
	(parameters.find("BatteryCapacity") == parameters.end()) ? (missingFiles += "BatteryCapacity, ", cnt++) : missingFiles += "";
	(parameters.find("BatteryConsumptionRate") == parameters.end()) ? (missingFiles += "BatteryConsumptionRate, ", cnt++) : missingFiles += "";
	(parameters.find("BatteryRechargeRate") == parameters.end()) ? (missingFiles += "BatteryRechargeRate, ", cnt++) : missingFiles += "";

	if (!missingFiles.empty()) {
		parent.setErr(true);
		cout << "config.ini missing " << cnt << " parameter(s): " << missingFiles.substr(0, missingFiles.find_last_of(","));

	}
}

const map<string, int>& importFiles::importConfig::getParameters() {
	return parameters;
}

//-------------------------------------------------------- Nested: import houses --------------------------------------------------------//

//TODO

importFiles::importHouses::importHouses(const string& iniPath, importFiles& _parent) :
		parent(_parent) {
	FilesListerWithSuffix housesLister = FilesListerWithSuffix(iniPath, ".house");
	//if dir is missing and no
	if (housesLister.getErr()) {
		cout << "Usage: simulator [­config <config path>] [­house_path <house path>][­algorithm_path <algorithm path>]\n" << endl;
		parent.setErr(true);
		return;
	}
	parent.setHousePath(housesLister.getBasePath());
	//if there are no .house files in the directory
	if (housesLister.getFilesList().size() == 0) {
		cout << "Usage: simulator [­config <config path>] [­house_path <house path>][­algorithm_path <algorithm path>]\n" << endl;
		parent.setErr(true);
		return;
	}
	insertHousesFromFile(housesLister.getFilesList());
	if (parent.getErr())
		return;

}

void importFiles::importHouses::insertHousesFromFile(vector<string> dirVec) {
	string name;
	string s_maxSteps, s_rows, s_cols;
	string err, line;
	int maxSteps, rows, cols;
	char** matrix;
	size_t errcnt = 0;
	for (vector<string>::const_iterator itr = dirVec.begin(); itr != dirVec.end(); ++itr) { //for each .house file:
		ifstream fin((*itr).c_str()); //open file
		if (!fin.good()) {  // check open() success
			err = string("cannot open file");
			errcnt++;
			houses.insert(std::make_pair(new House(NULL, 0, 0, (*itr).substr((*itr).find_last_of("/\\") + 1)), err));
			continue;
		}
		fin.ignore(); //skip name line
		name = (*itr).substr((*itr).find_last_of("/\\") + 1); //get file name from path
		//get parameters from file and check them
		getline(fin, s_maxSteps);
		if (!is_number(s_maxSteps)) {
			err = "line number 2 in house file shall be a positive number, found:" + s_maxSteps;
			errcnt++;
			houses.insert(std::make_pair(new House(NULL, 0, 0, (*itr).substr((*itr).find_last_of("/\\") + 1)), err));
			continue;
		}
		maxSteps = atoi(s_maxSteps.c_str());
		getline(fin, s_rows);
		if (!is_number(s_rows)) {
			err = "line number 3 in house file shall be a positive number, found:" + s_rows;
			errcnt++;
			houses.insert(std::make_pair(new House(NULL, 0, 0, (*itr).substr((*itr).find_last_of("/\\") + 1)), err));
			continue;
		}
		rows = atoi(s_rows.c_str());
		getline(fin, s_cols);
		if (!is_number(s_cols)) {
			err = "line number 4 in house file shall be a positive number, found:" + s_cols;
			errcnt++;
			houses.insert(std::make_pair(new House(NULL, 0, 0, (*itr).substr((*itr).find_last_of("/\\") + 1)), err));
			continue;
		}
		cols = atoi(s_cols.c_str());
		//allocate memory for the house matrix
		matrix = (char**) malloc(sizeof(char*) * rows);
		for (int i = 0; i < rows; i++) {
			matrix[i] = (char*) malloc(sizeof(char) * cols);
			getline(fin, line);
			int m = MIN(line.length(), cols);
			line.copy(matrix[i], m, 0);
			if (m < cols)
				for (int k = m; k < cols; k++)
					matrix[i][k] = SPACE;
		}
		//create the house object adn check validity
		House house = new House(matrix, cols, rows, (*itr).substr((*itr).find_last_of("/\\") + 1));
		if (house.getErr() == 0) {		//no docking stat
			houses.insert(std::make_pair(house, "missing docking station (no D in house)"));
			errcnt++;
		}
		if (house.getErr() > 1) {		//too many docking stat
			houses.insert(std::make_pair(house, "too many docking stations (more than one D in house)"));
			errcnt++;
		} else
			//1 docking
			houses.insert(std::make_pair(house, ""));
	}
	//if all files are bad
	if (errcnt == houses.size()) {
		parent.setErr(true);
		cout << "All house files in target folder" << parent.getHousePath() << "cannot be opened or are invalid:" << endl;
		for (map<House, string>::const_iterator itr = houses.begin(); itr != houses.end(); ++itr) {
			cout << (*itr).first.getName() << ":" << (*itr).second << endl;
		}
	}
}

bool importFiles::importHouses::is_number(const std::string& s) {
	return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) {return !std::isdigit(c);}) == s.end();
}

const map<House, string>& importFiles::importHouses::getHouses() {
	return houses; //TODO return only good houses in vector; create another getter for only errors
}

//-------------------------------------------------------- Nested: import algorithms --------------------------------------------------------//

//TODO
importFiles::importAlgs::importAlgs(const string& iniPath, importFiles& _parent) :
		parent(_parent) {
	FilesListerWithSuffix algorithmLister = FilesListerWithSuffix(iniPath, ".so");
	//if dir didnt exsit
	if (algorithmLister.getErr()) {
		cout << "Usage: simulator [­config <config path>] [­house_path <house path>][­algorithm_path <algorithm path>]\n" << endl;
		parent.setErr(true);
		return;
	}
	parent.setAlgPath(algorithmLister.getBasePath());
	//if there are no .so files in the directory
	if (algorithmLister.getFilesList().size() == 0) {
		cout << "Usage: simulator [­config <config path>] [­house_path <house path>][­algorithm_path <algorithm path>]\n" << endl;
		parent.setErr(true);
		return;
	}
	insertHousesFromFile(algorithmLister.getFilesList());
	if (parent.getErr())
		return;
	checkHousesValidity(); //TODO
}

const map<AbstractAlgorithm*, string>& importFiles::importAlgs::getAlgorithms() {

}

//-------------------------------------------------------- Nested: FileLister --------------------------------------------------------//
string importFiles::FilesLister::concatenateAbsolutePath(const string& dirPath, const string& fileName) {
	if (dirPath.empty())
		return fileName;
	if (dirPath.back() == '/')
		return dirPath + fileName;
	return dirPath + "/" + fileName;
}

importFiles::FilesLister::FilesLister(const string& basePath) :
		basePath_(basePath) {
	this->refresh();
}

void importFiles::FilesLister::refresh() {
	DIR *dir;
	struct dirent *ent;
	this->filesList_.clear();
	if ((dir = opendir(this->basePath_.c_str())) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			this->filesList_.push_back(concatenateAbsolutePath(this->basePath_, ent->d_name));
		}
		closedir(dir);
	} else {
		// could not open directory
		if (errno == ENOENT) //if sirectory does not exsits check in current dir
		{
			basePath_ = string(".");

			refresh();
			return;

		} else {
			err = true;
			return;
		}
	}
	std::sort(this->filesList_.begin(), this->filesList_.end());
}

bool importFiles::FilesLister::getErr() {
	return this->err;
}

vector<string> importFiles::FilesLister::getFilesList() {
	return this->filesList_;
}

//-------------------------------------------------------- Nested: FileLister-with-suffix --------------------------------------------------------//
inline bool importFiles::FilesListerWithSuffix::endsWith(std::string value, std::string ending) {
	if (ending.size() > value.size())
		return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

importFiles::FilesListerWithSuffix::FilesListerWithSuffix(const string& basePath, const string& suffix) :
		FilesLister(basePath), suffix_(suffix) {
	this->filterFiles();
}

void importFiles::FilesListerWithSuffix::refresh() {
	FilesLister::refresh();
	this->filterFiles();
}

void importFiles::FilesListerWithSuffix::filterFiles() {
	vector<string> temp = this->filesList_;
	this->filesList_.clear();
	for (vector<string>::iterator itr = temp.begin(); itr != temp.end(); ++itr) {
		if (endsWith(*itr, this->suffix_)) {
			this->filesList_.push_back(*itr);
		}
	}
}

string importFiles::FilesListerWithSuffix::getBasePath() const{
	return basePath_;
}

