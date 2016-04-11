#include <stdio.h>
#include <stdlib.h>
#include "configParser.h"

using namespace std;

  static vector<string> split(const string &s, char delim) {
    vector<string> elems;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
  }

  static string trim(string& str)
  {
    str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
    str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
    return str;
  }

  void configParser::processLine(const string& line)
  {

    vector<string> tokens = split(line, '=');
    if (tokens.size() != 2)
    {
      return;
    }

    this->parameters[trim(tokens[0])] = atoi(tokens[1].c_str()); //put parsed strings in "parametrs"
  }

  void configParser::loadFromFile(const string& iniPath)
    {
      this->parameters.clear();

      ifstream fin(iniPath.c_str());
      if (!fin.good())// check iniPath existence
    	  ifstream fin("config.ini");//TODO change from string to defined string "default_ConfigPath"
      string line;

      while (getline(fin, line)) //fill "parameters" with the the parameters and values
      {
        this->processLine(line);
      }
    }

  configParser::configParser(const string& iniPath)
  {
    this->loadFromFile(iniPath);
  }

const map<string,int>& configParser::getParameters(){
	return parameters;
}
