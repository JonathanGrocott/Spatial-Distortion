/*********************************************************************
** Program Filename: main.cpp
** Author: Jonathan Grocott
** Date: 10/15/19
** Description: Main for Spatial Distortion Game.
*********************************************************************/

#include"space.hpp"
#include"gameengine.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include<fstream>


using namespace std;

/*********************************************************************
** Description: Reads a text file into a ifstream and then returns
the input as a string. Used for ascii picture display to console.
** Input: ifstream by reference
** Output: string of file contents
*********************************************************************/
string getFileContents (ifstream& File)
{
    string Lines = "";        //All lines

    if(File){                      //Check if opens ok
        while (File.good ()){
            string TempLine;                  //Temp line
            getline (File , TempLine);        //Get temp line
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
    string pName;//player name
    int option;
	bool quit=false;

    GameEngine* game = new GameEngine;//default game engine
    Space* space;//default pointer to space
    Commands* com = new Commands; //default pointer to commands

	space = game->getCurrentLocation();

        do{
            cout<<"1) PLAY GAME" <<endl;
			cout<<"2) LOAD EXISTING GAME" << endl;
            cout<<"3) DISPLAY KEY TO GAME" <<endl;
            cout<<"4) END GAME"<<endl;
            cout<<"Enter Option: ";
            cin>>option;

            if(option==1){//main game driver
				game->createSpacesFromFiles(game, "Spaces/");

				game->setAllPossibleMoves();
				
				game->displayMenu(game, space, com);
				
            }
			if(option==2){//Load saved game

            }

            if(option==3){//Key to the game for grader

            }

            if(option==4){//exit game
                quit = true;
            }

        }while(!quit);//quit game bool



		//deallocate memory
        delete game;
        delete space;
        delete com;

    return 0;
}
