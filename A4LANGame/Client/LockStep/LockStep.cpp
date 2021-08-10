/******************************************************************************/
/*!
\file LockStep.cpp
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
#include "pch.h"
#include "Global.h"
#include "LockStep.h"

/******************************************************************************/
/*!
\brief Hashes the input

\param input

\return
	Returns the hash string
*/
/******************************************************************************/
std::string LockStep::HashInput(std::string input)
{
	return hash.HashString(input);
}

/******************************************************************************/
/*!
\brief Compares the string with a hash input

\param input

\param hashInput

\return
	Returns if it is the same
*/
/******************************************************************************/
bool LockStep::CompareInput(std::string input, std::string hashInput)
{
	return hash.HashString(input) == hashInput;
}