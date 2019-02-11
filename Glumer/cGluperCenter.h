// cGluperCenter.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CGLUPERCENTER_H
#define CGLUPERCENTER_H

#include "cMovementBase.h"
#include "cGluperDirection.h"
#include "cObjectMoveableBase.h"

namespace Glumer
{

	class cGluperCenter : public cMovementBase
	{
	private:
		float mX, mY, mZ;
		cObjectMoveableBase *mAnchorRotation;
		cGluperCenter *mAnchor;
	public:
		cGluperCenter(float X = 0.0f, float Y = 0.0f, float Z = 0.0f) : mX(X), mY(Y), mZ(Z), mAnchorRotation(0 /*NULL*/), mAnchor(0 /*NULL*/) { };
		inline void SetAnchorMatchingRotationTo(cObjectMoveableBase *anchor) { mAnchorRotation = anchor; };
		inline void SetAnchorTo(cGluperCenter *anchor) { mAnchor = anchor; };
		inline const cObjectMoveableBase* const/*can be null*/GetAnchor1()  { return mAnchorRotation; };
		inline const cMovementBase* const/*can be null*/GetAnchor2() { return mAnchor; };
		virtual ~cGluperCenter(void) {};

		inline void operator += (const cGluperDirection &direction)
		{
			float x, y, z;
			direction.GetXYZ(x, y, z);
			AddToXYZ(x, y, z);
		}

		void GetXYZ(float &x, float &y, float &z) const
		{
			x = mX;
			y = mY;
			z = mZ;
		}

		inline void AddToXYZ(float x, float y, float z) { mX += x; mY += y; mZ += z; };
		inline void FactorySetXYZ(float x, float y, float z) { mX = x; mY = y; mZ = z; };

		typedef struct
		{
			const cGluperCenter *ptr;
		} PTR;

		void /*cMovementBase::*/EventPossition(void);
	};
}

#endif