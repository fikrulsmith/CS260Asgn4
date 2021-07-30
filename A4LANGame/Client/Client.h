#pragma once
#include "ClientSender/ClientSender.h"
#include "ClientReceiver/ClientReceiver.h"

class Client
{
	std::vector<ClientInfo> clients;
	ClientSender sender;
	ClientReceiver receiver;
	ClientInfo MyInfo;

	int InitWSA();
public:
	const size_t DOES_NOT_EXIST = -1; 
	const int OK = 200;

	~Client();

	int InitMyInfo(std::string name, std::string port);
	int InitialiseClient(std::vector<std::pair<std::string, std::string>> allClients);

	size_t RegisterClient(std::string name, std::string port);
	bool DisconnectClient(SOCKET clientSocket);

	// run this first
	// check against DOES_NOT_EXIST before trying to get client
	size_t CheckClientExist(SOCKET clientSocket); 
	
	int ConnectToClient(ClientInfo& client);
	// gets the client info
	ClientInfo GetClient(size_t index);
};