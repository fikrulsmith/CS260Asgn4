#include "pch.h"
#include "Main.h"

/******************************************************************************/
/*!
	Static Variables & Globals
*/
/******************************************************************************/

void AsteroidsGameState::GameStateAsteroidsUpdate(void)
{
	//AllShips_.size();
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
			PlayerMoveForward(myShip->shipComp.sShipID);
		}

		if (AEInputCheckCurr(AEVK_DOWN))
		{
			PlayerMoveBackwards(myShip->shipComp.sShipID);
		}

		if (AEInputCheckCurr(AEVK_LEFT))
		{
			PlayerRotateLeft(myShip->shipComp.sShipID);
		}

		if (AEInputCheckCurr(AEVK_RIGHT))
		{
			PlayerRotateRight(myShip->shipComp.sShipID);
		}

		// Shoot a bullet if space is triggered (Create a new object instance)
		if (AEInputCheckTriggered(AEVK_SPACE))
		{
			PlayerShoot(myShip->shipComp.sShipID);
		}

		if (AEInputCheckCurr(AEVK_W))
		{
			PlayerMoveForward(ShipID::PLAYER2);
		}

		if (AEInputCheckCurr(AEVK_S))
		{
			PlayerMoveBackwards(ShipID::PLAYER2);
		}

		if (AEInputCheckCurr(AEVK_A))
		{
			PlayerRotateLeft(ShipID::PLAYER2);
		}

		if (AEInputCheckCurr(AEVK_D))
		{
			PlayerRotateRight(ShipID::PLAYER2);
		}

		// Shoot a bullet if space is triggered (Create a new object instance)
		if (AEInputCheckTriggered(AEVK_E))
		{
			PlayerShoot(ShipID::PLAYER2);
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
			GameObjInst* pInst = GameObjFactory_->GetGameObjInstData() + i;

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

	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = GameObjFactory_->GetGameObjInstData() + i;

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
				GameObjFactory_->gameObjInstDestroy(pInst);

			// if the bullet goes out of y-axis, remove
			if (pInst->posCurr.y > AEGfxGetWinMaxY() || pInst->posCurr.y < AEGfxGetWinMinY())
				GameObjFactory_->gameObjInstDestroy(pInst);
		}
	}


	// ====================
	// check for collision
	// ====================

	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = GameObjFactory_->GetGameObjInstData() + i;

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
				GameObjInst* _pInst = GameObjFactory_->GetGameObjInstData() + j;

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
						if (_pInst->shipComp.sShipLives > 1)
						{
							bulletExplosion(_pInst->shipComp.sShipID);
						}

						// Decrease ship lives by 1
						_pInst->shipComp.sShipLives -= 1;

						// Decrement Asteroids by 1
						ASTEROID_COUNT -= 1;

						// Reset Ship Position
						AEVec2Set(&_pInst->posCurr,
							_pInst->shipComp.InitialPosition.x,
							_pInst->shipComp.InitialPosition.y);

						// Reset Ship Velocity
						AEVec2Set(&_pInst->velCurr, 0.f, 0.f);
						// Reset Direction
						_pInst->dirCurr = _pInst->shipComp.InitialDirection;
						// Resets Special Power
						_pInst->shipComp.SPECIAL_TRIGGER = 0;

						// Destroy Asteroid
						GameObjFactory_->gameObjInstDestroy(pInst);

						// Set flag to true
						if (_pInst->shipComp.sShipScore <= 5000)
						{
							onValueChange = true;
						}

						if (sShipLives < 0)
						{
							// Reset Ship Position
							AEVec2Set(&myShip->posCurr, 0.f, 0.f);
							// Reset Ship Velocity
							AEVec2Set(&myShip->velCurr, 0.f, 0.f);
							// Reset Ship Direction
							myShip->dirCurr = 0.f;

							// Set flag to false
							onValueChange = false;
						}
					}
				}
				else if (_pInst->pObject->type == TYPE_BULLET) 
				{
					// Check collision between asteroid and bullet
					if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr,
						_pInst->boundingBox, _pInst->velCurr))
					{
						// Destroy Asteroid
						GameObjFactory_->gameObjInstDestroy(pInst);

						// Remove Bullet
						GameObjFactory_->gameObjInstDestroy(_pInst);

						// Update Score
						IDToPlayerShip_[_pInst->BulletSource]->shipComp.sShipScore += 100;
						sScore += 100;

						// Increment Special Trigger by 1
						IDToPlayerShip_[_pInst->BulletSource]->shipComp.SPECIAL_TRIGGER += 1;

						if (IDToPlayerShip_[_pInst->BulletSource]->shipComp.SPECIAL_TRIGGER >= 5)
						{
							IDToPlayerShip_[_pInst->BulletSource]->shipComp.SPECIAL_CHECK = TRUE;
						}
						else
						{
							IDToPlayerShip_[_pInst->BulletSource]->shipComp.SPECIAL_CHECK = FALSE;
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
						if (IDToPlayerShip_[_pInst->BulletSource]->shipComp.sShipScore > 5000)
						{
							//// Reset Ship Position
							//AEVec2Set(&myShip->posCurr, 0.f, 0.f);
							//// Reset Ship velocity
							//AEVec2Set(&myShip->velCurr, 0.f, 0.f);
							//// Reset Ship direction
							//myShip->dirCurr = 0.f;

							// Set flag to false
							onValueChange = false;
						}
					}

				}
			}
		}
	}

	// =====================================
	// calculate the matrix for all objects
	// =====================================

	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = GameObjFactory_->GetGameObjInstData() + i;
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

AsteroidsGameState::AsteroidsGameState() :
	GameObjFactory_{ std::make_shared<GameObjectFactory>() }
{}

void AsteroidsGameState::GameStateAsteroidsLoad(void)
{
	// zero the game object array
	memset(GameObjFactory_->GetGameObjData(), 0, sizeof(GameObj) * GAME_OBJ_NUM_MAX);
	// No game objects (shapes) at this point
	GameObjFactory_->SetGameObjSize(0);

	// zero the game object instance array
	memset(GameObjFactory_->GetGameObjInstData(), 0, sizeof(GameObjInst) * GAME_OBJ_INST_NUM_MAX);
	// No game object instances (sprites) at this point
	GameObjFactory_->SetGameObjInstSize(0);

	// load/create the mesh data (game objects / Shapes)
	GameObj* pObj;

	// =====================
	// create the ship shape
	// =====================

	pObj = GameObjFactory_->GetNextGameObjectPointer();
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

	pObj = GameObjFactory_->GetNextGameObjectPointer();
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

	pObj = GameObjFactory_->GetNextGameObjectPointer();
	pObj->type = TYPE_ASTEROID;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF808080, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFF808080, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF808080, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFF808080, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFF808080, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF808080, 0.0f, 0.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create object!!");
}

void AsteroidsGameState::GameStateAsteroidsInit(void)
{
	for (size_t i = 0; i < GSManager->PlayerCount_; ++i)
	{
		if (i == 0)
		{
			AEVec2 pos{ -20.f, 20.f };
			myShip = GameObjFactory_->gameObjInstCreate(TYPE_SHIP, SHIP_SIZE, &pos, nullptr, AEDegToRad(180.f));
			IDToPlayerShip_[static_cast<ShipID>(i)] = myShip;
			IDToPlayerShip_[static_cast<ShipID>(i)]->shipComp.sShipID = static_cast<ShipID>(i);
			IDToPlayerShip_[static_cast<ShipID>(i)]->shipComp.sShipScore = 0;
			IDToPlayerShip_[static_cast<ShipID>(i)]->shipComp.sShipLives = SHIP_INITIAL_NUM;
		}
		else
		{
			AEVec2 pos;
			float dir = 0.0f;

			switch (i)
			{
			case 1:
				pos.x = 20.f;
				pos.y = 20.f;
				dir = 0.0f;
				break;
			case 2:
				pos.x = -20.f;
				pos.y = 0.f;
				dir = 180.0f;
				break;
			case 3:
				pos.x = 20.f;
				pos.y = 0.f;
				dir = 0.0f;
				break;
			}

			GameObjInst* temp = GameObjFactory_->gameObjInstCreate(TYPE_SHIP, SHIP_SIZE, &pos, nullptr, AEDegToRad(dir));
			IDToPlayerShip_[static_cast<ShipID>(i)] = temp;
			IDToPlayerShip_[static_cast<ShipID>(i)]->shipComp.sShipID = static_cast<ShipID>(i);
			IDToPlayerShip_[static_cast<ShipID>(i)]->shipComp.sShipScore = 0;
			IDToPlayerShip_[static_cast<ShipID>(i)]->shipComp.sShipLives = SHIP_INITIAL_NUM;
		}

		AE_ASSERT(IDToPlayerShip_[static_cast<ShipID>(i)]);
	}

	// CREATE THE INITIAL ASTEROIDS INSTANCES USING THE "gameObjInstCreate" FUNCTION
	for (int i = 0; i < 4; i++)
	{
		spawnAsteroid();
	}

	sShipLives = SHIP_INITIAL_NUM;
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
		GameObjInst* pInst = GameObjFactory_->GetGameObjInstData() + i;

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
		GameObjInst* pInst = GameObjFactory_->GetGameObjInstData() + i;
		GameObjFactory_->gameObjInstDestroy(pInst);
	}

	IDToPlayerShip_.clear();
}

void AsteroidsGameState::GameStateAsteroidsUnload(void)
{
	// free all mesh data (shapes) of each object using "AEGfxTriFree"
	for (unsigned long i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj* pInst = GameObjFactory_->GetGameObjData() + i;
		if ((pInst->pMesh == nullptr))
			continue;
		AEGfxMeshFree(pInst->pMesh);
	}
}

void AsteroidsGameState::PlayerMoveForward(ShipID PlayerID)
{
	auto search = IDToPlayerShip_.find(PlayerID);

	if (search != IDToPlayerShip_.end())
	{
		AEVec2 added;
		AEVec2Set(&added, cosf(IDToPlayerShip_[PlayerID]->dirCurr),
			sinf(IDToPlayerShip_[PlayerID]->dirCurr));
		AEVec2Add(&IDToPlayerShip_[PlayerID]->posCurr,
			&IDToPlayerShip_[PlayerID]->posCurr, &added);

		// Find the velocity according to the acceleration
		IDToPlayerShip_[PlayerID]->velCurr.x += added.x * SHIP_ACCEL_FORWARD * g_dt;
		IDToPlayerShip_[PlayerID]->velCurr.y += added.y * SHIP_ACCEL_FORWARD * g_dt;
		// Limit your speed over here
		AEVec2Scale(&IDToPlayerShip_[PlayerID]->velCurr,
			&IDToPlayerShip_[PlayerID]->velCurr, static_cast<f32>(0.99));
	}
}

void AsteroidsGameState::PlayerMoveBackwards(ShipID PlayerID)
{
	auto search = IDToPlayerShip_.find(PlayerID);

	if (search != IDToPlayerShip_.end())
	{
		AEVec2 added;
		AEVec2Set(&added, -cosf(IDToPlayerShip_[PlayerID]->dirCurr),
			-sinf(IDToPlayerShip_[PlayerID]->dirCurr));
		AEVec2Add(&IDToPlayerShip_[PlayerID]->posCurr,
			&IDToPlayerShip_[PlayerID]->posCurr, &added);//YOU MAY NEED TO CHANGE/REPLACE THIS LINE

		// Find the velocity according to the decceleration
		IDToPlayerShip_[PlayerID]->velCurr.x += g_dt * SHIP_ACCEL_BACKWARD * added.x;
		IDToPlayerShip_[PlayerID]->velCurr.y += g_dt * SHIP_ACCEL_BACKWARD * added.y;
		// Limit your speed over here
		AEVec2Scale(&IDToPlayerShip_[PlayerID]->velCurr,
			&IDToPlayerShip_[PlayerID]->velCurr, static_cast<f32>(0.99));
	}
}

void AsteroidsGameState::PlayerRotateLeft(ShipID PlayerID)
{
	auto search = IDToPlayerShip_.find(PlayerID);

	if (search != IDToPlayerShip_.end())
	{
		IDToPlayerShip_[PlayerID]->dirCurr += SHIP_ROT_SPEED *
			(float)(AEFrameRateControllerGetFrameTime());
		IDToPlayerShip_[PlayerID]->dirCurr = AEWrap(IDToPlayerShip_[PlayerID]->dirCurr, -PI, PI);
	}
}

void AsteroidsGameState::PlayerRotateRight(ShipID PlayerID)
{
	auto search = IDToPlayerShip_.find(PlayerID);

	if (search != IDToPlayerShip_.end())
	{
		IDToPlayerShip_[PlayerID]->dirCurr -=
			SHIP_ROT_SPEED * (float)(AEFrameRateControllerGetFrameTime());
		IDToPlayerShip_[PlayerID]->dirCurr = AEWrap(IDToPlayerShip_[PlayerID]->dirCurr, -PI, PI);
	}
}

void AsteroidsGameState::PlayerShoot(ShipID PlayerID)
{
	auto search = IDToPlayerShip_.find(PlayerID);

	if (search != IDToPlayerShip_.end())
	{
		if (IDToPlayerShip_[PlayerID]->shipComp.SPECIAL_CHECK == TRUE)
		{
			for (int m = 0; m < 5; ++m)
			{
				spawnBulletHell(m, PlayerID);
			}

			IDToPlayerShip_[PlayerID]->shipComp.SPECIAL_TRIGGER = 0;
		}
		else {
			AEVec2 Bullet;
			// Get the bullet's direction according to the ship's direction
			AEVec2Set(&Bullet, cosf(IDToPlayerShip_[PlayerID]->dirCurr),
				sinf(IDToPlayerShip_[PlayerID]->dirCurr));
			// Set the velocity
			Bullet.x += BULLET_SPEED * Bullet.x;
			Bullet.y += BULLET_SPEED * Bullet.y;
			// Create an instance
			GameObjInst* bullet = GameObjFactory_->gameObjInstCreate(TYPE_BULLET, 10.0f,
				&IDToPlayerShip_[PlayerID]->posCurr, &Bullet, IDToPlayerShip_[PlayerID]->dirCurr);
			bullet->BulletSource = PlayerID;
		}
	}
}

// Creates bullet explosion when ship dies
void AsteroidsGameState::bulletExplosion(ShipID PlayerID)
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
		GameObjFactory_->gameObjInstCreate(TYPE_BULLET, 20.0f, 
			&IDToPlayerShip_[PlayerID]->posCurr, &BulletExp, BEDir);

		BERot += 0.2f;
	}
}

// Spawns an asteroid
void AsteroidsGameState::spawnAsteroid(void)
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
	added.y += static_cast<float>(velocity) * added.y;

	// Creates the game object
	GameObjFactory_->gameObjInstCreate(TYPE_ASTEROID, static_cast<float>(size), &AsteroidPos[index], &added, dir);

	// Keeps track of the spawned asteroids on screen
	ASTEROID_COUNT++;
}

// Creates Bullet Hell Mechanic
void AsteroidsGameState::spawnBulletHell(int i, ShipID PlayerID)
{
	AEVec2 BulletHell;
	AEVec2Set(&BulletHell, cosf(IDToPlayerShip_[PlayerID]->dirCurr) + 0.3f * i, 
		sinf(IDToPlayerShip_[PlayerID]->dirCurr) + 0.3f * i);
	// Set the velocity
	BulletHell.x += BULLET_SPEED * BulletHell.x;
	BulletHell.y += BULLET_SPEED * BulletHell.y;
	// Get the bullet's direction according to the ship's direction
	GameObjFactory_->gameObjInstCreate(TYPE_BULLET, 10.0f, &IDToPlayerShip_[PlayerID]->posCurr,
		&BulletHell, IDToPlayerShip_[PlayerID]->dirCurr);
}
