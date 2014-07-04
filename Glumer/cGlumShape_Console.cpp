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
   : cGlumShapeBase(id), mHUD_Colour(hud_colour), mLeftToRightIndex(0), mTopToBottomIndex(0), 
   /*mActionThread0(), mActionThread1(), mActionThread2(), mActionThreadIndex(0), */mMessageLines(), mScale(scale), debug_lock()
{
   cObjectMoveableBase::mObject = this;
	mMessageLines.reserve(25);
   /*mActionThread0.Init();
   mActionThread1.Init();
   mActionThread2.Init();*/
}
        
void cGlumShape_Console::EventShow(void)
{
   if (cGlumShapeBase::IsShown() == false) return;

   std::vector<std::string> copyOfLines;
   // lock scope
   {      
      TimerWrapper::cMutexWrapper::Lock lock(&debug_lock);
      copyOfLines.assign(mMessageLines.begin(), mMessageLines.end());
   }

   glPushMatrix();

      glScalef(mScale, mScale, mScale);
      mTopToBottomIndex = 0;
      int end = copyOfLines.size()-1;
      if (end != -1) 
         for (int i = 0; i <= end; i++)
         {
            DrawLine(copyOfLines[i]); 
         }

   glPopMatrix();
}

}