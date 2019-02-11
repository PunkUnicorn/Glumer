// cMovement_Camera.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CMOVEMENT_CAMERA_H
#define CMOVEMENT_CAMERA_H

#include "cGlumShapeBase.h"
#include "TimerWrapper.h"
#include <math.h>

namespace Glumer
{

	// adapt a cGlumShapeBase type (to give the camera 'game space coordinates') and adapt into a cMovementBase for possitioning world items
	class cMovement_Camera : public cGlumShapeBase, public cMovementBase, private TimerWrapper::cTimerWrapper
	{
	private:
		/*cTimerWrapper::*/void EventTimer(void);

	public:
		typedef struct
		{
			cMovement_Camera *ptr;
			bool operator ==(unsigned int id)
			{
				return ptr->GetID() == id;
			}
		} PTR;

		void Init(unsigned int callbackDelay)
		{
			/*TimerWrapper::cTimerWrapper::*/FactorySetDelay(callbackDelay);
		}

		virtual void Start(cMovementBase *world_offset)
		{
			/*TimerWrapper::cTimerWrapper::*/AddThread(this);
		}

		cMovement_Camera(unsigned int id, void *hud_colour) : cGlumShapeBase(id) { };
		~cMovement_Camera(void) {};

		void /*cMovementBase::*/EventPossition(void);
	};

}

#endif