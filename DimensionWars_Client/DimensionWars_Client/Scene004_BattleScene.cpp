#include "stdafx.h"
#include "RuntimeFrameWork.h"
#include "Scene004_BattleScene.h"
#include "Object003_SkyBox.h"
#include "Object101_GrimReaperPlayer.h"
#include "Object102_GamblerPlayer.h"
#include "Object103_ElfArcherPlayer.h"
#include "Camera000_BaseCamera.h"
#include "Material.h"
#include "Object007_DiffuseCubeObject.h"
#include "Object008_HeightmapTerrain.h"
#include "ResourceManager.h"
#include "AnimationController.h"	// �ε�� ����

BattleScene::BattleScene()
{

}


BattleScene::~BattleScene()
{
}

void BattleScene::SendChracterType(int a)
{
	if (a == 0 || a == 1 || a == 2) {
		CSPacket_CharacterType *myMovePacket = reinterpret_cast<CSPacket_CharacterType *>(m_pFramework->GetSendBuf());
		myMovePacket->size = sizeof(CSPacket_CharacterType);
		// Ŭ���̾�Ʈ�� ��� �������� �� �� Ű�Է� ������ ������ ��Ʈ�� ������ ������
		myMovePacket->character_type = a;
		myMovePacket->type = CS_Type::Character_Info;
		m_pFramework->SendPacket(reinterpret_cast<char *>(myMovePacket));
	}
}

void BattleScene::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{
	m_pGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 2, 45); //SuperCobra(17), Gunship(2), Player:Mi24(1), Angrybot()

	Material::PrepareShaders(pd3dDevice, pd3dCommandList, m_pGraphicsRootSignature);

	BuildLightsAndMaterials();
	
	XMFLOAT3 xmf3Scale(24.0f, 6.0f, 24.0f);
	XMFLOAT4 xmf4Color(0.1f, 0.1f, 0.1f, 0.5f);
	//m_pTerrain = new HeightMapTerrain(pd3dDevice, pd3dCommandList, m_pGraphicsRootSignature, _T("Texture/HeightMap.raw"), 257, 257, xmf3Scale, xmf4Color);
	//m_pTerrain->SetPosition(-3072.0f, 0.0f, -3072.0f);

	m_pFramework->GetResource()->AllModelLoad(pd3dDevice, pd3dCommandList, m_pGraphicsRootSignature);

	cmd = 0;
	printf("<ĳ���� ���� >\n�÷��̾� ĳ���� ������ ���� Ŀ�ǵ带 �Է��ϼ���. ( ��� : 0, ���ڻ� : 1 ) >>>  ");
	scanf_s("%d", &cmd);
	switch (cmd) {
	case 0: 
	{
		GrimReaperPlayer *pPlayer = new GrimReaperPlayer(pd3dDevice, pd3dCommandList, m_pGraphicsRootSignature, m_pTerrain, m_pFramework);
		m_pPlayer = pPlayer;
		for (int i = 0; i < MAX_PLAYER; ++i) {
			GamblerObject[i] = new GamblerPlayer(pd3dDevice, pd3dCommandList, m_pGraphicsRootSignature, m_pTerrain, m_pFramework);
			m_ppOtherPlayers[i] = GamblerObject[i];
			m_ppOtherPlayers[i]->SetPosition(XMFLOAT3(-100000.0f, -100000.0f, -100000.0f));// ��ġ �ʱ�ȭ�� �ϱ� �ؾߵǴϱ� ���� �� �׷��� ������ ¯�ھƵ���.
		}
	}
		break;
	case 1:
	{
		GamblerPlayer *pPlayer = new GamblerPlayer(pd3dDevice, pd3dCommandList, m_pGraphicsRootSignature, m_pTerrain, m_pFramework);
		m_pPlayer = pPlayer;
		for (int i = 0; i < MAX_PLAYER; ++i) {
			ReaperObject[i] = new GrimReaperPlayer(pd3dDevice, pd3dCommandList, m_pGraphicsRootSignature, m_pTerrain, m_pFramework);
			m_ppOtherPlayers[i] = ReaperObject[i];
			m_ppOtherPlayers[i]->SetPosition(XMFLOAT3(-100000.0f, -100000.0f, -100000.0f));// ��ġ �ʱ�ȭ�� �ϱ� �ؾߵǴϱ� ���� �� �׷��� ������ ¯�ھƵ���.
		}
	}
		break;
	default:
	{
		ElfArcherPlayer *pPlayer = new ElfArcherPlayer(pd3dDevice, pd3dCommandList, m_pGraphicsRootSignature, m_pTerrain, m_pFramework);
		m_pPlayer = pPlayer;
	}
		break;
	}
	
	m_nCubeObjects = 50;
	m_ppCubeObjects = new DiffuseCubeObject*[m_nCubeObjects];
	for (unsigned int i = 0; i < m_nCubeObjects; ++i) {
		if (i < 5) m_pFramework->cubeSize[i] = MAX_CUBE_SIZE - 400;
		else if (i < 10) m_pFramework->cubeSize[i] = MAX_CUBE_SIZE - 300;
		else if (i < 20) m_pFramework->cubeSize[i] = MAX_CUBE_SIZE - 200;
		else if (i < 30) m_pFramework->cubeSize[i] = MAX_CUBE_SIZE - 100;
		else if (i < 50) m_pFramework->cubeSize[i] = MAX_CUBE_SIZE;
		m_ppCubeObjects[i] = new DiffuseCubeObject(pd3dDevice, pd3dCommandList, m_pGraphicsRootSignature, m_pFramework->cubeSize[i]);
	}

	m_pSkyBox = new SkyBox(pd3dDevice, pd3dCommandList, m_pGraphicsRootSignature);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void BattleScene::ReleaseObjects()
{
	if (m_pGraphicsRootSignature) m_pGraphicsRootSignature->Release();

	if (m_pSkyBox) delete m_pSkyBox;

	ReleaseShaderVariables();
}

void BattleScene::ReleaseUploadBuffers()
{
	if (m_pSkyBox) m_pSkyBox->ReleaseUploadBuffers();

	if (m_pTerrain) m_pTerrain->ReleaseUploadBuffers();
}

bool BattleScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pPlayer) m_pPlayer->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:

		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:

		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
	return false;
}

bool BattleScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pPlayer) m_pPlayer->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);

	return false;
}

bool BattleScene::ProcessInput(UCHAR * pKeysBuffer, float fTimeElapsed)
{
	if (m_pPlayer) m_pPlayer->ProcessInput(pKeysBuffer, fTimeElapsed);
	return false;
}

void BattleScene::AnimateObjects(float fTimeElapsed)
{
	
	SendMoveDirection();
	SendAnimationInfo();
	
	

	if (m_pPlayer) m_pPlayer->Animate(fTimeElapsed);

	for (int i = 0; i < MAX_PLAYER; ++i)
		if (m_ppOtherPlayers[i]->connected) {
			m_ppOtherPlayers[i]->Animate(fTimeElapsed);
			
		}
	/*for (int i = 0; i < m_nCubeObjects; ++i) {
		m_ppCubeObjects[i]->SetPosition(m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y - 60, m_pPlayer->GetPosition().z - 50);
	}*/
}

void BattleScene::Render(ID3D12GraphicsCommandList * pd3dCommandList, BaseCamera * pCamera)
{
	SetGraphicsRootSignature(pd3dCommandList);
	if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	if (m_pSkyBox) m_pSkyBox->Render(pd3dCommandList, pCamera);

	if (m_pPlayer) m_pPlayer->Render(pd3dCommandList, pCamera);
	
	for (int i = 0; i < MAX_PLAYER; ++i)
		if (m_ppOtherPlayers[i]->connected) 
			m_ppOtherPlayers[i]->Render(pd3dCommandList, pCamera);


	for (unsigned int i = 0; i < m_nCubeObjects; ++i)
		if (m_ppCubeObjects && m_ppCubeObjects[i])
			m_ppCubeObjects[i]->Render(pd3dCommandList, pCamera);

	if (m_pTerrain) m_pTerrain->Render(pd3dCommandList, pCamera);
}

void BattleScene::BuildCube()
{
	if (!isBuilded) {
		for (unsigned int i = 0; i < m_nCubeObjects; ++i) {
#ifdef USE_CONSOLE_WINDOW
			// printf("UP Cube [%d] %.2f -  Pos : (%.2f, %.2f, %.2f) / Rot : (%.2f, %.2f, %.2f)\n", i, m_pFramework->cubeSize[i], m_pFramework->cubePos[i].x, m_pFramework->cubePos[i].y, m_pFramework->cubePos[i].z, m_pFramework->cubeRot[i].x, m_pFramework->cubeRot[i].y, m_pFramework->cubeRot[i].z);
#endif
			m_ppCubeObjects[i]->SetPosition(m_pFramework->cubePos[i].x, m_pFramework->cubePos[i].y, m_pFramework->cubePos[i].z);
			//m_ppCubeObjects[i]->SetPosition(m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y, m_pPlayer->GetPosition().z);
			m_ppCubeObjects[i]->Rotate(m_pFramework->cubeRot[i].x, m_pFramework->cubeRot[i].y, m_pFramework->cubeRot[i].z);
		}
		isBuilded = true;
	}
}

void BattleScene::ProcessPacket(char * ptr)
{
	static bool first_time = true;
	
	switch (ptr[1])
	{
	case SC_Type::LoginOK:
	{
		//printf("login\n");
		SCPacket_LoginOK *packet = reinterpret_cast<SCPacket_LoginOK *>(ptr);
		myid = packet->id;
#ifdef USE_CONSOLE_WINDOW
		printf("LOGIN\n");
#endif
		break;
	}
	case SC_Type::PutPlayer:
	{
#ifdef USE_CONSOLE_WINDOW
		printf("Put Player\n");
#endif
		SCPacket_PutPlayer *my_packet = reinterpret_cast<SCPacket_PutPlayer *>(ptr);
		unsigned int id = my_packet->id;
		if (first_time) {
			first_time = false;
			myid = id;
		}
		if (id == myid) {
			m_pPlayer->SetVisible(true);
			m_pPlayer->SetPosition((XMFLOAT3(my_packet->position.x, my_packet->position.y, my_packet->position.z)));
			
			CSPacket_CharacterType *myTypePacket = reinterpret_cast<CSPacket_CharacterType *>(m_pFramework->GetSendBuf());
			myTypePacket->size = sizeof(CSPacket_CharacterType);
			// Ŭ���̾�Ʈ�� ��� �������� �� �� Ű�Է� ������ ������ ��Ʈ�� ������ ������
			myTypePacket->character_type = cmd;
			myTypePacket->type = CS_Type::Character_Info;
			m_pFramework->SendPacket(reinterpret_cast<char *>(myTypePacket));
		}
		else if (id < MAX_PLAYER) {
			if (m_ppOtherPlayers[id]) {
				m_ppOtherPlayers[id]->connected = true;
				m_ppOtherPlayers[id]->SetPosition((XMFLOAT3(my_packet->position.x, my_packet->position.y, my_packet->position.z)));
			}

		}
		
		break;
	}
	case SC_Type::Position:
	{
		SCPacket_Position *my_packet = reinterpret_cast<SCPacket_Position *>(ptr);
		unsigned short other_id = my_packet->id;
		anime = my_packet->animation_state;
		//printf("%d", anime);
	//	printf("������! �������� �ޱ� ����\n");
		if (other_id == myid) {
			m_pPlayer->SetVisible(true);
			//printf("Your [%d] : (%.1f, %.1f, %.1f)\n", my_packet->id, my_packet->position.x, my_packet->position.y, my_packet->position.z);
			m_pPlayer->SetPosition((XMFLOAT3(my_packet->position.x, my_packet->position.y, my_packet->position.z)));
			
		}
		else if (other_id < MAX_PLAYER) {
			m_ppOtherPlayers[other_id]->m_pSkinnedAnimationController->SetAnimationSet(anime);
			m_ppOtherPlayers[other_id]->SetPosition((XMFLOAT3(my_packet->position.x, my_packet->position.y, my_packet->position.z)));
//#ifdef USE_CONSOLE_WINDOW
		}
		else if (other_id>=Card_start&&other_id<Card_end) {
			//printf("���ڻ� ��Ÿ : %1.f, %1.f, %1.f\n", my_packet->position.x, my_packet->position.y, my_packet->position.z);
			// �׷��ֽð� ��ġ ����
		}
		else if (other_id >= Slash_start&&other_id < Slash_end) {
			//printf("�˱� : %1.f, %1.f, %1.f\n", my_packet->position.x, my_packet->position.y, my_packet->position.z);
			// �׷��ֽð� ��ġ ����
		}
		break;
	}
	case SC_Type::RemovePlayer:
	{
#ifdef USE_CONSOLE_WINDOW
		printf("Remove\n");
#endif
//		SCPacket_RemovePlayer *my_packet = reinterpret_cast<SCPacket_RemovePlayer *>(ptr);
//		unsigned int other_id = my_packet->id;
//		if (other_id == myid) m_pPlayer->SetVisible(false);
//		if (other_id < MAX_USER) {
//			if (m_ppOtherPlayers[other_id]) {
//				//m_ppOtherPlayers[other_id]->SetVisible(false);
//				delete m_ppOtherPlayers[other_id];
//				m_ppOtherPlayers[other_id] = nullptr;
//#ifdef USE_CONSOLE_WINDOW
//				printf("Player [%d] Remove from Screen\n", my_packet->id);
//#endif
//			}
//		}
		
		break;
	}
	case SC_Type::Animation:
	{
		SCPacket_Animation *my_packet = reinterpret_cast<SCPacket_Animation*>(ptr);
		unsigned short other_id = my_packet->id;
		anime = my_packet->animation_state;

		if (other_id < MAX_PLAYER) {
			m_ppOtherPlayers[other_id]->m_pSkinnedAnimationController->SetAnimationSet(anime);
			//printf("�������� ���� : %d\n", anime);
		}
		//printf("����! �������� �ޱ� ����\n");
		break;
	}
	case SC_Type::MapInfo:
	{
		//ó��
		SCPacket_MapInfo *my_packet = reinterpret_cast<SCPacket_MapInfo *>(ptr);

		unsigned short id = my_packet->id - Cube_start;
		m_pFramework->cubePos[id] = XMFLOAT3(my_packet->position.x, my_packet->position.y, my_packet->position.z);
		m_pFramework->cubeRot[id] = XMFLOAT3(my_packet->rotate.x, my_packet->rotate.y, my_packet->rotate.z);
		break;
	}
	case SC_Type::ProjectTile:
	{
		
		SCPacket_ProjectTile *my_packet = reinterpret_cast<SCPacket_ProjectTile *>(ptr);
		if (my_packet->projectTile_type == ProjectTile::Card)
		{
			//printf("card (%.1f, %.1f, %.1f\n", my_packet->position.x, my_packet->position.y, my_packet->position.z);
			unsigned short card_id = my_packet->id;
			//my_packet->position = m_pPlayer[myid].GetPosition();
		}
		if (my_packet->projectTile_type == ProjectTile::Slash)
		{
			//printf("slash (%.1f, %.1f, %.1f\n", my_packet->position.x, my_packet->position.y, my_packet->position.z);
			unsigned short slash_id = my_packet->id;
			//my_packet->position = m_pPlayer[myid].GetPosition();
		}
		break;
	}
	default:
#ifdef USE_CONSOLE_WINDOW
		printf("Unknown PACKET type [%d]\n", ptr[1]);
#endif
		break;
	}


	
}
