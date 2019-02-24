// BubbleFindCollisions.h
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)

#ifndef BUBPACTUTILS_H
#define BUBPACTUTILS_H
//#include "BubbleBubble.h"
#include "BubbleDimensionCracker.h"
//#include "BubbleTrace.h"
#include "cGlumShapeBase.h"
#include "cMovement_Camera.h"
#include <vector>
#include <algorithm>
#include <map>
#include <functional>

namespace Bubbles
{

class cBubbleFindCollisions //: public std::unary_function<Glumer::cGlumShapeBase::PTR, void> 
{
private:
	inline static bool TRILATERATION_DATA_abs_dist_LessThan(Bubbles::cBubbleDimensionCracker::TRILATERATION_DATA &lhs, 
		Bubbles::cBubbleDimensionCracker::TRILATERATION_DATA &rhs)
	{
		return lhs.abs_dist < rhs.abs_dist ||
			(lhs.id == rhs.id && lhs.axis < rhs.axis /*ensure result order of X, Y, Z*/);
	}

	inline static bool TRILATERATION_DATA_id_LessThan(const Bubbles::cBubbleDimensionCracker::TRILATERATION_DATA &lhs, 
		const Bubbles::cBubbleDimensionCracker::TRILATERATION_DATA &rhs)
	{
		return lhs.id < rhs.id ||
			(lhs.id == rhs.id && lhs.axis < rhs.axis /*ensure result order of X, Y, Z*/);
	}

	inline static bool TRILATERATION_DATA_id_Equals(Bubbles::cBubbleDimensionCracker::TRILATERATION_DATA &lhs, 
		Bubbles::cBubbleDimensionCracker::TRILATERATION_DATA &rhs)
	{
		return lhs.id == rhs.id;
	}

   class OutsideRadius : public std::binary_function<cBubbleDimensionCracker::TRILATERATION_DATA, float, bool>
   {
      public:
         inline result_type operator () (const first_argument_type& otherThing, const second_argument_type& width) const
         {
			if (otherThing.dist < -2) 
				return true;
            else 
				return width < (otherThing.abs_dist - otherThing.width);
         };
   };

  
   unsigned int mClippingWidth;
   const std::vector<Glumer::cGlumShapeBase::PTR> &mGroupList;
   std::vector<cBubbleDimensionCracker::COLLISION_RESULT> &mCollisionResults;
   std::vector<cBubbleDimensionCracker::TRILATERATION_DATA> &mDistanceList;
   bool &mAbort;

public:
   cBubbleFindCollisions(
         unsigned int clippingWidth,
         const std::vector<Glumer::cGlumShapeBase::PTR> &collisionList, 
         std::vector<cBubbleDimensionCracker::TRILATERATION_DATA> &dlist,
         std::vector<cBubbleDimensionCracker::COLLISION_RESULT> &results,
         bool &abort) : mClippingWidth(clippingWidth), mGroupList(collisionList), mCollisionResults(results), mDistanceList(dlist), mAbort(abort) {};

   /* take distance units and judge if a collision has been made with the given clipping width */
   static void GetCollisionResults(std::vector<cBubbleDimensionCracker::COLLISION_RESULT> &results,
	   std::vector<cBubbleDimensionCracker::TRILATERATION_DATA> &distanceList, Glumer::cMovement_Camera::PTR &camera, float radius)
   {
	   std::vector<cBubbleDimensionCracker::TRILATERATION_DATA>::iterator new_last =
		   std::remove_if(distanceList.begin(), distanceList.end(), std::bind2nd(OutsideRadius(), radius));

	   cBubbleDimensionCracker::COLLISION_RESULT found_result;
	   if (new_last != distanceList.begin())
	   {
		   /* how many collisions .. it's only a collision if a point in all of it's three dimensions hit
			  so discount those that only have one or two dimension collisions */

			  // sort in order of ID value to cause grouping
		   std::sort(distanceList.begin(), new_last,
			   TRILATERATION_DATA_id_LessThan);

		   // find a sequence of 3 of the same ID's in a row
		   std::vector<cBubbleDimensionCracker::TRILATERATION_DATA>::iterator unit_iterator;

		   unsigned int previous_id = 0;
		   bool have_previous_id = false;
		   unsigned int id_count = 1;

		   for (unit_iterator = distanceList.begin(); unit_iterator != new_last; unit_iterator++)
		   {
			   if (have_previous_id && previous_id == unit_iterator->id)
			   {
				   id_count++;
			   }
			   else
			   {
				   have_previous_id = true;
				   previous_id = unit_iterator->id;
				   id_count = 1;
			   }

			   found_result.mDistanceUnits[id_count - 1] = *unit_iterator;

			   if (id_count == 3)
			   {
				   //an ID has occured three times, one for each axis so this is a collision
				   found_result.mCenterID = camera.ptr->GetID();
				   results.push_back(found_result);
			   }
		   }
	   }
   };

   //inline result_type operator () (const argument_type& center) const
   //{ 
   //   try
   //   {
   //      if (mAbort) throw -999;
   //      mDistanceList.clear();

   //      std::for_each(mGroupList.begin(), mGroupList.end(),                   
   //         std::bind1st(cBubbleDimensionCracker(mDistanceList, mAbort), center));

   //      GetCollisionResults(mCollisionResults, mDistanceList, center.ptr->GetID(), mClippingWidth);
   //   }
   //   catch (int code)
   //   {
   //      throw code;
   //   }
   //};
};

}

#endif