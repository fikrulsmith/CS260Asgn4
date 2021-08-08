#pragma once
#include "AEEngine.h"
class DeadReckoning
{
	AEVec2 OldVelocity{ 0,0 };
	AEVec2 OldPosition{ 0,0 };
	AEVec2 OldAcceleration{ 0,0 };
	AEVec2 LastKnownPosition{ 0,0 };
	AEVec2 LastKnownVelocity{ 0,0 };
	AEVec2 LastKnownAcceleration{ 0,0 };
	AEVec2 InstantVelocityBetweenDRpositions{0,0};

	float TimeOfUpdate{ 0 };
	float TimeelapsedsinceUpdate{ 0 };
	float Ttriangle{ 1 / 5 };
	float Mydirection;
	float Tarrow{ 0 };

	bool extrapolating{ false };
	bool secondUpdate{ false };
	public:
	void Predict(AEVec2& UpdatePosition, AEVec2& UpdateVelocity,float& direction,float dt);
	void Run(AEVec2& UpdatePosition, AEVec2& UpdateVelocity,float& direction,float dt);
	void ReceivedPacket(AEVec2 LKPosition,AEVec2 LKVelocity,AEVec2 LKAcceleration,float direction,double apptime);
	void UpdateTime(float dt);
	void Correction(AEVec2& UpdatePosition, AEVec2& UpdateVelocity,float& direction,float dt);
	void Snap(AEVec2& UpdatePosition, AEVec2& UpdateVelocity,float& direction);
};