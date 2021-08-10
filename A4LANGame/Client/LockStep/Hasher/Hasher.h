/******************************************************************************/
/*!
\file Hasher.h
\author Wong Swee Jong Nico
\par email: s.wong\@digipen.edu
\par DigiPen login: s.wong
\par Course: CS260-B
\par Assignment #04
\date 10/08/2021
\brief
This file contains an implementation of the functionality of hashing
*/
/******************************************************************************/
#pragma once

#include "../../Dep/PicoSHA2/picosha2.h"

class Hasher
{
public:
	std::string HashString(std::string input);
};