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

/*********************************************************************
** Description: Reads a text file into a ifstream and then returns
the input as a string. Used for ascii picture display to console.
** Input: ifstream by reference
** Output: string of file contents
*********************************************************************/
std::string getFileContents(std::ifstream& File)
{
	std::string Lines = "";        //All lines

	if (File) {                      
		while (File.good()) {
			std::string TempLine;            //Temp line
			std::getline(File, TempLine);        
			TempLine += "\n";                 
			Lines += TempLine;               
		}
		return Lines;
	}
	else { //Return error
		return "ERROR File does not exist.";
	}
}



int main()
{
    clearScreen();
    std::string option;
	bool quit=false;
    do{
		//ASCII Art Title
		std::ifstream Reader("Data/Art/SpatialDistortion.txt");
		std::string Art = getFileContents(Reader);
		std::cout << Art << std::endl;
		Reader.close();
		//Splash Screen Menu
        std::cout << "1) NEW GAME" << std::endl;
        std::cout << "2) LOAD EXISTING GAME" << std::endl;
        std::cout << "3) DISPLAY KEY TO GAME" << std::endl;
	std::cout << "4) EASTER EGG" << std::endl;
        std::cout << "5) END GAME" << std::endl;
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

            // start saved game from selected file
            if(input != 0)
            {
                GameEngine* game = new GameEngine(saveFiles[input-1]); //initialize new game with starting values
                game->mainGameLoop();
                delete game;    //deallocate memory
            }
            
            clearScreen();
        }

        if(option=="3"){//Key to the game for grader
		std::ifstream File("Data/Guides/walkthrough.txt");
		std::cout << std::endl;
        	std::cout << getFileContents(File) << std::endl; 
	}

	if(option=="4") {//Key to find easter egg
		std::ifstream File("Data/Guides/easter egg.txt");
		std::cout << std::endl;
		std::cout << getFileContents(File) << std::endl;
	}

        if(option=="5"){//exit game
			
		quit = true;
        }

    }while(!quit);//quit game bool

    clearScreen();
    
    return 0;
}
