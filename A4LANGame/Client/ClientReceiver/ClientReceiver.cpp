#include "pch.h"
#include "ClientReceiver.h"

int ClientReceiver::SendClient(ClientInfo info, std::string message)
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

int ClientReceiver::RecvClient(ClientInfo info, std::string& message)
{
	const size_t BUFFER_SIZE = 10000;
	char buffer[BUFFER_SIZE];

	int serverAddressSize = sizeof(*info.addr->ai_addr);
	const int bytesReceived = recvfrom(info.socket, buffer, BUFFER_SIZE - 1, 0, info.addr->ai_addr, &serverAddressSize);

	if (bytesReceived == SOCKET_ERROR)
	{
		size_t errorCode = WSAGetLastError();
		if (errorCode == WSAEWOULDBLOCK)
		{
			// A non-blocking call returned no data; sleep and try again.
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(200ms);
			//std::cerr << "trying again..." << std::endl;
			return;
		}
		else
		{
			std::cerr << "recv()from failed." << std::endl;
		}
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

