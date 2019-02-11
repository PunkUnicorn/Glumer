// cTimer_PistonMotion.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CTIMER_PISTONMOTION_H
#define CTIMER_PISTONMOTION_H

#include "TimerWrapper.h"
#include "cGlumShapeBase.h"

namespace Glumer
{

	class cTimer_PistonMotion : public TimerWrapper::cTimerWrapper
	{
	public:
		typedef void PistonValueChanged(cTimer_PistonMotion * /*who raised*/, cGlumShapeBase * /*you*/, float /*value*/, bool /*is this a reset callback*/ isReset);

	private:
		cGlumShapeBase *mCommon;

		PistonValueChanged *mOnPistonValueChanged;
		bool mDirectionUp;
		float mMin;
		float mMax;
		float mMid;
		float mIncrement;
		float mCurrentValue;
		bool mPaused;
		int mTopCount;
		int mBottomCount;

	public:
		inline float CalculateInitialValue() { return CalculateInitialValue(mMin, mMax); }
		inline float CalculateInitialValue(float min, float max) { return (mMin + mMax) / 2.0f; }

		inline float Min(void) const { return mMin; }
		inline float Max(void) const { return mMax; }
		inline float Mid(void) const { return mMid; }
		inline float Increment(void) const { return mIncrement; }
		inline int TopCount(void) const { return mTopCount; }
		inline int BottomCount(void) const { return mBottomCount; }
		inline bool GetPause(void) const { return mPaused; }
		inline void SetPause(bool wut) { cTimerWrapper::SetPause(wut); cTimer_PistonMotion::mPaused = wut; }

		inline cTimer_PistonMotion(cGlumShapeBase *common, PistonValueChanged *onpistonchange)
			: mCommon(common), mDirectionUp(true), mMin(0.0f), mMax(0.0f), mMid(0.0f), mIncrement(0.0f),
			mOnPistonValueChanged(onpistonchange), mPaused(true), mCurrentValue(0.0f), mTopCount(0), mBottomCount(0)
		{
			TimerWrapper::cTimerWrapper::FactorySetDelay(60);
			//cTimerWrapper::AddTimer(this);
			TimerWrapper::cTimerWrapper::AddTimer(this);
		}
		inline ~cTimer_PistonMotion(void) {}

		inline void EventTimer(void)
		{
			if (mPaused)
				return;

			mCurrentValue += mDirectionUp ? mIncrement : -mIncrement;

			if (mDirectionUp && mMax < mCurrentValue)
			{
				mDirectionUp = false;
				mTopCount++;
				mCurrentValue = mMax;
			}
			else if (mDirectionUp == false && mCurrentValue < mMin)
			{
				mDirectionUp = true;
				mBottomCount++;
				mCurrentValue = mMin;
			}

			mOnPistonValueChanged(this, mCommon, mCurrentValue, false);
		}

		inline void SetRange(float min, float max, float increment, float initialValue)
		{
			mMin = min;
			mMax = max;
			mIncrement = increment;
			mCurrentValue = initialValue;
		}
		inline void Reset(float min, float max, float increment, float currentValue, bool paused)
		{
			if (!mPaused)
			{
				SetPause(true);
			}

			SetRange(min, max, increment, currentValue);
			mDirectionUp = true;
			mTopCount = 0;
			mBottomCount = 0;

			mOnPistonValueChanged(this, mCommon, mCurrentValue, true);
			SetPause(paused);
		}
	};

}

#endif