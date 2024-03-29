/***************************************************************
** Program Filename: player.hpp
** Author: Adam Powell
** Date: 10/27/19
** Description: player class
***************************************************************/

#include "player.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include "ui.hpp"
#include "item.hpp"

/*********************************************************************
** Description: Constructor for a player object
** Input: 
** Output: 
*********************************************************************/
player::player(){
    this->currentLoc = nullptr;
}

/*********************************************************************
** Description: destructor for a player object
** Input: 
** Output: 
*********************************************************************/
player::~player(){
    this->currentLoc = nullptr;
}

/*********************************************************************
** Description: get current player location
** Input: 
** Output: Space pointer
*********************************************************************/
Space* player::getCurrentLoc(){
    return this->currentLoc;
}

/*********************************************************************
** Description: set current player location and update inventory
		items current locations
** Input: Space pointer
** Output: 
*********************************************************************/
void player::setCurrentLoc(Space* loc){
   this->currentLoc = loc;
}