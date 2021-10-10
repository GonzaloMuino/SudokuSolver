# Sudoku Solver
Simple program to solve Sudokus.

## Dependencies

You'll only need a C++ compiler. Makefile assumes g++ installed.

## Installation
Download this project and in root directory of it use **make** command. This creates the executable SudokuSolver.

## Usage
Simply execute the program in a terminal and give as argument the route to the file containing the sudoku.

## File Formats

3 file formats are supported

- **.sudoku**: Own fileformat of this program.  First line contains the number of columns of a sub-square of the Sudoku, and the following lines are the Sudoku itself. To indicate a vacant spot, -1 is used, and the columns are separated by |. An example is shown in Example.sudoku. Supports any size of standard Sudoku.
- **.sdk**: Based on the **Sadman** file format. Assumes size 3 in file and vacant numbers are indicated with a point. Example in Example.sdk.
- **.sdkstring**: Similar as **.sdk** but is in only one line and vacant numbers are indicated with 0. Example in Example.sdkstring
