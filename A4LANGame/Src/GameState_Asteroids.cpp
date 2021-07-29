/* Start Header **************************************************************/
/*!
\file GameState_Asteroids.cpp
\author Fikrul Islami Bin Abdullah, f.abdullah, 440005019
\par f.abdullah\@digipen.edu
\date February 09, 2020
\brief This *.cpp file contains the variables and definition of 6 functions;
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

#include "pch.h"
#include "Main.h"
#include "Global.h"

/******************************************************************************/
/*!
	Defines
*/
/******************************************************************************/
const unsigned int	SHIP_INITIAL_NUM		= 3;			// initial number of ship lives
const float			SHIP_SIZE				= 16.0f;		// ship size
const float			SHIP_ACCEL_FORWARD		= 40.0f;		// ship forward acceleration (in m/s^2)
const float			SHIP_ACCEL_BACKWARD		= 40.0f;		// ship backward acceleration (in m/s^2)
const float			SHIP_ROT_SPEED			= (2.0f * PI);	// ship rotation speed (degree/second)

const float			BULLET_SPEED			= 150.0f;		// bullet speed (m/s)
static int			SPECIAL_TRIGGER			= 0;			// charges up special
static bool			SPECIAL_CHECK           = false;			// check if player can use special power

const float			ASTEROID_SIZE			= 50.0f;		// asteroid size
unsigned int		ASTEROID_COUNT			= 0;			// no of asteroids that spawned


// ---------------------------------------------------------------------------


/******************************************************************************/
/*!
	Static Variables
*/
/******************************************************************************/

// list of original object
static GameObj				sGameObjList[GAME_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
static unsigned long		sGameObjNum;								// The number of defined game objects

// list of object instances
static GameObjInst			sGameObjInstList[GAME_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
static unsigned long		sGameObjInstNum;							// The number of used game object instances

// pointer to the ship object
static GameObjInst *		spShip;										// Pointer to the "Ship" game object instance

// number of ship available (lives 0 = game over)
static long					sShipLives;									// The number of lives left

// the score = number of asteroid destroyed
static unsigned long		sScore;										// Current score

// ---------------------------------------------------------------------------

// functions to create/destroy a game object instance
GameObjInst *		gameObjInstCreate (unsigned long type, float scale, 
											   AEVec2 * pPos, AEVec2 * pVel, float dir);
void					gameObjInstDestroy(GameObjInst * pInst);

// Creates bullet explosion when ship dies
void bulletExplosion(void)
{
	float BEDir = 0.0f;
	float BERot = 0.2f;
	AEVec2 BulletExp;

	for (int i = 0; i < 10; ++i)
	{
		BEDir = BERot * PI;

		// Get the bullet's direction according to the ship's direction
		AEVec2Set(&BulletExp, cosf(BEDir), sinf(BEDir));

		// Set the velocity
		BulletExp.x += BULLET_SPEED * BulletExp.x;
		BulletExp.y += BULLET_SPEED * BulletExp.y;

		// Create an instance
		gameObjInstCreate(TYPE_BULLET, 20.0f, &spShip->posCurr, &BulletExp, BEDir);

		BERot += 0.2f;
	}
}

// Spawns an asteroid
void spawnAsteroid(void)
{
	// Create an array of vectors for the asteroids
	AEVec2 AsteroidPos[4];
	// Velocity Vector
	AEVec2 added;

	// Randomize size from 0 - 99
	int size = rand() % 100;
	// Randomize position in array from 0-3
	int index = rand() % 4;
	// Randomize Velocity 75-100
	int velocity = rand() % (150 + 1 - 75) + 75;
	// Radomize direction
	float dir = (float)(rand() % (int)(2 * PI)) + 0.01f;

	// Set each spawn location in the array
	AsteroidPos[0] = { AEGfxGetWinMinX() / 2, AEGfxGetWinMinY() / 2 };
	AsteroidPos[1] = { AEGfxGetWinMinX() / 2, AEGfxGetWinMinY() };
	AsteroidPos[2] = { AEGfxGetWinMinX(), AEGfxGetWinMinY() / 2 };
	AsteroidPos[3] = { AEGfxGetWinMinX(), AEGfxGetWinMinY() };

	// Sets the direction of the asteroids
	AEVec2Set(&added, cosf(dir), sinf(dir));
	// Adds the velocity to the asteroids position
	AEVec2Add(&AsteroidPos[index], &AsteroidPos[index], &added);

	// Sets velocity of asteroids
	added.x += static_cast<float>(velocity) * added.x;
	added.y += static_cast<float>(velocity)* added.y;

	// Creates the game object
	gameObjInstCreate(TYPE_ASTEROID, static_cast<float>(size), &AsteroidPos[index], &added, dir);

	// Keeps track of the spawned asteroids on screen
	ASTEROID_COUNT++;
}

// Creates Bullet Hell Mechanic
void spawnBulletHell(int i)
{
	AEVec2 BulletHell;
	AEVec2Set(&BulletHell, cosf(spShip->dirCurr) + 0.3f * i, sinf(spShip->dirCurr) + 0.3f * i);
	// Set the velocity
	BulletHell.x += BULLET_SPEED * BulletHell.x;
	BulletHell.y += BULLET_SPEED * BulletHell.y;
	// Get the bullet's direction according to the ship's direction
	gameObjInstCreate(TYPE_BULLET, 10.0f, &spShip->posCurr, &BulletHell, spShip->dirCurr);
}

/******************************************************************************/
/*!
	A function that creates the game objects for this assignment.
*/
/******************************************************************************/
GameObjInst * gameObjInstCreate(unsigned long type, 
							   float scale, 
							   AEVec2 * pPos, 
							   AEVec2 * pVel, 
							   float dir)
{
	AEVec2 zero;
	AEVec2Zero(&zero);

	AE_ASSERT_PARM(type < sGameObjNum);
	
	// loop through the object instance list to find a non-used object instance
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst * pInst = sGameObjInstList + i;

		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject	= sGameObjList + type;
			pInst->flag		= FLAG_ACTIVE;
			pInst->scale	= scale;
			pInst->posCurr	= pPos ? *pPos : zero;
			pInst->velCurr	= pVel ? *pVel : zero;
			pInst->dirCurr	= dir;
			
			// return the newly created instance
			return pInst;
		}
	}

	// cannot find empty slot => return 0
	return 0;
}

/******************************************************************************/
/*!
	This function ensures that the game deallocates memory of all game objects.
*/
/******************************************************************************/
void gameObjInstDestroy(GameObjInst * pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	pInst->flag = 0;
}

void AsteroidsGameState::GameStateAsteroidsLoad(void)
{
	// zero the game object array
	memset(sGameObjList, 0, sizeof(GameObj) * GAME_OBJ_NUM_MAX);
	// No game objects (shapes) at this point
	sGameObjNum = 0;

	// zero the game object instance array
	memset(sGameObjInstList, 0, sizeof(GameObjInst) * GAME_OBJ_INST_NUM_MAX);
	// No game object instances (sprites) at this point
	sGameObjInstNum = 0;

	// The ship object instance hasn't been created yet, so this "spShip" pointer is initialized to 0
	spShip = nullptr;

	// load/create the mesh data (game objects / Shapes)
	GameObj* pObj;

	// =====================
	// create the ship shape
	// =====================

	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_SHIP;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 1.0f,
		0.5f, 0.0f, 0xFFFFFFFF, 1.0f, 1.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create object!!");


	// =======================
	// create the bullet shape
	// =======================

	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_BULLET;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.25f, 0.125f, 0xFFFFFF00, 0.0f, 0.0f,
		0.25f, 0.125f, 0xFFFFFF00, 0.0f, 1.0f,
		0.25f, -0.125f, 0xFFFFFF00, 1.0f, 1.0f);

	AEGfxTriAdd(
		-0.25f, 0.125f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.25f, -0.125f, 0xFFFFFF00, 0.0f, 1.0f,
		0.25f, -0.125f, 0xFFFFFF00, 1.0f, 1.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create object!!");


	// =========================
	// create the asteroid shape
	// =========================

	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_ASTEROID;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xBEC1BE, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xBEC1BE, 0.0f, 1.0f,
		0.5f, -0.5f, 0xCDD2CD, 1.0f, 1.0f);

	AEGfxTriAdd(
		0.5f, 0.5f, 0xFF808080, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFF808080, 0.0f, 1.0f,
		-0.5f, 0.5f, 0xCDD2CD, 1.0f, 1.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create object!!");
}

void AsteroidsGameState::GameStateAsteroidsInit(void)
{
	// create the main ship
	spShip = gameObjInstCreate(TYPE_SHIP, SHIP_SIZE, nullptr, nullptr, 0.0f);

	AE_ASSERT(spShip);

	// CREATE THE INITIAL ASTEROIDS INSTANCES USING THE "gameObjInstCreate" FUNCTION
	for (int i = 0; i < 4; i++)
	{
		spawnAsteroid();
	}

	// reset the score and the number of ships
	sScore = 0;
	sShipLives = SHIP_INITIAL_NUM;
}

void AsteroidsGameState::GameStateAsteroidsUpdate(void)
{
	// =========================
	// update according to input
	// =========================

	// This input handling moves the ship without any velocity nor acceleration
	// It should be changed when implementing the Asteroids project
	//
	// Updating the velocity and position according to acceleration is 
	// done by using the following:
	// Pos1 = 1/2 * a*t*t + v0*t + Pos0
	//
	// In our case we need to divide the previous equation into two parts in order 
	// to have control over the velocity and that is done by:
	//
	// v1 = a*t + v0		//This is done when the UP or DOWN key is pressed 
	// Pos1 = v1*t + Pos0

	// Restart Mechanic
	if (sScore >= 5000 || sShipLives == 0)
	{
		// Reset Key Values
		if (AEInputCheckCurr(AEVK_R))
		{
			GSManager->SetGameStateCurrIndex(GS_RESTART);
			/*gGameStateCurr = GS_RESTART;*/
			sScore = 0;
			sShipLives = SHIP_INITIAL_NUM;
			ASTEROID_COUNT = 0;
			SPECIAL_TRIGGER = 0;
			onValueChange = true;
		}
	}

	if (sShipLives > 0 && sScore <= 5000)
	{
		if (AEInputCheckCurr(AEVK_UP))
		{
			AEVec2 added;
			AEVec2Set(&added, cosf(spShip->dirCurr), sinf(spShip->dirCurr));
			AEVec2Add(&spShip->posCurr, &spShip->posCurr, &added);

			// Find the velocity according to the acceleration
			spShip->velCurr.x += added.x * SHIP_ACCEL_FORWARD * g_dt;
			spShip->velCurr.y += added.y * SHIP_ACCEL_FORWARD * g_dt;
			// Limit your speed over here
			AEVec2Scale(&spShip->velCurr, &spShip->velCurr, static_cast<f32>(0.99));
		}

		if (AEInputCheckCurr(AEVK_DOWN))
		{
			AEVec2 added;
			AEVec2Set(&added, -cosf(spShip->dirCurr), -sinf(spShip->dirCurr));
			AEVec2Add(&spShip->posCurr, &spShip->posCurr, &added);//YOU MAY NEED TO CHANGE/REPLACE THIS LINE

			// Find the velocity according to the decceleration
			spShip->velCurr.x += g_dt * SHIP_ACCEL_BACKWARD * added.x;
			spShip->velCurr.y += g_dt * SHIP_ACCEL_BACKWARD * added.y;
			// Limit your speed over here
			AEVec2Scale(&spShip->velCurr, &spShip->velCurr, static_cast<f32>(0.99));
		}

		if (AEInputCheckCurr(AEVK_LEFT))
		{
			spShip->dirCurr += SHIP_ROT_SPEED * (float)(AEFrameRateControllerGetFrameTime());
			spShip->dirCurr = AEWrap(spShip->dirCurr, -PI, PI);
		}

		if (AEInputCheckCurr(AEVK_RIGHT))
		{
			spShip->dirCurr -= SHIP_ROT_SPEED * (float)(AEFrameRateControllerGetFrameTime());
			spShip->dirCurr = AEWrap(spShip->dirCurr, -PI, PI);
		}

		// Shoot a bullet if space is triggered (Create a new object instance)
		if (AEInputCheckTriggered(AEVK_SPACE))
		{
			if (SPECIAL_CHECK == TRUE)
			{
				for (int m = 0; m < 5; ++m)
				{
					spawnBulletHell(m);
				}

				SPECIAL_TRIGGER = 0;
			}
			else {
				AEVec2 Bullet;
				// Get the bullet's direction according to the ship's direction
				AEVec2Set(&Bullet, cosf(spShip->dirCurr), sinf(spShip->dirCurr));
				// Set the velocity
				Bullet.x += BULLET_SPEED * Bullet.x;
				Bullet.y += BULLET_SPEED * Bullet.y;
				// Create an instance
				gameObjInstCreate(TYPE_BULLET, 10.0f, &spShip->posCurr, &Bullet, spShip->dirCurr);
			}
		}

		// ======================================================
		// update physics of all active game object instances
		//	-- Positions are updated here with the computed velocity
		//  -- Get the bounding rectangle of every active instance:
		//		boundingRect_min = -BOUNDING_RECT_SIZE * instance->scale + instance->pos
		//		boundingRect_max = BOUNDING_RECT_SIZE * instance->scale + instance->pos
		// ======================================================
		for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
		{
			GameObjInst* pInst = sGameObjInstList + i;

			// If the object is not active, don't do anything
			if ((pInst->flag & FLAG_ACTIVE) == 0)
			{
				continue;
			}

			// Update Positions of the Game Objects
			pInst->posCurr.x = pInst->posCurr.x + pInst->velCurr.x * g_dt;
			pInst->posCurr.y = pInst->posCurr.y + pInst->velCurr.y * g_dt;

			// Getting bounding rectangles at every active instance
			AEVec2Set(&pInst->boundingBox.min, -0.5f * pInst->scale + pInst->posCurr.x, -0.5f * pInst->scale + pInst->posCurr.y);
			AEVec2Set(&pInst->boundingBox.max, 0.5f * pInst->scale + pInst->posCurr.x, 0.5f * pInst->scale + pInst->posCurr.y);
		}
	}
	// ===================================
	// update active game object instances
	// Example:
	//		-- Wrap specific object instances around the world (Needed for the assignment)
	//		-- Removing the bullets as they go out of bounds (Needed for the assignment)
	//		-- If you have a homing missile for example, compute its new orientation 
	//			(Homing missiles are not required for the Asteroids project)
	//		-- Update a particle effect (Not required for the Asteroids project)
	// ===================================
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// check if the object is a ship
		if (pInst->pObject->type == TYPE_SHIP)
		{
			// warp the ship from one end of the screen to the other
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, AEGfxGetWinMinX() - SHIP_SIZE,
				AEGfxGetWinMaxX() + SHIP_SIZE);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, AEGfxGetWinMinY() - SHIP_SIZE,
				AEGfxGetWinMaxY() + SHIP_SIZE);
		}

		// Wrap asteroids here
		if (pInst->pObject->type == TYPE_ASTEROID)
		{
			// warp the ship from one end of the screen to the other
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, AEGfxGetWinMinX() - ASTEROID_SIZE,
				AEGfxGetWinMaxX() + ASTEROID_SIZE);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, AEGfxGetWinMinY() - ASTEROID_SIZE,
				AEGfxGetWinMaxY() + ASTEROID_SIZE);
		}

		// Remove bullets that go out of bounds
		if (pInst->pObject->type == TYPE_BULLET)
		{
			// if the bullet goes out of x-axis, remove
			if (pInst->posCurr.x > AEGfxGetWinMaxX() || pInst->posCurr.x < AEGfxGetWinMinX())
				gameObjInstDestroy(pInst);

			// if the bullet goes out of y-axis, remove
			if (pInst->posCurr.y > AEGfxGetWinMaxY() || pInst->posCurr.y < AEGfxGetWinMinY())
				gameObjInstDestroy(pInst);
		}
	}


	// ====================
	// check for collision
	// ====================

	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// If object is inactive, do nothing and skip
		if ((pInst->flag & FLAG_ACTIVE) == 0)
		{
			continue;
		}

		// Asteroids
		if (pInst->pObject->type == TYPE_ASTEROID)
		{
			for (unsigned long j = 0; j < GAME_OBJ_INST_NUM_MAX; j++)
			{
				GameObjInst* _pInst = sGameObjInstList + j;

				// If asteroid object is inactive, do nothing and skip
				if ((_pInst->flag & FLAG_ACTIVE) == 0 ||
					(_pInst->pObject->type == TYPE_ASTEROID))
				{
					continue;
				}

				// Ship
				if (_pInst->pObject->type == TYPE_SHIP)
				{
					// Check collision between the ship and the asteroids
					if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr,
						_pInst->boundingBox, _pInst->velCurr))
					{
						// Creates bullet explosion after each death.
						if (sShipLives > 1)
						{
							bulletExplosion();
						}

						// Decrease ship lives by 1
						sShipLives -= 1;

						// Decrement Asteroids by 1
						ASTEROID_COUNT -= 1;

						// Reset Ship Position
						AEVec2Set(&spShip->posCurr, 0.f, 0.f);
						// Reset Ship Velocity
						AEVec2Set(&spShip->velCurr, 0.f, 0.f);
						// Reset Direction
						spShip->dirCurr = 0.f;
						// Resets Special Power
						SPECIAL_TRIGGER = 0;

						// Destroy Asteroid
						gameObjInstDestroy(pInst);

						// Set flag to true
						if (sScore <= 5000)
						{
							onValueChange = true;
						}

						if (sShipLives < 0)
						{
							// Reset Ship Position
							AEVec2Set(&spShip->posCurr, 0.f, 0.f);
							// Reset Ship Velocity
							AEVec2Set(&spShip->velCurr, 0.f, 0.f);
							// Reset Ship Direction
							spShip->dirCurr = 0.f;

							// Set flag to false
							onValueChange = false;
						}
					}
				}
				else if (_pInst->pObject->type == TYPE_BULLET) {

					// Check collision between asteroid and bullet
					if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr,
						_pInst->boundingBox, _pInst->velCurr))
					{
						// Destroy Asteroid
						gameObjInstDestroy(pInst);

						// Remove Bullet
						gameObjInstDestroy(_pInst);

						// Update Score
						sScore += 100;

						// Increment Special Trigger by 1
						SPECIAL_TRIGGER += 1;

						if (SPECIAL_TRIGGER >= 5)
						{
							SPECIAL_CHECK = TRUE;
						}
						else
						{
							SPECIAL_CHECK = FALSE;
						}

						// Decrement Asteroids by 1
						ASTEROID_COUNT -= 1;

						// Generate 2 more asteroids after killing 1 
						for (int k = 0; k < 2; k++)
						{
							spawnAsteroid();
						}

						// Set update_strings flag to true
						onValueChange = true;

						// At Max Score
						if (sScore > 5000)
						{
							// Reset Ship Position
							AEVec2Set(&spShip->posCurr, 0.f, 0.f);
							// Reset Ship velocity
							AEVec2Set(&spShip->velCurr, 0.f, 0.f);
							// Reset Ship direction
							spShip->dirCurr = 0.f;

							// Set flag to false
							onValueChange = false;
						}
					}

				}
			}
		}
	}

	/*
	Notes:
	for each object instance: oi1
		if oi1 is not active
			skip

		if oi1 is an asteroid
			for each object instance oi2
				if(oi2 is not active or oi2 is an asteroid)
					skip

				if(oi2 is the ship)
					Check for collision between ship and asteroids (Rectangle - Rectangle)
					Update game behavior accordingly
					Update "Object instances array"
				else
				if(oi2 is a bullet)
					Check for collision between ship and asteroids (Rectangle - Rectangle)
					Update game behavior accordingly
					Update "Object instances array"
	*/

	// =====================================
	// calculate the matrix for all objects
	// =====================================

	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;
		AEMtx33		 trans, rot, scale;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// Compute the scaling matrix
		AEMtx33Scale(&scale, pInst->scale, pInst->scale);
		// Compute the rotation matrix 
		AEMtx33Rot(&rot, pInst->dirCurr);
		// Compute the translation matrix
		AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);
		// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
		// Translate <- Rotation <- Scaling
		AEMtx33Concat(&pInst->transform, &rot, &scale);
		AEMtx33Concat(&pInst->transform, &trans, &pInst->transform);
	}
}

void AsteroidsGameState::GameStateAsteroidsDraw(void)
{
	char strBuffer[1024] = { '\0' };
	char InGameBuffer[1024] = { '\0' };

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);

	// draw all object instances in the list
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// Set the current object instance's transform matrix using "AEGfxSetTransform"
		AEGfxSetTransform(pInst->transform.m);
		// Draw the shape used by the current object instance using "AEGfxMeshDraw"
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	}

	//You can replace this condition/variable by your own data.
	if (onValueChange)
	{
		sprintf_s(strBuffer, "Score: %d", sScore);
		//AEGfxPrint(10, 10, (u32)-1, strBuffer);
		printf("%s \n", strBuffer);

		sprintf_s(strBuffer, "Ship Left: %d", sShipLives >= 0 ? sShipLives : 0);
		//AEGfxPrint(600, 10, (u32)-1, strBuffer);
		printf("%s \n", strBuffer);

		if (SPECIAL_TRIGGER >= 5)
		{
			sprintf_s(strBuffer, "SPECIAL POWER READY");
			printf("%s \n", strBuffer);
		}
		else
		{
			sprintf_s(strBuffer, "I need to destroy %d more asteroids for my special power!", 5 - SPECIAL_TRIGGER);
			printf("%s \n", strBuffer);
		}

		// display the game over message
		if (sShipLives <= 0)
		{
			//AEGfxPrint(280, 260, 0xFFFFFFFF, "       GAME OVER       ");
			printf("GAME OVER\n");
			printf("Press ESC to quit.\n");
			printf("Press R to restart.\n");
		}

		if (sScore >= 5000)
		{
			printf("You Rock\n");
			printf("Press ESC to quit.\n");
			printf("Press R to restart.\n");
		}
		onValueChange = false;
	}

	//AEGfxPrint(fontID, shipName, 0, 0, 0.1f, 1.0f, 0.1f);

	// Updates the InGameBuffer during the game
	sprintf_s(InGameBuffer, "Ship Left: %d Score: %d", sShipLives >= 0 ? sShipLives : 0, sScore <= 5000 ? sScore : 5000);
}

void AsteroidsGameState::GameStateAsteroidsFree(void)
{
	// kill all object instances in the array using "gameObjInstDestroy"
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;
		gameObjInstDestroy(pInst);
	}
}

void AsteroidsGameState::GameStateAsteroidsUnload(void)
{
	// free all mesh data (shapes) of each object using "AEGfxTriFree"
	for (unsigned long i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = sGameObjList + i;
		if ((pInst->pMesh == nullptr))
			continue;
		AEGfxMeshFree(pInst->pMesh);
	}
}
