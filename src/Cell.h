#pragma once

#include <vector>

class Cell{
private:
	//Value of the cell
	int value;

	//Possible values of the cell
	std::vector<int> possibleValues;

	//Position of the cell on the grid
	//Follows this order:
	/*
	(x,y)
	(0,0)|(1,0)|(2,0)
	(0,1)|(1,1)|(2,1)
	(0,2)|(1,2)|(2,2)
	*/
	int positionX;
	int positionY;

public:
	Cell(int value, int positionX, int positionY, int maxValueCell);
	Cell(int value, int positionX, int positionY, std::vector<int> possibleValues);
	//Removes the number from the possible values
	void removeFromPossibleValues(int numberToRemove);
	std::vector<int> getPossibleValues();
	//True if the possibleValues is of size 1, meaning that the number has been determined
	bool valueDetermined();
	//Checks if the value is set
	bool valueSet();
	//Sets Value
	void setValue(int value);
	int getValue();
	int getPositionX();
	int getPositionY();
	//Indicates if the value is in possibles
	bool valueInPossibles(int value);
};