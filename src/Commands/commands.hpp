#ifndef COMMANDS_H
#define COMMANDS_H
#include "../space.hpp"

class Commands {
  public:
    Commands(); // Default Constructor
    ~Commands();
    
    void help(); // Shows all commands
    void alt(); // Shows alternate command wording
    void alt(std::string command); // Shows alternate wording for specified command
    std::string getCardinal(std::string command); // Helper function for go to get cardinal alternatives
    Space* go(Space *currLoc, std::string room); // Moves user to another room
    void look(Space *currLoc); // Gives a short description of the room (by default)
    void look(Space *currLoc, std::string specifier); // Gives the short or long description of room
};

#endif
