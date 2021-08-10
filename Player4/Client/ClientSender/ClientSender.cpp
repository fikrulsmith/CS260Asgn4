#include "pch.h"
#include "Global.h"
#include "ClientSender.h"

int ClientSender::SendClient(SOCKET serverSocket, sockaddr* sock, std::string message)
{
	std::cout << "SENDING: \n" << message << std::endl;
	int bytesSend = sendto(serverSocket, message.c_str(),
		static_cast<int>(message.length()), 0, sock, sizeof(*sock));

	if (bytesSend == SOCKET_ERROR)
	{
		size_t errorCode = WSAGetLastError();
		std::cout << errorCode << std::endl;
		std::cerr << "send failed" << std::endl;
		return bytesSend;
	}

	std::cout << bytesSend << std::endl;
	return bytesSend;
}

int ClientSender::SendCommand(SOCKET serverSocket, ClientInfo& info, std::string command)
{
	return SendClient(serverSocket, &info.sock, command);
}

int ClientSender::SendPacket(SOCKET serverSocket, ClientInfo& info, std::string packet)
{
	return SendClient(serverSocket, &info.sock, packet);
}
