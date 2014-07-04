// cGlumShapeFactory.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CGLUMSHAPEFACTORY_H
#define CGLUMSHAPEFACTORY_H

#include "cGlumShapeFactory_ShapeContainer.h"
#include "cGlumShape_Rock.h"
#include "cGlumShape_HUDLongRadar.h"
#include "cGlumShape_HUDShortRadar.h"
#include "cGlumShape_SwitchGadget.h"
#include "cGlumShape_Console.h"
#include "TimerWrapper.h"
#include "cGlumShapeFactoryState.h"
#include <map>

namespace Glumer
{

class cGlumShapeFactory
{
private:
	unsigned int mNextFreeID;
   cMovementBase *mCurrentCamera;
   TimerWrapper::cMutexWrapper mFactoryLock;
   cGlumShapeFactoryState mFactoryState;

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
   // move to factory state or something
	static cGlumShapeFactory_ShapeContainer<cGlumShape_Rock> mRock;
	static cGlumShapeFactory_ShapeContainer<cGlumShape_SwitchGadget> mSwitchGadget;
	static cGlumShapeFactory_ShapeContainer<cGlumShape_HUDShortRadar> mRadar;
	static cGlumShapeFactory_ShapeContainer<cGlumShape_HUDLongRadar> mHud;
   static cGlumShapeFactory_ShapeContainer<cGlumShape_Console> mConsole;
   static cGlumShapeFactory_ShapeContainer<cMovement_Camera> mCamera;

	cGlumShapeFactory(void);
	~cGlumShapeFactory(void) {};
	
   unsigned int GetCount(void)
   {
      return mRock.ListSize() + mSwitchGadget.ListSize() + mRadar.ListSize() + mHud.ListSize() + mConsole.ListSize() + mCamera.ListSize();
   }

   void Init(void) { };
	void Start(unsigned int cameraId);

   void DoubleBufferCoordinates(void);
   void DrawScene(cHUD_Colour *hud_colour);
   inline void HitTest(unsigned int mouse_x, unsigned int mouse_y, unsigned int mouse_z)
   {
      //TimerWrapper::cMutexWrapper::Lock lock();
      cSelectableBase::EventClick(mFactoryState.GetSelectableDrawListLock(), mFactoryState.GetSelectableDrawList(), mFactoryState.GetSelectableDrawListIndex(), mouse_x, mouse_y, mouse_z);
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

	cGlumShape_Rock::PTR CreateRock(cHUD_Colour *hud_colour, float scale, bool start, Glumer::GlumerOnClicked *onClicked);
	cGlumShape_Rock::PTR CreateBullet(cHUD_Colour *hud_colour, float scale, bool start);
	cGlumShape_SwitchGadget::PTR CreateSwitchGadget(cHUD_Colour *hud_colour, float scale, cTimer_PistonMotion::PistonValueChanged *onpistonchange, GlumerOnClickedBool *onclickedbool, GlumerOnClicked *onclicked);
	cGlumShape_HUDShortRadar::PTR CreateHUDShortRadar(cHUD_Colour *hud_colour, float range, float scale);
	cGlumShape_HUDLongRadar::PTR CreateHUDLongRadar(cHUD_Colour *hud_colour, float range, float scale);
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
      std::vector<cGlumShape_Rock::PTR>::iterator delIterator = mRock.Get(glumId, found);
      if (found) 
         mRock.Delete(delIterator);   

      if (found == false)
      {
         std::vector<cGlumShape_Console::PTR>::iterator delIterator = mConsole.Get(glumId, found);
         if (found) 
            mConsole.Delete(delIterator);    
      }

      deleteMe.ptr->FactorySetDeleted(true);

      //ClearDrawSceneCache();
      /*
      - mGlumShapeList
      - mDrawList
      - mSelectableDrawList
      -mRock.mList
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