#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev
	: public CBase
{
public:
	enum WINMODE {MODE_FULL, MODE_WIN};
	DECLARE_SINGLETON(CGraphicDev)
private:
	explicit CGraphicDev(void);
	virtual ~CGraphicDev(void);
public:
	LPDIRECT3DDEVICE9 Get_GraphicDev(void) const;
	HRESULT Get_GraphicDev(LPDIRECT3DDEVICE9* ppGraphicDev);
	void Set_RenderState(D3DRENDERSTATETYPE eType, _ulong dwFlag);

public:
	HRESULT Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, Engine::CGraphicDev** ppGraphicDev);
	void Render_Begin(D3DXCOLOR Color);
	void Render_End(void);
private:
	LPDIRECT3D9						m_pSDK;
	LPDIRECT3DDEVICE9				m_pGraphicDev;
public:
	inline virtual void Free(void);

};

END

#endif // GraphicDev_h__
