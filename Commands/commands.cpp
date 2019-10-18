#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <cctype>
#include "commands.hpp"

// For reference and error checking
const char *commandList[11] = {"help", "alt", "go", "look", "exit", "savegame",
                              "loadgame", "take", "inventory", "use", "combine"};


/*****************************************************
 * bool readCommand()
 * The main command parser that takes user input and
 * selects the appropriate function to execute based
 * on the input. Utilizes overloaded functions to
 * handle varying inputs. Returns true or false
 * depending on if the command is valid or not.
*****************************************************/

bool readCommand() {
  // Read in the command
  std::string command;
  std::cout << "What next? ";
  std::getline(std::cin, command);

  // Convert command to lowercase (doesn't handle non-ASCII at the moment)
  for (auto& ch: command) {
    ch = tolower(ch);
  }

  // Split up the command into a vector delimited by spaces
  std::istringstream split(command);
  std::vector<std::string> splitComs(std::istream_iterator<std::string>{split},
                                         std::istream_iterator<std::string>());

  // If no command was entered, return false
  if (splitComs.size() == 0) {
    std::cout << "You didn't enter a command!" << std::endl;
    return false;
  }

  // If the command was only one word, handle the appropriate commands
  if (splitComs.size() == 1) {
    if (splitComs.at(0).compare("help") == 0) {
      help();
      return true;
    } else if (splitComs.at(0).compare("alt") == 0) {
      alt();
      return true;
    } else if (splitComs.at(0).compare("look") == 0) {
      look();
      return true;
    } else if (splitComs.at(0).compare("go") == 0) {
      std::cout << "go requires a direction or room name" << std::endl << std::endl;
      return false;
    } else if (splitComs.at(0).compare("quit") == 0) {
      //exit();
      return true;
    } else {
      std::cout << "Invalid command. Type help for a list of commands." << std::endl << std::endl;
      return false;
    }
  }
  
  // If the command was two words, handle the appropriate commands
  else if (splitComs.size() == 2) {
    if (splitComs.at(0).compare("alt") == 0) {
      alt(splitComs.at(1));
      return true;
    } else if (splitComs.at(0).compare("look") == 0) {
      look(splitComs.at(1));
      return true;
    } else if (splitComs.at(0).compare("go") == 0) {
      alt(splitComs.at(1));
      return true;
    } else {
      std::cout << "Invalid command. Type help for a list of commands." << std::endl << std::endl;
      return false;
    }
  }

  return false;
}

/***************************************************
 * void help()
 * The basic help command which lists all the main
 * commands by pulling them from a text file.
***************************************************/

void help() {
  std::ifstream read("help.txt");
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

void alt() {
  std::ifstream read("alt.txt");
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

void alt(std::string command) {
  bool altFound;
  std::ifstream read("alt.txt");
  
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
  } else {
    std::cout << command << " is not valid. Please try again." << std::endl;
  }      

  read.close();
}


/**************************************************
 * void go(string room)
 * Checks for valid rooms and then moves the player
 * to the appropriate room.
**************************************************/

void go(std::string room) {

// Check for valid rooms

// Update player class to the correct room

// Give description of new room


}

/***************************************************
 * void look()
 * Gives the user the short description of the room
***************************************************/

void look() {

// Read current room description file

// Send short description to stdout

}

/***************************************************
 * void look(specifier)
 * Used to specify if the user wants the longer
 * description of a room (or short if they want
 * to type more).
***************************************************/

void look(std::string specifier) {

// Read current room description file

// Send specified description to stdout
  if (specifier.compare("long")) {
     
  } else {
    
  } 

}
