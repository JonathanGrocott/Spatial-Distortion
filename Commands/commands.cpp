#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <cctype>
#include <boost/algorithm/string.hpp>
#include "commands.hpp"

const char *commandList[11] = {"help", "alt", "go", "look", "exit", "savegame",
                              "loadgame", "take", "inventory", "use", "combine"};

// Constructor to instantiate commands object
Commands::Commands(){}


// Destructor to destroy commands object
Commands::~Commands(){}

/*****************************************************
 * bool readCommand(GameEngine *game)
 * The main command parser that takes user input and
 * selects the appropriate function to execute based
 * on the input. Utilizes overloaded functions to
 * handle varying inputs. Returns true or false
 * depending on if the command is valid or not.
*****************************************************/

bool Commands::readCommand(GameEngine *game) {
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
    if (splitComs.at(0).compare("help") == 0 ||
        splitComs.at(0).compare("guide") == 0 ||
        splitComs.at(0).compare("manual") == 0) {
      help();
    } 
    else if (splitComs.at(0).compare("alt") == 0 ||
             splitComs.at(0).compare("alternate") == 0) {
      alt();
    }
    else if (splitComs.at(0).compare("look") == 0 ||
             splitComs.at(0).compare("examine") == 0) {
      look(game);
    }
    else if (splitComs.at(0).compare("go") == 0 ||
             splitComs.at(0).compare("move") == 0) {
      std::cout << "go/move requires a direction or room name" << std::endl << std::endl;
      return false;
    }
    else if (splitComs.at(0).compare("quit") == 0 ||
             splitComs.at(0).compare("exit") == 0) {
      //exit();
      return true;
    }
    else {
      std::cout << "Invalid command. Type help for a list of commands." << std::endl << std::endl;
      return false;
    }
  }
  
  // If the command was two words, handle the appropriate commands
  else if (splitComs.size() == 2) {
    if (splitComs.at(0).compare("alt") == 0 ||
        splitComs.at(0).compare("alternate") == 0) {
      alt(splitComs.at(1));
    }
    else if (splitComs.at(0).compare("look") == 0 ||
             splitComs.at(0).compare("examine") == 0) {
      look(game, splitComs.at(1));
    }
    else if (splitComs.at(0).compare("go") == 0 ||
             splitComs.at(0).compare("move") == 0) {
      go(game, splitComs.at(1));
    }
    else {
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

void Commands::help() {
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

void Commands::alt() {
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

void Commands::alt(std::string command) {
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
  }
  else {
    std::cout << command << " is not valid. Please try again." << std::endl;
  }      

  read.close();
}


/**************************************************
 * void go(GameEngine *game, string room)
 * Checks for valid rooms and then moves the player
 * to the appropriate room.
**************************************************/

void Commands::go(GameEngine *game, std::string room) {
  // Get valid movements
  bool fp = false, lp = false, rp = false, bp = false; 
  Space *up, *down, *left, *right;
  std::string upRoom, downRoom, leftRoom, rightRoom;
  game->getCurrentLocation()->getLocationInfo(fp, lp, rp, bp);
  if (fp) {
    up = game->getCurrentLocation()->Forward;
    upRoom = boost::algorithm::to_lower_copy(up->getSpaceName());
  }
  if (bp) {
    down = game->getCurrentLocation()->Back;
    downRoom = boost::algorithm::to_lower_copy(down->getSpaceName());
  }
  if (lp) {
    left = game->getCurrentLocation()->Left;
    leftRoom = boost::algorithm::to_lower_copy(left->getSpaceName());
  }
  if (rp) {
    right = game->getCurrentLocation()->Right;
    rightRoom = boost::algorithm::to_lower_copy(right->getSpaceName());
  }

  // Check if user uses a direction instead of a room name
  const char *cardinals[13] = {"up", "down", "left", "right",
                               "north", "south", "west", "east",
                               "forward", "backward", "forwards", "backwards",
                               "back"}; 
  bool isCardinal = false;
  for(const std::string &dir : cardinals) {
    if (room.compare(dir) == 0) {
      isCardinal = true;
    }
  }
 
  // If a direction is indicated, check if room exists and move the player 
  if (isCardinal) {
    if ((room.compare("up") == 0 || room.compare("north") == 0 ||
        room.compare("forward") == 0 || room.compare("forwards") == 0) && fp) {
      game->moveLocation(up);
    } 
    else if ((room.compare("down") == 0 || room.compare("south") == 0 ||
             room.compare("backward") == 0 || room.compare("backwards") == 0 ||
             room.compare("back") == 0) && bp) {
      game->moveLocation(down);
    } 
    else if ((room.compare("left") == 0 || room.compare("west") == 0) && lp) {
      game->moveLocation(left);
    }
    else if ((room.compare("right") == 0 || room.compare("east") == 0) && rp) {
      game->moveLocation(right);
    }
    else {
      std::cout << room << " is currently not a valid direction. Please try again." << std::endl;
    }
  }
 
  // Check if room name is a valid choice and move player 
  else {
    if (room.compare(upRoom) == 0) {
      game->moveLocation(up);
    }
    else if (room.compare(downRoom) == 0) {
      game->moveLocation(down);
    }
    else if (room.compare(leftRoom) == 0) {
      game->moveLocation(left);
    }
    else if (room.compare(rightRoom) == 0) {
      game->moveLocation(right);
    }
    else {
      std::cout << room << " is currently not a valid room name. Please try again." << std::endl;
    }
  }
  
  // Display new room info
  look(game);
}

/***************************************************
 * void look(GameEngine *game)
 * Gives the user the short description of the room
***************************************************/

void Commands::look(GameEngine *game) {
  const std::string spaceName = game->getCurrentLocation()->getSpaceName();
  const std::string shortDesc = game->getCurrentLocation()->getShortDesc();
  std::cout << "Current Location: " << spaceName << std::endl;
  std::cout << shortDesc << std::endl << std::endl;
}

/***************************************************
 * void look(GameEngine *game, string specifier)
 * Used to specify if the user wants the longer
 * description of a room (or short if they want
 * to type more).
***************************************************/

void Commands::look(GameEngine *game, std::string specifier) {
  const std::string spaceName = game->getCurrentLocation()->getSpaceName();
  std::cout << "Current Location: " << spaceName << std::endl;

  if (specifier.compare("long") == 0) {
    const std::string longDesc = game->getCurrentLocation()->getLongDesc();
    std::cout << longDesc << std::endl << std::endl;
  } 
  else if (specifier.compare("short") == 0) {
    const std::string shortDesc = game->getCurrentLocation()->getShortDesc();
    std::cout << shortDesc << std::endl << std::endl;;
  } 
  else {
    //TODO: Add inventory looking
  } 

}
