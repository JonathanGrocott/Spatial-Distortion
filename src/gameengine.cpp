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
#include "puzzleFunctions.hpp"

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
                        "loadgame", "take", "inventory", "use", "combine", "quit", "solve",
			"clear", "teleport", "look at map", "fill", "pour"};

	//sort commands by size
	std::sort(this->commandList.begin(), this->commandList.end(), [](const std::string &s1, const std::string &s2) {return s1.size() > s2.size(); });
	//setup default world
	//load map from files in Space directory
	initializeGameMap();

	//set player to starting place
	this->gamePlayer.setCurrentLoc(this->gameMap.at("entry"));
	this->gameState = true;
	this->paradox = false;
	this->generatorSwitch = false;
}

GameEngine::GameEngine(std::string savedGame)
{
	//initialize command list
	this->commandList = {"help", "go", "look", "look at", "exit", "savegame", "drop",
                        "loadgame", "take", "inventory", "use", "combine", "quit", "solve",
			"clear", "teleport", "look at map", "fill", "pour"};

	//sort commands by size
	std::sort(this->commandList.begin(), this->commandList.end(), [](const std::string &s1, const std::string &s2) {return s1.size() > s2.size(); });
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
			// set switch
			if(!TempLine.compare("<switch>")){
				std::getline (File , TempLine);
				boost::algorithm::to_lower(TempLine);
				if(TempLine == "true")
					this->generatorSwitch = true;
				else
					this->generatorSwitch = false;
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
							else
								std::get<0>(this->itemsMap.at(result[0]))->setHidden(true);
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

		// power switch state 
		file << "<switch>" << std::endl;
		if(this->generatorSwitch)
			file << "true" << std::endl;
		else
			file << "false" << std::endl;
		file << "</switch>" << std::endl;

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
			//[itemName], player, isHidden, isTakeable
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
	//start game by displaying location
	displayMenu(true);
	this->gameMap.at("entry")->setVisited(true);
	//start game loop
	do{
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
** Input: none
** Output: console
*********************************************************************/
void GameEngine::displayMenu(bool clear) {
	if (clear)
		clearScreen();
	std::cout << std::endl << std::endl;
	//display ascii art if matches space name
	displayASCII(this->gamePlayer.getCurrentLoc()->getSpaceName());

	std::cout << std::string(75, '.') << std::endl;
	std::cout << "Current Location: " << this->gamePlayer.getCurrentLoc()->getSpaceName() << std::endl;
	if(!this->gamePlayer.getCurrentLoc()->getVisited())
		longDescDisplay(this->gamePlayer.getCurrentLoc());
	else
		shortDescDisplay(this->gamePlayer.getCurrentLoc());
	displayObjects();
	std::cout << "Solvable Puzzles: " << std::endl;
	puzzlesDisp(this->gamePlayer.getCurrentLoc(), this->puzzleTracker);
	std::cout << std::string(75, '.') << std::endl;
}
/*********************************************************************
** Description: Used to display ascii art that matches input name
** Input: string name
** Output: asciii art to console
*********************************************************************/
void GameEngine::displayASCII(std::string name) {
	std::string temp = "Data/Art/" + name + ".txt";
	std::ifstream Reader(temp);
	if (Reader) {
		std::string Art = getFileContents(Reader);
		std::cout << Art << std::endl;
	}
	Reader.close();
}

/*********************************************************************
** Description: Reads a text file into a ifstream and then returns
the input as a string. Used for ascii picture display to console.
** Input: ifstream by reference
** Output: string of file contents
*********************************************************************/
std::string GameEngine::getFileContents(std::ifstream& File)
{
	std::string Lines = "";        //All lines

	if (File) {
		while (File.good()) {
			std::string TempLine;            //Temp line
			std::getline(File, TempLine);
			TempLine += "\n";
			Lines += TempLine;
		}
		return Lines;
	}
	else { //Return error
		return "ERROR File does not exist.";
	}
}

/*********************************************************************
** Description: Used to display objects in a room. 
** Input:
** Output: 
*********************************************************************/

void GameEngine::displayObjects() {
	std::cout << std::string(75, '.') << std::endl;
	std::cout << "Objects in Room: " << std::endl;
	objectsDisp(this->gamePlayer.getCurrentLoc(), this->itemsMap);                
	std::cout << std::string(75, '.') << std::endl;

}

/*********************************************************************
** Description: Reads a player choice and validates the command.
** If valid it executes the appropriate command.
** Input: 
** Output: returns true if a valid command was given, else false
*********************************************************************/

bool GameEngine::readCommand() {
	
	// get player input
	std::cout << "What next? ";
	std::string input = playerInput();
	std::vector<std::string> listCommands;
	//find all locations in input
	if(input.size()>1)
	{
		//find commands in input
		for(auto it = this->commandList.begin(); it != this->commandList.end(); it++)
		{
			if(parser(input,*it))
				listCommands.push_back(*it);
		}
		//handles a single command
		if(!listCommands.empty()){
			if(listCommands.size() == 1){
				if(listCommands[0]=="quit" || listCommands[0]=="exit"){
					quit();
					return true;
				}
				else if(listCommands[0]=="look"){
					look();
					return true;
				}
				else if(listCommands[0]=="help"){
					help();
					std::cout << "Hit enter when finished reading " << std::endl;
					std::string input;
					std::getline(std::cin, input);
					displayMenu(true);
					return true;
				}
				else if(listCommands[0]=="inventory"){
					inventory();
					return true;
				}
				else if(listCommands[0]=="take"){
					std::vector<Item*> listRoomObjects;
					roomItemParser(listRoomObjects, input);
					if (listRoomObjects.size() > 0) {
						if (listRoomObjects[0]->isTakeable() && !listRoomObjects[0]->isHidden()) {
							take(listRoomObjects[0]);
							return true;
						}
						else {
							std::cout << listRoomObjects[0]->getItemName() << " is not valid to take." << std::endl;
						}
					}
				}
				else if(listCommands[0]=="drop"){
					std::vector<Item*> listInventory;
					inventoryParser(listInventory,input);
					if (listInventory.size() > 0) {
						drop(listInventory[0]);
						displayASCII(listInventory[0]->getItemName());//display ascii
						displayObjects();
						return true;	
					}
					else {
						std::cout << listInventory[0]->getItemName() << " is not in your inventory." << std::endl;
					}
				}
				else if(listCommands[0]=="look at") {
					std::vector<Item*> listRoomObjects;
					roomItemParser(listRoomObjects, input);
					std::vector<Item*> listInventory;
					inventoryParser(listInventory,input);
					if (listRoomObjects.size() > 0) {
						displayASCII(listRoomObjects[0]->getItemName()); //display ascii

						std::cout << listRoomObjects[0]->getItemDesc() << std::endl;
						for (auto it = this->itemsMap.begin(); it != this->itemsMap.end(); it++) {
							if (std::get<0>(it->second)->getTrigger() == listRoomObjects[0]->getItemName()) {
								std::get<0>(it->second)->setHidden(false);
								displayObjects();
							}
						}
						std::string temp;
						std::cout << "Hit enter when done looking...";
						std::getline(std::cin, temp);
						displayMenu(false);
						return true;
					}
					else if (listInventory.size() > 0) {
						displayASCII(listInventory[0]->getItemName());	
						std::cout << listInventory[0]->getItemDesc() << std::endl;
						std::string temp;
						std::cout << "Hit enter when done looking...";
						std::getline(std::cin, temp);
						displayMenu(false);
						return true;
					}
					else {
						std::cout << "This object is not valid to look at!" << std::endl;
					}
				}
				else if(listCommands[0]=="solve") {
					std::vector<Puzzle*> listPuzzles;
					puzzleParser(listPuzzles,input);
					if (listPuzzles.size() > 0 && !listPuzzles[0]->isHidden()) {
						clearScreen();
						std::cout << listPuzzles[0]->getPuzzDesc() << std::endl;
						if (solve(listPuzzles[0]->getPuzzName())) {
							displayASCII(listPuzzles[0]->getPuzzName()); //display ascii
							std::cout << listPuzzles[0]->getSuccess() << std::endl;
							updatePuzzMap(listPuzzles[0]);
							displayMenu(false);
						}
						else {
							std::cout << listPuzzles[0]->getFail() << std::endl;
						}
						return true;
					}
				}
				else if(listCommands[0]=="teleport") {
					std::vector<std::string> listLocations;
					locationParser(listLocations,input);
					if(listLocations.size() == 1){
						teleport(listLocations[0]);
						return true;
					}
					else
					std::cout << "You must provide a room name to teleport to." << std::endl;
					return false;
				}
				else if(listCommands[0]=="go") {
					std::vector<std::string> listLocations;
					locationParser(listLocations,input);
					if(listLocations.size() == 1){
						go(listLocations[0]);
						return true;
					}
					else
					return false;
				}
				else if(listCommands[0]=="look at map"){
					playerMap();
					return true;
				}
				else if(listCommands[0]=="use") {
					return use(input);
				}
				else if(listCommands[0]=="savegame"){
					saveGameState();
					return true;
				}
				else if(listCommands[0]=="loadgame"){
					loadGameStateMenu();
					return true;
				}
				else if(listCommands[0]=="combine") {
					return combine(input);
				}
				else if(listCommands[0]=="fill") {
					return fill(input);
				}
				else if(listCommands[0]=="pour") {
					return pour(input);
				}
				
			}
			else {
				std::cout << "Multiple command keywords were given! Please try again." << std::endl;
			}
		}
		std::vector<std::string> listLocations;
		locationParser(listLocations,input);
		if(!listLocations.empty()){
			if(listLocations.size() == 1 && listCommands.size() == 0 && input.length() == 0){
				go(listLocations[0]);
				return true;
			}
		}
	}
	else
		return false;

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
** Description: Updates the tuple in itemMaps to reflect
** items moving around the map
** Input: 
** Output: 
*********************************************************************/
void GameEngine::updateItemLoc()
{
	for (auto it = this->itemsMap.begin(); it != this->itemsMap.end(); it++) {
		if (std::get<2>(it->second) != nullptr)
			std::get<1>(it->second) = this->gamePlayer.getCurrentLoc();
	}
}


/*********************************************************************
** Description: Updates the tuple in itemMaps to reflect
** items being taken and added to inventory
** Input: Item*
** Output: 
*********************************************************************/
void GameEngine::updateInvent(Item* update, player* p)
{
	for (auto it = this->itemsMap.begin(); it != this->itemsMap.end(); it++) {
		if (!it->first.compare(update->getItemName()))
			std::get<2>(it->second) = p;
	}
}

/*********************************************************************
** Description: Updates the tuple in puzzleTracker to reflect
** puzzles being solved
** Input: Puzzle*
** Output: 
*********************************************************************/
void GameEngine::updatePuzzMap(Puzzle* update)
{
	for (auto it = this->puzzleTracker.begin(); it != this->puzzleTracker.end(); it++) {
		if (!it->first.compare(update->getPuzzName()))
			std::get<2>(it->second) = &(this->gamePlayer);
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
			// look for space after tofind
			else if (found == 0)
			{
				if(original.at(found+tofind.length()) == ' '){
					original.erase(found,tofind.length());
					boost::algorithm::trim_all(original);
					return true;
				}
				else
					return false;
			}
			// look for space on both sides of tofind
			else if ((found-1)>=0 && (found+tofind.length())<= (original.length()))
			{
				if(original.at(found-1) == ' ' && original.at(found+tofind.length() ==' ')){
					original.erase(found,tofind.length());
					boost::algorithm::trim_all(original);
					return true;
				}
				else
					return false;
			}
			// look for space before tofind
			else if ((found+tofind.length())<= (original.length()))
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
** Input: stringlook
** Output: 
*********************************************************************/

void GameEngine::go(std::string room) {
  // Check if room name is a valid choice and move player 
  if (this->gamePlayer.getCurrentLoc()->exitMap.count(room) == 1) {
	// check if player has required item to access room
	if(this->gamePlayer.getCurrentLoc()->exitMap.at(room)->getbReqItem()){
		std::vector<Item*> listInventory;
		std::string temp = this->gamePlayer.getCurrentLoc()->exitMap.at(room)->getReqItem();
		inventoryParser(listInventory,temp);
		
		if(this->gamePlayer.getCurrentLoc()->exitMap.at(room)->getReqItem() == "flashlight"){
			if(listInventory.size()>0){
				this->gamePlayer.setCurrentLoc(this->gamePlayer.getCurrentLoc()->exitMap.at(room));
				displayMenu(true);
				if(!(this->gamePlayer.getCurrentLoc()->getVisited()))
					this->gamePlayer.getCurrentLoc()->setVisited(true);
				this->updateItemLoc();
			}
			else{
				std::cout << "The " << room << " is too dark to see in, you cannot go there yet!" << std::endl;
			}

		}
	}
	else{
		this->gamePlayer.setCurrentLoc(this->gamePlayer.getCurrentLoc()->exitMap.at(room));
		displayMenu(true);
		if(!(this->gamePlayer.getCurrentLoc()->getVisited()))
			this->gamePlayer.getCurrentLoc()->setVisited(true);
		this->updateItemLoc();
	}
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

void GameEngine::inventory() {
  const int DASHES = 12;
  std::cout << std::string(DASHES, '-') << std::endl;
  std::cout << "|Inventory|" << std::endl;
  std::cout << std::string(DASHES, '-') << std::endl;
  int count = 0;
  for (auto it = this->itemsMap.begin(); it != this->itemsMap.end(); it++) {
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
** Input: string
** Output: bool
*********************************************************************/
bool GameEngine::solve(std::string puzzleName) {
	if (puzzleName == "lockbox"){
		if(lockboxPuzzle()){
			std::get<0>(this->itemsMap.at("power cord"))->setHidden(false);
			std::get<0>(this->itemsMap.at("power cord"))->setTakeable(true);
			return true;
		}
		else
			return false;
	}
	else if (puzzleName == "test tubes") {
		if(testTubePuzzle()){
			// Release the key
			std::get<0>(this->itemsMap.at("brass key"))->setTakeable(true);
			return true;
		}
		else
			return false;
	}
	else if (puzzleName == "robot") {
		if(robotPuzzle()){
			std::get<0>(this->itemsMap.at("flashlight"))->setTakeable(true);	
			return true;
		}
		else
			return false;
	}
	else if (puzzleName == "power draw") {
		return powerPuzzle();
	}
}



/*********************************************************************
** Description: Quits the game and prompts user for save
** Input:
** Output:
*********************************************************************/
void GameEngine::quit(){
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
}

/*********************************************************************
** Description: Gives the long form of the room again
** Input:
** Output:
*********************************************************************/
void GameEngine::look(){
		clearScreen();
		std::cout << std::endl << std::endl;
		displayASCII(this->gamePlayer.getCurrentLoc()->getSpaceName());
		std::cout << std::string(75, '.') << std::endl;
		std::cout << "Current Location: " << this->gamePlayer.getCurrentLoc()->getSpaceName() << std::endl;
		longDescDisplay(this->gamePlayer.getCurrentLoc());
		std::cout << std::string(75, '.') << std::endl;
		std::cout << "Objects in Room: " << std::endl;
		objectsDisp(this->gamePlayer.getCurrentLoc(), this->itemsMap);                
		std::cout << std::string(75, '.') << std::endl;
		std::cout << "Solvable Puzzles: " << std::endl;
		puzzlesDisp(this->gamePlayer.getCurrentLoc(), this->puzzleTracker);
		std::cout << std::string(75, '.') << std::endl;
}


/*********************************************************************
** Description: Drops items in rooms
** Input:
** Output:
*********************************************************************/
void GameEngine::drop(Item* droppedItem) {
	this->updateInvent(droppedItem, nullptr);
	droppedItem->setTaken(false);
	std::cout << droppedItem->getItemName() << " dropped." << std::endl;

}

/*********************************************************************
** Description: Takes items in rooms
** Input:
** Output:
*********************************************************************/
void GameEngine::take(Item* takenItem) {
	this->updateInvent(takenItem, &(this->gamePlayer));
	takenItem->setTaken(true);
	displayASCII(takenItem->getItemName());
	std::cout << takenItem->getItemName() << " added to inventory." << std::endl;
	displayMenu(false);

}

/*********************************************************************
** Description: Teleport to any previously visited room
** Input:
** Output:
*********************************************************************/
void GameEngine::teleport(std::string room) {
  if (this->gameMap.count(room) == 1) {
	if(this->gameMap.at(room)->getVisited())
	{
		this->gamePlayer.setCurrentLoc(this->gameMap.at(room));
		this->updateItemLoc();
		displayMenu(true);
	}
	else
		std::cout << room << " is currently not a valid teleport location." << std::endl;
  }
  else {
    std::cout << room << " is currently not a valid teleport location." << std::endl;
  }

}
/*********************************************************************
** Description: List out rooms a player has been in
** Input:
** Output:
*********************************************************************/
void GameEngine::playerMap() {
	std::cout << std::endl << "Visited Rooms:" << std::endl;
	std::cout << "--------------" << std::endl;
	for(auto it = this->gameMap.begin(); it!= this->gameMap.end(); it++)
	{
		if(it->second->getVisited())
			std::cout << it->second->getSpaceName() << std::endl;
	}
}

/*********************************************************************
** Description: Prompt player for input
** Input: 
** Output: returns player input in lower case
*********************************************************************/
std::string GameEngine::playerInput() {
	std::string input;
	std::getline(std::cin, input);
	// Convert command to lowercase
	boost::algorithm::to_lower(input);

	return input;
}

/*********************************************************************
** Description: Helper function for parsing Items in Inventory
** Input: reference to array of item*
** Output: 
*********************************************************************/
void GameEngine::inventoryParser(std::vector<Item*>& listInventory, std::string& input){
		//find inventory items in the input
		for(auto it = this->itemsMap.begin(); it != this->itemsMap.end(); it++)
		{
			if (std::get<2>(it->second) != nullptr) {
				if(parser(input, it->first))
					listInventory.push_back(std::get<0>(it->second));
			}
		}
}
/*********************************************************************
** Description: Helper function for parsing Items in Room
** Input: reference to array of item*
** Output: 
*********************************************************************/
void GameEngine::roomItemParser(std::vector<Item*>& listRoomObjects, std::string& input){
			//find objects in the room from input
		for(auto it = this->itemsMap.begin(); it != this->itemsMap.end(); it++)
		{
			if (std::get<2>(it->second) == nullptr && !std::get<0>(it->second)->isHidden()) {
				if(parser(input, it->first)) {
					if (!(std::get<1>(it->second)->getSpaceName().compare(this->gamePlayer.getCurrentLoc()->getSpaceName()))) {
						if (!(std::get<0>(it->second)->isTaken())) { 
							listRoomObjects.push_back(std::get<0>(it->second));
						}
					}
				}
			}
		}

}
/*********************************************************************
** Description: Helper function for parsing locations
** Input: reference to array of string
** Output: 
*********************************************************************/
void GameEngine::locationParser(std::vector<std::string>& listLocations, std::string& input){

		for(auto it = this->gamePlayer.getCurrentLoc()->exitMap.begin(); it!= this->gamePlayer.getCurrentLoc()->exitMap.end(); it++)
		{
			if(parser(input, it->first))
			{
				//std::cout << it->first << std::endl;
				listLocations.push_back(it->first);
			}
		}

		for(auto it = this->gameMap.begin(); it!= this->gameMap.end(); it++)
		{
			if(parser(input, it->first))
			{
				//std::cout << it->first << std::endl;
				listLocations.push_back(it->first);
			}
		}

}
/*********************************************************************
** Description: Helper function for parsing puzzles
** Input: reference to array of puzzle*
** Output: 
*********************************************************************/
void GameEngine::puzzleParser(std::vector<Puzzle*>& listPuzzles, std::string& input){
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
}

/*********************************************************************
** Description: Check win condition and end game
** Input: 
** Output: 
*********************************************************************/
void GameEngine::wincon(){

	if(this->paradox)
	{
		this->gameState = false;
		clearScreen();
		std::cout << "Congratulations you have stopped HAL 9001 using an infinite energy paradox," << std::endl;
		std::cout << "saving yourself from living the rest of your life in this tiny building." <<std::endl<<std::endl;

		std::string temp;
		std::cout << "Press Enter to return to main menu... ";
		std::getline(std::cin, temp);
	}

}

/*********************************************************************
** Description: Check win condition and end game
** Input: 
** Output: 
*********************************************************************/
void GameEngine::infin(){
	clearScreen();
	printFile("Data/infin/HAL.txt");

	std::string input;
	while (input != "quit") 
	{
		std::cout << "HAL 9001:~/$ ";
		input = playerInput();

		if(input.find("loop")!=std::string::npos)
		{
			if(input.find("generator room")!=std::string::npos)
			{
				if(this->generatorSwitch)
				{
					this->paradox = true;
					wincon();
					break;
				}
				else
				{
					std::cout << "Please don't loop this room, it's not very useful at all." << std::endl;
				}
				
			}
			else
			{
				std::cout << "Enjoy using that loop command over and over and over." << std::endl;
			}
			
		}
		else if(input == "map"){
			std::cout << std::endl << "Here are the rooms you know about:" << std::endl;
			std::cout << "--------------" << std::endl;
			for(auto it = this->gameMap.begin(); it!= this->gameMap.end(); it++)
			{
				if(it->second->getVisited())
					std::cout << it->second->getSpaceName() << std::endl;
			}
			std::cout << "--------------" << std::endl;
		}
		else if(input == "quit"){
		}
		else
		{
			std::cout << "Im afraid that the command " << input << " just doesn't make any sense!" << std::endl;
		}
	}
	if(input == "quit")
	{
		std::cout << "I\'m afraid I can't let you actually use any of those loop changes you might get hurt." << std::endl;
		std::cout << "I can't let you actually use you might get hurt." << std::endl;
		std::cout << "I\'m afraid I can't l37 you mak3 a*y of thos3 l**p ch@ng3s you might g3t hurt." << std::endl;
		std::cout << "The building has been returned to 'normal' but you are still trapped." << std::endl;
	}

}

void GameEngine::loadGameStateMenu(){
	clearScreen();

	// list out saved game files
	int i = 0;
	int input = 0;
	std::cout << i << ": return to game." << std::endl;

	namespace fs = boost::filesystem;
	fs::path spaceDir("Data/Saves/");
	std::vector<std::string> saveFiles;
	if(fs::is_directory(spaceDir)){
		for(auto& entry : boost::make_iterator_range(fs::directory_iterator(spaceDir), {}))
		{
			//check that entry is a file
			if(fs::is_regular_file(entry))
			{
				//setup room name and path vector
				i++;
				std::string filename = entry.path().filename().string();
				saveFiles.push_back(filename);
				std::cout << i << ": " << filename << std::endl;
			}
		}
	}
	// check user input and get user selected file
	do{
		if(!(input >= 0 && input <= saveFiles.size()))
		std::cout << "Not a valid choice! please choose an option between 0 and " << saveFiles.size() << std::endl;
		std::cout << "Enter Option: ";
		std::cin >> input;
	}while(input < 0 && input < saveFiles.size());
	
	//clear and flush stream
	std::cin.clear();
	std::cin.ignore(INT_MAX,'\n');

	if(input != 0)
	{
		loadGameState(saveFiles[input-1]);
		look();
	}
}
/*********************************************************************
** Description: Use item functions
** Input: string&
** Output: bool
*********************************************************************/
bool GameEngine::use(std::string& input){
	std::vector<Item*> validItems;
	if(input.size() > 0)
	{
		roomItemParser(validItems, input); // items in current room
		inventoryParser(validItems, input); // items in inventory
		if(validItems.size()>0){
			std::vector<std::string> itemList;
			for(auto it = validItems.begin(); it != validItems.end(); it++ ){
				itemList.push_back((*it)->getItemName());
			}
				if(std::find(itemList.begin(), itemList.end(), "broken infinity machine") != itemList.end()
				&& itemList.size()==1)
				{
					std::cout << "You attempt to use the Infinity Machine. But the screen is none functional." << std::endl;
					if(std::get<0>(this->itemsMap.at("damaged circuit board"))->isHidden()){
											if(std::get<1>(this->itemsMap.at("damaged circuit board"))->getSpaceName() == "prototype room")
						{
						std::cout << "While attempting to use the Infinity Machine you noticed one of the exposed\ncircuits has been damaged." << std::endl;	
						std::get<0>(this->itemsMap.at("damaged circuit board"))->setHidden(false);
						std::get<0>(this->itemsMap.at("damaged circuit board"))->setTakeable(true);
						}
					
					}
					displayMenu(false);
					return true;
				}
				else if(std::find(itemList.begin(), itemList.end(), "repaired infinity machine") != itemList.end()
				&& itemList.size()==1)
				{
					if (std::get<2>(this->puzzleTracker.at("power draw")) != nullptr) {
						infin();
						return true;
					}
					else {
						std::cout << "There is not enough power routed to the infinity machine!" << std::endl;
						std::cout << "Check the wiring in the power supply room." << std::endl;
						return false;
					}
				}
				else if(std::find(itemList.begin(), itemList.end(), "switch") != itemList.end()
				&& itemList.size() == 1)
				{
					if(this->generatorSwitch == false){
						this->generatorSwitch = true;
						if (std::get<0>(this->puzzleTracker.at("power draw"))->isHidden())
							std::get<0>(this->puzzleTracker.at("power draw"))->setHidden(false);
						std::cout << "As you flip the switch you hear the generators\nstart up in a distant part of the building."<<std::endl;
						displayMenu(false);
					}
					else{
						this->generatorSwitch = false;
						std::cout << "As you flip the switch you hear the generators\nstop running."<<std::endl;
					}

					return true;
				}
				else if(std::find(itemList.begin(), itemList.end(), "flashlight") != itemList.end()) {
					if (this->gamePlayer.getCurrentLoc()->getSpaceName() == "basement" &&
					    std::get<2>(this->puzzleTracker.at("lockbox")) == nullptr) {
						if (std::get<0>(this->puzzleTracker.at("lockbox"))->isHidden()) {
							std::get<0>(this->puzzleTracker.at("lockbox"))->setHidden(false);
							std::cout << "The flashlight reveals a lockbox on a table." << std::endl;
							std::cout << std::endl;
						}
						else
							std::cout << "You wave your flashlight around. Nothing eventful happens." << std::endl;
					}
					else
						std::cout << "You wave your flashlight around. Nothing eventful happens." << std::endl;
					displayMenu(false);
					return true;
				}
				else if(std::find(itemList.begin(), itemList.end(), "brass key") != itemList.end()) {
					if (this->gamePlayer.getCurrentLoc()->getSpaceName() == "robotics lab" &&
					    std::get<2>(this->puzzleTracker.at("robot")) == nullptr) {
						if (std::get<0>(this->puzzleTracker.at("robot"))->isHidden()) {
							std::get<0>(this->puzzleTracker.at("robot"))->setHidden(false);
							std::cout << "The key inserts into the console and activates the robot" << std::endl;
							std::cout << std::endl;
						}
						else {
							std::cout << "You turn the key in the console again, but nothing happens" << std::endl;
						}
					}
					else
						std::cout << "You swing the brass key around wildly seeking answers, but nothing happens." << std::endl;
					displayMenu(false);	
					return true;
				}
				else if(std::find(itemList.begin(), itemList.end(), "sink") != itemList.end()) {
					if (this->gamePlayer.getCurrentLoc()->getSpaceName() == "washroom") {
						if (std::get<0>(this->itemsMap.at("running sink"))->isHidden()) {
							std::cout << "The sink starts flowing with a dark green liquid." << std::endl;
							std::cout << "The knobs fall off and are no longer usable." << std::endl;
							std::get<0>(this->itemsMap.at("sink"))->setHidden(true);
							std::get<0>(this->itemsMap.at("running sink"))->setHidden(false);
							displayMenu(false);
							return true;
						}
						else {
							std::cout << "You can no longer turn off the sink!" << std::endl;
							return false;
						}
					}
				}
				else if(std::find(itemList.begin(), itemList.end(), "bucket") != itemList.end()) {
					std::cout << "I didn't quite understand that. Try \"fill bucket\" or \"pour bucket\"." << std::endl;
					return false;
				}
				else if(std::find(itemList.begin(), itemList.end(), "slime bucket") != itemList.end()) {
					std::cout << "Did you mean \"pour slime bucket\"?" << std::endl;
					return false;
				}

			else
			{
				std::cout << "That item can't be used like that." << std::endl;
			}
		}
		else
		{
			std::cout << input << " is not a valid item!" << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << "Nothing was specified to be used!" << std::endl;
		return false;
	}
}

/*********************************************************************
** Description: Combining item functions
** Input: string&
** Output: bool
*********************************************************************/
bool GameEngine::combine(std::string& input){
	std::vector<Item*> validItems;
	if(input.size() > 0)
	{
		roomItemParser(validItems, input); // items in current room
		inventoryParser(validItems, input); // items in inventory
		if(validItems.size()>0){
			std::vector<std::string> itemList;
			for(auto it = validItems.begin(); it != validItems.end(); it++ ){
				itemList.push_back((*it)->getItemName());
			}
			//call item functions in here
			if(std::find(itemList.begin(), itemList.end(), "powered soldering iron") != itemList.end()
				&& std::find(itemList.begin(), itemList.end(), "damaged circuit board") != itemList.end()
				&& std::find(itemList.begin(), itemList.end(), "electrical components") != itemList.end()
				&& itemList.size() == 3)
				{
					std::get<0>(this->itemsMap.at("repaired circuit board"))->setHidden(false);
					std::get<0>(this->itemsMap.at("repaired circuit board"))->setTakeable(true);

					std::get<0>(this->itemsMap.at("damaged circuit board"))->setHidden(true);
					std::get<0>(this->itemsMap.at("damaged circuit board"))->setTakeable(false);
					this->updateInvent(std::get<0>(this->itemsMap.at("damaged circuit board")), nullptr);

					std::get<0>(this->itemsMap.at("electrical components"))->setHidden(true);
					std::get<0>(this->itemsMap.at("electrical components"))->setTakeable(false);
					this->updateInvent(std::get<0>(this->itemsMap.at("electrical components")), nullptr);

					std::cout << "You use the soldering iron and electrical components to repair the circuit board." << std::endl;
					displayMenu(false);
					return true;
				}
				else if(std::find(itemList.begin(), itemList.end(), "cold soldering iron") != itemList.end()
				&& std::find(itemList.begin(), itemList.end(), "power cord") != itemList.end()
				&& itemList.size() == 2)
				{
					std::get<0>(this->itemsMap.at("powered soldering iron"))->setHidden(false);
					std::get<0>(this->itemsMap.at("cold soldering iron"))->setHidden(true);
					std::get<0>(this->itemsMap.at("power cord"))->setHidden(true);
					std::get<0>(this->itemsMap.at("power cord"))->setTakeable(false);
					std::get<0>(this->itemsMap.at("power cord"))->setTaken(false);
					this->updateInvent(std::get<0>(this->itemsMap.at("power cord")), nullptr);
					this->updateInvent(std::get<0>(this->itemsMap.at("cold soldering iron")), nullptr);

					std::cout << "You connect the power cord to the soldering iron. It turns on and is heating up." << std::endl;
					displayMenu(false);
					return true;
				}
				else if(std::find(itemList.begin(), itemList.end(), "broken infinity machine") != itemList.end()
				&& std::find(itemList.begin(), itemList.end(), "repaired circuit board") != itemList.end()
				&& itemList.size() == 2)
				{
					std::cout << "As you insert the repaired board, the monitor for the machine lights up." << std::endl;
					std::get<0>(this->itemsMap.at("repaired circuit board"))->setHidden(true);
					std::get<0>(this->itemsMap.at("repaired circuit board"))->setTakeable(false);
					std::get<0>(this->itemsMap.at("broken infinity machine"))->setHidden(true);
					std::get<0>(this->itemsMap.at("repaired infinity machine"))->setHidden(false);
					this->updateInvent(std::get<0>(this->itemsMap.at("repaired circuit board")), nullptr);
					this->updateInvent(std::get<0>(this->itemsMap.at("broken infinity machine")), nullptr);
					displayMenu(false);
					return true;
				}

			else
			{
				std::cout << "That item can't be used like that." << std::endl;
			}
		}
		else
		{
			std::cout << input << " is not a valid item!" << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << "Nothing was specified to be combined!" << std::endl;
		return false;
	}
}

/*********************************************************************
** Description: Fill function for bucket
** Input:
** Output: bool
*********************************************************************/
bool GameEngine::fill(std::string& input){
	if (input.size() > 0) {
		if (parser(input, "bucket")) {	
			if (std::get<2>(this->itemsMap.at("bucket")) != nullptr) {
				if (this->gamePlayer.getCurrentLoc()->getSpaceName() == "washroom" &&
	    	    	    	    !std::get<0>(this->itemsMap.at("running sink"))->isHidden()) {
					std::cout << "You fill the bucket with the green liquid." << std::endl;
					std::get<0>(this->itemsMap.at("bucket"))->setHidden(true);
					std::get<0>(this->itemsMap.at("bucket"))->setTaken(false);
					this->updateInvent(std::get<0>(this->itemsMap.at("bucket")), nullptr);
					std::get<0>(this->itemsMap.at("slime bucket"))->setHidden(false);
					std::get<0>(this->itemsMap.at("slime bucket"))->setTaken(true);
					this->updateInvent(std::get<0>(this->itemsMap.at("slime bucket")), std::addressof(this->gamePlayer));
					inventory();
					return true;
				}
				else 
					std::cout << "You need a water source to fill the bucket!" << std::endl;
			}
			else
				std::cout << "You don't have a bucket..." << std::endl;
		}
		else 
			std::cout << "Only buckets can be filled! Please find a bucket." << std::endl;
	}
	else 
		std::cout << "Please specify something to fill" << std::endl;
	
	return false;
}

/*********************************************************************
** Description: Pour function for bucket
** Input: string&
** Output: bool
*********************************************************************/
bool GameEngine::pour(std::string& input){
	if (input.size() > 0) {
		if (parser(input, "slime bucket")) {	
			if (std::get<2>(this->itemsMap.at("slime bucket")) != nullptr) {
				if (this->gamePlayer.getCurrentLoc()->getSpaceName() == "deck" &&
	    	    	    	    std::get<0>(this->itemsMap.at("hidden message"))->isHidden()) {
					std::cout << "You attempt to pour the slime into the fountain." << std::endl;
					std::cout << "The slime reacts violently and dissolves the bucket." << std::endl;
					std::cout << "The slime bounces around in the fountain and fills it up." << std::endl;
					std::cout << "A hidden message can be seen in the fountain." << std::endl;
					std::get<0>(this->itemsMap.at("fountain"))->setHidden(true);
					std::get<0>(this->itemsMap.at("slime bucket"))->setHidden(true);
					std::get<0>(this->itemsMap.at("slime bucket"))->setTaken(false);
					this->updateInvent(std::get<0>(this->itemsMap.at("slime bucket")), nullptr);
					std::get<0>(this->itemsMap.at("hidden message"))->setHidden(false);
					displayMenu(false);
					return true;
				}
				else {
					std::cout << "You try to pour the slime out, but the slime stays put." << std::endl;
				}
			}
			else
				std::cout << "You don't have a slime bucket..." << std::endl;
		}
		else if(parser(input, "bucket"))
			std::cout << "Your bucket is empty!" << std::endl;
		else 
			std::cout << "You can't pour that!" << std::endl;
		
	}
	else 
		std::cout << "Please specify something to pour." << std::endl;
	

	return false;
}
