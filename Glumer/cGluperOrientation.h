// cGluperOrientation.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CGLUPERORIENTATION_H
#define CGLUPERORIENTATION_H

#include "cMovementBase.h"

namespace Glumer
{

	class cGluperOrientation : public cMovementBase
	{
	private:
		/* the tail is at 0, 0, 0 */
		float mAngleX, mAngleY, mAngleZ;
		float mAngle, mAngleIncrement;

	public:
		inline void GetXYZAngleAndIncrement(float &angle, float &x, float &y, float &z, float &angleIncrement) const
		{
			angle = mAngle;
			x = mAngleX;
			y = mAngleY;
			z = mAngleZ;
			angleIncrement = mAngleIncrement;
		};
		inline float GetAngle(void) const { return mAngle; };
		inline float GetAngleIncrement(void) const { return mAngleIncrement; };

		inline void AddToAngle(float addMe) { mAngle = mAngle + addMe; };
		inline void AddToAngleXYZ(float x, float y, float z) { mAngleX = mAngleX + x; mAngleY = mAngleY + y; mAngleZ = /*Normalise(*/mAngleZ + z/*)*/; };
		inline void AddToAngleIncrement(float addMe) { mAngleIncrement = mAngleIncrement + addMe; };

		inline void FactorySetAngleXYZ(float x, float y, float z) { mAngleX = x; mAngleY = y; mAngleZ = z; };
		inline void FactorySetAngle(float newVal) { mAngle =newVal; };
		inline void FactorySetAngleIncrement(float newVal) { mAngleIncrement =newVal; };

		typedef struct
		{
			cGluperOrientation *ptr;
		} PTR;

		cGluperOrientation(float X = 0.0f, float Y = 0.0f, float Z = 0.0f)
			: mAngleX(X), mAngleY(Y), mAngleZ(Z), mAngle(0.0f), mAngleIncrement(0.0f) {};

		virtual ~cGluperOrientation(void) {};

		void /*cMovementBase::*/EventPossition(void);
	};

}

#endif