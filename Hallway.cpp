/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: Implimentation for the Hallway, which is derived
from the Space base class. The overriden virtual function from Space class,
locationInteraction(Space* cl), which
specifies the interation the user has with this derived class object.
*********************************************************************/

#include "Hallway.hpp"
#include<iostream>

using namespace std;

Hallway::Hallway()
{
    //ctor
}

Hallway::~Hallway()
{
    //dtor
}
/*********************************************************************
** Description: Function specifies interaction with derived class
Hallway, of base object space.
** Input: Space* cl, Player* pl
** Output:
*********************************************************************/
void Hallway::locationInteract(Space* cl){


}
