/*********************************************************************
** Program Filename: space.cpp
** Author: Jonathan Grocott
** Date: 10/15/19
** Description: Implimentation of abstract base class space. Constructor
is abstract and will not be instatiated.

*********************************************************************/

#include "space.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "boost/filesystem.hpp"
#include "boost/filesystem/fstream.hpp"
#include <boost/algorithm/string.hpp>

#include <fstream>
#include "ui.hpp"


/*********************************************************************
** Description: Constructor for a space object
** Input: Rerference to a file path
** Output: 
*********************************************************************/
Space::Space(std::string path, std::unordered_map<std::string,std::string> &tempMap)
{
	this->visited = false;
	this->looped = false;
	this->filledLiquid = false;
	this->filledSolid = false;
	std::ifstream File(path);

	if(File){                      //Check if opens ok
		this->spaceFilePath = path;
		while (File.good ()){
			std::string TempLine;                  //Temp line
	                std::getline (File , TempLine);        //Get temp line
			
			//setup the space name
			if(!TempLine.compare("<name>")){
				std::getline (File , TempLine);
				boost::algorithm::to_lower(TempLine);
				this->spaceName = TempLine;
			}
			
			//setup exits map
			if(!TempLine.compare("<exits>")){
				while(TempLine.compare("</exits>"))
				{
					std::getline (File , TempLine);
					if(TempLine.compare("</exits>")){
						boost::algorithm::to_lower(TempLine);
						std::vector<std::string> result; 
						boost::split(result, TempLine, boost::is_any_of(",")); 
					
						for (int i = 0; i < result.size(); i++)
						{

							this->exitMap[result[i]] = nullptr;
							tempMap[result[i]] = result[0];

						}
					}
				}
			}
        }
		File.close();
    }
    else{ //Return error
        std::cout << "ERROR File does not exist." << std::endl;
    }
}

/*********************************************************************
** Description: destructor for a space object
** Input: 
** Output: 
*********************************************************************/
Space::~Space()
{
	// set ptrs to null
	for (std::unordered_map<std::string,Space*>::iterator it=this->exitMap.begin(); it!=this->exitMap.end(); ++it){
		it->second = nullptr;
	}
}

/*********************************************************************
** Description: Update the nullptr for exits, after the game map has
** finished being initialized.
** Input: a completed map with constructed spaces.
** Output: 
*********************************************************************/
void Space::linkExitMapPtr(std::string exit, Space* exitptr){
	this->exitMap[exit] = exitptr;
}

/*********************************************************************
** Description: Return space name
** Input: 
** Output: string
*********************************************************************/
std::string Space::getSpaceName(){
    return this->spaceName;
}

/*********************************************************************
** Description: Return file path
** Input: 
** Output: string
*********************************************************************/
std::string Space::getFilePath(){
    return this->spaceFilePath;
}

/*********************************************************************
** Description: Return visited status
** Input: 
** Output: bool
*********************************************************************/
bool Space::getVisited() {
	return this->visited;
}

/*********************************************************************
** Description: Return visited status
** Input: 
** Output: bool
*********************************************************************/
bool Space::getLooped() {
	return this->looped;
}

/*********************************************************************
** Description: if room is filled with water 
** Input: 
** Output: bool
*********************************************************************/
bool Space::getFilledLiquid() {
	return this->filledLiquid;
}

/*********************************************************************
** Description: if room is filled with solid
** Input: 
** Output: bool
*********************************************************************/
bool Space::getFilledSolid() {
	return this->filledSolid;
}

/*********************************************************************
** Descriptwater
** Input: bwater
** Output: water
*********************************************************************/
void Space::setVisited(bool b) {
	this->visited = b;
}

/*********************************************************************
** Description: Change room looped status
** Input: bool
** Output: 
*********************************************************************/
void Space::setLooped(bool b){
	this->looped = b;
}

/*********************************************************************
** Description: Change room filled with water status
** Input: bool
** Output: 
*********************************************************************/
void Space::setFilledLiquid(bool b){
	this->filledLiquid = b;
}

/*********************************************************************
** Description: Change room filled with solid status
** Input: bool
** Output: 
*********************************************************************/
void Space::setFilledSolid(bool b){
	this->filledSolid = b;
}

/*********************************************************************
** Description: Generate a string of exits for space
** Input: 
** Output: string 
*********************************************************************/
std::string Space::findExits(){
	std::string exitStr = "";
	for (std::unordered_map<std::string,Space*>::iterator it=this->exitMap.begin(); it!=this->exitMap.end(); ++it)
		exitStr += it->first + " ";
	return exitStr;
}
