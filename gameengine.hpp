/*********************************************************************
** Program Filename: gameengine.hpp
** Author: Jonathan Grocott
** Date: 10/15/19
** Description: Header file for gameengine class.
*********************************************************************/

#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP


#include"space.hpp"

#include<string>


class GameEngine
{
    public:
        /** Default constructor */
        GameEngine();
        /** Default destructor */
        virtual ~GameEngine();


		void getSpaceContents(string file);
		void getObjectContents(string file);

		Space* currentLocation;
        void moveLocation(Space* temp);

        Space* getCurrentLocation();
        void setCurrentLocation(Space* cl);

        void displayMenu(Space* cL);//menu for each location

        bool xp, xn, yp, yn, zp, zn;

        string getFileContents (ifstream& File);//ascii display


        //space pointers
        Space* prototypeRoom;

};

#endif // GAMEENGINE_HPP
