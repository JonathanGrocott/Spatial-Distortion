/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: Implimentation for the imaging room, which is derived
from the Space base class. The overriden virtual function from Space class,
locationInteraction(Space* cl), which
specifies the interation the user has with this derived class object.
*********************************************************************/

#include "ImagingRoom.hpp"
#include<iostream>

using namespace std;

ImagingRoom::ImagingRoom()
{
    //ctor
}

ImagingRoom::~ImagingRoom()
{
    //dtor
}
/*********************************************************************
** Description: Function specifies interaction with derived class
ImagingRoom, of base object space.
** Input: Space* cl, Player* pl
** Output:
*********************************************************************/
void ImagingRoom::locationInteract(Space* cl){


}
