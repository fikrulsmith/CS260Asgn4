/******************************************************************************/
/*!
\file Parser.cpp
\author Wong Swee Jong Nico
\par email: s.wong\@digipen.edu
\par DigiPen login: s.wong
\par Course: CS260-B
\par Assignment #04
\date 10/08/2021
\brief
This file contains an implementation of a parser for our strings
*/
/******************************************************************************/
#include "pch.h"
#include "Parser.h"

/*
HEADER (includes the \n\n)

PAYLOAD
*/
/******************************************************************************/
/*!
\fn std::string Parser::CreatePacket(std::string headerCommand, std::string payload)
\brief Creates packet
\param headerCommand
\param payload
\return packet
*/
/******************************************************************************/
std::string Parser::CreatePacket(std::string headerCommand, std::string payload)
{
	std::string packet = headerCommand + "\n\n" + payload;
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
/******************************************************************************/
/*!
\fn std::string Parser::CreateHeader(std::string headerCommand, std::string name, std::string port, std::vector<std::string> params)
\brief Creates header
\param headerCommand
\param name
\param port
\param params
\return header
*/
/******************************************************************************/
std::string Parser::CreateHeader(std::string headerCommand, std::string name, std::string port, std::vector<std::string> params)
{
	std::string header;
	
	header = headerCommand;
	header += "\n\n";

	header += name + "\n" + port + "\n";
	for (auto& param : params)
	{
		header += param;
		header += "\n";
	}
	header += "\n";
	return header;
}
/******************************************************************************/
/*!
\fn std::string Parser::CreatePayload(std::string headerCommand, std::string name, std::string port, std::string payload)
\brief Creates payload
\param headerCommand
\param name
\param port
\param payload
\return header
*/
/******************************************************************************/
std::string Parser::CreatePayload(std::string headerCommand, std::string name, std::string port, std::string payload)
{
	std::string header;

	header = headerCommand;
	header += "\n\n";

	header += name + "\n" + port + "\n" + payload + "\n";
	return header;
}
/******************************************************************************/
/*!
\fn std::vector<std::string> Parser::GetHeader(std::string input, std::string& headerCommand)
\brief get header
\param input
\param headerCommand
\return params
*/
/******************************************************************************/
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
/******************************************************************************/
/*!
\fn std::string Parser::GetPacket(std::string input, std::string& headerCommand)
\brief gets packet
\param input
\param headerCommand
\return input
*/
/******************************************************************************/
std::string Parser::GetPacket(std::string input, std::string& headerCommand)
{
	size_t index = input.find("\n\n");
	headerCommand = input.substr(0, index);
	input = input.substr(index + 2);
	
	return input;
}
/******************************************************************************/
/*!
\fn std::vector<std::string> Parser::GetPayload(std::string input)
\brief gets payload
\param input
\return params
*/
/******************************************************************************/
std::vector<std::string> Parser::GetPayload(std::string input)
{
	input += "\n\n";
	std::vector<std::string> params;
	while (true)
	{
		size_t index = input.find("\n");
		size_t stopper = input.find("\n\n");

		params.push_back(input.substr(0, index));
		if (index == stopper)
			break;
		else
			input = input.substr(index + 1);
	}

	return params;
}
/******************************************************************************/
/*!
\fn std::string Parser::RemoveHeader(std::string input, std::string& headerCommand)
\brief removes header
\param input
\param headerCommand
\return input without header
*/
/******************************************************************************/
std::string Parser::RemoveHeader(std::string input, std::string& headerCommand)
{
	size_t index = input.find("\n\n");
	headerCommand = input.substr(0, index);
	return input.substr(index + 2);
}
/******************************************************************************/
/*!
\fn std::string Parser::VectorToString(std::vector<std::string> params)
\brief convert params to a combined string
\param params
\return combined
*/
/******************************************************************************/
std::string Parser::VectorToString(std::vector<std::string> params)
{
	std::string combined;
	for (auto string : params)
	{
		combined += string + "\n";
	}

	return combined;
}
/******************************************************************************/
/*!
\fn bool Parser::ParseInput(std::string ip, std::string& hostname, std::string& port)
\brief Parses input
\param ip
\param hostname
\param port
\return if parser 
*/
/******************************************************************************/
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
/******************************************************************************/
/*!
\fn bool Parser::GetAllPairsOfHostnameAndPorts(std::string input, 
	std::vector<std::pair<std::string, std::string>>& vec)
\brief gets all pairs of hostname and ports
\param input
\param vec
\return if parser passed
*/
/******************************************************************************/
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
/******************************************************************************/
/*!
\fn Parser::RemoveExtraDelim(std::string& str, const char delim)
\brief removes extra delim
\param str
\param delim
*/
/******************************************************************************/
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
/******************************************************************************/
/*!
\fn bool Parser::IsValidIP(const std::string& str)
\brief check if ip is valid
\param str
\return if ip is valid
*/
/******************************************************************************/
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
/******************************************************************************/
/*!
\fn std::vector<std::string> Parser::Tokenize(std::string const& str, const char delim)
\brief tokenizes str with delim
\param str
\param delim
\return tokenized string
*/
/******************************************************************************/
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
