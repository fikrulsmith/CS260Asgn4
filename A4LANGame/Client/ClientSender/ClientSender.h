#pragma once

class ClientSender
{
public:
	int SendClient(ClientInfo info, std::string message);
	int RecvClient(ClientInfo info, std::string& message);
};