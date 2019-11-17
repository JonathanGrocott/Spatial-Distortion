#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <array>
#include <boost/algorithm/string.hpp>
#include "commands.hpp"

// Constructor to instantiate commands object
Commands::Commands(){
  this->commandList = {"help", "go", "look", "look at", "exit", "savegame", "drop",
                        "loadgame", "take", "inventory", "use", "combine", "quit"};
}


// Destructor to destroy commands object
Commands::~Commands() {
  this->commandList.clear();
}

/***************************************************
 * void help()
 * The basic help command which lists all the main
 * commands by pulling them from a text file.
***************************************************/

void Commands::help() {
  std::ifstream read("Data/Commands/help.txt");
  for (std::string line; std::getline(read, line);) {
    std::cout << line << std::endl;
  }
  std::cout << std::endl;
  read.close();
}

/**************************************************
 * Space* go(Space *currentLocation, string room)
 * Checks for valid rooms and then moves the player
 * to the appropriate room.
**************************************************/

Space* Commands::go(Space *currLoc, std::string room) {
  // Check if room name is a valid choice and move player 
  if (currLoc->exitMap.count(room) == 1) {
    return currLoc->exitMap.at(room);
  }
  else {
    std::cout << room << " is currently not a valid room name. Please try again." << std::endl;
    return nullptr;
  }
}

/******************************************************************************
 * void lookAt(vector<Item*> itemList)
 * Used to look at specific objects in the game
 * as well as inventory items. 
******************************************************************************/

void Commands::lookAt(std::vector<Item*> itemList) {
	std::cout << itemList[0]->getItemDesc() << std::endl;
}

/*********************************************************************
 * void inventory(unordered_map<string, tuple<Item*, Space*, player*>)
 * Prints out the user's entire inventory.
*********************************************************************/

void Commands::inventory(std::unordered_map<std::string, std::tuple<Item*, Space*, player*>> itemsMap) {
  std::cout << "Inventory" << std::endl;
  std::cout << "----------" << std::endl;
  int count = 0;
  for (auto it = itemsMap.begin(); it != itemsMap.end(); it++) {
    if (std::get<2>(it->second) != nullptr) {
      std::cout << it->first << std::endl;
      count++;
    }
  }
  if (count == 0)
    std::cout << "Your inventory is empty!" << std::endl;
}

