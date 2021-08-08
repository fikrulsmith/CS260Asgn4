#include "pch.h"
#include "main.h"

#include <memory>
#include "../Client/Client.h"

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

	std::vector<std::pair<std::string, std::string>> vec; 
	Parser::GetAllPairsOfHostnameAndPorts(std::string{ command_line }, vec);

	Client client;
	client.InitialiseClient(vec);
	while (true)
	{
		std::cout << rand() << std::endl;
	}
	/*if (client.GetOwnPort() == "2048")
		client.SendClient();
	else
		client.ReceiveClient();*/

	GSManager = std::make_unique<GameStateManager>();
	GSManager->Init(GS_MAINMENU);

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
			std::cout << rand() << std::endl;
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