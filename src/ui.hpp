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
#include "space.hpp"

void uiDisplay(Space* room);
void exitDisplay(Space* temp);
void objectsDisp(Space* room);
void clearScreen();

#endif
