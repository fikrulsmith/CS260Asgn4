#pragma once

class Parser
{
public:
	static bool GetAllPairsOfHostnameAndPorts(std::string input, 
		std::vector<std::pair<std::string, std::string>>& vec);

// PACKETS
	static std::string CreatePacket(std::string header, std::string payload);
	static std::string CreateHeader(std::string headerCommand, std::string name, std::string port, std::vector<std::string> params);

	static std::vector<std::string> GetHeader(std::string input, std::string& headerCommand);
	static std::string GetPacket(std::string input, std::string& headerCommand);
	static std::vector<std::string> GetPayload(std::string input);

private:
	static bool ParseInput(std::string ip, std::string& hostname, std::string& port);
	static void RemoveExtraDelim(std::string& str, const char delim);
	static bool IsValidIP(const std::string& str);
	static std::vector<std::string> Tokenize(std::string const& str, const char delim);
};