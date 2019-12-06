/***************************************************************
** Program Filename: puzzleFunctions.hpp
** Author: 
** Date: 12/3/2019
** Description: Puzzle function definintions.
***************************************************************/

#ifndef PUZZLEFUNCTIONS_HPP
#define PUZZLEFUNCTIONS_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "boost/filesystem.hpp"
#include "boost/range/iterator_range.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim_all.hpp>

bool lockboxPuzzle(); // Checks answer for lockbox puzzle.
bool testTubePuzzle(); // Checks answer for testtube puzzle.
bool robotPuzzle(); // Checks answer for robot puzzle.
bool powerPuzzle(); // Checks answer for power puzzle.
bool isEqual(std::string [], std::string [], int); // Checks if two arrays are equal at every element
bool validNumber(std::string); // Checks if string is valid number for power puzzle.

#endif
