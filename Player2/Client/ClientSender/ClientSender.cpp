#include "pch.h"
#include "Global.h"
#include "ClientSender.h"

int ClientSender::SendClient(ClientInfo& info, std::string message)
{
	std::cout << "SEND to " << info.port << ": " << message << std::endl;
	int bytesSend = sendto(info.socket, message.c_str(),
		static_cast<int>(message.length()), 0, info.addr->ai_addr, sizeof(*info.addr->ai_addr));

	if (bytesSend == SOCKET_ERROR)
	{
		size_t errorCode = WSAGetLastError();
		std::cout << errorCode << std::endl;
		std::cerr << "send failed" << std::endl;
		return bytesSend;
	}

	return bytesSend;
}

int ClientSender::RecvClient(ClientInfo& info, std::string& message)
{
	const size_t BUFFER_SIZE = 10000;
	char buffer[BUFFER_SIZE];

	int serverAddressSize = sizeof(*info.addr->ai_addr);
	const int bytesReceived = recvfrom(info.socket, buffer, BUFFER_SIZE - 1, 0, info.addr->ai_addr, &serverAddressSize);

	if (bytesReceived == SOCKET_ERROR)
	{
		std::cerr << "Failed to receive!" << std::endl;
		return SOCKET_ERROR;
	}

	if (bytesReceived == 0)
	{
		std::cerr << "Server has disconnected!" << std::endl;
		return 0;
	}

	buffer[bytesReceived] = '\0';
	message.clear();
	message.append(buffer, bytesReceived);

	return bytesReceived;
}

int ClientSender::SendCommand(ClientInfo& info, std::string command)
{
	return SendClient(info, command);
}

int ClientSender::SendPacket(ClientInfo& info, std::string packet)
{
	return SendClient(info, packet);
}
