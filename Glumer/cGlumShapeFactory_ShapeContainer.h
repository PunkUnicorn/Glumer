// cGlumShapeFactory_ShapeContainer.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CGLUMSHAPEFACTORY_SHAPECONTAINER_H
#define CGLUMSHAPEFACTORY_SHAPECONTAINER_H

#include "TimerWrapper.h"
#include "cGlumShapeBase.h"
#include "cHUD_Colour.h"
#include <vector>

namespace Glumer 
{

template <class T_GAMESHAPE>
class cGlumShapeFactory_ShapeContainer
{
private:
   class cBlank : public std::unary_function<typename T_GAMESHAPE::PTR, bool>
         { public: bool operator()(typename T_GAMESHAPE::PTR test) { return (test.ptr == NULL); }; };

   static TimerWrapper::cMutexWrapper mReadWriteLock;
   static std::vector<typename T_GAMESHAPE::PTR> mList;
   static std::vector<typename T_GAMESHAPE::PTR> mRecycleBin;

public:
   static inline unsigned int ListSize(void)
   {
      TimerWrapper::cMutexWrapper::Lock lock(&mReadWriteLock);
      return mList.size();
   }

   //shit is fucked and bullshit. should be an abstract class screw you guys im going home
   cGlumShapeFactory_ShapeContainer(void) {};
   ~cGlumShapeFactory_ShapeContainer(void) {};

   static void Purge(void)
   {
      std::vector<typename T_GAMESHAPE::PTR> deleteList;
      std::vector<typename T_GAMESHAPE::PTR>::iterator deleteIt = mRecycleBin.begin();
      Uint32 now = SDL_GetTicks();
      for (; deleteIt != mRecycleBin.end(); deleteIt++)
      {
         if (deleteIt->ptr == NULL) continue;
         static const int FiveSeconds = 5000;
         if (deleteIt->ptr->GetDeleteAt() + FiveSeconds > now)
         {
            deleteList.push_back(*deleteIt);
            deleteIt->ptr = NULL;
         }
      }
      if (deleteList.size() > 0)
      {
         deleteIt = deleteList.begin();
         for (; deleteIt != deleteList.end(); deleteIt++)
         {
            T_GAMESHAPE *deleteMe = dynamic_cast<T_GAMESHAPE*>(deleteIt->ptr);
            delete deleteMe;
         }
      }
   }

   static typename T_GAMESHAPE::PTR CreateNew(unsigned int id, cHUD_Colour *hud_colour)
   {
		T_GAMESHAPE::PTR new_shape;
		new_shape.ptr = new T_GAMESHAPE(id, hud_colour);

		// lock scope
		{
			TimerWrapper::cMutexWrapper::Lock lock(&mReadWriteLock);
			mList.push_back(new_shape);
			Purge();
		}

		return new_shape;
   }

   static void Delete(typename std::vector<typename T_GAMESHAPE::PTR>::iterator deleteMe)
   {
      TimerWrapper::cMutexWrapper::Lock lock(&mReadWriteLock);
      typename T_GAMESHAPE::PTR blank = { NULL };

      std::vector<typename T_GAMESHAPE::PTR>::iterator freeRecycleSlot = 
               std::find_if(mRecycleBin.begin(), mRecycleBin.end(), cBlank());

      if (freeRecycleSlot == mRecycleBin.end())
         mRecycleBin.push_back(*deleteMe);
      else
         mRecycleBin.emplace(freeRecycleSlot, *deleteMe);

      std::swap(*deleteMe, mList.back());
	   mList.pop_back();
   }

   static void Start(cMovementBase *world_offset)
   {
      for (std::vector<typename T_GAMESHAPE::PTR>::iterator item = mList.begin();
         item != mList.end(); item++)
      {
         item->ptr->Start(world_offset);
      }
   }

   // return bool and take a pointer or something instead of this retarded way round
   static typename std::vector<typename T_GAMESHAPE::PTR>::iterator Get(unsigned int id, bool &haveFound)
   {
	   typename std::vector<typename T_GAMESHAPE::PTR>::iterator found;

      // lock scope
	   {
         TimerWrapper::cMutexWrapper::Lock lock(&mReadWriteLock);
		   found = std::find(mList.begin(), mList.end(), id);
		   haveFound = found != mList.end() && found->ptr->IsMarkedForDelete() == false;
         
         /*std::vector<typename T_GAMESHAPE::PTR> deleteList;
         std::vector<typename T_GAMESHAPE::PTR>::iterator deleteIt = mRecycleBin.begin();
         for (; deleteIt != mRecycleBin.end(); deleteIt++)
         {
            if (deleteIt->ptr == NULL) continue;
            static const int FiveSeconds = 5000;
            if (deleteIt->ptr->GetDeleteAt() + FiveSeconds > SDL_GetTicks())
            {
               deleteList.push_back(*deleteIt);
               deleteIt->ptr = NULL;
            }
         }
         if (deleteList.size() > 0)
         {
            deleteIt = deleteList.begin();
            for (; deleteIt != deleteList.end(); deleteIt++)
            {
               delete (T_GAMESHAPE*) deleteIt->ptr;
            }
         }*/
      }
	   return found;
   }
};

}

#endif