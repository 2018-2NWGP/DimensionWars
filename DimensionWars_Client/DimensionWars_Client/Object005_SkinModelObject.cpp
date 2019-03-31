#include "stdafx.h"
#include "Object005_SkinModelObject.h"
#include "AnimationController.h"


SkinModelObject::SkinModelObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, LoadedModelInfo *pModel, char* pFilePathName, int nAnimationTracks)
{
	LoadedModelInfo *pSkinnedModel = pModel;
	if (!pSkinnedModel) pSkinnedModel = SkinnedFrameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pFilePathName, NULL);

	SetChild(pSkinnedModel->m_pModelRootObject, true);
	m_pSkinnedAnimationController = new AnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pSkinnedModel);

	Rotate(-90.0f, 0.0f, 0.0f);	// 3Ds Max�� ��ǥ�踦 DirectX ��ǥ��� ��ȯ�ϱ� ���� �۾�
	SetScale(1.0f, 1.0f, 1.0f);
}


SkinModelObject::~SkinModelObject()
{
}
