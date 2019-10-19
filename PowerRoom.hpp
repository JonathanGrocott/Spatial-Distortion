/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: 
*********************************************************************/

#ifndef PowerRoom_HPP
#define PowerRoom_HPP

#include "Space.hpp"


class PowerRoom : public Space
{
    public:
        PowerRoom();
        virtual ~PowerRoom();

        void locationInteract(Space* cl);
};

#endif // PowerRoom_HPP
