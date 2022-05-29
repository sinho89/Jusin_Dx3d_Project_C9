#ifndef MainApp_h__
#define MainApp_h__

#include "Defines.h"
#include "Base.h"

namespace Engine
{
	class CGraphicDev;
	class CManagement;
}

class CMainApp 
	: public CBase
{
private:
	explicit	CMainApp(void);
	virtual		~CMainApp(void);
public:
	HRESULT		Ready_MainApp(void);
	void		Update_MainApp(const _float& fTimeDelta);
	void		Render_MainApp(void);
private:
	Engine::CGraphicDev*			m_pGraphicInstance;
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	Engine::CManagement*			m_pManagementInstance;
	_ulong							m_dwRenderCnt;
	_tchar							m_szFPS[128];
	_float							m_fTime;
	_bool							m_bOneClick;

private:
	HRESULT		SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT		Ready_ResourceForStatic(LPDIRECT3DDEVICE9 pGraphicDev, RESOURCEID eMax);
	HRESULT		Ready_OpeningScene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagementInstance);
private:	//인풋 장치 소실 했을때 다시 찾는 함수.
	void		Set_Focus(void);
public:
	static CMainApp* Create(void);
private:
	virtual void Free(void);
};

#endif // MainApp_h__
