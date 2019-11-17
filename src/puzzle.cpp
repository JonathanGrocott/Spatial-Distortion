#include "puzzle.hpp"

// Puzzle default constructor
Puzzle::Puzzle(std::string path) {
	this->complete = false;
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

		}
		File.close();
	}
}

// Puzzle destructor
Puzzle::~Puzzle() {} 

// Returns the puzzle name
std::string Puzzle::getPuzzName() {
	return this->puzzName;
}

// Returns the fail message
std::string Puzzle::getFail() {
	return this->failMessage;
}

// Returns the success message
std::string Puzzle::getSuccess() {
	return this->successMessage;
}

// Checks if the item has been taken
bool Puzzle::isComplete() {
	return this->complete;
}

// Sets the puzzle's completion status
void Puzzle::setComplete(bool status) {
	this->complete = status;
}
