#pragma once

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

	GameObjInst* myShip{ nullptr };
	std::unordered_map<ShipID, GameObjInst*> IDToPlayerShip_;
	std::unordered_map<ShipState, std::function<void(ShipID)>> StateToInput_;
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

	bool GameOver_NoShips{ false };
	bool GameOver_MaxScore{ false };

	std::shared_ptr<GameObjectFactory> GameObjFactory_;

	void PlayerMoveForward(ShipID PlayerID);
	void PlayerMoveBackwards(ShipID PlayerID);
	void PlayerRotateLeft(ShipID PlayerID);
	void PlayerRotateRight(ShipID PlayerID);
	void PlayerShoot(ShipID PlayerID);
	void bulletExplosion(ShipID PlayerID);
	void spawnAsteroid(void);
	void spawnBulletHell(int i, ShipID PlayerID);
	void RestartGameInit();
};



