#include <iostream>
#include <string>
#include "commands.hpp"
#include "../gameengine.hpp"

void tester(std::string a, std::string b) {
   std::cout << "EXPECTED: " << a << " || ACTUAL: " << b << std::endl;
   if (a.compare(b) == 0) {
     std::cout << "TEST PASSED." << std::endl << std::endl;
   } 
   else {
     std::cout << "TEST FAILED." << std::endl << std::endl;
   }
}


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

  //std::cout << "TEST: Testing help command" << std::endl;
  //c->help();

  //std::cout << "TEST: Testing alt command" << std::endl;
  //c->alt();

  //std::cout << "TEST: Testing alt command with savegame passed into command" << std::endl;
  //c->alt("savegame");
  
  std::cout << "TEST: Testing look command" << std::endl;
  c->look(game);

  std::cout << "TEST: Testing look command with long specifier" << std::endl;
  c->look(game, "long");

  std::cout << "TEST: Testing go command" << std::endl;
  std::cout << "Test A: Moving up from Entry (using \"go up\")" << std::endl;
  c->go(game, "up");
  tester("UpRoom", game->getCurrentLocation()->getSpaceName());

  std::cout << "Test B: Moving right from UpRoom (using \"go UpRoomConnect\")" << std::endl;
  std::cout << "UpRoomConnect is to the right of UpRoom" << std::endl;
  c->go(game, "uproomconnect");
  tester("UpRoomConnect", game->getCurrentLocation()->getSpaceName());

  std::cout << "Test C: Moving all the way to LeftRoomConnect" << std::endl;
  std::cout << "Sequence is as follows: go left, go down, go LeftRoom, go left" << std::endl;
  c->go(game, "left");
  c->go(game, "down");
  c->go(game, "leftroom");
  c->go(game, "left");
  tester("LeftRoomConnect", game->getCurrentLocation()->getSpaceName());

  std::cout << "Test D: Moving back to Entry to test go right" << std::endl;
  std::cout << "Sequence is as follows: go right, go Entry" << std::endl;
  c->go(game, "right");
  c->go(game, "right");
  tester("Entry", game->getCurrentLocation()->getSpaceName());
 
  return 0;
}
