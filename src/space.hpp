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
		bool visited;
		bool looped;
		bool filledLiquid;
		bool filledSolid;
		std::string spaceName;

    public:
        /** Default constructor */
        Space(std::string path);
        /** Default destructor */
        ~Space();
		std::map<std::string, Space*>exitMap;
        std::string getSpaceName();
		bool getVisited();
		bool getLooped();
		bool getFilledLiquid();
		bool getFilledSolid();

		void setVisited(bool b);
		void setLooped(bool b);
		void setFilledLiquid(bool b);
		void setFilledSolid(bool b);

		void linkExitMapPtr(std::map<std::string, Space*>);
		std::string findExits();
};

#endif // SPACE_HPP
