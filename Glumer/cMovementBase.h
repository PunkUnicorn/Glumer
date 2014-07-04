// cMovementBase.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#ifndef CMOVEMENTBASE_H
#define CMOVEMENTBASE_H

namespace Glumer
{

/*
*/
class cMovementBase 
{
private:
   bool mIgnore;

public:
   inline bool GetIgnore() const { return mIgnore; };
   inline void SetIgnore(bool value) { mIgnore = value; };

   typedef struct 
   {
      cMovementBase *ptr;
   } PTR;
        
   inline cMovementBase(void) : mIgnore(false) {};
   virtual ~cMovementBase(void) {};

virtual void EventPossition(void) = 0; //function to set up the translate, rotate, etc
};

}

#endif