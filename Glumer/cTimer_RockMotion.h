// cTimer_RockMotion.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CTIMER_ROCKMOTION_H
#define CTIMER_ROCKMOTION_H

#include "TimerWrapper.h"
#include "cGlumShapeBase.h"
#include "cMovement_Camera.h"
#include <vector>

namespace Glumer
{

// motion pump
class cTimer_RockMotion : public TimerWrapper::cTimerWrapper
{
	private:
		cGlumShapeBase *mCommon; 

	public:
		typedef struct
		{
			cTimer_RockMotion *ptr;
		} PTR;

		inline cTimer_RockMotion(cGlumShapeBase *common, unsigned int delay) 
         : mCommon(common)
		{
			TimerWrapper::cTimerWrapper::FactorySetDelay(delay);
			TimerWrapper::cTimerWrapper::AddTimer(this);
		};

		inline ~cTimer_RockMotion(void) {}

		void EventTimer(void);
};

}

#endif