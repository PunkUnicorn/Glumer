// cGluperDirection.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CGLUPERDIRECTION_H
#define CGLUPERDIRECTION_H

//#include <windows.h> /*max*/
#include <algorithm>

namespace Glumer
{

class cGluperDirection
{
	private:
		float mX, mY, mZ;
		//float mVelocity;

	public:
      inline void GetXYZ(float &x, float &y, float &z) const { x = mX; y = mY; z = mZ; };
		//inline float GetVelocity() const { return mVelocity; };
		inline void AddToXYZ(float x, float y, float z) { mX += x; mY += y; mZ += z; };

      //// GET RID OF VELOCITY

		//inline void AddToVelocity(float addMe) { mVelocity += addMe; };
		inline void FactorySetXYZ(float x, float y, float z) { mX = x; mY = y; mZ = z; };
		//inline void FactorySetVelocity(float velocity) { mVelocity = velocity; };

		typedef struct
		{
			cGluperDirection *ptr;
		} PTR;

		cGluperDirection(float x=0.0f, float y=0.0f, float z=0.0f) : mX(x), mY(y), mZ(z) {};
      virtual ~cGluperDirection(void) {};
};

}

#endif