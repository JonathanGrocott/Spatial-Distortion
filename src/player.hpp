/***************************************************************
** Program Filename: player.hpp
** Author: Adam Powell
** Date: 10/27/19
** Description: player class
***************************************************************/

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>
#include "space.hpp"

class player
{
	private:
        	Space* currentLoc;
		std::vector<std::string> inventory;
	public:
        	/** Default constructor */
        	player();
        	/** Default destructor */
        	~player();
        
        	Space* getCurrentLoc();
        	void setCurrentLoc(Space* loc);
		std::vector<std::string> getInventory();
		void addInvent(std::string item);
		void removeInvent(std::string item);
		void useItem(std::string item);
};



#endif
