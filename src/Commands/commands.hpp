#ifndef COMMANDS_H
#define COMMANDS_H
#include "../space.hpp"
#include "../player.hpp"
#include "../item.hpp"
#include <tuple>

class Commands {
  public:
    Commands(); // Default Constructor
    ~Commands();
    
    std::vector<std::string> commandList;
    void help(); // Shows all commands
    Space* go(Space *currLoc, std::string room); // Moves user to another room
    void lookAt(std::vector<Item*>); // Gives description of room or inventory objects 
    void inventory(std::unordered_map<std::string, std::tuple<Item*, Space*, player*>>); // Displays entire inventory
};

#endif
