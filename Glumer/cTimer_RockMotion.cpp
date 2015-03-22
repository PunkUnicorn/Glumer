// cTimer_RockMotion.cpp
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)

#include "cTimer_RockMotion.h"

namespace Glumer
{

void cTimer_RockMotion::EventTimer(void)
{
	mCommon->AddToCenter(mCommon->GetDirection());

	mCommon->FactoryGetOrientation().AddToAngle(mCommon->GetOrientation().GetAngleIncrement());
	if (mCommon->GetOrientation().GetAngle() > 359.9999f)
	{
		mCommon->FactoryGetOrientation().FactorySetAngle(0.0f);
	}
}

}