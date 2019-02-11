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
		mCommon->NormaliseOrientation();
		mCommon->NormaliseDirection();
		//const float threshold = 360.0f;
		//float angle = mCommon->GetOrientation().GetAngle();
		//if (angle >= threshold)
		//{
		//	mCommon->FactoryGetOrientation().FactorySetAngle(angle - threshold);
		//}
	}
}