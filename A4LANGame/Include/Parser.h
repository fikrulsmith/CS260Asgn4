#pragma once

class Parser
{
public:
	static bool GetAllPairsOfHostnameAndPorts(std::string input, 
		std::vector<std::pair<std::string, std::string>>& vec);
private:
	static bool ParseInput(std::string ip, std::string& hostname, std::string& port);
	static void RemoveExtraDelim(std::string& str, const char delim);
	static bool IsValidIP(const std::string& str);
	static std::vector<std::string> Tokenize(std::string const& str, const char delim);
};