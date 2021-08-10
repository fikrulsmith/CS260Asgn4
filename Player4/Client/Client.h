#pragma once
#include "ClientSender/ClientSender.h"
#include "ClientReceiver/ClientReceiver.h"
#include "LockStep/LockStep.h"
#include "DeadReckoning/DeadReckoning.h"
class Client
{
	std::vector<ClientInfo> clients;


	ClientSender sender;
	ClientReceiver receiver;
	ClientInfo MyInfo;
	LockStep lockStepManager;

	int InitWSA();
	int ConnectToClient(ClientInfo& client);

	void UpdateHash();
	bool CheckAllHash();
	bool AllHashUpdated();
	bool AllLocked();
	void ResetHash();

public:
	const size_t DOES_NOT_EXIST = -1;
	const int OK = 200;
	std::unordered_map<ShipID, DeadReckoning> IdtoDeadReckoning;
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
	ClientInfo* GetClientByName(std::string name, std::string port);

	// gets the client info
	ClientInfo* GetClient(size_t index);
	bool GetClientReadyCheck();
	ClientInfo* GetOwnInfo();

	bool CreatePlayer(SOCKET socket);
	size_t GetClientByID(ShipID entity);
	size_t GetNumberOfClients();

	int SendClient(SOCKET socket, std::string message);
	int SendClient(int index, std::string message);
	int SendAllClient(std::string message);

	int ReceiveClient(SOCKET socket, std::string& message);
	int ReceiveAllClient();
	void UpdateState(ShipState state);
	std::vector<std::string> PackData(ShipID id, GameObjInst* obj);

	void createDeadReckoning(ShipID id);
	void UpdateAllDeadReckoningDT(float dt);
	void UpdateDeadReckoning(ShipID id, AEVec2 Position, AEVec2 Velocity, AEVec2 Acceleration, float direction, float dt);
	void AllDeadReckoningCorrection(float dt);

	std::string GetOwnPort()
	{
		return MyInfo.port;
	}

	void HandleRecvMessage(SOCKET client, std::string message);
	void SendUpdatePacket(ShipID id);
};