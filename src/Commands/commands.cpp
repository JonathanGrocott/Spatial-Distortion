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
Commands::~Commands(){}

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

/***************************************************
 * void lookAt(Space *currentLocation, string object)
 * Used to look at specific objects in the game
 * as well as inventory items. 
***************************************************/

void Commands::lookAt(Space *currLoc, std::string object) {


}

/***************************************************
 * void inventory(player player)
 * Prints out the user's entire inventory.
***************************************************/

void Commands::inventory(player p) {
  if (p.getInventory().size() != 0) {
    std::cout << "Inventory: " << std::endl;
  for (auto & item : p.getInventory())
    std::cout << item << " ";
  std::cout << std::endl;
  }
  else
    std::cout << "Your inventory is empty!" << std::endl;
}


