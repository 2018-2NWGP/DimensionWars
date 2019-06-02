#pragma once

enum  Object_coll
{
	not_coll = 0,
	player_and_cube = 1,
	player_and_player = 2,
	player_and_card = 3,
	player_and_slash = 4,
	tile_and_cube = 5,
};
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
	// �� 


private:
	HANDLE								hIOCP;
	WSADATA								WSAData;
	SOCKADDR_IN							serverAddr;
	SOCKET								listenSocket;

	std::vector<std::thread>			workerThreads;
	std::priority_queue<TimerEvent, std::vector<TimerEvent>, TimerEvent::Priority>	timerQueue;
	std::mutex					timerQueue_Lock;
	SOCKETINFO							objects[MAX_OBJECTS];	// client, ����ü, ť�� ��ֹ� � �� ID�� ��� �� Ŭ���̾�Ʈ�� �ѷ���� �ϹǷ� Objects��� ���
	float fDistance = 10.0f;

	XMFLOAT3					m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3     				m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float           			m_fMaxVelocityXZ = 0.0f;
	float           			m_fMaxVelocityY = 0.0f;
	float           			m_fFriction = 0.0f;

	XMFLOAT3					m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3					m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);
	unsigned int ani_state;


private:
	bool NetworkInitialize();

	static DWORD WINAPI RegisterAcceptThread(LPVOID self);
	void AcceptThread();
	static DWORD WINAPI RegisterWorkerThread(LPVOID self);
	void WorkerThread();
	static DWORD WINAPI RegisterTimerThread(LPVOID self);
	void TimerThread();

	void ObjectInitialize();
	void SendPacket(unsigned short int id, char *packet);	// packet�� id Ŭ���̾�Ʈ�� ������
	void RecvPacket(unsigned short int id);	// �ش� id�� Ŭ���̾�Ʈ�� ��Ŷ�� �޾ƿ´�
	void SendLoginOKPacket(unsigned short int to);
	void SendPutPlayerPacket(unsigned short int to, unsigned short int obj);
	void SendPositionPacket(unsigned short to, unsigned short obj);
	void SendRemovePlayerPacket(unsigned short int to, unsigned short int id);
	void ProcessPacket(unsigned short int id, char *buf);
	void SendMapInfoPacket(unsigned short to, unsigned short obj);
	void SendAnimationPaket(unsigned short to, unsigned short obj);
	void SendCardPaket(unsigned short to, unsigned short obj);
	void SendSlashPaket(unsigned short to, unsigned short obj);


	void DisConnect(unsigned short int id);
	const unsigned short int GetNewID();
	bool isNearObject(unsigned short int a, unsigned short int b);
	float Distance(XMFLOAT3 vector1, XMFLOAT3 vector2);

	/*void Move(const XMFLOAT3 & xmf3Shift, bool bUpdateVelocity);
	void Update(float fTimeElapsed);*/

	//ť���浹
	int Collision();
	bool check_f, check_b, check_r, check_l, check_u, check_d;

	void AddTimerEvent(unsigned int id, TimerEvent::Command cmd = TimerEvent::Command::Update, double seconds = 0.01f);
	void Update(unsigned int id);
	//void ObjectCollision(unsigned int id_1, unsigned int id_2);
	
	int character_type = 99;
	int card_num = 0;
	int slash_num = 0;



};