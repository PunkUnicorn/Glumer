// cGlumShape_HUDLongRadar.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CGLUMSHAPE_HUDLONGRADAR_H
#define CGLUMSHAPE_HUDLONGRADAR_H

#include "cObjectMoveableBase.h"
#include "cGlumShapeBase.h"

#include "cMovement_Camera.h"
#include "cGlumShape_Console.h"
#include "cHUD_Colour.h"

namespace Glumer 
{


// out of order
class cGlumShape_HUDLongRadar : public cGlumShapeBase, public cObjectMoveableBase
{
	private:
		cMovementBase *mWorldOffset;
		static void ShowPingInternal(unsigned int myID, cGlumShape_Console &text, 
			void * ping, cMovementBase *mWorldOffset); /*get data from collision engine*/

		class ShowRadialPing : public std::unary_function<void *, void>
		{
			private:
				unsigned int mMyID;
				cGlumShape_Console &mText;
				cMovementBase *mWorldOffset;

			public:
				inline ShowRadialPing(unsigned int myID, cGlumShape_Console &text, cMovementBase *worldOffset) 
					: mMyID(myID), mText(text), mWorldOffset(worldOffset) { };

				inline result_type operator () (const argument_type& ping) const
				{
					ShowPingInternal(mMyID, mText, ping, mWorldOffset);
				};
		};

		bool mOff;
		cGlumShape_Console mText;

	public:
		typedef struct 
        {
            cGlumShape_HUDLongRadar *ptr;
			bool operator ==(unsigned int id) const
			{
				return ptr->GetID() == id;
			}
        } PTR;

		inline void SetVisible(bool off) {	mOff = off;	};

		static cHUD_Colour *mHUDColour;
		
		float mScale;
		float mRange;

		cGlumShape_HUDLongRadar(unsigned int id, cHUD_Colour *hud_colour);
      virtual ~cGlumShape_HUDLongRadar(void) { }

      virtual void Start(cMovementBase *world_offset) {}
      virtual void Stop(void) { }
		void Init(float range, float scale, cMovementBase *world_offset);
		
		virtual void cObjectBase::EventShow(void);
};

}

#endif