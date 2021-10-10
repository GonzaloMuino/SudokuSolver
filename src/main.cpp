#include "Sudoku.h"
#include <iostream>

int main(int argc, char* argv[]){
	Sudoku* sudoku = new Sudoku(argv[1]);
	sudoku->solve();
	sudoku->print();
}