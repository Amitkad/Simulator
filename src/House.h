#ifndef HOUSE_H
#define HOUSE_H
#include <string>
 

using namespace std;
class House {
	string name;
    char** matrix;
    int dockingX;
    int dockingY;
    int colCount;
    int rowCount;
    int errCode=1; //code 0 - no docking station; code 1 - ok; code 2 - too many docking stations

    void setWalls();// put walls around
    void setDock();//sets the docking station (x,y) and count if put error if no docking or more than one docking station.

	public:
	  
	  /*
	   constructor
	   matrix - the surface of the house
	   colCount - amount of cols
	   rowCount - amount of rows
	   name - house name
	   description - house description
	   */
      House( char** matrix , int colCount , int rowCount, string name);
      
      /*
       destructor
       free the matrix
       */
      ~House();
      /*getters*/
      const string& getName()const;
      int getDockStationX();
      int getDockStationY();
      int getColCount();
      int getRowCount();
      int getErr();
      char** getMatrix() const;
      /*
       * calculate dust amount in house
       * @return 
       * returns the sum of dust in the house
       */
      int initDustAmount();
};

#endif
