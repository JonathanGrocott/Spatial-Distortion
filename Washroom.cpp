/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: Implimentation for the prototype room, which is derived
from the Space base class. The overriden virtual function from Space class,
locationInteraction(Space* cl), which
specifies the interation the user has with this derived class object.
*********************************************************************/

#include "Washroom.hpp"
#include<iostream>

using namespace std;

Washroom::Washroom()
{
    //ctor
}

Washroom::~Washroom()
{
    //dtor
}
/*********************************************************************
** Description: Function specifies interaction with derived class
Washroom, of base object space.
** Input: Space* cl, Player* pl
** Output:
*********************************************************************/
void Washroom::locationInteract(Space* cl){


}
