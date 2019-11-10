/*********************************************************************
** Program Filename: gameengine.hpp
** Author: Jonathan Grocott
** Date: 10/15/19
** Description: Header file for gameengine class.

*********************************************************************/

#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP


#include "space.hpp"
#include "boost/filesystem.hpp"
#include "Commands/commands.hpp"
#include "player.hpp"
#include <unordered_map>
#include <tuple>
#include "item.hpp"
#include <string>

class GameEngine
{
	private:
		std::unordered_map<std::string, Space*> gameMap; //collection of pointers for all spaces in the game
		std::unordered_map<std::string, std::tuple<Item*, Space*, player*>> itemsMap;
		void initializeGameMap();
		void linkExitPtrs(std::string room, std::unordered_map<std::string,std::string> alias);
		bool gameState;
		player gamePlayer;
		Commands* commands;
		
	public:
       		/** Default constructor */
        	GameEngine();
       	 	/** Default destructor */
        	~GameEngine();

		void testMap();
		bool getGameState();
		void setGameState(bool b);
		void mainGameLoop();

		std::vector<std::string> split(std::string str, std::string token);

		void get_file_list(const std::string& path);

		void getSpaceContents(std::string file);
		void getObjectContents(std::string file);
		void displayMenu();//menu for each location
		bool readCommand();
		void updateItemLoc(player*, std::unordered_map<std::string, std::tuple<Item*, Space*, player*>>&);
		void updateInvent(Item*, player*, std::unordered_map<std::string, std::tuple<Item*, Space*, player*>>&);
		bool parser(std::string &original, std::string tofind);
		std::string getFileContents (std::ifstream& File);//ascii display

};

#endif // GAMEENGINE_HPP
