#include "pch.h"
#include "Global.h"
#include "LockStep.h"

std::string LockStep::HashInput(std::string input)
{
	return hash.HashString(input);
}

bool LockStep::CompareInput(std::string input, std::string hashInput)
{
	std::cout << "CHECK" << std::endl;
	std::cout << hash.HashString(input) << std::endl;
	std::cout << hashInput << std::endl;
	return hash.HashString(input) == hashInput;
}