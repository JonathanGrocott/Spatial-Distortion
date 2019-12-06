#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include "space.hpp"

#ifndef PUZZLES_HPP
#define PUZZLES_HPP

class Puzzle {
	private:
		std::string puzzName;
		std::string puzzDesc;
		std::string failMessage;
		std::string successMessage;
		std::string puzzFilePath;
		bool hidden;
		std::unordered_map<std::string, Space*> location;
	public:
		Puzzle(std::string, std::unordered_map<std::string, Space*>);
		~Puzzle();
		std::string getPuzzName();
		std::string getPuzzDesc();
		std::string getFail();
		std::string getSuccess();
		bool isHidden();
		Space* getPuzzLocation();
		void setHidden(bool);
};

#endif