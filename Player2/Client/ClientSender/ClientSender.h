#pragma once

class ClientSender
{
public:
	int SendClient(SOCKET serverSocket, sockaddr* sock, std::string message);

	int SendCommand(SOCKET serverSocket, ClientInfo& info, std::string command);
	int SendPacket(SOCKET serverSocket, ClientInfo& info, std::string packet);
};