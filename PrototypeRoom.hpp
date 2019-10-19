/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: 
*********************************************************************/

#ifndef PrototypeRoom_HPP
#define PrototypeRoom_HPP

#include "Space.hpp"


class PrototypeRoom : public Space
{
    public:
        PrototypeRoom();
        virtual ~PrototypeRoom();

        void locationInteract(Space* cl);
};

#endif // PrototypeRoom_HPP
