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
