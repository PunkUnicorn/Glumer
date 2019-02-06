// cGlumShape_SwitchGadget.cpp
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)

#include "cGlumShape_SwitchGadget.h"

namespace Glumer
{
// static
void cGlumShape_SwitchGadget::PistonShift_OnOffPulse(cTimer_PistonMotion *sender, cGlumShapeBase *common, float value, bool reset)
{
	cGlumShape_SwitchGadget *me = (cGlumShape_SwitchGadget *)common;

	if (me->mHaveSaved == false)
	{
		// save the scale because we mess around with this for effect
		me->mSavedX = me->mScale.mScaleX;
		me->mSavedY = me->mScale.mScaleY; // wants to be pushed and popped
		me->mSavedZ = me->mScale.mScaleZ;
		me->mHaveSaved = true;
	}
	me->mScale.mScaleX = me->mSavedX + value;
	me->mScale.mScaleY = me->mSavedY + value;
	me->mScale.mScaleZ = me->mSavedZ + value;
}

// static 
void cGlumShape_SwitchGadget::PistonShift_OnOffClick(cTimer_PistonMotion *sender, cGlumShapeBase *common, float value, bool reset)
{
	cGlumShape_SwitchGadget *me = (cGlumShape_SwitchGadget *)common;

	/*static char info_line[80];
	sprintf_s(info_line, "CLK%i %2.2f", me->GetName(), value);
	cGlumShape_Console::debug_console->AddMessage(info_line);*/
			
	if (me->mHaveSaved == false)
	{
		// save the scale because we mess around with this for effect
		me->mSavedX = me->mScale.mScaleX;
		me->mSavedY = me->mScale.mScaleY;
		me->mSavedZ = me->mScale.mScaleZ;
		me->mHaveSaved = true;				
	}
			
	me->mScale.mScaleX = me->mSavedX + value;
	me->mScale.mScaleY = me->mSavedY + value;
	me->mScale.mScaleZ = me->mSavedZ + value;					

	if (sender->TopCount() == 1)
	{
		sender->SetPause(true);
	}
}

// static 
void cGlumShape_SwitchGadget::PistonShift_ButtonClick(cTimer_PistonMotion *sender, cGlumShapeBase *common, float value, bool reset)
{
	cGlumShape_SwitchGadget *me = (cGlumShape_SwitchGadget *)common;

	/*static char info_line[80];
	sprintf_s(info_line, "CLK%i %2.2f", me->GetName(), value);
	cGlumShape_Console::debug_console->AddMessage(info_line);*/

	if (me->mHaveSaved == false)
	{
		// save the scale because we mess around with this for effect
		me->mSavedX = me->mScale.mScaleX;
		me->mSavedY = me->mScale.mScaleY;
		me->mSavedZ = me->mScale.mScaleZ;
		me->mHaveSaved = true;
	} 
			
	if (reset || sender->TopCount() == 1)
	{
		me->mScale.mScaleX = me->mSavedX;
		me->mScale.mScaleY = me->mSavedY;
		me->mScale.mScaleZ = me->mSavedZ;
		sender->SetPause(true);				
		me->mSelected = false;
		return;
	}

	me->mScale.mScaleX = me->mScale.mScaleX + value;
	me->mScale.mScaleY = me->mScale.mScaleY + value;
	me->mScale.mScaleZ = me->mScale.mScaleZ + value;
}

// static 
void cGlumShape_SwitchGadget::PistonShift_ButtonDoesntKnowWhatItIs(cTimer_PistonMotion *sender, cGlumShapeBase *common, float value, bool reset)
{
	cGlumShape_SwitchGadget *me = (cGlumShape_SwitchGadget *)common;

	if (me->mHaveSaved == false)
	{
		// save the scale because we mess around with this for effect
		me->mSavedX = me->mScale.mScaleX;
		me->mSavedY = me->mScale.mScaleY;
		me->mSavedZ /* */ = sender->Max();
		me->mHaveSaved = true;
	}

	if (reset)
	{
		me->mScale.mScaleX = me->mSavedX;
		me->mScale.mScaleY = me->mSavedY;
		me->mSavedZ /* */ = sender->Max();
		return;
	}

	me->mScale.mScaleX = me->mSavedX + (value*2.0f);
	me->mScale.mScaleY = me->mSavedY - (value*2.0f);
				
	if (sender->TopCount() == 3)
	{
		/*static char info_line[80];				
		sprintf_s(info_line, "UMT3%i %i", me->mName, sender->TopCount());
		cGlumShape_Console::debug_console->AddMessage(info_line);*/

		// then dont go back to the bottom, go to the middle
		sender->SetPause(true);
	}
}

void cGlumShape_SwitchGadget::Init(float scale, cTimer_PistonMotion::PistonValueChanged *onpistonchanged, GlumerOnClickedBool *onclickedbool, GlumerOnClicked *onclicked)
{
	mOnClickedBool = onclickedbool;
   mOnClicked = onclicked;
	mOnPistonValueChanged = onpistonchanged;
	mSize = scale;
   //mScale.mScaleX = scale;
   //mScale.mScaleY = scale;
   //mScale.mScaleZ = scale;
}

//void cGlumShape_SwitchGadget::Init(float scale, cTimer_PistonMotion::PistonValueChanged *onpistonchanged, GlumerOnClicked *onclicked)
//{
//	mOnClicked = onclicked;
//   mOnClickedBool = NULL;
//	mOnPistonValueChanged = onpistonchanged;
//}

void cGlumShape_SwitchGadget::EventSelected(void)
{
	mSelected = !mSelected;

	bool startPaused = true;
	if (mSelected)
		startPaused = false;

	mPistonMotion->Reset(minFIXED, maxFIXED, incFIXED, 
		mPistonMotion->CalculateInitialValue(minFIXED, maxFIXED), 
		startPaused);

   if (mOnClickedBool != NULL)
      mOnClickedBool(this->GetID(), mSelected);

	if (mOnClicked != NULL)
      mOnClicked(this->GetID());
}

}