/******************************************************************************/
/*!
\file Hasher.cpp
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
#include "pch.h"
#include "Global.h"
#include "Hasher.h"

std::string Hasher::HashString(std::string input)
{
	std::vector<unsigned char> hash(picosha2::k_digest_size);
	picosha2::hash256(input.begin(), input.end(), hash.begin(), hash.end());

	std::string hex_str = picosha2::bytes_to_hex_string(hash.begin(), hash.end());

	return hex_str;
}
