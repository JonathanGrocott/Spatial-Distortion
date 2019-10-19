/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: 
*********************************************************************/

#ifndef Office_HPP
#define Office_HPP

#include "Space.hpp"


class Office : public Space
{
    public:
        Office();
        virtual ~Office();

        void locationInteract(Space* cl);
};

#endif // Office_HPP
