#pragma once
#include "Hasher/Hasher.h"

class LockStep
{
	Hasher hash;
public:
	std::string HashInput(std::string input);
	bool CompareInput(std::string input, std::string hashInput);
};