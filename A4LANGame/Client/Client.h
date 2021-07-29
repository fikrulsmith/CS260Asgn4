#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Windows.h"		// Entire Win32 API...
// #include "winsock2.h"	// ...or Winsock alone
#include "ws2tcpip.h"		// getaddrinfo()

#include <vector>

#include "ClientInfo.h"

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