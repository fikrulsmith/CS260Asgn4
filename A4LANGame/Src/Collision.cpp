/******************************************************************************/
/*!
\file Collision.cpp
\author Fikrul Islami Bin Abdullah
\par email: f.abdullah\@digipen.edu
\par DigiPen login: f.abdullah
\par Course: CS260-B
\par Assignment #04
\date 10/08/2021
\brief
This file contains an implementation of AABB collision.
*/
/******************************************************************************/
#include "pch.h"
#include "main.h"
/******************************************************************************/
/*!
\fn bool CollisionIntersection_RectRect(const AABB& aabb1, const AEVec2& vel1,
    const AABB& aabb2, const AEVec2& vel2)
\brief Handles the collision between 2 rectangular bounding boxes based on 
	   their maximum and minimum points.
\param aabb1
\param vel1
\param aabb2
\param vel1
\return True if collide, otherwise, false
*/
/******************************************************************************/
bool CollisionIntersection_RectRect(const AABB& aabb1, const AEVec2& vel1,
    const AABB& aabb2, const AEVec2& vel2)
{
    //Static check if intersecting.
    if (aabb1.min.x > aabb2.max.x);
    else if (aabb2.min.x > aabb1.max.x);
    else if (aabb1.max.y < aabb2.min.y);
    else if (aabb2.max.y < aabb1.min.y);
    else
    {
        return 1;
    }

    float tFirst = 0;       //First time it will collide. Init to 0 as earliest possible time.
    float tLast = g_dt;     //Last time it will collide. Init to delta time as latest possible time.
    AEVec2 velRelative;
    AEVec2 _vel1 = vel1;    //Retrieve velcties for object 1 and 2
    AEVec2 _vel2 = vel2;
    AEVec2Sub(&velRelative, &_vel1, &_vel2); // Calculate relative velocity.

    //Start with checking on X axis
    if (velRelative.x < 0)
    {
        //Case 1. If moving away from each other.
        if (aabb1.min.x > aabb2.max.x)
        {
            return 0;
        }
        //Case 4. If moving towards each other.
        if (aabb1.max.x < aabb2.min.x)
        {
            float temp = (aabb1.max.x - aabb2.min.x) / velRelative.x;
            tFirst = max(temp, tFirst);
            tLast = min(temp, tLast);
        }
    }

    if (velRelative.x > 0)
    {
        //Case 3. Moving away from each other.
        if (aabb1.max.x < aabb2.min.x)
        {
            return 0;
        }
        //Case 2. If moving towards each other.
        if (aabb1.min.x > aabb2.max.x)
        {
            float temp = (aabb1.min.x - aabb2.max.x) / velRelative.x;
            tFirst = max(temp, tFirst);
            temp = (aabb1.max.x - aabb2.min.x) / velRelative.x;
            tLast = min(temp, tLast);
        }
    }
    //If 2 AABBs will never collide on x Axis
    if (velRelative.x == 0)
    {
        return 0;
    }

    if (tFirst > tLast)
    {
        return 0;
    }

    //Now check along Y axis

    if (velRelative.y < 0)
    {
        //Case 1. If moving away from each other.
        if (aabb1.min.y > aabb2.max.y)
        {
            return 0;
        }
        //Case 4. If moving towards each other.
        if (aabb1.max.y < aabb2.min.y)
        {
            float temp = (aabb1.max.y - aabb2.min.y) / velRelative.y;
            tFirst = max(temp, tFirst);
            tLast = min(temp, tLast);
        }
    }

    if (velRelative.y > 0)
    {
        //Case 3. Moving away from each other.
        if (aabb1.max.y < aabb2.min.y)
        {
            return 0;
        }
        //Case 2. If moving towards each other.
        if (aabb1.min.y > aabb2.max.y)
        {
            float temp = (aabb1.min.y - aabb2.max.y) / velRelative.y;
            tFirst = max(temp, tFirst);
            temp = (aabb1.max.y - aabb2.min.y) / velRelative.y;
            tLast = min(temp, tLast);
        }
    }
    //If 2 AABBs will never collide on Y Axis
    if (velRelative.y == 0)
    {
        return 0;
    }
    if (tFirst > tLast)
    {
        return 0;
    }
    //Else it has failed other checks and will collide.
    return 1;
}
