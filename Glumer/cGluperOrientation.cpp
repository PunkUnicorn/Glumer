// cGluperOrientation.cpp
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)

#include "cGluperOrientation.h"
#include <windows.h>
#include <gl/Gl.h>

namespace Glumer
{

void cGluperOrientation::EventPossition(void)
{
	if (cMovementBase::GetIgnore())
		return;

	glRotatef(mAngle, mAngleX, mAngleY, mAngleZ);
}

}