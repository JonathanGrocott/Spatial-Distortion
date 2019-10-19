/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: 
*********************************************************************/

#ifndef ResearchLab_HPP
#define ResearchLab_HPP

#include "Space.hpp"


class ResearchLab : public Space
{
    public:
        ResearchLab();
        virtual ~ResearchLab();

        void locationInteract(Space* cl);
};

#endif // ResearchLab_HPP
