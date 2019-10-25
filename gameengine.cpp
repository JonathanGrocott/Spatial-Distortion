/*********************************************************************
** Program Filename: gameengine.cpp
** Author: Jonathan Grocott
** Date: 10/15/19
** Description: Implimentation of gameengine class. 

*********************************************************************/

#include "gameengine.hpp"
#include "space.hpp"
#include "boost/filesystem.hpp"

#include<string>
#include<iostream>
#include <cstdlib>
#include<fstream>
#include<stdio.h>
#include<sstream>
#include<algorithm>
#include <iterator>
#include<direct.h>
#include<Windows.h>


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

    

}

GameEngine::~GameEngine()
{
    //dtor
	//delete all of the pointers
	delete Entry, PrototypeRoom, Washroom, ElectronicsLab, ImagingRoom, PowerRoom, Hallway1, ResearchLab1, ResearchLab2, Office1,
		RoboticsLab, Hallway2, Basement, GeneratorRoom, Deck;
    
}

void GameEngine::setAllPossibleMoves() {
	
	//Set the possible moves from each of the spaces
	Entry->setSpaceMoves(PrototypeRoom, NULL, Hallway1, NULL);
	PrototypeRoom->setSpaceMoves(NULL, NULL, NULL, Entry);
	Washroom->setSpaceMoves(NULL, NULL, NULL, Hallway1);
	ElectronicsLab->setSpaceMoves(NULL, NULL, ImagingRoom, Hallway1);
	//ImagingRoom->setSpaceMoves(NULL, ElectronicsLab, PowerRoom, Basement);
	//PowerRoom->setSpaceMoves(NULL, ImagingRoom, NULL, NULL);
	Hallway1->setSpaceMoves(Washroom, Entry, ResearchLab1, NULL);
	//ResearchLab1->setSpaceMoves(NULL, Hallway1, NULL, ResearchLab2);
	//ResearchLab2->setSpaceMoves(ResearchLab1, Hallway2, Basement, NULL);
	//Office1->setSpaceMoves(NULL, NULL, Hallway2, NULL);
	//RoboticsLab->setSpaceMoves(Hallway2, NULL, NULL, NULL);
	//Hallway2->setSpaceMoves(NULL, Office1, ResearchLab2, RoboticsLab);
	//Basement->setSpaceMoves(ImagingRoom, ResearchLab2, GeneratorRoom, NULL);
	//GeneratorRoom->setSpaceMoves(NULL, Basement, NULL, NULL);
	//Deck->setSpaceMoves(ResearchLab2, RoboticsLab, NULL, NULL);

	currentLocation = Entry; //set starting location
}

int GameEngine::getNumMoves() {
	return this->numMoves;
}

void GameEngine::setNumMoves(int n) {
	numMoves = n; 
}
/*********************************************************************
** Description: Helper function to split string by delimiters
** Input: string st, and string token
** Output: vector<string> 
*********************************************************************/
vector<string> GameEngine::split(string str, string token) {
	
	while (str.size()) {
		int index = str.find(token);
		if (index != string::npos) {
			//result.push_back(str.substr(0, index));
			str = str.substr(index + token.size());
			if (str.size() == 0)result.push_back(str);
		}
		else {
			result.push_back(str);
			//cout << str << endl;
			str = "";
		}
	}
	return result;
}



/*********************************************************************
** Description: Gets list of files in space directory and feeds them
into the getSpaceContents function. 

** Input: string directory path
** Output:
*********************************************************************/
void GameEngine::createSpacesFromFiles(GameEngine* game, string dir) {

	get_file_list(dir);
	for (unsigned int i = 0; i < files.size(); i++) {
		game->getSpaceContents(files.at(i));
	}
	
	
}

/*********************************************************************
** Description: Gets all files in a directory
** Input: string path
** Output: Vector files as reference is changed
*********************************************************************/
void GameEngine::get_file_list(const string& path)
{
	files.clear();

	if (!path.empty())
	{
		namespace fs = boost::filesystem;

		fs::path apk_path(path);
		fs::recursive_directory_iterator end;

		for (fs::recursive_directory_iterator i(apk_path); i != end; ++i)
		{
			const fs::path cp = (*i);
			files.push_back(cp.string());
		}
	}

}

/*********************************************************************
** Description: Gets space info from file. Instatiates the space objects
** fromt the file. 
** Input: string file path
** Output: 
*********************************************************************/
void GameEngine::getSpaceContents(string file){
	
	result.clear();//clear vector contents

	string st;
	int lineNum = 1;
	ifstream infile;
	infile.open(file);

	while (infile) {
		getline(infile, st);
		//cout << "Line " << lineNum << ": " << st << endl;
		split(st, ": ");
		lineNum++;
	}
	
	infile.close();
	
	int visited, numObjects, numActions, numExits, numCharacters;
	string type, name, longDesc, shortDesc, objects, actions, exits, characters, puzzle;

	//now have results vector with all inputs
	for (unsigned int i = 0; i < result.size(); i++) {
		if (i == 0){ //visited
			visited = atoi(result.at(0).c_str());
			//cout << "1: " << visited << endl;
		}
		if (i == 1) { //type
			type = result.at(1);
			//cout << "2: " << type << endl;
		}
		if (i == 2) { //name
			name = result.at(2);
			//cout << "3: " << name << endl;
		}
		if (i == 3) { //long description
			longDesc = result.at(3);
			//cout << "4: " << longDesc << endl;
		}
		if (i == 4) { //short description
			shortDesc = result.at(4);
			//cout << "5: " << shortDesc << endl;
		}
		if (i == 5) { //num objects
			numObjects = atoi(result.at(5).c_str());
			//cout << "6: " << numObjects << endl;
		}
		if (i == 6) { //objects
			objects = result.at(6);
			//cout << "7: " << objects << endl;
		}
		if (i == 7) { //num actions
			numActions = atoi(result.at(7).c_str());
			//cout << "8: " << numActions << endl;
		}
		if (i == 8) { //actions
			actions = result.at(8);
			//cout << "9: " << actions << endl;
		}
		if (i == 9) { //num Exits
			numExits = atoi(result.at(9).c_str());
			//cout << "10: " << numExits << endl;
		}
		if (i == 10) { //Exits
			exits = result.at(10);
			//cout << "11: " << exits << endl;
		}
		if (i == 11) { //num Characters
			numCharacters = atoi(result.at(11).c_str());
			//cout << "12: " << numCharacters << endl;
		}
		if (i == 12) { //Characters
			characters = result.at(12);
			//cout << "13: " << characters << endl;
		}
		if (i == 13) { //Puzzle
			puzzle = result.at(13);
			//cout << "14: " << puzzle << endl;
		}

	}

	if (name == "Entry") {
		Entry = new Space(name, type, longDesc, shortDesc, numObjects, numActions, numExits, exits, numCharacters, visited);
		cout << "Created Entry" << endl;
	}
	if (name == "PrototypeRoom") {
		PrototypeRoom = new Space(name, type, longDesc, shortDesc, numObjects, numActions, numExits, exits, numCharacters, visited);
		cout << "Created PrototypeRoom" << endl;
	}
	if (name == "Washroom") {
		Washroom = new Space(name, type, longDesc, shortDesc, numObjects, numActions, numExits, exits, numCharacters, visited);
		cout << "Created Washroom" << endl;
	}
	if (name == "ElectronicsLab") {
		ElectronicsLab = new Space(name, type, longDesc, shortDesc, numObjects, numActions, numExits, exits, numCharacters, visited);
		cout << "Created ElectronicsLab" << endl;
	}
	if (name == "ImagingRoom") {
		ImagingRoom = new Space(name, type, longDesc, shortDesc, numObjects, numActions, numExits, exits, numCharacters, visited);
		cout << "Created ImagingRoom" << endl;
	}
	if (name == "PowerRoom") {
		PowerRoom = new Space(name, type, longDesc, shortDesc, numObjects, numActions, numExits, exits, numCharacters, visited);
		cout << "Created PowerRoom" << endl;
	}
	if (name == "Hallway1") {
		Hallway1 = new Space(name, type, longDesc, shortDesc, numObjects, numActions, numExits, exits, numCharacters, visited);
		cout << "Created Hallway1" << endl;
	}
	if (name == "Hallway2") {
		Hallway2 = new Space(name, type, longDesc, shortDesc, numObjects, numActions, numExits, exits, numCharacters, visited);
		cout << "Created Hallway2" << endl;
	}
	if (name == "ResearchLab1") {
		ResearchLab1 = new Space(name, type, longDesc, shortDesc, numObjects, numActions, numExits, exits, numCharacters, visited);
		cout << "Created ResearchLab1" << endl;
	}
	if (name == "ResearchLab2") {
		ResearchLab2 = new Space(name, type, longDesc, shortDesc, numObjects, numActions, numExits, exits, numCharacters, visited);
		cout << "Created ResearchLab2" << endl;
	}
	if (name == "Office1") {
		Office1 = new Space(name, type, longDesc, shortDesc, numObjects, numActions, numExits, exits, numCharacters, visited);
		cout << "Created Office1" << endl;
	}
	if (name == "RoboticsLab") {
		RoboticsLab = new Space(name, type, longDesc, shortDesc, numObjects, numActions, numExits, exits, numCharacters, visited);
		cout << "Created RoboticsLab" << endl;
	}
	if (name == "Basement") {
		Basement = new Space(name, type, longDesc, shortDesc, numObjects, numActions, numExits, exits, numCharacters, visited);
		cout << "Created Basement" << endl;
	}
	if (name == "GeneratorRoom") {
		GeneratorRoom = new Space(name, type, longDesc, shortDesc, numObjects, numActions, numExits, exits, numCharacters, visited);
		cout << "Created GeneratorRoom" << endl;
	}
	if (name == "Deck") {
		Deck = new Space(name, type, longDesc, shortDesc, numObjects, numActions, numExits, exits, numCharacters, visited);
		cout << "Created Deck" << endl;
	}



	
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
    cout<<"Current Location: " <<currentLocation->getSpaceName()<<endl; //debug out
}

void GameEngine::moveLocation(Space* temp){
    this->currentLocation = temp;

}
/*********************************************************************
** Description: Main menu driver for game. 

** Input: Space* cl(current location)
** Output: 
*********************************************************************/
void GameEngine::displayMenu(Space* cL) {
	bool quit = false;
	string choice;
	do {
		cout << endl << endl;
		cout << "............................................" << endl;
		cout << "Current Location: ";
		currentLocation->displaySpaceInfo(currentLocation);
		cout << "............................................" << endl;
		cout << "Possible Moves: " << endl;
		currentLocation->getLocationInfo(fp, lp, rp, bp);
		cout << "............................................" << endl;
		cin >> choice;
	} while (!quit);

		//Read Command

	
}
	/*********************************************************************
	** Description: Reads a text file into a ifstream and then returns
	the input as a string.
	** Input: ifstream by reference
	** Output: string of file contents
	*********************************************************************/
	string GameEngine::getFileContents(ifstream& File)
	{
		string Lines = "";        //All lines

		if (File)                      //Check if everything is good
		{
			while (File.good())
			{
				string TempLine;                  //Temp line
				getline(File, TempLine);        //Get temp line
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


