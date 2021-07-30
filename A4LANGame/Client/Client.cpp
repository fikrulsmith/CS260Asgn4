#include "pch.h"
#include "Global.h"
#include "Client.h"

int Client::InitWSA()
{
	WSADATA wsaData{};
	SecureZeroMemory(&wsaData, sizeof(wsaData));

	// Initialize Winsock. You must call WSACleanup when you are finished.
	// As this function uses a reference counter, for each call to WSAStartup,
	// you must call WSACleanup or suffer memory issues.
	int errorCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (errorCode != NO_ERROR)
	{
		std::cerr << "WSAStartup() failed." << std::endl;
		return errorCode;
	}

	std::cout
		<< "Winsock version: "
		<< static_cast<int>(LOBYTE(wsaData.wVersion))
		<< "."
		<< static_cast<int>(HIBYTE(wsaData.wVersion))
		<< "\n"
		<< std::endl;

	return OK;
}

int Client::InitMyInfo(std::string name, std::string port)
{
	std::cout << "Initialising Own Info" << std::endl;
	MyInfo.name = name;
	MyInfo.port = port;

	addrinfo hints{};
	SecureZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;			// IPv4
	// For TCP use SOCK_STREAM instead of SOCK_DGRAM.
	hints.ai_socktype = SOCK_DGRAM;		// Best effort
	// Could be 0 for autodetect, but best effort over IPv4 is always UDP.
	hints.ai_protocol = IPPROTO_UDP;	// UDP

	int errorCode = getaddrinfo(nullptr, port.c_str(), &hints, &MyInfo.addr);
	if ((errorCode) || (MyInfo.addr == nullptr))
	{
		std::cerr << "getaddrinfo() failed." << std::endl;
		return errorCode;
	}
	std::cout << "Successful.\n" << std::endl;
	return OK;
}

Client::~Client()
{
	if (MyInfo.addr)
		freeaddrinfo(MyInfo.addr);

	for (auto client : clients)
	{
		if (client.addr)
			freeaddrinfo(client.addr);
	}

	WSACleanup();
}

int Client::InitialiseClient(std::vector<std::pair<std::string, std::string>> allClients)
{
	if (InitWSA() != OK) return -1;

	for (size_t i = 0; i < allClients.size(); i++)
	{
		
	}

	return 1;
}

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
	// maybe donnid to erase the client
	// jus add a boolean to say it's not active can already
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

int Client::ConnectToClient(ClientInfo& client)
{
	std::cout << "Attempting UDP Connection with Client" << std::endl;

	const std::string serverPortString = client.port;
	const std::string clientPortString = MyInfo.port;

	// -------------------------------------------------------------------------
	// Resolve own host name into IP addresses (in a singly-linked list).
	//
	// getaddrinfo()
	// -------------------------------------------------------------------------

	// Object hints indicates which protocols to use to fill in the info.
	addrinfo hints{};
	SecureZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;			// IPv4
	// For TCP use SOCK_STREAM instead of SOCK_DGRAM.
	hints.ai_socktype = SOCK_DGRAM;		// Best effort
	// Could be 0 for autodetect, but best effort over IPv4 is always UDP.
	hints.ai_protocol = IPPROTO_UDP;	// UDP

	addrinfo* info = nullptr;
	int errorCode = getaddrinfo(nullptr, clientPortString.c_str(), &hints, &info);
	if ((errorCode) || (info == nullptr))
	{
		std::cerr << "getaddrinfo() failed." << std::endl;
		return errorCode;
	}


	// -------------------------------------------------------------------------
	// Create a socket and bind it to own network interface controller.
	//
	// socket()
	// bind()
	// -------------------------------------------------------------------------

	SOCKET serverSocket = socket(
		hints.ai_family,
		hints.ai_socktype,
		hints.ai_protocol);
	if (serverSocket == INVALID_SOCKET)
	{
		std::cerr << "socket() failed." << std::endl;
		return 1;
	}

	errorCode = bind(
		serverSocket,
		info->ai_addr,
		static_cast<int>(info->ai_addrlen));
	if (errorCode != NO_ERROR)
	{
		std::cerr << "bind() failed." << std::endl;
		closesocket(serverSocket);
		freeaddrinfo(info);
		return 2;
	}

	freeaddrinfo(info);

	addrinfo* serverInfo = nullptr;
	errorCode = getaddrinfo(client.name.c_str(), serverPortString.c_str(), &hints, &serverInfo);
	if ((errorCode) || (serverInfo == nullptr))
	{
		std::cerr << "getaddrinfo() failed." << std::endl;
		return errorCode;
	}

	sockaddr serverAddress{};
	serverAddress = *serverInfo->ai_addr;

	client.addr = serverInfo;
	client.socket = serverSocket;

	std::cout << "Successfully connected to Client." << std::endl;

	return 200;
}