// cObjectMoveableBase.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef COBJECTMOVEABLEBASE_H
#define COBJECTMOVEABLEBASE_H

#include "cObjectBase.h"
#include "cMovementBase.h"
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <gl/GL.h>

namespace Glumer
{

/*
composite class to encapsulate a stack of movement actions against a single object
*/
class cObjectMoveableBase : public cObjectBase
{
protected:
    cObjectBase *mObject;
    static inline void FireEventPossition(cMovementBase::PTR &envoke_me) { envoke_me.ptr->EventPossition(); }
    std::vector<cMovementBase::PTR> mMovementStack;

public:
    typedef struct 
    {
      cObjectMoveableBase *ptr;
      /*bool operator ==(const cObjectMoveableBase::PTR *other) const
      {
         return ptr == other->ptr;
      }*/
    } PTR;
                
    cObjectMoveableBase(cObjectBase *make_moveable=NULL);
    virtual ~cObjectMoveableBase(void) {};

    inline void EventPossition(void)
    {
       std::for_each(mMovementStack.begin(), mMovementStack.end(), FireEventPossition);
    }
    void EventShow(void);
	 inline static void FireEventShow(cObjectMoveableBase::PTR &showme)
	 {
		glLoadIdentity();
		showme.ptr->cObjectMoveableBase::EventShow();
	 }

    /* bool operator ==( cObjectMoveableBase::PTR &other, cObjectMoveableBase::PTR &me) const
     {
         return me.ptr == other.ptr;
     }*/
};

}

#endif