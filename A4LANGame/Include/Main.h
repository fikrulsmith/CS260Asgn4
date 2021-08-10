/******************************************************************************/
/*!
\file Main.h
\author Fikrul Islami Bin Abdullah
\par email: f.abdullah\@digipen.edu
\par DigiPen login: f.abdullah
\par Course: CS260-B
\par Assignment #04
\date 10/08/2021
\brief
This file contains the global variables for the application to use.
*/
/******************************************************************************/
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
