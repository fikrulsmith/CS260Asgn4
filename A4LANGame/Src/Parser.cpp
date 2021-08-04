#include "pch.h"
#include "Parser.h"

/*
HEADER (includes the \n\n)

PAYLOAD
*/
std::string Parser::CreatePacket(std::string header, std::string payload)
{
	std::string packet = header + payload;
	return packet;
}

/*
HEADER COMMAND\n
\n
PARAM_1\n
PARAM_2\n
PARAM_3\n
\n
*/
std::string Parser::CreateHeader(std::string headerCommand, std::vector<std::string> params)
{
	std::string header;
	
	header = headerCommand;
	header += "\n\n";
	for (auto& param : params)
	{
		header += param;
		header += "\n";
	}
	header += "\n";
	return header;
}

std::vector<std::string> Parser::GetHeader(std::string input, std::string& headerCommand)
{
	size_t index = input.find("\n\n");
	headerCommand = input.substr(0, index);
	input = input.substr(index + 2);

	std::vector<std::string> params;
	while (true)
	{
		index = input.find("\n");
		size_t stopper = input.find("\n\n");

		params.push_back(input.substr(0, index));
		if (index == stopper)
			break;
		else
			input = input.substr(index + 1);
	}

	return params;
}

bool Parser::ParseInput(std::string ip, std::string& hostname, std::string& port)
{
	std::vector<std::string> tokens = Tokenize(ip, ':');

	if (tokens[0] != "localhost")
		if (!IsValidIP(ip))
			return false;

	hostname = tokens[0];
	port = tokens[1];

	return true;
}

bool Parser::GetAllPairsOfHostnameAndPorts(std::string input, 
	std::vector<std::pair<std::string, std::string>>& vec)
{
	RemoveExtraDelim(input, ' ');
	std::vector<std::string> tokens = Tokenize(input, ' ');
	std::string hostname;
	std::string port;

	for (auto& str : tokens)
	{
		if (!ParseInput(str, hostname, port))
			return false;

		vec.push_back(std::make_pair(hostname, port));
	}

	return true;
}

void Parser::RemoveExtraDelim(std::string& str, const char delim)
{
	// n is length of the original string 
	int n = static_cast<int>(str.length());
	int i = 0, j = -1;
	bool delimsFound = false;

	// Handles leading delimiters 
	while (++j < n && str[j] == delim);

	// read all characters of original string 
	while (j < n)
	{
		// if current characters is not the specified delimiter
		if (str[j] != delim)
		{
			// remove preceding delimiters before the punctuations
			if ((str[j] == '.' || str[j] == ',' ||
				str[j] == '?') && i - 1 >= 0 &&
				str[static_cast<size_t>(i) - 1] == delim)
				str[static_cast<size_t>(i) - 1] = str[j++];
			else
				str[i++] = str[j++];

			delimsFound = false;
		}
		// if current character is the specified delimiter
		else if (str[j++] == delim)
		{
			if (!delimsFound)
			{
				str[i++] = delim;
				delimsFound = true;
			}
		}
	}
	// Remove trailing delimiters 
	if (i <= 1)
		str.erase(str.begin() + i, str.end());
	else
		str.erase(str.begin() + i - 1, str.end() - 1);
}

bool Parser::IsValidIP(const std::string& str)
{
	int index = 0;

	while (index < str.size())
	{
		if (isxdigit(str[index]) == 0)
		{
			if (ispunct(str[index]) == 0)
			{
				return false;
			}
		}

		index++;
	}

	return true;
}

std::vector<std::string> Parser::Tokenize(std::string const& str, const char delim)
{
    std::vector<std::string> out;
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }

    return out;
}
