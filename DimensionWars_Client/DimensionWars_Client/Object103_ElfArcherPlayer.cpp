#include "stdafx.h"
#include "Object103_ElfArcherPlayer.h"
#include "Camera002_ThirdPersonCamera.h"
#include "AnimationController.h"
#include "RuntimeFrameWork.h"
#include "ResourceManager.h"
#include "Object008_HeightmapTerrain.h"


ElfArcherPlayer::ElfArcherPlayer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext, RuntimeFrameWork * pFramework, unsigned int index)
{
	m_pFramework = pFramework;
	m_pCamera = ChangeCamera(THIRD_PERSON_CAMERA, 0.0f);

	SetChild(m_pFramework->GetResource()->GetElfArcherModel(index)->m_pModelRootObject, true);
	
	m_pSkinnedAnimationController = new AnimationController(pd3dDevice, pd3dCommandList, 1, m_pFramework->GetResource()->GetElfArcherModel(index));
	m_pSkinnedAnimationController->SetTrackAnimationSet(0, 0);
	m_pSkinnedAnimationController->AddAnimationSet(0.0f, 40.0f * keyFrameUnit, "Idle");
	m_pSkinnedAnimationController->AddAnimationSet(42.0f * keyFrameUnit, 72.0f * keyFrameUnit, "OnHit", ANIMATION_TYPE_ONCE);
	m_pSkinnedAnimationController->AddAnimationSet(74.0f * keyFrameUnit, 114.0f * keyFrameUnit, "Guard", ANIMATION_TYPE_ONCE);
	m_pSkinnedAnimationController->AddAnimationSet(116.0f * keyFrameUnit, 156.0f * keyFrameUnit, "Burf", ANIMATION_TYPE_ONCE);
	m_pSkinnedAnimationController->AddAnimationSet(158.0f * keyFrameUnit, 176.0f * keyFrameUnit, "First Shot", ANIMATION_TYPE_ONCE);
	m_pSkinnedAnimationController->AddAnimationSet(176.0f * keyFrameUnit, 203.0f * keyFrameUnit, "Second Shot", ANIMATION_TYPE_ONCE);
	m_pSkinnedAnimationController->AddAnimationSet(203.0f * keyFrameUnit, 263.0f * keyFrameUnit, "Third Shot", ANIMATION_TYPE_ONCE);
	m_pSkinnedAnimationController->AddAnimationSet(265.0f * keyFrameUnit, 295.0f * keyFrameUnit, "Guide Shot", ANIMATION_TYPE_ONCE);
	m_pSkinnedAnimationController->AddAnimationSet(297.0f * keyFrameUnit, 367.0f * keyFrameUnit, "Snipe", ANIMATION_TYPE_ONCE);
	m_pSkinnedAnimationController->AddAnimationSet(369.0f * keyFrameUnit, 396.0f * keyFrameUnit, "Jump", ANIMATION_TYPE_ONCE);
	m_pSkinnedAnimationController->AddAnimationSet(398.0f * keyFrameUnit, 430.0f * keyFrameUnit, "Jump Attack", ANIMATION_TYPE_ONCE);
	m_pSkinnedAnimationController->AddAnimationSet(432.0f * keyFrameUnit, 452.0f * keyFrameUnit, "Fall");
	m_pSkinnedAnimationController->AddAnimationSet(454.0f * keyFrameUnit, 484.0f * keyFrameUnit, "Fall Attack", ANIMATION_TYPE_ONCE);
	m_pSkinnedAnimationController->AddAnimationSet(486.0f * keyFrameUnit, 516.0f * keyFrameUnit, "Move Forward");
	m_pSkinnedAnimationController->AddAnimationSet(518.0f * keyFrameUnit, 548.0f * keyFrameUnit, "Move Right Forward");
	m_pSkinnedAnimationController->AddAnimationSet(550.0f * keyFrameUnit, 580.0f * keyFrameUnit, "Move Left Forward");
	m_pSkinnedAnimationController->AddAnimationSet(582.0f * keyFrameUnit, 612.0f * keyFrameUnit, "Move Right");
	m_pSkinnedAnimationController->AddAnimationSet(614.0f * keyFrameUnit, 644.0f * keyFrameUnit, "Move Left");	
	m_pSkinnedAnimationController->AddAnimationSet(646.0f * keyFrameUnit, 676.0f * keyFrameUnit, "Move Backward");
	m_pSkinnedAnimationController->AddAnimationSet(678.0f * keyFrameUnit, 708.0f * keyFrameUnit, "Move Right Backward");
	m_pSkinnedAnimationController->AddAnimationSet(710.0f * keyFrameUnit, 740.0f * keyFrameUnit, "Move Left Backward");
	m_pSkinnedAnimationController->AddAnimationSet(742.0f * keyFrameUnit, 772.0f * keyFrameUnit, "Down", ANIMATION_TYPE_ONCE);
	m_pSkinnedAnimationController->SetAnimationSet(Idle);
	/*
	m_pSkinnedAnimationController->SetCallbackKeys(0, 0);
	#ifdef _WITH_SOUND_RESOURCE
	m_pSkinnedAnimationController->SetCallbackKey(1, 0, 0.1f, _T("Footstep01"));
	m_pSkinnedAnimationController->SetCallbackKey(1, 1, 0.5f, _T("Footstep02"));
	m_pSkinnedAnimationController->SetCallbackKey(1, 2, 0.9f, _T("Footstep03"));
	#else
	m_pSkinnedAnimationController->SetCallbackKey(1, 0, 0.1f, _T("Sound/Footstep01.wav"));
	m_pSkinnedAnimationController->SetCallbackKey(1, 1, 0.5f, _T("Sound/Footstep02.wav"));
	m_pSkinnedAnimationController->SetCallbackKey(1, 2, 0.9f, _T("Sound/Footstep03.wav"));
	#endif
	CAnimationCallbackHandler *pAnimationCallbackHandler = new CSoundCallbackHandler();
	m_pSkinnedAnimationController->SetAnimationCallbackHandler(1, pAnimationCallbackHandler);
	*/
	SetPlayerUpdatedContext(pContext);
	SetCameraUpdatedContext(pContext);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));

	SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));

	SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
}


ElfArcherPlayer::~ElfArcherPlayer()
{
}

void ElfArcherPlayer::OnPrepareRender()
{
	BasePlayer::OnPrepareRender();

	m_xmf4x4ToParent = Matrix4x4::Multiply(XMMatrixScaling(m_xmf3Scale.x, m_xmf3Scale.y, m_xmf3Scale.z), m_xmf4x4ToParent);
	m_xmf4x4ToParent = Matrix4x4::Multiply(XMMatrixRotationX(-1.57f), m_xmf4x4ToParent);
}

BaseCamera * ElfArcherPlayer::ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode) return(m_pCamera);
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		SetFriction(250.0f);
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(FIRST_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 20.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	case THIRD_PERSON_CAMERA:
		SetFriction(1000.0f);
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.25f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 150.0f, -350.0f));
		m_pCamera->SetPosition(Vector3::Add(m_xmf3Position, m_pCamera->GetOffset()));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	default:
		break;
	}
	Update(fTimeElapsed);

	return(m_pCamera);
}

void ElfArcherPlayer::Update(float fTimeElapsed)
{
	BasePlayer::Update(fTimeElapsed);
}

void ElfArcherPlayer::OnPlayerUpdateCallback(float fTimeElapsed)
{
	HeightMapTerrain *pTerrain = (HeightMapTerrain *)m_pPlayerUpdatedContext;
	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	XMFLOAT3 xmf3PlayerPosition = GetPosition();
	int z = (int)(xmf3PlayerPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(xmf3PlayerPosition.x, xmf3PlayerPosition.z, bReverseQuad) + 0.0f;
	if (xmf3PlayerPosition.y < fHeight)
	{
		XMFLOAT3 xmf3PlayerVelocity = GetVelocity();
		xmf3PlayerVelocity.y = 0.0f;
		SetVelocity(xmf3PlayerVelocity);
		xmf3PlayerPosition.y = fHeight;
		SetPosition(xmf3PlayerPosition);
	}
}

void ElfArcherPlayer::OnCameraUpdateCallback(float fTimeElapsed)
{
	HeightMapTerrain *pTerrain = (HeightMapTerrain *)m_pCameraUpdatedContext;
	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	XMFLOAT3 xmf3CameraPosition = m_pCamera->GetPosition();
	int z = (int)(xmf3CameraPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(xmf3CameraPosition.x, xmf3CameraPosition.z, bReverseQuad) + 15.0f;
	if (xmf3CameraPosition.y <= fHeight)
	{
		xmf3CameraPosition.y = fHeight;
		m_pCamera->SetPosition(xmf3CameraPosition);
		if (m_pCamera->GetMode() == THIRD_PERSON_CAMERA)
		{
			ThirdPersonCamera *p3rdPersonCamera = (ThirdPersonCamera *)m_pCamera;
			p3rdPersonCamera->SetLookAt(GetPosition());
		}

		XMFLOAT3 xmf3CameraPosition = m_pCamera->GetPosition();

		ThirdPersonCamera *p3rdPersonCamera = (ThirdPersonCamera *)m_pCamera;
		p3rdPersonCamera->SetLookAt(GetPosition());
	}
	/*
	XMFLOAT3 xmf3CameraPosition = m_pCamera->GetPosition();

	ThirdPersonCamera *p3rdPersonCamera = (ThirdPersonCamera *)m_pCamera;
	p3rdPersonCamera->SetLookAt(GetPosition());*/
}

void ElfArcherPlayer::ProcessInput(UCHAR * pKeysBuffer, float fTimeElapsed)
{
	char direction = 0;
	if (SecondShotTrigger) {
		if (m_pSkinnedAnimationController->m_pAnimationSets->GetAnimationSet(First_Shot)->m_bEndTrigger)// 1타 모션이 끝나기 전까지는 2타 시행 안함
			m_pSkinnedAnimationController->SetAnimationSet(state = Second_Shot);
		if (state == Second_Shot) // 1타 모션이 끝나서 2타 모션이 들어왔는지 확인해야 2타 트리거 종료
			SecondShotTrigger = false;
	}
	else if (ThirdShotTrigger) {
		if (m_pSkinnedAnimationController->m_pAnimationSets->GetAnimationSet(Second_Shot)->m_bEndTrigger)// 2타 모션이 끝나기 전까지는 3타 시행 안함
			m_pSkinnedAnimationController->SetAnimationSet(state = Third_Shot);
		if (state == Third_Shot) // 2타 모션이 끝나서 3타 모션이 들어왔는지 확인해야 2타 트리거 종료
			ThirdShotTrigger = false;
	}
	else if (isCancleEnabled()) {
		animation_check = true;
		if (pKeysBuffer[VK_SPACE] & 0xF0) {
			direction |= DIR_UP;
		}
		if (pKeysBuffer['f'] & 0xF0 || pKeysBuffer['F'] & 0xF0) {
			direction |= DIR_DOWN;
		}

		if (pKeysBuffer['w'] & 0xF0 || pKeysBuffer['W'] & 0xF0) {
			direction |= DIR_FORWARD;
			m_pSkinnedAnimationController->SetAnimationSet(state = Move_Forward);
		}
		if (pKeysBuffer['s'] & 0xF0 || pKeysBuffer['S'] & 0xF0) {
			direction |= DIR_BACKWARD;
			m_pSkinnedAnimationController->SetAnimationSet(state = Move_Backward);
		}
		if (pKeysBuffer['a'] & 0xF0 || pKeysBuffer['A'] & 0xF0) {
			direction |= DIR_LEFT;
			m_pSkinnedAnimationController->SetAnimationSet(state = Move_Left);
		}
		if (pKeysBuffer['d'] & 0xF0 || pKeysBuffer['D'] & 0xF0) {
			direction |= DIR_RIGHT;
			m_pSkinnedAnimationController->SetAnimationSet(state = Move_Right);
		}

		if ((direction & DIR_FORWARD) && (direction & DIR_LEFT))
			m_pSkinnedAnimationController->SetAnimationSet(state = Move_Left_Forward);
		if ((direction & DIR_FORWARD) && (direction & DIR_RIGHT))
			m_pSkinnedAnimationController->SetAnimationSet(state = Move_Right_Forward);
		if ((direction & DIR_BACKWARD) && (direction & DIR_LEFT))
			m_pSkinnedAnimationController->SetAnimationSet(state = Move_Left_Backward);
		if ((direction & DIR_BACKWARD) && (direction & DIR_RIGHT))
			m_pSkinnedAnimationController->SetAnimationSet(state = Move_Right_Backward);

		if ((direction & DIR_LEFT) && (direction & DIR_RIGHT)) {
			if ((direction & DIR_FORWARD) && (direction & DIR_BACKWARD))
				m_pSkinnedAnimationController->SetAnimationSet(state = Idle);
			else if (direction & DIR_FORWARD)
				m_pSkinnedAnimationController->SetAnimationSet(state = Move_Forward);
			else if (direction & DIR_BACKWARD)
				m_pSkinnedAnimationController->SetAnimationSet(state = Move_Backward);
			else
				m_pSkinnedAnimationController->SetAnimationSet(state = Idle);

		}
		if ((direction & DIR_FORWARD) && (direction & DIR_BACKWARD)) {
			if (direction & DIR_LEFT)
				m_pSkinnedAnimationController->SetAnimationSet(state = Move_Left);
			else if (direction & DIR_RIGHT)
				m_pSkinnedAnimationController->SetAnimationSet(state = Move_Right);
			else
				m_pSkinnedAnimationController->SetAnimationSet(state = Idle);
		}
		if (!direction)
			m_pSkinnedAnimationController->SetAnimationSet(state = Idle);
	}

	float cxDelta = 0.0f, cyDelta = 0.0f;
	POINT ptCursorPos;
	//if (GetCapture() == m_pFramework->GetHandle())
	if (m_pFramework->GetActivated()) {
		SetCursor(NULL);
		GetCursorPos(&ptCursorPos);
		cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
		cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
		// SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
		m_ptOldCursorPos = ptCursorPos;
	}

	if ((direction != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f)) {
		if (cxDelta || cyDelta)
		{
			//if (pKeysBuffer[VK_RBUTTON] & 0xF0) Rotate(cyDelta, 0.0f, -cxDelta);
			// else
			//Rotate(cyDelta, cxDelta, 0.0f);
			SendRotate(cyDelta, cxDelta, 0.0f);
		}
		//if (dwDirection) Move(dwDirection, 30.0f * fTimeElapsed, true);
	}
	SetDirectionBit(direction);
	Update(fTimeElapsed);
}

bool ElfArcherPlayer::isCancleEnabled()
{
	if (m_pSkinnedAnimationController->m_pAnimationSets->GetAnimationSet(state)->m_bEndTrigger &&
		m_pSkinnedAnimationController->m_pAnimationSets->GetAnimationSet(state)->m_nType != ANIMATION_TYPE_LOOP) {
		m_pSkinnedAnimationController->m_pAnimationSets->GetAnimationSet(state)->m_bEndTrigger = false;
		return true;
	}
	switch (state) {
	case Idle:
	case Move_Forward:
	case Move_Left_Forward:
	case Move_Right_Forward:
	case Move_Left:
	case Move_Right:
	case Move_Backward:
	case Move_Left_Backward:
	case Move_Right_Backward:
		return true;
	default:	// 위 상태가 아니면 현재 모션을 캔슬 할 수 없다.
		return false;
	}
}

bool ElfArcherPlayer::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		animation_check = true;
		SendArrow();
		switch (state) {
		case First_Shot:
			SecondShotTrigger = true;
			break;
		case Second_Shot:
			ThirdShotTrigger = true;
			break;
		default:
			if (isCancleEnabled()) {
				m_pSkinnedAnimationController->SetAnimationSet(state = First_Shot);
			}
			break;
		}
		::SetCapture(hWnd);
		::GetCursorPos(&m_ptOldCursorPos);
		break;
	case WM_RBUTTONDOWN:
		animation_check = true;
		if (isCancleEnabled() && state != Guard) {
			m_pSkinnedAnimationController->SetAnimationSet(state = Guard);
		}
		::SetCapture(hWnd);
		::GetCursorPos(&m_ptOldCursorPos);
		break;
	case WM_LBUTTONUP:
		animation_check = false;
		::ReleaseCapture();
		break;
	case WM_RBUTTONUP:
		animation_check = false;
		::ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
	return false;
}

bool ElfArcherPlayer::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		// 키보드를 누르고 있을 경우 최초 한번만 실행.
		if ((lParam & 0x40000000) != 0x40000000) {
			switch (wParam) {
			case '1':
				animation_check = true;
				switch (state) {
				case First_Shot:
					SecondShotTrigger = true;
					break;
				case Second_Shot:
					ThirdShotTrigger = true;
					break;
				default:
					if (isCancleEnabled())
						m_pSkinnedAnimationController->SetAnimationSet(state = First_Shot);
					break;
				}
				break;
			case '2':
				if (isCancleEnabled()) {
					m_pSkinnedAnimationController->SetAnimationSet(state = Guide_Shot);
				}
				break;
			case '3':
				if (isCancleEnabled()) {
					animation_check = true;
					m_pSkinnedAnimationController->SetAnimationSet(state = Third_Shot);
				}
				break;
			case 'q': case 'Q':
				if (isCancleEnabled()) {
					animation_check = true;
					m_pSkinnedAnimationController->SetAnimationSet(state = Burf);
				}
				break;
			case 'e': case 'E':
				if (isCancleEnabled()) {
					animation_check = true;
					m_pSkinnedAnimationController->SetAnimationSet(state = Snipe);
				}
				break;

			case VK_SPACE:
				if (isCancleEnabled()) {
					animation_check = true;
					m_pSkinnedAnimationController->SetAnimationSet(state = Jump);
				}
				break;

			case '0':
				ResetDir();
				break;
			}

		}
		break;
	case WM_KEYUP:
		animation_check = false;

		break;
	}
	return false;
}


void ElfArcherPlayer::SendArrow()
{
	CSPacket_Attack *myPacket = reinterpret_cast<CSPacket_Attack*>(m_pFramework->GetSendBuf());
	myPacket->size = sizeof(CSPacket_Attack);
	myPacket->type = CS_Type::Attack;
	myPacket->attack_type = ElfArcher::First_Shot;
	myPacket->position = GetPosition();
	m_pFramework->SendPacket(reinterpret_cast<char *>(myPacket));
	//m_pFramework->GetResource()->GetGrimReaperModel()->m_pModelRootObject->FindFrame("Cylinder002");
}