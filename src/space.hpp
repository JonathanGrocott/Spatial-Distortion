/*********************************************************************
** Program Filename: space.hpp
** Author: Jonathan Grocott
** Date: 10/15/19
** Description: Header file for abstract base class space.
*********************************************************************/

#ifndef SPACE_HPP
#define SPACE_HPP


#include <vector>
#include <string>
#include <map>


class Space
{
	private:
		enum {cards = 4};
		bool visited;
		bool looped;
		bool filledLiquid;
		bool filledSolid;
		std::string spaceName;
		std::string cardinals[cards];
	public:
		/** Default constructor */
		Space(std::string path);
		/** Default destructor */
		~Space();
		
		// function for linking default nullptr in exitMap
		void linkExitMapPtr(std::map<std::string, Space*>);

		// map of spaces exits
		std::map<std::string, Space*>exitMap;

		// get functions for private variables
        	std::string getSpaceName();
		bool getVisited();
		bool getLooped();
		bool getFilledLiquid();
		bool getFilledSolid();
		std::string getNorthExit();
		std::string getWestExit();
		std::string getSouthExit();
		std::string getEastExit();
		std::string findExits();
		//set functions for private variables
		void setVisited(bool b);
		void setLooped(bool b);
		void setFilledLiquid(bool b);
		void setFilledSolid(bool b);
};

#endif // SPACE_HPP
