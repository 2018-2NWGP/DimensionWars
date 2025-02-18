#pragma once
#include "Scene000_BaseScene.h"
#include "Object008_HeightmapTerrain.h"
constexpr unsigned short n_texture = 14;
class LobbyScene :
	public BaseScene
{
public:
	LobbyScene();
	LobbyScene(SceneTag tag, RuntimeFrameWork* pFramework)
	{
		m_Tag = tag;
		m_pFramework = pFramework;
	}
	virtual ~LobbyScene();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseObjects();
	virtual void ReleaseUploadBuffers();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual bool ProcessInput(UCHAR *pKeysBuffer, float fTimeElapsed = 0.0f);
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, BaseCamera *pCamera = nullptr);
	virtual void ProcessPacket(char *ptr) override;
	void SendRoomCreate();
	void SendEnterRoom();
	void SendLobby_RoomChange(unsigned short room_n);
private:
	
	bool left_active = false; 
	bool right_active = false;
	bool room_enter = false;
	HeightMapTerrain * m_pTerrain = nullptr;
	
};

