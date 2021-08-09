#pragma once
#include "AEEngine.h"
class DeadReckoning
{
	AEVec2 OldVelocity{ 0,0 };
	AEVec2 OldPosition{ 0,0 };
	AEVec2 OldAcceleration{ 0,0 };
	AEVec2 LastKnownPosition{ 0,0 };
	AEVec2 LastKnownVelocity{ 0,0 };
	AEVec2 CurrentPosition{ 0,0 };
	AEVec2 CurrentVelocity{ 0,0 };
	AEVec2 LastKnownAcceleration{ 0,0 };

	AEVec2 InstantVelocityBetweenDRpositions{0,0};

	float TimeelapsedsinceUpdate{ 0 };
	float Ttriangle{ 0.2f };
	float Mydirection;
	float Tarrow{ 0 };

	bool extrapolating{ false };
	bool isInit{ false };
	public:
	void Init(AEVec2 Position, AEVec2 Velocity, AEVec2 Acceleration, float direction);
	void Predict(AEVec2& UpdatePosition, AEVec2& UpdateVelocity,float& direction,float dt);
	void Run(AEVec2& UpdatePosition, AEVec2& UpdateVelocity,float& direction,float dt);
	void ReceivedPacket(AEVec2 LKPosition,AEVec2 LKVelocity,AEVec2 LKAcceleration,float direction,double apptime);
	void UpdateTime(float dt);
	void Correction(AEVec2& UpdatePosition, AEVec2& UpdateVelocity,float& direction,float dt);
	void Snap(AEVec2& UpdatePosition, AEVec2& UpdateVelocity,float& direction);
};