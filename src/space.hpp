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
#include <unordered_map>

class Space
{
	private:
		bool visited;
		bool looped;
		bool filledLiquid;
		bool filledSolid;
		bool bReqItem;
		std::string requiredItem = "";
		std::string spaceName;
		std::string spaceFilePath;

	public:
		/** Default constructor */
		Space(std::string path, std::unordered_map<std::string,std::string> &tempMap);
		/** Default destructor */
		~Space();
		
		// function for linking default nullptr in exitMap
		void linkExitMapPtr(std::string, Space*);

		// map of spaces exits
		std::unordered_map<std::string, Space*> exitMap;
		
		// get functions for private variables
        std::string getSpaceName();
		std::string getFilePath();
		bool getVisited();
		bool getLooped();
		bool getFilledLiquid();
		bool getFilledSolid();
		std::string findExits();
		std::string getReqItem();
		bool getbReqItem();
		//set functions for private variables
		void setVisited(bool b);
		void setLooped(bool b);
		void setFilledLiquid(bool b);
		void setFilledSolid(bool b);
};

#endif // SPACE_HPP