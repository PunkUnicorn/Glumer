// cGlumShape_HUDShortRadar.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CGLUMSHAPE_HUDSHORTRADAR_H
#define CGLUMSHAPE_HUDSHORTRADAR_H

#include "cObjectMoveableBase.h"
#include "cGlumShapeBase.h"
#include "cHUD_Colour.h"

#include "cGlumShape_Console.h"
#include "cMovement_Camera.h"

#include <Windows.h>
#include <gl/GL.h>

namespace Glumer 
{


// out of order
class cGlumShape_HUDShortRadar : public cObjectMoveableBase, public cGlumShapeBase
{
	private:
		class ShowPing : public std::unary_function<void *, void>
		{
			private:
				unsigned int mMyID;
				cGlumShape_Console &mText;

			public:
				inline ShowPing(unsigned int myID, cGlumShape_Console &text) : mMyID(myID), mText(text) { };
				inline result_type operator () (const argument_type& ping) const
				{
					
				};
		};

		const static void ShowCenter(float radius);
	
		cGlumShape_Console mPingText;
		cHUD_Colour *mHUDColour;
		bool mOff;
		float mScale;
		float mRange;

	public:
		typedef struct
		{
			cGlumShape_HUDShortRadar *ptr;
			bool operator ==(unsigned int id) const
			{
				return ptr->GetID() == id;
			}
		} PTR;

		inline void SetVisible(bool off) {	mOff = off;	};

		inline static void ShowCorner(float unit, float branch_length)
		{
			glBegin(GL_LINES);
				glVertex3f(-unit, unit, -unit);
				glVertex3f(-unit+branch_length, unit, -unit);

				glVertex3f(-unit, unit, -unit);
				glVertex3f(-unit, unit-branch_length, -unit);

				glVertex3f(-unit, unit, -unit);
				glVertex3f(-unit, unit, -unit+branch_length);
			glEnd();
		}

		static void ShowCorneredBox(float unit)
		{
			float sub_unit = unit/2;
			glPushMatrix();
				for (int u=0; u<2; u++)
				{
					for (int i=0; i<4; i++)
					{
						ShowCorner(unit, sub_unit);
						glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
					}
					glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
				}
			glPopMatrix();
		};

		cGlumShape_HUDShortRadar(unsigned int id, cHUD_Colour *hud_colour);

      virtual void Start(cMovementBase *world_offset) {};
      virtual void Stop(void) { }
		void Init(float range, float scale);

		void cObjectBase::EventShow(void);
};

}

#endif