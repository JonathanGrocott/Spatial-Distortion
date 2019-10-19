/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: Implimentation for the Entry Room, which is derived
from the Space base class. The overriden virtual function from Space class,
locationInteraction(Space* cl), which
specifies the interation the user has with this derived class object.
*********************************************************************/
#include "Entry.hpp"
#include<iostream>

using namespace std;

Entry::Entry()
{
    //ctor
}

Entry::~Entry()
{
    //dtor
}
/*********************************************************************
** Description: Function specifies interaction with derived class
entry, of base object space.
** Input: Space* cl, Player* pl
** Output:
*********************************************************************/
void Entry::locationInteract(Space* cl){


}
