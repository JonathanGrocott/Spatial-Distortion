/*********************************************************************
** Program Filename: gameengine.cpp
** Author: Jonathan Grocott
** Date: 10/15/19
** Description: Implimentation of gameengine class. 

*********************************************************************/

#include "gameengine.hpp"
#include "space.hpp"
#include "boost/filesystem.hpp"
#include "boost/range/iterator_range.hpp"
#include <boost/algorithm/string.hpp>

#include "ui.hpp"
#include "player.hpp"

#include <string>
#include <utility>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <vector>

/*********************************************************************
** Description: Constructor for GameEngine. Sets up default world.
** Input: 
** Output: 
*********************************************************************/
GameEngine::GameEngine()
{
	//initialize command object
	this->commands = new Commands;

    //setup default world
	//load map from files in Space directory
	initializeGameMap();


	//set player to starting place
	this->gamePlayer.setCurrentLoc(this->gameMap.at("entry"));
	this->gameState = true;
}

GameEngine::~GameEngine()
{
    // delete pointers
	for (std::unordered_map<std::string,Space*>::iterator it=this->gameMap.begin(); it!=this->gameMap.end(); ++it){
		delete it->second;
	}
}

/*********************************************************************
** Description: Helper function to initialize the game map from files
** Input: 
** Output:
*********************************************************************/
void GameEngine::initializeGameMap(){
	
	//setup boost filesystem and directory path
	namespace fs = boost::filesystem;
	fs::path spaceDir("Data/Spaces/");

	if(fs::is_directory(spaceDir)){
		std::vector<std::pair<std::string,std::string>> tempVector;
		std::vector<std::unordered_map<std::string,std::string>> tempAlias;
		//populate the game map from files in the spaces directory
		for(auto& entry : boost::make_iterator_range(fs::directory_iterator(spaceDir), {}))
		{
			//check that entry is a file
			if(fs::is_regular_file(entry))
			{
				//setup room name and path vector
				std::string filename = entry.path().filename().string();
				std::size_t pos = filename.find(".txt");
				filename = filename.substr(0,pos);
				tempVector.push_back(std::make_pair(entry.path().string(),filename));

			}
		}

		//create space objects for each file and save aliases for linking
		for(auto i=tempVector.begin(); i !=tempVector.end(); i++)
		{
			std::unordered_map<std::string,std::string> tempMap;
			Space* temp = new Space(i->first, tempMap);
			this->gameMap[temp->getSpaceName()] = temp;
			tempAlias.push_back(tempMap);
		}

		//linking space exits and aliases
		for(int i=0; i < tempVector.size(); i++)
		{	
			linkExitPtrs(tempVector[i].second,tempAlias[i]);
		}
	}
	else{
		std::cout << "Error in Space Directory" << std::endl;
	}
}

/*********************************************************************
** Description: Helper function to link exit pointers to the contructed
** room pointers
** Input: 
** Output:
*********************************************************************/
void GameEngine::linkExitPtrs(std::string room,std::unordered_map<std::string,std::string> alias){
	// iterate through any aliases and link pointers
	for (auto i = alias.begin(); i != alias.end(); i++) 
	{ 
		this->gameMap.at(room)->linkExitMapPtr(i->first,this->gameMap.at(i->second));
    } 
    
}

/*********************************************************************
** Description: Main game loop
**
** Input: 
** Output:
*********************************************************************/
void GameEngine::mainGameLoop(){
	do{
		displayMenu();
	}while(this->gameState);
}

/*********************************************************************
** Description: Return game state status
** Input: 
** Output: bool
*********************************************************************/
bool GameEngine::getGameState(){
	return this->gameState;
}

/*********************************************************************
** Description: set game state status
** Input: bool
** Output: 
*********************************************************************/
void GameEngine::setGameState(bool b){
	this->gameState = b;
}

/*********************************************************************
** Description: Main menu driver for game. 
**
** Input: Space* cl(current location), Commands* obj (to call commands)
** Output: 
*********************************************************************/
void GameEngine::displayMenu() {
	bool quit = false;
	std::string choice;
	do {
		std::cout << std::endl << std::endl;
		std::cout << "............................................" << std::endl;
		std::cout << "Current Location: " << this->gamePlayer.getCurrentLoc()->getSpaceName();
		uiDisplay(this->gamePlayer.getCurrentLoc());
		std::cout << "............................................" << std::endl;
		std::cout << "Possible Moves: " << std::endl;
		exitDisplay(this->gamePlayer.getCurrentLoc());
		std::cout << "............................................" << std::endl;
        	std::cout << "What next? ";
		std::getline(std::cin, choice);
		quit = readCommand( this->gamePlayer.getCurrentLoc(), choice);
	} while (!quit);
}

/*********************************************************************
** Description: Reads a player choice and validates the command.
** If valid it executes the appropriate command.
**
** Input: GameEngine* game, Space* cL (current location),
** Commands* object, string command
**
** Output: Varies depending on command
*********************************************************************/

bool GameEngine::readCommand(Space* cL, std::string command) {
	// Convert command to lowercase (doesn't handle non-ASCII at the moment)
	boost::algorithm::to_lower(command);
 	
	// Split up the command into a vector delimited by spaces
	std::istringstream split(command);
	std::vector<std::string> splitComs(std::istream_iterator<std::string>{split},
                                 std::istream_iterator<std::string>());

	// If no command was entered, return false
	if (splitComs.size() == 0) {
		std::cout << "You didn't enter a command!" << std::endl;
		return false;
	}

	// If the command was only one word, handle the appropriate commands
	if (splitComs.size() == 1) {
		// If the user only enters a room name, try to move to that room
		std::unordered_map<std::string, Space*>::iterator iter = this->gameMap.find(splitComs.at(0));
		if (iter != this->gameMap.end()) {
			Space* moving = this->commands->go(cL, splitComs.at(0));
			if (moving)
				this->gamePlayer.setCurrentLoc(moving);
		}
		else if (splitComs.at(0).compare("help") == 0 ||
	        	 splitComs.at(0).compare("guide") == 0 ||
		         splitComs.at(0).compare("manual") == 0) {
			this->commands->help();
		} 
		else if (splitComs.at(0).compare("alt") == 0 ||
		         splitComs.at(0).compare("alternate") == 0) {
			this->commands->alt();
		}
		else if (splitComs.at(0).compare("look") == 0 ||
			 splitComs.at(0).compare("examine") == 0) {
			this->commands->look(cL);
		}
		else if (splitComs.at(0).compare("go") == 0 ||
			 splitComs.at(0).compare("move") == 0) {
			std::cout << "go/move requires a direction or room name" << std::endl << std::endl;
			return false;
		}
		else if (splitComs.at(0).compare("quit") == 0 ||
			 splitComs.at(0).compare("exit") == 0) {
			//obj->exit();
			return true;
		}
		else {
			std::cout << "Invalid command. Type help for a list of commands." << std::endl << std::endl;
			return false;
		}
	}
  
	// If the command was two words, handle the appropriate commands
	else if (splitComs.size() == 2) {
		std::string room = splitComs.at(0) + " " + splitComs.at(1);
		std::unordered_map<std::string, Space*>::iterator iter = this->gameMap.find(room);
		if (iter != this->gameMap.end()) {
			Space* moving = this->commands->go(cL, room);
			if (moving)
				this->gamePlayer.setCurrentLoc(moving);
		}
		else if (splitComs.at(0).compare("alt") == 0 ||
		    splitComs.at(0).compare("alternate") == 0) {
			this->commands->alt(splitComs.at(1));
		}
		else if (splitComs.at(0).compare("look") == 0 ||
			 splitComs.at(0).compare("examine") == 0) {
			this->commands->look(cL, splitComs.at(1));
		}
		else if (splitComs.at(0).compare("go") == 0 ||
			 splitComs.at(0).compare("move") == 0) {
			Space* moving = this->commands->go(cL, splitComs.at(1));
                        if (moving)
				this->gamePlayer.setCurrentLoc(moving);
		}
		else {
			std::cout << "Invalid command. Type help for a list of commands." << std::endl << std::endl;
			return false;
		}
	}

	else if (splitComs.size() == 3) {
		std::string room = splitComs.at(0) + " " + splitComs.at(1) + " " + splitComs.at(2);
		std::unordered_map<std::string, Space*>::iterator iter = this->gameMap.find(room);
		if (iter != this->gameMap.end()) {
			Space* moving = this->commands->go(cL, room);
			if (moving)
				this->gamePlayer.setCurrentLoc(moving);
		}

		else if (!splitComs.at(0).compare("go") ||
		    !splitComs.at(0).compare("move")) {
			//TODO: Check for prepositions/articles
			std::string room = splitComs.at(1) + " " + splitComs.at(2);
			Space* moving = this->commands->go(cL, room);
			if (moving)
				this->gamePlayer.setCurrentLoc(moving);
		}	
	}

	else if (splitComs.size() == 4) {
		if (!splitComs.at(0).compare("go") ||
		    !splitComs.at(0).compare("move")) {
			//TODO: Check for prepositions/articles
			std::string room = splitComs.at(1) + " " + splitComs.at(2) + " " +
					   splitComs.at(3);
			Space* moving = this->commands->go(cL, room);
			if (moving)
				this->gamePlayer.setCurrentLoc(moving);
		}	
	}

	return false;
}

/*********************************************************************
** Description: Helper function to split string by delimiters
** Input: string st, and string token
** Output: vector<string> 
*********************************************************************/
std::vector<std::string> GameEngine::split(std::string str, std::string token) {
	
	std::vector<std::string> result;

	while (str.size()) {
		int index = str.find(token);
		if (index != std::string::npos) {
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
** Description: A test function that prints out the names of the 
** Space objects that exit in the gameMap.
** Input: 
** Output: 
*********************************************************************/
void GameEngine::testMap()
{
	std::cout << "game spaces loaded:" << std::endl;
	for (std::unordered_map<std::string,Space*>::iterator it=this->gameMap.begin(); it!=this->gameMap.end(); ++it)
    	std::cout << it->first << std::endl;
}
