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

/*********************************************************************
** Description: Constructor for a player object
** Input: 
** Output: 
*********************************************************************/
player::player(){
    this->currentLoc = nullptr;
    this->inventory = {};
}

/*********************************************************************
** Description: destructor for a player object
** Input: 
** Output: 
*********************************************************************/
player::~player(){
    this->currentLoc = nullptr;
    this->inventory.clear();
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
** Description: set current player location
** Input: Space pointer
** Output: 
*********************************************************************/
void player::setCurrentLoc(Space* loc){
   this->currentLoc = loc;
}

/*********************************************************************
** Description: get current player's inventory 
** Input: 
** Output: vector<string>
*********************************************************************/
std::vector<std::string> player::getInventory(){
   return this->inventory;
}

/*********************************************************************
** Description: adds an item to the player's inventory 
** Input: string
** Output:
*********************************************************************/
void player::addInvent(std::string item){
   this->inventory.push_back(item);
}

/*********************************************************************
** Description: removes an item from the player's inventory
** Input: string
** Output:
*********************************************************************/
void player::removeInvent(std::string item){
   std::vector<std::string>::iterator it = std::find(this->inventory.begin(),
						     this->inventory.end(),
						     item);
   if (it != this->inventory.end()) 
      this->inventory.erase(it);
   //else
      //std::cout << item << " is not in your inventory!" << std::endl;
  
}

/*********************************************************************
** Description: use an item from the inventory 
** Input: string
** Output:
*********************************************************************/
void player::useItem(std::string item){
   // Item effect
   
   // Durability check? (Remove if breakable)
   this->removeInvent(item);
}


