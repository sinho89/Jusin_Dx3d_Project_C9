#ifndef AnimationApp_h__
#define AnimationApp_h__

#include "Defines.h"
#include "Base.h"

namespace Engine
{
	class CGraphicDev;
	class CManagement;
}

class CAnimationApp
	: public CBase
{
private:
	explicit CAnimationApp(void);
	virtual ~CAnimationApp(void);
public://Getter
	HRESULT Get_GraphicDev(LPDIRECT3DDEVICE9* ppGraphicDev);
public:
	HRESULT Ready_AnimationApp(void);
	void Update_AnimationApp(const _float& fTimeDelta);
	void Render_AnimationApp(void);
private:
	Engine::CGraphicDev*			m_pGraphicInstance;
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	Engine::CManagement*			m_pManagementInstance;
	_ulong							m_dwRenderCnt;
	_tchar							m_szFPS[128];
	_float							m_fTime;

private:
	HRESULT SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT Ready_ResourceForStatic(LPDIRECT3DDEVICE9 pGraphicDev, RESOURCEID eMax);
	HRESULT Ready_OpeningScene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagementInstance);
public:
	static CAnimationApp* Create(void);
private:
	virtual void Free(void);
};

#endif // AnimationApp_h__
