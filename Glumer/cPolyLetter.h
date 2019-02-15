// cPolyLetter.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CPOLYLETTER_H
#define CPOLYLETTER_H

#include "cHUD_Colour.h"
#include <algorithm>
#include <map>
#include <Windows.h>
#include <gl/gl.h>

namespace Glumer
{

class cPolyLetter : public std::unary_function<char &, bool> //all I wanted for christmas was to use bind1st
{
private:
    cHUD_Colour *mHUD_Colour;
    int &mLeftToRightIndex;
    int &mTopToBottomIndex;
    int &mTabIndex;
    static std::map<const char /*glyph*/, GLuint /*drawlistID*/> mDrawLists;
    void static DrawLetterInternal(cHUD_Colour *hud_colour, char &letter, const float **pstroke0, const float **pstroke1, const float **pstroke2);
    void static LoadStrokes(char &letter, int &mLeftToRightIndex, int &mTopToBottomIndex, int &mTabIndex, const float **pstroke0, const float **pstroke1, const float **pstroke2);

public:     
	cPolyLetter(cHUD_Colour *hud_colour, int &ltr, int &ttb, int &tabIndex) : mHUD_Colour(hud_colour), mLeftToRightIndex(ltr), mTopToBottomIndex(ttb), mTabIndex(tabIndex)
	{
	}

	static void CompileLetter(cHUD_Colour *hud_colour, char letter)
	{
		const float *one = NULL, *two = NULL, *three = NULL;
		const float **pstroke0 = &one;
		const float **pstroke1 = &two;
		const float **pstroke2 = &three;
		int dummy = 0;
		LoadStrokes(letter, dummy, dummy, dummy, pstroke0, pstroke1, pstroke2);
		const char cletter = letter;
		cPolyLetter::mDrawLists[cletter] = glGenLists(1);
		glNewList(cPolyLetter::mDrawLists[cletter], GL_COMPILE);
			DrawLetterInternal(hud_colour, letter, pstroke0, pstroke1, pstroke2);
		glEndList();
	}
	static void CompileAlphabet(cHUD_Colour *hud_colour, char startLetter)
	{
		for (int i = 0; i < 26/*a-z*/; i++)
		{
			char letter = startLetter + i;
			CompileLetter(hud_colour, letter);
		}
	}

	static void CompileLetters(cHUD_Colour *hud_colour)
	{
		const char supportedNonAlphaNumerics[] = { '.','-' };
		const char startLetter[] = { 'a','A' };
		for (int i=0; i < sizeof(startLetter); i++)
			CompileAlphabet(hud_colour, startLetter[i]);

		for (int i = 0; i < sizeof(supportedNonAlphaNumerics); i++)
			CompileLetter(hud_colour, supportedNonAlphaNumerics[i]);

		for (int i=0; i<10; i++)
			CompileLetter(hud_colour, '0' + i);
	}

	inline result_type operator () (argument_type letter) const
	{
      glPushMatrix();
         glScalef(0.6f, 0.9f, 0.9f);

         const float charSize = 0.4f;      
         glTranslatef(charSize * mLeftToRightIndex, -charSize * mTopToBottomIndex, 0.0f); //charSize /*whu?*/);

		 const char cletter = letter;
		 //std::map<const char, GLuint>::iterator found = cPolyLetter::mDrawLists.end();
		 //if (mNoCompiled == true)
			//found = cPolyLetter::mDrawLists.find(cletter); //== cPolyLetter::mDrawLists.end()

   //      if (mNoCompiled ||
			//  found == cPolyLetter::mDrawLists.end()
			// || found->second == 0)
		 if (cPolyLetter::mDrawLists.find(cletter) == cPolyLetter::mDrawLists.end()
			 || cPolyLetter::mDrawLists[cletter] == 0)
         {				 
			 const float *one = NULL, *two = NULL, *three = NULL;
			 const float **pstroke0 = &one;
			 const float **pstroke1 = &two;
			 const float **pstroke2 = &three;
			 LoadStrokes(letter, mLeftToRightIndex, mTopToBottomIndex, mTabIndex, pstroke0, pstroke1, pstroke2);

			 DrawLetterInternal(mHUD_Colour, letter, pstroke0, pstroke1, pstroke2);
            //cPolyLetter::mDrawLists[letter] = glGenLists(1);

            //glNewList(cPolyLetter::mDrawLists[letter], GL_COMPILE_AND_EXECUTE);
            //   DrawLetterInternal(this, letter, pstroke0, pstroke1, pstroke2);
            //glEndList();
         }
		 else {
			glCallList(cPolyLetter::mDrawLists[cletter]);
		 }

      glPopMatrix();
      mLeftToRightIndex++;
      return true;
	}

#define region
#ifdef region 
        static const float A0f[];

		  static const float A1f[];
        
        static const float B0f[];
        
        static const float C0f[];
        
        static const float D0f[];

        static const float E0f[];
        static const float E1f[];

        static const float F0f[];
        static const float F1f[];

        static const float G0f[];

        static const float H0f[];
        static const float H1f[];
        static const float H2f[];

        static const float I0f[];
        static const float I1f[];
        static const float I2f[];

        static const float J0f[];

        static const float K0f[];
        static const float K1f[];
        static const float K2f[];

        static const float L0f[];

        static const float M0f[];

        static const float N0f[];

        static const float O0f[];

        static const float P0f[];

        static const float *Q0f;
        static const float Q1f[];

        static const float *R0f;
        static const float R1f[];

        static const float S0f[];

        static const float T0f[];
        static const float T1f[];

        static const float U0f[];

        static const float V0f[];

        static const float W0f[];

        static const float X0f[];
        static const float X1f[];

        static const float Y0f[];
        static const float Y1f[];

        static const float Z0f[];

        static const float N00f[];
        static const float N01f[];

        static const float N10f[];
        static const float N11f[];

        static const float N20f[];

        static const float N30f[];

        static const float N40f[];

        static const float N50f[];

        static const float N60f[];

        static const float N70f[];

        static const float N80f[];

        static const float N90f[];

        static const float MINUSf[];
        static const float DOTf[];

        static const float ERROR0f[];
        
        #define cPolyLetter_ENDSTROKE 2.0f
#endif
};

}

#endif