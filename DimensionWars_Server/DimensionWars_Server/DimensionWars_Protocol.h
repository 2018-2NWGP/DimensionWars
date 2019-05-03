#pragma once

// ���� ���α׷��ε� �̰� ������ �� �ǰ�����.
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <DirectXMath.h>

const unsigned short int const SERVERPORT = 3500;	// ���� ��Ʈ
const unsigned short int const BUFSIZE = 1024;		// ������ ũ��

constexpr unsigned char const MAX_USER = 6;	// �� ��� �� ������ �÷��̾�� 6��. �� ������ ����ü�� ���� �÷��̾ ���� ���� �ƴϴ�.
const unsigned char const MAX_PLAYER = MAX_USER;	// �ڵ� �� �� �������� �ʵ��� ���� �̸��� ���� ����

constexpr unsigned int const MAX_OBJECTS = 1000;		// �� ������ ������ �÷��̾�, ����ü ���� ������ ������ ���� ������ ��� ������Ʈ ����
// �÷��̾� 6��, ť�� 50��, ����ü X�� 

const float const WORLD_HORIZONTAL = 25000.0f; // ������ ����
const float const WORLD_WIDTH = WORLD_HORIZONTAL;	// �ڵ� �� �� �������� �ʵ��� ���� �̸��� ���� ����
const float const WORLD_HEIGHT = 25000.0f;		// ������ ����
const float const WORLD_VERTICAL = 25000.0f;	// ������ ����

const float const VIEW_RANGE = 2500.0f;	// �÷��̾��� �þ�


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
	char id;		// client�� ID
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

struct CSPacket_Base {
	char size;
	char type;
};

struct CSPacket_Move : CSPacket_Base {
	unsigned char dir;	// Moving_Direction
};

struct CSPacket_Attack : CSPacket_Base {
	unsigned char attack_type;	// � ��Ÿ����, ��ų����. �� ���� ��� �� �÷��̾� ĳ���Ϳ� ���ǵ� enum state ���� ����ִ´�.
};

#pragma pack(pop)

// value�� minimum, maximum ���̷� �����ϴ� �Լ�
template <typename var>
var clamp(var minimum, var value, var maximum) { return  (minimum > ((value < maximum) ? value : maximum)) ? minimum : ((value < maximum) ? value : maximum); }
