#pragma once
#include <thread>

class ClientReceiver
{
public:
	int RecvFromSocket(SOCKET serverSocket, std::string& message);
};