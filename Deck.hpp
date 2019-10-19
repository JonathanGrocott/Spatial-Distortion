/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: 
*********************************************************************/

#ifndef Deck_HPP
#define Deck_HPP

#include "Space.hpp"


class Deck : public Space
{
    public:
        Deck();
        virtual ~Deck();

        void locationInteract(Space* cl);
};

#endif // Deck_HPP
