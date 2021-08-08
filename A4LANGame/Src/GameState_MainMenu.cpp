#include "pch.h"
#include "GameState_MainMenu.h"
#include "Main.h"

AEGfxVertexList* MainMenuGameState::pMesh = nullptr;
AEGfxTexture* MainMenuGameState::pBackground = nullptr;

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

void MainMenuGameState::GameStateMainMenuInit(void)
{
	pBackground = AEGfxTextureLoad("Resources\\AsteroidMainMenu.PNG");
}

void MainMenuGameState::GameStateMainMenuUpdate(void)
{
	if (AEInputCheckTriggered(AEVK_RETURN))
	{
		client->SendAllClient("[READY]");
		while (!client->GetClientReadyCheck())
		{
			client->ReceiveAllClient(std::string{});
		}

		GSManager->SetGameStateNextIndex(GS_ASTEROIDS);
	}
}

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

void MainMenuGameState::GameStateMainMenuFree(void)
{
	AEGfxMeshFree(pMesh);
}

void MainMenuGameState::GameStateMainMenuUnload(void)
{
	AEGfxTextureUnload(pBackground);
}
