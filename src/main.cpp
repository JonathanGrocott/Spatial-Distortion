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
#include "ui.hpp"


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
            clearScreen();
            GameEngine* game = new GameEngine; //initialize new game with starting values
            game->mainGameLoop();
            delete game;    //deallocate memory

            clearScreen();
        }
        if(option=="2"){//Load saved game
            clearScreen();

            // list out saved game files
            int i = 0;
            int input = 0;
            std::cout << i << ": return to main menu." << std::endl;

            namespace fs = boost::filesystem;
	        fs::path spaceDir("Data/Saves/");
            std::vector<std::string> saveFiles;
            if(fs::is_directory(spaceDir)){
                for(auto& entry : boost::make_iterator_range(fs::directory_iterator(spaceDir), {}))
                {
                    //check that entry is a file
                    if(fs::is_regular_file(entry))
                    {
                        //setup room name and path vector
                        i++;
                        std::string filename = entry.path().filename().string();
                        saveFiles.push_back(filename);
                        std::cout << i << ": " << filename << std::endl;
                    }
                }
            }
            // check user input and get user selected file
            do{
                if(!(input >= 0 && input <= saveFiles.size()))
                std::cout << "Not a valid choice! please choose an option between 0 and " << saveFiles.size() << std::endl;
                std::cout << "Enter Option: ";
                std::cin >> input;
            }while(input < 0 && input < saveFiles.size());
            
            //clear and flush stream
            std::cin.clear();
            std::cin.ignore(INT_MAX,'\n');

            // start saved game
            if(input != 0)
            {
                GameEngine* game = new GameEngine(saveFiles[input-1]); //initialize new game with starting values
                game->mainGameLoop();
                delete game;    //deallocate memory
            }
            
            clearScreen();
        }

        if(option=="3"){//Key to the game for grader

        }

        if(option=="4"){//exit game
            quit = true;
        }

    }while(!quit);//quit game bool

    clearScreen();
    
    return 0;
}
