/******************************************************************************/
/*!
\file Collision.h
\author Fikrul Islami Bin Abdullah
\par email: f.abdullah\@digipen.edu
\par DigiPen login: f.abdullah
\par Course: CS260-B
\par Assignment #04
\date 10/08/2021
\brief
This file contains the declaration fo the implementation of AABB collision.
*/
/******************************************************************************/
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
