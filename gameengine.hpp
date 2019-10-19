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

		int numMoves;
		int getNumMoves();
		void setNumMoves(int n);

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
		//Spaces: Entry, Prototype Room, Washroom, Electronics Lab, Imaging Room, Power control room,
		//Hallway, Research Lab 1, Office 1, Hallway 2, Research Lab 2, Basement, Generator Room, Robtics Lab, Deck
        Space* Entry, PrototypeRoom, Washroom, ElectronicsLab, ImagingRoom, PowerRoom, Hallway1, ResearchLab1, ResearchLab2, Office1,
			Office2, Hallway2, Base;
		

};

#endif // GAMEENGINE_HPP
