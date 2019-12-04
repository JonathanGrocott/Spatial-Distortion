/***************************************************************
** Program Filename: puzzleFunctions.cpp
** Author: 
** Date: 12/3/2019
** Description: puzzle functions.
***************************************************************/

#include "puzzleFunctions.hpp"
#include <string>
#include <vector>
#include <iostream>
#include "boost/range/iterator_range.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim_all.hpp>

/*********************************************************************
** Description: Checks the answer for lockbox puzzle
** Input:
** Output: bool
*********************************************************************/
bool lockboxPuzzle() {
	std::string answer;
	std::cout << "What number do you input? ";
	std::getline(std::cin, answer);
	if (answer == "42" || answer == "forty two" || answer == "forty-two") {
		return true;
	}
	else if (answer == "cheat") {
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
bool testTubePuzzle() {
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
		
		if (input == "cheat")
			return true;

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
	return true;
}

/*********************************************************************
** Description: Robot grabbing flashlight puzzle checker
** Input:
** Output: bool
*********************************************************************/
bool robotPuzzle() {
	int player = 1; // player's current position
	const int WIDTH = 9;
	int move = 1; // How much the robot moves
	std::string input;
	do {

		// Display
		std::cout << "Robot is currently moving: " << move << " spaces." << std::endl;
		for (int i = 0; i < WIDTH; i++) {
			std::cout << "|";
			if (i == player - 1)
				std::cout << "X";
			else if (i == WIDTH - 1)
				std::cout << "F";
			else
				std::cout << "-";
			std::cout << "|";
		}
		// User input
		std::cout << std::endl;
		std::cout << "Your move: ";
		std::getline(std::cin, input);
		if (input == "q" || input == "quit" || input == "reset")
			return false;
		
		if (input == "cheat")
			return true;

		if (input == "left") {
			if (player - move < 1) 
				std::cout << "The robot slams into the wall and resets its position" << std::endl;
			else {
				player -= move;
				if (move + 1 > 3)
					move = 1;
				else
					move++;
			}
		}
		else if (input == "right") {
			if (player + move > WIDTH)
				std::cout << "The robot slams into the wall and resets its position" << std::endl;
			else {
				player += move;
				if (move + 1 > 3)
					move = 1;
				else
					move++;
			}
		}
		else
			std::cout << input << " is not a valid move!" << std::endl;

	} while (player != WIDTH);	
	
	return true;
}
