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
		std::vector<std::string> objects;
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
		std::unordered_map<std::string, Space*>exitMap;

		// get functions for private variables
        std::string getSpaceName();
		std::string getFilePath();
		bool getVisited();
		std::vector<std::string> getSpaceObjects();
		bool getLooped();
		bool getFilledLiquid();
		bool getFilledSolid();
		std::string findExits();
		//set functions for private variables
		void setVisited(bool b);
		void setLooped(bool b);
		void setFilledLiquid(bool b);
		void setFilledSolid(bool b);
		void addObject(std::string obj);
		void removeObject(std::string obj);
};

#endif // SPACE_HPP
