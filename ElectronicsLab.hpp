/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: 
*********************************************************************/

#ifndef ElectronicsLab_HPP
#define ElectronicsLab_HPP

#include "Space.hpp"


class ElectronicsLab : public Space
{
    public:
        ElectronicsLab();
        virtual ~ElectronicsLab();

        void locationInteract(Space* cl);
};

#endif // ElectronicsLab_HPP
