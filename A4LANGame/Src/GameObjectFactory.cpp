#pragma once

#include "pch.h"
#include "GameObjectFactory.h"
#include "AEEngine.h"

GameObjectFactory::GameObjectFactory() :
	sGameObjNum{ 0 }, sGameObjInstNum{ 0 }
{}

GameObjInst* GameObjectFactory::gameObjInstCreate(unsigned long type, float scale, AEVec2* pPos, AEVec2* pVel, float dir)
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

			// return the newly created instance
			return pInst;
		}
	}

	// cannot find empty slot => return 0
	return 0;
}

void GameObjectFactory::gameObjInstDestroy(GameObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	pInst->flag = 0;
}

GameObj* GameObjectFactory::GetNextGameObjectPointer()
{
	GameObj* temp = sGameObjList + sGameObjNum++;
	return temp;
}

GameObj* GameObjectFactory::GetGameObjData()
{
	return sGameObjList;
}

GameObjInst* GameObjectFactory::GetGameObjInstData()
{
	return sGameObjInstList;
}

unsigned long GameObjectFactory::GetGameObjSize() const
{
	return sGameObjNum;
}

unsigned long GameObjectFactory::GetGameObjInstSize() const
{
	return sGameObjInstNum;
}

void GameObjectFactory::SetGameObjSize(unsigned long size)
{
	sGameObjNum = size;
}

void GameObjectFactory::SetGameObjInstSize(unsigned long size)
{
	sGameObjInstNum = size;
}
