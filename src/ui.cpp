/***************************************************************
** Program Filename: ui.cpp
** Author: Adam Powell
** Date: 10/27/19
** Description: User Interface functions.
***************************************************************/

#include "ui.hpp"
#include <string>
#include <vector>
#include <iostream>
#include "space.hpp"
#include <map>

/*********************************************************************
** Description: display, output file contents
** Input: Space pointer
** Output: none.
*********************************************************************/
void uiDisplay(Space* room){

    //clearScreen();

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
** Description: display objects in the room
** Input: Space pointer, Items map
** Output: none.
*********************************************************************/
void objectsDisp(Space* room, std::unordered_map<std::string, Item*> items){
	for (auto it = items.begin(); it != items.end(); it++) {
		if (!it->second->getBegLoc()->getSpaceName().compare(room->getSpaceName())) {
			if (!it->second->isTaken()) {
				std::cout << it->second->getItemName() << std::endl;
			}
		}
	}
	std::cout << std::endl;
}



/*********************************************************************
** Description: helper function that clears the screen
** Input: none
** Output: none
*********************************************************************/
void clearScreen(){
    printf("\e[2J\e[H");
}
