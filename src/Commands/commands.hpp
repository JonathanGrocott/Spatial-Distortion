#ifndef COMMANDS_H
#define COMMANDS_H
#include "../space.hpp"
#include "../player.hpp"

class Commands {
  public:
    Commands(); // Default Constructor
    ~Commands();
    
    std::vector<std::string> commandList;
    void help(); // Shows all commands
    //void alt(); // Shows alternate command wording
    //void alt(std::string command); // Shows alternate wording for specified command
    Space* go(Space *currLoc, std::string room); // Moves user to another room
    void look(Space *currLoc); // Gives a long description of the room 
    void lookAt(Space *currLoc, std::string object); // Gives a description of an inventory item or object
    void inventory(player player); // Displays entire inventory
    //std::string take(Space *currLoc, std::string item); // Takes an item from a room
};

#endif
