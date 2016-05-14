#ifndef INPUTPARSER_H_
#define INPUTPARSER_H_
#include <stdio.h>
#include <stdlib.h>
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
#include <dirent.h>
#include <algorithm>
#include <errno.h>
#include <dlfcn.h>
#include "AbstractAlgorithm.h"
#include "House.h"

using namespace std;

/*a class which load program's files:
 * 			1. checking cmd line arguments validity
 * 			2. opens config file, checks data valididy and loads it into the program
 * 			3. opens all houses files, checks data valididy and loads them into the program
 * 			4. dynamically loads all .so algorithms into the program.
 */

class importFiles {

	// ---------- config parser nested private class -------//

	// subClass in charge of config file load
	class importConfig {

		importFiles& parent; //pointer to base class
		map<string, int> parameters; //config.ini <parameter,value>

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

		//check all necessary parameters were loaded
		void checkParameters();

	public:
		//c'tor
		importConfig(const string& iniPath, importFiles& _parent);
		//d'ctor
		~importConfig() = default;
		//no need for operator= and copy c'tor
		importConfig& operator=(const importConfig&) = delete;
		importConfig(const importConfig&) = delete;

		//"parametrs" getter function
		const map<string, int>& getParameters() const;
	};

	// ---------- houses parser nested private class -------//

	// subClass in charge of .house files load
	class importHouses {
		importFiles& parent; //pointer to base class
		map<House*, string> houses; //list of houses: <house*,Error> (if no errors then error="")

		/*inserts all .house files data to House objects
		 * @param
		 * dirList - list of .house files in housePath directory
		 */
		void insertHousesFromFile(vector<string> dirList);
		//returns true iff s is a string represents positive int.
		static bool is_number(const std::string& s);

	public:
		//c'tor, d'tor
		importHouses(const string& iniPath, importFiles& _parent);
		~importHouses();
		//no need for operator= and copy c'tor
		importHouses& operator=(const importHouses&) = delete;
		importHouses(const importHouses&) = delete;

		//"houses" getter
		const map<House*, string>& getHouses() const;
	};

	// ---------- algorithms parser nested private class -------//

	// subClass in charge of .so algorithm files dynamic load
	class importAlgs {
		importFiles& parent; //pointer to base class
		map<string, pair<AbstractAlgorithm*, string>> algorithms; //list of algorithms: map<alg_name,pair<abstractAlgorithm*, err_string>>(if no errors then error="")
		vector<void*> handlers; //handlers list to dlclose in d'ctor

		/*dynamically loads all .so files in dirList to algorithms
		 * @param
		 * dirList - list of .so files in algorithmsPath directory
		 */
		void insertAlgsFromFile(vector<string> dirList);

	public:
		//c'tor
		importAlgs(const string& iniPath, importFiles& _parent);
		~importAlgs();
		importAlgs& operator=(const importAlgs&) = delete;
		importAlgs(const importAlgs&) = delete;

		//"algorithms" getter function - **not const** because it might be changed by others
		map<string, pair<AbstractAlgorithm*, string>>& getAlgorithms();

	};

	// ---------- fileLister in use with algorithms and houses -------//
	//filesLister code from class. returns file list of a directory basePath
	class FilesLister {
	public:
		FilesLister(const string& basePath);
		virtual void refresh();
		vector<string> getFilesList();
		bool getErr() const;
		//err setter
		void setErr(bool err);

	protected:
		vector<string> filesList_;
		string basePath_;
		bool err = false; //fileLister err flag

	private:
		static string concatenateAbsolutePath(const string& dirPath, const string& fileName);
	};
	//inherited class of "fileLister for specific suffix"
	class FilesListerWithSuffix: public FilesLister {
	public:
		FilesListerWithSuffix(const string& basePath, const string& suffix);
		virtual void refresh();
		string getBasePath() const;

	protected:
		void filterFiles();
		static inline bool endsWith(std::string value, std::string ending);
		string suffix_;
	};


	//Base class's ("importFiles") members//

	bool err = false; //true iff there was an error
	string algorithmPath, housePath, configPath, scoreFormulaPath,threads; //paths of algorithms, houses, config directories
	importAlgs* algorithms; //algorithms parser
	importHouses* houses; //houses parser
	importConfig* config; //config.ini parser

	/*check flags validity and update directory paths accordingly. changes err to be true if something is wrong in cmd line or directories
	 * @params
	 * argc, argv - input arguments
	 */
	void checkArgValidity(int argc, char* argv[]);
	//if toChack flag exists then update pathVariable to be it's value
	void checkFlag(int argc, char* argv[], string toCheck, string& pathVariable);
	//finds toFind index in the array argv[]. return -1 if not found
	int indexOf(int argc, char* argv[], string toFind);

	//fills config, algorithms and houses data from files
	void fillInputFromFiles(int argc, char* argv[]);

public:
	//c'tor
	importFiles(int argc, char* argv[]);
	~importFiles();
	importFiles& operator=(const importFiles&) = delete;
	importFiles(const importFiles&) = delete;

	void printErrors();

	//class member getters
	map<string, pair<AbstractAlgorithm*, string>>& getAlgorithms();
	const map<House*, string>& getHouses() const;
	vector<House*> getGoodHouses() const;
	const map<string, int>& getParameters() const;
	string getAlgPath() const;
	string getHousePath() const;
	bool getErr();

	// setters
	void setErr(const bool err);
	void setHousePath(const string path);
	void setAlgPath(const string path);
};

#endif /* INPUTPARSER_H_ */

