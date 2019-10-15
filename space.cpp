/*********************************************************************
** Program Filename: space.cpp
** Author: Jonathan Grocott
** Date: 10/15/19
** Description: Implimentation of abstract base class space. Constructor
is abstract and will not be instatiated.
*********************************************************************/

#include "space.hpp"
#include<iostream>
#include<string>
#include<vector>
#include<fstream>

using namespace std;

Space::Space()
{
    //ctor
    //purely virtual abstract base class
    //constructor never called
}

Space::~Space()
{
    //dtor
}

string Space::getSpaceName(){
    return this->spaceName;
}

void Space::setSpaceName(string n){
    spaceName = n;
}

void Space::setSpaceInfo(string sInfo){

}


/*********************************************************************
** Description: Base function to set the space information and display
it to the console.
** Input: Space* sp
** Output: iostream
*********************************************************************/
void Space::displaySpaceInfo(Space* sp){
    //cout<<endl;
    cout<<sp->getSpaceName()<<endl<<endl;

    //cout<<endl;
}
/*********************************************************************
** Description: Base function for setting each spaces possible
moves, described in 3D as xpositive, xnegative, ypositive, ynegative,
zpositive, and znegative.
** Input: Space* xp, xn, yp, yn, zp, zn.
** Output: none
*********************************************************************/
void Space::setSpaceMoves(Space* xp, Space* xn, Space* yp,
                    Space* yn, Space* zp, Space* zn){
    xPositive=xp;
    xNegative=xn;
    yPositive=yp;
    yNegative=yn;
    zPositive=zp;
    zNegative=zn;
}

/*********************************************************************
** Description: Base function for getting possible moves that player
can make from each location.
** Input: bools as reference xp, xn, yp, yn, zp, zn.
** Output: iostream
*********************************************************************/

void Space::getLocationInfo(bool &xp, bool &yp, bool &yn, bool &zp, bool &zn, bool &xn){

    //possible moves
    if(this->xPositive!=NULL){
        xp=true;
    }
    if(this->yPositive!=NULL){
        yp=true;
    }
    if(this->yNegative!=NULL){
        yn=true;
    }
    if(this->zPositive!=NULL){
        zp=true;
    }
    if(this->zNegative!=NULL){
        zn=true;
    }
    if(this->xNegative!=NULL){
        xn=true;
    }

}
/*********************************************************************
** Description: Reads a text file into a ifstream and then returns
the input as a string.
** Input: ifstream by reference
** Output: string of file contents
*********************************************************************/
string Space::getFileContents (ifstream& File)
{
    string Lines = "";        //All lines

    if (File)                      //Check if everything is good
    {
	while (File.good ())
	{
	    string TempLine;                  //Temp line
	    getline (File , TempLine);        //Get temp line
	    TempLine += "\n";                      //Add newline character

	    Lines += TempLine;                     //Add newline
	}
	return Lines;
    }
    else                           //Return error
    {
	return "ERROR File does not exist.";
    }
}
