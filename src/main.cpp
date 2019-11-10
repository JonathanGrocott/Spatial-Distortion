/*********************************************************************
** Program Filename: main.cpp
** Author: Jonathan Grocott
** Date: 10/15/19
** Description: Main for Spatial Distortion Game.
*********************************************************************/
#include "gameengine.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>


int main()
{
    std::string option;
	bool quit=false;
    do{
        std::cout << "1) NEW GAME" << std::endl;
        std::cout << "2) LOAD EXISTING GAME" << std::endl;
        std::cout << "3) DISPLAY KEY TO GAME" << std::endl;
        std::cout << "4) END GAME"<< std::endl;
        std::cout << "Enter Option: ";
        std::getline(std::cin, option);

        if(option=="1"){//main game driver
            
            GameEngine* game = new GameEngine; //initialize new game with starting values
            game->mainGameLoop();
            delete game;    //deallocate memory
        }
        if(option=="2"){//Load saved game
            // GameEngine* game = new GameEngine(savedGame); // initial game with saved file
            //game->mainGameLoop();
        }

        if(option=="3"){//Key to the game for grader

        }

        if(option=="4"){//exit game
            quit = true;
        }

    }while(!quit);//quit game bool

    return 0;
}
