/******************************************************************************/
/*!
\file Client.h
\author Wong Swee Jong Nico
\par email: s.wong\@digipen.edu
\par DigiPen login: s.wong
\par Course: CS260-B
\par Assignment #04
\date 10/08/2021
\brief
This file contains an implementation of the functionality of a client
*/
/******************************************************************************/

#pragma once
#include "ClientSender/ClientSender.h"
#include "ClientReceiver/ClientReceiver.h"
#include "LockStep/LockStep.h"
#include "DeadReckoning/DeadReckoning.h"
class Client
{
	std::vector<ClientInfo> clients;

	addrinfo hints{};

	ClientSender sender;
	ClientReceiver receiver;
	ClientInfo MyInfo;
	SOCKET MySocket;
	LockStep lockStepManager;

	int InitWSA();
	int InitialiseClientMember(ClientInfo& client);

	std::string PackOwnData();
public:
	bool lock = false;
	const size_t DOES_NOT_EXIST = static_cast<size_t>(-1); 
	const int OK = 200;
	std::unordered_map<ShipID, DeadReckoning> IdtoDeadReckoning;
	~Client();

	// Initialise Client
	int InitMyInfo(std::string name, std::string port);
	int InitialiseClient(std::vector<std::pair<std::string, std::string>> allClients);

	// Register/Disconnect Clients
	size_t RegisterClient(std::string name, std::string port);

	// run this first
	// check against DOES_NOT_EXIST before trying to get client
	ClientInfo* GetClientByName(std::string name, std::string port);

	// gets the client info
	ClientInfo* GetClient(size_t index);
	bool GetClientReadyCheck();
	ClientInfo* GetOwnInfo();

	size_t GetClientByID(ShipID entity);
	size_t GetNumberOfClients();

	int SendClient(sockaddr* sock, std::string message);
	int SendAllClient(std::string message);

	int ReceiveClient(std::string& message);
	void UpdateState();
	void RecvUpdateState(ClientInfo* info, std::string hash);
	std::vector<std::string> PackData(ShipID id);

	void createDeadReckoning(ShipID id);
	void UpdateAllDeadReckoningDT(float dt);
	void UpdateDeadReckoning(ShipID id, AEVec2 Position, AEVec2 Velocity, AEVec2 Acceleration, float direction);
	void AllDeadReckoningCorrection(float dt);

	void HandleRecvMessage(std::string message);
	bool HandleLockStepMessage(std::pair<ShipID, std::string>& pair, std::string message);
};