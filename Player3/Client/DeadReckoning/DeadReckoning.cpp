/******************************************************************************/
/*!
\file DeadReckoning.cpp
\author Ong Guan Hin
\par email: guanhin.ong\@digipen.edu
\par DigiPen login: guanhin.ong
\par Course: CS260-B
\par Assignment #04
\date 10/08/2021
\brief
This file contains an implementation of the dead reckoning algorithm
*/
/******************************************************************************/
#include "pch.h"
#include "DeadReckoning.h"
#include "Global.h"
/******************************************************************************/
/*!
\fn void DeadReckoning::Init(AEVec2 Position, AEVec2 Velocity, AEVec2 Acceleration, float direction)
\brief Initialite DR base values
\param Position
\param Velocity
\param Acceleration
\param direction
*/
/******************************************************************************/
void DeadReckoning::Init(AEVec2 Position, AEVec2 Velocity, AEVec2 Acceleration, float direction)
{
	LastKnownPosition = Position;
	LastKnownVelocity = Velocity;
	LastKnownAcceleration = Acceleration;
	Mydirection = direction;
	isInit = true;
}
/******************************************************************************/
/*!
\fn void DeadReckoning::Predict(AEVec2& UpdatePosition, AEVec2& UpdateVelocity, float& direction, float dt)
\brief predict DR for first frame
\param UpdatePosition
\param UpdateVelocity
\param direction
\param dt
*/
/******************************************************************************/
void DeadReckoning::Predict(AEVec2& UpdatePosition, AEVec2& UpdateVelocity, float& direction, float dt)
{
	UpdatePosition.x = static_cast<float>(LastKnownPosition.x + (LastKnownVelocity.x * dt));
	UpdatePosition.y = static_cast<float>(LastKnownPosition.y + (LastKnownVelocity.y * dt));
	UpdateVelocity = LastKnownVelocity;
	CurrentPosition = UpdatePosition;
	CurrentVelocity = UpdateVelocity;
	direction = Mydirection;
}
/******************************************************************************/
/*!
\fn void DeadReckoning::UpdateTime(float dt)
\brief Update timeelapsed for DR
\param dt
*/
/******************************************************************************/
void DeadReckoning::UpdateTime(float dt)
{
	if (!isInit)
		return;

	TimeelapsedsinceUpdate += dt;
}
/******************************************************************************/
/*!
\fn void DeadReckoning::ReceivedPacket(AEVec2 LKPosition, AEVec2 LKVelocity, AEVec2 LKAcceleration, float direction)
\brief update DR after receiving a packet
\param LKPosition
\param LKVelocity
\param LKAcceleration
\param direction
*/
/******************************************************************************/
void DeadReckoning::ReceivedPacket(AEVec2 LKPosition, AEVec2 LKVelocity, AEVec2 LKAcceleration, float direction)
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
/******************************************************************************/
/*!
\fn DeadReckoning::Snap(AEVec2& UpdatePosition, AEVec2& UpdateVelocity, float& direction)
\brief snaps in case interpolation doesn't work
\param UpdatePosition
\param UpdateVelocity
\param direction
*/
/******************************************************************************/
void DeadReckoning::Snap(AEVec2& UpdatePosition, AEVec2& UpdateVelocity, float& direction)
{
	OldPosition = LastKnownPosition;
	OldVelocity = LastKnownVelocity;
	UpdatePosition = OldPosition;
	UpdateVelocity = OldVelocity;
	direction = Mydirection;
}
/******************************************************************************/
/*!
\fn void DeadReckoning::reset()
\brief resets DR
*/
/******************************************************************************/
void DeadReckoning::reset()
{
	OldPosition = { 0,0 };
	OldVelocity = { 0,0 };
	LastKnownPosition = { 0,0 };
	LastKnownVelocity = { 0,0 };
	LastKnownAcceleration = { 0,0 };
	CurrentPosition = { 0,0 };
	CurrentVelocity = { 0,0 };
	InstantVelocityBetweenDRpositions = { 0,0 };

	TimeelapsedsinceUpdate = 0;
	Mydirection = 0;
	Tarrow = 0;

	extrapolating = false;
	isInit = false;
	goingFront = false;
	goingBack = false;
	RotLeft = false;
	RotRight = false;
}
/******************************************************************************/
/*!
\fn void DeadReckoning::Correction(AEVec2& UpdatePosition, AEVec2& UpdateVelocity, float& direction, float dt, ShipID id)
\brief does DR correction after 2nd update
\param UpdatePosition
\param UpdateVelocity
\param direction
\param dt
\param id
*/
/******************************************************************************/
void DeadReckoning::Correction(AEVec2& UpdatePosition, AEVec2& UpdateVelocity, float& direction, float dt, ShipID id)
{
	AEVec2 VelocityBlend;
	AEVec2 Pt;
	AEVec2 PtPrime;
	AEVec2 FinalPosition;

	AEVec2 Added{ 0,0 };
	if (clientManager->GetClient(clientManager->GetClientByID(id))->state == ShipState::MOVINGFORWARD)
	{
		goingFront = true;
		goingBack = false;
	}
	if (clientManager->GetClient(clientManager->GetClientByID(id))->state == ShipState::MOVINGBACKWARDS)
	{
		goingBack = true;
		goingFront = false;
	}

	if (clientManager->GetClient(clientManager->GetClientByID(id))->state == ShipState::ROTATINGLEFT)
	{
		RotLeft = true;
		RotRight = false;
	}
	if (clientManager->GetClient(clientManager->GetClientByID(id))->state == ShipState::ROTATINGRIGHT)
	{
		RotLeft = false;
		RotRight = true;
	}

	if (clientManager->GetClient(clientManager->GetClientByID(id))->state == ShipState::NOTHING)
	{
		goingBack = false;
		goingFront = false;
		RotLeft = false;
		RotRight = false;
	}

	if (RotLeft)
	{
		Mydirection += (2.0f * PI) *
			(float)(AEFrameRateControllerGetFrameTime());
		Mydirection = AEWrap(Mydirection, -PI, PI);

		if (goingFront)
			AEVec2Set(&Added, cosf(Mydirection), sinf(Mydirection));
		else if (goingBack)
			AEVec2Set(&Added, -cosf(Mydirection), -sinf(Mydirection));

		if (goingBack || goingFront)
		{
			LastKnownVelocity.x = Added.x * 80.0f;
			LastKnownVelocity.y = Added.y * 80.0f;
		}
	}
	else if (RotRight)
	{
		Mydirection -= (2.0f * PI) *
			(float)(AEFrameRateControllerGetFrameTime());
		Mydirection = AEWrap(Mydirection, -PI, PI);

		if (goingFront)
			AEVec2Set(&Added, cosf(Mydirection), sinf(Mydirection));
		else if (goingBack)
			AEVec2Set(&Added, -cosf(Mydirection), -sinf(Mydirection));

		if (goingBack || goingFront)
		{
			LastKnownVelocity.x = Added.x * 80.0f;
			LastKnownVelocity.y = Added.y * 80.0f;
		}
	}


	Tarrow = TimeelapsedsinceUpdate / Ttriangle;
	VelocityBlend.x = static_cast<float>(OldVelocity.x + (LastKnownVelocity.x - OldVelocity.x) * Tarrow);
	VelocityBlend.y = static_cast<float>(OldVelocity.y + (LastKnownVelocity.y - OldVelocity.y) * Tarrow);
	Pt.x = static_cast<float>(OldPosition.x + (VelocityBlend.x * TimeelapsedsinceUpdate));
	Pt.y = static_cast<float>(OldPosition.y + (VelocityBlend.y * TimeelapsedsinceUpdate));
	PtPrime.x = static_cast<float>(LastKnownPosition.x + (LastKnownVelocity.x * TimeelapsedsinceUpdate));
	PtPrime.y = static_cast<float>(LastKnownPosition.y + (LastKnownVelocity.y * TimeelapsedsinceUpdate));

	if (TimeelapsedsinceUpdate > Ttriangle)
	{
		if ((RotLeft || RotRight) && (goingFront || goingBack))
		{
			FinalPosition.x = static_cast<float>(LastKnownPosition.x + (LastKnownVelocity.x * dt));
			FinalPosition.y = static_cast<float>(LastKnownPosition.y + (LastKnownVelocity.y * dt));
		}
		else
		{
			FinalPosition.x = static_cast<float>(LastKnownPosition.x + (LastKnownVelocity.x * TimeelapsedsinceUpdate));
			FinalPosition.y = static_cast<float>(LastKnownPosition.y + (LastKnownVelocity.y * TimeelapsedsinceUpdate));
		}
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
/******************************************************************************/
/*!
\fn void DeadReckoning::Run(AEVec2& UpdatePosition, AEVec2& UpdateVelocity, float& direction, float dt, ShipID id)
\brief determine predict or correction
\param UpdatePosition
\param UpdateVelocity
\param direction
\param dt
\param id
*/
/******************************************************************************/
void DeadReckoning::Run(AEVec2& UpdatePosition, AEVec2& UpdateVelocity, float& direction, float dt, ShipID id)
{
	if (!isInit)
		return;

	if (!extrapolating)
		Predict(UpdatePosition, UpdateVelocity, direction, dt);
	else
		Correction(UpdatePosition, UpdateVelocity, direction, dt, id);
}