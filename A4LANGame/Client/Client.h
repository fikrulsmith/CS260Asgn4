#pragma once
#include "../Client/ClientInfo.h"

class Client
{
	std::vector<ClientInfo> clients;
public:
	const size_t DOES_NOT_EXIST = -1; 


	// run this first
	// check against DOES_NOT_EXIST before trying to get client
	size_t CheckClientExist(SOCKET clientSocket); 
	
	// gets the client info
	ClientInfo GetClient(size_t index);
};