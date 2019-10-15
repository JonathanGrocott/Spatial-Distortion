#include <iostream>
#include <string>
#include "commands.h"

int main() {
  bool valid = false;
  while (!valid) {
    valid = readCommand();
  } 
  return 0;
}
