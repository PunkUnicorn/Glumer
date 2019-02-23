// cGlumShapeFactory.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CGLUMSHAPEFACTORY_H
#define CGLUMSHAPEFACTORY_H

#include "cGlumShapeFactory_ShapeContainer.h"
#include "cGlumShape_Rock.h"
#include "cGlumShape_RegularPolyhedron.h"
//#include "cGlumShape_HUDLongRadar.h"
//#include "cGlumShape_HUDShortRadar.h"
#include "cGlumShape_SwitchGadget.h"
#include "cGlumShape_Console.h"
#include "TimerWrapper.h"
#include "cGlumShapeFactoryState.h"
#include <map>
//#include "BubbleDimensionCracker.h"
#include "BubbleFindCollisions.h"

namespace Glumer
{

class cGlumShapeFactory
{
private:
   unsigned int mNextFreeID;
   cMovementBase *mCurrentCamera;
   cMovement_Camera *mMovement_Camera;
   TimerWrapper::cMutexWrapper mFactoryLock;
   cGlumShapeFactoryState mFactoryState;
   bool mIsBuffered;
   float mClipWidth;
public:
	static const unsigned int RESERVE_FACTORYLIST = 1000;
	//lock factory lock first, then EITHER of
	//    - cGlumShapeFactory_ShapeContainer list locks
	//    - mFactoryState
	inline TimerWrapper::cMutexWrapper *FactoryLock(void) { return &mFactoryLock; } 
	inline bool IsCameraSet(void) const { return mCurrentCamera != NULL; }
	cMovementBase *GetCamera(void) { return mCurrentCamera; }

	// annoying template system seemed like a good idea but was not
	// they are just meant to be containers
	// Important Note: This shit is fucked and bullshit
	static cGlumShapeFactory_ShapeContainer<cGlumShape_RegularPolyhedron> mRegularPolyhedron;
	static cGlumShapeFactory_ShapeContainer<cGlumShape_SwitchGadget> mSwitchGadget;
	static cGlumShapeFactory_ShapeContainer<cGlumShape_Console> mConsole;
	static cGlumShapeFactory_ShapeContainer<cMovement_Camera> mCamera;

	cGlumShapeFactory(void);
	~cGlumShapeFactory(void) {};
	
	unsigned int GetCount(void)
	{
		return mSwitchGadget.ListSize() + 
			mConsole.ListSize() + 
			mCamera.ListSize() + 
			mRegularPolyhedron.ListSize();
	}
	void MakeDrawList(std::vector<Bubbles::cBubbleDimensionCracker::COLLISION_RESULT> source, cGlumShapeFactoryState::DrawList & drawList);
	void GetClosest(std::vector<Bubbles::cBubbleDimensionCracker::TRILATERATION_DATA> &data, float quantity, cMovement_Camera::PTR &center, std::vector<Bubbles::cBubbleDimensionCracker::COLLISION_RESULT> &results);

	void Init(void) { };
	void Start(unsigned int cameraId);

	void DoubleBufferCoordinates(void);
	void DrawScene(cHUD_Colour *hud_colour);
	void SetDrawClipWidth(float clipwidth) { mClipWidth = clipwidth; }
	inline bool HitTest(unsigned int mouse_x, unsigned int mouse_y, unsigned int mouse_z)
	{
		return cSelectableBase::EventClick(mFactoryState.GetSelectableDrawListLock(), mFactoryState.GetSelectableDrawList(), mFactoryState.GetSelectableDrawListIndex(), mouse_x, mouse_y, mouse_z);
	}

	void GetCoords(unsigned int bubbleId, float &X, float &Y, float &Z)
	{
		TimerWrapper::cMutexWrapper::Lock lock(mFactoryState.GetGlumShapeMapLock());

		X=Y=Z=0.0f;
		if (mFactoryState.GetGlumShapeMap().find(bubbleId) == mFactoryState.GetGlumShapeMap().end()) 
			return;

		if (mFactoryState.GetGlumShapeMap()[bubbleId].ptr->IsMarkedForDelete()) 
			return;

		const cGluperCenter &center = mFactoryState.GetGlumShapeMap()[bubbleId].ptr->GetCenter();
		center.GetXYZ(X, Y, Z);
	}

	void GetGlumShapes(std::vector<unsigned int> bubbleIds, std::vector<cGlumShapeBase::PTR> &pimps)
	{
		TimerWrapper::cMutexWrapper::Lock lock(mFactoryState.GetGlumShapeMapLock());

		int size = bubbleIds.size();
		float X, Y, Z;
		for (int i = 0; i < size; i++)
		{
			unsigned int bubbleId = bubbleIds[i];
			X = Y = Z = 0.0f;

			if (mFactoryState.GetGlumShapeMap().find(bubbleId) == mFactoryState.GetGlumShapeMap().end())
				continue;

			if (mFactoryState.GetGlumShapeMap()[bubbleId].ptr->IsMarkedForDelete())
				continue;

			const cGlumShapeBase::PTR &pimp = mFactoryState.GetGlumShapeMap()[bubbleId];
			pimps.push_back(pimp);
			//center.GetXYZ(X, Y, Z);
			//Xv.push_back(X);
			//Yv.push_back(Y);
			//Zv.push_back(Z);
		}
	}


	cGlumShape_RegularPolyhedron::PTR CreateRegularPolyhedron(cHUD_Colour *hud_colour, PolyhedronType type, float scale, bool start, Glumer::GlumerOnClicked *onClicked);
	cGlumShape_RegularPolyhedron::PTR CreateGLCommand(cHUD_Colour *hud_colour, float scale, int GL_BEGIN_MODE_TYPE, float floats[], unsigned int floatCount, Glumer::GlumerOnClicked *onClicked);
	cGlumShape_RegularPolyhedron::PTR CreateGLCompiledName(cHUD_Colour *hud_colour, float scale, unsigned int compiledName, Glumer::GlumerOnClicked *onClicked);

	cGlumShape_SwitchGadget::PTR CreateSwitchGadget(cHUD_Colour *hud_colour, float scale, cTimer_PistonMotion::PistonValueChanged *onpistonchange, GlumerOnClickedBool *onclickedbool, GlumerOnClicked *onclicked);
	cGlumShape_Console::PTR CreateConsole(cHUD_Colour *hud_colour, float scale);
	cMovement_Camera::PTR CreateCamera(void);
   
private:
	void PurgeOne(cGlumShapeBase::PTR deleteMe, unsigned int glumId)
	{
		cObjectMoveableBase::PTR pimp = { dynamic_cast<cObjectMoveableBase *>( deleteMe.ptr ) };
		if (pimp.ptr != NULL) 
		{
			mFactoryState.DrawListDelete(pimp, glumId);
			mFactoryState.SelectableDrawListDelete((cObjectBase*) pimp.ptr, glumId);
		}
		mFactoryState.FactoryListDelete(deleteMe, glumId);

		// find out what it is and delete it from the main list
		bool found = false;

		/* look in different lists for the id */

		/* scope block */
		{
			std::vector<cGlumShape_RegularPolyhedron::PTR>::iterator delIterator 
				= mRegularPolyhedron.Get(glumId, found);

			if (found) 
				mRegularPolyhedron.Delete(delIterator);   
		}

		if (found == false)
		{
			std::vector<cGlumShape_Console::PTR>::iterator delIterator 
				= mConsole.Get(glumId, found);

			if (found) 
				mConsole.Delete(delIterator);    
		}

		deleteMe.ptr->FactorySetDeleted(true);

		//ClearDrawSceneCache();
		/*
		- mGlumShapeList
		- mDrawList
		- mSelectableDrawList
		- mRock.mList
		- mRegularPolyhedronList
		mSwitchGadget.mList
		mRadar.mList
		mHud.mList
		- mConsole.mList
		mCamera.mList
		- DrawSceneCache*/
	}
public:
	inline void Delete(cGlumShapeBase::PTR deleteMe, unsigned int glumId)
	{
		// take it out of the lists too
		deleteMe.ptr->Stop();
		PurgeOne(deleteMe, glumId);
	}

	inline void SetInvisible(cGlumShapeBase::PTR setVisibleId, bool inVisible)
	{
		setVisibleId.ptr->FactorySetInvisible(inVisible);
	}

	inline cGlumShapeBase::PTR Get(unsigned int id)
	{
		TimerWrapper::cMutexWrapper::Lock lock(mFactoryState.GetGlumShapeMapLock());
		static const cGlumShapeBase::PTR dud = { NULL };
		if (mFactoryState.GetGlumShapeMap().find(id) == mFactoryState.GetGlumShapeMap().end()) 
			return dud;
		if (mFactoryState.GetGlumShapeMap()[id].ptr->IsMarkedForDelete()) 
			return dud;
		return mFactoryState.GetGlumShapeMap()[id];
	}
};

}

#endif