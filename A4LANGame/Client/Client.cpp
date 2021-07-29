#include "pch.h"
#include "Client.h"

ClientInfo Client::GetClient(size_t index)
{
	return clients[index];
}

bool Client::RegisterClient(ClientInfo client)
{
	clients.push_back(client);
	return true;
}

bool Client::DisconnectClient(SOCKET clientSocket)
{
	for (auto it = clients.begin(); it != clients.end(); it++)
	{
		if (it->socket == clientSocket)
		{
			clients.erase(it);
			return true;
		}
	}

	return false;
}

size_t Client::CheckClientExist(SOCKET clientSocket)
{
	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i].socket == clientSocket)
			return i;
	}

	return DOES_NOT_EXIST;
}
