#ifndef Map_Fire_h__
#define Map_Fire_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CStaticMesh;
	class CShader;
	class CNavigationMesh;
	class CCamera;
}

//class CMouseCol;

class CMap_Fire
	:public Engine::CGameObject
{
private:
	explicit CMap_Fire(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CMap_Fire(void);
public:
	Engine::CStaticMesh**	GetMapMesh(_int* iSize);
public:
	virtual HRESULT	Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	Engine::CTransform*			m_pTransCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CShader*			m_pShaderCom;
	Engine::CStaticMesh*		m_pMeshCom[33];
	Engine::CNavigationMesh*	m_pNaviCom;

	Engine::CCamera*			m_pLightCam;
	Engine::CCamera*			m_pCurCam;		

	_float						m_fAlpha;

private:
	HRESULT Ready_Object(void);
	HRESULT Add_Component(void);
	HRESULT	Set_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT	Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect);
	HRESULT Set_Minimap_ConstanTable(LPD3DXEFFECT pEffect);
public:
	static CMap_Fire* Create(LPDIRECT3DDEVICE9 pGraphicDev
		,const wstring& wstrName);
private:
	virtual void Free(void);
};
#endif // MapObj_h__
