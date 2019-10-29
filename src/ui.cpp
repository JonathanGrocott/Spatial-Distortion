/***************************************************************
** Program Filename: ui.cpp
** Author: Adam Powell
** Date: 10/27/19
** Description: User Interface functions.
***************************************************************/

#include "ui.hpp"
#include <string>
#include <iostream>
#include "space.hpp"
#include <map>

/*********************************************************************
** Description: display, output file contents
** Input: Space pointer
** Output: none.
*********************************************************************/
void uiDisplay(Space* room){

    clearScreen();

    std::string line;
    std::string location = "Data/Spaces/"+room->getSpaceName()+".txt";
    std::ifstream myfile (location);

    if (myfile.is_open()){          // comfirm file opened
        while ( myfile.good() ){    // read to end of file

            std::getline (myfile, line);

			if(!line.compare("<long_desc>")){
				while(line.compare("</long_desc>"))
				{
					std::getline (myfile , line);
					if(line.compare("</long_desc>"))
						std::cout << line << '\n';
				}
			}
        }
    myfile.close();
    }
    else std::cout << "Unable to open file" << std::endl; 
}

/*********************************************************************
** Description: display, output exits
** Input: Space pointer
** Output: none.
*********************************************************************/
void exitDisplay(Space* temp){

    std::cout << temp->findExits() << std::endl;
}

/*********************************************************************
** Description: helper function that clears the screen
** Input: none
** Output: none
*********************************************************************/
void clearScreen(){
    printf("\e[2J\e[H");
}