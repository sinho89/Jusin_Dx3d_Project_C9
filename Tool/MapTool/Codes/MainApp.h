#ifndef MainApp_h__
#define MainApp_h__

#include "Defines.h"
#include "Base.h"

namespace Engine
{
	class CGraphicDev;
//	class CTimeMgr;
	//class CResourceMgr;
//	class CGameObject;
	class CManagement;
}

class CMouse;
class CMouseCol;
//class CSkyBox;

class CMainApp
	: public CBase
{
private:
	explicit CMainApp(void);
	virtual ~CMainApp(void);

public://Getter
	HRESULT Get_GraphicDev(LPDIRECT3DDEVICE9* ppGraphicDev);

public:
	HRESULT Ready_MainApp(void);
	void Update_MainApp(const _float& fTimeDelta);
	void Render_MainApp(void);

private:
	Engine::CGraphicDev*			m_pGraphicInstance;
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	Engine::CManagement*			m_pManagementInstance;
	_ulong							m_dwRenderCnt;
	_tchar							m_szFPS[128];
	_float							m_fTime;

	_tchar							m_szCamEye[128];


private:
	HRESULT SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT Ready_ResourceForStatic(LPDIRECT3DDEVICE9 pGraphicDev, RESOURCEID eMax);
	HRESULT Ready_OpeningScene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagementInstance);

private:	//인풋 장치 소실 했을때 다시 찾는 함수.
	void Set_Focus(void);

public:
//	list<Engine::CGameObject*>	m_ListCamera;	//카메라
	//list<Engine::CGameObject*>  m_ListLandObj;
private:
	CMouse*						m_pMouse;
	CMouseCol*					m_pMouseCol;
	//CSkyBox*					m_pSkyBox;
	//bool						m_bDrag;
	//bool						m_bClick;
	//bool						m_bClickNavi;
	//bool						m_bRClick;
	//D3DXVECTOR3					m_vNaviPos[3];

public:
	static LPDIRECT3DDEVICE9	m_pDevice;

public:
//	static  TOOL_MODE			m_eToolMode;
//	static  NAVI_BUTTON			m_eNaviBtn;
//	static  NAVI_MODE			m_eNaviMode;

public:
	D3DXVECTOR3					m_vMousePos;

public: // Creator
	static CMainApp* Create(void);

private: // Destroyer
	virtual void Free(void);
};

#endif