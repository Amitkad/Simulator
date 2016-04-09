#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "House.h"

House::House(char** matrix, int colCount, int rowCount, string& _name, string& _description) {
	this->matrix = matrix;
	this->colCount = colCount;
	this->rowCount = rowCount;
	this->name = _name;
	this->description=_description;
	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < colCount; j++) {
			if (matrix[i][j] == 'D') {
				this->dockingY = i;
				this->dockingX = j;
			}
		}
	}
}
House::~House(){
	for (int i = 0; i < rowCount; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

const string& House::getName(){
	return this->name;
}
const string& House::getDescription() {
	return this->description;
}


int House::getDockStationX() {
	return this->dockingX;
}
int House::getDockStationY() {
	return this->dockingY;
}
int House::getColCount() {
	return this->colCount;
}
int House::getRowCount() {
	return this->rowCount;
}
char** House::getMatrix() const {
	return this->matrix;
}
int House::initDustAmount() {
	int sum = 0;
	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < colCount; j++) {
			if ((matrix[i][j] > '0') && (matrix[i][j] <= '9')) {
				sum += (matrix[i][j] - '0');
			}
		}
	}
	return sum;
}

