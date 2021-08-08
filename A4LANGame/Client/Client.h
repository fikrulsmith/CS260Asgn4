#pragma once
#include "ClientSender/ClientSender.h"
#include "ClientReceiver/ClientReceiver.h"
#include "LockStep/LockStep.h"

class Client
{
	std::vector<ClientInfo> clients;
	ClientSender sender;
	ClientReceiver receiver;
	ClientInfo MyInfo;

	LockStep lockStepManager;

	int InitWSA();
	int ConnectToClient(ClientInfo& client);
public:
	const size_t DOES_NOT_EXIST = -1; 
	const int OK = 200;

	~Client();

	// Initialise Client
	int InitMyInfo(std::string name, std::string port);
	int InitialiseClient(std::vector<std::pair<std::string, std::string>> allClients);

	// Register/Disconnect Clients
	size_t RegisterClient(std::string name, std::string port);
	bool DisconnectClient(SOCKET clientSocket);

	// run this first
	// check against DOES_NOT_EXIST before trying to get client
	size_t CheckClientExist(SOCKET clientSocket); 

	// gets the client info
	ClientInfo* GetClient(size_t index);

	bool CreatePlayer(SOCKET socket);
	size_t GetClientByGamePtr(GameObjInst* entity);
	

	int SendClient(SOCKET socket, std::string message);
	int SendClient(int index, std::string message);
	int SendAllClient(std::string message);

	int ReceiveClient(std::string message);

	void UpdateState(ShipID id, ShipState state);

	std::string GetOwnPort()
	{
		return MyInfo.port;
	}
	
	void HandleRecvMessage(std::string message);
};