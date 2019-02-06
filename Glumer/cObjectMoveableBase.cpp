// cObjectMoveableBase.cpp
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)

#include "cObjectMoveableBase.h"
#include <SDL.h>

namespace Glumer
{

/*
composite class to encapsulate a stack of movement actions against a single object
*/
cObjectMoveableBase::cObjectMoveableBase(cObjectBase *make_moveable) : mObject(make_moveable), mMovementStack() 
{
	mMovementStack.reserve(5);
}

void cObjectMoveableBase::EventShow(void)
{
   EventPossition();
   glPushAttrib(GL_CURRENT_BIT);
   mObject->EventShow();
   glPopAttrib();

}

}