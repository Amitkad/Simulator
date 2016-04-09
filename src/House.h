#ifndef HOUSE_H
#define HOUSE_H
#include <string>
 

using namespace std;
class House {
	string name;
	string description;
    char** matrix;
    int dockingX;
    int dockingY;
    int colCount;
    int rowCount;

	public:
	  
	  /*
	   constructor
	   matrix - the surface of the house
	   colCount - amount of cols
	   rowCount - amount of rows
	   name - house name
	   description - house description
	   */
      House( char** matrix , int colCount , int rowCount, string& name, string& description );
      
      /*
       destructor
       free the matrix
       */
      ~House();
      /*getters*/
      const string& getName();
      const string& getDescription();
      int getDockStationX();
      int getDockStationY();
      int getColCount();
      int getRowCount();
      char** getMatrix() const;
      /*
       * calculate dust amount in house
       * @return 
       * returns the sum of dust in the house
       */
      int initDustAmount();
};

#endif
