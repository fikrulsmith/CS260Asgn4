#pragma once

// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "Math.h"

#include "GameStateMgr.h"
#include "GameState_Asteroids.h"
#include "Collision.h"
#include "../Client/Client.h"

//------------------------------------
// Globals

extern float	g_dt;
extern double	g_appTime;
extern std::unique_ptr<GameStateManager> GSManager;
extern std::unique_ptr<Client> clientManager;












