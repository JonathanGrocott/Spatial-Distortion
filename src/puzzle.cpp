#include "puzzle.hpp"

// Puzzle default constructor
Puzzle::Puzzle(std::string path, std::unordered_map<std::string, Space*> gameMap) {
	this->puzzDesc = "";
	this->failMessage = "";
	this->successMessage = "";
	
	std::ifstream File(path);
	if (File) {
		this->puzzFilePath = path;
		while (File.good()) {
			std::string tempLine;
			std::getline(File, tempLine);

			if (!tempLine.compare("<name>")) {
				std::getline(File, tempLine);
				this->puzzName = tempLine;
			}
			if (!tempLine.compare("<desc>")) {
				while(tempLine.compare("</desc>")) {
					std::getline(File, tempLine);
					if (tempLine.compare("</desc>"))
						this->puzzDesc = this->puzzDesc + tempLine + "\n";
				}
			}
			if (!tempLine.compare("<fail>")) {
				while(tempLine.compare("</fail>")) {
					std::getline(File, tempLine);
					if (tempLine.compare("</fail>"))
						this->failMessage = this->failMessage + tempLine + "\n";
				}
			}
			if (!tempLine.compare("<success>")) {
				while(tempLine.compare("</success>")) {
					std::getline(File, tempLine);
					if (tempLine.compare("</success>"))
						this->successMessage = this->successMessage + tempLine + "\n";
				}
			}
			if (!tempLine.compare("<found_in>")) {
				std::getline(File, tempLine);
				boost::algorithm::to_lower(tempLine);
				this->location[tempLine] = nullptr;	
				
			}
			if (!tempLine.compare("<hidden>")) {
				std::getline(File, tempLine);
				boost::algorithm::to_lower(tempLine);
				if (!tempLine.compare("true"))
					this->hidden = true;
				else
					this->hidden = false;	
			}

		}
		File.close();
		
		for (auto it = this->location.begin(); it != this->location.end(); it++) {
			if (gameMap.count(it->first)) {
				it->second = gameMap.at(it->first);
			}
			else
				std::cout << "This location does not exist!" << std::endl;
		}
	}
}

// Puzzle destructor
Puzzle::~Puzzle() {} 

// Returns the puzzle name
std::string Puzzle::getPuzzName() {
	return this->puzzName;
}

// Returns the puzzle description
std::string Puzzle::getPuzzDesc() {
	return this->puzzDesc;
}

// Returns the fail message
std::string Puzzle::getFail() {
	return this->failMessage;
}

// Returns the success message
std::string Puzzle::getSuccess() {
	return this->successMessage;
}

// Returns the puzzle's location
Space* Puzzle::getPuzzLocation() {
	return this->location.begin()->second;
}

// Gets the puzzle's hidden status
bool Puzzle::isHidden() {
	return this->hidden;
}

// Sets the puzzle's hidden status
void Puzzle::setHidden(bool status) {
	this->hidden = status;
}