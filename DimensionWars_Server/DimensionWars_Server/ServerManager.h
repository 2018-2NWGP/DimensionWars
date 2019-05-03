#pragma once

class ServerManager
{
public:
	ServerManager();
	~ServerManager();

	// ���� �����ڴ� �����Ǿ�� �Ѵ�.
	ServerManager(const ServerManager&) = delete;
	ServerManager& operator=(const ServerManager&) = delete;

	// Main �Լ������� �̰� �ϳ��� ��������. �ٸ� public �Լ��� �ʿ� ����.
	void Run();

private:
	HANDLE								hIOCP;
	WSADATA								WSAData;
	SOCKADDR_IN							serverAddr;
	SOCKET								listenSocket;

	std::vector<std::thread>			workerThreads;

	SOCKETINFO							objects[MAX_USER];	// client, ����ü, ť�� ��ֹ� � �� ID�� ��� �� Ŭ���̾�Ʈ�� �ѷ���� �ϹǷ� Objects��� ���

private:
	bool NetworkInitialize();

	static DWORD WINAPI RegisterAcceptThread(LPVOID self);
	void AcceptThread();
	static DWORD WINAPI RegisterWorkerThread(LPVOID self);
	void WorkerThread();


	void ObjectInitialize();
	void SendPacket(unsigned short int id, char *packet);	// packet�� id Ŭ���̾�Ʈ�� ������
	void RecvPacket(unsigned short int id);	// �ش� id�� Ŭ���̾�Ʈ�� ��Ŷ�� �޾ƿ´�
	void SendLoginOKPacket(unsigned short int to);
	void SendPutPlayerPacket(unsigned short int to, unsigned short int obj);
	void SendPositionPacket(unsigned short int to, unsigned short int obj);
	void SendRemovePlayerPacket(unsigned short int to, unsigned short int id);
	void ProcessPacket(unsigned short int id, char *buf);

	void DisConnect(unsigned short int id);
	const unsigned short int GetNewID();
	bool isNearObject(unsigned short int a, unsigned short int b);
};