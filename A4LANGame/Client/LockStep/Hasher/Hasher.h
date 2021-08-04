#pragma once

#include "../../Dep/PicoSHA2/picosha2.h"

class Hasher
{
	std::vector<std::string> hashInput;
public:
	std::string HashString(std::string input);
};