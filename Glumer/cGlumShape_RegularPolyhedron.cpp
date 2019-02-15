#include "cGlumShape_RegularPolyhedron.h"

namespace Glumer
{

	void cGlumShape_RegularPolyhedron::Start(cMovementBase *world_offset)
	{
		if (mTimer != NULL) throw - 943;

		cMovementBase::PTR world_offset_ptr;
		world_offset_ptr.ptr = world_offset;
		mMovementStack.insert(mMovementStack.begin(), world_offset_ptr);

		static const float lineWidths[] = { 0.1f, 0.2f, 0.4f, 0.8f, 1.6f, 3.2f };

		float radius = GetRadius();
		// make larger polyhedrons have thicker lines so they're easier to differenciate from small slow rocks close up
		mLineWidth = radius < 8.0f ? lineWidths[0] :
			radius < 16.0f ? lineWidths[1] :
			radius < 32.0f ? lineWidths[2] :
			radius < 64.0f ? lineWidths[3] :
			radius < 128.0f ? lineWidths[4] :
			lineWidths[5];

		//mTimer = new cTimer_RockMotion(this, mDelay);
	}

}