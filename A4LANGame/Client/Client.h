#pragma once
#include "../Client/ClientInfo.h"
#include "ClientSender/ClientSender.h"
#include "ClientReceiver/ClientReceiver.h"
class Client
{
	std::vector<ClientInfo> clients;
	ClientSender sender;
	ClientReceiver receiver;
	SOCKET MySocket;
public:
	Client() = delete;
	Client(std::string name, std::string port);
	~Client();
	const size_t DOES_NOT_EXIST = -1; 

	bool RegisterClient(ClientInfo client);
	bool DisconnectClient(SOCKET clientSocket);

	// run this first
	// check against DOES_NOT_EXIST before trying to get client
	size_t CheckClientExist(SOCKET clientSocket); 
	void SetupSocket(SOCKET& setupSocket, std::string port);
	// gets the client info
	ClientInfo GetClient(size_t index);
};