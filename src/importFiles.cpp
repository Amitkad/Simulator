#include "importFiles.h"
#define SPACE 32
#define MIN(a,b) ((a) < (b) ? (a) : (b))

using namespace std;

//TODO - destructors,move,copy...
// -------------------------------------------------- Base Class: import files --------------------------------------------------------//

//c'tor
importFiles::importFiles(int argc, char* argv[]) :
		algorithmPath("./"), housePath("./"), configPath("./config.ini") {

	checkArgValidity(argc, argv);
	if (err)
		return;
	this->fillInputFromFiles(argc, argv);
}

//check flags validity and number of parameters, updates to new paths and put error accordingly
void importFiles::checkArgValidity(int argc, char* argv[]) {

	checkFlag(argc,argv,string("-config"), configPath);
	if (err)
		return;
	checkFlag(argc,argv,string("-house_path"), housePath);
	if (err)
		return;
	checkFlag(argc,argv,string("­-algorithm_path"), algorithmPath);
	if (err)
		return;
}
void importFiles::checkFlag(int argc, char* argv[], string toCheck, string& pathVariable) {
	int i = indexOf(argc,argv,toCheck);
	if (i != -1) {
		if (i != (argc - 1)) {
			if ((string(argv[i + 1]) != "-config") && (string(argv[i + 1]) != "-house_path") && (string(argv[i + 1]) != "-algorithm_path")) {
				pathVariable = string(argv[i + 1]);
			} else {
				setErr(true);
				cout << "Usage: simulator [-config <config path>] [-house_path <house path>][-algorithm_path <algorithm path>]\n" << endl;
				return;
			}
		}else {
			setErr(true);
			cout << "Usage: simulator [-config <config path>] [-house_path <house path>][-algorithm_path <algorithm path>]\n" << endl;
			return;
		}
	}
}

int importFiles::indexOf(int argc, char* argv[], string toFind){
	std::vector<std::string> args(argv, argv+argc);
	  for (size_t i = 1; i < args.size(); ++i) {
	      if (args[i] == toFind) {
	          return i;
	      }
	  }
	  return -1;
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
	cout << "success" << endl;

}

//class member getters
map<string, pair<AbstractAlgorithm*, string>>& importFiles::getAlgorithms() {
	return algorithms->getAlgorithms();
}
const map<House*, string>& importFiles::getHouses() const {
	return houses->getHouses();
}
vector<House*> importFiles::getGoodHouses() const {
	vector<House*> vec;
	map<House*, string> map = houses->getHouses();
	for (auto itr = map.begin(); itr != map.end(); ++itr) {
		if (itr->second.empty())
			vec.push_back(itr->first);
	}
	return vec;
}
const map<string, int>& importFiles::getParameters() const {
	return config->getParameters();
}

//setters
void importFiles::setErr(bool err) {
	this->err = err;
}
void importFiles::setHousePath(const string path) {
	this->housePath = path;
}
void importFiles::setAlgPath(const string path) {
	this->algorithmPath = path;
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
		cout << "Usage: simulator [-config <config path>] [-house_path <house path>][-algorithm_path <algorithm path>]\n" << endl;
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

	if (parameters.find("MaxStepsAfterWinner") == parameters.end()) {
		missingFiles += "MaxStepsAfterWinner, ";
		cnt++;
	}
	if (parameters.find("BatteryCapacity") == parameters.end()) {
		missingFiles += "BatteryCapacity, ";
		cnt++;
	}
	if (parameters.find("BatteryConsumptionRate") == parameters.end()) {
		missingFiles += "BatteryConsumptionRate, ";
		cnt++;
	}
	if (parameters.find("BatteryRechargeRate") == parameters.end()) {
		missingFiles += "BatteryRechargeRate, ";
		cnt++;
	}
	if (!missingFiles.empty()) {
		parent.setErr(true);
		cout << "config.ini missing " << cnt << " parameter(s): " << missingFiles.substr(0, missingFiles.find_last_of(",")) << endl;
	}
}

const map<string, int>& importFiles::importConfig::getParameters() const {
	return parameters;
}

//-------------------------------------------------------- Nested: import houses --------------------------------------------------------//

importFiles::importHouses::importHouses(const string& iniPath, importFiles& _parent) :
		parent(_parent) {
	FilesListerWithSuffix housesLister = FilesListerWithSuffix(iniPath, ".house");
//if dir is missing and no
	if (housesLister.getErr()) {
		cout << "Usage: simulator [-config <config path>] [-house_path <house path>][-algorithm_path <algorithm path>]\n" << endl;
		parent.setErr(true);
		return;
	}
	parent.setHousePath(housesLister.getBasePath());
//if there are no .house files in the directory
	if (housesLister.getFilesList().size() == 0) {
		cout << "Usage: simulator [-config <config path>] [-house_path <house path>][-algorithm_path <algorithm path>]\n" << endl;
		parent.setErr(true);
		return;
	}
	insertHousesFromFile(housesLister.getFilesList());
	if (parent.getErr())
		return;

}

void importFiles::importHouses::insertHousesFromFile(vector<string> dirVec) {
	string name;
	string s_maxSteps, s_rows, s_cols; //value in string
	string err, line;
	int maxSteps, rows, cols; //values in numbers
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
		getline(fin, name);
		//fin.ignore(); //skip name line
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
		House* house = new House(matrix, cols, rows, (*itr).substr((*itr).find_last_of("/\\") + 1));
		if (house->getErr() == 0) {		//no docking stat
			houses.insert(std::make_pair(house, "missing docking station (no D in house)"));
			errcnt++;
		}
		if (house->getErr() > 1) {		//too many docking stat
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
		for (map<House*, string>::const_iterator itr = houses.begin(); itr != houses.end(); ++itr) {
			cout << (*itr).first->getName() << ":" << (*itr).second << endl;
		}
	}
}

bool importFiles::importHouses::is_number(const std::string& s) {
	return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) {return !std::isdigit(c);}) == s.end();
}

const map<House*, string>& importFiles::importHouses::getHouses() const {
	return houses;
}

//-------------------------------------------------------- Nested: import algorithms --------------------------------------------------------//

importFiles::importAlgs::importAlgs(const string& iniPath, importFiles& _parent) :
		parent(_parent) {
	FilesListerWithSuffix algorithmLister = FilesListerWithSuffix(iniPath, ".so");
//if dir didnt exsit
	if (algorithmLister.getErr()) {
		cout << "Usage: simulator [-config <config path>] [-house_path <house path>][-algorithm_path <algorithm path>]\n" << endl;
		parent.setErr(true);
		return;
	}
	parent.setAlgPath(algorithmLister.getBasePath());
//if there are no .so files in the directory
	if (algorithmLister.getFilesList().size() == 0) {
		cout << "Usage: simulator [-config <config path>] [-house_path <house path>][-algorithm_path <algorithm path>]\n" << endl;
		parent.setErr(true);
		return;
	}
	insertAlgsFromFile(algorithmLister.getFilesList());
	if (parent.getErr())
		return;
}

void importFiles::importAlgs::insertAlgsFromFile(vector<string> dirVec) {
	string err = "";
	size_t errcnt = 0;
	void *hndl;
	typedef AbstractAlgorithm* (*maker)();
	for (vector<string>::const_iterator itr = dirVec.begin(); itr != dirVec.end(); ++itr) { //for each .house file:
		hndl = dlopen((*itr).c_str(), RTLD_NOW);
		if (hndl == nullptr) { //if there was an error opening .so file
			err = string("file cannot be loaded or is not a valid .so");
			errcnt++;
			algorithms.insert(make_pair((*itr).substr((*itr).find_last_of("/\\") + 1), pair<AbstractAlgorithm*, string>(NULL, err))); //insert <file_name,<NULL,err>>
			continue;
		}
		maker algMaker = (maker) dlsym(hndl, "getAbstractAlgorithmPointer");
		if (algMaker == nullptr) {
			err = string("file cannot be loaded or is not a valid .so"); //TODO
			errcnt++;
			algorithms.insert(std::make_pair((*itr).substr((*itr).find_last_of("/\\") + 1), pair<AbstractAlgorithm*, string>(NULL, err))); //insert <file_name,<NULL,err>>
			continue;
		}
		algorithms.insert(make_pair((*itr).substr((*itr).find_last_of("/\\") + 1), make_pair(algMaker(), err))); //insert <file_name,<new alg,err>>
	}
//if all files in folder are bad
	if (errcnt == algorithms.size()) {
		parent.setErr(true);
		cout << "All house files in target folder" << parent.getAlgPath() << "cannot be opened or are invalid:" << endl;
		for (auto itr = algorithms.begin(); itr != algorithms.end(); ++itr) {
			cout << (*itr).first << ":" << (*itr).second.second << endl;
		}
		return;
	}
}

map<string, pair<AbstractAlgorithm*, string>> & importFiles::importAlgs::getAlgorithms() {
	return algorithms;
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

string importFiles::FilesListerWithSuffix::getBasePath() const {
	return basePath_;
}

