// Glumer.cpp
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#define BUILDING_DLL
#include "cHUD_Colour.h"
#include "cGlumShapeFactory.h"
#include <vector>
#include <map>
#include <SDL.h>
#include "cGlumShapeFactory.h"
#include "STDCALL.h"
#include "CameraMovement.h"

#include "DLL_PUBLIC.h"
#include "Glumer.h"

using namespace Glumer;

//static cHUD_Colour hudColour;
static cGlumShapeFactory factory;

extern "C" DLL_PUBLIC bool STDCALL InitGlumer(byte r, byte b, byte g)
{
	Uint32 flags = SDL_INIT_TIMER;
	factory.Init(r, b, g);
	SDL_Init(SDL_WasInit(0) | SDL_INIT_TIMER);
	if (SDL_WasInit(flags) != flags)
	{
		return false;
	}
	return true;
}

extern "C" DLL_PUBLIC void STDCALL UnInitGlumer(void)
{
}

extern "C" DLL_PUBLIC unsigned int STDCALL GetRenderCount(void)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      return factory.GetCount();
   }
   catch (...)
   {
      return 0;
   }
}

extern "C" DLL_PUBLIC bool STDCALL Delete(unsigned int deleteId)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      cGlumShapeBase::PTR thing = { (factory.Get(deleteId).ptr) };
      if (thing.ptr == NULL) return false;
      factory.Delete(thing, deleteId);
	  return true;
   }
   catch (...)
   { return false; }
}

extern "C" DLL_PUBLIC bool STDCALL SetInvisible(unsigned glumId, bool visible)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());        
      cGlumShapeBase::PTR thing = { (factory.Get(glumId).ptr) };
      if (thing.ptr == NULL) return false;
      factory.SetInvisible(thing, visible);
	  return true;
   }
   catch (...)
   { return false; }
}

extern "C" DLL_PUBLIC unsigned int STDCALL CreateCamera(void)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      cMovement_Camera::PTR cameraPimp = factory.CreateCamera();
      if (cameraPimp.ptr == NULL) return 0;
      cMovement_Camera &camera = *cameraPimp.ptr;

      if (factory.IsCameraSet())
         camera.Start(NULL);

      return camera.GetID();
   }
   catch (...)
   {
      return 0;
   }
}

extern "C" DLL_PUBLIC unsigned int STDCALL CreatePolyhedron(float scale, PolyhedronType type, float x, float y, float z, Glumer::GlumerOnClicked *onClicked)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
	  cGlumShape_RegularPolyhedron::PTR shapePimp = factory.CreateRegularPolyhedron(type, scale, false, onClicked);
      if (shapePimp.ptr == NULL) return 0;
      cGlumShape_RegularPolyhedron &shape = *shapePimp.ptr;
      cGluperCenter &center = shape.FactoryGetCenter();
      center.FactorySetXYZ(x, y, z);
      shape.CopyBuffer();

      if (factory.IsCameraSet())
         shape.Start(factory.GetCamera());
      return shape.GetID();
   }
   catch (...)
   {
      return 0;
   }
}

extern "C" DLL_PUBLIC unsigned int STDCALL CreateGLCommand(float scale, int GL_BEGIN_MODE_TYPE, float x, float y, float z, float floats[], unsigned int floatCount, Glumer::GlumerOnClicked *onClicked)
{
	try
	{
		TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
		cGlumShape_RegularPolyhedron::PTR shapePimp = factory.CreateGLCommand(scale, GL_BEGIN_MODE_TYPE, floats, floatCount, onClicked);
		if (shapePimp.ptr == NULL) return 0;
		cGlumShape_RegularPolyhedron &shape = *shapePimp.ptr;
		cGluperCenter &center = shape.FactoryGetCenter();
		center.FactorySetXYZ(x, y, z);
		shape.CopyBuffer();

		if (factory.IsCameraSet())
			shape.Start(factory.GetCamera());
		return shape.GetID();
	}
	catch (...)
	{
		return 0;
	}
}

extern "C" DLL_PUBLIC unsigned int STDCALL CreateGLCompiledName(float scale, unsigned int compiledName, float x, float y, float z, Glumer::GlumerOnClicked *onClicked)
{
	try
	{
		TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
		cGlumShape_RegularPolyhedron::PTR shapePimp = factory.CreateGLCompiledName(scale, compiledName, onClicked);
		if (shapePimp.ptr == NULL) return 0;
		cGlumShape_RegularPolyhedron &shape = *shapePimp.ptr;
		cGluperCenter &center = shape.FactoryGetCenter();
		center.FactorySetXYZ(x, y, z);
		shape.CopyBuffer();

		if (factory.IsCameraSet())
			shape.Start(factory.GetCamera());
		return shape.GetID();
	}
	catch (...)
	{
		return 0;
	}
}

extern "C" DLL_PUBLIC bool STDCALL SetPolyhedronOnClicked(unsigned int id, Glumer::GlumerOnClicked *onClicked)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      bool found = false;
	  cGlumShape_RegularPolyhedron::PTR thing = *(factory.mRegularPolyhedron.Get(id, found));
      if (thing.ptr == NULL) return false;
      thing.ptr->FactorySetOnCliced(onClicked);
      return true;
   }
   catch (...)
   {
      return false;
   }
}

extern "C" DLL_PUBLIC bool STDCALL GetPolyhedronRadius(unsigned int glumId, float &radius)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      bool found = false;
	  cGlumShape_RegularPolyhedron::PTR thing = *(factory.mRegularPolyhedron.Get(glumId, found));
      if (found == false) return false;
      radius = thing.ptr->GetRadius();
      return true;
   }
   catch (...)
   {
      return false;
   }
}

extern "C" DLL_PUBLIC unsigned int STDCALL CreateSwitch(float scale, float x, float y, float z, bool pulseWhenOn, GlumerOnClickedBool *onClickedBool)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      cGlumShape_SwitchGadget::PTR switchPimp
         = factory.CreateSwitchGadget(scale, pulseWhenOn ? &cGlumShape_SwitchGadget::PistonShift_OnOffPulse :
                                                                       &cGlumShape_SwitchGadget::PistonShift_OnOffClick, onClickedBool, NULL);
	  if (switchPimp.ptr == NULL) return 0;
      cGlumShape_SwitchGadget &switchG = *switchPimp.ptr;
      cGluperCenter &center = switchG.FactoryGetCenter();
      center.FactorySetXYZ(x, y, z);
      switchG.CopyBuffer();

      if (factory.IsCameraSet())
         switchG.Start(factory.GetCamera());

      return switchG.GetID();
   }
   catch (...)
   {
      return 0;
   }
}

extern "C" DLL_PUBLIC unsigned int STDCALL CreateButton(float scale, float x, float y, float z, GlumerOnClicked *onClicked)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      cGlumShape_SwitchGadget::PTR switchPimp
         = factory.CreateSwitchGadget(scale, &cGlumShape_SwitchGadget::PistonShift_ButtonClick, NULL, onClicked);

	  if (switchPimp.ptr == NULL) return 0;

      cGlumShape_SwitchGadget &switchG = *switchPimp.ptr;
      cGluperCenter &center = switchG.FactoryGetCenter();
      center.FactorySetXYZ(x, y, z);
      switchG.CopyBuffer();

      if (factory.IsCameraSet())
         switchG.Start(factory.GetCamera());

      return switchG.GetID();
   }
   catch (...)
   {
      return 0;
   }
}

extern "C" DLL_PUBLIC unsigned int STDCALL CreateConsole(float scale, float x, float y, float z)
{
	try
	{
		TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
		cGlumShape_Console::PTR consolePimp = factory.CreateConsole(scale);
		if (consolePimp.ptr == NULL) return 0;
		cGlumShape_Console &console = *consolePimp.ptr;
		cGluperCenter &center = console.FactoryGetCenter();
		center.FactorySetXYZ(x, y, z);
		console.CopyBuffer();

		if (factory.IsCameraSet())
			console.Start(factory.GetCamera());

		return console.GetID();
	}
	catch (...)
	{
		return 0;
	}
}

extern "C" DLL_PUBLIC bool STDCALL SetConsoleText(unsigned int id, const char *text, unsigned int text_size)
{
	try
	{
		TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
		bool haveFound = false;
		cGlumShape_Console::PTR found = *(factory.mConsole.Get(id, haveFound));
		if (haveFound)
			found.ptr->SetMessage(text);   
		return haveFound;
	}
	catch (...)
	{
		return false;
	}
}

extern "C" DLL_PUBLIC bool STDCALL AddConsoleText(unsigned int id, const char *text, unsigned int text_size)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      if (factory.mConsole.ListSize() == 0) return false;
      bool haveFound = false;
      cGlumShape_Console::PTR found = *(factory.mConsole.Get(id, haveFound));
      if (haveFound)
         found.ptr->AddMessage(text);   
      return haveFound;
   }
   catch (...)
   {
      return false;
   }

}

extern "C" DLL_PUBLIC bool STDCALL AddConsoleCode(unsigned int id, const char *text, unsigned int text_size)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      if (factory.mConsole.ListSize() == 0) return false;
      bool haveFound = false;
      cGlumShape_Console::PTR found = *(factory.mConsole.Get(id, haveFound));
      if (haveFound)
         found.ptr->AddCode(text);   
      return haveFound;
   }
   catch (...)
   {
      return false;
   }
}

extern "C" DLL_PUBLIC bool STDCALL ScrollConsoleText(unsigned int id, unsigned int lines)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      if (factory.mConsole.ListSize() == 0) return false;
      bool haveFound = false;
      cGlumShape_Console::PTR found = *(factory.mConsole.Get(id, haveFound));
      if (haveFound)
         found.ptr->ScrollMessage(lines);
      return haveFound;
   }
   catch (...)
   {
      return false;
   }
}

extern "C" DLL_PUBLIC bool STDCALL CompileConsoleText(unsigned int id, const char *text, unsigned int text_size)
{
	try
	{
		TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
		bool haveFound = false;
		cGlumShape_Console::PTR found = *(factory.mConsole.Get(id, haveFound));
		if (haveFound)
		{
			found.ptr->CompileMessage(text);
		}
		return haveFound;
	}
	catch (...)
	{
		return false;
	}
}

extern "C" DLL_PUBLIC void STDCALL Start(unsigned int cameraId)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      factory.Start(cameraId);
	  CameraMovement::SetCameraId(cameraId);
   }
   catch (...)
   { }     
}

extern "C" DLL_PUBLIC void STDCALL DrawScene(void)
{
   try
   {
      //hudColour.m_red = r;
      //hudColour.m_green = g;
      //hudColour.m_blue = b;

      // lock scope
      {
         TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
         factory.DoubleBufferCoordinates();
		 factory.DrawScene();// &hudColour);
      }
   }
   catch (...)
   { }
}

extern "C" DLL_PUBLIC void STDCALL SetDrawClipWidth(float clipwidth)
{
	try
	{
		factory.SetDrawClipWidth(clipwidth);
	}
	catch (...)
	{
	}
}
extern "C" DLL_PUBLIC void STDCALL HitTest(unsigned int mouse_x, unsigned int mouse_y, int mouse_z)
{
   try
   {
      factory.HitTest(mouse_x, mouse_y, mouse_z);
   }
   catch (...)
   { }
}

extern "C" DLL_PUBLIC void STDCALL PointerMotionChange(unsigned int mouse_x, unsigned int mouse_y, int mouse_z)
{
	try
	{
		CameraMovement::PointerMotionChange(mouse_x, mouse_y, mouse_z);
	}
	catch (...)
	{
	}
}

extern "C" DLL_PUBLIC void STDCALL FirstPersonForwardMove(float step)
{
	try
	{
		CameraMovement::FirstPersonForwardMove(step);
	}
	catch (...)
	{
	}
}

extern "C" DLL_PUBLIC void STDCALL FirstPersonBackMove(float step)
{
	try
	{
		CameraMovement::FirstPersonBackMove(step);
	}
	catch (...)
	{
	}
}

extern "C" DLL_PUBLIC void STDCALL FirstPersonStop()
{
	try
	{
		CameraMovement::FirstPersonStop();
	}
	catch (...)
	{
	}
}

static void STDCALL GetCoords(unsigned int engineId, unsigned int bubbleId, float &X, float &Y, float &Z)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      factory.GetCoords(bubbleId, X, Y, Z);
   }
   catch (...)
   { }
}

extern "C" DLL_PUBLIC GetCoordsFunc * STDCALL CallToGetCoords(void)
{
   return GetCoords;
}

extern "C" DLL_PUBLIC bool STDCALL GetLocation(unsigned int id, float &x, float &y, float &z)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      cGlumShapeBase *pthing = factory.Get(id).ptr;
      if (pthing == NULL) return false;
      cGlumShapeBase &thing  = *pthing;
      const cGluperCenter &center = thing.GetCenter();
      center.GetXYZ(x, y, z);
      return true;
   }
   catch (...)
   {      
      return false;
   }
}

extern "C" DLL_PUBLIC bool STDCALL SetLocation(unsigned int id, float x, float y, float z)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      cGlumShapeBase *pthing = factory.Get(id).ptr;
      if (pthing == NULL) return false;
      cGlumShapeBase &thing  = *pthing;
      cGluperCenter &center = thing.FactoryGetCenter();
      center.FactorySetXYZ(x, y, z);
      thing.CopyBuffer();

      return true;
   }
   catch (...)
   {
      return false;
   }
}

extern "C" DLL_PUBLIC bool STDCALL AddLocation(unsigned int id, float x, float y, float z)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      cGlumShapeBase *pthing = factory.Get(id).ptr;
      if (pthing == NULL) return false;
      cGlumShapeBase &thing  = *pthing;
      cGluperCenter &center = thing.FactoryGetCenter();
      center.AddToXYZ(x, y, z);
      return true;
   }
   catch (...)
   {
      return false;
   }
}

extern "C" DLL_PUBLIC bool STDCALL GetDirection(unsigned int id, float &x, float &y, float &z)//, float &velocity)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      cGlumShapeBase *pthing = factory.Get(id).ptr;
      if (pthing == NULL) return false;
      cGlumShapeBase &thing  = *pthing;
      const cGluperDirection &direction = thing.GetDirection();
      direction.GetXYZ(x, y, z);
      //velocity = direction.GetVelocity();
      return true;
   }
   catch (...)
   {
      return false;
   }
}

extern "C" DLL_PUBLIC bool STDCALL SetDirection(unsigned int id, float x, float y, float z)//, float velocity)
{
   try
   {
     TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      cGlumShapeBase *pthing = factory.Get(id).ptr;
      if (pthing == NULL) return false;
      cGlumShapeBase &thing  = *pthing;
      cGluperDirection &direction = thing.FactoryGetDirection();
      direction.FactorySetXYZ(x, y, z);
	  if (thing.IsAnimated())
		  thing.AnimationStart();
	  else
		  thing.AnimationStop();
      //direction.FactorySetVelocity(velocity);
      thing.CopyBuffer();

      return true;
   }
   catch (...)
   {
      return false;
   }
}

extern "C" DLL_PUBLIC bool STDCALL AddDirection(unsigned int id, float x, float y, float z)//, float velocity)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      cGlumShapeBase *pthing = factory.Get(id).ptr;
      if (pthing == NULL) return false;
      cGlumShapeBase &thing  = *pthing;
      cGluperDirection &direction = thing.FactoryGetDirection();
      direction.AddToXYZ(x, y, z);
	  if (thing.IsAnimated())
		  thing.AnimationStart();
	  else
		  thing.AnimationStop();
	  //direction.AddToVelocity(velocity);
      return true;
   }
   catch (...)
   {
      return false;
   }
}

extern "C" DLL_PUBLIC bool STDCALL GetOrientation(unsigned int id, float &angle, float &x, float &y, float &z, float &angleIncrement)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      cGlumShapeBase *pthing = factory.Get(id).ptr;
      if (pthing == NULL) return false;
      cGlumShapeBase &thing  = *pthing;
      const cGluperOrientation &orientation = thing.GetOrientation();
      orientation.GetXYZAngleAndIncrement(angle, x, y, z, angleIncrement);
      return true;
   }
   catch (...)
   {
      return false;
   }
}

extern "C" DLL_PUBLIC bool STDCALL SetOrientation(unsigned int id, float angle, float x, float y, float z, float angleIncrement)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      cGlumShapeBase *pthing = factory.Get(id).ptr;
      if (pthing == NULL) return false;
      cGlumShapeBase &thing  = *pthing;
      cGluperOrientation &orientation = thing.FactoryGetOrientation();
      orientation.FactorySetAngleXYZ(x, y, z);
      orientation.FactorySetAngle(angle);
      orientation.FactorySetAngleIncrement(angleIncrement);
	  if (thing.IsAnimated())
		  thing.AnimationStart();
	  else
		  thing.AnimationStop();
	  //thing.CopyBuffer();

      return true;
   }
   catch (...)
   {
      return false;
   }
}

extern "C" DLL_PUBLIC bool STDCALL AddOrientation(unsigned int id, float angle, float x, float y, float z, float angleIncrement)
{
   try
   {
      TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
      cGlumShapeBase *pthing = factory.Get(id).ptr;
      if (pthing == NULL) return false;
      cGlumShapeBase &thing = *pthing;
      cGluperOrientation &orientation = thing.FactoryGetOrientation();
      orientation.AddToAngleXYZ(x, y, z);
      orientation.AddToAngle(angle);
      orientation.AddToAngleIncrement(angleIncrement);
	  if (thing.IsAnimated())
		  thing.AnimationStart();
	  else
		  thing.AnimationStop();
	  return true;
   }
   catch (...)
   {
      return false;
   }
}

//// If no anchor then your own id is returned
//extern "C" DLL_PUBLIC unsigned int STDCALL GetAnchor1(unsigned int id)
//{
//	try
//	{
//		TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
//		cGlumShapeBase *pmebase = factory.Get(id).ptr;
//
//		if (pmebase == NULL) return false;
//		cGlumShapeBase &me = *pmebase;
//		const cObjectMoveableBase *anchorCenter = me.FactoryGetCenter().GetAnchor1();
//		
//		if (anchorCenter == NULL)
//			return id;
//
//		unsigned int anchor = (dynamic_cast<const cGlumShapeBase*>(anchorCenter))->GetID();
//		return anchor;
//	}
//	catch (...)
//	{
//		return id;
//	}
//}

// to reset pass anchor same as id
extern "C" DLL_PUBLIC bool STDCALL SetAnchorMatchingRotationTo(unsigned int id, unsigned int anchor)
{
	try
	{
		TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
		cGlumShapeBase *pthing = factory.Get(id).ptr;
		if (pthing == NULL) return false;
		cGlumShapeBase &thing = *pthing;

		if (id == anchor)
		{
			thing.FactoryGetCenter().SetAnchorMatchingRotationTo(NULL);
		}
		else
		{
			cGlumShapeBase *pAnchorThing = factory.Get(anchor).ptr;
			if (pAnchorThing == NULL) return false;

			cObjectMoveableBase *moveable = dynamic_cast<cObjectMoveableBase*>(pAnchorThing);
			thing.FactoryGetCenter().SetAnchorMatchingRotationTo(moveable);

			// set both anchors !!
			cGluperCenter *anchorCenter = &(pAnchorThing->FactoryGetCenter());
			thing.FactoryGetCenter().SetAnchorTo(anchorCenter);
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}

// If no anchor then your own id is returned


// anchor needs to be a cGlumShapeBase in this case, so it can get both the mCenter and the id


//extern "C" DLL_PUBLIC unsigned int STDCALL GetAnchor2(unsigned int id)
//{
//	try
//	{
//		TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
//		cGlumShapeBase *pmebase = factory.Get(id).ptr;
//
//		if (pmebase == NULL) return false;
//		cGlumShapeBase &me = *pmebase;
//		const cMovementBase *anchorCenter = me.FactoryGetCenter().GetAnchor2();
//
//		if (anchorCenter == NULL)
//			return id;
//
//		unsigned int anchor = (dynamic_cast<const cGlumShapeBase*>(anchorCenter))->GetID();
//		return anchor;
//	}
//	catch (...)
//	{
//		return id;
//	}
//}


// to reset pass anchor same as id
extern "C" DLL_PUBLIC bool STDCALL SetAnchorTo(unsigned int id, unsigned int anchor)
{
	try
	{
		TimerWrapper::cMutexWrapper::Lock lock(factory.FactoryLock());
		cGlumShapeBase *pthing = factory.Get(id).ptr;
		if (pthing == NULL) return false;
		cGlumShapeBase &thing = *pthing;


		if (id == anchor)
		{
			thing.FactoryGetCenter().SetAnchorTo(NULL);
		}
		else
		{
			cGlumShapeBase *pAnchorThing = factory.Get(anchor).ptr;
			if (pAnchorThing == NULL) return false;

			cGluperCenter *anchorCenter = &(pAnchorThing->FactoryGetCenter());
			thing.FactoryGetCenter().SetAnchorTo(anchorCenter);
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}
