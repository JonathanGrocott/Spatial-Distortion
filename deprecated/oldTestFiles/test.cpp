#include <iostream>
#include <string>
#include "commands.hpp"
#include "../gameengine.hpp"

int main() {
  bool exit = false;
  Commands *c = new Commands;
  GameEngine *game = new GameEngine;
  
  // Test forward room
  Space *forward = new Space(std::string("UpRoom"), std::string("Test"), 
                             std::string("There is a malodorous smell coming from the vase in this room"),
                             std::string("This room stinks."),
                             1, 2, 1, 0, false);
  Space *forwardConn = new Space(std::string("UpRoomConnect"), std::string("RightConnection"), 
                                 std::string("Long Description"),
                                 std::string("Short Description"),
                                 1, 1, 1, 0, false);
  forward->setSpaceMoves(NULL, NULL, forwardConn, game->Entry);
  forwardConn->setSpaceMoves(NULL, forward, NULL, NULL); 

  // Test left room
  Space *left = new Space(std::string("LeftRoom"), std::string("Test"),
                          std::string("A stump sits in the center of the dimly lit room. There appears to be writing on top."),
                          std::string("There is a stump in this room."),
                          1, 2, 1, 0, false);
  Space *leftConn = new Space(std::string("LeftRoomConnect"), std::string("LeftConnection"), 
                              std::string("Long Description"),
                              std::string("Short Description"),
                              1, 1, 1, 0, false);
  left->setSpaceMoves(NULL, leftConn, game->Entry, NULL);
  leftConn->setSpaceMoves(NULL, NULL, left, NULL);
  
  // Initialize game
  game->Entry->setSpaceMoves(forward, left, NULL, NULL);
  game->setCurrentLocation(game->Entry);

  while (!exit) {
     exit = c->readCommand(game);
  }
 
  return 0;
}
