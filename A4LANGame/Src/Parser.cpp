#include "pch.h"
#include "Parser.h"

bool Parser::ParseInput(std::string ip, std::string& hostname, std::string& port)
{
	if (!IsValidIP(ip))
		return false;

	std::vector<std::string> tokens = Tokenize(ip, ':');
	hostname = tokens[0];
	port = tokens[1];

	return true;
}

bool Parser::GetHostnameToIPMap(std::string input, std::unordered_map<std::string, std::string>& map)
{
	RemoveExtraDelim(input, ' ');
	std::vector<std::string> tokens = Tokenize(input, ' ');
	std::string hostname;
	std::string port;

	for (auto& str : tokens)
	{
		if (!ParseInput(str, hostname, port))
			return false;

		map[hostname] = str;
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
