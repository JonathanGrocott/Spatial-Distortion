/*********************************************************************
** Program Filename: gameengine.cpp
** Author: Jonathan Grocott
** Date: 10/15/19
** Description: Implimentation of gameengine class. 

*********************************************************************/

#include "gameengine.hpp"
#include "space.hpp"
#include "puzzle.hpp"
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
	//initialize command list
	this->commandList = {"help", "go", "look", "look at", "exit", "savegame", "drop",
                        "loadgame", "take", "inventory", "use", "combine", "quit", "solve"};

	//setup default world
	//load map from files in Space directory
	initializeGameMap();

	//set player to starting place
	this->gamePlayer.setCurrentLoc(this->gameMap.at("entry"));
	this->gameState = true;
}

GameEngine::GameEngine(std::string savedGame)
{
	//initialize command list
	this->commandList = {"help", "go", "look", "look at", "exit", "savegame", "drop",
                        "loadgame", "take", "inventory", "use", "combine", "quit", "solve"};

	//setup default world
	initializeGameMap();

	// set roomstates, item locations, player location
	loadGameState(savedGame);
	this->gameState = true;
}

GameEngine::~GameEngine()
{
	// delete pointers
	for (std::unordered_map<std::string,Space*>::iterator it=this->gameMap.begin(); it!=this->gameMap.end(); ++it)
	{
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

	fs::path puzzDir("Data/Puzzles/");

	if (fs::is_directory(puzzDir)) {
		// populate the game map from files in the items directory
		for (auto& entry : boost::make_iterator_range(fs::directory_iterator(puzzDir), {})) {
			//check that entry is a file
			if (fs::is_regular_file(entry)) {
				Puzzle* temp = new Puzzle(entry.path().string(), this->gameMap);
				this->puzzleTracker[temp->getPuzzName()] = std::make_tuple(temp, temp->getPuzzLocation(), nullptr);
			}
		}
	}
	else
		std::cout << "Error in Puzzle Directory" << std::endl;
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
** Description: Helper function to initialize game from a saved state
** Input: string with file name
** Output:
*********************************************************************/
void GameEngine::loadGameState(std::string savedGame){

	std::ifstream File("Data/Saves/"+savedGame);

	if(File){                      //Check if opens ok
		while (File.good ()){
			std::string TempLine;                  //Temp line
		        std::getline (File , TempLine);        //Get temp line
			
			// set player location
			if(!TempLine.compare("<player>")){
				std::getline (File , TempLine);
				boost::algorithm::to_lower(TempLine);
				this->gamePlayer.setCurrentLoc(this->gameMap.at(TempLine));
			}
			
			// set visted rooms
			if(!TempLine.compare("<visted>")){
				while(TempLine.compare("</visted>"))
				{
					std::getline (File , TempLine);
					if(TempLine.compare("</visted>")){
						boost::algorithm::to_lower(TempLine);
						this->gameMap.at(TempLine)->setVisited(true);
					}
				}
			}
			// set item locations
			if(!TempLine.compare("<items>")){
				while(TempLine.compare("</items>"))
				{
					std::getline (File , TempLine);
					if(TempLine.compare("</items>")){
						boost::algorithm::to_lower(TempLine);
						std::vector<std::string> result; 
						boost::split(result, TempLine, boost::is_any_of(","));
						if(result[1] == "player"){
							std::get<2>(this->itemsMap.at(result[0])) =  std::addressof(this->gamePlayer);
							std::get<1>(this->itemsMap.at(result[0])) =  this->gamePlayer.getCurrentLoc();
							std::get<0>(this->itemsMap.at(result[0]))->setTaken(true);
							std::get<0>(this->itemsMap.at(result[0]))->setTakeable(true);
							std::get<0>(this->itemsMap.at(result[0]))->setHidden(false);

						} 
						else
						{
							std::get<2>(this->itemsMap.at(result[0])) = nullptr;
							std::get<1>(this->itemsMap.at(result[0])) = this->gameMap.at(result[1]);
							if(result[2] == "0") 
								std::get<0>(this->itemsMap.at(result[0]))->setHidden(false);
							if(result[3] == "1")
								std::get<0>(this->itemsMap.at(result[0]))->setTakeable(true);
							
						}
						
					}
				}
			}
			// set puzzle completion
			if(!TempLine.compare("<puzzles>")) {
				while(TempLine.compare("</puzzles>"))
				{
					std::getline (File , TempLine);
					if(TempLine.compare("</puzzles>")) {
						boost::algorithm::to_lower(TempLine);
						std::vector<std::string> result;
						boost::split(result, TempLine, boost::is_any_of(","));
						if(result[1] == "true"){
							std::get<2>(this->puzzleTracker.at(result[0])) = std::addressof(this->gamePlayer);
						}
						else {
							std::get<2>(this->puzzleTracker.at(result[0])) = nullptr;
						}
					}
				}
			}
        }
	File.close();
    }
    else{ //Return error
        std::cout << "ERROR cannot find File or does not exist." << std::endl;
    }
}

/*********************************************************************
** Description: Helper function to create a save game
** Input: string with file name
** Output:
*********************************************************************/
void GameEngine::saveGameState(){
	namespace fs = boost::filesystem;
	fs::path saveDir("Data/Saves/");
	//check if the save directory exists if not create it
	if(!fs::exists(saveDir)){
		fs::create_directory(saveDir);
	}

	std::string input;
	std::string confirm;

	do{
		std::cout << "Enter a save name: ";
		std::getline(std::cin, input);
		if(fs::is_regular_file("Data/Saves/"+input+".txt")){
			std::cout << "This save already exits do you want to overwrite it? ";
			std::getline(std::cin, confirm);
		}
	}while(fs::is_regular_file("Data/Saves/"+input+".txt") && confirm != "yes");
	
	std::ofstream file("Data/Saves/"+input+".txt");

	if(file.is_open()){

		// save player location
		file << "<player>" << std::endl;
		file << this->gamePlayer.getCurrentLoc()->getSpaceName() << std::endl;
		file << "</player>" << std::endl;

		// save visted rooms
		file << "<visted>" << std::endl;
		for (std::unordered_map<std::string,Space*>::iterator it=this->gameMap.begin(); it!=this->gameMap.end(); ++it){
			if(it->second->getVisited()){
				file << it->second->getSpaceName() << std::endl;
			}
		}
		file << "</visted>" << std::endl;

		// save item locations
		file << "<items>" << std::endl;
		for (std::unordered_map<std::string, std::tuple<Item*, Space*, player*>>::iterator it=this->itemsMap.begin(); it!=this->itemsMap.end(); ++it){
			if(std::get<2>(it->second) == nullptr){
				file << it->first << "," << std::get<1>(it->second)->getSpaceName() << "," <<
				std::to_string(std::get<0>(it->second)->isHidden()) << "," << 
				std::to_string(std::get<0>(it->second)->isTakeable()) << std::endl;
			}
			else{
				file << it->first << "," << "player,false,true" << std::endl;
			}
		}
		file << "</items>" << std::endl;

		// save puzzle completion
		file << "<puzzles>" << std::endl;
		for (auto it = this->puzzleTracker.begin(); it != this->puzzleTracker.end(); ++it) {
			if(std::get<2>(it->second) == nullptr) {
				file << it->first << ",false" << std::endl;
			}
			else {
				file << it->first << ",true" << std::endl;
			}
		}
		file << "</puzzles>" << std::endl;

		file.close();
	}
	else
	{
		std::cout << "Failed to open file!" << std::endl;
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
		std::cout << "Solvable Puzzles: " << std::endl;
		puzzlesDisp(this->gamePlayer.getCurrentLoc(), this->puzzleTracker);
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
	std::vector<Puzzle*> listPuzzles;
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

	//find objects in the input
	for(auto it = this->itemsMap.begin(); it != this->itemsMap.end(); it++)
	{
		if (std::get<2>(it->second) == nullptr) {
			if(parser(input, it->first)) {
				if (!(std::get<1>(it->second)->getSpaceName().compare(this->gamePlayer.getCurrentLoc()->getSpaceName()))) {
					if (!(std::get<0>(it->second)->isTaken())) { 
						listRoomObjects.push_back(std::get<0>(it->second));
					}
				}
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
	
	//find puzzle names in the input
	for(auto it = this->puzzleTracker.begin(); it != this->puzzleTracker.end(); it++)
	{
		if(parser(input, it->first)) {
			if (std::get<2>(it->second) == nullptr) {
				if (!(std::get<1>(it->second)->getSpaceName().compare(this->gamePlayer.getCurrentLoc()->getSpaceName()))) {
					listPuzzles.push_back(std::get<0>(it->second));
				}
			}
			else {
				std::cout << it->first << " has already been solved!" << std::endl;
			}
		}
	}

		
	//find commands in input
	for(auto it = this->commandList.begin(); it != this->commandList.end(); it++)
	{
		if(parser(input,*it))
			listCommands.push_back(*it);
	}
	

	//handles a location by moving
	if(listLocations.size() == 1){
		go(listLocations[0]);
		return true;
	}

	//handles a single command
	if(listCommands.size() == 1){
		if(listCommands[0]=="quit"){
			//check if player would like to save
			clearScreen();
			std::string input;
			std::cout << "Would you like to save the game: ";
       			std::getline(std::cin, input);
			//set gamestate to false
			if(input == "y" || input == "yes")
			{
				saveGameState();
			}
			this->setGameState(false);
			return true;
		}
		else if(listCommands[0]=="look"){
			uiDisplay(this->gamePlayer.getCurrentLoc());
			return true;
		}
		else if(listCommands[0]=="help"){
			help();
			return true;
		}
		else if(listCommands[0]=="inventory"){
			inventory(itemsMap);
			return true;
		}
		else if(listCommands[0]=="take"){
			if (listRoomObjects.size() > 0) {
				if (listRoomObjects[0]->isTakeable() && !listRoomObjects[0]->isHidden()) {
					this->updateInvent(listRoomObjects[0], &gamePlayer, itemsMap);
					listRoomObjects[0]->setTaken(true);
					std::cout << listRoomObjects[0]->getItemName() << " added to inventory." << std::endl;
					return true;
				}
				else {
					std::cout << listRoomObjects[0]->getItemName() << " is not valid to take." << std::endl;
				}
			}
		}
		else if(listCommands[0]=="drop"){
			if (listInventory.size() > 0) {
				this->updateInvent(listInventory[0], nullptr, itemsMap);
				listInventory[0]->setTaken(false);
				std::cout << listInventory[0]->getItemName() << " dropped." << std::endl;
				return true;	
			}
			else {
				std::cout << listInventory[0]->getItemName() << " is not in your inventory." << std::endl;
			}
		}
		else if(listCommands[0]=="look at") {
			if (listRoomObjects.size() > 0) {
				std::cout << listRoomObjects[0]->getItemDesc() << std::endl;
				for (auto it = itemsMap.begin(); it != itemsMap.end(); it++) {
					if (std::get<0>(it->second)->getTrigger() == listRoomObjects[0]->getItemName())
						std::get<0>(it->second)->setHidden(false);
				}
				return true;
			}
			else if (listInventory.size() > 0) {
				std::cout << listInventory[0]->getItemDesc() << std::endl;
				return true;
			}
			else {
				std::cout << "This object is not valid to look at!" << std::endl;
			}
		}
		else if(listCommands[0]=="solve") {
			if (listPuzzles.size() > 0) {
				std::cout << listPuzzles[0]->getPuzzDesc() << std::endl;
				if (solve(listPuzzles[0]->getPuzzName())) {
					std::cout << listPuzzles[0]->getSuccess() << std::endl;
					updatePuzzMap(listPuzzles[0], &gamePlayer, puzzleTracker);
				}
				else {
					std::cout << listPuzzles[0]->getFail() << std::endl;
				}
				return true;
			}
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
		if (std::get<2>(it->second) != nullptr)
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
** Description: Updates the tuple in puzzleTracker to reflect
** puzzles being solved
** Input: Puzzle*, player*, unordered_map<string, tuple<Item*, Space*, player*>>
** Output: 
*********************************************************************/
void GameEngine::updatePuzzMap(Puzzle* update, player* p, std::unordered_map<std::string, std::tuple<Puzzle*, Space*, player*>> &puzzleTracker)
{
	for (auto it = puzzleTracker.begin(); it != puzzleTracker.end(); it++) {
		if (!it->first.compare(update->getPuzzName()))
			std::get<2>(it->second) = p;
	}
}


/*********************************************************************
** Description: Command parser
** Input: string, string
** Output: 
*********************************************************************/
bool GameEngine::parser(std::string &original, std::string tofind){

	if(tofind.length() <= original.length()) // check that its logical to proceed
	{
		size_t found = original.find(tofind);
		if(found!=std::string::npos)
		{
			// if find and original are same length then they are identical
			if(original.length() == tofind.length())
			{
				original.erase(found,tofind.length());
				boost::algorithm::trim_all(original);
				return true;
			}
			else if (found == 0)
			{
				if(original.at(found+1) == ' '){
					original.erase(found,tofind.length());
					boost::algorithm::trim_all(original);
					return true;
				}
				else
					return false;
			}
			else if ((found-1)>=0 && (found+1)<= (original.length()-1))
			{
				if(original.at(found-1) == ' ' && original.at(found+1 ==' ')){
					original.erase(found,tofind.length());
					boost::algorithm::trim_all(original);
					return true;
				}
				else
					return false;
			}
			else if ((found+tofind.length())<= (original.length()-1))
			{
				if(original.at(found-1) == ' '){
					original.erase(found,tofind.length());
					boost::algorithm::trim_all(original);
					return true;
				}
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}

/*********************************************************************
** Description: Prints the list of main commands
** Input: 
** Output: 
*********************************************************************/

void GameEngine::help() {
  std::ifstream read("Data/Commands/help.txt");
  for (std::string line; std::getline(read, line);) {
    std::cout << line << std::endl;
  }
  std::cout << std::endl;
  read.close();
}

/*********************************************************************
** Description: Moves the player to a room
** Input: string
** Output: 
*********************************************************************/

void GameEngine::go(std::string room) {
  // Check if room name is a valid choice and move player 
  if (this->gamePlayer.getCurrentLoc()->exitMap.count(room) == 1) {
	this->gamePlayer.setCurrentLoc(this->gamePlayer.getCurrentLoc()->exitMap.at(room));
	if(!(this->gamePlayer.getCurrentLoc()->getVisited()))
		this->gamePlayer.getCurrentLoc()->setVisited(true);
	updateItemLoc(&gamePlayer, itemsMap);
  }
  else {
    std::cout << room << " is currently not a valid location. Please try again." << std::endl;
  }
}

/*********************************************************************
** Description: Prints the inventory out to the player
** Input: unordered_map<string, tuple<Item*, Space*, player*>>
** Output: 
*********************************************************************/

void GameEngine::inventory(std::unordered_map<std::string, std::tuple<Item*, Space*, player*>> itemsMap) {
  std::cout << "Inventory" << std::endl;
  std::cout << "----------" << std::endl;
  int count = 0;
  for (auto it = itemsMap.begin(); it != itemsMap.end(); it++) {
    if (std::get<2>(it->second) != nullptr) {
      std::cout << it->first << std::endl;
      count++;
    }
  }
  if (count == 0)
    std::cout << "Your inventory is empty!" << std::endl;
}

/*********************************************************************
** Description: Finds the correct puzzle and checks the answer
** Input: string, string
** Output: bool
*********************************************************************/
bool GameEngine::solve(std::string puzzleName) {
	if (puzzleName == "lockbox")
		return lockboxPuzzle();
	else if (puzzleName == "test tubes") {
		return testTubePuzzle();
	}
}

/*********************************************************************
** Description: Checks the answer for lockbox puzzle
** Input:
** Output: bool
*********************************************************************/
bool GameEngine::lockboxPuzzle() {
	std::string answer;
	std::cout << "What number do you input? ";
	std::getline(std::cin, answer);
	if (answer == "42" || answer == "forty two" || answer == "forty-two") {
		std::get<0>(this->itemsMap.at("power cord"))->setHidden(false);
		std::get<0>(this->itemsMap.at("power cord"))->setTakeable(true);
		return true;
	}
	else { 
		return false;
	}
}

/*********************************************************************
** Description: Checks the answer for test tube puzzle
** Input:
** Output: bool
*********************************************************************/
bool GameEngine::testTubePuzzle() {
	int smallTube = 0;
	const int MAX_SMALL = 5;
	int mediumTube = 0;
	const int MAX_MED = 7;
	int largeTube = 12;
	int diff;
	std::string input;
	while (largeTube != mediumTube) {
		// Display quantities
		std::cout << std::endl;
		std::cout << "The large tube (1) currently has: " << std::to_string(largeTube) << " units" << std::endl;
		std::cout << "The medium tube (2) currently has: " << std::to_string(mediumTube) << " units" << std::endl;
		std::cout << "The small tube (3) currently has: " << std::to_string(smallTube) << " units" << std::endl;
		
		// User input
		std::cout << "Pouring: ";
		std::getline(std::cin, input);
		if (input == "q" || input == "quit" || input == "reset") 
			return false;

		// Parse input
		std::vector<std::string> splitInput;
		boost::split(splitInput, input, boost::is_any_of(" "));
		if (splitInput.size() == 3 && splitInput[1] == "to") {	
			// Pouring into the large tube (tube 1)
			if (splitInput[2] == "1") {
				// Med -> Large
				if (splitInput[0] == "2") {
					largeTube += mediumTube;
					mediumTube = 0;
				}
				// Small -> Large
				else if (splitInput[0] == "3") {
					largeTube += smallTube;
					smallTube = 0;
				}
			else
				std::cout << "That's not a valid pour." << std::endl;
			}
		
			// Pouring into the medium tube (tube 2)
			else if (splitInput[2] == "2") {
				// Large -> Med
				if (splitInput[0] == "1") {
					if (mediumTube + largeTube > MAX_MED) {
						diff = MAX_MED - mediumTube; 
						mediumTube = MAX_MED;
						largeTube -= diff;
					}
					else {
						mediumTube += largeTube;
						largeTube = 0;
					}
				}
				// Small -> Med
				else if (splitInput[0] == "3") {
					if (mediumTube + smallTube > MAX_MED) {
						diff = MAX_MED - mediumTube;
						mediumTube = MAX_MED;
						smallTube -= diff;
					}
					else {
						mediumTube += smallTube;
						smallTube = 0;
					}
				}
				else
					std::cout << "That's not a valid pour." << std::endl;
			}
	
			// Pouring into the small tube (tube 3)
			else if (splitInput[2] == "3") {
				// Large -> Small
				if (splitInput[0] == "1") {
					if (smallTube + largeTube > MAX_SMALL) { 
						diff = MAX_SMALL - smallTube;
						smallTube = MAX_SMALL;
						largeTube -= diff;
					}
					else {
						smallTube += largeTube;
						largeTube = 0;
					}
				}
				// Large -> Med
				else if (splitInput[0] == "2") {
					if (smallTube + mediumTube > MAX_SMALL) {
						diff = MAX_SMALL - smallTube;
						smallTube = MAX_SMALL;
						mediumTube -= diff;
					}
					else {
						smallTube += mediumTube;
						mediumTube = 0;
					}
				}
				else
					std::cout << "That's not a valid pour." << std::endl;
			}
			
			// User entered strange input
			else {
				std::cout << "Please follow the format x to y when deciding your pour." << std::endl;
			}

		}
		else {
			std::cout << "Please follow the format x to y when deciding your pour." << std::endl;
		
		}
	}
	
	// Release the key
	std::get<0>(this->itemsMap.at("brass key"))->setTakeable(true);
	return true;
}

