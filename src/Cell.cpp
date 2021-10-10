#include "Cell.h"

Cell::Cell(int value, int positionX, int positionY, int maxValueCell) {
	this->value = value;
	this->positionX = positionX;
	this->positionY = positionY;

	//If value isn't -1, we put all possible values in the array
	if (value == -1) {
		for (int i = 1; i <= maxValueCell; i++) {
			this->possibleValues.push_back(i);
		}
	}
}

Cell::Cell(int value, int positionX, int positionY, std::vector<int> possibleValues) {
	this->value = value;
	this->positionX = positionX;
	this->positionY = positionY;

	//If value isn't -1, we put all possible values in the array
	for (int i : possibleValues) {
		this->possibleValues.push_back(i);
	}
}

void Cell::removeFromPossibleValues(int numberToRemove) {
	int positionOfNumber = -1;

	for (int i = 0; i < this->possibleValues.size(); i++) {
		if (this->possibleValues.at(i) == numberToRemove) {
			positionOfNumber = i;
		}
	}

	//We check if we found the number and if so, we remove it
	if (positionOfNumber != -1) {
		this->possibleValues.erase(this->possibleValues.begin() + positionOfNumber);
	}
}

std::vector<int> Cell::getPossibleValues() {
	return this->possibleValues;
}

bool Cell::valueDetermined() {
	return this->possibleValues.size() == 1;
}

bool Cell::valueSet() {
	return this->value != -1;
}

void Cell::setValue(int value) {
	this->value = value;
}

int Cell::getValue() {
	return this->value;
}

int Cell::getPositionX() {
	return this->positionX;
}

int Cell::getPositionY() {
	return this->positionY;
}

bool Cell::valueInPossibles(int value) {
	if (this->value != -1) {
		return false;
	}

	bool found = false;
	//We only have to check if the value hasn't been determined for this cell
	for (int possibleValue : this->possibleValues) {
		if (possibleValue == value) {
			found = true;
		}
	}
	return found;
}