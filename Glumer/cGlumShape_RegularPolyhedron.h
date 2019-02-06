#pragma once
#include "cGlumGlCommand.h"
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
class cGlumShape_RegularPolyhedron : public cObjectMoveableBase, public cGlumShapeBase, public cSelectableBase
{
public:
	typedef struct 
	{
		cGlumShape_RegularPolyhedron *ptr;
		bool operator ==(unsigned int id)
		{
			return ptr->GetID() == id;
		}
	} PTR;

private:
	PolyhedronType mShape;

	inline void Cube(float radius) const
	{
		//glPushMatrix();

			//		 _ |/_
			//		  /|
glVertex3f(  radius, -radius, radius );
glVertex3f(  radius,  radius, radius );
glVertex3f( -radius,  radius, radius );
glVertex3f( -radius, -radius, radius );
 
// Purple side - RIGHT
glVertex3f( radius, -radius, -radius );
glVertex3f( radius,  radius, -radius );
glVertex3f( radius,  radius,  radius );
glVertex3f( radius, -radius,  radius );
//glVertex3f( radius, -radius,  radius );
 
// Green side - LEFT
glVertex3f( -radius, -radius,  radius );
glVertex3f( -radius,  radius,  radius );
glVertex3f( -radius,  radius, -radius );
glVertex3f( -radius, -radius, -radius );
 
// Blue side - TOP
glVertex3f(  radius,  radius,  radius );
glVertex3f(  radius,  radius, -radius );
glVertex3f( -radius,  radius, -radius );
glVertex3f( -radius,  radius,  radius );
 
// Red side - BOTTOM
glVertex3f(  radius, -radius, -radius );
glVertex3f(  radius, -radius,  radius );
glVertex3f( -radius, -radius,  radius );
glVertex3f( -radius, -radius, -radius );
			//for (int i = 0; i < 4; i++)
			//{
			//	//			x,		y,		z
			//	glVertex3f( radius, radius, radius);
			//	glVertex3f( radius, -radius, radius);
			//	glVertex3f( -radius, -radius, radius);
			//	glVertex3f( -radius, radius, radius);
			//	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			//}

		//glPopMatrix();
	}

	void GlCommand(cGlumGlCommand *command, float scale) const
	{
		const int groups_of_three = 3;
		unsigned int len = command->GetFloatCount();
		if (len % groups_of_three > 0)
			len -= len % groups_of_three;

		glBegin(command->GetCommand());

			float const *floats = command->GetFloats();
			for (unsigned int i = 0; i < len; i += groups_of_three)
			{
				glVertex3f(floats[i], floats[i + 1], floats[i + 2]);
			}

		glEnd();
	}
	inline void Octahedron2(float RADIUS) const
	{
		//glBegin(GL_TRIANGLES);


		//original +3
			glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
			glNormal3d(0.577350269189, 0.577350269189, 0.577350269189); glVertex3d(RADIUS, 0.0, 0.0); glVertex3d(0.0, RADIUS, 0.0); glVertex3d(0.0, 0.0, RADIUS);
			glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
			glNormal3d(0.577350269189, 0.577350269189, -0.577350269189); glVertex3d(RADIUS, 0.0, 0.0); glVertex3d(0.0, 0.0, -RADIUS); glVertex3d(0.0, RADIUS, 0.0);
			glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
			glNormal3d(0.577350269189, -0.577350269189, 0.577350269189); glVertex3d(RADIUS, 0.0, 0.0); glVertex3d(0.0, 0.0, RADIUS); glVertex3d(0.0, -RADIUS, 0.0);
		glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		glNormal3d(0.577350269189, -0.577350269189, -0.577350269189); glVertex3d(RADIUS, 0.0, 0.0); glVertex3d(0.0, -RADIUS, 0.0); glVertex3d(0.0, 0.0, -RADIUS);
			glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
			glNormal3d(-0.577350269189, 0.577350269189, 0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0); glVertex3d(0.0, 0.0, RADIUS); glVertex3d(0.0, RADIUS, 0.0);
		glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		//glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		glNormal3d(-0.577350269189, 0.577350269189, -0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0); glVertex3d(0.0, RADIUS, 0.0); glVertex3d(0.0, 0.0, -RADIUS);
		glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		glNormal3d(-0.577350269189, -0.577350269189, 0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0); glVertex3d(0.0, -RADIUS, 0.0); glVertex3d(0.0, 0.0, RADIUS);
		glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		//glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		glNormal3d(-0.577350269189, -0.577350269189, -0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0); glVertex3d(0.0, 0.0, -RADIUS); glVertex3d(0.0, -RADIUS, 0.0);


		////original +2
		//glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		//glNormal3d(0.577350269189, 0.577350269189, 0.577350269189); glVertex3d(RADIUS, 0.0, 0.0); glVertex3d(0.0, RADIUS, 0.0); glVertex3d(0.0, 0.0, RADIUS);
		//glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		//glNormal3d(0.577350269189, 0.577350269189, -0.577350269189); glVertex3d(RADIUS, 0.0, 0.0); glVertex3d(0.0, 0.0, -RADIUS); glVertex3d(0.0, RADIUS, 0.0);
		//glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		//glNormal3d(0.577350269189, -0.577350269189, 0.577350269189); glVertex3d(RADIUS, 0.0, 0.0); glVertex3d(0.0, 0.0, RADIUS); glVertex3d(0.0, -RADIUS, 0.0);
		//glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		//glNormal3d(0.577350269189, -0.577350269189, -0.577350269189); glVertex3d(RADIUS, 0.0, 0.0); glVertex3d(0.0, -RADIUS, 0.0); glVertex3d(0.0, 0.0, -RADIUS);
		//glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		//glNormal3d(-0.577350269189, 0.577350269189, 0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0); glVertex3d(0.0, 0.0, RADIUS); glVertex3d(0.0, RADIUS, 0.0);
		//glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		////glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		//glNormal3d(-0.577350269189, 0.577350269189, -0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0); glVertex3d(0.0, RADIUS, 0.0); glVertex3d(0.0, 0.0, -RADIUS);
		//glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		////glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		//glNormal3d(-0.577350269189, -0.577350269189, 0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0); glVertex3d(0.0, -RADIUS, 0.0); glVertex3d(0.0, 0.0, RADIUS);
		//glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		//glNormal3d(-0.577350269189, -0.577350269189, -0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0); glVertex3d(0.0, 0.0, -RADIUS); glVertex3d(0.0, -RADIUS, 0.0);


		//original +1
		//glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		//glNormal3d(0.577350269189, 0.577350269189, 0.577350269189); glVertex3d(RADIUS, 0.0, 0.0); glVertex3d(0.0, RADIUS, 0.0); glVertex3d(0.0, 0.0, RADIUS);
		//glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		//glNormal3d(0.577350269189, 0.577350269189, -0.577350269189); glVertex3d(RADIUS, 0.0, 0.0); glVertex3d(0.0, 0.0, -RADIUS); glVertex3d(0.0, RADIUS, 0.0);
		//glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		//glNormal3d(0.577350269189, -0.577350269189, 0.577350269189); glVertex3d(RADIUS, 0.0, 0.0); glVertex3d(0.0, 0.0, RADIUS); glVertex3d(0.0, -RADIUS, 0.0);
		//glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		//glNormal3d(0.577350269189, -0.577350269189, -0.577350269189); glVertex3d(RADIUS, 0.0, 0.0); glVertex3d(0.0, -RADIUS, 0.0); glVertex3d(0.0, 0.0, -RADIUS);
		//glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		//glNormal3d(-0.577350269189, 0.577350269189, 0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0); glVertex3d(0.0, 0.0, RADIUS); glVertex3d(0.0, RADIUS, 0.0);
		//glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		//glNormal3d(-0.577350269189, 0.577350269189, -0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0); glVertex3d(0.0, RADIUS, 0.0); glVertex3d(0.0, 0.0, -RADIUS);
		//glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		//glNormal3d(-0.577350269189, -0.577350269189, 0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0); glVertex3d(0.0, -RADIUS, 0.0); glVertex3d(0.0, 0.0, RADIUS);
		//glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		//glNormal3d(-0.577350269189, -0.577350269189, -0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0); glVertex3d(0.0, 0.0, -RADIUS); glVertex3d(0.0, -RADIUS, 0.0);

		// original
		//glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		//glNormal3d(0.577350269189, 0.577350269189, 0.577350269189); glVertex3d(RADIUS, 0.0, 0.0); glVertex3d(0.0, RADIUS, 0.0); glVertex3d(0.0, 0.0, RADIUS);
		//glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		//glNormal3d(0.577350269189, 0.577350269189, -0.577350269189); glVertex3d(RADIUS, 0.0, 0.0); glVertex3d(0.0, 0.0, -RADIUS); glVertex3d(0.0, RADIUS, 0.0);
		//glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		//glNormal3d(0.577350269189, -0.577350269189, 0.577350269189); glVertex3d(RADIUS, 0.0, 0.0); glVertex3d(0.0, 0.0, RADIUS); glVertex3d(0.0, -RADIUS, 0.0);
		//glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		//glNormal3d(0.577350269189, -0.577350269189, -0.577350269189); glVertex3d(RADIUS, 0.0, 0.0); glVertex3d(0.0, -RADIUS, 0.0); glVertex3d(0.0, 0.0, -RADIUS);
		//glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		//glNormal3d(-0.577350269189, 0.577350269189, 0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0); glVertex3d(0.0, 0.0, RADIUS); glVertex3d(0.0, RADIUS, 0.0);
		//glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		//glNormal3d(-0.577350269189, 0.577350269189, -0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0); glVertex3d(0.0, RADIUS, 0.0); glVertex3d(0.0, 0.0, -RADIUS);
		//glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		//glNormal3d(-0.577350269189, -0.577350269189, 0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0); glVertex3d(0.0, -RADIUS, 0.0); glVertex3d(0.0, 0.0, RADIUS);
		//glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		//glNormal3d(-0.577350269189, -0.577350269189, -0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0); glVertex3d(0.0, 0.0, -RADIUS); glVertex3d(0.0, -RADIUS, 0.0);


		//glEnd();
	}
	inline void Octahedron(float radius) const
	{
		glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		glVertex3f( 0.0f, radius, 0.0f);
		glVertex3f( 0.0f, 0.0f, radius);
		glVertex3f( radius, 0.0f, 0.0f);

		glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		glVertex3f( 0.0f, radius, 0.0f);
		glVertex3f( radius, 0.0f, 0.0f);
		glVertex3f( 0.0f, 0.0f, -radius);

		glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		glVertex3f( 0.0f, radius, 0.0f);
		glVertex3f( 0.0f, 0.0f, -radius);
		glVertex3f( -radius, 0.0f, 0.0f);

		glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		glVertex3f( 0.0f, radius, 0.0f);
		glVertex3f( 0.0f, 0.0f, radius);
		glVertex3f( -radius, 0.0f, 0.0f);

		glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		glVertex3f( 0.0f, -radius, 0.0f);
		glVertex3f( 0.0f, 0.0f, radius);
		glVertex3f( radius, 0.0f, 0.0f);

		glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		glVertex3f( 0.0f, -radius, 0.0f);
		glVertex3f( radius, 0.0f, 0.0f);
		glVertex3f( 0.0f, 0.0f, -radius);

		glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
		glVertex3f( 0.0f, -radius, 0.0f);
		glVertex3f( 0.0f, 0.0f, -radius);
		glVertex3f( -radius, 0.0f, 0.0f);

		glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
		glVertex3f( 0.0f, -radius, 0.0f);
		glVertex3f( 0.0f, 0.0f, radius);
		glVertex3f( -radius, 0.0f, 0.0f);
	}

	bool mCompiled;
	GLuint mListIndex;
	inline void Show(float radius) const
	{
		float RADIUS = radius;
		switch (mShape)
		{

		case cPolyhedronType::Cube:

			glLineWidth(mDefaultLineWidth);
			// black inner shape
			glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
			glPushMatrix();
			glScalef(0.92f, 0.92f, 0.92f);
			glBegin(GL_POLYGON);
				Cube(radius);
			glEnd();
			glPopMatrix();

			// coloured outer line graphic shell
			glLineWidth(mLineWidth);
			glColor3ub(0, 0, 0);
			glPushMatrix();
			glScalef(0.929f, 0.929f, 0.929f);
			glBegin(GL_LINE_LOOP);
				Cube(radius);
			glEnd();

			glPopMatrix();

			glLineWidth(mLineWidth / 2.0f);
			glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
			glBegin(GL_LINE_LOOP);
				Cube(radius);
			glEnd();

			glLineWidth(mDefaultLineWidth);
			break;

		case cPolyhedronType::Octahedron:
			glLineWidth(mDefaultLineWidth);
			// black inner shape
			glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
			glPushMatrix();
				glScalef(0.92f, 0.92f, 0.92f);
				glBegin(GL_TRIANGLES);// GL_POLYGON);
					Octahedron2(radius);
				glEnd();
			glPopMatrix();

			// coloured outer line graphic shell
			glLineWidth(mLineWidth);
			glColor3ub(0, 0, 0);
			glPushMatrix();
				glScalef(0.929f, 0.929f, 0.929f);
				glBegin(GL_LINE_LOOP);
					Octahedron2(radius);
				glEnd();
			glPopMatrix();

			glLineWidth(mLineWidth / 2.0f);
			glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
			glBegin(GL_LINE_LOOP);
				Octahedron2(radius);
			glEnd();

			glLineWidth(mDefaultLineWidth);
			break;

			//http://www.glprogramming.com/red/chapter02.html

		}
	};

   cTimer_RockMotion *mTimer;
   unsigned int mDelay;
   float mLineWidth, mDefaultLineWidth;
   float mRadius;
   GlumerOnClicked *mOnClicked;
   cGlumGlCommand *mGlCommand;

public:
	cHUD_Colour *mHUDColour;

	inline cGlumShape_RegularPolyhedron(unsigned int id, cHUD_Colour *hud_colour) 
		: cGlumShapeBase(id), cObjectMoveableBase(), cSelectableBase(id), mShape(cPolyhedronType::Cube), mCompiled(false), mListIndex(0), mTimer(NULL), mDelay(cGlumShapeBase::MOVEMENT_UPDATE_INTERVAL), mLineWidth(1.0f), mDefaultLineWidth(0.1f), mRadius(1.0f), mOnClicked(NULL), mHUDColour(hud_colour), mGlCommand(NULL)
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
	inline PolyhedronType GetShape(void) const { return mShape; }
	inline void FactorySetRadius(float radius) { mRadius = radius; };
	inline void FactorySetOnCliced(GlumerOnClicked *onClicked) { mOnClicked = onClicked; }
	inline void FactorySetDelay(unsigned int delay) { mDelay = delay; }
	inline void FactorySetShape(PolyhedronType type) { mShape = type; }
	inline void FactorySetGlCommand(int GL_BEGIN_MODE_TYPE, float floats[], unsigned int floatCount) { mGlCommand = new cGlumGlCommand(GL_BEGIN_MODE_TYPE, floats, floatCount); };
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

	virtual ~cGlumShape_RegularPolyhedron(void)
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
			float r = GetRadius();
			mCompiled = true;
			mListIndex = glGenLists(1);
			glNewList(mListIndex, GL_COMPILE);

				if (this->mShape == cPolyhedronType::GLCommand)
				{
					GlCommand(this->mGlCommand, r);
				}
				else
				{
					//glLineWidth(mDefaultLineWidth);

					//// black inner shape
					//glColor3ub(mHUDColour->GetSecondRed(), mHUDColour->GetSecondGreen(), mHUDColour->GetSecondBlue());
					//glPushMatrix();
					//	glScalef(0.92f, 0.92f, 0.92f);
					//	glBegin(GL_POLYGON);// GL_TRIANGLE_FAN);
					//		Show(r);// r - mLineWidth);
					//	glEnd();
					//glPopMatrix();

					//// coloured outer line graphic shell
					//glLineWidth(mLineWidth);
					//glColor3ub(0, 0, 0);
					//glPushMatrix();
					//	glScalef(0.929f, 0.929f, 0.929f);
					//	glBegin(GL_LINE_LOOP);
     //    					Show(r);
					//	glEnd();
					//glPopMatrix();

					//glLineWidth(mLineWidth/2.0f);
					//glColor3ub(mHUDColour->m_red, mHUDColour->m_green, mHUDColour->m_blue);
					//glBegin(GL_LINE_LOOP);
					//Show(r);
					//glEnd();

					//glLineWidth(mDefaultLineWidth);
					Show(r);
				}

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