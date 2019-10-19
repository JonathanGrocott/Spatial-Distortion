/*********************************************************************
** Program Filename: space.hpp
** Author: Jonathan Grocott
** Date: 10/15/19
** Description: Header file for abstract base class space.

*********************************************************************/

#ifndef SPACE_HPP
#define SPACE_HPP


#include<vector>
#include<string>

using namespace std;

class Space
{
    public:
        /** Default constructor */
        Space();
		//Constructor
		Space(string n, string st, string ld, string sd, int no, int na, int ne, int nc, bool v);
        /** Default destructor */
        virtual ~Space();

		bool visited;
		string spaceName, spaceType, longDesc, shortDesc;
		int numObjects, numActions, numExits, numCharacters;

        string getSpaceName();
        void setSpaceName(string n);
		string getSpaceType();
		void setSpaceType(string n);
		string getLongDesc();
		void setLongDesc(string n);
		string getShortDesc();
		void setShortDesc(string n);
		bool getVisited();
		void setVisited(bool b);
		int getNumObjects();
		void setNumObjects(int n);
		int getNumActions();
		void setNumActions(int n);
		int getNumExits();
		void setNumExits(int n);
		int getNumCharacters();
		void setNumCharacters(int n);


        void setSpaceMoves(Space* fp, Space* lp, Space* rp, Space* bp);


        void displaySpaceInfo(Space* sp);

        virtual void getLocationInfo(bool &fp, bool &lp, bool &rp, bool &bp);

        Space* Forward;
        Space* Left;
        Space* Right;
        Space* Back;

		void locationInteract(Space* cl);

        string getFileContents (ifstream& File);


};

#endif // SPACE_HPP
