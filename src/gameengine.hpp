/*********************************************************************
** Program Filename: gameengine.hpp
** Author: Jonathan Grocott
** Date: 10/15/19
** Description: Header file for gameengine class.

*********************************************************************/

#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP


#include"space.hpp"
#include"boost/filesystem.hpp"
#include"Commands/commands.hpp"

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
	vector<string>result;

	vector<string>files;
	void get_file_list(const string& path);

	vector<string> split(string str, string token);

	void setAllPossibleMoves();

	void createSpacesFromFiles(GameEngine* game, string dir);
			
	void getSpaceContents(string file);
	void getObjectContents(string file);

	Space* currentLocation;
        void moveLocation(Space* temp);

        Space* getCurrentLocation();
        void setCurrentLocation(Space* cl);

        void displayMenu(GameEngine* game, Space* cL, Commands* obj);//menu for each location
	bool readCommand(GameEngine* game, Space* cL, Commands* obj, string choice);

        bool fp, lp, rp, bp;

        string getFileContents (ifstream& File);//ascii display


        //space pointers
		//Spaces: Entry, Prototype Room, Washroom, Electronics Lab, Imaging Room, Power control room,
		//Hallway, Research Lab 1, Office 1, Hallway 2, Research Lab 2, Basement, Generator Room, Robtics Lab, Deck
		Space* Entry;
		Space* PrototypeRoom;
		Space* Washroom;
		Space* ElectronicsLab;
		Space* ImagingRoom;
		Space* PowerRoom;
		Space* Hallway1;
		Space* ResearchLab1;
		Space* ResearchLab2;
		Space* Office1;
		Space* RoboticsLab;
		Space* Hallway2;
		Space* Basement;
		Space* GeneratorRoom;
		Space* Deck;
		

};

#endif // GAMEENGINE_HPP
