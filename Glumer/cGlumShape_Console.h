// cGlumShape_Console.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CGLUMSHAPE_CONSOLE_H
#define CGLUMSHAPE_CONSOLE_H

#include "TimerWrapper.h"
#include "cPolyLetter.h"
#include "cObjectMoveableBase.h"
#include "cGlumShapeBase.h"
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

namespace Glumer
{

class MessageCodeFound : public std::binary_function<std::string, std::string, bool> //all I wanted for christmas was to use bind1st
{
	public:
		inline result_type operator () (const first_argument_type& code, const second_argument_type& s) const
		{
			return code.compare(s) == 0;
		}
};

class cGlumShape_Console : public cObjectMoveableBase, public cGlumShapeBase
{
public:
   typedef struct 
   {
      cGlumShape_Console *ptr;
      bool operator ==(unsigned int id)
      {
         return ptr->GetID() == id;
      }
   } PTR;

private:
   cHUD_Colour *mHUD_Colour;
   int mLeftToRightIndex;
   int mTopToBottomIndex;

   inline void DrawLine(std::string &draw_me)
   {
      int tabIndex = 0;
      std::for_each(draw_me.begin(), draw_me.end(), 
         cPolyLetter(mHUD_Colour, mLeftToRightIndex, mTopToBottomIndex, tabIndex));

      mTopToBottomIndex++;
      mLeftToRightIndex=0;
   }

	class MessageCodeFound : public std::binary_function<std::string, std::string, bool> //all I wanted for christmas was to use bind1st
	{
		public:
			inline result_type operator () (const first_argument_type& code, const second_argument_type& s) const
			{
				unsigned int len = code.length();
				if (s.length() < len)
					len = s.length();

				return memcmp(code.c_str(), s.c_str(), len) == 0;
			}
	};

   std::vector<std::string> mMessageLines;
	float mScale;
	TimerWrapper::cMutexWrapper debug_lock;

public:
   virtual void Start(cMovementBase *world_offset) 
   {
      cObjectMoveableBase::mObject = this;
		cMovementBase::PTR ptr;
		ptr.ptr = &mCenter;
		mMovementStack.push_back(ptr);
		ptr.ptr = &mOrientation;
		mMovementStack.push_back(ptr);	
   };
   virtual void Stop(void) { }

   inline float GetScale(void) const {return mScale;};
   inline void SetScale(float newScale) {mScale=newScale;};

	// sets this string over the previous one
	void SetMessage(std::string setMe)
	{
      TimerWrapper::cMutexWrapper::Lock lock(&debug_lock);
		mMessageLines.clear();
		mMessageLines.push_back(setMe);
	}

	// adds this string to the end of the previous ones
	void AddMessage(std::string setMe)
	{
      TimerWrapper::cMutexWrapper::Lock lock(&debug_lock);
		mMessageLines.push_back(setMe);
	}

	// replaces message with matching 'code' (first space character indicates end of the code)
	void AddCode(std::string addMe)
	{
		int spaceIndex = addMe.find(" ");
		int length = addMe.length();
		if (spaceIndex == std::string::npos)
			spaceIndex = length;

      std::vector<std::string> copyOfLines;
      // lock scope
      {      
         TimerWrapper::cMutexWrapper::Lock lock(&debug_lock);
         copyOfLines.assign(mMessageLines.begin(), mMessageLines.end());
      }

         std::string code(addMe.substr(0, spaceIndex));
      std::vector<std::string>::iterator found = std::find_if(copyOfLines.begin(), copyOfLines.end(), 
	      std::bind1st(cGlumShape_Console::MessageCodeFound(), code));
        
      if (found == copyOfLines.end())
	      copyOfLines.push_back(addMe);
      else
	      found->assign(addMe);
     
      // lock scope
      {      
         TimerWrapper::cMutexWrapper::Lock lock(&debug_lock);
         mMessageLines.assign(copyOfLines.begin(), copyOfLines.end());
      }
	}

	// adds to the bottom of the message culling so there are 'lines' amount of lines
	void ScrollMessage(unsigned int lines)
	{
      TimerWrapper::cMutexWrapper::Lock lock(&debug_lock);
      int toRemove = mMessageLines.size() - lines;
      if (toRemove > 0)
      {
         std::copy(mMessageLines.begin() + toRemove, mMessageLines.end(), mMessageLines.begin());
         mMessageLines.resize(lines);
      }
	 }
    cGlumShape_Console(unsigned int id, cHUD_Colour *hud_colour, float scale = 1.0f);

    void /*cObjectMoveableBase::*/EventShow(void);
};

}

#endif