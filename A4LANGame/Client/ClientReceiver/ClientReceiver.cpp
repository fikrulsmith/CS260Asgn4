#include "pch.h"
#include "Global.h"
#include "ClientReceiver.h"

int ClientReceiver::RecvFromSocket(SOCKET serverSocket, std::string& message)
{
	sockaddr clientAddress;
	SecureZeroMemory(&clientAddress, sizeof(clientAddress));

	constexpr size_t BUFFER_SIZE = 500;
	char buffer[BUFFER_SIZE];

	int clientAddressSize = sizeof(clientAddress);

	const int bytesReceived = recvfrom(serverSocket,
		buffer,
		BUFFER_SIZE - 1,
		0,
		&clientAddress,
		&clientAddressSize);

	if (bytesReceived == SOCKET_ERROR)
	{
		size_t errorCode = WSAGetLastError();
		if (errorCode == WSAEWOULDBLOCK)
		{
			return 0;
		}
		else
		{
			std::cerr << "recv()from failed." << std::endl;
			return -1;
		}
	}

	if (bytesReceived == 0)
	{
		std::cerr << "Server has disconnected!" << std::endl;
		return 0;
	}

	if (bytesReceived > BUFFER_SIZE)
		buffer[BUFFER_SIZE - 1] = '\0';
	else
		buffer[bytesReceived] = '\0';
	
	message.clear();
	message.append(buffer, bytesReceived);

	std::string header;
	Parser::GetPacket(message, header);

	if (header == "[LOCK]")
	{
		clientManager->lock = true;
	}

	std::cout << "RECV: \n" << message << std::endl;
	std::cout << bytesReceived << std::endl;

	return bytesReceived;
}


