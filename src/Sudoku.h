#pragma once

#include <fstream>
#include <vector>

class Cell;

class Sudoku {
private:
	//Cells of the sudoku
	std::vector<Cell*> cells;
	//Number of columns in one subquare
	int size;

	//Splits string
	std::vector<std::string> splitString(std::string string, std::string splitter);
public:
	Sudoku(std::string file);
	Sudoku(std::vector<Cell*> cells, int size);
	//Prints the current sudoku to terminal
	void print();
	//Solves the Sudoku
	void solve();
	//Gives the cell at that position
	Cell* getCellAt(int positionX, int positionY);
	//Gives the row of the cell
	std::vector<Cell*> getRowAt(int positionY);
	//Gives the row of the cell
	std::vector<Cell*> getColumnAt(int positionX);
	//Gives the sub-square of the cell
	std::vector<Cell*> getSubSquare(int positionX, int positionY);
	//Indicates if it has been solved
	bool solved();
	//Function to put a number in and eliminate its new value from row, column and sub-square
	void setValueForCell(int positionX, int positionY, int value);
	//Indicates if it's still solvable
	bool solvable();
};