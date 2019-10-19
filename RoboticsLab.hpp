/*********************************************************************
** Program Filename: 
** Author: Jonathan Grocott
** Date: 10/17/19
** Description: 
*********************************************************************/

#ifndef RoboticsLab_HPP
#define RoboticsLab_HPP

#include "Space.hpp"


class RoboticsLab : public Space
{
    public:
        RoboticsLab();
        virtual ~RoboticsLab();

        void locationInteract(Space* cl);
};

#endif // RoboticsLab_HPP
