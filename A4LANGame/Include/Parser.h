#pragma once

class Parser
{
public:
	static bool ParseInput(std::string ip, std::string& hostname, std::string& port);
	static bool GetHostnameToIPMap(std::string input, std::unordered_map<std::string, std::string>& map);
private:
	static void RemoveExtraDelim(std::string& str, const char delim);
	static bool IsValidIP(const std::string& str);
	static std::vector<std::string> Tokenize(std::string const& str, const char delim);
};