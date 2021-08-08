#pragma once

#include "AEEngine.h"

class MainMenuGameState
{
public:
	void GameStateMainMenuLoad(void);
	void GameStateMainMenuInit(void);
	void GameStateMainMenuUpdate(void);
	void GameStateMainMenuDraw(void);
	void GameStateMainMenuFree(void);
	void GameStateMainMenuUnload(void);

	static AEGfxVertexList* pMesh;
	static AEGfxTexture* pBackground;
};