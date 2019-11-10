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
    //void alt(); // Shows alternate command wording
    //void alt(std::string command); // Shows alternate wording for specified command
    Space* go(Space *currLoc, std::string room); // Moves user to another room
    void look(Space *currLoc); // Gives a long description of the room 

    // Gives a description of an inventory item or room object
    void lookAt(Space*, std::vector<Item*>, std::unordered_map<std::string, Item*>, std::string); 
    void inventory(std::unordered_map<std::string, std::tuple<Item*, Space*, player*>>); // Displays entire inventory
    //std::string take(Space *currLoc, std::string item); // Takes an item from a room
};

#endif
