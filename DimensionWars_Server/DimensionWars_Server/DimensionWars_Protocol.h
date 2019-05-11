#pragma once

// ���� ���α׷��ε� �̰� ������ �� �ǰ�����.
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <DirectXMath.h>

constexpr unsigned short SERVERPORT = 3500;	// ���� ��Ʈ
constexpr unsigned short BUFSIZE = 1024;		// ������ ũ��

constexpr unsigned short MAX_USER = 500;	// �� ��� �� ������ �÷��̾�� 6��. �� ������ ����ü�� ���� �÷��̾ ���� ���� �ƴϴ�.
constexpr unsigned short Cube_start = 450;
constexpr unsigned short MAX_PLAYER = 6;	// �ڵ� �� �� �������� �ʵ��� ���� �̸��� ���� ����

constexpr unsigned int MAX_OBJECTS = 1000;		// �� ������ ������ �÷��̾�, ����ü ���� ������ ������ ���� ������ ��� ������Ʈ ����
// �÷��̾� 6��, ť�� 50��, ����ü X�� 

constexpr float WORLD_HORIZONTAL = 25000.0f; // ������ ����
constexpr float WORLD_WIDTH = WORLD_HORIZONTAL;	// �ڵ� �� �� �������� �ʵ��� ���� �̸��� ���� ����
constexpr float WORLD_HEIGHT = 25000.0f;		// ������ ����
constexpr float WORLD_VERTICAL = 25000.0f;	// ������ ����

constexpr float VIEW_RANGE = 2500.0f;	// �÷��̾��� �þ�

constexpr float MAX_CUBE_SIZE = 700.0f;	// ť�� ������


// �÷��̾��� Ű �Է� ������ �м��ϱ� ���� ������ define ������. 6��Ʈ�� ���
enum Key_Moving_Direction {
	DIR_FORWARD = 0x01,
	DIR_BACKWARD = 0x02,
	DIR_LEFT = 0x04,
	DIR_RIGHT = 0x08,
	DIR_UP = 0x10,	// ��Ÿ� ����ϴ� �� ���
	DIR_DOWN = 0x20 // �߶�
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
	
};

struct SCPacket_PutPlayer : SCPacket_Position {
	// �÷��̾ � ĳ�����ΰ� ���� ������ �߰�
};

struct SCPacket_RemovePlayer : SCPacket_Base {
	// �� id�� ���� ��ü�� ȭ�鿡�� �����
};

struct SCPacket_MapInfo : SCPacket_Base {
	DirectX::XMFLOAT3 position; // ��ġ����
	DirectX::XMFLOAT3 rotate; // ȸ������


	// ���� �⺻ ����
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

};

struct CSPacket_Attack : CSPacket_Base {
	unsigned char attack_type;	// � ��Ÿ����, ��ų����. �� ���� ��� �� �÷��̾� ĳ���Ϳ� ���ǵ� enum state ���� ����ִ´�.
};

#pragma pack(pop)

// value�� minimum, maximum ���̷� �����ϴ� �Լ�
template <typename var>
var clamp(var minimum, var value, var maximum) { return  (minimum > ((value < maximum) ? value : maximum)) ? minimum : ((value < maximum) ? value : maximum); }
