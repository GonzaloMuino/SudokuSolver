#include <fstream>
#include <iostream>
#include <string>

#include "Cell.h"
#include "Sudoku.h"

//Function to split a 
std::vector<std::string> Sudoku::splitString(std::string line, std::string separator) {
	std::vector<std::string> output;
	std::size_t current, previous = 0;

	//We find the first separator
	current = line.find(separator);

	//We search for segments while we aren't at the final position
	while (current != std::string::npos) {
		//We get the next segment of the string
		output.push_back(line.substr(previous, current - previous));
		//We get the starting position of the next segment
		previous = current + 1;
		//We find the next separator
		current = line.find(separator, previous);
	}

	//We have to get the final segment
	output.push_back(line.substr(previous, current - previous));

	return output;
}


Sudoku::Sudoku(std::vector<Cell*> cells, int size) {
	this->size = size;
	for (Cell* cell : cells) {
		this->cells.push_back(new Cell(cell->getValue(), cell->getPositionX(), cell->getPositionY(), cell->getPossibleValues()));
	}
}

Sudoku::Sudoku(std::string fileName) {
	//We detect the filetype
	std::vector<std::string> fileNameParts = this->splitString(fileName, ".");

	//We open the file
	std::ifstream file;
	file.open(fileName);

	//Holds the lines from the file
	std::string line;

	if (fileNameParts.size() == 0 || fileNameParts.at(fileNameParts.size() - 1).compare("sudoku") == 0) {
		//Personal type

		//We start reading the file. First line contains the size of the sudoku. 
		//It represents the amount of columns in one sub-square
		std::getline(file, line);
		this->size = std::atoi(line.c_str());

		//Now we read the rest of the file.
		//The format is the following:
		//A|B|C|D|E...
		//A number different than -1 indicated a found value

		//Used to save the position of the cell
		int positionX = 0;
		int positionY = 0;
		while (std::getline(file, line)) {
			std::vector<std::string> lineParts = this->splitString(line, "|");
			for (std::string linepart : lineParts) {
				this->cells.push_back(new Cell(std::atoi(linepart.c_str()), positionX, positionY, this->size * this->size));
				positionX++;
			}
			positionX = 0;
			positionY++;
		}
	}
	else if (fileNameParts.at(fileNameParts.size() - 1).compare("sdk") == 0) {
		//Sadman based

		//Always size 3
		this->size = 3;

		//It can have comments starting with #
		//We simply ignore it

		//Used to save the current position of the cell
		int positionY = 0;
		while (std::getline(file, line)) {
			if (line[0] != '#') {
				for (int i = 0; i < line.size(); i++) {
					char aux = line.at(i);
					int value;
					if (aux == '.') {
						value = -1;
					}
					else {
						value = std::atoi(&aux);
					}
					this->cells.push_back(new Cell(value, i, positionY, this->size * this->size));
					
				}
				positionY++;
			}
		}
	}
	else if (fileNameParts.at(fileNameParts.size() - 1).compare("sdkstring") == 0) {
		//One line

		//Always size 3
		this->size = 3;

		//We get the line
		std::getline(file, line);

		for (int j = 0; j < this->size * this->size; j++) {
			for (int i = 0; i < this->size * this->size; i++) {
				char aux = line.at(i + this->size * this->size * j);
				int value = std::atoi(&aux);
				if (value == 0) {
					value = -1;
				}
				this->cells.push_back(new Cell(value, i, j, this->size * this->size));
			}
		}
	}
}

void Sudoku::print() {
	int currentColumn = 0;
	//Size of the strings to print. Used just to prettify
	int columnStringSize = std::to_string(this->size*this->size).length();

	//We start by drawing a line of -. First we determine the amount to draw 
	int amountToDraw = this->size * this->size * (columnStringSize + 1) + (this->size + 1) * 2 - 1;
	for (int i = 0; i < amountToDraw; i++) {
		std::cout << "-";
	}

	//We draw a new line
	std::cout << "\n";

	for (Cell* currentCell : this->cells) {
		//If it's the first, we draw a |
		if (currentCell->getPositionX() == 0) {
			std::cout << "| ";
		}

		//We draw the value
		int value = currentCell->getValue();
		if (value == -1) {
			for (int j = 0; j < columnStringSize; j++) {
				std::cout << ".";
			}
		}
		else {
			std::string valueString = std::to_string(currentCell->getValue());
			while (valueString.length() < columnStringSize) {
				valueString = "0" + valueString;
			}
			std::cout << valueString;
		}
		//We check if we have to draw a column separating the sub-squares
		if ((currentCell->getPositionX() + 1) % this->size == 0) {
			std::cout << " | ";
		}
		else {
			std::cout << " ";
		}

		//We check if we are at the last column. If so, we draw a line of - if we have to
		if (currentCell->getPositionX() == this->size * this->size - 1 && (currentCell->getPositionY() + 1) % this->size == 0) {
			//We draw a new line
			std::cout << "\n";

			//We draw the -
			for (int i = 0; i < amountToDraw; i++) {
				std::cout << "-";
			}

			//We draw a new line
			std::cout << "\n";
		}else if (currentCell->getPositionX() == this->size * this->size - 1) {
			//Final column, we transition to a new file
			std::cout << "\n";
		}
	}
}

void Sudoku::solve() {
	//First we prepare all the cells to eliminate the possible values that are already in that subsquare, row and column
	for (int i = 0; i < this->size * this->size; i++) {
		for (int j = 0; j < this->size * this->size; j++) {
			Cell* currentCell = this->getCellAt(i, j);
			if (currentCell->valueSet()) {
				//We have to remove the value of this cell from all the others in the possible vector
				int currentCellValue = currentCell->getValue();

				//We remove it from the row
				std::vector<Cell*> row = this->getRowAt(j);
				for (Cell* rowCell : row) {
					rowCell->removeFromPossibleValues(currentCellValue);
				}

				//We remove it from the column
				std::vector<Cell*> column = this->getColumnAt(i);
				for (Cell* columnCell : column) {
					columnCell->removeFromPossibleValues(currentCellValue);
				}

				//We remove it from the subSquare
				std::vector<Cell*> subSquare = this->getSubSquare(i,j);
				for (Cell* subSquareCell : subSquare) {
					subSquareCell->removeFromPossibleValues(currentCellValue);
				}
			}
		}
	}

	this->print();

	//We check how many loops have been made without a change
	int amountOfLoopsWithoutChange = 0;

	//Now we start to solve it
	while(!this->solved()){

		//We increment the counter
		amountOfLoopsWithoutChange++;

		/*
		//TODO Debug, Remove
		this->print();
		for (int i = 0; i < this->size * this->size; i++) {
			for (int j = 0; j < this->size * this->size; j++) {
				Cell* currentCell = this->getCellAt(i, j);

				if (!currentCell->valueSet()) {
					std::cout << currentCell->getPositionX();
					std::cout << "|";
					std::cout << currentCell->getPositionY();
					std::cout << "\n";

					std::vector<int> possibleValues = currentCell->getPossibleValues();
					for (int aux : possibleValues) {
						std::cout << aux;
						std::cout << ";";
					}
					std::cout << "\n";
				}
			}
		}*/

		//First we check if there is a cell that only has one value in it. If so, we put it in
		for (int i = 0; i < this->size * this->size; i++) {
			for (int j = 0; j < this->size * this->size; j++) {
				Cell* currentCell = this->getCellAt(i, j);
				if (!currentCell->valueSet() && currentCell->valueDetermined()) {
					this->setValueForCell(i, j, currentCell->getPossibleValues().at(0));
					amountOfLoopsWithoutChange = 0;
				}
			}
		}

		//Now we check if there is a row/column/sub-square that has a missing number only possible on a single cell

		//We check the rows
		for (int i = 0; i < this->size * this->size; i++) {
			//We get the row
			std::vector<Cell*> row = this->getRowAt(i);

			//We check for all numbers
			for (int j = 1; j <= this->size * this->size; j++) {
				//We have to check how many have the number in their possibles
				int cellsWithNumberInPossibles = 0;

				//This is made just to make it faster. We save the position of the last one with the number in possibles. If it's only one, we have already it's position
				int cellWithPossible = -1;

				//We have to get it's position
				int currentPosition = 0;
				for (Cell* currentCell : row) {
					if (!currentCell->valueSet()) {
						if (currentCell->valueInPossibles(j)) {
							cellsWithNumberInPossibles++;
							cellWithPossible = currentPosition;
						}
					}
					currentPosition++;
				}

				//We now check if it's only one cell with that number in possibles
				if (cellsWithNumberInPossibles == 1) {
					this->setValueForCell(cellWithPossible, i, j);
					amountOfLoopsWithoutChange = 0;
				}
			}
		}

		//We check the columns
		for (int i = 0; i < this->size * this->size; i++) {
			//We get the row
			std::vector<Cell*> column = this->getColumnAt(i);

			//We check for all numbers
			for (int j = 1; j <= this->size * this->size; j++) {
				//We have to check how many have the number in their possibles
				int cellsWithNumberInPossibles = 0;

				//This is made just to make it faster. We save the position of the last one with the number in possibles. If it's only one, we have already it's position
				int cellWithPossible = -1;

				//We have to get it's position
				int currentPosition = 0;
				for (Cell* currentCell : column) {
					if (!currentCell->valueSet()) {
						if (currentCell->valueInPossibles(j)) {
							cellsWithNumberInPossibles++;
							cellWithPossible = currentPosition;
						}
					}
					currentPosition++;
				}

				//We now check if it's only one cell with that number in possibles
				if (cellsWithNumberInPossibles == 1) {
					this->setValueForCell(i, cellWithPossible, j);
					amountOfLoopsWithoutChange = 0;
				}
			}
		}

		//We check the subsquare
		for (int i = 0; i < this->size * this->size; i++) {
			for (int j = 0; j < this->size * this->size; j++) {
				std::vector<Cell*> subSquare = this->getSubSquare(i, j);

				//We check for all numbers
				for (int z = 1; z <= this->size * this->size; z++) {
					//We have to check how many have the number in their possibles
					int cellsWithNumberInPossibles = 0;

					//This is made just to make it faster. We save the position of the last one with the number in possibles. If it's only one, we have already it's position
					int cellWithPossibleX = -1;
					int cellWithPossibleY = -1;

					for (Cell* currentCell : subSquare) {
						if (!currentCell->valueSet()) {
							if (currentCell->valueInPossibles(j)) {
								cellsWithNumberInPossibles++;
								cellWithPossibleX = currentCell->getPositionX();
								cellWithPossibleY = currentCell->getPositionY();
							}
						}
					}

					//We now check if it's only one cell with that number in possibles
					if (cellsWithNumberInPossibles == 1) {
						this->setValueForCell(cellWithPossibleX, cellWithPossibleY, j);
						amountOfLoopsWithoutChange = 0;
					}
				}
			}
		}
		
		
		//Next, we check if there is a subsquare, in which a number is only possible in one of it's columns/rows. If so, we have to remove it from the rest of subsquares of that row/column
		//We iterate through all subsquares
		for (int i = 0; i < this->size * this->size; i = i + this->size) {
			for (int j = 0; j < this->size * this->size; j = j + this->size) {

				//We are on the top-left cell of the subsquare. We start by checking if it's row has a unique value in it's possibles cells
				for (int z = 0; z < this->size; z++) {
					for (int t = 0; t < this->size; t++) {
						//We get the next checked cell
						//NOTE: We are currently on the column z and row t inside the sub-square
						Cell* currentCell = this->getCellAt(i + z, j + t);
						if (!currentCell->valueSet()) {
							//We get it's possible values
							std::vector<int> possibleValuesOfCell = currentCell->getPossibleValues();

							//We have to iterate now over this vector. We check if a value is present on other rows inside the cell
							for (int currentValue : possibleValuesOfCell) {
								bool valueInOtherRows = false;
								for (int u = 0; u < this->size; u++) {
									for (int v = 0; v < this->size; v++) {
										//We are currently checking the subsquare. First we have to make sure we aren't checking the same row
										//NOTE: We are currently comparing row u and column v
										if (t != v) {
											//We get the cell
											Cell* comparedCellInSubSquare = this->getCellAt(i + u, j + v);
											if (!comparedCellInSubSquare->valueSet() && comparedCellInSubSquare->valueInPossibles(currentValue)) {
												valueInOtherRows = true;
											}
										}
									}
								}
								//We have now checked all other rows in the subsquare. If it isn't in other rows, we must eliminate it from the rest of the possible in that row in other subsquares
								if (!valueInOtherRows) {
									int row = j + t;
									for (int u = 0; u < i; u++) {
										Cell* cell = this->getCellAt(u, row);
										if (!cell->valueSet() && cell->valueInPossibles(currentValue)) {
											cell->removeFromPossibleValues(currentValue);
											amountOfLoopsWithoutChange = 0;
										}
									}
									//We divide in 2 fors to skip over the ones in the subsquare
									for (int u = i + this->size; u < this->size * this->size; u++) {
										Cell* cell = this->getCellAt(u, row);
										if (!cell->valueSet() && cell->valueInPossibles(currentValue)) {
											cell->removeFromPossibleValues(currentValue);
											amountOfLoopsWithoutChange = 0;
										}
									}
								}
							}
						}
					}
				}

				//We repeat for the columns
				for (int z = 0; z < this->size; z++) {
					for (int t = 0; t < this->size; t++) {
						//We get the next checked cell
						//NOTE: We are currently on the row z and column t inside the sub-square
						Cell* currentCell = this->getCellAt(i + z, j + t);
						if (!currentCell->valueSet()) {
							//We get it's possible values
							std::vector<int> possibleValuesOfCell = currentCell->getPossibleValues();

							//We have to iterate now over this vector. We check if a value is present on other rows inside the cell
							for (int currentValue : possibleValuesOfCell) {
								bool valueInOtherColumns = false;
								for (int u = 0; u < this->size; u++) {
									for (int v = 0; v < this->size; v++) {
										//We are currently checking the subsquare. First we have to make sure we aren't checking the same row
										//NOTE: We are currently comparing row u and column v
										if (u != z) {
											//We get the cell
											Cell* comparedCellInSubSquare = this->getCellAt(i + u, j + v);
											if (!comparedCellInSubSquare->valueSet() && comparedCellInSubSquare->valueInPossibles(currentValue)) {
												valueInOtherColumns = true;
											}
										}
									}
								}
								//We have now checked all other rows in the subsquare. If it isn't in other rows, we must eliminate it from the rest of the possible in that row in other subsquares
								if (!valueInOtherColumns) {
									int column = i + z;
									for (int u = 0; u < j; u++) {
										Cell* cell = this->getCellAt(column, u);
										if (!cell->valueSet() && cell->valueInPossibles(currentValue)) {
											cell->removeFromPossibleValues(currentValue);
											amountOfLoopsWithoutChange = 0;
										}
									}
									//We divide in 2 fors to skip over the ones in the subsquare
									for (int u = j + this->size; u < this->size * this->size; u++) {
										Cell* cell = this->getCellAt(column, u);
										if (!cell->valueSet() && cell->valueInPossibles(currentValue)) {
											cell->removeFromPossibleValues(currentValue);
											amountOfLoopsWithoutChange = 0;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		if (amountOfLoopsWithoutChange == 2) {
			//We are going to guess a number. To do this we are going to iterate over all possible values
			//and create a new Sudoku from it
			for (int i = 0; i < this->size * this->size; i++) {
				for (int j = 0; j < this->size * this->size; j++) {
					Cell* cell = this->getCellAt(i, j);
					if (!cell->valueSet()) {
						for (int z : cell->getPossibleValues()) {
							Sudoku* sudoku = new Sudoku(this->cells, this->size);
							sudoku->setValueForCell(i, j, z);
							if (sudoku->solvable()) {
								sudoku->solve();
								if (sudoku->solved()) {
									this->cells = sudoku->cells;
									return;
								}
							}
							delete(sudoku);
						}
					}
				}
			}
			return;
		}
	}
}

Cell* Sudoku::getCellAt(int positionX, int positionY) {
	return this->cells.at(positionX + this->size * this->size * positionY);
}

std::vector<Cell*> Sudoku::getRowAt(int positionY) {
	std::vector<Cell*> output;
	for (int i = 0; i < this->size*this->size; i++) {
		output.push_back(this->getCellAt(i, positionY));
	}
	return output;
}

std::vector<Cell*> Sudoku::getColumnAt(int positionX) {
	std::vector<Cell*> output;
	for (int i = 0; i < this->size * this->size; i++) {
		output.push_back(this->getCellAt(positionX, i));
	}
	return output;
}

std::vector<Cell*> Sudoku::getSubSquare(int positionX, int positionY) {
	//We get which sub-square in the columns and rows we have to get
	int subSquareColumn = positionX / this->size;
	int subSquareRow = positionY / this->size;

	std::vector<Cell*> output;
	for (int i = 0; i < this->size; i++) {
		for (int j = 0; j < this->size; j++) {
			output.push_back(this->getCellAt(subSquareColumn * this->size + i, subSquareRow * this->size + j));
		}
	}
	return output;
}

bool Sudoku::solved() {
	bool solved = true;
	for (int i = 0; i < this->size * this->size; i++) {
		for (int j = 0; j < this->size * this->size; j++) {
			if (!this->getCellAt(i, j)->valueSet()) {
				solved = false;
			}
		}
	}
	return solved;
}
	
void Sudoku::setValueForCell(int positionX, int positionY, int value) {
	//We set the value
	this->getCellAt(positionX, positionY)->setValue(value);

	//We have to remove the value of this cell from all the others in the possible vector
	//We remove it from the row
	std::vector<Cell*> row = this->getRowAt(positionY);
	for (Cell* rowCell : row) {
		rowCell->removeFromPossibleValues(value);
	}

	//We remove it from the column
	std::vector<Cell*> column = this->getColumnAt(positionX);
	for (Cell* columnCell : column) {
		columnCell->removeFromPossibleValues(value);
	}

	//We remove it from the subSquare
	std::vector<Cell*> subSquare = this->getSubSquare(positionX, positionY);
	for (Cell* subSquareCell : subSquare) {
		subSquareCell->removeFromPossibleValues(value);
	}
}

bool Sudoku::solvable() {
	bool solvable = true;
	for (int i = 0; i < this->size * this->size; i++) {
		for (int j = 0; j < this->size * this->size; j++) {
			Cell* cell = this->getCellAt(i, j);
			if (!cell->valueSet() && cell->getPossibleValues().size() == 0) {
				solvable = false;
			}
		}
	}
	return solvable;
}