/*********************************************************************
** Program Filename: gameengine.hpp
** Author: Jonathan Grocott
** Date: 10/15/19
** Description: Header file for gameengine class.

*********************************************************************/

#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP


#include "space.hpp"
#include "puzzle.hpp"
#include "boost/filesystem.hpp"
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
		std::unordered_map<std::string, std::tuple<Puzzle*, Space*, player*>> puzzleTracker; 
		void initializeGameMap();
		void linkExitPtrs(std::string room, std::unordered_map<std::string,std::string> alias);
		void loadGameState(std::string savedGame);
		void saveGameState();
		bool gameState;
		player gamePlayer;
		// commands
		std::vector<std::string> commandList;
		void help(); // Shows all commands
		void go(std::string room); // Moves user to another room
		bool solve(std::string); // Find the appropriate puzzle and checks the answer 
		bool lockboxPuzzle(); // Checks answer for lockbox puzzle.
		bool testTubePuzzle(); // Checks answer for testtube puzzle.
		void lookAt(std::vector<Item*>); // Gives description of room or inventory objects 
		void inventory(); // Displays entire inventory
		void quit();
		void look();
		void take(Item*);
		void drop(Item*);
	public:
		/** Default constructor */
		GameEngine();
		/** Saved constructor */
		GameEngine(std::string);
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
		void updateItemLoc();
		void updateInvent(Item*, player*);
		void updatePuzzMap(Puzzle*);
		bool parser(std::string &original, std::string tofind);
		std::string getFileContents (std::ifstream& File);//ascii display
};

#endif // GAMEENGINE_HPP
