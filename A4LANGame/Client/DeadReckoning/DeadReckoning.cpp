#include "pch.h"
#include "DeadReckoning.h"
void DeadReckoning::Predict(AEVec2& UpdatePosition,AEVec2& UpdateVelocity)
{
	UpdatePosition.x = static_cast<float>(LastKnownPosition.x + (LastKnownVelocity.x * g_dt) + (0.5 * LastKnownAcceleration.x * g_dt * g_dt));
	UpdatePosition.y = static_cast<float>(LastKnownPosition.y + (LastKnownVelocity.y * g_dt) + (0.5 * LastKnownAcceleration.y * g_dt * g_dt));
	UpdateVelocity = OldVelocity;
	extrapolating = true;
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
	TimeOfUpdate = static_cast<float>(g_appTime);
	TimeelapsedsinceUpdate = 0;
	LastKnownPosition = LKPosition;
	LastKnownVelocity = LKVelocity;
	LastKnownAcceleration = LKAcceleration;
}

void DeadReckoning::Snap(AEVec2& UpdatePosition, AEVec2& UpdateVelocity)
{
	OldPosition = LastKnownPosition;
	OldVelocity = LastKnownVelocity;
	OldAcceleration = LastKnownAcceleration;
	UpdatePosition = OldPosition;
	UpdateVelocity = OldVelocity;
}

void DeadReckoning::Correction(AEVec2& UpdatePosition,AEVec2& UpdateVelocity)
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
		FinalPosition.x = static_cast<float>(Pt.x + (PtPrime.x - Pt.x) * Tarrow);
		FinalPosition.y = static_cast<float>(Pt.y + (PtPrime.y - Pt.y) * Tarrow);
	}

	UpdatePosition = FinalPosition;
	UpdateVelocity = VelocityBlend;
}

void DeadReckoning::Run(AEVec2& UpdatePosition, AEVec2& UpdateVelocity)
{
	if (!extrapolating)
		Predict(UpdatePosition, UpdateVelocity);
	else
		Correction(UpdatePosition, UpdateVelocity);
}