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
        /** Default destructor */
        virtual ~Space();

		bool visited;
		string spaceName, spaceType, longDesc, shortDesc;
		int numObjects, numActions, numExits, numCharacters;

        string getSpaceName();
        void setSpaceName(string n);


        void setSpaceMoves(Space* xp, Space* xn, Space* yp,
                        Space* yn, Space* zp, Space* zn);


        void setSpaceInfo(string sInfo);
        void displaySpaceInfo(Space* sp);

        virtual void getLocationInfo(bool &xp, bool &yp, bool &yn, bool &zp, bool &zn, bool &xn);

        Space* xPositive;
        Space* yNegative;
        Space* yPositive;
        Space* xNegative;
        Space* zPositive;
        Space* zNegative;


        string getFileContents (ifstream& File);


};

#endif // SPACE_HPP
