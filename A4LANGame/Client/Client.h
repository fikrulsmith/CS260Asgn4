#pragma once
#include "../Client/ClientInfo.h"
#include "ClientSender/ClientSender.h"
#include "ClientReceiver/ClientReceiver.h"
class Client
{
	std::vector<ClientInfo> clients;
	ClientSender sender;
	ClientReceiver receiver;
public:
	const size_t DOES_NOT_EXIST = -1; 

	bool RegisterClient(ClientInfo client);
	bool DisconnectClient(SOCKET clientSocket);

	// run this first
	// check against DOES_NOT_EXIST before trying to get client
	size_t CheckClientExist(SOCKET clientSocket); 
	void SetupSocket(SOCKET& setupSocket, sockaddr Address);
	// gets the client info
	ClientInfo GetClient(size_t index);
};