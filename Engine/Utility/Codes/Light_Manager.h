#ifndef Light_Manager_h__
#define Light_Manager_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CLight;
class CScreen_Texture;
class ENGINE_DLL CLight_Manager 
	: public CBase
{
public:
	DECLARE_SINGLETON(CLight_Manager)

private:
	explicit								CLight_Manager(void);
	virtual									~CLight_Manager(void);

public:
//	D3DLIGHT9*								Get_LightInfo(const _uint& iIndex = 0);
	CLight*									Get_PointLight(const _uint& iIndex = 0);

public:
	map<const _uint, CLight*>*				Get_PointLightMap(void);
	map<const _uint, CLight*>*				Get_SpotLightMap(void);

public:
	HRESULT									Ready_ScreenBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT									Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);
	void									Render_Light(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXEFFECT pEffect);

	void									Render_Directional_Light(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXEFFECT pEffect, const _uint& iIndex);
	void									Render_Point_Light(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXEFFECT pEffect, const _uint& iIndex);
	void									Render_Spot_Light(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXEFFECT pEffect, const _uint& iIndex);

private:
	void									Directional_Update(CLight* pLight, LPD3DXEFFECT pEffect);
	void									Point_Update(LPDIRECT3DDEVICE9 pGraphicDev, CLight* pLight, LPD3DXEFFECT pEffect);
	void									Spot_Update(CLight* pLight, LPD3DXEFFECT pEffect);

public:
	void									LightMap_Clear(void);
	CLight*									Find_Light(const _uint sLightTag, const _short LightType);
	D3DLIGHT9*								Find_LightInfo(const _uint sLightTag, const _short LightType);

	void									Add_Light(CLight* pLight, const _short LightType);

private:
	/*list<CLight*>			m_LightList;
	typedef	list<CLight*>	LIGHTLIST;*/

	typedef map<const _uint, CLight*>		MAPLIGHT;
	MAPLIGHT								m_mapDirectionalLight;
	MAPLIGHT								m_mapPointLight;
	MAPLIGHT								m_mapSpotLight;

	CScreen_Texture*					m_pDirectionalScreen;
	CScreen_Texture*					m_pPointScreen;
	CScreen_Texture*					m_pSpotScreen;

private:
	virtual void							Free(void);
};


END

#endif // Light_Manager_h__
