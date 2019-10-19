/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: 
*********************************************************************/

#ifndef GeneratorRoom_HPP
#define GeneratorRoom_HPP

#include "Space.hpp"


class GeneratorRoom : public Space
{
    public:
        GeneratorRoom();
        virtual ~GeneratorRoom();

        void locationInteract(Space* cl);
};

#endif // GeneratorRoom_HPP
