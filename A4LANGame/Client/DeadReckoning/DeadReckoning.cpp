#include "pch.h"
#include "DeadReckoning.h"
#include "Global.h"
void DeadReckoning::Init(AEVec2 Position, AEVec2 Velocity, AEVec2 Acceleration, float direction)
{
	LastKnownPosition = Position;
	LastKnownVelocity = Velocity;
	LastKnownAcceleration = Acceleration;
	Mydirection = direction;
	isInit = true;
}
void DeadReckoning::Predict(AEVec2& UpdatePosition,AEVec2& UpdateVelocity,float& direction,float dt)
{
	UpdatePosition.x = static_cast<float>(LastKnownPosition.x + (LastKnownVelocity.x * dt));
	UpdatePosition.y = static_cast<float>(LastKnownPosition.y + (LastKnownVelocity.y * dt));
	UpdateVelocity = LastKnownVelocity;
	CurrentPosition = UpdatePosition;
	CurrentVelocity = UpdateVelocity;
	direction = Mydirection;
}

void DeadReckoning::UpdateTime(float dt)
{
	if (!isInit)
		return;

	TimeelapsedsinceUpdate += dt;
}

void DeadReckoning::ReceivedPacket(AEVec2 LKPosition, AEVec2 LKVelocity, AEVec2 LKAcceleration,float direction,double apptime)
{
	OldPosition = CurrentPosition;
	OldVelocity = InstantVelocityBetweenDRpositions;
	TimeelapsedsinceUpdate = 0;
	LastKnownPosition = LKPosition;
	LastKnownVelocity = LKVelocity;
	LastKnownAcceleration = LKAcceleration;
	Mydirection = direction;
	if (isInit)
		extrapolating = true;
}

void DeadReckoning::Snap(AEVec2& UpdatePosition, AEVec2& UpdateVelocity,float& direction)
{
	OldPosition = LastKnownPosition;
	OldVelocity = LastKnownVelocity;
	UpdatePosition = OldPosition;
	UpdateVelocity = OldVelocity;
	direction = Mydirection;
}

void DeadReckoning::Correction(AEVec2& UpdatePosition,AEVec2& UpdateVelocity,float& direction,float dt)
{
	AEVec2 VelocityBlend;
	AEVec2 Pt;
	AEVec2 PtPrime;
	AEVec2 FinalPosition;
	Tarrow = TimeelapsedsinceUpdate / Ttriangle;
	VelocityBlend.x = static_cast<float>(OldVelocity.x + (LastKnownVelocity.x - OldVelocity.x) * Tarrow);
	VelocityBlend.y = static_cast<float>(OldVelocity.y + (LastKnownVelocity.y - OldVelocity.y) * Tarrow);
	Pt.x = static_cast<float>(OldPosition.x + (VelocityBlend.x * TimeelapsedsinceUpdate));
	Pt.y = static_cast<float>(OldPosition.y + (VelocityBlend.y * TimeelapsedsinceUpdate));
	PtPrime.x = static_cast<float>(LastKnownPosition.x + (LastKnownVelocity.x * TimeelapsedsinceUpdate));
	PtPrime.y = static_cast<float>(LastKnownPosition.y + (LastKnownVelocity.y * TimeelapsedsinceUpdate));

	if (TimeelapsedsinceUpdate > Ttriangle)
	{
		FinalPosition.x = static_cast<float>(LastKnownPosition.x + (LastKnownVelocity.x * TimeelapsedsinceUpdate));
		FinalPosition.y = static_cast<float>(LastKnownPosition.y + (LastKnownVelocity.y * TimeelapsedsinceUpdate));
	}					
	else
	{
		FinalPosition.x = static_cast<float>(Pt.x + ((PtPrime.x - Pt.x) * Tarrow));
		FinalPosition.y = static_cast<float>(Pt.y + ((PtPrime.y - Pt.y) * Tarrow));
	}

	InstantVelocityBetweenDRpositions.x = (FinalPosition.x - CurrentPosition.x) / dt;
	InstantVelocityBetweenDRpositions.y = (FinalPosition.y - CurrentPosition.y) / dt;

	UpdatePosition = FinalPosition;
	CurrentPosition = FinalPosition;
	UpdateVelocity = VelocityBlend;
	CurrentVelocity = VelocityBlend;
	direction = Mydirection;
}

void DeadReckoning::Run(AEVec2& UpdatePosition, AEVec2& UpdateVelocity,float& direction,float dt)
{
	if (!isInit)
		return;

	if (!extrapolating)
		Predict(UpdatePosition, UpdateVelocity,direction,dt);
	else
		Correction(UpdatePosition, UpdateVelocity,direction,dt);
}