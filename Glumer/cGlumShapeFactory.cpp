// cGlumShapeFactory.cpp
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)

#include "cGlumShapeFactory.h"
#include "cObjectMoveableBase.h"

namespace Glumer
{

// template instances
cGlumShapeFactory_ShapeContainer<cGlumShape_Rock> cGlumShapeFactory::mRock;
std::vector<cGlumShape_Rock::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_Rock>::mList;
std::vector<cGlumShape_Rock::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_Rock>::mRecycleBin;
TimerWrapper::cMutexWrapper cGlumShapeFactory_ShapeContainer<cGlumShape_Rock>::mReadWriteLock;

cGlumShapeFactory_ShapeContainer<cGlumShape_SwitchGadget> cGlumShapeFactory::mSwitchGadget;
std::vector<cGlumShape_SwitchGadget::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_SwitchGadget>::mList;
std::vector<cGlumShape_SwitchGadget::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_SwitchGadget>::mRecycleBin;
TimerWrapper::cMutexWrapper cGlumShapeFactory_ShapeContainer<cGlumShape_SwitchGadget>::mReadWriteLock;

cGlumShapeFactory_ShapeContainer<cGlumShape_HUDShortRadar> cGlumShapeFactory::mRadar;
std::vector<cGlumShape_HUDShortRadar::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_HUDShortRadar>::mList; 
   std::vector<cGlumShape_HUDShortRadar::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_HUDShortRadar>::mRecycleBin;
TimerWrapper::cMutexWrapper cGlumShapeFactory_ShapeContainer<cGlumShape_HUDShortRadar>::mReadWriteLock;

cGlumShapeFactory_ShapeContainer<cGlumShape_HUDLongRadar> cGlumShapeFactory::mHud;
std::vector<cGlumShape_HUDLongRadar::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_HUDLongRadar>::mList;
std::vector<cGlumShape_HUDLongRadar::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_HUDLongRadar>::mRecycleBin; 
TimerWrapper::cMutexWrapper cGlumShapeFactory_ShapeContainer<cGlumShape_HUDLongRadar>::mReadWriteLock;

cGlumShapeFactory_ShapeContainer<cGlumShape_Console> cGlumShapeFactory::mConsole;
std::vector<cGlumShape_Console::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_Console>::mList; 
std::vector<cGlumShape_Console::PTR> cGlumShapeFactory_ShapeContainer<cGlumShape_Console>::mRecycleBin; 
TimerWrapper::cMutexWrapper cGlumShapeFactory_ShapeContainer<cGlumShape_Console>::mReadWriteLock;

cGlumShapeFactory_ShapeContainer<cMovement_Camera> cGlumShapeFactory::mCamera;
std::vector<cMovement_Camera::PTR> cGlumShapeFactory_ShapeContainer<cMovement_Camera>::mList; 
std::vector<cMovement_Camera::PTR> cGlumShapeFactory_ShapeContainer<cMovement_Camera>::mRecycleBin; 
TimerWrapper::cMutexWrapper cGlumShapeFactory_ShapeContainer<cMovement_Camera>::mReadWriteLock;


cGlumShapeFactory::cGlumShapeFactory(void) : mNextFreeID(0), mCurrentCamera(NULL), 
      mFactoryLock(),
      mFactoryState(RESERVE_FACTORYLIST)
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

   mRock.Start(mCurrentCamera);
   mRadar.Start(mCurrentCamera);
   mHud.Start(mCurrentCamera);
   mSwitchGadget.Start(mCurrentCamera);   
   mConsole.Start(mCurrentCamera);
   //mCamera.Start(mCurrentCamera);
}

void cGlumShapeFactory::DoubleBufferCoordinates(void)
{
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

cGlumShape_Rock::PTR cGlumShapeFactory::CreateRock(cHUD_Colour *hud_colour, float scale, bool start, Glumer::GlumerOnClicked *onClicked)
{
	cGlumShape_Rock::PTR new_one;
	new_one = mRock.CreateNew(++mNextFreeID, hud_colour);
   new_one.ptr->FactorySetOnCliced(onClicked);

   // lock scope
   {
      new_one.ptr->FactorySetRadius(scale);

      cGlumShapeBase::PTR gsbPimp = { (cGlumShapeBase*) new_one.ptr };
      cObjectMoveableBase::PTR pimp = { dynamic_cast<cObjectMoveableBase *>( new_one.ptr ) };
      if (pimp.ptr == NULL) throw -12345;
      mFactoryState.DrawListAdd(pimp, new_one.ptr->GetID());
      mFactoryState.FactoryListAdd(gsbPimp);
      mFactoryState.SelectableDrawListAdd(new_one.ptr, new_one.ptr->GetID());
   }
	return new_one;
}

cGlumShape_Rock::PTR cGlumShapeFactory::CreateBullet(cHUD_Colour *hud_colour, float scale, bool start)
{
	cGlumShape_Rock::PTR new_one;
	new_one = mRock.CreateNew(++mNextFreeID, hud_colour);
   new_one.ptr->FactorySetDelay(5);
   // lock scope
   {
      new_one.ptr->FactorySetRadius(scale);

      cGlumShapeBase::PTR gsbPimp = { (cGlumShapeBase*) new_one.ptr };
      cObjectMoveableBase::PTR pimp = { dynamic_cast<cObjectMoveableBase *>( new_one.ptr ) };
      if (pimp.ptr == NULL) throw -12346;
      mFactoryState.DrawListAdd(pimp, new_one.ptr->GetID());
      mFactoryState.FactoryListAdd(gsbPimp);
   }
	return new_one;
}

cGlumShape_SwitchGadget::PTR cGlumShapeFactory::CreateSwitchGadget(cHUD_Colour *hud_colour, float scale, cTimer_PistonMotion::PistonValueChanged *onpistonchange, GlumerOnClickedBool *onclickedbool, GlumerOnClicked *onclicked)
{
	cGlumShape_SwitchGadget::PTR new_one;
	new_one = mSwitchGadget.CreateNew(++mNextFreeID, hud_colour);
   new_one.ptr->Init(scale, onpistonchange, onclickedbool, onclicked);   	

   cGlumShapeBase::PTR gsbPimp = { (cGlumShapeBase*) new_one.ptr };   
   cObjectMoveableBase::PTR pimp = { dynamic_cast<cObjectMoveableBase *>( new_one.ptr ) };

   mFactoryState.DrawListAdd(pimp, new_one.ptr->GetID());
   mFactoryState.FactoryListAdd(gsbPimp);

   mFactoryState.SelectableDrawListAdd(new_one.ptr, new_one.ptr->GetID());
	return new_one;
}

cGlumShape_HUDShortRadar::PTR cGlumShapeFactory::CreateHUDShortRadar(cHUD_Colour *hud_colour, float range, float scale)
{
	cGlumShape_HUDShortRadar::PTR new_one;
	new_one = mRadar.CreateNew(++mNextFreeID, hud_colour);
   cGlumShapeBase::PTR gsbPimp = { (cGlumShapeBase*) new_one.ptr };
   cObjectMoveableBase::PTR pimp = { dynamic_cast<cObjectMoveableBase *>( new_one.ptr ) };

   mFactoryState.DrawListAdd(pimp, new_one.ptr->GetID());
   mFactoryState.FactoryListAdd(gsbPimp);

	new_one.ptr->Init(range, scale);
	return new_one;
}

cGlumShape_HUDLongRadar::PTR cGlumShapeFactory::CreateHUDLongRadar(cHUD_Colour *hud_colour, float range, float scale)
{
	cGlumShape_HUDLongRadar::PTR new_one;
	new_one = mHud.CreateNew(++mNextFreeID, hud_colour);
   cGlumShapeBase::PTR gsbPimp = { (cGlumShapeBase*) new_one.ptr };
   cObjectMoveableBase::PTR pimp = { dynamic_cast<cObjectMoveableBase *>( new_one.ptr ) };

   mFactoryState.DrawListAdd(pimp, new_one.ptr->GetID());
   mFactoryState.FactoryListAdd(gsbPimp);

	new_one.ptr->cGlumShapeBase::FactorySetInvisible(true);
	new_one.ptr->Init(range, scale, mCurrentCamera);
	return new_one;
}

cGlumShape_Console::PTR cGlumShapeFactory::CreateConsole(cHUD_Colour *hud_colour, float scale)
{
   cGlumShape_Console::PTR new_one;
	new_one = mConsole.CreateNew(++mNextFreeID, hud_colour);
   cGlumShapeBase::PTR gsbPimp = { (cGlumShapeBase*) new_one.ptr };
   cObjectMoveableBase::PTR pimp = { dynamic_cast<cObjectMoveableBase *>( new_one.ptr ) };

   mFactoryState.DrawListAdd(pimp, new_one.ptr->GetID());
   mFactoryState.FactoryListAdd(gsbPimp);
   return new_one;
}

cMovement_Camera::PTR cGlumShapeFactory::CreateCamera(void)
{
   cMovement_Camera::PTR new_one;
	new_one = mCamera.CreateNew(++mNextFreeID, NULL);
   cGlumShapeBase::PTR gsbPimp = { (cGlumShapeBase*) new_one.ptr };

   mFactoryState.FactoryListAdd(gsbPimp);
   return new_one;
}

}