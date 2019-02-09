// cGlumShape_Console.h
// Copyright (c) 2019
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CGLUMGLCOMMAND_H
#define CGLUMGLCOMMAND_H

namespace Glumer
{


	class cGlumGlCommand
	{
	public:
		cGlumGlCommand(int GL_BEGIN_MODE_TYPE, float floats[], unsigned int floatCount) : mCompiledName(0), mGL_BEGIN_MODE_TYPE(GL_BEGIN_MODE_TYPE), mfloats(floats), mfloatCount(floatCount) { }
		cGlumGlCommand(unsigned int compiledName) : mCompiledName(compiledName), mGL_BEGIN_MODE_TYPE(0), mfloats(0 /*NULL*/), mfloatCount(0) { }
		inline const unsigned int GetCompiledName(void) { return this->mCompiledName; }
		inline const int GetCommand(void) { return this->mGL_BEGIN_MODE_TYPE; }
		inline const float * const GetFloats(void) { return this->mfloats; }
		inline const unsigned int GetFloatCount() { return this->mfloatCount; }
	private:
		unsigned int mCompiledName;
		int mGL_BEGIN_MODE_TYPE;
		float *mfloats;
		unsigned int mfloatCount;
	};


}

#endif