// cGlumShapeBase.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CGLUMSHAPEBASE_H
#define CGLUMSHAPEBASE_H

#include "cGluperCenter.h"
#include "cGluperDirection.h"
#include "cGluperOrientation.h"
#include <SDL.h>

namespace Glumer
{

	class cGlumShapeBase
	{
	protected:
		cGluperCenter mCenter;
		cGluperDirection mDirection;
		cGluperOrientation mOrientation;

		cGluperCenter mBufferedCenter;
		cGluperOrientation mBufferedOrientation;
		cGluperDirection mBufferedDirection;

		unsigned int mID;
		bool mInvisible;
		bool mMarkedForDelete;
		Uint32 mDeletedAt;
		bool mIsBuffered;

	public:
		virtual void AnimationStart() = 0;
		virtual void AnimationStop() = 0;
		static const unsigned int MOVEMENT_UPDATE_INTERVAL = 12;
		static const unsigned int MOVEMENT_UPDATE_INTERVAL_END = 21;
		static const unsigned int MOVEMENT_UPDATE_INTERVAL_STEP = 3;
		inline bool IsAnimated()
		{
			return mDirection.IsAnimated() || mOrientation.IsAnimated();
		}
		inline bool IsShown(void) const { return (mInvisible || mMarkedForDelete) == false; }
		virtual void Start(cMovementBase *world_offset) { };
		virtual void Stop(void) { };
		static inline void NormaliseDirection(void)
		{
		};

		static inline bool NormaliseValue(const float Normal, float &coord)
		{
			if (coord > Normal)
			{
				coord = Normal;
				return true;
			}
			if (coord < -Normal)
			{
				coord = -Normal;
				return true;
			}
			return false;
		}
		void NormaliseOrientation(void)
		{
			static const float minAngle = -360.0f;
			static const float maxAngle = 360.0f;
			static const float Normal = 1.0f;

			float angle, x, y, z, angleIncrement;
			mOrientation.GetXYZAngleAndIncrement(angle, x, y, z, angleIncrement);

			if (angle >= maxAngle)
				mOrientation.FactorySetAngle(minAngle);
			else if (angle <= minAngle)
				mOrientation.FactorySetAngle(maxAngle);

			if (NormaliseValue(Normal, x) || NormaliseValue(Normal, y) || NormaliseValue(Normal, z))
				mOrientation.FactorySetAngleXYZ(x, y, z);
		}

		inline void CopyBuffer(void)
		{
			if (!mIsBuffered) return;
			if (mMarkedForDelete) return;
			float x, y, z;
			mCenter.GetXYZ(x, y, z);
			mBufferedCenter.FactorySetXYZ(x, y, z);

			//NormaliseDirection();
			//NormaliseOrientation();

			float angle, angleInc;
			mOrientation.GetXYZAngleAndIncrement(angle, x, y, z, angleInc);
			mBufferedOrientation.FactorySetAngleXYZ(x, y, z);
			mBufferedOrientation.FactorySetAngle(angle);
			mBufferedOrientation.FactorySetAngleIncrement(angleInc);

			mDirection.GetXYZ(x, y, z);
			mBufferedDirection.FactorySetXYZ(x, y, z);
		}

		/*inline void GetCoords(float &x, float &y, float &z) const 
		{ 
			if (mMarkedForDelete == false) 
				mCenter.GetXYZ(x, y, z); 
			else x = y = z = 0.0f; 

			cObjectMoveableBase* anchor1 = mCenter.GetAnchor1();

			if (anchor1 != NULL)
			{
				cGlumShapeBase *base = (cGlumShapeBase *)(mCenter.GetAnchor1()->GetGlumShapeBase());
				float x2, y2, z2;
				base->GetCenter().GetXYZ(x2, y2, z2);
				x += x2;
				y += y2;
				z += z2;
			}
			else if (mCenter.GetAnchor2() != NULL)
			{
				cGlumShapeBase *base = dynamic_cast<cGlumShapeBase *>(mCenter.GetAnchor2());
				
				float x2, y2, z2;
				base->GetCenter().GetXYZ(x2, y2, z2);
				x += x2;
				y += y2;
				z += z2;
			}
		};*/
		inline const cGluperCenter &GetCenter(void) const { return mIsBuffered ? mBufferedCenter : mCenter; };
		inline cGluperCenter &FactoryGetCenter(void) { cGluperCenter &center = mCenter; return center; };
		inline void AddToCenter(const cGluperDirection &addMe) { mCenter += addMe; };

		inline const cGluperDirection &GetDirection(void) const { return mIsBuffered ? mBufferedDirection : mDirection; };
		inline cGluperDirection &FactoryGetDirection(void) { cGluperDirection &direction = mDirection; return direction; };

		inline const cGluperOrientation &GetOrientation(void) const { return mIsBuffered ? mBufferedOrientation : mOrientation; };
		inline cGluperOrientation &FactoryGetOrientation(void) { cGluperOrientation &orientation = mOrientation; return orientation; };

		inline unsigned int GetID() const { return mID; };
		inline void FactorySetID(unsigned int id) { mID = id; };

		inline bool IsInvisible() const { return mInvisible; };
		inline void FactorySetInvisible(bool ic) { mInvisible = ic; };

		inline bool IsMarkedForDelete() const { return mMarkedForDelete; };
		inline void FactorySetDeleted(bool ic) { mMarkedForDelete = ic; mDeletedAt = SDL_GetTicks(); };
		inline Uint32 GetDeleteAt(void) const { return mDeletedAt; }

		typedef struct
		{
			cGlumShapeBase *ptr;
		} PTR;

		cGlumShapeBase(unsigned int id = 0) : mCenter(), mDirection(), mOrientation(), mID(id), mInvisible(false), mMarkedForDelete(false), mDeletedAt(0), mIsBuffered(false){};
		virtual ~cGlumShapeBase(void) {};
	};

}

#endif