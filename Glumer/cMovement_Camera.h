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
		//float mPitch, mYaw, mRoll;
		void TimerWrapper::cTimerWrapper::EventTimer(void);

	public:
      typedef struct
      {
         cMovement_Camera *ptr;
         bool operator ==(unsigned int id)
	      {
		      return ptr->GetID() == id;
	      }
      } PTR;

		//inline float GetPitch() const { return mPitch; };
		//inline float GetYaw() const { return mYaw; };
		//inline float GetRoll() const { return mRoll; };
		/*inline void FactorySetPitch(float pitch) { mPitch = pitch; };
		inline void FactorySetYaw(float yaw) { mYaw = yaw; };
		inline void FactorySetRoll(float roll) { mRoll = roll; };*/

		void Init(unsigned int callbackDelay)
		{
			TimerWrapper::cTimerWrapper::FactorySetDelay(callbackDelay);
      }

      virtual void Start(cMovementBase *world_offset)
      {
			TimerWrapper::cTimerWrapper::AddThread(this);
		}

		cMovement_Camera(unsigned int id, void *hud_colour) : cGlumShapeBase(id)/*, mPitch(0.0f), mYaw(0.0f), mRoll(0.0f)*/ { };
		~cMovement_Camera(void) {};

		void /*cMovementBase::*/EventPossition(void);
};

}

#endif