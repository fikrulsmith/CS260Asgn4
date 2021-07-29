#include "pch.h"
#include "ClientSender.h"

int ClientSender::SendClient(ClientInfo info, std::string message)
{
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
