// cGlumShapeFactory.cpp
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)

#include "cGlumShapeFactory.h"
#include "cObjectMoveableBase.h"

namespace Glumer
{

	// template instances
	cGlumShapeFactory_ShapeContainer<cGlumShape_RegularPolyhedron> cGlumShapeFactory::mRegularPolyhedron;
	template<> std::vector<cGlumShape_RegularPolyhedron::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_RegularPolyhedron>::mList;
	template<> std::vector<cGlumShape_RegularPolyhedron::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_RegularPolyhedron>::mRecycleBin;
	template<> TimerWrapper::cMutexWrapper cGlumShapeFactory_ShapeContainer<cGlumShape_RegularPolyhedron>::mReadWriteLock;

	cGlumShapeFactory_ShapeContainer<cGlumShape_SwitchGadget> cGlumShapeFactory::mSwitchGadget;
	template<> std::vector<cGlumShape_SwitchGadget::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_SwitchGadget>::mList;
	template<> std::vector<cGlumShape_SwitchGadget::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_SwitchGadget>::mRecycleBin;
	template<> TimerWrapper::cMutexWrapper cGlumShapeFactory_ShapeContainer<cGlumShape_SwitchGadget>::mReadWriteLock;

	// cGlumShapeFactory_ShapeContainer<cGlumShape_HUDShortRadar> cGlumShapeFactory::mRadar;
	// std::vector<cGlumShape_HUDShortRadar::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_HUDShortRadar>::mList; 
	   // std::vector<cGlumShape_HUDShortRadar::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_HUDShortRadar>::mRecycleBin;
	// TimerWrapper::cMutexWrapper cGlumShapeFactory_ShapeContainer<cGlumShape_HUDShortRadar>::mReadWriteLock;

	// cGlumShapeFactory_ShapeContainer<cGlumShape_HUDLongRadar> cGlumShapeFactory::mHud;
	// std::vector<cGlumShape_HUDLongRadar::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_HUDLongRadar>::mList;
	// std::vector<cGlumShape_HUDLongRadar::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_HUDLongRadar>::mRecycleBin; 
	// TimerWrapper::cMutexWrapper cGlumShapeFactory_ShapeContainer<cGlumShape_HUDLongRadar>::mReadWriteLock;

	cGlumShapeFactory_ShapeContainer<cGlumShape_Console> cGlumShapeFactory::mConsole;
	template<> std::vector<cGlumShape_Console::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_Console>::mList;
	template<> std::vector<cGlumShape_Console::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_Console>::mRecycleBin;
	template<> TimerWrapper::cMutexWrapper cGlumShapeFactory_ShapeContainer<cGlumShape_Console>::mReadWriteLock;

	cGlumShapeFactory_ShapeContainer<cMovement_Camera> cGlumShapeFactory::mCamera;
	template<> std::vector<cMovement_Camera::PTR> cGlumShapeFactory_ShapeContainer<cMovement_Camera>::mList;
	template<> std::vector<cMovement_Camera::PTR> cGlumShapeFactory_ShapeContainer<cMovement_Camera>::mRecycleBin;
	template<> TimerWrapper::cMutexWrapper cGlumShapeFactory_ShapeContainer<cMovement_Camera>::mReadWriteLock;


	cGlumShapeFactory::cGlumShapeFactory(void) : mNextFreeID(0), mCurrentCamera(NULL),
		mFactoryLock(),
		mFactoryState(RESERVE_FACTORYLIST), 
		mIsBuffered(false)
	{

	}

	void cGlumShapeFactory::Start(unsigned int cameraId)
	{
		bool found = false;
		cMovement_Camera::PTR camera = *(mCamera.Get(cameraId, found));
		if (found)
		{
			mCurrentCamera = camera.ptr;
			camera.ptr->Init(cGlumShapeBase::MOVEMENT_UPDATE_INTERVAL);
			camera.ptr->Start(NULL);
		}
		mRegularPolyhedron.Start(mCurrentCamera);
		mSwitchGadget.Start(mCurrentCamera);
		mConsole.Start(mCurrentCamera);
	}

	void cGlumShapeFactory::DoubleBufferCoordinates(void)
	{
		if (this->mIsBuffered == false) return;

		TimerWrapper::cMutexWrapper::Lock lock(mFactoryState.GetGlumShapeMapLock());
		std::vector<unsigned int>::iterator renderId = mFactoryState.GetGlumShapeList().begin();
		for (; renderId != mFactoryState.GetGlumShapeList().end(); renderId++)
		{
			cGlumShapeBase::PTR result = mFactoryState.GetGlumShapeMap()[*renderId];
			if (result.ptr == NULL) continue;

			result.ptr->CopyBuffer();
		}
	}

	void cGlumShapeFactory::DrawScene(cHUD_Colour *hud_colour)
	{
		std::for_each(mFactoryState.GetDrawList().begin(), mFactoryState.GetDrawList().end(),
			cObjectMoveableBase::FireEventShow);
	}

	cGlumShape_RegularPolyhedron::PTR cGlumShapeFactory::CreateRegularPolyhedron(cHUD_Colour *hud_colour, PolyhedronType type, float scale, bool start, Glumer::GlumerOnClicked *onClicked)
	{
		cGlumShape_RegularPolyhedron::PTR new_one;
		new_one = mRegularPolyhedron.CreateNew(++mNextFreeID, hud_colour);
		new_one.ptr->FactorySetShape(type);
		new_one.ptr->FactorySetOnCliced(onClicked);

		static unsigned int intervalDelay = cGlumShapeBase::MOVEMENT_UPDATE_INTERVAL;
		intervalDelay += cGlumShapeBase::MOVEMENT_UPDATE_INTERVAL_STEP;
		new_one.ptr->FactorySetDelay(intervalDelay);
		if (intervalDelay == cGlumShapeBase::MOVEMENT_UPDATE_INTERVAL_END)
			intervalDelay = cGlumShapeBase::MOVEMENT_UPDATE_INTERVAL;


		new_one.ptr->FactorySetRadius(scale);

		cGlumShapeBase::PTR gsbPimp = { (cGlumShapeBase*)new_one.ptr };
		cObjectMoveableBase::PTR pimp = { dynamic_cast<cObjectMoveableBase *>(new_one.ptr) };
		if (pimp.ptr == NULL) throw - 123456;
		mFactoryState.DrawListAdd(pimp, new_one.ptr->GetID());
		mFactoryState.FactoryListAdd(gsbPimp);
		mFactoryState.SelectableDrawListAdd(new_one.ptr, new_one.ptr->GetID());

		return new_one;
	}

	cGlumShape_RegularPolyhedron::PTR cGlumShapeFactory::CreateGLCommand(cHUD_Colour *hud_colour, float scale, int GL_BEGIN_MODE_TYPE, float floats[], unsigned int floatCount, Glumer::GlumerOnClicked *onClicked)
	{
		cGlumShape_RegularPolyhedron::PTR new_one;
		new_one = mRegularPolyhedron.CreateNew(++mNextFreeID, hud_colour);
		new_one.ptr->FactorySetShape(cPolyhedronType::GLCommand);
		new_one.ptr->FactorySetGlCommand(GL_BEGIN_MODE_TYPE, floats, floatCount);
		new_one.ptr->FactorySetOnCliced(onClicked);

		static unsigned int intervalDelay = cGlumShapeBase::MOVEMENT_UPDATE_INTERVAL;
		intervalDelay += intervalDelay += cGlumShapeBase::MOVEMENT_UPDATE_INTERVAL_STEP;
		new_one.ptr->FactorySetDelay(intervalDelay);
		if (intervalDelay == cGlumShapeBase::MOVEMENT_UPDATE_INTERVAL_END)
			intervalDelay = cGlumShapeBase::MOVEMENT_UPDATE_INTERVAL;

		new_one.ptr->FactorySetRadius(scale);

		cGlumShapeBase::PTR gsbPimp = { (cGlumShapeBase*)new_one.ptr };
		cObjectMoveableBase::PTR pimp = { dynamic_cast<cObjectMoveableBase *>(new_one.ptr) };
		if (pimp.ptr == NULL) throw - 123456;
		mFactoryState.DrawListAdd(pimp, new_one.ptr->GetID());
		mFactoryState.FactoryListAdd(gsbPimp);
		mFactoryState.SelectableDrawListAdd(new_one.ptr, new_one.ptr->GetID());

		return new_one;
	}

	cGlumShape_RegularPolyhedron::PTR cGlumShapeFactory::CreateGLCompiledName(cHUD_Colour *hud_colour, float scale, unsigned int compiledName, Glumer::GlumerOnClicked *onClicked)
	{
		cGlumShape_RegularPolyhedron::PTR new_one;
		new_one = mRegularPolyhedron.CreateNew(++mNextFreeID, hud_colour);
		new_one.ptr->FactorySetShape(cPolyhedronType::GlCompiledName);
		new_one.ptr->FactorySetGlCompiledName(compiledName);
		new_one.ptr->FactorySetOnCliced(onClicked);

		static unsigned int intervalDelay = cGlumShapeBase::MOVEMENT_UPDATE_INTERVAL;
		intervalDelay += intervalDelay += cGlumShapeBase::MOVEMENT_UPDATE_INTERVAL_STEP;;
		new_one.ptr->FactorySetDelay(intervalDelay);
		if (intervalDelay == cGlumShapeBase::MOVEMENT_UPDATE_INTERVAL_END)
			intervalDelay = cGlumShapeBase::MOVEMENT_UPDATE_INTERVAL;

		new_one.ptr->FactorySetRadius(scale);

		cGlumShapeBase::PTR gsbPimp = { (cGlumShapeBase*)new_one.ptr };
		cObjectMoveableBase::PTR pimp = { dynamic_cast<cObjectMoveableBase *>(new_one.ptr) };
		if (pimp.ptr == NULL) throw - 123456;
		mFactoryState.DrawListAdd(pimp, new_one.ptr->GetID());
		mFactoryState.FactoryListAdd(gsbPimp);
		mFactoryState.SelectableDrawListAdd(new_one.ptr, new_one.ptr->GetID());

		return new_one;
	}

	cGlumShape_SwitchGadget::PTR cGlumShapeFactory::CreateSwitchGadget(cHUD_Colour *hud_colour, float scale, cTimer_PistonMotion::PistonValueChanged *onpistonchange, GlumerOnClickedBool *onclickedbool, GlumerOnClicked *onclicked)
	{
		cGlumShape_SwitchGadget::PTR new_one;
		new_one = mSwitchGadget.CreateNew(++mNextFreeID, hud_colour);
		new_one.ptr->Init(scale, onpistonchange, onclickedbool, onclicked);

		cGlumShapeBase::PTR gsbPimp = { (cGlumShapeBase*)new_one.ptr };
		cObjectMoveableBase::PTR pimp = { dynamic_cast<cObjectMoveableBase *>(new_one.ptr) };

		mFactoryState.DrawListAdd(pimp, new_one.ptr->GetID());
		mFactoryState.FactoryListAdd(gsbPimp);

		mFactoryState.SelectableDrawListAdd(new_one.ptr, new_one.ptr->GetID());
		return new_one;
	}

	cGlumShape_Console::PTR cGlumShapeFactory::CreateConsole(cHUD_Colour *hud_colour, float scale)
	{
		cGlumShape_Console::PTR new_one;
		new_one = mConsole.CreateNew(++mNextFreeID, hud_colour);
		cGlumShapeBase::PTR gsbPimp = { (cGlumShapeBase*)new_one.ptr };
		cObjectMoveableBase::PTR pimp = { dynamic_cast<cObjectMoveableBase *>(new_one.ptr) };
		new_one.ptr->SetScale(scale);
		mFactoryState.DrawListAdd(pimp, new_one.ptr->GetID());
		mFactoryState.FactoryListAdd(gsbPimp);
		return new_one;
	}

	cMovement_Camera::PTR cGlumShapeFactory::CreateCamera(void)
	{
		cMovement_Camera::PTR new_one;
		new_one = mCamera.CreateNew(++mNextFreeID, NULL);
		cGlumShapeBase::PTR gsbPimp = { (cGlumShapeBase*)new_one.ptr };

		mFactoryState.FactoryListAdd(gsbPimp);
		return new_one;
	}

}