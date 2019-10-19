/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: 
*********************************************************************/

#ifndef Basement_HPP
#define Basement_HPP

#include "Space.hpp"


class Basement : public Space
{
    public:
        Basement();
        virtual ~Basement();

        void locationInteract(Space* cl);
};

#endif // Basement_HPP
