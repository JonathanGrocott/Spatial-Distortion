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
#include <algorithm>
#include "space.hpp"
#include "item.hpp"

class player
{
	private:
        	Space* currentLoc;
		std::vector<Item*> inventory;
	public:
        	/** Default constructor */
        	player();
        	/** Default destructor */
        	~player();
        
        	Space* getCurrentLoc();
        	void setCurrentLoc(Space* loc);
		std::vector<Item*> getInventory();
		void addInvent(Item* item);
		void removeInvent(Item* item);
		void useItem(Item* item);
};



#endif
