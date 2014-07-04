// cGlumShape_HUDShortRadar.cpp
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)

#include "cGlumShape_HUDShortRadar.h"
#include "cGlumShape_Rock.h"
#include "cGlumShapeFactory_ShapeContainer.h"

namespace Glumer
{

//static member variables
//cHUD_Colour *cGlumShape_HUDShortRadar::mHUDColour;

//  static member function
//void cGlumShape_HUDShortRadar::ShowPingInternal(unsigned int myID, cPolyLetter_Console &text, const cGlumShapeCollisionBase::TRILATERATION &ping)
//{
//	if (ping.mCenterID != myID)
//		return;
//
//	std::vector<cGlumShape_Rock::PTR>::iterator rock_result;
//	
//	bool haveFound = false;
//	rock_result = cGlumShapeFactory_ShapeContainer<cGlumShape_Rock>::Get(ping.mDistanceUnits[0].id, haveFound);
//	if (haveFound == false)
//		return;
//
//	glPushMatrix();
//
//		std::for_each(rock_result->ptr->cObjectMoveableBase::mMovementStack.begin(),
//						rock_result->ptr->cObjectMoveableBase::mMovementStack.end(),
//						rock_result->ptr->cObjectMoveableBase::FireEventPossition);
//
//		float unit = max(rock_result->ptr->GetRadius(), 10.0f);
//
//		ShowCorneredBox(unit);
//
//	glPopMatrix();
//
//	glPushMatrix();
//
//		rock_result->ptr->mOrientation.cMovementBase::SetIgnore(true);
//
//			std::for_each(rock_result->ptr->cObjectMoveableBase::mMovementStack.begin(),
//							rock_result->ptr->cObjectMoveableBase::mMovementStack.end(),
//							rock_result->ptr->cObjectMoveableBase::FireEventPossition);
//
//		rock_result->ptr->mOrientation.cMovementBase::SetIgnore(false);
//
//		//glRotatef(180.0f, -1.0f, 0.0f, 0.0f);
//		//rock_result->ptr->cObjectMoveableBase::mMovementStack[0].ptr->EventPossition();
//		//glRotatef(180.0f, -1.0f, 0.0f, 0.0f);
//		char strbuf[80];
//		sprintf_s(strbuf, "%u", ping.mDistanceUnits[0].id);
//		text.SetMessage(strbuf);
//
//		glTranslatef(0.0f, -rock_result->ptr->GetRadius(), 0.0f);
//		text.EventShow();
//				
//		/*static char info_line[80];
//
//		mRockInfoPtr->mMessageLines.clear();
//		sprintf(info_line, "%s", "WARNING");
//		mRockInfoPtr->mMessageLines.push_back(info_line);
//
//		sprintf(info_line, "X %2.0f", rock_result->ptr->mCenter.mX);
//		mRockInfoPtr->mMessageLines.push_back(info_line);
//
//		sprintf(info_line, "Y %2.0f", rock_result->ptr->mCenter.mY);
//		mRockInfoPtr->mMessageLines.push_back(info_line);
//
//		sprintf(info_line, "Z %2.0f", rock_result->ptr->mCenter.mZ);
//		mRockInfoPtr->mMessageLines.push_back(info_line);
//
//		glTranslatef(0.0f, -rock_result->ptr->mRadius, 0.0f);
//		mRockInfoPtr->EventShow();*/
//	glPopMatrix();
//
//}

// static member function
const void cGlumShape_HUDShortRadar::ShowCenter(float radius)
{
	float unit = radius;
	float sub_unit = unit/2;

	ShowCorneredBox(unit);
}

cGlumShape_HUDShortRadar::cGlumShape_HUDShortRadar(unsigned int id, cHUD_Colour *hud_colour) 
	: cGlumShapeBase(id),  
	//mWorldOffset(NULL), 
	mScale(0.0f),
	mRange(0.0f),
	mOff(false),
	//mCollisionResultsCopy(),
	mPingText(0, hud_colour, 100.0f),
	mHUDColour(hud_colour)
{
	//mCollisionResultsCopy.reserve(cGlumShapeCollisionEngine::RESERVE_COLLISIONRESULTS);

	cObjectMoveableBase::mObject = this;

	cMovementBase::PTR ptr = { &mCenter} ;
//	ptr.ptr = &mCenter;
	mMovementStack.push_back(ptr);
	ptr.ptr = &mOrientation;
	mMovementStack.push_back(ptr);
}

void cGlumShape_HUDShortRadar::Init(float range, float scale)//, cMovementBase *world_offset)
{
	mScale = scale;
	//cGlumShapeBase::FactorySetRadius(range/2.0f);
	mRange = range;
	//mWorldOffset = world_offset;
}

void cGlumShape_HUDShortRadar::EventShow(void)
{
	/*std::vector<cGlumShapeCollisionBase::TRILATERATION>::iterator result;		*/	

	glPushMatrix();

		glScalef(mScale, mScale, mScale); 
		ShowCenter(mRange);
		ShowCorneredBox(10.0f);

		//if (mOff == false)
		//	if (mCollisionResultsCopy.size() > 0)			
		//		std::for_each(mCollisionResultsCopy.begin(), mCollisionResultsCopy.end(), 
		//			ShowPing(this->cGlumShapeCollisionBase::GetID(), mPingText));
					
	glPopMatrix();
}

}