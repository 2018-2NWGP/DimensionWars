#pragma once

class BaseMesh;
class Material;
class BaseShader;
class BaseCamera;

struct CB_GAMEOBJECT_INFO
{
	XMFLOAT4X4	 m_xmf4x4World;
	unsigned int m_nObjectID;
	unsigned int m_nMaterialID;
};


class BaseObject
{
private:
	bool skinnedMesh = false;

public:
	BaseObject();
	BaseObject(unsigned int nMaterials);
//	BaseObject(unsigned int nMeshes = 1);
	~BaseObject();

	virtual void Release() {}

public:
	XMFLOAT4X4						m_xmf4x4World;

	//int							m_nMeshes = 0;
	BaseMesh						*m_pMesh = nullptr;
	
	int								m_nMaterials = 0;
	Material						**m_ppMaterials = nullptr;

	D3D12_GPU_DESCRIPTOR_HANDLE		m_d3dCbvGPUDescriptorHandle;


	ID3D12Resource					*m_pd3dcbGameObject = nullptr;
	CB_GAMEOBJECT_INFO				*m_pcbMappedGameObject = nullptr;

	bool							connected = false;
	unsigned short room_name = 0;
	unsigned short n_member = 0;  // 방에있는 인원수
	unsigned short c_type = 0; // 룸에서 사용하는 캐릭터 타입
	
protected:
	char directionBit = 0;

public:
	void SetMesh(BaseMesh *pMesh, bool isSkinnedMeshType = false, int nIndex = 0);
	void SetShader(BaseShader *pShader);
	void SetShader(int nMaterial, BaseShader *pShader);
	void BuildShaders(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);

	void SetMaterial(int nMaterial, Material *pMaterial);
	Material* GetMaterial(int nMaterial) const { 
		if (m_ppMaterials && m_nMaterials > nMaterial) 
			if (m_ppMaterials[nMaterial]) 
				return m_ppMaterials[nMaterial]; 
		return nullptr;
	}

	virtual void BuildLight();
	virtual void BuildMaterial();
	virtual void BuildLightsAndMaterials();

	virtual void BuildMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) { }

	void SetCbvGPUDescriptorHandle(D3D12_GPU_DESCRIPTOR_HANDLE d3dCbvGPUDescriptorHandle) { m_d3dCbvGPUDescriptorHandle = d3dCbvGPUDescriptorHandle; }
	void SetCbvGPUDescriptorHandlePtr(UINT64 nCbvGPUDescriptorHandlePtr) { m_d3dCbvGPUDescriptorHandle.ptr = nCbvGPUDescriptorHandlePtr; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetCbvGPUDescriptorHandle() { return(m_d3dCbvGPUDescriptorHandle); }

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, Material *pMaterial);
	
	virtual void ReleaseUploadBuffers();

	virtual void SetRootParameter(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void OnPrepareRender();
	virtual void OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList, BaseCamera *pCamera);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, BaseCamera *pCamera = nullptr);

	virtual void Animate(float fTimeElapsed);

	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetLook() const;
	XMFLOAT3 GetUp() const;
	XMFLOAT3 GetRight() const;

	virtual void SetPosition(float x, float y, float z);
	virtual void SetPosition(XMFLOAT3 xmf3Position);

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	virtual void Rotate(float fPitch, float fYaw, float fRoll);
	virtual void Rotate(XMFLOAT3 *pxmf3Axis, float fAngle);
	virtual void Rotate(XMFLOAT4 *pxmf4Quaternion);

	unsigned int GetMeshType(int nIndex = 0);

	char GetDirectionBit() const { return directionBit; }
	void SetDirectionBit(char dwBit) { directionBit = dwBit; }

	void SetLookAt(XMFLOAT3& xmf3Target, XMFLOAT3& xmf3Up);

	const bool isSkinnedMesh() const { return skinnedMesh; }
};