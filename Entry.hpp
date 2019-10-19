/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: 
*********************************************************************/
#ifndef ENTRY_HPP
#define ENTRY_HPP

#include "Space.hpp"


class Entry : public Space
{
    public:
        Entry();
        virtual ~Entry();

        void locationInteract(Space* cl);
};

#endif // ENTRY_HPP
