/******************************************************************************/
/*!
\file LockStep.h
\author Wong Swee Jong Nico
\par email: s.wong\@digipen.edu
\par DigiPen login: s.wong
\par Course: CS260-B
\par Assignment #04
\date 10/08/2021
\brief
This file contains an implementation of the functionality of lockstep
*/
/******************************************************************************/
#pragma once
#include "Hasher/Hasher.h"

class LockStep
{
	Hasher hash;
public:
	std::string HashInput(std::string input);
	bool CompareInput(std::string input, std::string hashInput);
};