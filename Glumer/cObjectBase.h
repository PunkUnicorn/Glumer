// cObjectBase.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef COBJECTBASE_H
#define COBJECTBASE_H

namespace Glumer
{

/*
base class to encapsulate OpenGL draw commands
*/
class cObjectBase
{
public:
    cObjectBase(void) {};
    virtual ~cObjectBase(void) {};

    typedef struct 
    {
        cObjectBase *ptr;
    } PTR;

    virtual void EventShow(void) = 0;   //glBegin() .. glEnd()
};

}

#endif