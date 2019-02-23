// cGlumShapeFactoryState.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CGLUMSHAPEFACTORYSTATE_H
#define CGLUMSHAPEFACTORYSTATE_H

#include "cObjectMoveableBase.h"
#include "cGlumShapeBase.h"
#include "TimerWrapper.h"
#include <map>
#include <vector>

namespace Glumer
{

class cGlumShapeFactoryState
{
public:
   typedef std::map<unsigned int/*glum shape ID*/, cGlumShapeBase::PTR> GlumShapeMap;
   typedef std::vector<unsigned int/*glum shape ID*/> FactoryList;

   typedef std::map<unsigned int/*glum shape ID*/, cObjectMoveableBase::PTR> DrawListMap;
   typedef std::vector<cObjectMoveableBase::PTR> DrawList;
   typedef std::vector<unsigned int/*glum Id*/> DrawListIndex;

   typedef std::vector<cObjectBase *> SelectableDrawList;
   typedef std::vector<unsigned int/*glum Id*/> SelectableDrawListIndex;

private:
   TimerWrapper::cMutexWrapper mGlumShapeMapLock;
   GlumShapeMap mGlumShapeMap;
   FactoryList mGlumShapeList;   
   
   // these two are unguarded
   DrawListMap mDrawListMap;
   DrawList mDrawList;
   DrawListIndex mDrawListIndex;

   TimerWrapper::cMutexWrapper mSelectableDrawListLock;
   SelectableDrawList mSelectableDrawList;
   SelectableDrawListIndex mSelectableDrawListIndex;

public:
   inline TimerWrapper::cMutexWrapper *GetGlumShapeMapLock(void) { return &mGlumShapeMapLock; };
   inline GlumShapeMap &GetGlumShapeMap(void) { return mGlumShapeMap; };
   inline FactoryList &GetGlumShapeList(void) { return mGlumShapeList; };

   inline DrawListMap &GetDrawListMap(void) { return mDrawListMap;  };
   inline DrawList &GetDrawList(void) { return mDrawList; };

   inline TimerWrapper::cMutexWrapper *GetSelectableDrawListLock(void) { return &mSelectableDrawListLock; };
   inline SelectableDrawList &GetSelectableDrawList(void) { return mSelectableDrawList; }
   inline SelectableDrawListIndex &GetSelectableDrawListIndex(void) { return mSelectableDrawListIndex; }

   cGlumShapeFactoryState(unsigned int reserveSize) 
      : mGlumShapeMapLock(), mGlumShapeMap(), mGlumShapeList(), mDrawListMap(), mDrawList(), mDrawListIndex(),
      mSelectableDrawListLock(), mSelectableDrawList(), mSelectableDrawListIndex()
   {
      mGlumShapeList.reserve(reserveSize);
      mDrawList.reserve(reserveSize);
   }

   inline void FactoryListAdd(cGlumShapeBase::PTR gsbPimp)
   {  
      TimerWrapper::cMutexWrapper::Lock lock(GetGlumShapeMapLock()); 
      mGlumShapeMap[gsbPimp.ptr->GetID()] = gsbPimp;
      mGlumShapeList.push_back(gsbPimp.ptr->GetID());
   }
   inline void FactoryListDelete(cGlumShapeBase::PTR, unsigned int glumId)
   {
      TimerWrapper::cMutexWrapper::Lock lock(GetGlumShapeMapLock()); 
      GlumShapeMap::iterator it = mGlumShapeMap.find(glumId);
      if (it == mGlumShapeMap.end()) return;
      mGlumShapeMap.erase(it);

      std::vector<unsigned int>::iterator intIt = std::find(mGlumShapeList.begin(), mGlumShapeList.end(), glumId);
      std::swap(*intIt, mGlumShapeList.back());
	   mGlumShapeList.pop_back();
   }

   inline void DrawListAdd(cObjectMoveableBase::PTR add, unsigned int glumId)
   {
	  mDrawListMap[glumId] = add;
      mDrawList.push_back(add);
      mDrawListIndex.push_back(glumId);
   }
   void DrawListDelete(cObjectMoveableBase::PTR deleteMe, unsigned int glumId)
   {
      std::vector<cObjectMoveableBase::PTR>::iterator it = mDrawList.begin();
      for (; it != mDrawList.end(); it++) 
         if (it->ptr == deleteMe.ptr) 
            break;
      if (it == mDrawList.end()) return;
      mDrawList.erase(it);

      std::vector<unsigned int>::iterator intIt = std::find(mDrawListIndex.begin(), mDrawListIndex.end(), glumId);
      std::swap(*intIt, mDrawListIndex.back());
	  mDrawListIndex.pop_back();
	  mDrawListMap.erase(glumId);
   }

   inline void SelectableDrawListAdd(cObjectBase *add, unsigned int glumId)
   {
      TimerWrapper::cMutexWrapper::Lock lock(GetSelectableDrawListLock());
      mSelectableDrawList.push_back(add);
      mSelectableDrawListIndex.push_back(glumId);
   }
   void SelectableDrawListDelete(cObjectBase *deleteMe, unsigned int glumId)
   {
      TimerWrapper::cMutexWrapper::Lock lock(GetSelectableDrawListLock());

      std::vector<cObjectBase *>::iterator it = mSelectableDrawList.begin();
      for (; it != mSelectableDrawList.end(); it++) 
         if (*it == deleteMe) 
            break;
      if (it == mSelectableDrawList.end()) 
         return;
      std::swap(*it, mSelectableDrawList.back());
      mSelectableDrawList.pop_back();
   
      std::vector<unsigned int>::iterator intIt = std::find(mSelectableDrawListIndex.begin(), mSelectableDrawListIndex.end(), glumId);
      std::swap(*intIt, mSelectableDrawListIndex.back());
	   mSelectableDrawListIndex.pop_back();
   }
};

}

#endif