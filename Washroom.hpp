/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: 
*********************************************************************/

#ifndef Washroom_HPP
#define Washroom_HPP

#include "Space.hpp"


class Washroom : public Space
{
    public:
        Washroom();
        virtual ~Washroom();

        void locationInteract(Space* cl);
};

#endif // Washroom_HPP
