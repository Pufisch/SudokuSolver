// Copyright, Matteo Wittenberg 2024

#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <chrono>

// struct to represent 2D-coordinates in the sudoku grid
struct Cell {
    int row_;
    int col_;

    // equality operator
    bool operator==(const Cell &other) const;
};

// specialization of std::hash for type Cell so that it can be used as key in unordered_map
template<> struct std::hash<Cell> {
    size_t operator()(const Cell &coordinate) const noexcept;
};

// type alias for the sudoku grid -> maps a cell to a set of candidates (possible values)
using Grid = std::unordered_map<Cell, std::unordered_set<int>>;

// sudoku solver class
class SudokuSolver {
    // sudoku to be solved
    Grid grid_;
    // amount of time needed to find a solution in ms
    std::chrono::duration<double, std::milli> duration_;

    // function to read in a 9x9 sudoku grid from file
    // stores gid in the grid member variable
    void read(const std::string &filename);

    // recursive function to solve the sudoku grid using backtracking
    // returns true if the sudoku was solved, false otherwise
    // writes result in grid_, if solved
    //
    // chooses the next cell (one with the least candidates) and then tries all these candidates by setting the removing
    // the chosen candidate from every group the cell is in (a group is a row, colum or 3x3 square)
    // if a candidate leads to a contradiction, it goes one decision back (recursive backtracking)
    // and tries the next candidate
    // this is done recursively for all cells and all possible candidates
    // this algorithm has exponential complexity -> using rule-based approach leads to polynomial complexity,
    // but only if the sudoku grid is solvable without guessing
    bool tryFrom(Grid &grid);

    // function that returns the coordinate in the grid, that has the least amount of candidates
    // returns nullopt, when all cells have only one candidate -> sudoku is solved
    static std::optional<Cell> getNextCell(const Grid &grid);

    // function that removes given value from all groups of the given coordinate
    // returns false if one cell becomes empty, true otherwise
    static bool removeCandidate(Grid &grid, const Cell &coordinate, int value);

public:
    // Constructor to initialize the grid from a file
    explicit SudokuSolver(const std::string &filename);
    // function to solve the given grid using backtracking
    bool solve();
    // function to print a 9x9 sudoku grid to the console
    void print() const;
    // returns the time needed to solve the sudoku in ms
    double getDuration() const;
};
