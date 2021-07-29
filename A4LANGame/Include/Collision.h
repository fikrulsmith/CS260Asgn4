/*/* Start Header **************************************************************/
/*!
\file GameState_Asteroids.h
\author Fikrul Islami Bin Abdullah, f.abdullah, 440005019
\par f.abdullah\@digipen.edu
\date February 09, 2020
\brief This *.h file contains structure of AABB and its collision function
	   declaration.

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header ****************************************************************/

#ifndef CS230_COLLISION_H_
#define CS230_COLLISION_H_

//#include "AEEngine.h"

/**************************************************************************/
/*!
	Struct of AAB; The minimum and maximum point of the rectangle.
*/
/**************************************************************************/
struct AABB
{
	//AEVec2	c; // center
	//float  r[2]; // holds half width and half height
	
	AEVec2	min;
	AEVec2	max;
};

bool CollisionIntersection_RectRect(const AABB & aabb1, const AEVec2 & vel1, 
									const AABB & aabb2, const AEVec2 & vel2);


#endif // CS230_COLLISION_H_