// cGlumShape_SwitchGadget.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CGLUMSHAPE_SWITCHGADGET_H
#define CGLUMSHAPE_SWITCHGADGET_H

#include "cObjectMoveableBase.h"
#include "cSelectableBase.h"
#include "cGluperScale.h"
#include "cTimer_PistonMotion.h"
#include "cMovement_Camera.h"
#include "cHUD_Colour.h"

#include <Windows.h>
#include <gl/gl.h>
#include <gl/GLU.h>
#include "cGlumShape_Console.h"
#define BUILDING_DLL
#include "Glumer.h"

namespace Glumer
{

	class cGlumShape_SwitchGadget : public cGlumShapeBase, public cObjectMoveableBase, public cSelectableBase
	{
	public:
		//messed up
		typedef struct
		{
			cGlumShape_SwitchGadget *ptr;
			bool operator ==(unsigned int id)
			{
				return ptr->GetID() == id;
			};
		} PTR;

		// note: rewrite everything
		inline static float GetLineWidth(bool defaultLineWidth = false)
		{
			static bool setup = false;

			if (setup)
				goto shit;

			static GLfloat fSizes[2];      // Line width range metrics

			// Get line size metrics and save the smallest value
			glGetFloatv(GL_LINE_WIDTH_RANGE, fSizes);
			static float mDefaultLineWidth = fSizes[0];
			static float mLineWidth = fSizes[1] / 4.25f;

		shit:
			if (defaultLineWidth)
				return mDefaultLineWidth;
			else
				return mLineWidth;
		}

	private:
		inline void Show(void)
		{
			if (IsShown() == false) return;

			glLineWidth(mLineWidth);

			float unit = mSize / 2.0f;

			glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);

			bool caught = false;
			glPushMatrix();
			try
			{
				// Special size if selected
				if (mSelected)
				{
					glScalef(mScale.mScaleX * mScale.mScaleX, mScale.mScaleY * mScale.mScaleY, mScale.mScaleZ * mScale.mScaleZ);
				}

				// square (not cube)
				glBegin(GL_QUADS);
				glVertex3f(unit + mXSizeMod, unit, unit);
				glVertex3f(-unit, unit, unit);
				glVertex3f(-unit, -unit, unit);
				glVertex3f(unit + mXSizeMod, -unit, unit);
				glEnd();

				glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());

				glBegin(GL_QUADS);
				glVertex3f(unit + mXSizeMod, unit, unit - 0.1f);
				glVertex3f(-unit, unit, unit - 0.1f);
				glVertex3f(-unit, -unit, unit - 0.1f);
				glVertex3f(unit + mXSizeMod, -unit, unit - 0.1f);

				glEnd();
			}
			catch (...)
			{
				caught = true;

			}
			glPopMatrix();

			if (caught)
			{
				glBegin(GL_QUADS);
				glVertex3f(1.0f, 1.0f, 1.0f);
				glVertex3f(-1.0f, 1.0f, 1.0f);
				glVertex3f(-1.0f, -1.0f, 1.0f);
				glVertex3f(1.0f, -1.0f, 1.0f);
				glEnd();
			}

			float subUnit = unit;
			glBegin(GL_LINE_LOOP);
			glVertex3f(unit + subUnit + mXSizeMod + mXSizeMod, unit + subUnit, unit + subUnit);
			glVertex3f(-(unit + subUnit + mXSizeMod + mXSizeMod), unit + subUnit, unit + subUnit);
			glVertex3f(-(unit + subUnit + mXSizeMod + mXSizeMod), -(unit + subUnit), unit + subUnit);
			glVertex3f(unit + subUnit + mXSizeMod + mXSizeMod, -(unit + subUnit), unit + subUnit);
			glEnd();
			glLineWidth(GetLineWidth(true));
		};

		cGluperScale mScale;
		float mXSizeMod; // for stretching

		cTimer_PistonMotion *mPistonMotion;
		cTimer_PistonMotion::PistonValueChanged *mOnPistonValueChanged;
		GlumerOnClickedBool *mOnClickedBool;
		GlumerOnClicked *mOnClicked;

		float mSize;
		float mSavedX, mSavedY, mSavedZ;
		bool mHaveSaved;

		cHUD_Colour *mHUDColour;

		const float minFIXED;
		const float maxFIXED;
		const float incFIXED;
		float mLineWidth, mDefaultLineWidth;

	public:
		void cGlumShapeBase::AnimationStart(void) { };
		void cGlumShapeBase::AnimationStop(void) { };


		inline cGlumShape_SwitchGadget(unsigned int id, cHUD_Colour *hud_colour)
			: cGlumShapeBase(id),
			cObjectMoveableBase(),
			cSelectableBase(id),
			mHUDColour(hud_colour),
			mPistonMotion(NULL),
			mSize(0.25f),
			mSelected(false),
			mSavedX(0.0f),
			mSavedY(0.0f),
			mSavedZ(0.0f),
			mHaveSaved(false),
			mOnClickedBool(NULL),
			mOnClicked(NULL),
			mOnPistonValueChanged(NULL),
			mScale(),
			mXSizeMod(0.0f),
			minFIXED(-0.1f),
			maxFIXED(0.1f),
			incFIXED(0.02f),
			mLineWidth(0.1f),
			mDefaultLineWidth(0.1f)
		{
			cObjectMoveableBase::mObject = this;
			cSelectableBase::FactorySetGuts(this);

			cMovementBase::PTR ptr;
			ptr.ptr = &mCenter;
			mMovementStack.push_back(ptr);
			ptr.ptr = &mOrientation;
			mMovementStack.push_back(ptr);
			ptr.ptr = &mScale;
			mMovementStack.push_back(ptr);
		}

		static void PistonShift_OnOffPulse(cTimer_PistonMotion *sender, cGlumShapeBase *common, float value, bool reset);
		static void PistonShift_OnOffClick(cTimer_PistonMotion *sender, cGlumShapeBase *common, float value, bool reset);
		static void PistonShift_ButtonClick(cTimer_PistonMotion *sender, cGlumShapeBase *common, float value, bool reset);
		static void PistonShift_ButtonDoesntKnowWhatItIs(cTimer_PistonMotion *sender, cGlumShapeBase *common, float value, bool reset);

		virtual void Start(cMovementBase *world_offset)
		{
			if (mPistonMotion != NULL) throw - 321;

			cMovementBase::PTR world_offset_ptr;
			world_offset_ptr.ptr = world_offset;
			mMovementStack.insert(mMovementStack.begin(), world_offset_ptr);

			mPistonMotion = new cTimer_PistonMotion(this, mOnPistonValueChanged);
			mPistonMotion->Reset(minFIXED, maxFIXED, incFIXED, mPistonMotion->CalculateInitialValue(minFIXED, maxFIXED), true);
		};
		virtual void Stop(void) { delete mPistonMotion; mPistonMotion = NULL; }
		void Init(float scale, cTimer_PistonMotion::PistonValueChanged *onpistonchanged, GlumerOnClickedBool *onclickedbool, GlumerOnClicked *onclicked);

		inline ~cGlumShape_SwitchGadget(void)
		{
			if (mPistonMotion != NULL)
				delete mPistonMotion;
		};

		inline void EventShow(void)
		{
			//cGlumShapeBase::
			if (IsShown() == false) return;
			Show();
		};

		void EventSelected(void);

		bool mSelected;
	};

}

#endif