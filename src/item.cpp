#include "item.hpp"

// Item constructor
Item::Item(std::string path, std::unordered_map<std::string, Space*> gameMap) {
	this->taken = false;
	std::ifstream File(path);
	if (File) {
		this->itemFilePath = path;
		while (File.good()) {
			std::string tempLine;
			std::getline(File, tempLine);

			if (!tempLine.compare("<name>")) {
				std::getline(File, tempLine);
				boost::algorithm::to_lower(tempLine);
				this->itemName = tempLine;
			}
			if (!tempLine.compare("<breakable>")) {
				std::getline(File, tempLine);
				boost::algorithm::to_lower(tempLine);
				if (tempLine.compare("true"))
					this->breakable = true;
				else
					this->breakable = false;
			}
			if (!tempLine.compare("<takeable>")) {
				std::getline(File, tempLine);
				boost::algorithm::to_lower(tempLine);
				if (!tempLine.compare("true"))
					this->takeable = true;
				else
					this->takeable = false;
			}
			if (!tempLine.compare("<desc>")) {
				std::getline(File, tempLine);
				boost::algorithm::to_lower(tempLine);
				this->itemDesc = tempLine;
			}
			if (!tempLine.compare("<found_in>")) {
				std::getline(File, tempLine);
				boost::algorithm::to_lower(tempLine);
				this->foundAt[tempLine] = nullptr;
			}
		}
	}
	for (auto it = this->foundAt.begin(); it != this->foundAt.end(); it++) {
		// Set the beginning location and current location
		if (gameMap.count(it->first)) {
			it->second = gameMap.at(it->first);
			this->setCurrentLoc(it->second);
		}
		else
			std::cout << it->first << " does not exist." << std::endl;
	}

}

// Item destructor
Item::~Item() {
	for (auto it = this->foundAt.begin(); it != this->foundAt.end(); it++)
		it->second = nullptr;
} 

// Returns the item name
std::string Item::getItemName() {
	return this->itemName;
}

// Returns the item description
std::string Item::getItemDesc() {
	return this->itemDesc;
}

// Gets the current location of the item
Space* Item::getCurrentLoc() {
	return this->currentLoc;
}

// Gets the beginning location of the item
Space* Item::getBegLoc() {
	return foundAt.begin()->second;
}

// Checks if the item has been taken
bool Item::isTaken() {
	return this->taken;
}

// Checks if the item can break or not
bool Item::isBreakable() {
	return this->breakable;
}

// Checks if the item is takeable or not
bool Item::isTakeable() {
	return this->takeable;
}

// Sets the item's current location
void Item::setCurrentLoc(Space *loc) {
	this->currentLoc = loc;
}

// Sets the item's taken status
void Item::setTaken(bool status) {
	this->taken = status;
}
