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

#include "Global.h"
#include "GameObjectFactory.h"

// ---------------------------------------------------------------------------

class AsteroidsGameState
{
public:
	AsteroidsGameState();
	void GameStateAsteroidsLoad(void);
	void GameStateAsteroidsInit(void);
	void GameStateAsteroidsUpdate(void);
	void GameStateAsteroidsDraw(void);
	void GameStateAsteroidsFree(void);
	void GameStateAsteroidsUnload(void);

	// pointer to the ship object
	// Pointer to the "Ship" game object instance
	static GameObjInst* spShip;
	// number of ship available (lives 0 = game over)
	static long sShipLives;
	// the score = number of asteroid destroyed
	// Current score
	static unsigned long sScore;
private:
	// initial number of ship lives
	const unsigned int SHIP_INITIAL_NUM{ 3 };
	// ship size
	const float SHIP_SIZE = 16.0f;
	// ship forward acceleration (in m/s^2)
	const float SHIP_ACCEL_FORWARD = 40.0f;
	// ship backward acceleration (in m/s^2)
	const float SHIP_ACCEL_BACKWARD = 40.0f;
	// ship rotation speed (degree/second)
	const float SHIP_ROT_SPEED = (2.0f * PI);
	// bullet speed (m/s)
	const float BULLET_SPEED = 150.0f;
	// charges up special
	int SPECIAL_TRIGGER = 0;
	// check if player can use special power
	bool SPECIAL_CHECK = false;
	// asteroid size
	const float ASTEROID_SIZE = 50.0f;
	// no of asteroids that spawned
	unsigned int ASTEROID_COUNT = 0;

	std::shared_ptr<GameObjectFactory> GameObjFactory_;

	void bulletExplosion(void);
	void spawnAsteroid(void);
	void spawnBulletHell(int i);
};

// ---------------------------------------------------------------------------

#endif // CS230_GAME_STATE_PLAY_H_


