#pragma once

#include "../../Dep/PicoSHA2/picosha2.h"

class Hasher
{
public:
	std::string HashString(std::string input);
};