// cSelectableBase.cpp
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)

#include "cSelectableBase.h"
#include "cObjectMoveableBase.h"

#include <Windows.h>
#include <gl/gl.h>   
#include <gl/glu.h> 
#include <algorithm>
//#include <vector>

#include "cGlumShape_Console.h"

namespace Glumer
{

static const float WORLD_RADIUS = 5000.0f;
static const float WORLD_DIAMITER = 10000.0f;

TimerWrapper::cMutexWrapper cSelectableBase::mSelectableLock;

//int cSelectableBase::mNextFreeName=0;
//cSelectableBase* cSelectableBase::mFirst=NULL;

cSelectableBase::cSelectableBase(unsigned int id) 
    :/* mNext(0),*/ mName(id), /*mMeAsBase(NULL), */mMeAsMoveable(NULL)
{
    //a) assign a unique openGL name
    ////mName = ++cSelectableBase::mNextFreeName; now a parameter
            
    //b) facilitate enumerations through an embeded linked list
    /*cSelectableBase **current;
    for (  
            current = &cSelectableBase::mFirst; 
            *current != NULL; 
            current = & ( (*current)->mNext ) 
        ) {}
            
    *current = this;
    mNext = NULL;*/
}
        
/* EventClick
    called to determain if draw_selectable was clicked on, and if so 
    fires EventClicked .. otherwise it does nothing 

    returns true if an item was clicked
         
    draw_selectable must re-draw all possible selectable world objects

	Thanks to http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=32 (nehe productions, OpenGL tutorials, lesson 32)
	copywrite nehe productions
*/
// static class function
bool cSelectableBase::EventClick(TimerWrapper::cMutexWrapper *lock, std::vector<cObjectBase *> &draw_selectable, std::vector<unsigned int/*glum Id*/> &index, unsigned int mouse_x, unsigned int mouse_y, int mouse_z)
{
    GLint	viewport[4];
    GLuint	*buffer;
    static const unsigned int buffer_size = 512;
            
    // This Sets The Array <viewport> To The Size And Location Of The Screen Relative To The Window
            
    buffer = new GLuint[buffer_size];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(buffer_size, buffer);						// Tell OpenGL To Use Our Array For Selection
            
    (void) glRenderMode(GL_SELECT);
    glInitNames();								// Initializes The Name Stack
    //glPushName(0);								// Push 0 (At Least One Entry) Onto The Stack
        glMatrixMode(GL_PROJECTION);						// Selects The Projection Matrix
        glPushMatrix();								// Push The Projection Matrix
            glLoadIdentity();							// Resets The Matrix
                    
            static const float farDistance = WORLD_RADIUS;
            gluPickMatrix((GLdouble) mouse_x, (GLdouble) (viewport[3]-mouse_y), 1.0f, 1.0f, viewport);
            gluPerspective(45.0f, (GLfloat) (viewport[2]-viewport[0])/(GLfloat) (viewport[3]-viewport[1]), 0.1f, farDistance);//100.0f);
                    
            glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
            glLoadIdentity();

            //lock scope
            try
            {
              TimerWrapper::cMutexWrapper::Lock lock(&mSelectableLock);
							std::for_each(draw_selectable.begin(), draw_selectable.end(), cSelectableBase::FireEventShow);
            }
            catch (...)
            {
               glScalef(0.0f, 0.0f, 0.0f);
            }
            //draw_selectable->EventShow();
                    
			glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
        glPopMatrix();								// Pop The Projection Matrix
        glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
                
        int hits = glRenderMode(GL_RENDER);
                
        int	found_name = 0;
        int	depth = 0;					// Store How Far Away It Is		     
        
        for (int loop = 0; loop < hits; loop++)				// Loop Through All The Detected Hits
        {
            if (found_name == 0)
            {
              found_name = buffer[3];					// Make Our Selection The First Object
              depth = buffer[1];
            }
			   // If This Object Is Closer To Us Than The One We Have Selected
            if (buffer[loop*4+1] < GLuint(depth))
            {
            	found_name = buffer[loop*4+3];			// Select The Closer Object
            	depth = buffer[loop*4+1];			// Store How Far Away It Is
            }       
        }
            
		bool hit = false;
        if (hits != 0 && found_name != 0)
        {     
            TimerWrapper::cMutexWrapper::Lock lock(&mSelectableLock);

            std::vector<unsigned int>::iterator selected;
            std::vector<cObjectBase *>::iterator drawItem = draw_selectable.begin();
            for (
                   selected = index.begin();
            	    selected != index.end();
            	    selected++
                )
            {
               if (*selected == found_name)
				   {
                  cSelectableBase *selectable = dynamic_cast<cSelectableBase *>(*drawItem);
                  if (selectable != NULL)
                  {
					  hit = true;
                     selectable->EventSelected();
                     break; // the EventSelected may have changed the draw_selectable iterator so get out of here
                  }
				   }
               drawItem++;
            }
        }
        
    //glPopName();
    delete[] buffer;
	return hit;
}


inline void cSelectableBase::FireEventShow(cObjectBase *showme)
{
//#ifdef DEBUG
	cSelectableBase *selectable = dynamic_cast<cSelectableBase *>(showme);
//#else
//	cSelectableBase *selectable = (cSelectableBase *)showme;
//#endif

    glLoadIdentity();
     
	   //glPushName(selectable->mName);
         
    selectable->DrawSelectionArea();

	   //glPopName();
}

}