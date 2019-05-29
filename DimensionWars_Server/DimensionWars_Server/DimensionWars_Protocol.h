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
constexpr unsigned int MAX_OBJECTS = 1000;		// �� ������ ������ �÷��̾�, ����ü ���� ������ ������ ���� ������ ��� ������Ʈ ����
// �÷��̾� 6��, ť�� 50��, ����ü X�� 

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
		Count

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
		Character_Info,
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
	
};

struct SCPacket_RemovePlayer : SCPacket_Base {
	// �� id�� ���� ��ü�� ȭ�鿡�� �����
};

struct SCPacket_MapInfo : SCPacket_Base {
	DirectX::XMFLOAT3 position; // ��ġ����
	DirectX::XMFLOAT3 rotate; // ȸ������

	// ���� �⺻ ����
};

struct SCPacket_Attack : SCPacket_Base {
	unsigned int animation_state;
};

struct SCPacket_ProjectTile : SCPacket_Base {
	DirectX::XMFLOAT3 position;
	unsigned short projectTile_type;
	
	
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

struct CSPacket_Attack : CSPacket_Base {
	unsigned char attack_type;	// � ��Ÿ����, ��ų����. �� ���� ��� �� �÷��̾� ĳ���Ϳ� ���ǵ� enum state ���� ����ִ´�.
	unsigned int animation_state;
};


struct CSPacket_CharacterType : CSPacket_Base {
	unsigned short character_type;	// ĳ�� ����
};

#pragma pack(pop)

// value�� minimum, maximum ���̷� �����ϴ� �Լ�
template <typename var>
var clamp(var minimum, var value, var maximum) { return  (minimum > ((value < maximum) ? value : maximum)) ? minimum : ((value < maximum) ? value : maximum); }
