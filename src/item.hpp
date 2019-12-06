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
		bool takeable;
		bool hidden;
		std::string trigger; // Trigger to reveal itself
		std::string itemDesc; //description
		std::string itemFilePath;
		std::unordered_map<std::string, Space*> foundAt;	
	public:
		Item(std::string path, std::unordered_map<std::string, Space*>);
		~Item();
		std::string getItemName();
		std::string getItemDesc();
		Space* getBegLoc(); // Beginning Location
		bool isTaken();
		bool isTakeable();
		bool isHidden();
		std::string getTrigger();
		void setTaken(bool);
		void setTakeable(bool);
		void setHidden(bool);
};

#endif
