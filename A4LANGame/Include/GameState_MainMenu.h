/******************************************************************************/
/*!
\file GameState_MainMenu.h
\author Fikrul Islami Bin Abdullah
\par email: f.abdullah\@digipen.edu
\par DigiPen login: f.abdullah
\par Course: CS260-B
\par Assignment #04
\date 10/08/2021
\brief
This file contains the declarations for the implementation of the main system 
of the Main Menu game
*/
/******************************************************************************/
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