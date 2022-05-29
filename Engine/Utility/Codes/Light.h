#ifndef Light_h__
#define Light_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CScreen_Texture;
class CRenderer;
class ENGINE_DLL CLight 
	: public CBase
{
private:
	explicit			CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual				~CLight(void);
public:
	void				Set_LightPos(const _vec3* pPos);
	void				Set_LightDiffuse(const D3DXCOLOR& tDiffuse);
	void				Set_LightRange(const _float& fRange);
public:
	D3DLIGHT9*			Get_LightInfo();

	const _matrix		Get_matLightView(void) const;
	void				Set_matLightView(const _matrix matView);
	const _matrix		Get_matLightProj(void) const;
	void				Set_matLightProj(const _matrix matProj);

public:
	//void				Render(LPD3DXEFFECT	pEffect);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	//CScreen_Texture*	m_pScreenTexture;
	CRenderer*			m_pRenderer;

private:
	D3DLIGHT9			m_LightInfo;
	_uint				m_iIndex;

	_matrix				m_matLightView;
	_matrix				m_matLightProj;

	_bool				m_bLightRender;

public:
	HRESULT				Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex);

	const _bool			GetLightRender(void) const { return m_bLightRender; }
	void				SetLightRender(const _bool bLightRender) { m_bLightRender = bLightRender; }

public:
	static CLight*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);

private:
	virtual void		Free(void);
};

END

#endif // Light_h__
