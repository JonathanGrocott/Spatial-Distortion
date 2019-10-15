#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <cctype>
#include "commands.hpp"

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
  std::ifstream read("alt.txt");
  for (std::string line; std::getline(read, line);) {
    if (line.find(command) != std::string::npos) {
      std::cout << "Alternate wording for " << line << std::endl << std::endl; 
    }
  }    
  read.close();
}

