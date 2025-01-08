// Copyright, Matteo Wittenberg 2024

#include <iostream>
#include <string>
#include "Sudoku.h"

int main(const int argc, const char **argv) {
    if (argc != 2) {
        std::cout << "Usage: ./SudokuSolver <filename>" << std::endl;
        std::exit(1);
    }
    const std::string filename = argv[1];
    SudokuSolver sudokuSolver(filename);
    sudokuSolver.print();
    std::cout << "\nTrying to solve " << filename << " ..." << std::endl;
    if (sudokuSolver.solve()) {
        std::cout << "Solved " << filename << " in " << sudokuSolver.getDuration() << " ms. Result:\n" << std::endl;
        sudokuSolver.print();
        std::cout << std::endl;
    } else {
        std::cout << "Could not solve Sudoku" << std::endl;
    }
}
