// cGlumShape_Console.cpp
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)

#include "cGlumShape_Console.h"
#include <windows.h>
#include <gl/GL.h>

namespace Glumer
{

// static member variable
//cGlumShape_Console *cGlumShape_Console::debug_console;
//TimerFromBubbles::cMutexWrapper cGlumShape_Console::debug_lock;

cGlumShape_Console::cGlumShape_Console(unsigned int id, cHUD_Colour *hud_colour, float scale/*=1.0f*/) 
   : cGlumShapeBase(id), cObjectMoveableBase(), mHUD_Colour(hud_colour), mLeftToRightIndex(0), mTopToBottomIndex(0), mDelay(cGlumShapeBase::MOVEMENT_UPDATE_INTERVAL), mTimer(NULL),
   /*mActionThread0(), mActionThread1(), mActionThread2(), mActionThreadIndex(0), */mMessageLines(), mScale(scale), debug_lock()
{
   cObjectMoveableBase::mObject = this;
	mMessageLines.reserve(25);


	cMovementBase::PTR ptr;
	ptr.ptr = &mCenter;
	mMovementStack.push_back(ptr);
	ptr.ptr = &mOrientation;
	mMovementStack.push_back(ptr);
}
        


}