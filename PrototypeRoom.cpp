/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: Implimentation for the prototype room, which is derived
from the Space base class. The overriden virtual function from Space class,
locationInteraction(Space* cl), which
specifies the interation the user has with this derived class object.
*********************************************************************/

#include "PrototypeRoom.hpp"
#include<iostream>

using namespace std;

PrototypeRoom::PrototypeRoom()
{
    //ctor
}

PrototypeRoom::~PrototypeRoom()
{
    //dtor
}
/*********************************************************************
** Description: Function specifies interaction with derived class
PrototypeRoom, of base object space.
** Input: Space* cl, Player* pl
** Output:
*********************************************************************/
void PrototypeRoom::locationInteract(Space* cl){


}
