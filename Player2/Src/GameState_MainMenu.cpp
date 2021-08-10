/******************************************************************************/
/*!
\file GameState_MainMenu.cpp
\author Fikrul Islami Bin Abdullah
\par email: f.abdullah\@digipen.edu
\par DigiPen login: f.abdullah
\par Course: CS260-B
\par Assignment #04
\date 10/08/2021
\brief
This file contains an implementation of the main system of the Main Menu game
*/
/******************************************************************************/
#include "pch.h"
#include "GameState_MainMenu.h"
#include "Main.h"

AEGfxVertexList* MainMenuGameState::pMesh = nullptr;
AEGfxTexture* MainMenuGameState::pBackground = nullptr;
/******************************************************************************/
/*!
\fn void MainMenuGameState::GameStateMainMenuLoad(void)
\brief Load Main Menu Game State Load Function
*/
/******************************************************************************/
void MainMenuGameState::GameStateMainMenuLoad(void)
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF000000, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFF000000, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);

	pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh, "fail to create object!!");
}
/******************************************************************************/
/*!
\fn void MainMenuGameState::GameStateMainMenuInit(void)
\brief Load Main Menu Game State Init Function
*/
/******************************************************************************/
void MainMenuGameState::GameStateMainMenuInit(void)
{
	pBackground = AEGfxTextureLoad("Resources\\AsteroidMainMenu.PNG");
}
/******************************************************************************/
/*!
\fn void MainMenuGameState::GameStateMainMenuUpdate(void)
\brief Load Main Menu Game State Update Function
*/
/******************************************************************************/
void MainMenuGameState::GameStateMainMenuUpdate(void)
{
	if (AEInputCheckTriggered(AEVK_RETURN))
	{
		ClientInfo* info = clientManager->GetOwnInfo();
		info->readyCheck = true;
		std::string message = Parser::CreateHeader("[READY]", 
			info->name, info->port, std::vector<std::string>{});
		clientManager->SendAllClient(message);
	}

	if (clientManager->GetClientReadyCheck())
		GSManager->SetGameStateNextIndex(GS_ASTEROIDS);
}
/******************************************************************************/
/*!
\fn void MainMenuGameState::GameStateMainMenuDraw(void)
\brief Load Main Menu Game State Draw Function
*/
/******************************************************************************/
void MainMenuGameState::GameStateMainMenuDraw(void)
{
	AEMtx33 trans, scale;
	AEMtx33 menuTransform;

	// To render Image
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(0.0f, 0.0f);
	AEGfxTextureSet(pBackground, -34, 0);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	// Transform <- Translate <- Scale
	AEMtx33Identity(&scale);
	AEMtx33Scale(&scale, 800.0f, 600.0f);

	AEMtx33Identity(&trans);
	AEMtx33Trans(&trans, 0, 0);

	AEMtx33Concat(&menuTransform, &trans, &scale);
	AEGfxSetTransform(menuTransform.m);

	// Draw Mesh
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
}
/******************************************************************************/
/*!
\fn void MainMenuGameState::GameStateMainMenuFree(void)
\brief Load Main Menu Game State Free Function
*/
/******************************************************************************/
void MainMenuGameState::GameStateMainMenuFree(void)
{
	AEGfxMeshFree(pMesh);
}
/******************************************************************************/
/*!
\fn void MainMenuGameState::GameStateMainMenuUnload(void)
\brief Load Main Menu Game State Unload Function
*/
/******************************************************************************/
void MainMenuGameState::GameStateMainMenuUnload(void)
{
	AEGfxTextureUnload(pBackground);
}
