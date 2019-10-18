#ifndef COMMANDS_H
#define COMMANDS_H

bool readCommand(); // Reads user's command input
void help(); // Shows all commands
void alt(); // Shows alternate command wording
void alt(std::string command); // Shows alternate wording for specified command
void go(std::string room); // Moves user to another room
void look(); // Gives a short description of the room (by default)
void look(std::string specifier); // Gives the short or long description of room
#endif
