#pragma once

#include "AEEngine.h"

/**************************************************************************/
/*!
	Struct of AAB; The minimum and maximum point of the rectangle.
*/
/**************************************************************************/
struct AABB
{
	AEVec2	min;
	AEVec2	max;
};

bool CollisionIntersection_RectRect(const AABB & aabb1, const AEVec2 & vel1, 
									const AABB & aabb2, const AEVec2 & vel2);


