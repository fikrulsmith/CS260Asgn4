#include "pch.h"
#include "DeadReckoning.h"
void DeadReckoning::predict(AEVec2& UpdatePosition,AEVec2& UpdateVelocity,AEVec2& UpdateAcceleration)
{
	if (extrapolating)
		return;
	UpdatePosition.x = static_cast<float>(OldPosition.x + (OldVelocity.x * g_dt) + (0.5 * OldAcceleration.x * g_dt * g_dt));
	UpdatePosition.y = static_cast<float>(OldPosition.y + (OldVelocity.y * g_dt) + (0.5 * OldAcceleration.y * g_dt * g_dt));
	UpdateVelocity = OldVelocity;																				
}

void DeadReckoning::UpdateTime()
{
	TimeelapsedsinceUpdate += g_dt;
}

void DeadReckoning::ReceivedPacket(AEVec2 LKPosition, AEVec2 LKVelocity, AEVec2 LKAcceleration)
{
	OldPosition = LastKnownPosition;
	OldVelocity = LastKnownVelocity;
	OldAcceleration = LastKnownAcceleration;
	TimeOfUpdate = g_appTime;
	TimeelapsedsinceUpdate = 0;
	LastKnownPosition = LKPosition;
	LastKnownVelocity = LKVelocity;
	LastKnownAcceleration = LKAcceleration;
	extrapolating = true;
}

void DeadReckoning::correction(AEVec2& UpdatePosition,AEVec2& UpdateVelocity)
{
	if (!extrapolating)
		return;
	AEVec2 VelocityBlend;
	AEVec2 Pt;
	AEVec2 PtPrime;
	AEVec2 FinalPosition;
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
		FinalPosition.x = static_cast<float>(Pt.x + (PtPrime.x - Pt.x) * Tarrow);
		FinalPosition.y = static_cast<float>(Pt.y + (PtPrime.y - Pt.y) * Tarrow);
	}

	UpdatePosition = FinalPosition;
	UpdateVelocity = VelocityBlend;
}