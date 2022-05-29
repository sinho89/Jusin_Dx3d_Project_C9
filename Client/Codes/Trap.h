#ifndef Trap_h__
#define Trap_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CDynamicMesh;
	class CShader;
}


class CTrap
	:public Engine::CGameObject
{

protected:
	explicit					CTrap(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual						~CTrap(void);

public:
	virtual HRESULT				Start_ForScene(void);
	virtual void				Update_Object(const _float& fTimeDelta);
	virtual void				Render_Object(void);
	virtual	void				Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

public:
	Engine::CTransform*			GetTransCom(void);

public:
	void						SetObjectInfo(_vec3* vPos, _vec3* vScale, _vec3* vAngle);
	OBJ_INFO*					GetObjectInfo(void);

protected:
	Engine::CTransform*			m_pTransCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CShader*			m_pShaderCom;
	Engine::CDynamicMesh*		m_pMeshCom;

	Engine::CCalculator*		m_pCalculatorCom;
	Engine::CRigidbody*			m_pRigidbodyCom;

	Engine::CCollider*			m_pSphereColCom;	//몬스터 타격.

protected:
	Engine::CTransform*			m_pTargetTransCom;

protected:
	Engine::CStaticMesh**		m_ppMapMesh;

private:
	_float						m_fFrame;
	_long						m_lMouseMove;
	_float						m_fAlpha;

private:
	_bool						m_bCheck;

protected:
	virtual HRESULT				Ready_Object(void);
	HRESULT						Add_Component(void);
	HRESULT						Set_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT						Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect);
	HRESULT						Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect);

public:
	static CTrap*				Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);
protected:
	virtual void				Free(void);
};


#endif // Environment_h__
