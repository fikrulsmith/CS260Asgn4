/******************************************************************************/
/*!
\file ClientReceiver.h
\author Ong Guan Hin
\par email: guanhin.ong\@digipen.edu
\par DigiPen login: guanhin.ong
\par Course: CS260-B
\par Assignment #04
\date 10/08/2021
\brief
This file contains an implementation receiving for client
*/
/******************************************************************************/
#pragma once
#include <thread>

class ClientReceiver
{
public:
	int RecvFromSocket(SOCKET serverSocket, std::string& message);
};