/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: 
*********************************************************************/

#ifndef Hallway_HPP
#define Hallway_HPP

#include "Space.hpp"


class Hallway : public Space
{
    public:
        Hallway();
        virtual ~Hallway();

        void locationInteract(Space* cl);
};

#endif // Hallway_HPP
