#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "House.h"

House::House(char** matrix, int colCount, int rowCount, string _name) {
	this->matrix = matrix;
	this->colCount = colCount;
	this->rowCount = rowCount;
	this->name = _name;
	dockingX=0;
	dockingY=0;
	this->setWalls();
	this->setDock();
}
House::~House() {
	for (int i = 0; i < rowCount; i++) {
		free(matrix[i]);
	}
	if (matrix != NULL)
		free(matrix);
}
void House::setWalls() {
	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < colCount; j++) {
			if (i == 0 || j == 0 || i == rowCount - 1 || j == colCount - 1)
				matrix[i][j] = 'W';
		}
	}
}

void House::setDock() {
	int cnt=0;
	for (int i = 0; i < rowCount; i++) {
			for (int j = 0; j < colCount; j++) {
				if (matrix[i][j] == 'D') {
					cnt++;
					this->dockingY = i;
					this->dockingX = j;
				}
			}
		}
	errCode=cnt;
}

const string& House::getName() const {
	return this->name;
}

int House::getDockStationX() const {
	return this->dockingX;
}
int House::getDockStationY() const {
	return this->dockingY;
}
int House::getColCount() const {
	return this->colCount;
}
int House::getRowCount() const{
	return this->rowCount;
}
char** House::getMatrix() const {
	return this->matrix;
}
int House::getErr()const{
	return errCode;
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

