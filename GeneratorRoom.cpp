/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: Implimentation for the GeneratorRoom, which is derived
from the Space base class. The overriden virtual function from Space class,
locationInteraction(Space* cl), which
specifies the interation the user has with this derived class object.
*********************************************************************/

#include "GeneratorRoom.hpp"
#include<iostream>

using namespace std;

GeneratorRoom::GeneratorRoom()
{
    //ctor
}

GeneratorRoom::~GeneratorRoom()
{
    //dtor
}
/*********************************************************************
** Description: Function specifies interaction with derived class
GeneratorRoom, of base object space.
** Input: Space* cl, Player* pl
** Output:
*********************************************************************/
void GeneratorRoom::locationInteract(Space* cl){


}
