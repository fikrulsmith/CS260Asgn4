/******************************************************************************/
/*!
\file ClientSender.cpp
\author Wong Swee Jong Nico
\par email: s.wong\@digipen.edu
\par DigiPen login: s.wong
\par Course: CS260-B
\par Assignment #04
\date 10/08/2021
\brief
This file contains an implementation of the functionality of a sender
*/
/******************************************************************************/
#include "pch.h"
#include "Global.h"
#include "ClientSender.h"

/******************************************************************************/
/*!
\brief Sends the client the message

\param serverSocket
\param sock
\param message

\return 
	Returns the number of bytes send or error code
*/
/******************************************************************************/
int ClientSender::SendClient(SOCKET serverSocket, sockaddr* sock, std::string message)
{
	int bytesSend = sendto(serverSocket, message.c_str(),
		static_cast<int>(message.length()), 0, sock, sizeof(*sock));

	if (bytesSend == SOCKET_ERROR)
	{
		size_t errorCode = WSAGetLastError();
		std::cout << errorCode << std::endl;
		std::cerr << "send failed" << std::endl;
		return bytesSend;
	}

	std::cout << "SENDING: \n" << message << std::endl;
	std::cout << std::endl;
	return bytesSend;
}
