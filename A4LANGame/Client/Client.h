#pragma once
#include "../Client/ClientInfo.h"
#include "ClientSender/ClientSender.h"

class Client
{
	std::vector<ClientInfo> clients;
	ClientSender sender;
public:
	const size_t DOES_NOT_EXIST = -1; 

	bool RegisterClient(ClientInfo client);
	bool DisconnectClient(SOCKET clientSocket);

	// run this first
	// check against DOES_NOT_EXIST before trying to get client
	size_t CheckClientExist(SOCKET clientSocket); 
	
	// gets the client info
	ClientInfo GetClient(size_t index);
};