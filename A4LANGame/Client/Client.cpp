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

	std::cout << "Own Address: " << name << std::endl;
	std::cout << "Own Port: " << port << std::endl;

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
	if (InitMyInfo(allClients[0].first, allClients[0].second) != OK) return -1;

	allClients.erase(allClients.begin());

	for (size_t i = 0; i < allClients.size(); i++)
	{
		size_t index = RegisterClient(allClients[i].first, allClients[i].second);
		ConnectToClient(clients[index]);
	}

	return 1;
}

ClientInfo* Client::GetClient(size_t index)
{
	return &clients[index];
}

bool Client::GetClientReadyCheck()
{
	for (auto client : clients)
	{
		if (!client.readyCheck)
			return false;
	}

	return true;
}

/*****************************************************************

					NEEDS TO BE DONE!!!!

******************************************************************/
bool Client::CreatePlayer(SOCKET socket)
{
	// call fik's function to get pointer
	// playerEntity 
	return true;
}

size_t Client::GetClientByGamePtr(GameObjInst* entity)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		
	}

	return -1;
}

size_t Client::GetNumberOfClients()
{
	return clients.size();
}

int Client::SendClient(SOCKET socket, std::string message)
{
	size_t index = CheckClientExist(socket);
	if (index == DOES_NOT_EXIST) return -1;
	
	return sender.SendClient(*GetClient(index), message);
}

int Client::SendClient(int index, std::string message)
{
	return sender.SendClient(*GetClient(index), message);
}

int Client::SendAllClient(std::string message)
{
	for (auto client : clients)
	{
		SendClient(client.socket, message);
	}

	return 1;
}

void Client::UpdateState(ShipID id, ShipState state)
{
	for (auto client : clients)
	{
		if (client.id == id)
		{
			client.state = state;
			std::string message;
			SendAllClient(message);
		}
	}
}

int Client::ReceiveClient(SOCKET socket,std::string& message)
{
	size_t index = CheckClientExist(socket);
	if (index == DOES_NOT_EXIST) return -1;

	return receiver.RecvClient(*GetClient(index), message);
}
int Client::ReceiveAllClient()
{
	for (auto client : clients)
	{
		std::string input;
		ReceiveClient(client.socket, input);
		HandleRecvMessage(client.socket, input);
	}

	return 1;
}
size_t Client::RegisterClient(std::string name, std::string port)
{
	ClientInfo client;
	client.name = name;
	client.port = port;

	clients.push_back(client);
	return clients.size() - 1;
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
	
	std::cout << "CLIENT: " << std::endl;
	std::cout << client.name << std::endl;
	std::cout << client.port << std::endl;

	std::cout << "Successfully stored client address." << std::endl;

	return 200;
}
void Client::createDeadReckoning(ShipID id)
{
	IdtoDeadReckoning.insert(std::make_pair(id, DeadReckoning{}));
}

void Client::UpdateAllDeadReckoningDT()
{
	for (auto client : clients)
	{
		IdtoDeadReckoning[client.id].UpdateTime();
	}
}


void Client::HandleRecvMessage(SOCKET client,std::string message)
{
	std::vector<std::string> params;
	std::string header;
	params = Parser::GetHeader(message, header);
	if (header == "[UPDATE]")
	{
		//check ID against your container of players
		int playerID = std::stoi(params[0]);
		AEVec2 Position;
		AEVec2 Velocity;
		AEVec2 Acceleration;
		float direction;
		Position.x = std::stof(params[1]);
		Position.y = std::stof(params[2]);
		Velocity.x = std::stof(params[3]);
		Velocity.y = std::stof(params[4]);
		Acceleration.x = std::stof(params[5]);
		Acceleration.y = std::stof(params[6]);
		direction = std::stof(params[7]);
		//players[playerID].deadreckoning.ReceivedPacket(Position, Velocity, Acceleration);
		// add your stuff here nico


	}
	else if (header == "[READY]")
	{
		size_t index = CheckClientExist(client);
		if (index == DOES_NOT_EXIST) return;

		ClientInfo* info = GetClient(index);
		info->readyCheck = true;

	}
}