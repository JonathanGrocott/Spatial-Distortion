#ifndef COMMANDS_H
#define COMMANDS_H

bool readCommand(); // Reads user's command input
void help(); // Shows all commands
void alt(); // Shows alternate command wording
void alt(std::string command); // Shows alternate wording for specified command

#endif
