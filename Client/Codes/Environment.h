#ifndef Environment_h__
#define Environment_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CStaticMesh;
	class CDynamicMesh;
	class CResource_Manager;
	class CShader;
	class CCollider;
	class CAnimator;
}

class CMouseCol;

class CEnvironment
	:public Engine::CGameObject
{

private:
	explicit CEnvironment(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CEnvironment(void);
public:
	virtual HRESULT	Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

public:
	void						Stop(void);
	void						Complete(void);
	Engine::CTransform*			GetTransCom(void);
	OBJECT_MODE			GetMode(void);
	

public:
	void		SetMode(OBJECT_MODE eMode);
	void		SetObjectInfo(_vec3* vPos, _vec3* vScale, _vec3* vAngle);
	_bool		GetChecked(void);
	void		CheckTrue(void);
	void		CheckFalse(void);
	OBJ_INFO*	GetObjectInfo(void);

private:
	Engine::CResource_Manager* m_pResourceMgr;
private:
	Engine::CTransform*		m_pTransCom;
	Engine::CRenderer*		m_pRendererCom;
	Engine::CShader*		m_pShaderCom;
	Engine::CStaticMesh*	m_pMeshCom;
	Engine::CCollider*		m_pColliderCom;
	OBJ_INFO				m_tInfo;

	Engine::CAnimator*		m_pTargetAnimator;
	Engine::CDynamicMesh*	m_pTargetMesh;

private:
	_bool					m_bStop;
	_bool					m_bComplete;
	_long					m_lMouseMove;
	_float					m_fAlpha;

private:
	_bool					m_bCheck;

	_bool					m_bBardielObject;


private:
	HRESULT Ready_Object(void);
	HRESULT Add_Component(void);
	HRESULT	Set_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect);
	HRESULT Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect);

private:
	void    SettingWork(void);
public:
	static CEnvironment* Create(LPDIRECT3DDEVICE9 pGraphicDev
		,const wstring& wstrName);
private:
	virtual void Free(void);
};


#endif // Environment_h__
