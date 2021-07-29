#pragma once

class ClientSender
{
public:
	int SendClient(ClientInfo& info, std::string message);
	int RecvClient(ClientInfo& info, std::string& message);

	int SendCommand(ClientInfo& info, std::string command);
	int SendPacket(ClientInfo& info, std::string packet);
};