#pragma once
#include "Main.h"
class DeadReckoning
{
	bool extrapolating{ false };
	AEVec2 OldVelocity{ 0,0 };
	AEVec2 OldPosition{ 0,0 };
	AEVec2 OldAcceleration{ 0,0 };
	AEVec2 LastKnownPosition{ 0,0 };
	AEVec2 LastKnownVelocity{ 0,0 };
	AEVec2 LastKnownAcceleration{ 0,0 };

	float TimeOfUpdate{ 0 };
	float TimeelapsedsinceUpdate{ 0 };
	float Ttriangle{ 1 / 60 };
	float Tarrow{ 0 };
	void predict(AEVec2& position, AEVec2& velocity, AEVec2& acceleration);
	void ReceivedPacket(AEVec2 LKPosition,AEVec2 LKVelocity,AEVec2 LKAcceleration);
	void UpdateTime();
	void correction(AEVec2& UpdatePosition, AEVec2& UpdateVelocity);
};