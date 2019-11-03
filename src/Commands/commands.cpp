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
  this->commandList = {"help", "go", "look", "exit", "savegame",
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
 * void alt()
 * The basic alt function which lists all alternate
 * command wordings for the main commands.
**************************************************/

/*void Commands::alt() {
  std::ifstream read("Data/Commands/alt.txt");
  for (std::string line; std::getline(read, line);) {
    std::cout << line << std::endl;
  }
  std::cout << std::endl;
  read.close();
}*/

/****************************************************
 * void alt(string command)
 * The overloaded alt function which only displays
 * alternate information about the specific command.
****************************************************/

/*void Commands::alt(std::string command) {
  bool altFound;
  std::ifstream read("Data/Commands/alt.txt");
 
  // Checks for valid commands  
  for(const std::string &comm : commandList) {
    if (command.compare(comm) == 0) {
      altFound = true;
    }
  }  

  // Search for the command
  if (altFound) {
    for (std::string line; std::getline(read, line);) {
      if (line.find(command) != std::string::npos) {
        std::cout << "Alternate wording for " << line << std::endl << std::endl;
      }
    }
  }
  else {
    std::cout << command << " is not valid. Please try again." << std::endl;
  }      

  read.close();
}
*/
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


/***************************************************
 * void look(Space *currentLocation)
 * Gives the user the long description of the room
***************************************************/

void Commands::look(Space *currLoc) {

}

/******************************************************************************
 * void lookAt(Space *currentLocation, vector<Item*> inventory,
 * unordered_map<std::string, Item*> mapItems, string object)
 * Used to look at specific objects in the game
 * as well as inventory items. 
******************************************************************************/

void Commands::lookAt(Space *cL, std::vector<Item*> inv, std::unordered_map<std::string, Item*> items, std::string obj) {
	auto it = items.find(obj);
	// Look at room objects
	if (it != items.end()) {
		if (!cL->getSpaceName().compare(it->second->getBegLoc()->getSpaceName())) {
			if (!it->second->isTaken()) {
				std::cout << it->second->getItemDesc() << std::endl;
			}
		}
	}
	// Look at inventory objects
	if (inv.size() != 0) {
		for (auto item : inv) {
			if (!item->getItemName().compare(obj)) {
				std::cout << item->getItemDesc() << std::endl;
			}
		}
	}
	else
		std::cout << obj << " cannot be looked at." << std::endl;

}

/***************************************************
 * void inventory(player player)
 * Prints out the user's entire inventory.
***************************************************/

void Commands::inventory(std::vector<Item*> items) {
  if (items.size() != 0) {
    std::cout << "Inventory" << std::endl;
    std::cout << "----------" << std::endl;
    for (auto & item : items)
      std::cout << item->getItemName() << std::endl;
  }
  else
    std::cout << "Your inventory is empty!" << std::endl;
}


