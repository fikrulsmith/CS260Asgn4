#include "pch.h"
#include "Client.h"

ClientInfo Client::GetClient(size_t index)
{
	return clients[index];
}

bool Client::RegisterClient(ClientInfo client)
{
	clients.push_back(client);
	return true;
}

bool Client::DisconnectClient(SOCKET clientSocket)
{
	for (auto it = clients.begin(); it != clients.end(); it++)
	{
		if (it->socket == clientSocket)
		{
			clients.erase(it);
			return true;
		}
	}

	return false;
}

size_t Client::CheckClientExist(SOCKET clientSocket)
{
	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i].socket == clientSocket)
			return i;
	}

	return DOES_NOT_EXIST;
}

void Client::SetupSocket(SOCKET& setupSocket,std::string port)
{
	// Object hints indicates which protocols to use to fill in the info.
	addrinfo hints{};
	SecureZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;			// IPv4
	// For TCP use SOCK_STREAM instead of SOCK_DGRAM.
	hints.ai_socktype = SOCK_DGRAM;		// Best effort
	// Could be 0 for autodetect, but best effort over IPv4 is always UDP.
	hints.ai_protocol = IPPROTO_UDP;	// UDP

	addrinfo* info = nullptr;
	int errorCode = getaddrinfo(nullptr, port.c_str(), &hints, &info);
	if ((errorCode) || (info == nullptr))
	{
		std::cerr << "getaddrinfo() failed." << std::endl;
	}


	// -------------------------------------------------------------------------
	// Create a socket and bind it to own network interface controller.
	//
	// socket()
	// bind()
	// -------------------------------------------------------------------------

	setupSocket = socket(
		hints.ai_family,
		hints.ai_socktype,
		hints.ai_protocol);
	if (setupSocket == INVALID_SOCKET)
	{
		std::cerr << "socket() failed." << std::endl;
		freeaddrinfo(info);
	}

	errorCode = bind(
		setupSocket,
		info->ai_addr,
		static_cast<int>(info->ai_addrlen));
	if (errorCode != NO_ERROR)
	{
		std::cerr << "bind() failed." << std::endl;
		closesocket(setupSocket);
		freeaddrinfo(info);
	}

	freeaddrinfo(info);
}