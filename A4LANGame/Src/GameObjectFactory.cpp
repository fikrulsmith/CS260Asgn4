/******************************************************************************/
/*!
\file GameObjectFactory.cpp
\author Fikrul Islami Bin Abdullah
\par email: f.abdullah\@digipen.edu
\par DigiPen login: f.abdullah
\par Course: CS260-B
\par Assignment #04
\date 10/08/2021
\brief
This file contains an implementation of a factory settings that manages
creation and destruction of game objects.
*/
/******************************************************************************/
#pragma once

#include "pch.h"
#include "GameObjectFactory.h"
#include "AEEngine.h"
/******************************************************************************/
/*!
\fn GameObjectFactory::GameObjectFactory()
\brief GameObjectFactory Default Constructor
*/
/******************************************************************************/
GameObjectFactory::GameObjectFactory() :
	sGameObjNum{ 0 }, sGameObjInstNum{ 0 }
{}
/******************************************************************************/
/*!
\fn GameObjInst* GameObjectFactory::gameObjInstCreate(unsigned long type,
	float scale, AEVec2* pPos, AEVec2* pVel, float dir)
\brief Creation and initialization of Game Objects
\param type
\param scale
\param pPos
\param pVel
\param dir
\return Address of Game Object Instance
*/
/******************************************************************************/
GameObjInst* GameObjectFactory::gameObjInstCreate(unsigned long type, 
	float scale, AEVec2* pPos, AEVec2* pVel, float dir)
{
	AEVec2 zero;
	AEVec2Zero(&zero);

	AE_ASSERT_PARM(type < sGameObjNum);

	// loop through the object instance list to find a non-used object instance
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject = sGameObjList + type;
			pInst->flag = FLAG_ACTIVE;
			pInst->scale = scale;
			pInst->posCurr = pPos ? *pPos : zero;
			pInst->velCurr = pVel ? *pVel : zero;
			pInst->dirCurr = dir;

			if (type == TYPE_SHIP)
			{
				pInst->shipComp.InitialDirection = dir;
				pInst->shipComp.InitialPosition = pPos ? *pPos : zero;
			}

			// return the newly created instance
			return pInst;
		}
	}

	// cannot find empty slot => return 0
	return 0;
}
/******************************************************************************/
/*!
\fn void GameObjectFactory::gameObjInstDestroy(GameObjInst* pInst)
\brief Destruction of Game Objects
\param pInst
*/
/******************************************************************************/
void GameObjectFactory::gameObjInstDestroy(GameObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	pInst->flag = 0;
}
/******************************************************************************/
/*!
\fn void GameObjectFactory::gameObjInstDestroy(GameObjInst* pInst)
\brief Iterates to the next game object pointer in the list
\return Address of Game Object
*/
/******************************************************************************/
GameObj* GameObjectFactory::GetNextGameObjectPointer()
{
	GameObj* temp = sGameObjList + sGameObjNum++;
	return temp;
}
/******************************************************************************/
/*!
\fn GameObj* GameObjectFactory::GetGameObjData()
\brief Get the list of Game Objects
\return Address of Game List
*/
/******************************************************************************/
GameObj* GameObjectFactory::GetGameObjData()
{
	return sGameObjList;
}
/******************************************************************************/
/*!
GameObjInst* GameObjectFactory::GetGameObjInstData()
\brief Get the list of Game Objects Instance
\return Address of Game List Instance
*/
/******************************************************************************/
GameObjInst* GameObjectFactory::GetGameObjInstData()
{
	return sGameObjInstList;
}
/******************************************************************************/
/*!
\fn unsigned long GameObjectFactory::GetGameObjSize() const
\brief Get the numbers of game objects
\return sGameObjNum
*/
/******************************************************************************/
unsigned long GameObjectFactory::GetGameObjSize() const
{
	return sGameObjNum;
}
/******************************************************************************/
/*!
\fn unsigned long GameObjectFactory::GetGameObjInstSize() const
\brief Get the numbers of game objects
\return sGameObjInstNum
*/
/******************************************************************************/
unsigned long GameObjectFactory::GetGameObjInstSize() const
{
	return sGameObjInstNum;
}
/******************************************************************************/
/*!
\fn void GameObjectFactory::SetGameObjSize(unsigned long size)
\brief Set the numbers of game objects
\param size
*/
/******************************************************************************/
void GameObjectFactory::SetGameObjSize(unsigned long size)
{
	sGameObjNum = size;
}
/******************************************************************************/
/*!
\fn void GameObjectFactory::SetGameObjInstSize(unsigned long size)
\brief Set the numbers of game object instances
\param size
*/
/******************************************************************************/
void GameObjectFactory::SetGameObjInstSize(unsigned long size)
{
	sGameObjInstNum = size;
}
