#pragma once
#include "BaseObject.h"
class CubeObject : public BaseObject
{
public:
	CubeObject();
	~CubeObject();

	float cube_size; // ť�� ũ��
	XMFLOAT3 rotate; // ȸ������

};

