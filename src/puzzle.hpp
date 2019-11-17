#include <iostream>
#include <string>
#include <fstream>

#ifndef PUZZLES_HPP
#define PUZZLES_HPP

class Puzzle {
	private:
		std::string puzzName;
		bool complete;
		std::string failMessage;
		std::string successMessage;
		std::string puzzFilePath;
	public:
		Puzzle(std::string);
		~Puzzle();
		std::string getPuzzName();
		std::string getFail();
		std::string getSuccess();
		bool isComplete();
		void setComplete(bool);
};

#endif
