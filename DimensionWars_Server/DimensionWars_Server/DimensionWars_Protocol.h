#pragma once

// ���� ���α׷��ε� �̰� ������ �� �ǰ�����.
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <DirectXMath.h>

constexpr unsigned short SERVERPORT = 3500;	// ���� ��Ʈ
constexpr unsigned short BUFSIZE = 1024;		// ������ ũ��

constexpr unsigned short MAX_USER = 500;	// �� ��� �� ������ �÷��̾�� 6��. �� ������ ����ü�� ���� �÷��̾ ���� ���� �ƴϴ�.
constexpr unsigned short Cube_start = 10;
constexpr unsigned short MAX_PLAYER = 6;	// �ڵ� �� �� �������� �ʵ��� ���� �̸��� ���� ����
constexpr unsigned short Slash_start = 100; // �˱� ����
constexpr unsigned short Card_start = 200;  // ���ڻ� ��Ÿ ����
constexpr unsigned int MAX_OBJECTS = 1001;		// �� ������ ������ �÷��̾�, ����ü ���� ������ ������ ���� ������ ��� ������Ʈ ����
// �÷��̾� 6��, ť�� 50��, ����ü X�� 
constexpr unsigned short Arrow_start = 300;
constexpr unsigned short Arrow_end = 600;

constexpr unsigned short Potal_start = 970;
constexpr unsigned short Potal_end = 1000;
constexpr unsigned int Reaper_scy = 1001;  // ��� ��

constexpr float WORLD_HORIZONTAL = 25000.0f; // ������ ����
constexpr float WORLD_WIDTH = WORLD_HORIZONTAL;	// �ڵ� �� �� �������� �ʵ��� ���� �̸��� ���� ����
constexpr float WORLD_HEIGHT = 25000.0f;		// ������ ����
constexpr float WORLD_VERTICAL = 25000.0f;	// ������ ����

constexpr float VIEW_RANGE = 2500.0f;	// �÷��̾��� �þ�

constexpr float MAX_CUBE_SIZE = 700.0f;	// ť�� ������

constexpr unsigned short Slash_end = 200;
constexpr unsigned short Card_end = 300;

namespace GrimReaper
{
	// 0�� Full �ִϸ��̼��� �׽�Ʈ������ FBX ������ ��� �ִϸ��̼��� ��ȸ�ϸ� ����ϴ� ��, �������� �ʿ� ����

	enum GrimReaperState {	// ��� �ִϸ��̼� ��Ŷ
		Full = 0,
		Idle = 1,
		OnHit = 2,
		Guard = 3,
		Burf = 4,
		Hide_Invasion = 5,
		// Full_Attack = 6,
		First_Attack = 6,
		Second_Attack = 7,
		Third_Attack = 8,
		Slash_Wave = 9,
		Beheading = 10,
		Move_Forward = 11,
		Move_Right = 13,	//Move_Right = 12,
		Move_Left = 12,		//Move_Left = 13,
		Move_Backward = 14,
		Down = 15
	};
}

namespace Gambler
{
	enum GamblerState {	// ���ڻ� �ִϸ��̼� ��Ŷ
		Full = 0,
		Idle = 1,
		OnHit = 2,
		Guard = 3,
		Burf = 4,
		Shuffle = 5,
		Idle_Attack = 6,
		Multi_Shot = 7,
		Wild_Card = 8,
		Jump = 9,
		Jump_Attack = 10,
		Fall = 11,
		Fall_Attack = 12,
		Move_Forward = 13,
		Move_Forward_Attack1 = 14,
		Move_Forward_Attack2 = 15,
		Move_Right_Forward = 19,			// Move_Right_Forward = 16,
		Move_Right_Forward_Attack1 = 20,	// Move_Right_Forward_Attack1 = 17,
		Move_Right_Forward_Attack2 = 21,	// Move_Right_Forward_Attack2 = 18,
		Move_Left_Forward = 16,				// Move_Left_Forward = 19,
		Move_Left_Forward_Attack1 = 17,		// Move_Left_Forward_Attack1 = 20,
		Move_Left_Forward_Attack2 = 18,		// Move_Left_Forward_Attack2 = 21,
		Move_Right = 25,					// Move_Right = 22,
		Move_Right_Attack1 = 26,			// Move_Right_Attack1 = 23,
		Move_Right_Attack2 = 27,			// Move_Right_Attack2 = 24,
		Move_Left = 22,						// Move_Left = 25,
		Move_Left_Attack1 = 23,				// Move_Left_Attack1 = 26,
		Move_Left_Attack2 = 24,				// Move_Left_Attack2 = 27,
		Move_Backward = 28,
		Move_Backward_Attack1 = 29,
		Move_Backward_Attack2 = 30,
		Move_Right_Backward = 34,			//Move_Right_Backward = 31,
		Move_Right_Backward_Attack1 = 35,	//Move_Right_Backward_Attack1 = 32,
		Move_Right_Backward_Attack2 = 36,	//Move_Right_Backward_Attack2 = 33,
		Move_Left_Backward = 31,			//Move_Left_Backward = 34,
		Move_Left_Backward_Attack1 = 32,	//Move_Left_Backward_Attack1 = 35,
		Move_Left_Backward_Attack2 = 33,	//Move_Left_Backward_Attack2 = 36,
		Down = 37
	};
}

namespace ElfArcher
{
	enum ElfArcherState {	// ���ڻ� �ִϸ��̼� ��Ŷ
		Full = 0,
		Idle = 1,
		OnHit = 2,
		Guard = 3,
		Burf = 4,
		First_Shot = 5,
		Second_Shot = 6,
		Third_Shot = 7,
		Guide_Shot = 8,
		Snipe = 9,
		Jump = 10,
		Jump_Attack = 11,
		Fall = 12,
		Fall_Attack = 13,
		Move_Forward = 14,
		Move_Right_Forward = 16,	// Move_Right_Forward = 15,
		Move_Left_Forward = 15,		// Move_Left_Forward = 16,
		Move_Right = 18,			// Move_Right = 17,
		Move_Left = 17,				// Move_Left = 18,
		Move_Backward = 19,
		Move_Right_Backward = 21,	// Move_Right_Backward = 20,
		Move_Left_Backward = 20,	// Move_Left_Backward = 21,
		Down = 22
	};
}

using ReaperState = GrimReaper::GrimReaperState;
using GamblerState = Gambler::GamblerState;
using ElfState = ElfArcher::ElfArcherState;


// �÷��̾��� Ű �Է� ������ �м��ϱ� ���� ������ define ������. 6��Ʈ�� ���
enum Key_Moving_Direction {
	DIR_FORWARD = 0x01,
	DIR_BACKWARD = 0x02,
	DIR_LEFT = 0x04,
	DIR_RIGHT = 0x08,
	DIR_UP = 0x10,	// ��Ÿ� ����ϴ� �� ���
	DIR_DOWN = 0x20 // �߶�
};

enum ProjectTile {
	Card = 0,
	Slash=1,
	Arrow=2
};

namespace SC 
{
	enum ServerToClientSocketType {
		LoginOK = 1,
		PutPlayer = 2,
		RemovePlayer = 3,
		Position = 4,
		OnHit = 5, // �ش� Ŭ���̾�Ʈ�� �浹�������Ƿ� ���� �ִϸ��̼��� ����ض�.
		Down = 6, // �ش� Ŭ���̾�Ʈ�� �ٿ� ���ݿ� �ǰݵǾ����Ƿ� �ǰ� �ִϸ��̼��� ����ض�.
		MapInfo =7,
		Attack=8,
		ProjectTile = 9,
		Animation= 10,
		Potal =11,
		Count=12,
		CreateRoom=13,
		EnterRoom=14,
		ChangeScene_L_R=15,
		ChangeScene_R_L = 16,
		ExitRoom=17,
		InfoScene=18,
		Rotate=19,
		Chracter_type=20,
		GameTime=21,
		ReadyGame=22,
	};
}
using SC_Type = SC::ServerToClientSocketType;	// ���� ���·� ����� ���̴�.

namespace CS 
{
	enum ClientToServerSocketType {
		Move = 1,
		Attack = 2,
		Guard = 3,
		Burf = 4,
		Animation =5,
		Character_Info=6,
		Room_Create=7,
		Room_Enter=8,
		Scene_Change_L_R=9,
		Scene_Change_R_L = 10,
		Room_Exit=11,
		Sceneinfo =12,
		Rotate=13,
		GameTimer=14,
		GameReady=15,
	};
}
using CS_Type = CS::ClientToServerSocketType;	// ���� ���·� ����� ���̴�.

#pragma pack(push, 1)
// ����ü ����� ���� Ȱ���Ͽ� �ߺ��� �ڵ��� ���̸� ����.
struct SCPacket_Base {
	char size;
	char type;
	unsigned short id;		// client�� ID
};

using SCPacket_LoginOK = SCPacket_Base; // Login OK ��Ŷ�� �⺻ ��Ŷ�� ���� ��Ұ� �����ϹǷ� �ܼ� Using

struct SCPacket_Position : SCPacket_Base {
	// �� id�� ���� ��ü�� �ش� ��ǥ��
	DirectX::XMFLOAT3 position;
	unsigned int animation_state;
};

struct SCPacket_PutPlayer : SCPacket_Position {
	// �÷��̾ � ĳ�����ΰ� ���� ������ �߰�
	unsigned short character_type;
	float hp;
	float sp;
	
	
};

struct SCPacket_RemovePlayer : SCPacket_Base {
	// �� id�� ���� ��ü�� ȭ�鿡�� �����
};

struct SCPacket_MapInfo : SCPacket_Base {
	DirectX::XMFLOAT3 position; // ��ġ����
	DirectX::XMFLOAT3 rotate; // ȸ������
	bool build_cube;
	// ���� �⺻ ����
};

struct SCPacket_PotalInfo : SCPacket_Base {
	DirectX::XMFLOAT3 position; // ��ġ����
	DirectX::XMFLOAT3 rotate; // ȸ������
	bool build_portal;
							  // ���� �⺻ ����
};

struct SCPacket_Animation : SCPacket_Base {
	unsigned int animation_state;
};

struct SCPacket_Attack : SCPacket_Base {
	unsigned char attack_type;
	DirectX::XMFLOAT3 position;
};

struct SCPacket_ProjectTile : SCPacket_Base {
	DirectX::XMFLOAT3 position;
	unsigned short projectTile_type;
	bool build_projecttile;
	
	
};

struct SCPacket_Hit : SCPacket_Base {
	float hp;
};

struct SCPacket_CreateRoom : SCPacket_Base {
	unsigned short room_num;
	unsigned short player_num;
	unsigned short scene;
	bool check;
};

struct SCPacket_EnterRoom : SCPacket_Base {
	unsigned short room_num;
	unsigned short player_num;
	unsigned short scene;
	bool check;
};

struct SCPacket_ChangeScene_L_R : SCPacket_Base {
	unsigned short room_num;
	unsigned short player_num;
	unsigned short scene;
	bool check;
};


struct SCPacket_ChangeScene_R_L : SCPacket_Base {
	unsigned short room_num;
	unsigned short player_num;
	unsigned short scene;
	bool check;
};

struct SCPacket_ExitRoom : SCPacket_Base {
	unsigned short room_num;
	unsigned short player_num;
	unsigned short scene;
	bool check;
};

struct SCPacket_InfoScene : SCPacket_Base {
	unsigned short scene;
};

struct SCPacket_Rotate : SCPacket_Base {
	float x;
	float y;
	float z;
	DirectX::XMFLOAT3 m_Right;
	DirectX::XMFLOAT3 m_Up;
	DirectX::XMFLOAT3 m_Look;
};
struct SCPacket_OtherCharacter : SCPacket_Base {
	unsigned short character_type;
};

struct SCPacket_GameTime : SCPacket_Base {
	unsigned short time;
};

struct SCPacket_ReadyGame : SCPacket_Base {
	unsigned short ready_state;
};

struct CSPacket_Base {
	char size;
	char type;
};

struct CSPacket_Move : CSPacket_Base {
	unsigned char dir;	// Moving_Direction
	DirectX::XMFLOAT3 m_Right;
	DirectX::XMFLOAT3 m_Up;
	DirectX::XMFLOAT3 m_Look;
	unsigned int animation_state;
	

};

struct CSPacket_Animation : CSPacket_Base {
	unsigned int animation_state;
};

struct CSPacket_Attack : CSPacket_Base {
	unsigned char attack_type;	// � ��Ÿ����, ��ų����. �� ���� ��� �� �÷��̾� ĳ���Ϳ� ���ǵ� enum state ���� ����ִ´�.
	DirectX::XMFLOAT3 position;
};


struct CSPacket_CharacterType : CSPacket_Base {
	unsigned short character_type;	// ĳ�� ����
};


struct CSPacket_RoomCreate : CSPacket_Base {
	unsigned short room_num;
	unsigned short player_num;
	unsigned short scene;
	unsigned short enter_type;
	bool check;
};

struct CSPacket_RoomEnter : CSPacket_Base {
	unsigned short room_num;
	unsigned short player_num;
	unsigned short scene;
	bool check;
};

struct CSPacket_SceneChange_L_R : CSPacket_Base {
	unsigned short room_num;
	unsigned short player_num;
	unsigned short scene;
	bool check;
};

struct CSPacket_SceneChange_R_L : CSPacket_Base {
	unsigned short room_num;
	unsigned short player_num;
	unsigned short scene;
	bool check;
};

struct CSPacket_RoomExit : SCPacket_Base {
	unsigned short room_num;
	unsigned short player_num;
	unsigned short scene;
	bool check;
};

struct CSPacket_SceneInfo : SCPacket_Base {
	unsigned short scene;
	unsigned short c_type;
};

struct CSPacket_Rotate : SCPacket_Base {
	float x;
	float y;
	float z;
	DirectX::XMFLOAT3 m_Right;
	DirectX::XMFLOAT3 m_Up;
	DirectX::XMFLOAT3 m_Look;
};

struct CSPacket_GameTimer : SCPacket_Base {
	unsigned short time;
};

struct CSPacket_GameReady : SCPacket_Base {
	unsigned short ready_state;
};


#pragma pack(pop)

// value�� minimum, maximum ���̷� �����ϴ� �Լ�
template <typename var>
var clamp(var minimum, var value, var maximum) { return  (minimum > ((value < maximum) ? value : maximum)) ? minimum : ((value < maximum) ? value : maximum); }
