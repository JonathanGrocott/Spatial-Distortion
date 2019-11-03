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
** Description: set current player location and update inventory
		items current locations
** Input: Space pointer
** Output: 
*********************************************************************/
void player::setCurrentLoc(Space* loc){
   this->currentLoc = loc;
   for (auto & item : inventory)
	item->setCurrentLoc(loc);	
}

/*********************************************************************
** Description: get current player's inventory 
** Input: 
** Output: vector<string>
*********************************************************************/
std::vector<Item*> player::getInventory(){
   return this->inventory;
}

/*********************************************************************
** Description: adds an item to the player's inventory 
** Input: string
** Output:
*********************************************************************/
void player::addInvent(Item* item){
   this->inventory.push_back(item);
}

/*********************************************************************
** Description: removes an item from the player's inventory
** Input: string
** Output:
*********************************************************************/
void player::removeInvent(Item* item){
   std::vector<Item*>::iterator it = std::find(this->inventory.begin(),
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
void player::useItem(Item* item){
   // Item effect
   
   // Remove if one time use 
   if (item->isBreakable())
   	this->removeInvent(item);
}


