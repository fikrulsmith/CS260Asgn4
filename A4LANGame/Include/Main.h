/*/* Start Header **************************************************************/
/*!
\file Main.h
\author Fikrul Islami Bin Abdullah, f.abdullah, 440005019
\par f.abdullah\@digipen.edu
\date February 09, 2020
\brief This *.h file contains all the header files needed for this assignment.

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header ****************************************************************/


#ifndef CS230_MAIN_H_
#define CS230_MAIN_H_

// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "Math.h"

#include "GameStateMgr.h"
#include "GameState_Asteroids.h"
#include "Collision.h"

//------------------------------------
// Globals

extern float	g_dt;
extern double	g_appTime;
extern std::unique_ptr<GameStateManager> GSManager;


#endif // CS230_MAIN_H_











