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
	    if(!room->getVisited()) {
	        if(!line.compare("<long_desc>")){
		   while(line.compare("</long_desc>"))
		   {
		       std::getline (myfile , line);
		       if(line.compare("</long_desc>"))
		           std::cout << line << '\n';
		   }
		}
	    }
	    else {
	        if(!line.compare("<short_desc>")){
	           while(line.compare("</short_desc>"))
		   {
		       std::getline (myfile , line);
		       if(line.compare("</short_desc>"))
		           std::cout << line << '\n';
		   }
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
void objectsDisp(Space* room, std::unordered_map<std::string, std::tuple<Item*, Space*, player*>> itemsMap){
	for (auto it = itemsMap.begin(); it != itemsMap.end(); it++) {
		if (!(std::get<1>(it->second)->getSpaceName().compare(room->getSpaceName()))) {
			// If not taken and not hidden 
			if (!(std::get<0>(it->second)->isTaken()) && !(std::get<0>(it->second)->isHidden())) {
				std::cout << it->first << std::endl;
			}
		}
	}
}

/*********************************************************************
** Description: display puzzles in the room
** Input: Space pointer, Puzzles map
** Output: none.
*********************************************************************/
void puzzlesDisp(Space* room, std::unordered_map<std::string, std::tuple<Puzzle*, Space*, player*>> puzzlesMap){
	for (auto it = puzzlesMap.begin(); it != puzzlesMap.end(); it++) {
		if (!(std::get<1>(it->second)->getSpaceName().compare(room->getSpaceName()))) {
			// If not completed
			if (std::get<2>(it->second) == nullptr) {
				std::cout << it->first << std::endl;
			}
		}
	}
}


/*********************************************************************
** Description: helper function that clears the screen
** Input: none
** Output: none
*********************************************************************/
void clearScreen(){
    printf("\e[2J\e[H");
}
