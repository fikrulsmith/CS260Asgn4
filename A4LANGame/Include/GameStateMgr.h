#pragma once

#include "AEEngine.h"
#include "GameStateList.h"
#include "GameState_Asteroids.h"
#include "GameState_MainMenu.h"
#include "Global.h"

class GameStateManager
{
public:
	void GameSystemUpdate();
	void Init(unsigned int gameStateInit);

	void GameStateLoad();
	void GameStateInit();
	void GameStateUpdate();
	void GameStateDraw();
	void GameStateFree();
	void GameStateUnload();

	unsigned int GetGameStateInitIndex() const;
	unsigned int GetGameStateCurrIndex() const;
	unsigned int GetGameStatePrevIndex() const;
	unsigned int GetGameStateNextIndex() const;

	MainMenuGameState& GetMainMenuGameState();
	AsteroidsGameState& GetAsteroidGameState();

	void SetGameStateInitIndex(unsigned int index);
	void SetGameStateCurrIndex(unsigned int index);
	void SetGameStatePrevIndex(unsigned int index);
	void SetGameStateNextIndex(unsigned int index);

	size_t PlayerCount_{ 2 };
private:
	unsigned int gGameStateInit{ 0 };
	unsigned int gGameStateCurr{ 0 };
	unsigned int gGameStatePrev{ 0 };
	unsigned int gGameStateNext{ 0 };

	MainMenuGameState MainMenu_;
	AsteroidsGameState Asteroids_;
	std::unordered_map<GameSystemType, std::function<void()>> GS_FunctionMap_;
};
