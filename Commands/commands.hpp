#ifndef COMMANDS_H
#define COMMANDS_H
#include "../gameengine.hpp"

class Commands {
  public:
    Commands(); // Default Constructor
    virtual ~Commands();
    bool readCommand(GameEngine *game); // Reads user's command input
    void help(); // Shows all commands
    void alt(); // Shows alternate command wording
    void alt(std::string command); // Shows alternate wording for specified command
    void go(GameEngine *game, std::string room); // Moves user to another room
    void look(GameEngine *game); // Gives a short description of the room (by default)
    void look(GameEngine *game, std::string specifier); // Gives the short or long description of room
};

#endif
