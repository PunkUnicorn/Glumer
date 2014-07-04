// cGlumShape_HUDLongRadar.cpp
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)

#include "cGlumShape_HUDLongRadar.h"
#include "cGlumShape_HUDShortRadar.h"
#include "cGlumShapeFactory_ShapeContainer.h"
#include "cGlumShape_Rock.h"
#include "cGlumShape_Console.h"

namespace Glumer
{

// static member variables
cHUD_Colour *cGlumShape_HUDLongRadar::mHUDColour;
//cMovement_Camera *cGlumShape_HUDLongRadar::mWorldOffset;

//// local function
//inline static void RotateResults(float pitch, float yaw, float roll, 
//									float x, float y, float z, 
//									float &dx, float &dy, float &dz);


// static member function
//void cGlumShape_HUDLongRadar::ShowPingInternal(unsigned int myID, cPolyLetter_Console &text, cGlumShapeBase::TRILATERATION &ping, cMovementBase *mWorldOffset)
//{
//	//static cPolyLetter_Console text(mHUDColour, 0.5f);
//	std::vector<cGlumShape_Rock::PTR>::iterator rock_result;
//
//	bool haveFound = false;
//	rock_result = cGlumShapeFactory_ShapeContainer<cGlumShape_Rock>::Get(ping.mDistanceUnits[0].id, haveFound);
//	if (haveFound == false)
//		return;
//
//	char strbuf[80];
//	sprintf_s(strbuf, "%u", ping.mDistanceUnits[0].id);
//	text.SetMessage(strbuf);
//
//	float x, y, z;
//	/*x = rock_result->ptr->cGlumShapeCollisionBase::GetCollisionCenter().GetX();
//	y = rock_result->ptr->cGlumShapeCollisionBase::GetCollisionCenter().GetY();
//	z = rock_result->ptr->cGlumShapeCollisionBase::GetCollisionCenter().GetZ();*/
//
//	for (int i=0; i<3; i++)
//	{
//		switch (ping.mDistanceUnits[i].axis)
//		{
//			case cGlumShapeCollisionBase::X:
//				x = ping.mDistanceUnits[i].rel_coord;// - mWorldOffset->mCenter.GetX();
//				break;
//			case cGlumShapeCollisionBase::Y:
//				y = ping.mDistanceUnits[i].rel_coord;// - mWorldOffset->mCenter.GetY();
//				break;
//			case cGlumShapeCollisionBase::Z:
//				z = ping.mDistanceUnits[i].rel_coord;// - mWorldOffset->mCenter.GetZ();
//				break;
//		}
//	}
//
//	glPushMatrix();
//
//		glTranslatef(0.0f, -2.5f, 0.0f);
//
////#pragma warning(L"The function should be in a singleton resource")
//		//cGlumShape_HUDShortRadar::ShowCorneredBox(0.02f);
//		
//
//		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
//
//    dynamic cast needed
//		RotateResults(-mWorldOffset->GetPitch(), -mWorldOffset->GetYaw(), -mWorldOffset->GetRoll(), x, y, z, x, y, z);
//
//		float rad_angle = atan2(x, z);
//		float angle = rad_angle * (180 / (float)PI);
//
//		glRotatef(angle, 0.0f, 0.0f, 1.0f);
//
//		//http://www.codeguru.com/cpp/misc/misc/math/article.php/c10123/#more
//		/*float hypoternuse = z/cos(rad_angle);
//
//		float rad_pitch = -mWorldOffset->mPitch.mAngle / (180 / PI);
//		float opp_affected_by_pitch = hypoternuse * sin(rad_pitch);
//
//		float shift_angle = acos(opp_affected_by_pitch/hypoternuse);
//		shift_angle *= 180 / PI;
//		glRotatef(shift_angle, 0.0f, 0.0f, 1.0f);*/
//
//		glTranslatef(0.0f, -1.5f, 0.0f);
//		text.EventShow();
//
//		/*float unit = 1.0f;
//		glBegin(GL_LINES);
//			glVertex3f(-unit, unit, -unit);
//			glVertex3f( unit, unit, -unit);
//
//			glVertex3f(-unit, unit, -unit);
//			glVertex3f(-unit, unit,  unit);
//
//			glVertex3f( unit, unit, -unit);
//			glVertex3f( unit, unit,  unit);
//
//			glVertex3f(-unit,-unit, -unit);
//			glVertex3f(-unit, unit, -unit);
//
//		glEnd();*/
//	glPopMatrix();
//};

cGlumShape_HUDLongRadar::cGlumShape_HUDLongRadar(unsigned int id, cHUD_Colour *hud_colour) 
	: cGlumShapeBase(id), mScale(0.0f), mWorldOffset(NULL), mRange(0.0f),// mCollisionResultsCopy(), 
	/*mShowCollisionList(), */mText(0, hud_colour, 0.5f) /*mUpdateScale(1.0f), mPulseCount(0)*/, mOff(false)
{
	//mShowCollisionList.reserve(cGlumShapeCollisionEngine::RESERVE_COLLISIONRESULTS);

	mHUDColour = hud_colour;
	cObjectMoveableBase::mObject = this;

	cMovementBase::PTR ptr;
	ptr.ptr = &mCenter;
	mMovementStack.push_back(ptr);
	ptr.ptr = &mOrientation;
	mMovementStack.push_back(ptr);

	mOff = true;
}

void cGlumShape_HUDLongRadar::Init(float range, float scale, cMovementBase *world_offset)
{
	mScale = scale;
	//cGlumShapeBase::FactorySetRadius(range/3.0f);
	mRange = range;
	mWorldOffset = world_offset;

	//cTimerWrapper::mEventCallbackDelay = 500;
	//cTimerWrapper::AddTimer(this);
}

void cGlumShape_HUDLongRadar::/*cObjectBase::*/EventShow(void)
{
   if (cGlumShapeBase::IsShown() == false) return;
	if (mOff)
		return;

	//std::vector<cGlumShapeCollisionBase::TRILATERATION>::const_iterator result;
	//result = mShowCollisionList.cbegin();


	glPushMatrix();

		//if (mShowCollisionList.size() > 0)
			//std::for_each(mShowCollisionList.begin(), mShowCollisionList.end(), ShowRadialPing(this->cGlumShapeCollisionBase::GetID(), mText, mWorldOffset));

	glPopMatrix();
}

//static void RotateResults(float pitch, float yaw, float roll, 
//									float x, float y, float z, 
//									float &dx, float &dy, float &dz)
//{
//   const float PI = 3.14159f;
//	float rX = pitch / (180.0f / (float)PI);
//	float rY = yaw / (180.0f / (float)PI);
//	float rZ = roll / (180.0f / (float)PI);
//
//	//float dx, dy, dz;
//	float anotherX, anotherY, anotherZ;
//			
//	//--> rotate around y-axis				
//	anotherX = (x * cos(rY)) - (z * sin(rY));
//	anotherZ = (x * sin(rY)) + (z * cos(rY));
//
//	//--> rotate around x-axis		
//	dz	 =  (anotherZ * cos(rX)) - (y * sin(rX));
//	anotherY =  (anotherZ * sin(rX)) + (y * cos(rX));
//
//	//--> rotate around z-axis
//	dx	=  (anotherX * cos(rZ)) + (anotherY * sin(rZ));
//	dy	=  (anotherY * cos(rZ)) - (anotherX * sin(rZ));
//};

}