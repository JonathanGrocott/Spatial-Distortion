/***************************************************************
** Program Filename: ui.hpp
** Author: Adam Powell
** Date: 10/27/19
** Description: User Interface function definintions.
***************************************************************/

#ifndef UI_HPP
#define UI_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "space.hpp"
#include "item.hpp"
#include "player.hpp"
#include "puzzle.hpp"

void longDescDisplay(Space* room);
void shortDescDisplay(Space* room);
void exitDisplay(Space* temp);
void objectsDisp(Space* room, std::unordered_map<std::string, std::tuple<Item*, Space*, player*>>);
void puzzlesDisp(Space* room, std::unordered_map<std::string, std::tuple<Puzzle*, Space*, player*>>);
void clearScreen();

#endif
