// cGluperDirection.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CGLUPERDIRECTION_H
#define CGLUPERDIRECTION_H

#include <algorithm>

namespace Glumer
{

	class cGluperDirection
	{
	private:
		float mX, mY, mZ;

	public:
		inline void GetXYZ(float &x, float &y, float &z) const { x = mX; y = mY; z = mZ; };
		inline void AddToXYZ(float x, float y, float z) { mX += x; mY += y; mZ += z; };
		inline void FactorySetXYZ(float x, float y, float z) { mX = x; mY = y; mZ = z; };

		typedef struct
		{
			cGluperDirection *ptr;
		} PTR;

		cGluperDirection(float x = 0.0f, float y = 0.0f, float z = 0.0f) : mX(x), mY(y), mZ(z) {};
		virtual ~cGluperDirection(void) {};
	};

}

#endif