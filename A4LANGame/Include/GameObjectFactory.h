/******************************************************************************/
/*!
\file GameObjectFactory.h
\author Fikrul Islami Bin Abdullah
\par email: f.abdullah\@digipen.edu
\par DigiPen login: f.abdullah
\par Course: CS260-B
\par Assignment #04
\date 10/08/2021
\brief
This file contains the declarations of the implementation of a factory settings 
that manages creation and destruction of game objects.
*/
/******************************************************************************/
#pragma once

#include "Global.h"
#include "AEVec2.h"

class GameObjectFactory
{
public:
	GameObjectFactory();
	GameObjInst* gameObjInstCreate(unsigned long type, float scale,
		AEVec2* pPos, AEVec2* pVel, float dir);
	void gameObjInstDestroy(GameObjInst* pInst);
	GameObj* GetNextGameObjectPointer();
	GameObj* GetGameObjData();
	GameObjInst* GetGameObjInstData();

	unsigned long GetGameObjSize() const;
	unsigned long GetGameObjInstSize() const;
	void SetGameObjSize(unsigned long size);
	void SetGameObjInstSize(unsigned long size);
private:
	GameObj sGameObjList[GAME_OBJ_NUM_MAX] = { 0 };
	GameObjInst sGameObjInstList[GAME_OBJ_INST_NUM_MAX] = { 0 };
	unsigned long sGameObjNum;
	unsigned long sGameObjInstNum;
};
