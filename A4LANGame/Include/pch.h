#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <vector>
#include <iostream>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Windows.h"		// Entire Win32 API...
// #include "winsock2.h"	// ...or Winsock alone
#include "ws2tcpip.h"		// getaddrinfo()

//#include "ClientSender/ClientSender.h"
#include "../Client/ClientInfo.h"

