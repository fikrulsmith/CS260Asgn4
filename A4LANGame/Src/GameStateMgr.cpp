/******************************************************************************/
/*!
\file GameStateMgr.cpp
\author Fikrul Islami Bin Abdullah
\par email: f.abdullah\@digipen.edu
\par DigiPen login: f.abdullah
\par Course: CS260-B
\par Assignment #04
\date 10/08/2021
\brief
This file contains an implementation of a game state manager.
*/
/******************************************************************************/
#include "pch.h"
#include "main.h"
/******************************************************************************/
/*!
\fn void GameStateManager::GameSystemUpdate()
\brief Update the function pointers for the game state
*/
/******************************************************************************/
void GameStateManager::GameSystemUpdate()
{
	if ((gGameStateCurr == GS_RESTART) || (gGameStateCurr == GS_QUIT))
		return;

	switch (gGameStateCurr)
	{
	case GS_ASTEROIDS:
		GS_FunctionMap_[GameSystemType::LOAD]
			= std::bind(&AsteroidsGameState::GameStateAsteroidsLoad, std::ref(Asteroids_));
		GS_FunctionMap_[GameSystemType::INIT]
			= std::bind(&AsteroidsGameState::GameStateAsteroidsInit, std::ref(Asteroids_));
		GS_FunctionMap_[GameSystemType::UPDATE]
			= std::bind(&AsteroidsGameState::GameStateAsteroidsUpdate, std::ref(Asteroids_));
		GS_FunctionMap_[GameSystemType::DRAW]
			= std::bind(&AsteroidsGameState::GameStateAsteroidsDraw, std::ref(Asteroids_));
		GS_FunctionMap_[GameSystemType::FREE]
			= std::bind(&AsteroidsGameState::GameStateAsteroidsFree, std::ref(Asteroids_));
		GS_FunctionMap_[GameSystemType::UNLOAD]
			= std::bind(&AsteroidsGameState::GameStateAsteroidsUnload, std::ref(Asteroids_));
		break;
	case GS_MAINMENU:
		GS_FunctionMap_[GameSystemType::LOAD]
			= std::bind(&MainMenuGameState::GameStateMainMenuLoad, std::ref(MainMenu_));
		GS_FunctionMap_[GameSystemType::INIT]		  
			= std::bind(&MainMenuGameState::GameStateMainMenuInit, std::ref(MainMenu_));
		GS_FunctionMap_[GameSystemType::UPDATE]		  
			= std::bind(&MainMenuGameState::GameStateMainMenuUpdate, std::ref(MainMenu_));
		GS_FunctionMap_[GameSystemType::DRAW]		  
			= std::bind(&MainMenuGameState::GameStateMainMenuDraw, std::ref(MainMenu_));
		GS_FunctionMap_[GameSystemType::FREE]		  
			= std::bind(&MainMenuGameState::GameStateMainMenuFree, std::ref(MainMenu_));
		GS_FunctionMap_[GameSystemType::UNLOAD]		  
			= std::bind(&MainMenuGameState::GameStateMainMenuUnload, std::ref(MainMenu_));
		break;
	default:
		AE_FATAL_ERROR("invalid state!!");
	}
}
/******************************************************************************/
/*!
\fn void GameStateManager::Init(unsigned int gameStateInit)
\brief Initialite game state
\param gameStateInit
*/
/******************************************************************************/
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
/******************************************************************************/
/*!
\fn GameStateManager::GameStateLoad()
\brief Load current game state
*/
/******************************************************************************/
void GameStateManager::GameStateLoad()
{
	GS_FunctionMap_[GameSystemType::LOAD]();
}
/******************************************************************************/
/*!
\fn void GameStateManager::GameStateInit()
\brief Initialize current game state
*/
/******************************************************************************/
void GameStateManager::GameStateInit()
{
	GS_FunctionMap_[GameSystemType::INIT]();
}
/******************************************************************************/
/*!
\fn void GameStateManager::GameStateUpdate()
\brief Update current game state
*/
/******************************************************************************/
void GameStateManager::GameStateUpdate()
{
	GS_FunctionMap_[GameSystemType::UPDATE]();
}
/******************************************************************************/
/*!
\fn void GameStateManager::GameStateDraw()
\brief Draw current game state
*/
/******************************************************************************/
void GameStateManager::GameStateDraw()
{
	GS_FunctionMap_[GameSystemType::DRAW]();
}
/******************************************************************************/
/*!
\fn void GameStateManager::GameStateFree()
\brief Free current game state
*/
/******************************************************************************/
void GameStateManager::GameStateFree()
{
	GS_FunctionMap_[GameSystemType::FREE]();
}
/******************************************************************************/
/*!
\fn void GameStateManager::GameStateUnload()
\brief Unload current game state
*/
/******************************************************************************/
void GameStateManager::GameStateUnload()
{
	GS_FunctionMap_[GameSystemType::UNLOAD]();
}
/******************************************************************************/
/*!
\fn unsigned int GameStateManager::GetGameStateInitIndex() const
\brief Get game state init index
\return gGameStateInit
*/
/******************************************************************************/
unsigned int GameStateManager::GetGameStateInitIndex() const
{
	return gGameStateInit;
}
/******************************************************************************/
/*!
\fn unsigned int GameStateManager::GetGameStateCurrIndex() const
\brief Get game state current index
\return gGameStateCurr
*/
/******************************************************************************/
unsigned int GameStateManager::GetGameStateCurrIndex() const
{
	return gGameStateCurr;
}
/******************************************************************************/
/*!
\fn unsigned int GameStateManager::GetGameStatePrevIndex() const
\brief Get game state previous index
\return gGameStatePrev
*/
/******************************************************************************/
unsigned int GameStateManager::GetGameStatePrevIndex() const
{
	return gGameStatePrev;
}
/******************************************************************************/
/*!
\fn unsigned int GameStateManager::GetGameStateNextIndex() const
\brief Get game state next index
\return gGameStateNext
*/
/******************************************************************************/
unsigned int GameStateManager::GetGameStateNextIndex() const
{
	return gGameStateNext;
}
/******************************************************************************/
/*!
\fn MainMenuGameState& GameStateManager::GetMainMenuGameState()
\brief Get Main Menu GameState instance
\return MainMenu_
*/
/******************************************************************************/
MainMenuGameState& GameStateManager::GetMainMenuGameState()
{
	return MainMenu_;
}
/******************************************************************************/
/*!
\fn AsteroidsGameState& GameStateManager::GetAsteroidGameState()
\brief Get Asteroids GameState instance
\return Asteroids_
*/
/******************************************************************************/
AsteroidsGameState& GameStateManager::GetAsteroidGameState()
{
	return Asteroids_;
}
/******************************************************************************/
/*!
\fn void GameStateManager::SetGameStateInitIndex(unsigned int index)
\brief Set game state init index
\param index
*/
/******************************************************************************/
void GameStateManager::SetGameStateInitIndex(unsigned int index)
{
	gGameStateInit = index;
}
/******************************************************************************/
/*!
\fn void GameStateManager::SetGameStateCurrIndex(unsigned int index)
\brief Set game state current index
\param index
*/
/******************************************************************************/
void GameStateManager::SetGameStateCurrIndex(unsigned int index)
{
	gGameStateCurr = index;
}
/******************************************************************************/
/*!
\fn void GameStateManager::SetGameStatePrevIndex(unsigned int index)
\brief Set game state previous index
\param index
*/
/******************************************************************************/
void GameStateManager::SetGameStatePrevIndex(unsigned int index)
{
	gGameStatePrev = index;
}
/******************************************************************************/
/*!
\fn void GameStateManager::SetGameStateNextIndex(unsigned int index)
\brief Set game state next index
\param index
*/
/******************************************************************************/
void GameStateManager::SetGameStateNextIndex(unsigned int index)
{
	gGameStateNext = index;
}
