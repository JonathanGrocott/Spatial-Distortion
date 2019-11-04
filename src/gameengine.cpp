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
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim_all.hpp>

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

	fs::path itemDir("Data/Items/");
	
	if (fs::is_directory(itemDir)) {
		// populate the game map from files in the items directory
		for (auto& entry : boost::make_iterator_range(fs::directory_iterator(itemDir), {})) {
			//check that entry is a file
			if (fs::is_regular_file(entry)) {
				Item* temp = new Item(entry.path().string(), this->gameMap);
				this->itemsMap[temp->getItemName()] = std::make_tuple(temp, temp->getBegLoc(), nullptr) ;
			}
		}
	}
	else
		std::cout << "Error in Item Directory" << std::endl;
}

/*********************************************************************
** Description: Helper function to link exit pointers to the
** constructed room pointers
** Input: string, unordered_map<string, string>
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
		//output to ui
		displayMenu();
		//get and process input
		bool loop =false;
		while (!loop){
			loop = readCommand();
			if (!loop)
				std::cout << "Not a valid command!" << std::endl;
		}
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
** Input:
** Output: 
*********************************************************************/
void GameEngine::displayMenu() {
		std::cout << std::endl << std::endl;
		std::cout << "............................................" << std::endl;
		std::cout << "Current Location: " << this->gamePlayer.getCurrentLoc()->getSpaceName() << std::endl;
		uiDisplay(this->gamePlayer.getCurrentLoc());
		std::cout << "............................................" << std::endl;
		std::cout << "Objects in Room: " << std::endl;
		objectsDisp(this->gamePlayer.getCurrentLoc(), this->itemsMap);                
		std::cout << "............................................" << std::endl;
		std::cout << "Possible Moves: " << std::endl;
		exitDisplay(this->gamePlayer.getCurrentLoc());
		std::cout << "............................................" << std::endl;
}

/*********************************************************************
** Description: Reads a player choice and validates the command.
** If valid it executes the appropriate command.
** Input: 
** Output: returns true if a valid command was given, else false
*********************************************************************/

bool GameEngine::readCommand() {
	
	std::cout << "What next? ";
	std::string input;
	std::getline(std::cin, input);
	// Convert command to lowercase
	boost::algorithm::to_lower(input);
	std::vector<std::string> listLocations;
	std::vector<Item*> listRoomObjects;
	std::vector<Item*> listInventory;
	std::vector<std::string> listCommands;
	
	//find all locations in input
	for(auto it = this->gamePlayer.getCurrentLoc()->exitMap.begin(); it!= this->gamePlayer.getCurrentLoc()->exitMap.end(); it++)
	{
		if(parser(input, it->first))
		{
			//std::cout << it->first << std::endl;
			listLocations.push_back(it->first);
		}
			
	}
	
	//find commands in input
	for(auto it = commands->commandList.begin(); it != commands->commandList.end(); it++)
	{
		if(parser(input,*it))
			listCommands.push_back(*it);
	}
	
	//find objects in the input
	for(auto it = this->itemsMap.begin(); it != this->itemsMap.end(); it++)
	{
		if(parser(input, it->first)) {
			if (!(std::get<0>(it->second)->getBegLoc()->getSpaceName().compare(this->gamePlayer.getCurrentLoc()->getSpaceName()))) {
				if (!(std::get<0>(it->second)->isTaken()))
					listRoomObjects.push_back(std::get<0>(it->second));
			}
		}
	}
	
	//find inventory items in the input
	for(auto it = this->itemsMap.begin(); it != this->itemsMap.end(); it++)
	{
		if(parser(input, it->first)) {
			if (std::get<2>(it->second) != nullptr)
				listInventory.push_back(std::get<0>(it->second));
		}
	}

	//handles a location by moving
	if(listLocations.size() == 1){
		Space* move = this->commands->go(this->gamePlayer.getCurrentLoc(),listLocations[0]);
		if(move){
			this->gamePlayer.setCurrentLoc(move);
			updateItemLoc(&gamePlayer, itemsMap);
			return true;
		}
	}

	//handles a single command
	if(listCommands.size() == 1){
		if(listCommands[0]=="quit"){
			this->setGameState(false);
			return true;
		}
		else if(listCommands[0]=="help"){
			this->commands->help();
			return true;
		}
		else if(listCommands[0]=="inventory"){
			this->commands->inventory(itemsMap);
			return true;
		}
		else if(listCommands[0]=="take") {
			if (listRoomObjects.size() > 0) {
				if (listRoomObjects[0]->isTakeable()) {
					this->updateInvent(listRoomObjects[0], &gamePlayer, itemsMap);
					listRoomObjects[0]->setTaken(true);
					std::cout << listRoomObjects[0]->getItemName() << " added to inventory." << std::endl;
					return true;
				}
				else {
					std::cout << listRoomObjects[0]->getItemName() << " is not valid to take." << std::endl;
				}
			}
			else
				std::cout << "This item is not an object in the room!" << std::endl;
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

/*********************************************************************
** Description: Updates the tuple in itemMaps to reflect
** items moving around the map
** Input: Item*, player*, unordered_map<string, tuple<Item*, Space*, player*>>
** Output: 
*********************************************************************/
void GameEngine::updateItemLoc(player* p, std::unordered_map<std::string, std::tuple<Item*, Space*, player*>> &itemsMap)
{
	for (auto it = itemsMap.begin(); it != itemsMap.end(); it++) {
		if (!(std::get<2>(it->second) != nullptr))
			std::get<1>(it->second) = p->getCurrentLoc();
	}
}


/*********************************************************************
** Description: Updates the tuple in itemMaps to reflect
** items being taken and added to inventory
** Input: Item*, player*, unordered_map<string, tuple<Item*, Space*, player*>>
** Output: 
*********************************************************************/
void GameEngine::updateInvent(Item* update, player* p, std::unordered_map<std::string, std::tuple<Item*, Space*, player*>> &itemsMap)
{
	for (auto it = itemsMap.begin(); it != itemsMap.end(); it++) {
		if (!it->first.compare(update->getItemName()))
			std::get<2>(it->second) = p;
	}
}


/*********************************************************************
** Description: A test function that prints out the names of the 
** Space objects that exit in the gameMap.
** Input: 
** Output: 
*********************************************************************/
bool GameEngine::parser(std::string &original, std::string tofind){

	if(original.find(tofind)!=std::string::npos)
	{
		original.erase(original.find(tofind),tofind.length());
		boost::algorithm::trim_all(original);

		return true;
	}
	else
		return false;
	
}
