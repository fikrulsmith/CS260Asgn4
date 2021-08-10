/******************************************************************************/
/*!
\file ClientSender.h
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
#pragma once

class ClientSender
{
public:
	int SendClient(SOCKET serverSocket, sockaddr* sock, std::string message);
};