#pragma once
#include "pch.h"

#include "Collision.h"

// The total number of different objects (Shapes)
const unsigned int	GAME_OBJ_NUM_MAX = 32;
// The total number of different game object instances
const unsigned int	GAME_OBJ_INST_NUM_MAX = 2048;
// The idea is to display any of these variables/strings whenever a change in their value happens changes
static bool			onValueChange = true;
// object flag definition
const unsigned long FLAG_ACTIVE = 0x00000001;

enum TYPE
{
	// list of game object types
	TYPE_SHIP = 0,
	TYPE_BULLET,
	TYPE_ASTEROID,
	TYPE_HEALTH,

	TYPE_NUM
};

//Game object structure
struct GameObj
{
	// object type
	unsigned long type;		
	// This will hold the triangles which will form the shape of the object
	AEGfxVertexList* pMesh;
};

//Game object instance structure
struct GameObjInst
{
	// pointer to the 'original' shape
	GameObj* pObject;
	// bit flag or-ed together
	unsigned long flag;
	// scaling value of the object instance
	float scale;
	// object current position
	AEVec2 posCurr;
	// object current velocity
	AEVec2 velCurr;
	// object current direction
	float dirCurr;
	// object bouding box that encapsulates the object
	AABB boundingBox;
	// object transformation matrix: Each frame, 
	// calculate the object instance's transformation matrix and save it here
	AEMtx33 transform;
};

enum class GameSystemType
{
	UNASSIGNED,
	LOAD,
	INIT,
	UPDATE,
	DRAW,
	FREE,
	UNLOAD
};

struct ClientInfo
{
	SOCKET socket = INVALID_SOCKET;
	addrinfo* addr = nullptr;
	std::string name;
	std::string port;

	GameObjInst* playerEntity = nullptr;
};
