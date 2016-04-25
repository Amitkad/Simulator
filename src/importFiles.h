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
#include "AbstractAlgorithm.h"
#include "house.h"

using namespace std;

//TODO put comments
class importFiles {


	// ---------- config parser nested private class -------//
	class importConfig {
		importFiles& parent;
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
		map<AbstractAlgorithm*, string> algorithms; //list of algorithms: <algorithm,Error>(if no errors then error="")

		/*load the file iniPath or default config.ini if iniPath doesn't exists
		 * @param
		 * iniPath - path for config file
		 */
		void loadFromFile(const string& iniPath);
	public:
		//c'tor
		importAlgs(const string& iniPath,importFiles& _parent);
		//"algorithms" getter function
		const map<AbstractAlgorithm*, string>& getAlgorithms();
	};

	// ---------- houses parser nested private class -------//
	class importHouses {
		importFiles& parent;
		map<House, string> houses; //list of houses: <house,Error> (if no errors then error="")


		void insertHousesFromFile(vector<string> dirList);

		void checkHousesValidity();
		static bool is_number(const std::string& s);

		/*load the file iniPath or default config.ini if iniPath doesn't exists
		 * @param
		 * iniPath - path for config file
		 */
		void loadFromFile(const string& iniPath);
	public:
		//c'tor
		importHouses(const string& iniPath,importFiles& _parent);
		//"houses" getter function
		const map<House, string>& getHouses();
	};
	// ---------- fileLister in use with algorithms and houses -------//

	class FilesLister
	{
	public:
	  FilesLister(const string& basePath);
	  virtual void refresh();
	  vector<string> getFilesList();
	  bool getErr();
	  //err setter
	  void setErr(bool err);

	protected:
	  vector<string> filesList_;
	  string basePath_;
	  bool err=false; //fileLister err flag


	private:
	  static string concatenateAbsolutePath(const string& dirPath, const string& fileName);
	};
	//inherited class of fileLister for specidif suffix
	class FilesListerWithSuffix : public FilesLister
	{
	public:
	  FilesListerWithSuffix(const string& basePath, const string& suffix);
	  virtual void refresh();

	protected:
	  void filterFiles();
	  static inline bool endsWith(std::string value, std::string ending);
	  string suffix_;
	};


	//end of nested classes//

	bool err = false;//true iff there was an error
	string algorithmPath, housePath, configPath;
	importAlgs* algorithms; //algorithms parser
	importHouses* houses; //houses parser
	importConfig* config; //cnfig.ini parser

	//check flags validity and number of parameters
	bool checkArgValidity(int argc, char* argv[]);

	//fills config, algorithms and houses data from files
	void fillInputFromFiles(int argc, char* argv[]);

public:
	//c'tor
	importFiles(int argc, char* argv[]);

	//class member getters
	const map<AbstractAlgorithm*, string>& getAlgorithms();
	const map<House, string>& getHouses();
	const map<string, int>& getParameters();
	string getAlgPath() const;
	string getHousePath() const;
	bool getErr();
	//err setter
	void setErr(bool err);
};

#endif /* INPUTPARSER_H_ */

//vector<map<AbstractAlgorithm*, char*>> algorithms; //list of algorithms: <algorithm,Error>
//	vector<map<House, char*>> houses; //list of houses: <house,Error> (if no errors then error="")
//	map<string, int> parameters; //cnfig.ini <parameter,value>
