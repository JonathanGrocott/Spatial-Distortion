/*********************************************************************
** Program Filename: gameengine.cpp
** Author: Jonathan Grocott
** Date: 10/15/19
** Description: Implimentation of gameengine class. 

*********************************************************************/

#include "gameengine.hpp"
#include<string>
#include<iostream>
#include <cstdlib>
#include<fstream>

using namespace std;

/*********************************************************************
** Description: Constructor for GameEngine. Sets up default world.
** Input: 
** Output: 
*********************************************************************/
GameEngine::GameEngine()
{
    //setup default world - Needs to read space and object files
	//and create all of the spaces, objects, etc. 
    
    //Example for adding Spaces
	//researchLab1 = new researchLab1("name");

    //currentLocation = researchLab1;//set starting location

	Entry = new Space("Entry", "Start", "You enter looking down a long dark hallway", "Long dark hallway", 2, 3, 3, 1, true);
	

}

GameEngine::~GameEngine()
{
    //dtor
	//delete all of the pointers
	delete Entry;
    
}
int GameEngine::getNumMoves() {
	return this->numMoves;
}

void GameEngine::setNumMoves(int n) {
	numMoves = n; 
}

/*********************************************************************
** Description: Gets space info from file. 

** Input: string file path
** Output: 
*********************************************************************/
void GameEngine::getSpaceContents(string file){
	
	
}
/*********************************************************************
** Description: Gets object info from file. 

** Input: string file path
** Output: 
*********************************************************************/
void GameEngine::getObjectContents(string file){
	
	
}

Space* GameEngine::getCurrentLocation(){

    //cout<<currentLocation->getSpaceName()<<endl;
    return currentLocation;
}

void GameEngine::setCurrentLocation(Space* cl){
    currentLocation = cl;
    //cout<<currentLocation->getSpaceName()<<endl;
}

void GameEngine::moveLocation(Space* temp){
    this->currentLocation = temp;

}
/*********************************************************************
** Description: Main menu driver for game. 

** Input: Space* cl(current location)
** Output: 
*********************************************************************/
void GameEngine::displayMenu(Space* cL){
	cout << endl << endl;
	cout << "............................................" << endl;
	cout << "Current Location: ";
	currentLocation->displaySpaceInfo(currentLocation);
	cout << "............................................" << endl;
	cout << endl;

	//Read Command

}
/*********************************************************************
** Description: Reads a text file into a ifstream and then returns
the input as a string.
** Input: ifstream by reference
** Output: string of file contents
*********************************************************************/
string GameEngine::getFileContents (ifstream& File)
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

