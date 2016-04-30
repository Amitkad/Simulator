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
		//TODO - �����
		void checkParameters();
	public:
		//c'tor
		importConfig(const string& iniPath,importFiles& _parent);
		//"parametrs" getter function
		 const map<string, int>& getParameters() const;
	};



	// ---------- houses parser nested private class -------//
	class importHouses {
		importFiles& parent;
		map<House*, string> houses; //list of houses: <house*,Error> (if no errors then error="")

		void insertHousesFromFile(vector<string> dirList);
		static bool is_number(const std::string& s);

	public:
		//c'tor
		importHouses(const string& iniPath,importFiles& _parent);
		//"houses" getter function
		 const map<House*, string>& getHouses() const;
	};

	// ---------- algorithms parser nested private class -------//
		class importAlgs {
			importFiles& parent;
			map<string,pair<AbstractAlgorithm*,string>> algorithms; //list of algorithms: map<alg_name,pair<abstractAlgorithm*, err_string>>(if no errors then error="")

			void insertAlgsFromFile(vector<string> dirList);

		public:
			//c'tor
			importAlgs(const string& iniPath,importFiles& _parent);
			//"algorithms" getter function
			 map<string,pair<AbstractAlgorithm*,string>>& getAlgorithms();
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
	  string getBasePath() const;

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
	void checkArgValidity(int argc, char* argv[]);
	void checkFlag(int argc, char* argv[], string toCheck, string& pathVariable);
	int indexOf(int argc, char* argv[], string toFind);

	//fills config, algorithms and houses data from files
	void fillInputFromFiles(int argc, char* argv[]);

public:
	//c'tor
	importFiles(int argc, char* argv[]);

	//class member getters
	map<string,pair<AbstractAlgorithm*,string>>& getAlgorithms();
	const map<House*, string>& getHouses() const;
	vector<House*> getGoodHouses() const;
	const map<string, int>& getParameters()const;

	string getAlgPath() const;
	string getHousePath() const;
	bool getErr();
	// setters
	void setErr(const bool err);
	void setHousePath(const string path);
	void setAlgPath(const string path);
};

#endif /* INPUTPARSER_H_ */

//vector<map<AbstractAlgorithm*, char*>> algorithms; //list of algorithms: <algorithm,Error>
//	vector<map<House, char*>> houses; //list of houses: <house,Error> (if no errors then error="")
//	map<string, int> parameters; //cnfig.ini <parameter,value>
