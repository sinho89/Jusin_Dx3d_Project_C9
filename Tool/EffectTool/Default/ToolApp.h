#ifndef ToolApp_h__
#define ToolApp_h__

#include "Defines.h"
#include "Base.h"

namespace Engine
{
	class CGraphicDev;
	class CManagement;
}


class CToolApp
	: public CBase
{
private:
	explicit CToolApp(void);
	virtual ~CToolApp(void);

public:
	HRESULT Ready_ToolApp(void);
	void	Update_ToolApp(const _float& fTimeDelta);
	void	Render_ToolApp(void);
public://Getter
	HRESULT Get_GraphicDev(LPDIRECT3DDEVICE9* ppGraphicDev);

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

private:	//인풋 장치 소실 했을때 다시 찾는 함수.
	void Set_Focus(void);

public:
	static CToolApp*		Create(void);

private:
	virtual void Free(void);
};


#endif // ToolApp_h__
