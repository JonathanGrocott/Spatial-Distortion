#include <iostream>
#include <string>
#include "commands.hpp"

int main() {
  bool valid = false;
  while (!valid) {
    valid = readCommand();
  } 
  return 0;
}
