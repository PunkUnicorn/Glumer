#pragma once
#include "cObjectMoveableBase.h"
#include "cGlumShapeBase.h"
#include "cSelectableBase.h"
#include "cTimer_RockMotion.h"
#include "cHUD_Colour.h"
#include <Windows.h>
#include <gl/gl.h>
#define BUILDING_DLL
#include "Glumer.h"

namespace Glumer
{

// morph into a polyhedra generator rather than 'rock' generator
class cGlumShape_Rock : public cGlumShapeBase, public cObjectMoveableBase, public cSelectableBase
{
public:
   typedef struct 
   {
      cGlumShape_Rock *ptr;
	   bool operator ==(unsigned int id)
	   {
		   return ptr->GetID() == id;
	   }
   } PTR;
   typedef enum Shape { Four, Six, Eight, Twelve, Twenty };

private:
   Shape mShape;
   inline void Octahedron(float radius) const
   {
      glVertex3f( 0.0f, radius, 0.0f);
      glVertex3f( 0.0f, 0.0f, radius);
      glVertex3f( radius, 0.0f, 0.0f);

      glVertex3f( 0.0f, radius, 0.0f);
      glVertex3f( radius, 0.0f, 0.0f);
      glVertex3f( 0.0f, 0.0f, -radius);

      glVertex3f( 0.0f, radius, 0.0f);
      glVertex3f( 0.0f, 0.0f, -radius);
      glVertex3f( -radius, 0.0f, 0.0f);

      glVertex3f( 0.0f, radius, 0.0f);
      glVertex3f( 0.0f, 0.0f, radius);
      glVertex3f( -radius, 0.0f, 0.0f);


      glVertex3f( 0.0f, -radius, 0.0f);
      glVertex3f( 0.0f, 0.0f, radius);
      glVertex3f( radius, 0.0f, 0.0f);

      glVertex3f( 0.0f, -radius, 0.0f);
      glVertex3f( radius, 0.0f, 0.0f);
      glVertex3f( 0.0f, 0.0f, -radius);

      glVertex3f( 0.0f, -radius, 0.0f);
      glVertex3f( 0.0f, 0.0f, -radius);
      glVertex3f( -radius, 0.0f, 0.0f);

      glVertex3f( 0.0f, -radius, 0.0f);
      glVertex3f( 0.0f, 0.0f, radius);
      glVertex3f( -radius, 0.0f, 0.0f);
   }

   bool mCompiled;
   GLuint mListIndex;
   inline void Show(float radius) const
   {
      switch (mShape)
      {
      case cGlumShape_Rock::Eight:
         Octahedron(radius);
         break;

      //http://www.glprogramming.com/red/chapter02.html
      }
   };

   cTimer_RockMotion *mTimer;
   unsigned int mDelay;
   float mLineWidth, mDefaultLineWidth;
   float mRadius;
   GlumerOnClicked *mOnClicked;

public:
   cHUD_Colour *mHUDColour;

   inline cGlumShape_Rock(unsigned int id, cHUD_Colour *hud_colour) 
      : cGlumShapeBase(id), cObjectMoveableBase(), cSelectableBase(id), mShape(cGlumShape_Rock::Eight), mCompiled(false), mListIndex(0), mTimer(NULL), mDelay(cGlumShapeBase::MOVEMENT_UPDATE_INTERVAL), mLineWidth(0.1f), mDefaultLineWidth(0.1f), mRadius(1.0f), mOnClicked(NULL), mHUDColour(hud_colour)
   {
	   cObjectMoveableBase::mObject = this;
      cSelectableBase::FactorySetGuts(this);
	   cMovementBase::PTR ptr;
	   ptr.ptr = &mCenter;
	   mMovementStack.push_back(ptr);
	   ptr.ptr = &mOrientation;
	   mMovementStack.push_back(ptr);
   }

   inline float GetRadius(void) const { return mRadius; };
	inline void FactorySetRadius(float radius) { mRadius = radius; };
   inline void FactorySetOnCliced(GlumerOnClicked *onClicked) { mOnClicked = onClicked; }
   inline void FactorySetDelay(unsigned int delay) { mDelay = mDelay; }
   inline unsigned int GetDelay(void) const { return mDelay; }

   virtual void NormaliseDirection(void) 
   {
      // if mBufferedDirection different to mDirection
      // see if vector needs normalising and the rock motion inverval increased or decreased
   }

   virtual void Start(cMovementBase *world_offset);
   virtual void Stop(void) 
   { 
      mTimer->SetPause(true);
      mTimer->Abort();
   }

   virtual ~cGlumShape_Rock(void)
   {
      if (mCompiled)
         glDeleteLists(mListIndex, 1);

      if (mTimer != NULL)
      {
         mTimer->Abort();
         while (mTimer->HasAborted() == false)
            SDL_Delay(1);

		   delete mTimer;
      }
   };

   virtual void /*cObjectBase::*/EventShow(void) 
   {
      if (cGlumShapeBase::IsShown() == false) return;

      if (mCompiled)
      {
         glCallList(mListIndex);
      }
      else
      {
         mCompiled = true;
         mListIndex = glGenLists(1);
         glNewList(mListIndex, GL_COMPILE);
            float r = GetRadius();

	         glLineWidth(mLineWidth);

               glColor3ub(0, 0, 0); 
               glBegin(GL_TRIANGLE_FAN);
                  Show(r-mLineWidth);
               glEnd();

               glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue); 
               glBegin(GL_LINE_LOOP);
         		   Show(r);
               glEnd();

	         glLineWidth(mDefaultLineWidth);


         glEndList();
      }
   };

   virtual void EventSelected(void)
   {
      if (mOnClicked != NULL)
         (*mOnClicked)(GetID());
   }
};

}