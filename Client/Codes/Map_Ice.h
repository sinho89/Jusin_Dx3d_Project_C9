#ifndef Map_Ice_h__
#define Map_Ice_h__

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
class CSound;
class CMap_Ice
	:public Engine::CGameObject
{
private:
	explicit CMap_Ice(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CMap_Ice(void);
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
	Engine::CStaticMesh*		m_pMeshCom[69];
	Engine::CNavigationMesh*	m_pNaviCom;

	CSound*						m_pSoundCom;

	Engine::CCamera*			m_pLightCam;
	Engine::CCamera*			m_pCurCam;		

	_float						m_fAlpha;

private://SnowBall°ü·Ã..
	_bool						m_bSnowBall;
	_float						m_fSnowBallTime;
	_vec3						m_vSnowBallStart;
	_vec3						m_vSnowBallEnd;

private:
	Engine::CTransform*			m_pTargetTransCom;

private:
	HRESULT Ready_Object(void);
	HRESULT Add_Component(void);
	HRESULT	Set_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT	Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect);
	HRESULT Set_Minimap_ConstanTable(LPD3DXEFFECT pEffect);
public:
	static CMap_Ice* Create(LPDIRECT3DDEVICE9 pGraphicDev
		,const wstring& wstrName);
private:
	virtual void Free(void);
};
#endif // MapObj_h__
