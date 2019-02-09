// cGluperScale.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CGLUPERSCALE_H
#define CGLUPERSCALE_H

#include "cMovementBase.h"

namespace Glumer
{

#include <gl/Gl.h>

	class cGluperScale : public cMovementBase
	{
	public:
		typedef struct
		{
			cGluperScale *ptr;
		} PTR;
		float mScaleX;
		float mScaleY;
		float mScaleZ;

		cGluperScale(float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f)
			: cMovementBase(), mScaleX(scaleX), mScaleY(scaleY), mScaleZ(scaleZ) { };

		virtual ~cGluperScale(void) {};

		void /*cMovementBase::*/EventPossition(void)
		{
			if (cMovementBase::GetIgnore())
				return;

			glScalef(mScaleX, mScaleY, mScaleZ);
		}
	};

}

#endif