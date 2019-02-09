// cSelectableBase.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CSELECTABLEBASE_H
#define CSELECTABLEBASE_H

#include "cObjectBase.h"
#include "cObjectMoveableBase.h"
#include "TimerWrapper.h"
#include <vector>

namespace Glumer
{

	/*
		cSelectableBase
		a) needs to be 'named' in openGL terms.. an openGL 'name' is an integer
		b) needs to be enumeratable so to find an instance from a given openGL name

		The instance data of this class stores the openGL 'name' which must be
		loaded using glLoadName(this->mName); just before associated drawing is done
	*/
	class cSelectableBase
	{
	private:
		// facilitate enumeration through all selectable items
		//static int mNextFreeName;
		//static cSelectableBase *mFirst;
		//cSelectableBase *mNext;
		unsigned int mName;
		static TimerWrapper::cMutexWrapper mSelectableLock;
		// handy de-refs to avoid expensive RTTI
		//cObjectBase *mMeAsBase; 
		cObjectMoveableBase *mMeAsMoveable;

	public:
		unsigned int GetName(void) const { return mName; };

		virtual void EventSelected(void) = 0;

		void DrawSelectionArea(void)
		{
			glPushName(GetName());

			if (mMeAsMoveable != NULL)
				mMeAsMoveable->cObjectMoveableBase::EventShow();

			glPopName();
		}
		inline void FactorySetGuts(cObjectMoveableBase *asMoveable)
		{
			mMeAsMoveable = asMoveable;
		}
		cSelectableBase(unsigned int id);

		/* EventClick
		called to determain if draw_selectable was clicked on, and if so
		fires EventClicked .. otherwise it does nothing

		draw_selectable must re-draw all possible selectable world objects

		  Thanks to http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=32 (nehe productions, OpenGL tutorials, lesson 32)
		  copywrite nehe productions
		*/
		static bool EventClick(TimerWrapper::cMutexWrapper *lock, std::vector<cObjectBase *> &draw_selectable, std::vector<unsigned int/*glum Id*/> &index, unsigned int mouse_x, unsigned int mouse_y, int mouse_z);

		static void FireEventShow(cObjectBase *showme);
	};

}

#endif