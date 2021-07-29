#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Windows.h"		// Entire Win32 API...
// #include "winsock2.h"	// ...or Winsock alone
#include "ws2tcpip.h"		// getaddrinfo()

#include <string>
#include <iostream>
#include "../ClientInfo.h"

class ClientSender
{
public:
	/*int SendClient(ClientInfo info, std::string message);*/
};