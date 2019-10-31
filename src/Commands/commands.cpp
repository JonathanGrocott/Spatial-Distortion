#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <array>
#include <boost/algorithm/string.hpp>
#include "commands.hpp"

// List for reference and error checking
const char *commandList[11] = {"help", "alt", "go", "look", "exit", "savegame",
                              "loadgame", "take", "inventory", "use", "combine"};

// Constructor to instantiate commands object
Commands::Commands(){}


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

void Commands::alt() {
  std::ifstream read("Data/Commands/alt.txt");
  for (std::string line; std::getline(read, line);) {
    std::cout << line << std::endl;
  }
  std::cout << std::endl;
  read.close();
}

/****************************************************
 * void alt(string command)
 * The overloaded alt function which only displays
 * alternate information about the specific command.
****************************************************/

void Commands::alt(std::string command) {
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


/**********************************************
 * string getCardinal(string command)
 * Returns the string for cardinal direction
 * to be used by the go function
**********************************************/
std::string Commands::getCardinal(std::string command) {
  std::array<std::string, 4> north = {std::string("north"), "up", "forward", "forwards"};
  std::array<std::string, 2> west = {std::string("west"), "left"};
  std::array<std::string, 5> south = {std::string("south"), "down", "backward", "backwards", "back"};
  std::array<std::string, 2> east = {std::string("east"), "right"};

  for (int i = 0; i < north.size(); i++) {
    if (!command.compare(north[i]))
      return std::string("north");
  }

  for (int i = 0; i < west.size(); i++) {
    if (!command.compare(west[i]))
      return std::string("west");
  }

  for (int i = 0; i < south.size(); i++) {
    if (!command.compare(south[i]))
      return std::string("south");
  }

  for (int i = 0; i < east.size(); i++) {
    if (!command.compare(east[i]))
      return std::string("east");
  }
  
  return std::string("");
}


/**************************************************
 * Space* go(Space *currentLocation, string room)
 * Checks for valid rooms and then moves the player
 * to the appropriate room.
**************************************************/

Space* Commands::go(Space *currLoc, std::string room) {
  // Check for cardinal directions and move there if possible
  std::string card;
  card = getCardinal(room);
  if (card.compare("")) {
    if (!card.compare("north") && currLoc->getNorthExit().compare(""))
      return currLoc->exitMap.at(currLoc->getNorthExit());
    else if (!card.compare("west") && currLoc->getWestExit().compare(""))
      return currLoc->exitMap.at(currLoc->getWestExit());
    else if (!card.compare("south") && currLoc->getSouthExit().compare(""))
      return currLoc->exitMap.at(currLoc->getSouthExit());
    else if (!card.compare("east") && currLoc->getEastExit().compare(""))
      return currLoc->exitMap.at(currLoc->getEastExit());
    else
      std::cout << room << " is currently not a valid direction!" << std::endl;
  }

  // Check if room name is a valid choice and move player 
  else if (currLoc->exitMap.count(room) == 1) {
    return currLoc->exitMap.at(room);
  }
  else {
    std::cout << room << " is currently not a valid room name. Please try again." << std::endl;
    return nullptr;
  }
}


/***************************************************
 * void look(Space *currentLocation)
 * Gives the user the short description of the room
***************************************************/

void Commands::look(Space *currLoc) {
  //const std::string spaceName = currLoc->getSpaceName();
  //const std::string shortDesc = currLoc->getShortDesc();
  //std::cout << "Current Location: " << spaceName << std::endl;
 // std::cout << shortDesc << std::endl << std::endl;
}

/***************************************************
 * void look(Space *currentLocation, string specifier)
 * Used to specify if the user wants the longer
 * description of a room (or short if they want
 * to type more).
***************************************************/

void Commands::look(Space *currLoc, std::string specifier) {
  const std::string spaceName = currLoc->getSpaceName();
  std::cout << "Current Location: " << spaceName << std::endl;

  if (specifier.compare("long") == 0) {
    //const std::string longDesc = currLoc->getLongDesc();
    //std::cout << longDesc << std::endl << std::endl;
  } 
  else if (specifier.compare("short") == 0) {
    //const std::string shortDesc = currLoc->getShortDesc();
    //std::cout << shortDesc << std::endl << std::endl;;
  } 
  else {
    //TODO: Add inventory looking
  } 

}
