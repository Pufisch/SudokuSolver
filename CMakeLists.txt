cmake_minimum_required(VERSION 3.29)
project(Sudoku)

# set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add source files
set(SOURCE_FILES
        src/SudokuMain.cpp
        src/Sudoku.cpp
)

# specify include directories
include_directories(include)

# add source files
add_executable(SudokuSolver ${SOURCE_FILES})
