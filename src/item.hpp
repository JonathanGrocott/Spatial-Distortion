#include "space.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <boost/algorithm/string.hpp>

#ifndef ITEMS_HPP
#define ITEMS_HPP
 
class Item {
	private:
		std::string itemName;
		bool taken; // Has the item been taken?
		bool breakable;
		bool takeable;
		std::string itemDesc; //description
		std::string itemFilePath;
		Space* currentLoc;
		std::unordered_map<std::string, Space*> foundAt;	
	public:
		Item(std::string path);
		~Item();
		std::string getItemName();
		std::string getItemDesc();
		Space* getCurrentLoc();
		Space* getBegLoc(); // Beginning Location
		void linkItemToRoom(std::unordered_map<std::string, Space*>);
		bool isTaken();
		bool isBreakable();
		bool isTakeable();
		void setCurrentLoc(Space*);
		void setTaken(bool);
};

#endif
