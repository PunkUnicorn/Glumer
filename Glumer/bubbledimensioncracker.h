// BubbleDimensionCracker.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)

#ifndef BUBBLEDIMENSIONCRACKER_H
#define BUBBLEDIMENSIONCRACKER_H
#include <vector>
#include <algorithm>
#include <map>
#include "cGlumShapeBase.h"


//#include "BubbleBubble.h"
//#include "BubbleTrace.h"

namespace Bubbles 
{

/* The purpose of this class is to take a game object with an ID and use subtraction and the less than operator to discover distances of items from each other.
   Find the distance for each of items x, y and z dimensions: each axis on their own individual plane taking a 3D object and reducing it to a series of 1D intersections */
class cBubbleDimensionCracker : public std::unary_function<Glumer::cGlumShapeBase::PTR, void>
{
public:

class cAxisSplitterAXIS { public: enum { X = 0, Y, Z, NoOfDimensions }; };

typedef struct
{
	/*cAxisSplitterAXIS*/int axis; //which axis this represents either the x(0), y(1) or z(2)
	float abs_dist; //absolute distance from id
	//float rel_coord; //value of the axis (value of x, y or z)
	float width;
	unsigned int id;
	float dist;
} TRILATERATION_DATA;

typedef struct
{
	unsigned int mCenterID;
	TRILATERATION_DATA mDistanceUnits[cAxisSplitterAXIS::NoOfDimensions];
} COLLISION_RESULT;

private:
   std::vector<TRILATERATION_DATA> &mDistanceList;
   float mClipWidth;
   std::vector<Glumer::cGlumShapeBase::PTR> &mShapes;
   Glumer::cMovement_Camera::PTR from_this_shape;

public:
   cBubbleDimensionCracker(
	   std::vector<Glumer::cGlumShapeBase::PTR> &shapes,
	   std::vector<TRILATERATION_DATA> &data, 
	   float clipWidth,
	   Glumer::cMovement_Camera::PTR pfrom_this_shape) : mShapes(shapes), mDistanceList(data), mClipWidth(clipWidth), from_this_shape(pfrom_this_shape)
   { };

   inline void MakeAxisUnit(const argument_type &universeCenter, Glumer::cMovement_Camera::PTR from_this_shape,
						float from_this_shapeX, float universeCenterX, TRILATERATION_DATA &new_unit) const
   {
      if (universeCenterX > from_this_shapeX)
         new_unit.abs_dist = (universeCenterX) - (from_this_shapeX);
      else
         new_unit.abs_dist = (from_this_shapeX) - (universeCenterX);
	  new_unit.dist = from_this_shapeX - universeCenterX;
	  mDistanceList.push_back(new_unit);
   }

   /* 
   find the distance units from 'me' to everything else in the universe  
   
std::binary_function<Glumer::cGlumShapeBase::PTR, Glumer::cMovement_Camera::PTR, void>

   */
   inline result_type operator () (argument_type &universeCenter) const
   {
	  // ignore our own shadow
      if (from_this_shape.ptr->GetID() == universeCenter.ptr->GetID()) 
         return;

	  Uint32 now = SDL_GetTicks();

      float from_this_shapeX, from_this_shapeY, from_this_shapeZ, from_this_shapeR;
      float universeCenterX, universeCenterY, universeCenterZ;

	  unsigned int newId = universeCenter.ptr->GetID();
	  from_this_shape.ptr->GetXYZ(from_this_shapeX, from_this_shapeY, from_this_shapeZ);
      from_this_shapeR = mClipWidth;
      universeCenter.ptr->GetCenter().GetXYZ(universeCenterX, universeCenterY, universeCenterZ);

	  /* scope block */
      {
         TRILATERATION_DATA new_unit = {0};
         new_unit.id = newId;
         new_unit.axis = cAxisSplitterAXIS::X;
         new_unit.width = from_this_shapeR;

         MakeAxisUnit(universeCenter, from_this_shape,
            from_this_shapeX,
            universeCenterX,
            new_unit);
      }

	  /* scope block attack of the clones*/
      {
         TRILATERATION_DATA new_unit = {0};
         new_unit.id = newId;
         new_unit.axis = cAxisSplitterAXIS::Y; // difference
         new_unit.width = from_this_shapeR;

         MakeAxisUnit(universeCenter, from_this_shape,
            from_this_shapeY, // difference
            universeCenterY, // difference
            new_unit); // difference
      }


	  /* scope block revenge of the Ctrl-Cth*/
      {
         TRILATERATION_DATA new_unit = {0};
         new_unit.id = newId;
         new_unit.axis = cAxisSplitterAXIS::Z; // difference
         new_unit.width = from_this_shapeR;

         MakeAxisUnit(universeCenter, from_this_shape,
            from_this_shapeZ, // difference
            universeCenterZ, // difference
            new_unit); // difference
      }
   };
};

}

#endif