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

Space::Space(string n, string st, string ld, string sd, int no, int na, int ne, int nc, bool v)
{
	setSpaceName(n);
	setSpaceType(st);
	setLongDesc(ld);
	setShortDesc(sd);
	setNumObjects(no);
	setNumActions(na);
	setNumExits(ne);
	setNumCharacters(nc);
	setVisited(v);
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


string Space::getSpaceType() {
	return this->spaceType;
}
void Space::setSpaceType(string n) {
	spaceType = n;
}
string Space::getLongDesc() {
	return this->longDesc;
}
void Space::setLongDesc(string n) {
	longDesc = n;
}
string Space::getShortDesc() {
	return this->shortDesc;
}
void Space::setShortDesc(string n) {
	shortDesc = n;
}
bool Space::getVisited() {
	return this->visited;
}
void Space::setVisited(bool b) {
	visited = b;
}
int Space::getNumObjects() {
	return this->numObjects;
}
void Space::setNumObjects(int n) {
	numObjects = n;
}
int Space::getNumActions() {
	return this->numActions;
}
void Space::setNumActions(int n) {
	numActions = n;
}
int Space::getNumExits() {
	return this->numExits;
}
void Space::setNumExits(int n) {
	numExits = n;
}
int Space::getNumCharacters() {
	return this->numCharacters;
}
void Space::setNumCharacters(int n) {
	numCharacters = n;
}

/*********************************************************************
** Description: Base function to set the space information and display
it to the console.
** Input: Space* sp
** Output: iostream
*********************************************************************/
void Space::displaySpaceInfo(Space* sp){
	cout << sp->getSpaceName() << endl << endl;
	cout << sp->longDesc << endl;
}
/*********************************************************************
** Description: Base function for setting each spaces possible
moves, described in 3D as xpositive, xnegative, ypositive, ynegative,
zpositive, and znegative.
** Input: Space* xp, xn, yp, yn, zp, zn.
** Output: none
*********************************************************************/
void Space::setSpaceMoves(Space* fp, Space* lp, Space* rp, Space* bp){
	Forward = fp;
	Left = lp;
	Right = rp;
	Back = bp;
}

/*********************************************************************
** Description: Base function for getting possible moves that player
can make from each location.
** Input: bools as reference
** Output: iostream
*********************************************************************/

void Space::getLocationInfo(bool &fp, bool &lp, bool &rp, bool &bp){

    //possible moves
    if(this->Forward!=NULL){
        fp=true;
    }
    if(this->Left!=NULL){
        lp=true;
    }
    if(this->Right!=NULL){
        rp=true;
    }
    if(this->Back!=NULL){
        bp=true;
    }

}
/*********************************************************************
** Description: Interaction with the space

** Input:
** Output:
*********************************************************************/
void locationInteract(Space* cl) {


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
