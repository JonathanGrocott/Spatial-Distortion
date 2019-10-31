/*********************************************************************
** Program Filename: main.cpp
** Author: Jonathan Grocott
** Date: 10/15/19
** Description: Main for Spatial Distortion Game.
*********************************************************************/
#include"gameengine.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include<fstream>




/*********************************************************************
** Description: Reads a text file into a ifstream and then returns
the input as a string. Used for ascii picture display to console.
** Input: ifstream by reference
** Output: string of file contents
*********************************************************************/
std::string getFileContents (std::ifstream& File)
{
    std::string Lines = "";        //All lines

    if(File){                      //Check if opens ok
        while (File.good ()){
            std::string TempLine;                  //Temp line
            std::getline (File , TempLine);        //Get temp line
            TempLine += "\n";                 //Add newline character
            Lines += TempLine;               //Add newline
        }
            return Lines;
    }
    else{ //Return error
        return "ERROR File does not exist.";
    }
}


int main()
{
    int option;
	bool quit=false;
    do{
        std::cout << "1) NEW GAME" << std::endl;
        std::cout << "2) LOAD EXISTING GAME" << std::endl;
        std::cout << "3) DISPLAY KEY TO GAME" << std::endl;
        std::cout << "4) END GAME"<< std::endl;
        std::cout << "Enter Option: ";
        std::cin >> option;

        if(option==1){//main game driver
            
            GameEngine* game = new GameEngine; //initialize new game with starting values
            game->mainGameLoop();
            delete game;    //deallocate memory
        }
        if(option==2){//Load saved game
            // GameEngine* game = new GameEngine(savedGame); // initial game with saved file
            //game->mainGameLoop();
        }

        if(option==3){//Key to the game for grader

        }

        if(option==4){//exit game
            quit = true;
        }

    }while(!quit);//quit game bool

    return 0;
}
