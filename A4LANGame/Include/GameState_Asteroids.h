/* Start Header **************************************************************/
/*!
\file GameState_Asteroids.h
\author Fikrul Islami Bin Abdullah, f.abdullah, 440005019
\par f.abdullah\@digipen.edu
\date February 09, 2020
\brief This *.h file contains all the declarations for the 6 functions.
	   1) Load
	   2) Initialization
	   3) Update
	   4) Draw
	   5) Free
	   6) Unload

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header ****************************************************************/

#ifndef CS230_GAME_STATE_PLAY_H_
#define CS230_GAME_STATE_PLAY_H_

// ---------------------------------------------------------------------------

void GameStateAsteroidsLoad(void);
void GameStateAsteroidsInit(void);
void GameStateAsteroidsUpdate(void);
void GameStateAsteroidsDraw(void);
void GameStateAsteroidsFree(void);
void GameStateAsteroidsUnload(void);

// ---------------------------------------------------------------------------

#endif // CS230_GAME_STATE_PLAY_H_


