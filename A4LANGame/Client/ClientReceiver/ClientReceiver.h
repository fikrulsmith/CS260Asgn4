#pragma once
#include <thread>
class ClientReceiver
{
public:
	int SendClient(ClientInfo info, std::string message);
	int RecvClient(ClientInfo info, std::string& message);
};