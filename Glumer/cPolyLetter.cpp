// cPolyLetter.cpp
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)

#include "cPolyLetter.h"
#include <vector> //to get NULL

namespace Glumer
{

static const int tabSize = 6;

static void LoadStrokes(char &letter, int &mLeftToRightIndex, int &mTopToBottomIndex, const float **pstroke0, const float **pstroke1, const float **pstroke2);

std::map<char /*glyph*/, GLuint /*drawlistID*/> cPolyLetter::mDrawLists;

#define region
#ifdef region

float const cPolyLetter::A0f[]        = {0.0f, 0.0f, 0.0f,   0.5f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   cPolyLetter_ENDSTROKE};
float const cPolyLetter::A1f[]        = {0.25f, 0.5f, 0.0f,   0.75f, 0.5f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::B0f[]        = {0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.875f, 1.0f, 0.0f,   1.0f, 0.875f, 0.0f,   1.0f, 0.75f, 0.0f,   1.0f/*0.875f*/, 0.66f, 0.0f,   0.33f, 0.66f, 0.0f,   1.0f, 0.5f, 0.0f,   1.0f, 0.25f, 0.0f,   0.875f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,    cPolyLetter_ENDSTROKE};

float const cPolyLetter::C0f[]        = {1.0f, 0.375f, 0.0f,  0.875f, 0.0f, 0.0f,   0.125f, 0.0f, 0.0f,   0.0f, 0.375f, 0.0f,   0.0f, 0.875f, 0.0f,   0.125f, 1.0f, 0.0f,   0.875f, 1.0f, 0.0f,  1.0f, 0.875f, 0.0f,  cPolyLetter_ENDSTROKE};

float const cPolyLetter::D0f[]        = {1.0f, 0.375f, 0.0f,   0.875f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.125f, 1.0f, 0.0f,  0.875f, 1.0f, 0.0f,   1.0f, 0.875f, 0.0f,   1.0f, 0.375f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::E0f[]        = {1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   cPolyLetter_ENDSTROKE};
float const cPolyLetter::E1f[]        = {0.0f, 0.66f, 0.0f,   0.875f, 0.66f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::F0f[]        = {0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f, cPolyLetter_ENDSTROKE};
float const cPolyLetter::F1f[]        = {0.0f, 0.66f, 0.0f,   0.875f, 0.66f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::G0f[]        = {0.33f, 0.66f, 0.0f,   1.0f, 0.66f, 0.0f,   1.0f, 0.25f, 0.0f,   0.875, 0.0f, 0.0f,   0.125f, 0.0f, 0.0f,  0.0f, 0.375f, 0.0f,  0.0f, 0.875f, 0.0f,  0.125f, 1.0f, 0.0f,  0.875f, 1.0f, 0.0f,  1.0f, 0.875f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::H0f[]        = {0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   cPolyLetter_ENDSTROKE};
float const cPolyLetter::H1f[]        = {1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   cPolyLetter_ENDSTROKE};
float const cPolyLetter::H2f[]        = {0.0f, 0.66f, 0.0f,   0.875f, 0.66f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::I0f[]        = {0.33f, 1.0f, 0.0f,   0.66f, 1.0f, 0.0f,   cPolyLetter_ENDSTROKE};
float const cPolyLetter::I1f[]        = {0.33f, 0.0f, 0.0f,   0.66f, 0.0f, 0.0f,   cPolyLetter_ENDSTROKE};
float const cPolyLetter::I2f[]        = {0.5f, 1.0f, 0.0f,   0.5f, 0.0f, 0.0f,    cPolyLetter_ENDSTROKE};

float const cPolyLetter::J0f[]        = {1.0f, 1.0f, 0.0f,   1.0f, 0.375f, 0.0f,   0.875f, 0.0f, 0.0f,   0.125f, 0.0f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::K0f[]        = {0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   cPolyLetter_ENDSTROKE};
float const cPolyLetter::K1f[]        = {0.0f, 0.66f, 0.0f,   1.0f, 1.0f, 0.0f,   cPolyLetter_ENDSTROKE};
float const cPolyLetter::K2f[]        = {0.0f, 0.66f, 0.0f,   1.0f, 0.0f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::L0f[]        = {0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::M0f[]        = {0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.5f, 0.66f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::N0f[]        = {0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::O0f[]        = {1.0f, 0.375f, 0.0f,  0.875f, 0.0f, 0.0f,   0.125f, 0.0f, 0.0f,   0.0f, 0.375f, 0.0f,   0.0f, 0.875f, 0.0f,   0.125f, 1.0f, 0.0f,   0.875f, 1.0f, 0.0f,  1.0f, 0.875f, 0.0f,  1.0f, 0.375f, 0.0f,  cPolyLetter_ENDSTROKE};

float const cPolyLetter::P0f[]        = {0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.875f, 1.0f, 0.0f,   1.0f, 0.875f, 0.0f,   1.0f, 0.75f, 0.0f,    0.875f, 0.66f, 0.0f,   0.33f, 0.66f, 0.0f,   0.0f, 0.66f, 0.0f, cPolyLetter_ENDSTROKE};

float const *cPolyLetter::Q0f         = cPolyLetter::O0f;
float const cPolyLetter::Q1f[]        = {1.0f, 0.0f, 0.0f,   0.66f, 0.5f, 0.0f,   cPolyLetter_ENDSTROKE};

float const *cPolyLetter::R0f          = cPolyLetter::P0f;
float const cPolyLetter::R1f[]        = {0.0f, 0.66f, 0.0f,    1.0f, 0.0f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::S0f[]        = {1.0f, 0.875f, 0.0f,   0.875f, 1.0f, 0.0f,   0.125f, 1.0f, 0.0f,   0.0f, 0.875f, 0.0f,    0.0f, 0.66f, 0.0f,   0.33f, 0.66f, 0.0f,   1.0f, 0.66f, 0.0f,   1.0f, 0.25f, 0.0f,   0.875, 0.0f, 0.0f,   0.125f, 0.0f, 0.0f,   0.0f,  0.25f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::T0f[]        = {0.5f, 0.0f, 0.0f,   0.5f, 1.0f, 0.0f,   cPolyLetter_ENDSTROKE};
float const cPolyLetter::T1f[]        = {0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::U0f[]        = {0.0f, 1.0f, 0.0f,   0.0f, 0.25f, 0.0f,   0.25f, 0.0f, 0.0f,   0.75f, 0.0f, 0.0f,   1.0f, 0.25f, 0.0f,   1.0f, 1.0f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::V0f[]        = {0.0f, 1.0f, 0.0f,   0.5f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::W0f[]        = {0.0f, 1.0f, 0.0f,   0.0f, 0.375f, 0.0f,   0.125f, 0.0f, 0.0f,   0.5f, 0.66f, 0.0f,    0.875f, 0.0f, 0.0f,   1.0f, 0.375f, 0.0f,   1.0f, 1.0f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::X0f[]        = {0.0f, 1.0f, 0.0f,   0.5f, 0.66f, 0.0f,   1.0f, 0.0f, 0.0f,   cPolyLetter_ENDSTROKE};
float const cPolyLetter::X1f[]        = {1.0f, 1.0f, 0.0f,   0.5f, 0.66f, 0.0f,   0.0f, 0.0f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::Y0f[]        = {0.0f, 1.0f, 0.0f,   0.5f, 0.66f, 0.0f,   1.0f, 1.0f, 0.0f,   cPolyLetter_ENDSTROKE};
float const cPolyLetter::Y1f[]        = {0.5f, 0.66f, 0.0f,  0.5f, 0.0f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::Z0f[]        = {0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::N00f[]        = {0.5f, 1.0f, 0.0f,   0.0f, 0.875f, 0.0f,   0.0f, 0.375f, 0.0f,   0.375f, 0.0f, 0.0f,   0.625f, 0.0f, 0.0f,   1.0f, 0.375f, 0.0f,   1.0f, 0.875f, 0.0f,   0.5f, 1.0f, 0.0f,   cPolyLetter_ENDSTROKE};
float const cPolyLetter::N01f[]        = {1.0f, 0.875f, 0.0f,   0.1875f, 0.1875f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::N10f[]        = {0.33f, 0.875f, 0.0f,   0.5f, 1.0f, 0.0f,   0.5f, 0.0f, 0.0f,   cPolyLetter_ENDSTROKE};
float const cPolyLetter::N11f[]        = {0.33f, 0.0f, 0.0f,   0.66f, 0.0f, 0.0f,    cPolyLetter_ENDSTROKE};

float const cPolyLetter::N20f[]        = {0.0f, 0.875f, 0.0f,   0.5f, 1.0f, 0.0f,   1.0f, 0.875f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::N30f[]         = {0.0f, 0.875f, 0.0f,   0.5f, 1.0f, 0.0f,   1.0f, 0.875f, 0.0f,   1.0f, 0.75f, 0.0f,    1.0f, 0.66f, 0.0f,   0.33f, 0.66f, 0.0f,   1.0f, 0.5f, 0.0f,   1.0f, 0.25f, 0.0f,   0.875, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,    cPolyLetter_ENDSTROKE};

float const cPolyLetter::N40f[]        = {0.875f, 0.0f, 0.0f,   0.875f, 1.0f, 0.0f,   0.0f, 0.5f, 0.0f,   1.0f, 0.5f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::N50f[]        = {1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.5f, 0.0f,   1.0f, 0.5f, 0.0f,   1.0f, 0.5f, 0.0f,   1.0f, 0.25f, 0.0f,   0.875, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::N60f[]        = {1.0f, 0.875f, 0.0f,   0.5f, 1.0f, 0.0f,   0.0f, 0.875f, 0.0f,   0.0f, 0.375f, 0.0f,   0.375f, 0.0f, 0.0f,   0.625f, 0.0f, 0.0f,   1.0f, 0.375f, 0.0f,   1.0f, 0.5f, 0.0f,   0.5f, 0.66f, 0.0f,   0.0f, 0.5f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::N70f[]        = {0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.5f, 0.0f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::N80f[]        = {0.5f, 1.0f, 0.0f,   0.875f, 0.875f, 0.0f,   0.875f, 0.75f, 0.0f,   0.66f, 0.66f, 0.0f,   0.33f, 0.66f, 0.0f,   0.0f, 0.33f, 0.0f,   0.33f, 0.0f, 0.0f,   0.66f, 0.0f, 0.0f,   1.0f, 0.33f, 0.0f,   0.66f, 0.66f, 0.0f,   0.33f, 0.66f, 0.0f,   0.125f, 0.75, 0.0f,   0.125f, 0.875f, 0.0f,   0.5f, 1.0f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::N90f[]        = {1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.125f, 1.0f, 0.0f,   0.0f, 0.875f, 0.0f,   0.0f, 0.75f, 0.0f,    0.125f, 0.66f, 0.0f,   0.66f, 0.66f, 0.0f,   1.0f, 0.66f, 0.0f, cPolyLetter_ENDSTROKE};

float const cPolyLetter::MINUSf[]      = {0.13f, 0.5f, 0.0f,   0.86f, 0.5f, 0.0f,   cPolyLetter_ENDSTROKE};
float const cPolyLetter::DOTf[]        = {0.5f, 0.1f, 0.0f,   0.6f, 0.1f, 0.0f,   cPolyLetter_ENDSTROKE};

float const cPolyLetter::ERROR0f[]     = {0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   cPolyLetter_ENDSTROKE};
#endif

static void Shape(const float **pstroke0, const float **pstroke1, const float **pstroke2)
{
   const float charSizeScale = 4.0f;
   glBegin(GL_LINE_STRIP);
   if (*pstroke0 != NULL)
   {
      for (int i=0; (*pstroke0)[i] < cPolyLetter_ENDSTROKE; i+=3)
            glVertex3f((*pstroke0)[i]/charSizeScale, (*pstroke0)[i+1]/charSizeScale, (*pstroke0)[i+2]/charSizeScale);
   }
   glEnd();
    
   glBegin(GL_LINE_STRIP);
   if (*pstroke1 != NULL)
   {
      for (int i=0; (*pstroke1)[i] < cPolyLetter_ENDSTROKE; i+=3)
            glVertex3f((*pstroke1)[i]/charSizeScale, (*pstroke1)[i+1]/charSizeScale, (*pstroke1)[i+2]/charSizeScale);
   }
   glEnd();
    
   glBegin(GL_LINE_STRIP);
   if (*pstroke2 != NULL)
   {
      for (int i=0; (*pstroke2)[i] < cPolyLetter_ENDSTROKE; i+=3)
            glVertex3f((*pstroke2)[i]/charSizeScale, (*pstroke2)[i+1]/charSizeScale, (*pstroke2)[i+2]/charSizeScale);
   }
   glEnd();
}

// static class member        
void cPolyLetter::DrawLetterInternal(const cPolyLetter *me, char &letter, const float **pstroke0, const float **pstroke1, const float **pstroke2)
{
	// const float *one=NULL, *two=NULL, *three=NULL;
   // const float **pstroke0=&one;
   // const float **pstroke1=&two;
   // const float **pstroke2=&three;

	 /*LoadStrokes(letter, me->mLeftToRightIndex, me->mTopToBottomIndex, pstroke0, pstroke1, pstroke2);*/

		glPushMatrix();
		glColor3ub(me->mHUD_Colour->m_red, me->mHUD_Colour->m_green, me->mHUD_Colour->m_blue);
		//glBegin(GL_TRIANGLE_FAN);
	    glLineWidth(5.0f);
   		Shape(pstroke0, pstroke1, pstroke2);
         //glEnd();

		glColor3ub(0, 0, 0);
		const float defaultLineWidth = 2.f;
	     glLineWidth(defaultLineWidth);
         //glBegin(GL_LINE_LOOP);//GL_TRIANGLE_FAN );//
   		   Shape(pstroke0, pstroke1, pstroke2);
         //glEnd();
	      //glLineWidth(mDefaultLineWidth)      
            

      //glScalef(0.6f, 0.9f, 0.9f);

      //const float charSize = 0.4f;
      //glTranslatef(charSize * me->mLeftToRightIndex, -charSize * me->mTopToBottomIndex, charSize);
            
      
      /*const float charSizeScale = 4.0f;
      glBegin(GL_LINE_STRIP);
      if (*pstroke0 != NULL)
      {
         for (int i=0; (*pstroke0)[i] < cPolyLetter_ENDSTROKE; i+=3)
               glVertex3f((*pstroke0)[i]/charSizeScale, (*pstroke0)[i+1]/charSizeScale, (*pstroke0)[i+2]/charSizeScale);
      }
      glEnd();
    
      glBegin(GL_LINE_STRIP);
      if (*pstroke1 != NULL)
      {
         for (int i=0; (*pstroke1)[i] < cPolyLetter_ENDSTROKE; i+=3)
               glVertex3f((*pstroke1)[i]/charSizeScale, (*pstroke1)[i+1]/charSizeScale, (*pstroke1)[i+2]/charSizeScale);
      }
      glEnd();
    
      glBegin(GL_LINE_STRIP);
      if (*pstroke2 != NULL)
      {
         for (int i=0; (*pstroke2)[i] < cPolyLetter_ENDSTROKE; i+=3)
               glVertex3f((*pstroke2)[i]/charSizeScale, (*pstroke2)[i+1]/charSizeScale, (*pstroke2)[i+2]/charSizeScale);
      }
      glEnd();*/
    glPopMatrix();                        
}
            
//cPolyLetter::cPolyLetter(cHUD_Colour *hud_colour) : mHUDColour(hud_colour) 
//{
//};

void cPolyLetter::LoadStrokes(char &letter, int &mLeftToRightIndex, int &mTopToBottomIndex, int &mTabIndex, const float **pstroke0, const float **pstroke1, const float **pstroke2)
{
	switch (letter)
   {
        case 'A':
        case 'a':
            *pstroke0 = cPolyLetter::A0f;
            *pstroke1 = cPolyLetter::A1f;      
			   *pstroke2 = NULL;

            break;
                
        case 'B':
        case 'b':
            *pstroke0 = cPolyLetter::B0f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;
                    
        case 'C':
        case 'c':
            *pstroke0 = cPolyLetter::C0f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;
                    
        case 'D':
        case 'd':
            *pstroke0 = cPolyLetter::D0f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case 'E':
        case 'e':
            *pstroke0 = cPolyLetter::E0f;
            *pstroke1 = cPolyLetter::E1f;
            *pstroke2 = NULL;
            break;

        case 'F':
        case 'f':
            *pstroke0 = cPolyLetter::F0f;
            *pstroke1 = cPolyLetter::F1f;
            *pstroke2 = NULL;
            break;

        case 'G':
        case 'g':
            *pstroke0 = cPolyLetter::G0f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case 'H':
        case 'h':
            *pstroke0 = cPolyLetter::H0f;
            *pstroke1 = cPolyLetter::H1f;
            *pstroke2 = cPolyLetter::H2f;
            break;

        case 'I':
        case 'i':
            *pstroke0 = cPolyLetter::I0f;
            *pstroke1 = cPolyLetter::I1f;
            *pstroke2 = cPolyLetter::I2f;
            break;

        case 'J':
        case 'j':
            *pstroke0 = cPolyLetter::J0f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case 'K':
        case 'k':
            *pstroke0 = cPolyLetter::K0f;
            *pstroke1 = cPolyLetter::K1f;
            *pstroke2 = cPolyLetter::K2f;
            break;

        case 'L':
        case 'l':
            *pstroke0 = cPolyLetter::L0f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case 'M':
        case 'm':
            *pstroke0 = cPolyLetter::M0f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case 'N':
        case 'n':
            *pstroke0 = cPolyLetter::N0f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case 'O':
        case 'o':
            *pstroke0 = cPolyLetter::O0f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case 'P':
        case 'p':
            *pstroke0 = cPolyLetter::P0f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case 'Q':
        case 'q':
            *pstroke0 = cPolyLetter::Q0f;
            *pstroke1 = cPolyLetter::Q1f;
            *pstroke2 = NULL;
            break;

        case 'R':
        case 'r':
            *pstroke0 = cPolyLetter::R0f;
            *pstroke1 = cPolyLetter::R1f;
            *pstroke2 = NULL;
            break;

        case 'S':
        case 's':
            *pstroke0 = cPolyLetter::S0f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case 'T':
        case 't':
            *pstroke0 = cPolyLetter::T0f;
            *pstroke1 = cPolyLetter::T1f;
            *pstroke2 = NULL;
            break;

        case 'U':
        case 'u':
            *pstroke0 = cPolyLetter::U0f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case 'V':
        case 'v':
            *pstroke0 = cPolyLetter::V0f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case 'W':
        case 'w':
            *pstroke0 = cPolyLetter::W0f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case 'X':
        case 'x':
            *pstroke0 = cPolyLetter::X0f;
            *pstroke1 = cPolyLetter::X1f;
            *pstroke2 = NULL;
            break;

        case 'Y':
        case 'y':
            *pstroke0 = cPolyLetter::Y0f;
            *pstroke1 = cPolyLetter::Y1f;
            *pstroke2 = NULL;
            break;

        case 'Z':
        case 'z':
            *pstroke0 = cPolyLetter::Z0f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case '0':
            *pstroke0 = cPolyLetter::N00f;
            *pstroke1 = cPolyLetter::N01f;
            *pstroke2 = NULL;
            break;

        case '1':
            *pstroke0 = cPolyLetter::N10f;
            *pstroke1 = cPolyLetter::N11f;
            *pstroke2 = NULL;
            break;

        case '2':
            *pstroke0 = cPolyLetter::N20f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case '3':
            *pstroke0 = cPolyLetter::N30f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case '4':
            *pstroke0 = cPolyLetter::N40f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case '5':
            *pstroke0 = cPolyLetter::N50f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case '6':
            *pstroke0 = cPolyLetter::N60f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case '7':
            *pstroke0 = cPolyLetter::N70f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case '8':
            *pstroke0 = cPolyLetter::N80f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case '9':
            *pstroke0 = cPolyLetter::N90f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case '-':
            *pstroke0 = cPolyLetter::MINUSf;
            *pstroke1 = NULL;
            *pstroke2 = NULL;  
            break;
                                        
        case '.':
            *pstroke0 = cPolyLetter::DOTf;
            *pstroke1 = NULL;
            *pstroke2 = NULL;  
            break;
                                        
        case ' ':
            *pstroke0 = NULL;
            *pstroke1 = NULL;
            *pstroke2 = NULL;  
            break;
                
        case '\n':
            mLeftToRightIndex=-1;
            mTopToBottomIndex++;
            *pstroke0 = NULL;
            *pstroke1 = NULL;
            *pstroke2 = NULL;
            break;

        case '\t':
            mLeftToRightIndex = mTabIndex * tabSize;
            mTabIndex++;
            break;

        case '\v':
            {
               int remainder = mTopToBottomIndex % tabSize;
               if (remainder == 0) remainder = tabSize;
               mTopToBottomIndex += remainder;
            }
            break;

        // case '\x1':
            // mLeftToRightIndex = tabSize * 1;
            // break;

        // case '\x2':
            // mLeftToRightIndex = tabSize * 2;
            // break;

        // case '\x4':
            // mLeftToRightIndex = tabSize * 4;
            // break;

        // case '\x6':
            // mLeftToRightIndex = tabSize * 6;
            // break;

        // case '\x8':
            // mLeftToRightIndex = tabSize * 8;
            // break;

        default:
            *pstroke0 = cPolyLetter::ERROR0f;
            *pstroke1 = NULL;
            *pstroke2 = NULL;                    
            break;
    }
}

}