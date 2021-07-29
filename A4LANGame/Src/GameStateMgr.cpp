/* Start Header **************************************************************/
/*!
\file GameStateMgr.cpp
\author Fikrul Islami Bin Abdullah, f.abdullah, 440005019
\par f.abdullah\@digipen.edu
\date February 09, 2020
\brief This *.cpp file contains the variables and definition that handles
	   the transition of game states within the game.

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header ****************************************************************/

#include "pch.h"
#include "main.h"

void GameStateManager::GameSystemUpdate()
{
	if ((gGameStateCurr == GS_RESTART) || (gGameStateCurr == GS_QUIT))
		return;

	switch (gGameStateCurr)
	{
	case GS_ASTEROIDS:
		GS_FunctionMap_[GameSystemType::LOAD]
			= std::bind(&AsteroidsGameState::GameStateAsteroidsLoad, Asteroids_);
		GS_FunctionMap_[GameSystemType::INIT]
			= std::bind(&AsteroidsGameState::GameStateAsteroidsInit, Asteroids_);
		GS_FunctionMap_[GameSystemType::UPDATE]
			= std::bind(&AsteroidsGameState::GameStateAsteroidsUpdate, Asteroids_);
		GS_FunctionMap_[GameSystemType::DRAW]
			= std::bind(&AsteroidsGameState::GameStateAsteroidsDraw, Asteroids_);
		GS_FunctionMap_[GameSystemType::FREE]
			= std::bind(&AsteroidsGameState::GameStateAsteroidsFree, Asteroids_);
		GS_FunctionMap_[GameSystemType::UNLOAD]
			= std::bind(&AsteroidsGameState::GameStateAsteroidsUnload, Asteroids_);
		break;
	default:
		AE_FATAL_ERROR("invalid state!!");
	}
}

void GameStateManager::Init(unsigned int gameStateInit)
{
	// set the initial game state
	gGameStateInit = gameStateInit;

	// reset the current, previoud and next game
	gGameStateCurr = gGameStateInit;
	gGameStatePrev = gGameStateInit;
	gGameStateNext = gGameStateInit;

	// call the update to set the function pointers
	GameSystemUpdate();
}

void GameStateManager::GameStateLoad()
{
	GS_FunctionMap_[GameSystemType::LOAD]();
}

void GameStateManager::GameStateInit()
{
	GS_FunctionMap_[GameSystemType::INIT]();
}

void GameStateManager::GameStateUpdate()
{
	GS_FunctionMap_[GameSystemType::UPDATE]();
}

void GameStateManager::GameStateDraw()
{
	GS_FunctionMap_[GameSystemType::DRAW]();
}

void GameStateManager::GameStateFree()
{
	GS_FunctionMap_[GameSystemType::FREE]();
}

void GameStateManager::GameStateUnload()
{
	GS_FunctionMap_[GameSystemType::UNLOAD]();
}

unsigned int GameStateManager::GetGameStateInitIndex() const
{
	return gGameStateInit;
}

unsigned int GameStateManager::GetGameStateCurrIndex() const
{
	return gGameStateCurr;
}

unsigned int GameStateManager::GetGameStatePrevIndex() const
{
	return gGameStatePrev;
}

unsigned int GameStateManager::GetGameStateNextIndex() const
{
	return gGameStateNext;
}

void GameStateManager::SetGameStateInitIndex(unsigned int index)
{
	gGameStateInit = index;
}

void GameStateManager::SetGameStateCurrIndex(unsigned int index)
{
	gGameStateCurr = index;
}

void GameStateManager::SetGameStatePrevIndex(unsigned int index)
{
	gGameStatePrev = index;
}

void GameStateManager::SetGameStateNextIndex(unsigned int index)
{
	gGameStateNext = index;
}
