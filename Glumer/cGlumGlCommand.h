// cGlumShape_Console.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CGLUMGLCOMMAND_H
#define CGLUMGLCOMMAND_H

//#include "TimerWrapper.h"
//#include "cPolyLetter.h"
//#include "cObjectMoveableBase.h"
//#include "cGlumShapeBase.h"
//#include "cTimer_RockMotion.h"
//#include <string>
//#include <vector>
//#include <algorithm>
//#include <functional>

namespace Glumer
{


	class cGlumGlCommand
	{
	public:
		cGlumGlCommand(int GL_BEGIN_MODE_TYPE, float floats[], unsigned int floatCount) : mGL_BEGIN_MODE_TYPE(GL_BEGIN_MODE_TYPE), mfloats(floats), mfloatCount(floatCount) { }
		inline const int GetCommand(void) { return this->mGL_BEGIN_MODE_TYPE; }
		inline const float * const GetFloats(void) { return this->mfloats; } 
		inline const unsigned int GetFloatCount() { return this->mfloatCount;  }
	private:
		int mGL_BEGIN_MODE_TYPE;
		float *mfloats;
		unsigned int mfloatCount;
	};


}

#endif