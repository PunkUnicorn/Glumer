// cGluperCenter.cpp
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)

#include "cGluperCenter.h"
#include "cObjectMoveableBase.h"
#include <Windows.h>
#include <gl/GL.h>

namespace Glumer
{

	void cGluperCenter::EventPossition(void)
	{
		if (mAnchorRotation != NULL)
			mAnchorRotation->EventPossition();
		else if (mAnchor != NULL)
			mAnchor->EventPossition();


		if (cMovementBase::GetIgnore())
			return;

		glTranslatef(mX, mY, mZ);
	}

}