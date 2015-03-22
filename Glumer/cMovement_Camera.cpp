// cMovement_Camera.cpp
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)

#include "cMovement_Camera.h"
#include "cGlumShape_Console.h"
#include <windows.h>
#include <gl/GL.h>

namespace Glumer
{

inline static void DebugOutput(const cMovement_Camera *ref)
{
	/*static char info_line[80];

	sprintf_s(info_line, "RX %2.2f", ref.GetPitch());
	cGlumShape_Console::debug_console->AddMessage(info_line);
			  
	sprintf_s(info_line, "RY %2.2f", ref.GetYaw());
	cGlumShape_Console::debug_console->AddMessage(info_line);

	sprintf_s(info_line, "YZ %2.2f", ref.GetRoll());
	cGlumShape_Console::debug_console->AddMessage(info_line);*/
}

void cMovement_Camera::/*cMovementBase::*/EventPossition(void)
{
	DebugOutput(this);

   if (cMovementBase::GetIgnore())
		return;

   //glRotatef(GetPitch(),1.0f, 0.0f, 0.0f);
	//glRotatef(GetYaw(),	0.0f, 1.0f, 0.0f);
	//glRotatef(GetRoll(),	0.0f, 0.0f, 1.0f);


   float x, y, z;
   float angle, angleInc;
   mOrientation.GetXYZAngleAndIncrement(angle, x, y, z, angleInc);
   glRotatef(-angle, x, y, z);

   mCenter.GetXYZ(x, y, z);
   glTranslatef(-x, -y, -z);
}

void cMovement_Camera::EventTimer(void)
{
	cGlumShapeBase::AddToCenter(mDirection);
	cGlumShapeBase::mOrientation.AddToAngle(mOrientation.GetAngleIncrement());
	if (mOrientation.GetAngle() > 359.9999f)
	{
		mOrientation.FactorySetAngle(0.0f);
	}
}

}