#include "pch.h"
#include "DeadReckoning.h"
void DeadReckoning::Predict(AEVec2& UpdatePosition,AEVec2& UpdateVelocity,float& direction)
{
	UpdatePosition.x = static_cast<float>(LastKnownPosition.x + (LastKnownVelocity.x * g_dt) + (0.5 * LastKnownAcceleration.x * g_dt * g_dt));
	UpdatePosition.y = static_cast<float>(LastKnownPosition.y + (LastKnownVelocity.y * g_dt) + (0.5 * LastKnownAcceleration.y * g_dt * g_dt));
	UpdateVelocity = OldVelocity;
	direction = Mydirection;
}

void DeadReckoning::UpdateTime()
{
	TimeelapsedsinceUpdate += g_dt;
}

void DeadReckoning::ReceivedPacket(AEVec2 LKPosition, AEVec2 LKVelocity, AEVec2 LKAcceleration,float direction)
{
	OldPosition = LastKnownPosition;
	OldVelocity = InstantVelocityBetweenDRpositions;
	OldAcceleration = LastKnownAcceleration;
	TimeOfUpdate = static_cast<float>(g_appTime);
	TimeelapsedsinceUpdate = 0;
	LastKnownPosition = LKPosition;
	LastKnownVelocity = LKVelocity;
	LastKnownAcceleration = LKAcceleration;
	Mydirection = direction;
	
	if (secondUpdate)
	{
		extrapolating = true;
	}
	else
		secondUpdate = true;
}

void DeadReckoning::Snap(AEVec2& UpdatePosition, AEVec2& UpdateVelocity,float& direction)
{
	OldPosition = LastKnownPosition;
	OldVelocity = LastKnownVelocity;
	OldAcceleration = LastKnownAcceleration;
	UpdatePosition = OldPosition;
	UpdateVelocity = OldVelocity;
	direction = Mydirection;
}

void DeadReckoning::Correction(AEVec2& UpdatePosition,AEVec2& UpdateVelocity,float& direction)
{
	AEVec2 VelocityBlend;
	AEVec2 Pt;
	AEVec2 PtPrime;
	AEVec2 FinalPosition;
	Tarrow = TimeelapsedsinceUpdate / Ttriangle;
	VelocityBlend.x = static_cast<float>(OldVelocity.x + (LastKnownVelocity.x - OldVelocity.x) * Tarrow);
	VelocityBlend.y = static_cast<float>(OldVelocity.y + (LastKnownVelocity.y - OldVelocity.y) * Tarrow);
	Pt.x = static_cast<float>(OldPosition.x + (VelocityBlend.x * TimeelapsedsinceUpdate) + (0.5 * LastKnownAcceleration.x * TimeelapsedsinceUpdate * TimeelapsedsinceUpdate));
	Pt.y = static_cast<float>(OldPosition.y + (VelocityBlend.y * TimeelapsedsinceUpdate) + (0.5 * LastKnownAcceleration.y * TimeelapsedsinceUpdate * TimeelapsedsinceUpdate));
	PtPrime.x = static_cast<float>(LastKnownPosition.x + (LastKnownVelocity.x * TimeelapsedsinceUpdate) + (0.5 * LastKnownAcceleration.x * TimeelapsedsinceUpdate * TimeelapsedsinceUpdate));
	PtPrime.y = static_cast<float>(LastKnownPosition.y + (LastKnownVelocity.y * TimeelapsedsinceUpdate) + (0.5 * LastKnownAcceleration.y * TimeelapsedsinceUpdate * TimeelapsedsinceUpdate));

	if (TimeelapsedsinceUpdate > Ttriangle)
	{
		FinalPosition.x = static_cast<float>(LastKnownPosition.x + (LastKnownVelocity.x * TimeelapsedsinceUpdate) + (0.5 * LastKnownAcceleration.x * TimeelapsedsinceUpdate * TimeelapsedsinceUpdate));
		FinalPosition.y = static_cast<float>(LastKnownPosition.y + (LastKnownVelocity.y * TimeelapsedsinceUpdate) + (0.5 * LastKnownAcceleration.y * TimeelapsedsinceUpdate * TimeelapsedsinceUpdate));
	}					
	else
	{
		FinalPosition.x = static_cast<float>(Pt.x + ((PtPrime.x - Pt.x) * Tarrow));
		FinalPosition.y = static_cast<float>(Pt.y + ((PtPrime.y - Pt.y) * Tarrow));
	}

	InstantVelocityBetweenDRpositions.x = (FinalPosition.x - UpdatePosition.x) / g_dt;
	InstantVelocityBetweenDRpositions.y = (FinalPosition.y - UpdatePosition.y) / g_dt;

	UpdatePosition = FinalPosition;
	UpdateVelocity = VelocityBlend;
	direction = Mydirection;
}

void DeadReckoning::Run(AEVec2& UpdatePosition, AEVec2& UpdateVelocity,float& direction)
{
	if (!extrapolating)
		Predict(UpdatePosition, UpdateVelocity,direction);
	else
		Correction(UpdatePosition, UpdateVelocity,direction);
}