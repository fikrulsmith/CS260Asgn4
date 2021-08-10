#include "pch.h"
#include "Global.h"
#include "Client.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

int Client::InitWSA()
{
	WSADATA wsaData{};
	SecureZeroMemory(&wsaData, sizeof(wsaData));

	// Initialize Winsock. You must call WSACleanup when you are finished.
	// As this function uses a reference counter, for each call to WSAStartup,
	// you must call WSACleanup or suffer memory issues.
	int errorCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (errorCode != NO_ERROR)
	{
		std::cerr << "WSAStartup() failed." << std::endl;
		return errorCode;
	}

	std::cout
		<< "Winsock version: "
		<< static_cast<int>(LOBYTE(wsaData.wVersion))
		<< "."
		<< static_cast<int>(HIBYTE(wsaData.wVersion))
		<< "\n"
		<< std::endl;

	SecureZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;			// IPv4
	// For TCP use SOCK_STREAM instead of SOCK_DGRAM.
	hints.ai_socktype = SOCK_DGRAM;		// Best effort
	// Could be 0 for autodetect, but best effort over IPv4 is always UDP.
	hints.ai_protocol = IPPROTO_UDP;	// UDP

	return OK;
}

int Client::InitMyInfo(std::string name, std::string port)
{
	std::cout << "Initialising Own Info" << std::endl;
	MyInfo.name = name;
	MyInfo.port = port;

	std::cout << "Own Address: " << name << std::endl;
	std::cout << "Own Port: " << port << std::endl;

	int errorCode = getaddrinfo(name.c_str(), port.c_str(), &hints, &MyInfo.addr);
	if ((errorCode) || (MyInfo.addr == nullptr))
	{
		std::cerr << "getaddrinfo() failed." << std::endl;
		return errorCode;
	}

	MySocket = socket(
		hints.ai_family,
		hints.ai_socktype,
		hints.ai_protocol);
	if (MySocket == INVALID_SOCKET)
	{
		std::cerr << "socket() failed." << std::endl;
		return 1;
	}

	/*char enables = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enables, sizeof(int)) < 0)
		std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;*/

	errorCode = bind(
		MySocket,
		MyInfo.addr->ai_addr,
		static_cast<int>(MyInfo.addr->ai_addrlen));
	if (errorCode != NO_ERROR)
	{
		std::cerr << "bind() failed." << std::endl;
		closesocket(MySocket);
		return 2;
	}

	u_long enable = 1;
	ioctlsocket(MySocket, FIONBIO, &enable);

	std::cout << "Successful.\n" << std::endl;
	return OK;
}

Client::~Client()
{
	if (MyInfo.addr)
		freeaddrinfo(MyInfo.addr);

	for (auto client : clients)
	{
		if (client.addr)
			freeaddrinfo(client.addr);
	}

	WSACleanup();
}

int Client::InitialiseClient(std::vector<std::pair<std::string, std::string>> allClients)
{
	if (InitWSA() != OK) return -1;
	if (InitMyInfo(allClients[0].first, allClients[0].second) != OK) return -1;

	allClients.erase(allClients.begin());
	for (size_t i = 0; i < allClients.size(); i++)
	{
		size_t index = RegisterClient(allClients[i].first, allClients[i].second);

		if (InitialiseClientMember(clients[index]) != OK)
		{
			std::cout << "Client could not connect! Exiting Game!" << std::endl;
			GSManager->SetGameStateNextIndex(GS_QUIT);
		}
	}

	for (size_t i = 0; i < clients.size(); i++)
	{
		int id = static_cast<int>(ShipID::PLAYER1);

		for (size_t j = 0; j < clients.size(); j++)
		{
			if (j == i) continue;
			if (std::stoi(clients[i].port) > std::stoi(clients[j].port))
				id++;
		}

		clients[i].id = static_cast<ShipID>(id);
	}

	std::vector<ClientInfo> oldClient = clients;
	std::vector<ClientInfo> newClient = clients;
	clients.clear();

	newClient.push_back(MyInfo);
	std::sort(newClient.begin(), newClient.end(),
		[](const ClientInfo& a, const ClientInfo& b) -> bool
		{
			return std::stoi(a.port) < std::stoi(b.port);
		}
	);

	for (size_t i = 0; i < newClient.size(); i++)
	{
		ShipID ID = static_cast<ShipID>(i);

		if (MyInfo.port == newClient[i].port)
			MyInfo.id = ID;

		newClient[i].id = ID;
	}

	newClient.erase(std::remove_if(newClient.begin(), newClient.end(),
		[this](const ClientInfo& a)
		{
			return a.port == MyInfo.port;
		}),
		newClient.end()
			);

	for (size_t i = 0; i < oldClient.size(); i++)
	{
		for (size_t j = 0; j < newClient.size(); j++)
		{
			if (oldClient[i].port == newClient[j].port)
			{
				clients.push_back(newClient[j]);
				break;
			}
		}
	}

	for (size_t k = 0; k < clients.size(); k++)
	{
		createDeadReckoning(clients[k].id);
	}

	return 1;
}

ClientInfo* Client::GetClient(size_t index)
{
	return &clients[index];
}

bool Client::GetClientReadyCheck()
{
	for (auto client : clients)
	{
		if (!client.readyCheck)
		{
			//std::cout << client.port << " RETURNED FALSE." << std::endl;
			return false;
		}
	}

	if (!MyInfo.readyCheck) return false;

	return true;
}

ClientInfo* Client::GetOwnInfo()
{
	return &MyInfo;
}

/*****************************************************************

					NEEDS TO BE DONE!!!!

******************************************************************/
bool Client::CreatePlayer(SOCKET socket)
{
	// call fik's function to get pointer
	// playerEntity 
	return true;
}

size_t Client::GetClientByID(ShipID entity)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].id == entity)
			return i;
	}

	return -1;
}

size_t Client::GetNumberOfClients()
{
	return clients.size();
}

int Client::SendClient(sockaddr* sock, std::string message)
{
	return sender.SendClient(MySocket, sock, message);
}

int Client::SendAllClient(std::string message)
{
	for (auto client : clients)
	{
		sender.SendClient(MySocket, &client.sock, message);
	}

	return 1;
}

void Client::UpdateState()
{
	if (lock) return;
	// send lock
	std::string actual = PackOwnData();
	std::string hash = lockStepManager.HashInput(actual);

	// tell all clients to lock and send a hash input
	std::vector<std::string> info;
	info.push_back(hash);
	SendAllClient(Parser::CreateHeader("[LOCK]", MyInfo.name, MyInfo.port, info));

	// recv hash from clients
	std::unordered_map<ShipID, std::string> hashStrings;
	while (hashStrings.size() != clients.size())
	{
		std::string message;
		if (ReceiveClient(message) <= 0) continue;
		std::pair<ShipID, std::string> pair;
		if (HandleLockStepMessage(pair, message))
			hashStrings.insert(pair);
	}

	// send actual
	std::vector<std::string> actualVec;
	actualVec.push_back(actual);
	SendAllClient(Parser::CreateHeader("[ACTUAL]", MyInfo.name, MyInfo.port, actualVec));

	// receive actual
	std::unordered_map<ShipID, std::string> lockedStrings;
	while (lockedStrings.size() != clients.size())
	{
		std::string message;
		if (ReceiveClient(message) <= 0) continue;
		std::pair<ShipID, std::string> pair;
		if (HandleLockStepMessage(pair, message))
			lockedStrings.insert(pair);
	}

	// compare hash
	for (size_t i = 0; i < lockedStrings.size(); i++)
	{
		std::vector<std::string> params = Parser::GetPayload(lockedStrings[static_cast<ShipID>(i)]);
		if (params.size() < 9) continue;
		int playerID = std::stoi(params[0]);
		AEVec2 Position;
		AEVec2 Velocity;
		AEVec2 Acceleration;
		float direction;
		Position.x = std::stof(params[1]);
		Position.y = std::stof(params[2]);
		Velocity.x = std::stof(params[3]);
		Velocity.y = std::stof(params[4]);
		Acceleration.x = std::stof(params[5]);
		Acceleration.y = std::stof(params[6]);
		direction = std::stof(params[7]);

		size_t index = GetClientByID(static_cast<ShipID>(i));
		ClientInfo* info = GetClient(index);
		info->state = static_cast<ShipState>(std::stoi(params[8]));
		std::cout << "CHANGING STATE AH: " << static_cast<int>(info->state) << std::endl;
		UpdateDeadReckoning(static_cast<ShipID>(playerID), Position, Velocity, Acceleration, direction, g_dt);
	}
}

void Client::RecvUpdateState(ClientInfo* info, std::string hash)
{
	// receive lock
	std::string hashString = hash;

	// create hash
	std::string actual = PackOwnData();

	// send hash input
	std::string OwnHash = lockStepManager.HashInput(actual);

	// send hash
	SendClient(&info->sock, Parser::CreatePayload("[HASHED]", MyInfo.name, MyInfo.port, OwnHash));

	// receive actual
	std::string lockString;
	while (lockString.empty())
	{
		std::string message;
		ReceiveClient(message);

		if (!message.empty())
		{
			std::vector<std::string> params = Parser::GetHeader(message, std::string{});
			params.erase(params.begin()); // erase name
			params.erase(params.begin()); // erase port
			lockString = Parser::VectorToString(params);
		}
	}

	SendClient(&info->sock, Parser::CreatePayload("[ACTUAL]", MyInfo.name, MyInfo.port, actual));

	if (lockStepManager.CompareInput(lockString, hashString))
	{
		std::vector<std::string> params = Parser::GetPayload(lockString);
		int playerID = std::stoi(params[0]);
		AEVec2 Position;
		AEVec2 Velocity;
		AEVec2 Acceleration;
		float direction;
		Position.x = std::stof(params[1]);
		Position.y = std::stof(params[2]);
		Velocity.x = std::stof(params[3]);
		Velocity.y = std::stof(params[4]);
		Acceleration.x = std::stof(params[5]);
		Acceleration.y = std::stof(params[6]);
		direction = std::stof(params[7]);

		info->state = static_cast<ShipState>(std::stoi(params[8]));
		UpdateDeadReckoning(static_cast<ShipID>(playerID), Position, Velocity, Acceleration, direction, g_dt);
	}

	lock = false;
}

std::vector<std::string> Client::PackData(ShipID id, GameObjInst* obj)
{
	std::vector<std::string> params;
	params.push_back(std::to_string(static_cast<int>(id)));
	params.push_back(std::to_string(GSManager->GetAsteroidGameState().IDToPlayerShip_[id]->posCurr.x));
	params.push_back(std::to_string(GSManager->GetAsteroidGameState().IDToPlayerShip_[id]->posCurr.y));
	params.push_back(std::to_string(GSManager->GetAsteroidGameState().IDToPlayerShip_[id]->velCurr.x));
	params.push_back(std::to_string(GSManager->GetAsteroidGameState().IDToPlayerShip_[id]->velCurr.y));
	params.push_back(std::to_string(40.0f));
	params.push_back(std::to_string(40.0f));
	params.push_back(std::to_string(GSManager->GetAsteroidGameState().IDToPlayerShip_[id]->dirCurr));

	return params;
}

int Client::ReceiveClient(std::string& message)
{
	const int bytesReceived = receiver.RecvFromSocket(MySocket, message);

	return bytesReceived;
}

size_t Client::RegisterClient(std::string name, std::string port)
{
	ClientInfo client;
	client.name = name;
	client.port = port;

	clients.push_back(client);

	return clients.size() - 1;
}

ClientInfo* Client::GetClientByName(std::string name, std::string port)
{
	ClientInfo* clientInfo = nullptr;
	for (auto& _client : clients)
	{
		if (name == _client.name && port == _client.port)
		{
			clientInfo = &_client;
			break;
		}
	}

	return clientInfo;
}

int Client::InitialiseClientMember(ClientInfo& client)
{
	std::cout << "Attempting UDP Connection with Client" << std::endl;

	const std::string serverPortString = client.port;
	const std::string clientPortString = MyInfo.port;

	// -------------------------------------------------------------------------
	// Create a socket and bind it to own network interface controller.
	//
	// socket()
	// bind()
	// -------------------------------------------------------------------------

	addrinfo* serverInfo = nullptr;
	int errorCode = getaddrinfo(client.name.c_str(), client.port.c_str(), &hints, &serverInfo);
	if ((errorCode) || (serverInfo == nullptr))
	{
		std::cerr << "getaddrinfo() failed." << std::endl;
		return errorCode;
	}

	sockaddr serverAddress{};
	serverAddress = *serverInfo->ai_addr;

	client.sock = serverAddress;

	freeaddrinfo(serverInfo);

	std::cout << "CLIENT: " << std::endl;
	std::cout << client.name << std::endl;
	std::cout << client.port << std::endl;

	std::cout << "Successfully stored client address." << std::endl;

	return 200;
}
void Client::UpdateHash()
{
	while (!AllHashUpdated())
	{
		std::string message;
		ReceiveClient(message);
		HandleRecvMessage(message);
	}
}
bool Client::CheckAllHash()
{
	while (!AllLocked())
	{
		std::string message;
		const int bytesReceived = ReceiveClient(message);
		if (bytesReceived == 0 || bytesReceived == SOCKET_ERROR) continue;
		HandleRecvMessage(message);
	}

	for (auto client : clients)
	{
		if (lockStepManager.HashInput(client.sendlockedState) != client.sendhashString)
		{
			return false;
		}
	}

	return true;
}
bool Client::AllHashUpdated()
{
	for (auto client : clients)
	{
		if (client.sendhashString.empty())
			return false;
	}

	return true;
}

bool Client::AllLocked()
{
	for (auto client : clients)
	{
		if (client.sendlockedState.empty())
			return false;
	}

	return true;
}

void Client::ResetHash()
{
	for (auto& client : clients)
	{
		client.recvhashString.clear();
		client.recvlockedState.clear();
		client.sendhashString.clear();
		client.sendlockedState.clear();
	}
}

std::string Client::PackOwnData()
{
	auto it = GSManager->GetAsteroidGameState().IDToPlayerShip_.find(MyInfo.id);
	std::vector<std::string> param = PackData(MyInfo.id, it->second);
	param.push_back(std::to_string(static_cast<int>(MyInfo.state)));

	std::string actual;
	for (auto string : param)
	{
		actual += string + "\n";
	}

	return actual;
}

void Client::UpdateDeadReckoning(ShipID id, AEVec2 Position, AEVec2 Velocity, AEVec2 Acceleration, float direction, float dt)
{
	IdtoDeadReckoning[id].ReceivedPacket(Position, Velocity, Acceleration, direction, dt);
}

void Client::AllDeadReckoningCorrection(float dt)
{
	for (auto client : clients)
	{
		AEVec2 position;
		AEVec2 velocity;
		float direction;
		IdtoDeadReckoning[client.id].Run(position, velocity, direction, dt, client.id);
		//pass back to fikrul here
		GSManager->GetAsteroidGameState().IDToPlayerShip_[client.id]->posCurr = position;
		GSManager->GetAsteroidGameState().IDToPlayerShip_[client.id]->velCurr = velocity;
		GSManager->GetAsteroidGameState().IDToPlayerShip_[client.id]->dirCurr = direction;
	}
}

void Client::SendUpdatePacket(ShipID id)
{

	std::vector<std::string> params;
	params.push_back(std::to_string(static_cast<int>(id)));
	params.push_back(std::to_string(GSManager->GetAsteroidGameState().IDToPlayerShip_[id]->posCurr.x));
	params.push_back(std::to_string(GSManager->GetAsteroidGameState().IDToPlayerShip_[id]->posCurr.y));
	params.push_back(std::to_string(GSManager->GetAsteroidGameState().IDToPlayerShip_[id]->velCurr.x));
	params.push_back(std::to_string(GSManager->GetAsteroidGameState().IDToPlayerShip_[id]->velCurr.y));
	params.push_back(std::to_string(40.0f));
	params.push_back(std::to_string(40.0f));
	params.push_back(std::to_string(GSManager->GetAsteroidGameState().IDToPlayerShip_[id]->dirCurr));

	//SendAllClient(Parser::CreateHeader("[UPDATE]", params));
}
void Client::createDeadReckoning(ShipID id)
{
	IdtoDeadReckoning.insert(std::make_pair(id, DeadReckoning{}));
}

void Client::UpdateAllDeadReckoningDT(float dt)
{
	for (auto client : clients)
	{
		IdtoDeadReckoning[client.id].UpdateTime(dt);
	}
}

void Client::HandleRecvMessage(std::string message)
{
	if (message.empty()) return;
	std::vector<std::string> params;
	std::string header;
	params = Parser::GetHeader(message, header);

	ClientInfo* info = GetClientByName(params[0], params[1]);
	params.erase(params.begin());
	params.erase(params.begin());

	if (header == "[READY]")
	{
		// set the client to true for readycheck
		info->readyCheck = true;

		// send back a ready ready command
		std::vector<std::string> paramsToSend;
		paramsToSend.push_back(std::to_string(MyInfo.readyCheck));
		std::string message = Parser::CreateHeader("[READYREADY]", MyInfo.name, MyInfo.port, paramsToSend);

		SendClient(&info->sock, message);
	}
	else if (header == "[READYREADY]")
	{
		info->readyCheck = std::stoi(params[0]);
	}
	else if (header == "[LOCK]")
	{
		RecvUpdateState(info, params[0]);
	}
	else if (header == "[HASHED]")
	{
		info->sendhashString = params[0];
	}
	else if (header == "[UNLOCK]")
	{
		auto it = GSManager->GetAsteroidGameState().IDToPlayerShip_.find(MyInfo.id);
		std::vector<std::string> params = PackData(MyInfo.id, it->second);
		params.push_back(std::to_string(static_cast<int>(MyInfo.state)));

		std::string _message;
		for (auto string : params)
		{
			_message += string + "\n";
		}

		_message = Parser::CreatePacket("[UNLOCKED]", _message);
		SendClient(&info->sock, _message);
	}
	else if (header == "[UNLOCKED]")
	{
		std::string locked;
		for (auto string : params)
		{
			locked += string + "\n";
		}

		info->sendlockedState = locked;
	}
	else if (header == "[RESTART]")
	{
		info->readyCheck = true;
	}
	else if (header == "[QUIT]")
	{
		GSManager->SetGameStateNextIndex(GS_QUIT);
	}
}

bool Client::HandleLockStepMessage(std::pair<ShipID, std::string>& pair, std::string message)
{
	std::string header;
	std::vector<std::string> params = Parser::GetHeader(message, header);

	ClientInfo* info = GetClientByName(params[0], params[1]);
	params.erase(params.begin());
	params.erase(params.begin());

	if (header == "[HASHED]")
	{
		ShipID id = info->id;
		pair = std::make_pair(id, Parser::VectorToString(params));
		return true;
	}
	else if (header == "[ACTUAL]")
	{
		ShipID id = static_cast<ShipID>(std::stoi(params[0]));
		pair = std::make_pair(id, Parser::VectorToString(params));
		return true;
	}

	return false;
}
