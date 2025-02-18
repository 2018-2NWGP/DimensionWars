#include "stdafx.h"
#include "RuntimeFrameWork.h"
#include "Texture.h"
#include "Material.h"
#include "ResourceManager.h"
#include "AnimationController.h"	// 로드모델 때문

#include "Scene004_BattleScene.h"

#include "Camera000_BaseCamera.h"

#include "Object003_SkyBox.h"
#include "Object006_TextureRectObject.h"
#include "Object007_DiffuseCubeObject.h"
#include "Object008_HeightmapTerrain.h"
#include "Object010_SlashWaveObject.h"
#include "Object011_CardObject.h"
#include "Object012_PortalObject.h"
#include "Object013_ArrowObject.h"
#include "Object014_ScreenTextureObject.h"
#include "Object101_GrimReaperPlayer.h"
#include "Object102_GamblerPlayer.h"
#include "Object103_ElfArcherPlayer.h"
#include "Object104_DummyPlayer.h"

#include "Shader005_TextureRectangleShader.h"
#include "Shader009_UIShader.h"



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
		// 클라이언트가 어느 방향으로 갈 지 키입력 정보를 저장한 비트를 서버로 보내기
		myMovePacket->character_type = a;
		myMovePacket->type = CS_Type::Character_Info;
		m_pFramework->SendPacket(reinterpret_cast<char *>(myMovePacket));
	}
}

void BattleScene::BuildObjects(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCommandList)
{

	//m_pFramework->m_pGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	//CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 2, 45); //SuperCobra(17), Gunship(2), Player:Mi24(1), Angrybot()

	Material::PrepareShaders(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature);

	BuildLightsAndMaterials();
	
	XMFLOAT3 xmf3Scale(24.0f, 6.0f, 24.0f);
	XMFLOAT4 xmf4Color(0.1f, 0.1f, 0.1f, 0.5f);
	//m_pTerrain = new HeightMapTerrain(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, _T("Texture/HeightMap.raw"), 257, 257, xmf3Scale, xmf4Color);
	//m_pTerrain->SetPosition(-3072.0f, 0.0f, -3072.0f);
	
	m_pFramework->GetResource()->AllModelLoad(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature);

	m_pSkyBox = new SkyBox(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature);
	
	GrimReaperPlayer *pPlayer = new GrimReaperPlayer(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, m_pTerrain, m_pFramework);
	m_pPlayer = pPlayer;

	for (int i = 0; i < MAX_PLAYER; ++i){
		Dummy[i] = new GamblerPlayer(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, m_pTerrain, m_pFramework, i);
		m_ppOtherPlayers[i] = Dummy[i];
		m_ppOtherPlayers[i]->SetPosition(XMFLOAT3(-100000.0f, -100000.0f, -100000.0f));// 위치 초기화를 하긴 해야되니까 절대 안 그려질 곳에다 짱박아두자.
	}
	
	for (int i = 0; i < MAX_PLAYER; ++i){
		ReaperObject[i] = new GrimReaperPlayer(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, m_pTerrain, m_pFramework, i);
		ReaperObject[i]->SetPosition(XMFLOAT3(-100000.0f, -100000.0f, -100000.0f));
		// ReaperObject[i]->SetChild(m_pFramework->GetResource()->GetGrimReaperModel(i)->m_pModelRootObject, true);
	}

	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		GamblerObject[i] = new GamblerPlayer(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, m_pTerrain, m_pFramework, i);
		GamblerObject[i]->SetPosition(XMFLOAT3(-100000.0f, -100000.0f, -100000.0f));
		// GamblerObject[i]->SetChild(m_pFramework->GetResource()->GetGamblerModel(i)->m_pModelRootObject, true);
	}
	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		ElfObject[i] = new ElfArcherPlayer(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, m_pTerrain, m_pFramework, i);
		//ElfObject[i]->SetChild(m_pFramework->GetResource()->GetElfArcherModel(i)->m_pModelRootObject, true);
		ElfObject[i]->SetPosition(XMFLOAT3(-100000.0f, -100000.0f, -100000.0f));
	}

	//cmd = 0;
	////ConsolePrint("<캐릭터 선택 >\n플레이어 캐릭터 선택을 위해 커맨드를 입력하세요. ( 사신 : 0, 도박사 : 1 ) >>>  ");
	////scanf_s("%d", &cmd);
	//switch (cmd) {
	//case 0: 
	//{
	//	GrimReaperPlayer *pPlayer = new GrimReaperPlayer(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, m_pTerrain, m_pFramework);
	//	m_pPlayer = pPlayer;
	//	for (int i = 0; i < MAX_PLAYER; ++i) {
	//		GamblerObject[i] = new GamblerPlayer(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, m_pTerrain, m_pFramework);
	//		m_ppOtherPlayers[i] = ElfObject[i];
	//		m_ppOtherPlayers[i]->SetPosition(XMFLOAT3(-100000.0f, -100000.0f, -100000.0f));// 위치 초기화를 하긴 해야되니까 절대 안 그려질 곳에다 짱박아두자.
	//	}
	//}
	//	break;
	//case 1:
	//{
	//	GamblerPlayer *pPlayer = new GamblerPlayer(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, m_pTerrain, m_pFramework);
	//	m_pPlayer = pPlayer;
	//	for (int i = 0; i < MAX_PLAYER; ++i) {
	//		ReaperObject[i] = new GrimReaperPlayer(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, m_pTerrain, m_pFramework);
	//		m_ppOtherPlayers[i] = ReaperObject[i];
	//		m_ppOtherPlayers[i]->SetPosition(XMFLOAT3(-100000.0f, -100000.0f, -100000.0f));// 위치 초기화를 하긴 해야되니까 절대 안 그려질 곳에다 짱박아두자.
	//	}
	//}
	//	break;
	//default:
	//{
	//	ElfArcherPlayer *pPlayer = new ElfArcherPlayer(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, m_pTerrain, m_pFramework);
	//	m_pPlayer = pPlayer;
	//	for (int i = 0; i < MAX_PLAYER; ++i) {
	//		ReaperObject[i] = new GrimReaperPlayer(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, m_pTerrain, m_pFramework);
	//		m_ppOtherPlayers[i] = ReaperObject[i];
	//		m_ppOtherPlayers[i]->SetPosition(XMFLOAT3(-100000.0f, -100000.0f, -100000.0f));// 위치 초기화를 하긴 해야되니까 절대 안 그려질 곳에다 짱박아두자.
	//	}
	//}
	//	break;
	//}
	
	m_nCubeObjects = 50;
	m_ppCubeObjects = new TextureCubeObject*[m_nCubeObjects];
	
	for (unsigned int i = 0; i < m_nCubeObjects; ++i) {
		if (i < 5) m_ppCubeObjects[i] = new TextureCubeObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, 300);
		else if (i < 15) m_ppCubeObjects[i] = new TextureCubeObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, 400);
		else if (i < 35) m_ppCubeObjects[i] = new TextureCubeObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, 500);
		else if (i < 45) m_ppCubeObjects[i] = new TextureCubeObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, 600);
		else if (i < 50) m_ppCubeObjects[i] = new TextureCubeObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, 700);
	}
	
	slashWave = new SlashWaveObject*[Slash_end - Slash_start];
	for (unsigned int i = 0; i < Slash_end - Slash_start; ++i) {
		slashWave[i] = new SlashWaveObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, m_pTerrain, m_pFramework, i);
		// slashWave[i]->SetChild(m_pFramework->GetResource()->GetSlashWaveModel(i)->m_pModelRootObject, true);
	}

	arrow = new ArrowObject*[Arrow_end - Arrow_start];
	for (unsigned int i = 0; i < Arrow_end - Arrow_start; ++i) {
		arrow[i] = new ArrowObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, m_pTerrain, m_pFramework, i);
		// arrow[i]->SetChild(m_pFramework->GetResource()->GetArrowObject(i), true);
	}

	
	card = new CardObject*[Card_end - Card_start];
	for (unsigned int i = 0; i < Card_end - Card_start; ++i) {
		card[i] = new CardObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, m_pTerrain, m_pFramework, i);
		// card[i]->SetChild(m_pFramework->GetResource()->GetCardObject(i), true);
	}

	m_ppPotalObjects = new TexturePortalObject*[Potal_end - Potal_start];
	for (unsigned int i = 0; i < Potal_end - Potal_start; ++i) {
		m_ppPotalObjects[i] = new TexturePortalObject(pd3dDevice, pd3dCommandList,m_pFramework->m_pGraphicsRootSignature, 30);
	}

	
	Texture *battleImage[ui_texture];

	//0
	battleImage[0] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/W0.dds", 0);
	//1
	battleImage[1] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/W1.dds", 0);
	//2
	battleImage[2] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[2]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/W2.dds", 0);
	//3
	battleImage[3] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[3]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/W3.dds", 0);
	//4
	battleImage[4] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[4]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/W4.dds", 0);
	//5
	battleImage[5] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[5]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/W5.dds", 0);
	//6
	battleImage[6] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[6]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/W6.dds", 0);
	//7
	battleImage[7] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[7]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/W7.dds", 0);
	//8
	battleImage[8] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[8]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/W8.dds", 0);
	//9
	battleImage[9] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[9]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/W9.dds", 0);
	// hp
	battleImage[10] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[10]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/HPBar_360x60.dds", 0);
	// sp
	battleImage[11] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[11]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/SPBar_360x60.dds", 0);
	// 레이더
	battleImage[12] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[12]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/Rada.dds", 0);
	// 점수표
	battleImage[13] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[13]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/Score_chart.dds", 0);
	// :
	battleImage[14] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[14]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/Wcolon.dds", 0);
	// 빈 sp
	battleImage[15] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[15]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/EmptyBar_360x60.dds", 0);

	battleImage[16] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[16]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/Win.dds", 0);

	battleImage[17] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[17]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/Lose.dds", 0);

	battleImage[18] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[18]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/Y1.dds", 0);
	//8
	battleImage[19] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[19]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/Y2.dds", 0);
	//9
	battleImage[20] = new Texture(1, RESOURCE_TEXTURE2D, 0);
	battleImage[20]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Texture/Battle/Y3.dds", 0);
	


	//TextureRectangleShader *pTextureShader = new TextureRectangleShader();
	UIShader *pTextureShader = new UIShader();
	pTextureShader->CreateShader(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature);
	pTextureShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	for (int i = 0; i < ui_texture; ++i)
	{
		CreateShaderResourceViews(pd3dDevice, pd3dCommandList, battleImage[i], 15, true);
	}


	Material *battleMaterial[ui_texture];

	for (int i = 0; i < ui_texture; ++i)
	{
		battleMaterial[i] = new Material(1);
		battleMaterial[i]->SetTexture(battleImage[i]);
		battleMaterial[i]->SetShader(pTextureShader);

	}

	m_nObjects = 96; //텍스쳐 로드는 45개 
	m_battleObjects = new BaseObject*[m_nObjects];

	// 202x290 -> 101 145
	// 1초단위 숫자 10개 
	for (int i = 0; i < 9; ++i)
	{
		ScreenTextureObject *battleImageObject = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature,0.1f,0.75f,0.3f,0.55f);
		m_battleObjects[i] = battleImageObject;
		m_battleObjects[i]->SetMaterial(0, battleMaterial[i]);
	}
	// 10초단위 숫자 6개 
	for (int i = 9; i < 15; ++i)
	{
		ScreenTextureObject *battleImageObject1 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, 0.0f, 0.75f, 0.2f, 0.55f);
		m_battleObjects[i] = battleImageObject1;
		m_battleObjects[i]->SetMaterial(0, battleMaterial[i-9]);
	}
	// 1분단위 숫자 6개 
	for (int i = 15; i < 21; ++i)
	{
		ScreenTextureObject *battleImageObject2 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, -0.2f, 0.75f, 0.0f, 0.55f);
		m_battleObjects[i] = battleImageObject2;
		m_battleObjects[i]->SetMaterial(0, battleMaterial[i - 15]);
	}
	// hp
	ScreenTextureObject *battleImageObject3 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature,0,0,0,0);
	m_battleObjects[21] = battleImageObject3;
	m_battleObjects[21]->SetMaterial(0, battleMaterial[10]);
	// sp
	ScreenTextureObject *battleImageObject4 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, 0,0,0,0);
	m_battleObjects[22] = battleImageObject4;
	m_battleObjects[22]->SetMaterial(0, battleMaterial[11]);
	// 레이더
	ScreenTextureObject *battleImageObject5 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, 0.7f, 1.0f, 1.0f, 0.6f);
	m_battleObjects[23] = battleImageObject5;
	m_battleObjects[23]->SetMaterial(0, battleMaterial[12]);
	// 점수표
	ScreenTextureObject *battleImageObject6 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, -0.25f, 0.95f, 0.25f, 0.75f);
	m_battleObjects[24] = battleImageObject6;
	m_battleObjects[24]->SetMaterial(0, battleMaterial[13]);
	// : 184x289
	ScreenTextureObject *battleImageObject7 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, -0.092f, 0.7445f, 0.092f, 0.6f);
	m_battleObjects[25] = battleImageObject7;
	m_battleObjects[25]->SetMaterial(0, battleMaterial[14]);
	// 빈 hp
	ScreenTextureObject *battleImageObject11 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, -0.98f, 0.955f, -0.42f, 0.845f); //-0.98f, 0.955f, -0.42f, 0.845f
	m_battleObjects[26] = battleImageObject11;
	m_battleObjects[26]->SetMaterial(0, battleMaterial[15]);
	// 빈 sp
	ScreenTextureObject *battleImageObject12 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, -0.98f, 0.805f, -0.42f, 0.695f);
	m_battleObjects[27] = battleImageObject12;
	m_battleObjects[27]->SetMaterial(0, battleMaterial[15]);
	// 승리화면
	ScreenTextureObject *battleImageObject13 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, -0.5f, 0.5f, 0.5f, -0.5f);
	m_battleObjects[28] = battleImageObject13;
	m_battleObjects[28]->SetMaterial(0, battleMaterial[16]);
	// 패패 화면
	ScreenTextureObject *battleImageObject14 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, -0.5f, 0.5f, 0.5f, -0.5f);
	m_battleObjects[29] = battleImageObject14;
	m_battleObjects[29]->SetMaterial(0, battleMaterial[17]);
	// 노란 숫자
	ScreenTextureObject *battleImageObject15 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, -0.20f, 0.95f, -0.03f, 0.75f);
	m_battleObjects[30] = battleImageObject15;
	m_battleObjects[30]->SetMaterial(0, battleMaterial[18]);

	ScreenTextureObject *battleImageObject16 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, -0.20f, 0.95f, -0.03f, 0.75f);
	m_battleObjects[31] = battleImageObject16;
	m_battleObjects[31]->SetMaterial(0, battleMaterial[19]);

	ScreenTextureObject *battleImageObject17 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, -0.20f, 0.95f, -0.03f, 0.75f);
	m_battleObjects[32] = battleImageObject17;
	m_battleObjects[32]->SetMaterial(0, battleMaterial[20]);

	ScreenTextureObject *battleImageObject18 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, 0.03f, 0.95f, 0.20f, 0.75f);
	m_battleObjects[33] = battleImageObject18;
	m_battleObjects[33]->SetMaterial(0, battleMaterial[18]);

	ScreenTextureObject *battleImageObject19 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, 0.03f, 0.95f, 0.20f, 0.75f);
	m_battleObjects[34] = battleImageObject19;
	m_battleObjects[34]->SetMaterial(0, battleMaterial[19]);

	ScreenTextureObject *battleImageObject20 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, 0.03f, 0.95f, 0.20f, 0.75f);
	m_battleObjects[35] = battleImageObject20;
	m_battleObjects[35]->SetMaterial(0, battleMaterial[20]);

	for (int i = 0; i < 10; ++i)
	{
		ScreenTextureObject *battleImageObject21 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, -0.95f+0.05f*i, 0.95f, -0.90f + 0.05f*i, 0.85f);
		m_battleObjects[36+i] = battleImageObject21;
		m_battleObjects[36+i]->SetMaterial(0, battleMaterial[10]);
	}

	for (int i = 0; i < 50; ++i)
	{
		ScreenTextureObject *battleImageObject22 = new ScreenTextureObject(pd3dDevice, pd3dCommandList, m_pFramework->m_pGraphicsRootSignature, -0.95f + 0.01f*i, 0.8f, -0.94f + 0.01f*i, 0.7f);
		m_battleObjects[46 + i] = battleImageObject22;
		m_battleObjects[46 + i]->SetMaterial(0, battleMaterial[11]);
	}


	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void BattleScene::ReleaseObjects()
{
	if (m_pFramework->m_pGraphicsRootSignature) m_pFramework->m_pGraphicsRootSignature->Release();

	if (m_pSkyBox) delete m_pSkyBox;

	ReleaseShaderVariables();
}

void BattleScene::ReleaseUploadBuffers()
{
	if (m_pSkyBox) m_pSkyBox->ReleaseUploadBuffers();

	if (m_pTerrain) m_pTerrain->ReleaseUploadBuffers();

	if (m_battleObjects) {
		for (unsigned int i = 0; i < m_nObjects ; ++i)
			if (m_battleObjects[i])
				m_battleObjects[i]->ReleaseUploadBuffers();
	}
}

bool BattleScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pFramework->GetActivated() && m_pPlayer) m_pPlayer->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);

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
	if (m_pFramework->GetActivated() && m_pPlayer) m_pPlayer->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID)
	{
	case WM_KEYDOWN:
		// 키보드를 누르고 있을 경우 최초 한번만 실행.
		if ((lParam & 0x40000000) != 0x40000000) {
			switch (wParam) {
			case VK_RETURN:
				if (m_pPlayer->death_count == MAX_DEATH)
				{
					m_pFramework->ChangeScene(BaseScene::SceneTag::Room);
				}
				for (int i = 0; i < MAX_PLAYER; ++i)
				{
					if (m_ppOtherPlayers[i]->connected)
					{
						if (m_ppOtherPlayers[i]->death_count == MAX_DEATH)
						{
							m_pFramework->ChangeScene(BaseScene::SceneTag::Room);
						}
					}
				}
				break;
				/*case 'O':
					m_pPlayer->GetCamera()->SetPosition(XMFLOAT3(0, 0, -10000));
					m_pPlayer->GetCamera()->OrthoMatrix(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
					break;
				case 'P':
					m_pPlayer->GetCamera()->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
					break;*/
			case 'P':
				m_pPlayer->sp -= 1;
				if (m_pPlayer->sp <= 0)
				{
					m_pPlayer->sp = 50;
				}
				break; 

			}


		}
		break;
	case WM_KEYUP:

		break;
	}
	return false;
}

bool BattleScene::ProcessInput(UCHAR * pKeysBuffer, float fTimeElapsed)
{
	if (m_pFramework->GetActivated() && m_pPlayer) {
		m_pPlayer->ProcessInput(pKeysBuffer, fTimeElapsed);
		return true;
	}
	else
		;
	return false;
}

void BattleScene::AnimateObjects(float fTimeElapsed)
{
	


	
	if (m_pFramework->GetActivated()) {
		SendMoveDirection();
		SendAnimationInfo();
	}
	
	
	if (cmd == 1) {
		//m_pPlayer->GetCamera()->SetOffset(XMFLOAT3(0, 150, -350));
		//m_pPlayer->GetCamera()->SetPosition(Vector3::Add(m_pPlayer->GetCamera()->GetPosition(), m_pPlayer->GetCamera()->GetOffset()));
	}
	
	
	BuildCube();
	
	if (m_pPlayer) m_pPlayer->Animate(fTimeElapsed);

	for (int i = 0; i < MAX_PLAYER; ++i)
		if (m_ppOtherPlayers[i]->connected)
			m_ppOtherPlayers[i]->Animate(fTimeElapsed);


	//slashWave[0]->SetPosition(m_pPlayer->GetPosition());


	for (unsigned int i = 0; i < Slash_end - Slash_start; ++i)
	{
		if (slashWave && slashWave[i])
		{
			slashWave[i]->m_xmf4x4ToParent._11 = m_pPlayer->GetCamera()->GetRightVector().x;
			slashWave[i]->m_xmf4x4ToParent._12 = m_pPlayer->GetCamera()->GetRightVector().y;
			slashWave[i]->m_xmf4x4ToParent._13 = m_pPlayer->GetCamera()->GetRightVector().z;
			slashWave[i]->m_xmf4x4ToParent._21 = m_pPlayer->GetCamera()->GetUpVector().x;
			slashWave[i]->m_xmf4x4ToParent._22 = m_pPlayer->GetCamera()->GetUpVector().y;
			slashWave[i]->m_xmf4x4ToParent._23 = m_pPlayer->GetCamera()->GetUpVector().z;
			slashWave[i]->m_xmf4x4ToParent._31 = m_pPlayer->GetCamera()->GetLookVector().x;
			slashWave[i]->m_xmf4x4ToParent._32 = m_pPlayer->GetCamera()->GetLookVector().y;
			slashWave[i]->m_xmf4x4ToParent._33 = m_pPlayer->GetCamera()->GetLookVector().z;
		}
	}
	for (unsigned int i = 0; i < Card_end - Card_start; ++i)
	{
		if (card && card[i]) {
			card[i]->m_xmf4x4ToParent._11 = m_pPlayer->GetCamera()->GetRightVector().x;
			card[i]->m_xmf4x4ToParent._12 = m_pPlayer->GetCamera()->GetRightVector().y;
			card[i]->m_xmf4x4ToParent._13 = m_pPlayer->GetCamera()->GetRightVector().z;
			card[i]->m_xmf4x4ToParent._21 = m_pPlayer->GetCamera()->GetUpVector().x;
			card[i]->m_xmf4x4ToParent._22 = m_pPlayer->GetCamera()->GetUpVector().y;
			card[i]->m_xmf4x4ToParent._23 = m_pPlayer->GetCamera()->GetUpVector().z;
			card[i]->m_xmf4x4ToParent._31 = m_pPlayer->GetCamera()->GetLookVector().x;
			card[i]->m_xmf4x4ToParent._32 = m_pPlayer->GetCamera()->GetLookVector().y;
			card[i]->m_xmf4x4ToParent._33 = m_pPlayer->GetCamera()->GetLookVector().z;
		}
	}


	for (unsigned int i = 0; i < Arrow_end - Arrow_start; ++i)
	{
		if (arrow && arrow[i]) {
			arrow[i]->m_xmf4x4ToParent._11 = m_pPlayer->GetCamera()->GetRightVector().x;
			arrow[i]->m_xmf4x4ToParent._12 = m_pPlayer->GetCamera()->GetRightVector().y;
			arrow[i]->m_xmf4x4ToParent._13 = m_pPlayer->GetCamera()->GetRightVector().z;
			arrow[i]->m_xmf4x4ToParent._21 = m_pPlayer->GetCamera()->GetUpVector().x;
			arrow[i]->m_xmf4x4ToParent._22 = m_pPlayer->GetCamera()->GetUpVector().y;
			arrow[i]->m_xmf4x4ToParent._23 = m_pPlayer->GetCamera()->GetUpVector().z;
			arrow[i]->m_xmf4x4ToParent._31 = m_pPlayer->GetCamera()->GetLookVector().x;
			arrow[i]->m_xmf4x4ToParent._32 = m_pPlayer->GetCamera()->GetLookVector().y;
			arrow[i]->m_xmf4x4ToParent._33 = m_pPlayer->GetCamera()->GetLookVector().z;
		}
	}
	
	
	/*for (int i = 0; i < m_nCubeObjects; ++i) {
		m_ppCubeObjects[i]->SetPosition(m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y - 60, m_pPlayer->GetPosition().z - 50);
	}*/

	// for (unsigned int i = 0; i < Slash_end - Slash_start; ++i) if (slashWave && slashWave[i]) slashWave[i]->Animate(fTimeElapsed);
	//for (unsigned int i = 0; i < Card_end - Card_start; ++i) if (card && card[i]) card[i]->Animate(fTimeElapsed);
	//slashWave[0]->SetPosition(m_pPlayer->GetPosition());
	
}

void BattleScene::Render(ID3D12GraphicsCommandList * pd3dCommandList, BaseCamera * pCamera)
{
	m_pFramework->SetGraphicsRootSignature(pd3dCommandList);
	if (m_pd3dCbvSrvDescriptorHeap) pd3dCommandList->SetDescriptorHeaps(1, &m_pd3dCbvSrvDescriptorHeap);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	if (m_pSkyBox) m_pSkyBox->Render(pd3dCommandList, pCamera);

	if (m_pPlayer) m_pPlayer->Render(pd3dCommandList, pCamera);
	
	for (int i = 0; i < MAX_PLAYER; ++i)
		if (m_ppOtherPlayers[i]->connected)
		{
			m_ppOtherPlayers[i]->Render(pd3dCommandList, pCamera);
		}

	for (unsigned int i = 0; i < m_nCubeObjects; ++i)
		if (m_ppCubeObjects && m_ppCubeObjects[i])
			m_ppCubeObjects[i]->Render(pd3dCommandList, pCamera);

	for (unsigned int i = 0; i < Slash_end - Slash_start; ++i) 
		if (slashWave && slashWave[i])
			slashWave[i]->Render(pd3dCommandList, pCamera);
	
	for (unsigned int i = 0; i < Card_end - Card_start; ++i) {
		if (card && card[i])
			card[i]->Render(pd3dCommandList, pCamera);
	}

	for (unsigned int i = 0; i < Arrow_end - Arrow_start; ++i)
		if (arrow && arrow[i])
			arrow[i]->Render(pd3dCommandList, pCamera);

	for (unsigned int i = 0; i < Potal_end - Potal_start; ++i) {
		if (m_ppPotalObjects && m_ppPotalObjects[i]) {
			m_ppPotalObjects[i]->Render(pd3dCommandList, pCamera);
			
		}

	}

	if (m_pTerrain) m_pTerrain->Render(pd3dCommandList, pCamera);


	for (int i = 0; i < 1; ++i)
	{
		m_battleObjects[i]->Render(pd3dCommandList, pCamera);
	}

	for (int i = 9; i < 10; ++i)
	{
		m_battleObjects[i]->Render(pd3dCommandList, pCamera);
	}
	// 분
	for (int i = 15; i < 16; ++i)
	{
		m_battleObjects[i]->Render(pd3dCommandList, pCamera);
	}


	if (m_pPlayer->death_count == 1)
		m_battleObjects[33]->Render(pd3dCommandList, pCamera);
	if (m_pPlayer->death_count == 2)
		m_battleObjects[34]->Render(pd3dCommandList, pCamera);
	if (m_pPlayer->death_count == 3)
		m_battleObjects[35]->Render(pd3dCommandList, pCamera);

	
	
	if (m_ppOtherPlayers[sample_id]->connected)
	{
		if (m_ppOtherPlayers[sample_id]->death_count == 1)
			m_battleObjects[30]->Render(pd3dCommandList, pCamera);
		if (m_pPlayer->death_count == 2)
			m_battleObjects[31]->Render(pd3dCommandList, pCamera);
		if (m_pPlayer->death_count == 3)
			m_battleObjects[32]->Render(pd3dCommandList, pCamera);
	}

	
	//hp
	
	for (int i = 0; i < 10; ++i)
	{
		
		if (m_pPlayer->hp >= i+1)
		{
			m_battleObjects[36+i]->Render(pd3dCommandList, pCamera);
		}
		
	}
	//sp
	for (int i = 0; i < 50; ++i)
	{

		if (m_pPlayer->sp >= i+1)
		{
			m_battleObjects[46 + i]->Render(pd3dCommandList, pCamera);
		}

	}
	



	for (int i = 0; i < 7; ++i)
	{
		if (i != 2)
		{
			m_battleObjects[21 + i]->Render(pd3dCommandList, pCamera);
		}
	}

	if (m_pPlayer->death_count == MAX_DEATH)
	{
		m_battleObjects[29]->Render(pd3dCommandList, pCamera);
	}
	
	if (m_ppOtherPlayers[sample_id]->connected)
	{
		if (m_ppOtherPlayers[sample_id]->death_count == MAX_DEATH)
		{
			m_battleObjects[28]->Render(pd3dCommandList, pCamera);
		}
	}
	
	
}

void BattleScene::BuildCube()
{
	
	if (cube_build) {
		for (unsigned int i = 0; i < m_nCubeObjects; ++i) {
			// ConsolePrint("UP Cube [%d] %.2f -  Pos : (%.2f, %.2f, %.2f) / Rot : (%.2f, %.2f, %.2f)\n", i, m_pFramework->cubeSize[i], m_pFramework->cubePos[i].x, m_pFramework->cubePos[i].y, m_pFramework->cubePos[i].z, m_pFramework->cubeRot[i].x, m_pFramework->cubeRot[i].y, m_pFramework->cubeRot[i].z);
			m_ppCubeObjects[i]->SetPosition(m_pFramework->cubePos[i]);
			//m_ppCubeObjects[i]->SetPosition(m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y, m_pPlayer->GetPosition().z);
			m_ppCubeObjects[i]->Rotate(m_pFramework->cubeRot[i].x, m_pFramework->cubeRot[i].y, m_pFramework->cubeRot[i].z);
		}
		cube_build = false;
	}
	if (slash_build)
	{
		for (unsigned int i = 0; i < Slash_end - Slash_start; ++i)
			if (slashWave && slashWave[i])
				slashWave[i]->SetPosition(10000, 10000, 10000);
		slash_build = false;
	}
	if (card_build)
	{
		for (unsigned int i = 0; i < Card_end - Card_start; ++i)
			if (card && card[i])
				card[i]->SetPosition(10000, 10000, 10000);
		card_build = false;
	}
	if (arrow_build)
	{
		for (unsigned int i = 0; i < Arrow_end - Arrow_start; ++i)
			if (arrow && arrow[i])
				arrow[i]->SetPosition(10000, 10000, 10000);
		arrow_build = false;
	}

	if(portal_build)
	{
		for (unsigned int i = 0; i < Potal_end - Potal_start; ++i)
			if (m_ppPotalObjects&& m_ppPotalObjects[i])
				m_ppPotalObjects[i]->SetPosition(m_pFramework->potalPos[i]);
		
		portal_build = false;
	}
}

void BattleScene::ProcessPacket(char * ptr)
{
	static bool first_time = true;
	
	switch (ptr[1])
	{
	case SC_Type::LoginOK:
	{
		ConsolePrint("login\n");
		/*SCPacket_LoginOK *packet = reinterpret_cast<SCPacket_LoginOK *>(ptr);
		m_pFramework->myid = packet->id;
		ConsolePrint("LOGIN\n");
		*/
		break;
	}
	case SC_Type::PutPlayer:
	{
		ConsolePrint("Put Player Packet Process\n");
		SCPacket_PutPlayer *my_packet = reinterpret_cast<SCPacket_PutPlayer *>(ptr);
		unsigned int id = my_packet->id;
		ConsolePrint("Put Player ID : %d\n", id);
		if (first_time) {
			first_time = false;
			m_pFramework->myid = id;
		}
	
		if (id == m_pFramework->myid) {

			m_pPlayer->SetVisible(true);
			m_pPlayer->character_type = my_packet->character_type;
			

			if (m_pPlayer->character_type == Character_type::Reaper)
			{
				m_pPlayer = ReaperObject[id];
				m_pFramework->m_pPlayer = m_pPlayer;
				m_pFramework->m_pCamera = m_pPlayer->GetCamera();
			}
			if (m_pPlayer->character_type == Character_type::Gamber)
			{
				m_pPlayer = GamblerObject[id];
				m_pFramework->m_pPlayer = m_pPlayer;
				m_pFramework->m_pCamera = m_pPlayer->GetCamera();
			}
			if (m_pPlayer->character_type == Character_type::Elf)
			{
				m_pPlayer = ElfObject[id];
				m_pFramework->m_pPlayer = m_pPlayer;
				m_pFramework->m_pCamera = m_pPlayer->GetCamera();
			}
			m_pPlayer->SetPosition((XMFLOAT3(my_packet->position.x, my_packet->position.y, my_packet->position.z)));
			m_pPlayer->hp = my_packet->hp;
			m_pPlayer->sp = my_packet->sp;
			m_pPlayer->death_count = 0;
		}
		else if (id < MAX_PLAYER) {
			if (m_ppOtherPlayers[id]) {
				m_ppOtherPlayers[id]->character_type = my_packet->character_type;			
				if (m_ppOtherPlayers[id]->character_type == Character_type::Reaper)
				{
					m_ppOtherPlayers[id] = ReaperObject[id];
					//m_pFramework->m_pCamera = m_ppOtherPlayers[id]->GetCamera();
					
				}
				if (m_ppOtherPlayers[id]->character_type == Character_type::Gamber)
				{
					m_ppOtherPlayers[id] = GamblerObject[id];
					
					
					//m_pFramework->m_pCamera = m_ppOtherPlayers[id]->GetCamera();
					
				}
				if (m_ppOtherPlayers[id]->character_type == Character_type::Elf)
				{
					m_ppOtherPlayers[id] = ElfObject[id];
					
					//m_pFramework->m_pCamera = m_ppOtherPlayers[id]->GetCamera();
				}
				m_ppOtherPlayers[id]->connected = true;
				m_ppOtherPlayers[id]->SetPosition((XMFLOAT3(my_packet->position.x, my_packet->position.y, my_packet->position.z)));
				m_ppOtherPlayers[id]->hp = my_packet->hp;
				m_ppOtherPlayers[id]->sp = my_packet->sp;
				m_ppOtherPlayers[id]->death_count = 0;

				
			}

		}
		
		break;
	}
	case SC_Type::Position:
	{
		SCPacket_Position *my_packet = reinterpret_cast<SCPacket_Position *>(ptr);
		unsigned short other_id = my_packet->id;
		anime = my_packet->animation_state;
		//ConsolePrint("%d", anime);
	//	ConsolePrint("포지션! 서버한테 받기 성공\n");
		if (other_id == m_pFramework->myid && m_pFramework->GetActivated()) {
			m_pPlayer->SetVisible(true);
			//ConsolePrint("Your [%d] : (%.1f, %.1f, %.1f)\n", my_packet->id, my_packet->position.x, my_packet->position.y, my_packet->position.z);
			m_pPlayer->SetPosition(my_packet->position);
		}
		else if (other_id < MAX_PLAYER) {
			//m_ppOtherPlayers[other_id]->m_pSkinnedAnimationController->SetAnimationSet(anime);
			m_ppOtherPlayers[other_id]->SetPosition(my_packet->position);
		}
		else if (other_id>=Card_start&&other_id<Card_end) {
			//ConsolePrint("도박사 평타 : %1.f, %1.f, %1.f\n", my_packet->position.x, my_packet->position.y, my_packet->position.z);
			// 그려주시고 위치 설정
			if (card && card[other_id - Card_start]) {
				card[other_id - Card_start]->SetPosition(my_packet->position);
				//ConsolePrint("도박사 평타 받은 후  : %1.f, %1.f, %1.f\n", card[other_id - Card_start]->GetPosition().x, card[other_id - Card_start]->GetPosition().y, card[other_id - Card_start]->GetPosition().z);
			}
		}
		else if (other_id >= Arrow_start&&other_id<Arrow_end) {
			//ConsolePrint("도박사 평타 : %1.f, %1.f, %1.f\n", my_packet->position.x, my_packet->position.y, my_packet->position.z);
			// 그려주시고 위치 설정
			if (arrow && arrow[other_id - Arrow_start]) {
				arrow[other_id - Arrow_start]->SetPosition(my_packet->position);
				//ConsolePrint("도박사 평타 받은 후  : %1.f, %1.f, %1.f\n", card[other_id - Card_start]->GetPosition().x, card[other_id - Card_start]->GetPosition().y, card[other_id - Card_start]->GetPosition().z);
			}
		}
		else if (other_id >= Slash_start&&other_id < Slash_end) {
			//ConsolePrint("검기 : %1.f, %1.f, %1.f\n", my_packet->position.x, my_packet->position.y, my_packet->position.z);
			// 그려주시고 위치 설정
			if (slashWave && slashWave[other_id - Slash_start]) {
				slashWave[other_id - Slash_start]->SetPosition(my_packet->position);
				/*ConsolePrint("Right : %f, %f, %f\n", slashWave[other_id - Slash_start]->GetRight().x, slashWave[other_id - Slash_start]->GetRight().y, slashWave[other_id - Slash_start]->GetRight().z);
				ConsolePrint("Up : %f, %f, %f\n", slashWave[other_id - Slash_start]->GetUp().x, slashWave[other_id - Slash_start]->GetUp().y, slashWave[other_id - Slash_start]->GetUp().z);
				ConsolePrint("Look : %f, %f, %f\n", slashWave[other_id - Slash_start]->GetLook().x, slashWave[other_id - Slash_start]->GetLook().y, slashWave[other_id - Slash_start]->GetLook().z);*/
				//slashWave[other_id - Slash_start]->MoveForward()
				//ConsolePrint("검기 받은 후 : %1.f, %1.f, %1.f\n", slashWave[other_id - Slash_start]->GetPosition().x, slashWave[other_id - Slash_start]->GetPosition().y, slashWave[other_id - Slash_start]->GetPosition().z);			
			}
		}
		break;
	}
	case SC_Type::Rotate:
	{
		SCPacket_Rotate *my_packet = reinterpret_cast<SCPacket_Rotate *>(ptr);
		unsigned short other_id = my_packet->id;
		obj_rot_x = my_packet->x;
		obj_rot_y = my_packet->y;
		obj_rot_z = my_packet->z;
		
		//ConsolePrint(" %f, %f, %f\n", my_packet->m_Look.x, my_packet->m_Look.y, my_packet->m_Look.z);
		if (other_id == m_pFramework->myid && m_pFramework->GetActivated()) {
			m_pPlayer->SetVisible(true);
			m_pPlayer->GetCamera()->Rotate(my_packet->y, my_packet->x, my_packet->z);
			m_pPlayer->SetRight(my_packet->m_Right);
			m_pPlayer->SetUp(my_packet->m_Up);
			m_pPlayer->SetLook(my_packet->m_Look);
			
			

			/*float a = Vector3::DotProduct(slashWave[0]->GetLook(), m_pPlayer->GetCamera()->GetLookVector());
			printf("%f\n", a);
			
			float b = sqrtf(1 - a*a);
			float c = a*a + b * b;*/
			//printf("%f\n", c);
			/*if (a < 1 && a>0)
			{
				slashWave[0]->m_xmf4x4ToParent._11 = a;
				slashWave[0]->m_xmf4x4ToParent._12 = 0;
				slashWave[0]->m_xmf4x4ToParent._13 = b;
				slashWave[0]->m_xmf4x4ToParent._21 = 0;
				slashWave[0]->m_xmf4x4ToParent._22 = 1;
				slashWave[0]->m_xmf4x4ToParent._23 = 0;
				slashWave[0]->m_xmf4x4ToParent._31 = m_pPlayer->GetCamera()->GetLookVector().x;
				slashWave[0]->m_xmf4x4ToParent._32 = m_pPlayer->GetCamera()->GetLookVector().y;
				slashWave[0]->m_xmf4x4ToParent._33 = m_pPlayer->GetCamera()->GetLookVector().z;
			}*/
			/*slashWave[0]->m_xmf4x4ToParent._11 = a;
			slashWave[0]->m_xmf4x4ToParent._12 = 0;
			slashWave[0]->m_xmf4x4ToParent._13 = -b;
			slashWave[0]->m_xmf4x4ToParent._21 = 0;
			slashWave[0]->m_xmf4x4ToParent._22 = 1;
			slashWave[0]->m_xmf4x4ToParent._23 = 0;
			slashWave[0]->m_xmf4x4ToParent._31 = b;
			slashWave[0]->m_xmf4x4ToParent._32 = 0;
			slashWave[0]->m_xmf4x4ToParent._33 = a;*/
			//slashWave[0]->Rotate(&slashWave[0]->GetUp(),obj_rot_y);
			//m_battleObjects[21]->SetLookAt(XMFLOAT3(0,0,0), my_packet->m_Up);
			//m_battleObjects[21]->Rotate(0,my_packet->x,0);
			//m_pPlayer->GetCamera()->SetLookAt(my_packet->m_Look);

			//m_pPlayer->SetLook(my_packet->m_Look);
			
			//ConsolePrint("Your [%d] : (%.1f, %.1f, %.1f)\n", my_packet->id, my_packet->position.x, my_packet->position.y, my_packet->position.z);
		//	m_pPlayer->GetCamera()->Rotate(my_packet->y, my_packet->x, my_packet->z);
			//m_pPlayer->Rotate(my_packet->y, my_packet->x, my_packet->z);
			
			/*slashWave[0]->m_xmf4x4World._11 = m_pPlayer->m_xmf4x4World._11; slashWave[0]->m_xmf4x4World._12 = m_pPlayer->m_xmf4x4World._12; slashWave[0]->m_xmf4x4World._13 = m_pPlayer->m_xmf4x4World._13;
			slashWave[0]->m_xmf4x4World._21 = m_pPlayer->m_xmf4x4World._21; slashWave[0]->m_xmf4x4World._22 = m_pPlayer->m_xmf4x4World._22; slashWave[0]->m_xmf4x4World._23 = m_pPlayer->m_xmf4x4World._23;
			slashWave[0]->m_xmf4x4World._31 = m_pPlayer->m_xmf4x4World._31; slashWave[0]->m_xmf4x4World._32 = m_pPlayer->m_xmf4x4World._32; slashWave[0]->m_xmf4x4World._33 = m_pPlayer->m_xmf4x4World._33;
			*/
		}
		else if (other_id < MAX_PLAYER) {
			m_ppOtherPlayers[other_id]->SetRight(my_packet->m_Right);
			m_ppOtherPlayers[other_id]->SetUp(my_packet->m_Up);
			m_ppOtherPlayers[other_id]->SetLook(my_packet->m_Look);
			
		}
		
		
		break;
	}
	case SC_Type::RemovePlayer:
	{
		ConsolePrint("Remove Player\n");
//		SCPacket_RemovePlayer *my_packet = reinterpret_cast<SCPacket_RemovePlayer *>(ptr);
//		unsigned int other_id = my_packet->id;
//		if (other_id == m_pFramework->myid) m_pPlayer->SetVisible(false);
//		if (other_id < MAX_USER) {
//			if (m_ppOtherPlayers[other_id]) {
//				//m_ppOtherPlayers[other_id]->SetVisible(false);
//				delete m_ppOtherPlayers[other_id];
//				m_ppOtherPlayers[other_id] = nullptr;
//				ConsolePrint("Player [%d] Remove from Screen\n", my_packet->id);
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
			if (m_ppOtherPlayers[other_id]->isCancleEnabled())
			{
				m_ppOtherPlayers[other_id]->animation_check = true;
				m_ppOtherPlayers[other_id]->m_pSkinnedAnimationController->SetAnimationSet(anime);
			}

		}

		break;
	}
	case SC_Type::MapInfo:
	{
		//처리
		
		SCPacket_MapInfo *my_packet = reinterpret_cast<SCPacket_MapInfo *>(ptr);

		unsigned short id = my_packet->id - Cube_start;
		m_pFramework->cubePos[id] = XMFLOAT3(my_packet->position.x, my_packet->position.y, my_packet->position.z);
		m_pFramework->cubeRot[id] = XMFLOAT3(my_packet->rotate.x, my_packet->rotate.y, my_packet->rotate.z);
		if(cube_build==false)
			cube_build = my_packet->build_cube;
		//ConsolePrint("%.1f, %.1f, %.1f\n", m_pFramework->cubePos[id].x, m_pFramework->cubePos[id].y, m_pFramework->cubePos[id].z);
		
		break;
	}
	case SC_Type::Potal:
	{
	
		SCPacket_PotalInfo *my_packet = reinterpret_cast<SCPacket_PotalInfo *>(ptr);
		unsigned short id = my_packet->id - Potal_start;
		m_pFramework->potalPos[id] = XMFLOAT3(my_packet->position.x, my_packet->position.y, my_packet->position.z);
		portal_build = my_packet->build_portal;
		break;
	}
	case SC_Type::ProjectTile:
	{
		SCPacket_ProjectTile *my_packet = reinterpret_cast<SCPacket_ProjectTile *>(ptr);
		if (my_packet->projectTile_type == ProjectTile::Card)
		{
			//ConsolePrint("card (%.1f, %.1f, %.1f)\n", my_packet->position.x, my_packet->position.y, my_packet->position.z);
			unsigned short card_id = my_packet->id - Card_start;
			m_pFramework->cardPos[card_id] = my_packet->position;
			my_packet->position = m_pPlayer[m_pFramework->myid].GetPosition();
			card_build = my_packet->build_projecttile;
		}
		if (my_packet->projectTile_type == ProjectTile::Slash)
		{
			//ConsolePrint("slash (%.1f, %.1f, %.1f)\n", my_packet->position.x, my_packet->position.y, my_packet->position.z);
			unsigned short slash_id = my_packet->id - Slash_start;
			m_pFramework->slashWavePos[slash_id] = my_packet->position;
			my_packet->position = m_pPlayer[m_pFramework->myid].GetPosition();
			slash_build = my_packet->build_projecttile;
		}
		if (my_packet->projectTile_type == ProjectTile::Arrow)
		{
			//ConsolePrint("slash (%.1f, %.1f, %.1f)\n", my_packet->position.x, my_packet->position.y, my_packet->position.z);
			unsigned short arrow_id = my_packet->id - Arrow_start;
			m_pFramework->arrowPos[arrow_id] = my_packet->position;
			my_packet->position = m_pPlayer[m_pFramework->myid].GetPosition();
			arrow_build = my_packet->build_projecttile;
		}
		break;
	}

	case SC_Type::OnHit:
	{
		SCPacket_Hit *my_packet = reinterpret_cast<SCPacket_Hit *>(ptr);
		unsigned short other_id = my_packet->id;
		if (other_id == m_pFramework->myid) {
			m_pPlayer->hp = my_packet->hp;
			//printf("맞음 %d\n",m_pPlayer->hp);
		}
		else if (other_id < MAX_PLAYER) {
			m_ppOtherPlayers[other_id]->hp = my_packet->hp;
			if (m_ppOtherPlayers[other_id]->character_type == Character_type::Gamber)
			{
				m_ppOtherPlayers[other_id]->m_pSkinnedAnimationController->SetAnimationSet(Gambler::OnHit);
			}
			if (m_ppOtherPlayers[other_id]->character_type == Character_type::Reaper)
			{
				m_ppOtherPlayers[other_id]->m_pSkinnedAnimationController->SetAnimationSet(GrimReaper::OnHit);
			}
			if (m_ppOtherPlayers[other_id]->character_type == Character_type::Elf)
			{
				m_ppOtherPlayers[other_id]->m_pSkinnedAnimationController->SetAnimationSet(ElfArcher::OnHit);
			}
		}
		break;
	}
	case SC_Type::Attack:
	{
		SCPacket_Attack *my_packet = reinterpret_cast<SCPacket_Attack *>(ptr);
		//m_pFramework->GetResource()->GetGrimReaperModel()->m_pModelRootObject->FindFrame("Cylinder002")->SetPosition(my_packet->position);
		break;
	}
	case SC_Type::GameTime:
	{
		SCPacket_GameTime *packet = reinterpret_cast<SCPacket_GameTime*>(ptr);
		//ConsolePrint("%d\n", packet->time);
		break;
	}
	case SC_Type::GameOver:
	{
		SCPacket_GameOver *my_packet = reinterpret_cast<SCPacket_GameOver*>(ptr);
		unsigned short other_id = my_packet->id;

		if (other_id == m_pFramework->myid) {
			m_pPlayer->death_count = my_packet->death_count;
			//printf("나\n");
		}
		else if (other_id < MAX_PLAYER) {
			sample_id = my_packet->id;
			m_ppOtherPlayers[other_id]->death_count = my_packet->death_count;
			//printf("너\n");
		}
		
		//ConsolePrint("%d\n", packet->time);
		break;
	}

	case SC_Type::CreateRoom:
	{
		SCPacket_CreateRoom *my_packet = reinterpret_cast<SCPacket_CreateRoom *>(ptr);
		break;
	}
	default:
		ConsolePrint("Unknown PACKET type [%d]\n", ptr[1]);
		break;
	}
	
}
