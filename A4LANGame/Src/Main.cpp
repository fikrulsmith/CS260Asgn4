/* Start Header **************************************************************/
/*!
\file Main.cpp
\author Fikrul Islami Bin Abdullah, f.abdullah, 440005019
\par f.abdullah\@digipen.edu
\date February 09, 2020
\brief This *.cpp file contains the variables and definition that controls
	   the main game loop.

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header ****************************************************************/

#include "pch.h"
#include "main.h"

#include <memory>

// ---------------------------------------------------------------------------
// Globals
float	 g_dt;
double	 g_appTime;
std::unique_ptr<GameStateManager> GSManager;


/******************************************************************************/
/*!
	Starting point of the application
*/
/******************************************************************************/
int WINAPI WinMain(_In_ HINSTANCE instanceH, _In_opt_ HINSTANCE prevInstanceH, _In_ LPSTR command_line, _In_ int show)
{
	UNREFERENCED_PARAMETER(prevInstanceH);
	UNREFERENCED_PARAMETER(command_line);

	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif
	
	// Initialize the system
	AESysInit (instanceH, show, 800, 600, 1, 60, false, NULL);
	// Changing the window title
	AESysSetWindowTitle("CS260 Asteroids");

	//set background color
	AEGfxSetBackgroundColor(0.0f, 0.1f, 1.0f);

	GSManager = std::make_unique<GameStateManager>();
	GSManager->Init(GS_ASTEROIDS);

	while(GSManager->GetGameStateCurrIndex() != GS_QUIT)
	{
		// reset the system modules
		AESysReset();

		// If not restarting, load the gamestate
		if(GSManager->GetGameStateCurrIndex() != GS_RESTART)
		{
			GSManager->GameSystemUpdate();
			GSManager->GameStateLoad();
		}
		else
		{
			GSManager->SetGameStateCurrIndex(GSManager->GetGameStatePrevIndex());
			GSManager->SetGameStateNextIndex(GSManager->GetGameStateCurrIndex());
		}

		// Initialize the gamestate
		GSManager->GameStateInit();

		while(GSManager->GetGameStateCurrIndex() == GSManager->GetGameStateNextIndex())
		{
			AESysFrameStart();
			AEInputUpdate();

			GSManager->GameStateUpdate();
			GSManager->GameStateDraw();
			
			AESysFrameEnd();

			// check if forcing the application to quit
			if ((AESysDoesWindowExist() == false) || AEInputCheckTriggered(AEVK_ESCAPE))
				GSManager->SetGameStateNextIndex(GS_QUIT);

			g_dt = (f32)AEFrameRateControllerGetFrameTime();
			g_appTime += g_dt;
		}
		
		GSManager->GameStateFree();

		if (GSManager->GetGameStateNextIndex() != GS_RESTART)
			GSManager->GameStateUnload();

		GSManager->SetGameStatePrevIndex(GSManager->GetGameStateCurrIndex());
		GSManager->SetGameStateCurrIndex(GSManager->GetGameStateNextIndex());
	}

	// free the system
	AESysExit();
}