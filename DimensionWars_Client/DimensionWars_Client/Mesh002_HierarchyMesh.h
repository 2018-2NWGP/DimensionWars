#pragma once
#include "Mesh000_BaseMesh.h"


class HierarchyMesh : public BaseMesh
{
public:
	HierarchyMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) ;
	virtual ~HierarchyMesh();

public:
	char							m_pstrMeshName[64] = { 0 };

protected:


	int								m_nSubMeshes = 0;
	int								*m_pnSubSetIndices = nullptr;
	unsigned int					**m_ppnSubSetIndices = nullptr;

	ID3D12Resource					**m_ppd3dSubSetIndexBuffers = nullptr;
	ID3D12Resource					**m_ppd3dSubSetIndexUploadBuffers = nullptr;
	D3D12_INDEX_BUFFER_VIEW			*m_pd3dSubSetIndexBufferViews = nullptr;


	XMFLOAT4						*m_pxmf4Colors = nullptr;
	XMFLOAT3						*m_pxmf3Normals = nullptr;
	XMFLOAT3						*m_pxmf3Tangents = nullptr;
	XMFLOAT3						*m_pxmf3BiTangents = nullptr;
	XMFLOAT2						*m_pxmf2TextureCoords0 = nullptr;
	XMFLOAT2						*m_pxmf2TextureCoords1 = nullptr;

	ID3D12Resource					*m_pd3dTextureCoord0Buffer = nullptr;
	ID3D12Resource					*m_pd3dTextureCoord0UploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dTextureCoord0BufferView;

	ID3D12Resource					*m_pd3dTextureCoord1Buffer = nullptr;
	ID3D12Resource					*m_pd3dTextureCoord1UploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dTextureCoord1BufferView;

	ID3D12Resource					*m_pd3dNormalBuffer = nullptr;
	ID3D12Resource					*m_pd3dNormalUploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dNormalBufferView;

	ID3D12Resource					*m_pd3dTangentBuffer = nullptr;
	ID3D12Resource					*m_pd3dTangentUploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dTangentBufferView;

	ID3D12Resource					*m_pd3dBiTangentBuffer = nullptr;
	ID3D12Resource					*m_pd3dBiTangentUploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dBiTangentBufferView;

public:
	unsigned int GetType() const { return(m_nType); }

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) { }
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList) { }
	virtual void ReleaseShaderVariables() { }

	virtual void ReleaseUploadBuffers();

	virtual void OnPreRender(ID3D12GraphicsCommandList *pd3dCommandList, void *pContext);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, int nSubSet);
	virtual void OnPostRender(ID3D12GraphicsCommandList *pd3dCommandList, void *pContext);

	virtual void LoadMeshFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, FILE *pInFile);
};
