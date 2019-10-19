/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: 
*********************************************************************/

#ifndef ImagingRoom_HPP
#define ImagingRoom_HPP

#include "Space.hpp"


class ImagingRoom : public Space
{
    public:
        ImagingRoom();
        virtual ~ImagingRoom();

        void locationInteract(Space* cl);
};

#endif // ImagingRoom_HPP
