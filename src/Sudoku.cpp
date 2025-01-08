// Copyright, Matteo Wittenberg 2024

#include "Sudoku.h"
#include <fstream>
#include <iostream>

bool Cell::operator==(const Cell &other) const { return row_ == other.row_ && col_ == other.col_; }

size_t std::hash<Cell>::operator()(const Cell &coordinate) const noexcept {
    const auto hash1 = std::hash<size_t>{}(coordinate.row_);
    const auto hash2 = std::hash<size_t>{}(coordinate.col_);
    return 2 * hash1 ^ hash2;
}

SudokuSolver::SudokuSolver(const std::string &filename) { read(filename); }

void SudokuSolver::read(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    const std::unordered_set digits{1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            char value;
            file >> value;
            if (const int converted_value = value - '0'; digits.contains(converted_value)) {
                grid_[Cell{row, col}].insert(converted_value);
            } else {
                grid_[Cell{row, col}] = digits;
            }
        }
    }
    file.close();
}

void SudokuSolver::print() const {
    for (int row = 0; row < 9; row++) {
        std::string line;
        for (int col = 0; col < 9; col++) {
            if (const auto &values = grid_.at(Cell{row, col}); values.size() == 1) {
                line += std::to_string(*values.begin());
            } else {
                line += "-";
            }
        }
        std::cout << line << std::endl;
    }
}

bool SudokuSolver::solve() {
    const auto start = std::chrono::high_resolution_clock::now();
    const bool result = tryFrom(grid_);
    const auto end = std::chrono::high_resolution_clock::now();
    duration_ = end - start;
    return result;
}

bool SudokuSolver::tryFrom(Grid &grid) {
    // get next cell with the least amount of candidates
    const std::optional<Cell> cellOpt = getNextCell(grid);
    // check if sudoku is already solved -> every cell has only one candidate
    if (!cellOpt.has_value()) {
        grid_ = std::move(grid);
        return true;
    }
    // try all candidates the cell has
    for (const Cell cell = cellOpt.value(); const auto value : grid.at(cell)) {
        // copy the current grid and reduce the candidates of cell to only the chosen value
        Grid nextGrid = grid;
        nextGrid[cell].clear();
        nextGrid[cell].insert(value);
        // remove the chosen value from all groups cell is in
        // if a cell becomes empty, the chosen value is not a valid candidate -> try the next candidate then
        if (removeCandidate(nextGrid, cell, value)) {
            // recursive call to tryFrom with the new grid
            if (tryFrom(nextGrid)) {
                // return true if one of the candidates has led to a solution
                return true;
            }
        }
    }
    // return false if none of the candidates lead to a solution
    return false;
}

bool SudokuSolver::removeCandidate(Grid &grid, const Cell &coordinate, const int value) {
    // baseRow and baseCol are the coordinates of the cell in the top left corner of the 3x3 square the cell is in
    const int baseRow = coordinate.row_ / 3 * 3;
    const int baseCol = coordinate.col_ / 3 * 3;
    // remove candidate from every cell in the same 3x3 square, except the cell itself
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            // remove candidate from cell, if cell is not the cell itself
            if (!(baseRow + row == coordinate.row_ && baseCol + col == coordinate.col_)) {
                grid.at(Cell{baseRow + row, baseCol + col}).erase(value);
                // return false if a cell becomes empty
                if (grid.at(Cell{baseRow + row, baseCol + col}).empty()) {
                    return false;
                }
            }
        }
    }
    for (int i = 0; i < 9; i++) {
        // remove the candidate from every cell in the same row, except the cell itself
        if (i != coordinate.row_) {
            grid.at(Cell{i, coordinate.col_}).erase(value);
            // return false if a cell becomes empty
            if (grid.at(Cell{i, coordinate.col_}).empty()) {
                return false;
            }
        }
        // remove the candidate from every cell in the same column, except the cell itself
        if (i != coordinate.col_) {
            grid.at(Cell{coordinate.row_, i}).erase(value);
            // return false if a cell becomes empty
            if (grid.at(Cell{coordinate.row_, i}).empty()) {
                return false;
            }
        }
    }
    // return true if no cell has become empty
    return true;
}

std::optional<Cell> SudokuSolver::getNextCell(const Grid &grid) {
    Cell minCell{};
    size_t minSize = std::numeric_limits<size_t>::max();
    bool solved = true;
    for (const auto &[coordinate, candidates] : grid) {
        if (candidates.size() > 1 && candidates.size() < minSize) {
            minSize = candidates.size();
            minCell = coordinate;
        }
        if (candidates.size() != 1) {
            solved = false;
        }
    }
    if (solved) {
        return std::nullopt;
    }
    return minCell;
}

double SudokuSolver::getDuration() const {
    return duration_.count();
}
