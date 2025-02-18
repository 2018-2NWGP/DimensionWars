#include "stdafx.h"
#include "Material.h"
#include "Texture.h"

#include "Scene000_BaseScene.h"
#include "Shader004_SkinnedAnimationShader.h"
#include "Object002_SkinnedFrameObject.h"

Material::Material(unsigned int nTextures)
{
	m_nTextures = nTextures;

	m_ppTextures = new Texture*[m_nTextures];
	m_ppstrTextureNames = new _TCHAR[m_nTextures][64];
	for (int i = 0; i < m_nTextures; ++i) 
		m_ppTextures[i] = nullptr;
	for (int i = 0; i < m_nTextures; ++i) 
		m_ppstrTextureNames[i][0] = '\0';
}

Material::~Material()
{
	if (m_pShader) m_pShader->Release();

	if (m_nTextures > 0) {
		for (int i = 0; i < m_nTextures; i++) 
			if (m_ppTextures[i]) 
				m_ppTextures[i]->Release();
		delete[] m_ppTextures;

		if (m_ppstrTextureNames) delete[] m_ppstrTextureNames;
	}
}

void Material::SetShader(BaseShader * pShader)
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();
}

void Material::SetTexture(Texture * pTexture, unsigned int nTexture)
{
	if (m_ppTextures[nTexture]) 
		m_ppTextures[nTexture]->Release();
	m_ppTextures[nTexture] = pTexture;
	if (m_ppTextures[nTexture]) 
		m_ppTextures[nTexture]->AddRef();
}

void Material::UpdateShaderVariable(ID3D12GraphicsCommandList * pd3dCommandList)
{
	//pd3dCommandList->SetGraphicsRoot32BitConstants(1, 4, &m_xmf4AmbientColor, 16);
	//pd3dCommandList->SetGraphicsRoot32BitConstants(1, 4, &m_xmf4AlbedoColor, 20);
	//pd3dCommandList->SetGraphicsRoot32BitConstants(1, 4, &m_xmf4SpecularColor, 24);
	//pd3dCommandList->SetGraphicsRoot32BitConstants(1, 4, &m_xmf4EmissiveColor, 28);

	//pd3dCommandList->SetGraphicsRoot32BitConstants(1, 1, &m_nType, 32);
	for (int i = 0; i < m_nTextures; i++)
		if (m_ppTextures[i]) 
			m_ppTextures[i]->UpdateShaderVariable(pd3dCommandList, 0);
}

void Material::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_nTextures; i++)
		if (m_ppTextures[i])
			m_ppTextures[i]->ReleaseUploadBuffers();
}


BaseShader *Material::m_pWireFrameShader = nullptr;
BaseShader *Material::m_pSkinnedAnimationWireFrameShader = nullptr;
BaseShader *Material::m_pStandardShader = nullptr;
BaseShader *Material::m_pSkinnedAnimationShader = nullptr;

void Material::PrepareShaders(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, ID3D12RootSignature * pd3dGraphicsRootSignature)
{	
	m_pWireFrameShader = new TestColorShader();
	m_pWireFrameShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	m_pWireFrameShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pSkinnedAnimationWireFrameShader = new SkinnedAnimationTestColorShader();
	m_pSkinnedAnimationWireFrameShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	m_pSkinnedAnimationWireFrameShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pStandardShader = new StandardLightingShader();
	m_pStandardShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	m_pStandardShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pSkinnedAnimationShader = new SkinnedAnimationShader();
	m_pSkinnedAnimationShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	m_pSkinnedAnimationShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
}


void Material::LoadTextureFromFile(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList, UINT nType, UINT nRootParameter, _TCHAR * pwstrTextureName, Texture ** ppTexture, SkinnedFrameObject * pParent, FILE * pInFile, BaseShader * pShader)
{
	
	char pstrTextureName[64] = { '\0' };

	BYTE nStrLength = 64;
	UINT nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
	nReads = (UINT)::fread(pstrTextureName, sizeof(char), nStrLength, pInFile);
	pstrTextureName[nStrLength] = '\0';

	bool bDuplicated = false;
	if (strcmp(pstrTextureName, "null"))
	{
		SetMaterialType(nType);

		char pstrFilePath[64] = { '\0' };
		strcpy_s(pstrFilePath, 64, "Model/Textures/");

		bDuplicated = (pstrTextureName[0] == '@');
		strcpy_s(pstrFilePath + 15, 64 - 15, (bDuplicated) ? (pstrTextureName + 1) : pstrTextureName);
		strcpy_s(pstrFilePath + 15 + ((bDuplicated) ? (nStrLength - 1) : nStrLength), 64 - 15 - ((bDuplicated) ? (nStrLength - 1) : nStrLength), ".dds");

		size_t nConverted = 0;
		mbstowcs_s(&nConverted, pwstrTextureName, 64, pstrFilePath, _TRUNCATE);

#define _WITH_DISPLAY_TEXTURE_NAME

#ifdef _WITH_DISPLAY_TEXTURE_NAME
		static int nTextures = 0, nRepeatedTextures = 0;
		TCHAR pstrDebug[256] = { 0 };
		_stprintf_s(pstrDebug, 256, _T("Texture Name: %d %c %s\n"), (pstrTextureName[0] == '@') ? nRepeatedTextures++ : nTextures++, (pstrTextureName[0] == '@') ? '@' : ' ', pwstrTextureName);
		OutputDebugString(pstrDebug);
#endif
		if (!bDuplicated)
		{
			*ppTexture = new Texture(1, RESOURCE_TEXTURE2D, 0);
			(*ppTexture)->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pwstrTextureName, 0, true);
			if (*ppTexture) (*ppTexture)->AddRef();

			//if (pShader) 
			BaseScene::CreateShaderResourceViews(pd3dDevice,pd3dCommandList, *ppTexture, nRootParameter, false);
		}
		else
		{
			if (pParent)
			{
				while (pParent)
				{
					if (!pParent->m_pParent) break;
					pParent = pParent->m_pParent;
				}
				SkinnedFrameObject *pRootGameObject = pParent;
				*ppTexture = pRootGameObject->FindReplicatedTexture(pwstrTextureName);
				if (*ppTexture) (*ppTexture)->AddRef();
			}
		}
	}
}
