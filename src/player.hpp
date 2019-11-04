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

class player
{
	private:
        	Space* currentLoc;
	public:
        	/** Default constructor */
        	player();
        	/** Default destructor */
        	~player();
        
        	Space* getCurrentLoc();
        	void setCurrentLoc(Space* loc);
};



#endif
