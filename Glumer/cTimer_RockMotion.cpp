// cTimer_RockMotion.cpp
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)

#include "cTimer_RockMotion.h"

namespace Glumer
{

void cTimer_RockMotion::EventTimer(void)
{
   SDL_Delay(0); // Breath
   mCommon->AddToCenter(mCommon->GetDirection());

   SDL_Delay(0);
   mCommon->FactoryGetOrientation().AddToAngle(mCommon->GetOrientation().GetAngleIncrement());
   SDL_Delay(0);
	if (mCommon->GetOrientation().GetAngle() > 359.9999f)
	{
		mCommon->FactoryGetOrientation().FactorySetAngle(0.0f);
	}
}

}